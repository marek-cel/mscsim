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

#include <fdm/models/fdm_Blade.h>

#include <fdm/main/fdm_Aerodynamics.h>

#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Matrix3x3 Blade::getRAS2AAS( double psi )
{
    double tempPsi = -psi + M_PI / 2.0;

    Matrix3x3 result;

//    result(0,0) = -1.0;
//    result(0,1) =  0.0;
//    result(0,2) =  0.0;

//    result(1,0) =  0.0;
//    result(1,1) =  1.0;
//    result(1,2) =  0.0;

//    result(2,0) =  0.0;
//    result(2,1) =  0.0;
//    result(2,2) = -1.0;

    double cosPsi = cos( tempPsi );
    double sinPsi = sin( tempPsi );

    result(0,0) = -cosPsi;
    result(0,1) = -sinPsi;
    result(0,2) =  0.0;

    result(1,0) = -sinPsi;
    result(1,1) =  cosPsi;
    result(1,2) =  0.0;

    result(2,0) =  0.0;
    result(2,1) =  0.0;
    result(2,2) = -1.0;

    return result;
}

////////////////////////////////////////////////////////////////////////////////

Matrix3x3 Blade::getAAS2BAS( double beta )
{
    Matrix3x3 result;

    double cosBeta = cos( beta );
    double sinBeta = sin( beta );

    result = Matrix3x3::createIdentityMatrix();

//    result(0,0) =  1.0;
//    result(0,1) =  0.0;
//    result(0,2) =  0.0;

//    result(1,0) =  0.0;
//    result(1,1) =  cosBeta;
//    result(1,2) =  sinBeta;

//    result(2,0) =  0.0;
//    result(2,1) = -sinBeta;
//    result(2,2) =  cosBeta;

//    result(0,0) =  1.0;
//    result(0,1) =  0.0;
//    result(0,2) =  0.0;

//    result(1,0) =  0.0;
//    result(1,1) =  cosBeta;
//    result(1,2) = -sinBeta;

//    result(2,0) =  0.0;
//    result(2,1) =  sinBeta;
//    result(2,2) =  cosBeta;

    return result;
}

////////////////////////////////////////////////////////////////////////////////

Matrix3x3 Blade::getRAS2BAS( double psi, double beta )
{
    return getRAS2AAS( psi ) * getAAS2BAS( beta );
}

////////////////////////////////////////////////////////////////////////////////

Blade::Blade() :
    _b ( 0.0 ),
    _c ( 0.0 ),
    _e ( 0.0 ),
    _m ( 0.0 ),

    _beta_min ( 0.0 ),
    _beta_max ( 0.0 ),

    _twist_0 ( 0.0 ),
    _twist_rate ( 0.0 ),

    _sb ( 0.0 ),
    _ib ( 0.0 ),

    _thrust ( 0.0 ),
    _hforce ( 0.0 ),
    _yforce ( 0.0 ),
    _torque ( 0.0 ),
    _moment ( 0.0 ),

    _beta_0 ( 0.0 ),
    _beta_1 ( 0.0 ),
    _beta_2 ( 0.0 ),

    _beta ( _beta_0 ),

    _theta ( 0.0 )
{
    _cd = Table::createOneRecordTable( 0.0 );
    _cl = Table::createOneRecordTable( 0.0 );

    _ras2aas = Matrix3x3::createIdentityMatrix();
    _aas2ras = Matrix3x3::createIdentityMatrix();
    _aas2bas = Matrix3x3::createIdentityMatrix();
    _bas2aas = Matrix3x3::createIdentityMatrix();
    _ras2bas = Matrix3x3::createIdentityMatrix();
    _bas2ras = Matrix3x3::createIdentityMatrix();
}

////////////////////////////////////////////////////////////////////////////////

Blade::~Blade() {}

////////////////////////////////////////////////////////////////////////////////

void Blade::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _m , "blade_mass"   );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _b , "blade_span" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _c , "blade_chord"  );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _e , "hinge_offset" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _beta_min, "beta_min" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _beta_max, "beta_max" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _twist_0    , "twist_0" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _twist_rate , "twist_rate" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _cd, "cd" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _cl, "cl" );

        if ( result == FDM_SUCCESS )
        {
            _r_fh_aas = Vector3( 0.0, _e, 0.0 );

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

void Blade::computeForceAndMoment( const Vector3 &vel_air_ras,
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
    update( vel_air_ras,
            omg_air_ras,
            omg_ras,
            acc_ras,
            eps_ras,
            grav_ras,
            omega,
            azimuth,
            airDensity,
            theta_0,
            theta_1c,
            theta_1s );
}

////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fdm/utils/fdm_Units.h>
void Blade::integrate( double timeStep,
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
    update( vel_air_ras,
            omg_air_ras,
            omg_ras,
            acc_ras,
            eps_ras,
            grav_ras,
            omega,
            azimuth,
            airDensity,
            theta_0,
            theta_1c,
            theta_1s );

    double beta_0_prev = _beta_0;
    double beta_1_prev = _beta_1;

    double moment = _moment;

    if ( 0 )
    {
        // limiting flapping angle
        if ( _beta_0 < _beta_min || _beta_0 > _beta_max )
        {
            double delta = 0.0;

            if ( _beta_0 < _beta_min )
                delta = _beta_0 - _beta_min;
            else
                delta = _beta_0 - _beta_max;

            moment -= delta * fabs( delta ) * 1.0e7;
            moment -= _beta_1 * 1.0e4;
        }
    }

    // flaping angle time 2nd derivative
    _beta_2 = moment / _ib;

    // flapping angle and flaping angle time derivative Euler integration
    _beta_1 += _beta_2 * timeStep;
    _beta_0 += _beta_1 * timeStep;

    // limiting flapping angle
    _beta_0 = Misc::satur( _beta_min, _beta_max, _beta_0 );

    //std::cout << "beta= " << Units::rad2deg( _beta_0 ) << std::endl;

    // back calculating flaping angle time derivatives
    if ( timeStep >= FDM_TIME_STEP_MIN )
    {
        _beta_1 = ( _beta_0 - beta_0_prev ) / timeStep;
        _beta_2 = ( _beta_1 - beta_1_prev ) / timeStep;
    }
}

////////////////////////////////////////////////////////////////////////////////

void Blade::update( const Vector3 &vel_air_ras,
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
    // zeroize results
    _thrust = 0.0;
    _hforce = 0.0;
    _yforce = 0.0;
    _torque = 0.0;
    _moment = 0.0;

    // transform matrices
    _ras2aas = getRAS2AAS( azimuth );
    _aas2ras = _ras2aas.getTransposed();

    _aas2bas = getAAS2BAS( _beta );
    _bas2aas = _aas2bas.getTransposed();

    _ras2bas = _ras2aas * _aas2bas;
    _bas2ras = _ras2bas.getTransposed();

    // feathering angle
    double cosPsi = cos( azimuth );
    double sinPsi = sin( azimuth );

    _theta = theta_0 + theta_1c * cosPsi + theta_1s * sinPsi;

    // convert parameters
    Vector3 vel_air_aas = _ras2aas * vel_air_ras;
    Vector3 omg_air_aas = _ras2aas * omg_air_ras;
    Vector3 omg_aas     = _ras2aas * omg_ras;
    Vector3 acc_aas     = _ras2aas * acc_ras;
    Vector3 eps_aas     = _ras2aas * eps_ras;
    Vector3 grav_aas    = _ras2aas * grav_ras;

    // rotor angular velocity expressed in BAAS
    // Notice
    Vector3 omg_r_aas( 0.0, 0.0, omega );

    // integration along blade span
    const int steps_y = 20;

    const double coef = 1.0 / (double)( steps_y );
    const double dy = _b * coef;
    const double dm = _m * coef;

    for ( int i = 0; i < steps_y; i++ )
    {
        // center of element coordiantes
        double y = ( i + 0.5 ) * dy;
        Vector3 r_fh_aas = _bas2aas * Vector3( 0.0, y, 0.0 );
        Vector3 r_rh_aas = _r_fh_aas + r_fh_aas;
        double r = r_rh_aas.getLength();

        //std::cout << r_rh_aas.toString() << std::endl;

        // flapping motion
        Vector3 omg_f_aas( _beta_1, 0.0, 0.0 );

        // acceleration of element
        Vector3 a_aas = acc_aas
                      + ( omg_aas % ( omg_aas % r_rh_aas ) )
                      + ( eps_aas % r_rh_aas )
                      // rotor rotation centrifugal
                      + ( omg_r_aas % ( omg_r_aas % r_rh_aas ) )
                      // flapping motion centrifugal
                      + ( omg_f_aas % ( omg_f_aas % r_fh_aas ) )
                      // acceleration due to gravity
                      - grav_aas;
        //std::cout << a_aas.toString() << std::endl;
        //std::cout << ( omg_r_aas % ( omg_r_aas % r_rh_aas ) ).toString() << std::endl;

        // velocity (relative to airflow)
        Vector3 v_air_aas = vel_air_aas
                          + ( omg_air_aas % r_rh_aas )
                          + ( omg_r_aas   % r_rh_aas )
                          + ( omg_f_aas   % r_fh_aas );
        Vector3 v_air_bas = _aas2bas * v_air_aas;

        // section angle of attack
        double uv = -v_air_bas.x();
        double w  = -v_air_bas.z();
        double angleOfAttack = Aerodynamics::getAngleOfAttack( uv, w ) + _theta;
        //std::cout << Units::rad2deg( angleOfAttack ) << std::endl;

        // dynamic pressure
        double dynPress = 0.5 * airDensity * v_air_bas.getLength2();

        // elementary forces
        double dD = dynPress * _cd.getValue( angleOfAttack ) * _c;
        double dL = dynPress * _cl.getValue( angleOfAttack ) * _c;

        double sinAlpha = sin( angleOfAttack );
        double cosAlpha = cos( angleOfAttack );

        double dX = cosAlpha * dD - sinAlpha * dL;
        double dZ = sinAlpha * dD + cosAlpha * dL;

        Vector3 d_for_bfas( dX, 0.0, dZ );
        Vector3 d_for_ras = _bas2ras * d_for_bfas;

        double dQ = r * dX;
        double dM = y * dZ;

        _hforce += dy * d_for_ras.x() * -1.0;
        _yforce += dy * d_for_ras.y();
        _thrust += dy * d_for_ras.z() * -1.0;
        _torque += dy * dQ;
        _moment += dy * dM - ( r_fh_aas % ( dm * a_aas ) ).x();

//        std::cout << ( dm * a_aas ).toString();
//        std::cout << " ; ";
//        std::cout << ( r_fh_aas % ( dm * a_aas ) ).toString();
//        std::cout << std::endl;
    }
}
