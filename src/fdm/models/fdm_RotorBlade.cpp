/****************************************************************************//*
 * Copyright (C) 2020 Marek M. Cel
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 ******************************************************************************/

#include <fdm/models/fdm_RotorBlade.h>

#include <iostream>

#include <fdm/fdm_Log.h>
#include <fdm/main/fdm_Aerodynamics.h>
#include <fdm/utils/fdm_Units.h>
#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Matrix3x3 RotorBlade::getRAS2SRA( double psi, bool ccw )
{
    double ccw_coef = ccw ? 1.0 : -1.0;

    Matrix3x3 a;

    a(0,0) =  0.0;
    a(0,1) = -ccw;
    a(0,2) =  0.0;

    a(1,0) = -ccw;
    a(1,1) =  0.0;
    a(1,2) =  0.0;

    a(2,0) =  0.0;
    a(2,1) =  0.0;
    a(2,2) = -1.0;

    double cosPsi = cos( ccw_coef * psi );
    double sinPsi = sin( ccw_coef * psi );

    Matrix3x3 b;

    b(0,0) =  cosPsi;
    b(0,1) = -sinPsi;
    b(0,2) =  0.0;

    b(1,0) =  sinPsi;
    b(1,1) =  cosPsi;
    b(1,2) =  0.0;

    b(2,0) =  0.0;
    b(2,1) =  0.0;
    b(2,2) =  1.0;

    return a * b;
}

////////////////////////////////////////////////////////////////////////////////

Matrix3x3 RotorBlade::getSRA2BSA( double beta, bool ccw )
{
    double ccw_coef = ccw ? 1.0 : -1.0;

    double cosBeta = cos( ccw_coef * beta );
    double sinBeta = sin( ccw_coef * beta );

    Matrix3x3 result;

    result(0,0) =  1.0;
    result(0,1) =  0.0;
    result(0,2) =  0.0;

    result(1,0) =  0.0;
    result(1,1) =  cosBeta;
    result(1,2) =  sinBeta;

    result(2,0) =  0.0;
    result(2,1) = -sinBeta;
    result(2,2) =  cosBeta;

    return result;
}

////////////////////////////////////////////////////////////////////////////////

RotorBlade::RotorBlade( bool ccw ) :
    _ccw ( ccw ),
    _dirFactor ( _ccw ? 1.0 : -1.0 ),

    _m ( 0.0 ),
    _b ( 0.0 ),
    _c ( 0.0 ),
    _e ( 0.0 ),

    _beta_min ( 0.0 ),
    _beta_max ( 0.0 ),

    _sb ( 0.0 ),
    _ib ( 0.0 ),

    _xforce ( 0.0 ),
    _yforce ( 0.0 ),
    _zforce ( 0.0 ),
    _torque ( 0.0 ),
    _moment ( 0.0 ),

    _beta     ( _stateVect( 0 ) ),
    _beta_dot ( _stateVect( 1 ) ),

    _theta ( 0.0 )
{
    _cd = Table1::createOneRecordTable( 0.0 );
    _cl = Table1::createOneRecordTable( 0.0 );

    _twist = Table1::createOneRecordTable( 0.0 );

    _ras2sra = Matrix3x3::createIdentityMatrix();
    _sra2ras = Matrix3x3::createIdentityMatrix();
    //_sra2bsa = Matrix3x3::createIdentityMatrix();
    //_bsa2sra = Matrix3x3::createIdentityMatrix();
}

////////////////////////////////////////////////////////////////////////////////

RotorBlade::~RotorBlade() {}

////////////////////////////////////////////////////////////////////////////////

void RotorBlade::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _m , "blade_mass"   );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _b , "blade_length" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _c , "blade_chord"  );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _e , "hinge_offset" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _beta_min, "beta_min" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _beta_max, "beta_max" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _twist, "twist" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _cd, "cd" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _cl, "cl" );

        if ( result == FDM_SUCCESS )
        {
            _pos_fh_sra = Vector3( 0.0, _dirFactor * _e, 0.0 );

            _sb = _m * _b / 2.0;
            _ib = _m * _b * _b / 3.0;
        }
        else
        {
            XmlUtils::throwError( __FILE__, __LINE__, dataNode );
        }
    }
    else
    {
        XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
}

////////////////////////////////////////////////////////////////////////////////

#ifdef SIM_ROTOR_TEST
void RotorBlade::TEST_INIT()
{
    //_beta_0 = _beta_max;
    _beta = _beta_min;

    //Log::out() << _beta_0 << std::endl;
}
#endif

////////////////////////////////////////////////////////////////////////////////

void RotorBlade::computeForceAndMoment( const Vector3 &vel_air_ras,
                                        const Vector3 &omg_air_ras,
                                        const Vector3 &omg_ras,
                                        const Vector3 &acc_ras,
                                        const Vector3 &eps_ras,
                                        const Vector3 &grav_ras,
                                        double omega,
                                        double azimuth,
                                        double airDensity,
                                        double theta_0,
                                        double theta_1c,
                                        double theta_1s )
{
    _ras2sra = getRAS2SRA( azimuth, _ccw );
    _sra2ras = _ras2sra.getTransposed();

    //_sra2bsa = getSRA2BSA( _beta, _direction );
    //_bsa2sra = _sra2bsa.getTransposed();

    _theta = getTheta( azimuth, theta_0, theta_1c, theta_1s );

    integrateSpanwise( vel_air_ras,
                       omg_air_ras,
                       omg_ras,
                       acc_ras,
                       eps_ras,
                       grav_ras,
                       omega,
                       airDensity,
                       _beta,
                       _beta_dot );

    _for_ras = _sra2ras * Vector3( _xforce, _yforce, _zforce );
    _mom_ras = Vector3( 0.0, 0.0, _dirFactor * _torque );
}

////////////////////////////////////////////////////////////////////////////////

void RotorBlade::integrate( double timeStep,
                            const Vector3 &vel_air_ras,
                            const Vector3 &omg_air_ras,
                            const Vector3 &omg_ras,
                            const Vector3 &acc_ras,
                            const Vector3 &eps_ras,
                            const Vector3 &grav_ras,
                            double omega,
                            double azimuth,
                            double airDensity,
                            double theta_0,
                            double theta_1c,
                            double theta_1s )
{
    _ras2sra = getRAS2SRA( azimuth, _ccw );
    _sra2ras = _ras2sra.getTransposed();

    _theta = getTheta( azimuth, theta_0, theta_1c, theta_1s );

    double beta_prev = _beta;

//    integrateEulerRect( timeStep,
//                        vel_air_ras,
//                        omg_air_ras,
//                        omg_ras,
//                        acc_ras,
//                        eps_ras,
//                        grav_ras,
//                        omega,
//                        airDensity );

    integrateRungeKutta4( timeStep,
                          vel_air_ras,
                          omg_air_ras,
                          omg_ras,
                          acc_ras,
                          eps_ras,
                          grav_ras,
                          omega,
                          airDensity );

    // limiting flapping angle
    if ( Misc::isOutside( _beta_min, _beta_max, _beta ) )
    {
        //_beta_0 = Misc::satur( _beta_min, _beta_max, _beta_0 );

        double beta_new = Misc::satur( _beta_min, _beta_max, _beta );
        double beta_tc = 0.01;
        _beta = Misc::inertia( beta_new, _beta, timeStep, beta_tc );

        // back calculating flaping angle time derivatives
        if ( timeStep >= FDM_TIME_STEP_MIN )
        {
            _beta_dot = ( _beta - beta_prev ) / timeStep;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void RotorBlade::computeStateDeriv( const StateVector &stateVect,
                                    StateVector *derivVect )
{
    (*derivVect)( 0 ) = stateVect( 1 );
    (*derivVect)( 1 ) = _moment / _ib;
}

////////////////////////////////////////////////////////////////////////////////

double RotorBlade::getTheta( double azimuth,
                             double theta_0,
                             double theta_1c,
                             double theta_1s )
{
    double cosPsi = cos( azimuth );
    double sinPsi = sin( azimuth );

    return theta_0 + theta_1c * cosPsi + theta_1s * sinPsi;
}

////////////////////////////////////////////////////////////////////////////////

void RotorBlade::integrateEulerRect( double timeStep,
                                     const Vector3 &vel_air_ras,
                                     const Vector3 &omg_air_ras,
                                     const Vector3 &omg_ras,
                                     const Vector3 &acc_ras,
                                     const Vector3 &eps_ras,
                                     const Vector3 &grav_ras,
                                     double omega,
                                     double airDensity )
{
    integrateSpanwise( vel_air_ras,
                       omg_air_ras,
                       omg_ras,
                       acc_ras,
                       eps_ras,
                       grav_ras,
                       omega,
                       airDensity,
                       _beta,
                       _beta_dot );

    computeStateDeriv( _stateVect, &_derivVect );

    _stateVect = _stateVect + _derivVect * timeStep;
}

////////////////////////////////////////////////////////////////////////////////

void RotorBlade::integrateRungeKutta4( double timeStep,
                                       const Vector3 &vel_air_ras,
                                       const Vector3 &omg_air_ras,
                                       const Vector3 &omg_ras,
                                       const Vector3 &acc_ras,
                                       const Vector3 &eps_ras,
                                       const Vector3 &grav_ras,
                                       double omega,
                                       double airDensity )
{
    StateVector xt = _stateVect;

    // k1 - derivatives calculation
    StateVector k1;
    integrateSpanwise( vel_air_ras,
                       omg_air_ras,
                       omg_ras,
                       acc_ras,
                       eps_ras,
                       grav_ras,
                       omega,
                       airDensity,
                       xt( 0 ),
                       xt( 1 ) );
    computeStateDeriv( xt, &k1 );

    xt = _stateVect + k1 * ( timeStep / 2.0 );

    // k2 - derivatives calculation
    StateVector k2;
    integrateSpanwise( vel_air_ras,
                       omg_air_ras,
                       omg_ras,
                       acc_ras,
                       eps_ras,
                       grav_ras,
                       omega,
                       airDensity,
                       xt( 0 ),
                       xt( 1 ) );
    computeStateDeriv( xt, &k2 );

    xt = _stateVect + k2 * ( timeStep / 2.0 );

    // k3 - derivatives calculation
    StateVector k3;
    integrateSpanwise( vel_air_ras,
                       omg_air_ras,
                       omg_ras,
                       acc_ras,
                       eps_ras,
                       grav_ras,
                       omega,
                       airDensity,
                       xt( 0 ),
                       xt( 1 ) );
    computeStateDeriv( xt, &k3 );

    xt = _stateVect + k3 * timeStep;

    // k4 - derivatives calculation
    StateVector k4;
    integrateSpanwise( vel_air_ras,
                       omg_air_ras,
                       omg_ras,
                       acc_ras,
                       eps_ras,
                       grav_ras,
                       omega,
                       airDensity,
                       xt( 0 ),
                       xt( 1 ) );
    computeStateDeriv( xt, &k4 );

    // integration
    _stateVect = _stateVect + ( k1 + k2 * 2.0 + k3 * 2.0 + k4 ) * ( timeStep / 6.0 );
}

////////////////////////////////////////////////////////////////////////////////

void RotorBlade::integrateSpanwise( const Vector3 &vel_air_ras,
                                    const Vector3 &omg_air_ras,
                                    const Vector3 &omg_ras,
                                    const Vector3 &acc_ras,
                                    const Vector3 &eps_ras,
                                    const Vector3 &grav_ras,
                                    double omega,
                                    double airDensity,
                                    double beta,
                                    double beta_dot )
{
    Matrix3x3 sra2bsa = getSRA2BSA( beta, _ccw );
    Matrix3x3 bsa2sra = sra2bsa.getTransposed();

    // velocity relative to airflow
    Vector3 vel_air_sra = _ras2sra * vel_air_ras;
    Vector3 omg_air_sra = _ras2sra * omg_air_ras;

    // angular velocity due to rotor rotation
    Vector3 omega_r_ras( 0.0, 0.0, -_dirFactor * omega );
    Vector3 omega_r_sra = _ras2sra * omega_r_ras;

    // angular velocity due to blade flapping
    Vector3 omega_f_bsa( _dirFactor * beta_dot, 0.0, 0.0 );
    Vector3 omega_f_sra = bsa2sra * omega_f_bsa;

    // total angular velocity
    // omega_f_sra not included
    // this velocity is used to determine inertial forces
    Vector3 omg_tot_sra
            = _ras2sra * omg_ras
            + omega_r_sra
            ;
    Vector3 omg_tot_bsa = sra2bsa * omg_tot_sra;

    // total angular velocity relative to airflow
    // omega_f_sra included
    // this velocity is used to determine section airflow
    Vector3 omg_air_tot_sra
            = omg_air_sra
            + omega_r_sra
            + omega_f_sra
            ;
    Vector3 omg_air_tot_bsa = sra2bsa * omg_air_tot_sra;

    // linear velocity of flapping hinge relative to airflow
    Vector3 vel_fh_air_bsa = sra2bsa * ( vel_air_sra + omg_air_tot_sra % _pos_fh_sra );

    // accelerations
    Vector3 eps_bsa = sra2bsa * ( _ras2sra * eps_ras );
    Vector3 acc_bsa
            = sra2bsa * ( _ras2sra * acc_ras )
            + ( omg_tot_bsa % ( omg_tot_bsa % _pos_fh_sra ) ) // centrifugal force
            + ( eps_bsa % _pos_fh_sra )                       // Euler force
            ;

    // gravity acceleration
    Vector3 grav_bsa = sra2bsa * ( _ras2sra * grav_ras );

    const int steps = 10;

    double dy = _b / (double)(steps);
    double dm = _m / (double)(steps);

    _xforce = 0.0;
    _yforce = 0.0;
    _zforce = 0.0;
    _torque = 0.0;
    _moment = 0.0;

    for ( int i = 0; i < steps; i++ )
    {
        double y = ( i + 0.5 ) * dy;

        Vector3 pos_i_bsa( 0.0, _dirFactor * y, 0.0 );
        //Vector3 pos_i_sra = _pos_fh_sra + _bsa2sra * pos_i_bsa;

        // moment due to gravity
        Vector3 mom_grav_bsa = dm * ( pos_i_bsa % grav_bsa );

        // moment due to inertia
        Vector3 acc_i_bsa
                = acc_bsa
                + ( omg_tot_bsa % ( omg_tot_bsa % pos_i_bsa ) ) // centrifugal acceleration
                + ( eps_bsa % pos_i_bsa )                       // Euler acceleration
                ;
        Vector3 mom_iner_bsa = -dm * ( pos_i_bsa % acc_i_bsa );

        // moment due to
        //Vector3 for_cf_sra = -dm * ( omg_tot_sra % ( omg_tot_sra % pos_i_sra ) );
        //Vector3 for_cf_bsa = _sra2bsa * for_cf_sra;
        //Vector3 mom_cf_bsa = pos_i_bsa % for_cf_bsa;

        // velocity (relative to airflow)
        Vector3 vel_i_air_bsa = vel_fh_air_bsa + omg_air_tot_bsa % pos_i_bsa;

        // section angle of attack
        double u = -vel_i_air_bsa.u();
        double w = -vel_i_air_bsa.w();
        double angleOfAttack = Aerodynamics::getAngleOfAttack( u, w );
        double angleOfAttackTot = angleOfAttack;
        if ( fabs( u ) > 0.1 )
        {
            angleOfAttackTot += _theta + _twist.getValue( y );
            angleOfAttackTot = Angles::normalize( angleOfAttackTot, -M_PI );
        }

        // dynamic pressure
        double dynPress = 0.5 * airDensity * vel_i_air_bsa.getLength2();

        // elementary forces
        double dD = dynPress * _cd.getValue( angleOfAttackTot ) * _c;
        double dL = dynPress * _cl.getValue( angleOfAttackTot ) * _c;

        double sinAlpha = sin( angleOfAttack );
        double cosAlpha = cos( angleOfAttack );

        double dX = cosAlpha * dD - sinAlpha * dL;
        double dZ = sinAlpha * dD + cosAlpha * dL;

        Vector3 for_aero_bsa( dX, 0.0, dZ );
        Vector3 for_aero_sra = bsa2sra * for_aero_bsa;
        Vector3 mom_aero_bsa = pos_i_bsa % for_aero_sra;
        Vector3 mom_aero_sra = bsa2sra * mom_aero_bsa;

#       ifdef SIM_ROTOR_TEST
        {
            Vector3 pos_i_sra = _pos_fh_sra + bsa2sra * pos_i_bsa;

            int i1 = 3 * i;
            int i2 = i1 + 1;
            int i3 = i2 + 1;

            span[ i1 ].visible = true;
            span[ i2 ].visible = true;
            span[ i3 ].visible = true;

            span[ i1 ].b_sra = pos_i_sra;
            span[ i2 ].b_sra = pos_i_sra;
            span[ i3 ].b_sra = pos_i_sra;

            span[ i1 ].v_sra = bsa2sra * Vector3(  dX, 0.0, 0.0 );
            span[ i2 ].v_sra = bsa2sra * Vector3( 0.0, 0.0,  dZ );
            span[ i3 ].v_sra = bsa2sra * vel_i_air_bsa;
        }
#       endif

        // aerodynamic forces
        _xforce += for_aero_sra.x();
        _yforce += for_aero_sra.y();
        _zforce += for_aero_sra.z();

        _torque += _dirFactor * mom_aero_sra.z();

        // total moment about flapping hinge
        Vector3 mom_tot_bsa
                = mom_grav_bsa
                + mom_iner_bsa
                + mom_aero_bsa
                ;

        _moment += _dirFactor * mom_tot_bsa.x();
    }
}
