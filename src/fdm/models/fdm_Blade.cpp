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

#include <iostream>

#include <fdm/fdm_Log.h>
#include <fdm/main/fdm_Aerodynamics.h>
#include <fdm/utils/fdm_Units.h>
#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Matrix3x3 Blade::getRAS2SRA( double psi, Direction direction )
{
    double ccw = ( direction == MainRotor::CCW ) ? 1.0 : -1.0;

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

    double cosPsi = cos( ccw * psi );
    double sinPsi = sin( ccw * psi );

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

Matrix3x3 Blade::getSRA2BSA( double beta, Direction direction )
{
    double ccw = ( direction == MainRotor::CCW ) ? 1.0 : -1.0;

    double cosBeta = cos( ccw * beta );
    double sinBeta = sin( ccw * beta );

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

Blade::Blade( Direction direction ) :
    _direction ( direction ),

    _dirFactor ( _direction == MainRotor::CCW ? 1.0 : -1.0 ),

    _r ( 0.0 ),
    _c ( 0.0 ),
    _e ( 0.0 ),
    _m ( 0.0 ),

    _beta_min ( 0.0 ),
    _beta_max ( 0.0 ),

    _b ( 0.0 ),

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

    _twist = Table::createOneRecordTable( 0.0 );

    _ras2sra = Matrix3x3::createIdentityMatrix();
    _sra2ras = Matrix3x3::createIdentityMatrix();
    _sra2bsa = Matrix3x3::createIdentityMatrix();
    _bsa2sra = Matrix3x3::createIdentityMatrix();
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
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _r , "rotor_radius" );
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

            _b = _r - _e;

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

void Blade::TEST_INIT()
{
    Log::i() << "REMOVE ME" << std::endl;

    _beta_0 = _beta_max;

    Log::out() << _beta_0 << std::endl;
}

////////////////////////////////////////////////////////////////////////////////

void Blade::update( double timeStep,
                    const Vector3 &vel_air_ras,
                    const Vector3 &omg_air_ras,
                    const Vector3 &omg_ras,
                    const Vector3 &grav_ras,
                    double omega,
                    double azimuth,
                    double airDensity,
                    double theta_0,
                    double theta_1c,
                    double theta_1s )
{
    _ras2sra = getRAS2SRA( azimuth, _direction );
    _sra2ras = _ras2sra.getTransposed();

    _sra2bsa = getSRA2BSA( _beta, _direction );
    _bsa2sra = _sra2bsa.getTransposed();

    // feathering angle
    double cosPsi = cos( azimuth );
    double sinPsi = sin( azimuth );

    _theta = theta_0 + theta_1c * cosPsi + theta_1s * sinPsi;

    xxx( vel_air_ras,
         omg_air_ras,
         omg_ras,
         grav_ras,
         omega,
         airDensity );

    double beta_0_prev = _beta_0;
    double beta_1_prev = _beta_1;

    // flaping angle time 2nd derivative
    _beta_2 = _moment / _ib;

    // flapping angle and flaping angle time derivative Euler integration
    _beta_1 += _beta_2 * timeStep;
    _beta_0 += _beta_1 * timeStep;

    // limiting flapping angle
    if ( !Misc::inside( _beta_min, _beta_max, _beta_0 ) )
    {
        //_beta_0 = Misc::satur( _beta_min, _beta_max, _beta_0 );

        double beta_new = Misc::satur( _beta_min, _beta_max, _beta_0 );
        double beta_tc = 0.01;
        _beta_0 = Misc::inertia( beta_new, _beta_0, timeStep, beta_tc );

        // back calculating flaping angle time derivatives
        if ( timeStep >= FDM_TIME_STEP_MIN )
        {
            _beta_1 = ( _beta_0 - beta_0_prev ) / timeStep;
            _beta_2 = ( _beta_1 - beta_1_prev ) / timeStep;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Blade::xxx( const Vector3 &vel_air_ras,
                 const Vector3 &omg_air_ras,
                 const Vector3 &omg_ras,
                 const Vector3 &grav_ras,
                 double omega,
                 double airDensity )
{
    // velocity relative to airflow
    Vector3 vel_air_sra = _ras2sra * vel_air_ras;
    Vector3 omg_air_sra = _ras2sra * omg_air_ras;

    // angular velocity due to rotor rotation
    Vector3 omega_r_ras( 0.0, 0.0, -_dirFactor * omega );
    Vector3 omega_r_sra = _ras2sra * omega_r_ras;

    // angular velocity due to blade flapping
    Vector3 omega_f_bsa( _dirFactor * _beta_1, 0.0, 0.0 );
    Vector3 omega_f_sra = _bsa2sra * omega_f_bsa;

    // total angular velocity
    Vector3 omg_tot_sra
            = _ras2sra * omg_ras
            + omega_r_sra
            + omega_f_sra   // ????????
            ;

    // total angular velocity relative to airflow
    Vector3 omg_air_tot_sra
            = omg_air_sra
            + omega_r_sra
            + omega_f_sra
            ;
    Vector3 omg_air_tot_bsa = _sra2bsa * omg_air_tot_sra;

    // linear velocity of flapping hinge relative to airflow
    Vector3 vel_fh_air_bsa = _sra2bsa * ( vel_air_sra + omg_air_tot_sra % _pos_fh_sra );

    // gravity acceleration
    Vector3 grav_sra = _ras2sra * grav_ras;
    Vector3 grav_bsa = _sra2bsa * grav_sra;

    const int steps = 10;

    double dy = _b / (double)(steps);
    double dm = _m / (double)(steps);

    // moment about flapping hinge
    _moment = 0.0;

    for ( int i = 0; i < steps; i++ )
    {
        double y = ( i + 0.5 ) * dy;

        Vector3 pos_i_bsa( 0.0, _dirFactor * y, 0.0 );
        Vector3 pos_i_sra = _pos_fh_sra = _bsa2sra * pos_i_bsa;

        // moment due to gravity
        Vector3 mom_grav_bsa = dm * ( pos_i_bsa % grav_bsa );

        // moment due to centrifugal force
        Vector3 for_cf_sra = -dm * ( omg_tot_sra % ( omg_tot_sra % pos_i_sra ) );
        Vector3 for_cf_bsa = _sra2bsa * for_cf_sra;
        Vector3 mom_cf_bsa = pos_i_bsa % for_cf_bsa;

        // velocity (relative to airflow)
        Vector3 vel_i_air_bsa = vel_fh_air_bsa + omg_air_tot_bsa % pos_i_bsa;

        // section angle of attack
        double uv = -vel_i_air_bsa.x();
        double w  = -vel_i_air_bsa.z();
        double angleOfAttack = Aerodynamics::getAngleOfAttack( uv, w );
        double angleOfAttackTheta = angleOfAttack + _theta;

        // dynamic pressure
        double dynPress = 0.5 * airDensity * vel_i_air_bsa.getLength2();

        // elementary forces
        double dD = dynPress * _cd.getValue( angleOfAttackTheta ) * _c;
        double dL = dynPress * _cl.getValue( angleOfAttackTheta ) * _c;

        double sinAlpha = sin( angleOfAttack );
        double cosAlpha = cos( angleOfAttack );

        double dX = cosAlpha * dD - sinAlpha * dL;
        double dZ = sinAlpha * dD + cosAlpha * dL;

        Vector3 for_aero_bsa( dX, 0.0, dZ );
        Vector3 for_aero_sra = _bsa2sra * for_aero_bsa;
        Vector3 mom_aero_bsa = pos_i_bsa % for_aero_sra;

#       ifdef SIM_ROTOR_TEST
        int i1 = 3 * i;
        int i2 = i1 + 1;
        int i3 = i2 + 1;

        span[ i1 ].visible = true;
        span[ i2 ].visible = true;
        span[ i3 ].visible = true;

        span[ i1 ].b_sra = pos_i_sra;
        span[ i2 ].b_sra = pos_i_sra;
        span[ i3 ].b_sra = pos_i_sra;

        span[ i1 ].v_sra = _bsa2sra * Vector3(  dX, 0.0, 0.0 );
        span[ i2 ].v_sra = _bsa2sra * Vector3( 0.0, 0.0,  dZ );
        span[ i3 ].v_sra = _bsa2sra * vel_i_air_bsa;
#       endif

        // total moment
        Vector3 mom_tot_bsa
                = mom_grav_bsa
                + mom_cf_bsa
                + mom_aero_bsa
                ;

        _moment += _dirFactor * mom_tot_bsa.x();
    }
}
