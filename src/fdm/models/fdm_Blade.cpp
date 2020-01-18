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

Matrix3x3 Blade::getRAS2Psi( double psi )
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

Matrix3x3 Blade::getPsi2IBC( double beta )
{
    Matrix3x3 result = Matrix3x3::createIdentityMatrix();

    // TODO

    return result;
}

////////////////////////////////////////////////////////////////////////////////

Matrix3x3 Blade::getRAS2IBC( double psi, double beta )
{
    return getRAS2Psi( psi ) * getPsi2IBC( beta );
}

////////////////////////////////////////////////////////////////////////////////

Blade::Blade() :
    _b ( 0.0 ),
    _c ( 0.0 ),
    _e ( 0.0 ),
    _m ( 0.0 ),

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
    _beta_2 ( 0.0 )
{
    _cd = Table::createOneRecordTable( 0.0 );
    _cl = Table::createOneRecordTable( 0.0 );
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

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _beta_max, "beta_max" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _twist_0    , "twist_0" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _twist_rate , "twist_rate" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _cd, "cd" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _cl, "cl" );

        if ( result == FDM_SUCCESS )
        {
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

#include <iostream>
#include <fdm/utils/fdm_Units.h>
void Blade::computeForceAndMoment( const Vector3 &vel_bas,
                                   const Vector3 &omg_bas,
                                   const Vector3 &acc_bas,
                                   const Vector3 &eps_bas,
                                   const Vector3 &grav_bas,
                                   const Vector3 &vel_air_ras,
                                   const Vector3 &omg_air_ras,
                                   double omega,
                                   double azimuth,
                                   double airDensity,
                                   double theta_0,
                                   double theta_1c,
                                   double theta_1s )
{
    _thrust = 0.0;
    _hforce = 0.0;
    _yforce = 0.0;
    _torque = 0.0;
    _moment = 0.0;

    Matrix3x3 ras2psi = getRAS2Psi( azimuth );
    Matrix3x3 psi2ras = ras2psi.getTransposed();

    Matrix3x3 ras2ibc = ras2psi * getPsi2IBC( _beta_0 );
    Matrix3x3 ibc2ras = ras2ibc.getTransposed();

    double cosAzim = cos( azimuth );
    double sinAzim = sin( azimuth );

    double cosBeta = cos( _beta_0 );
    double sinBeta = sin( _beta_0 );

    double theta = theta_0 + theta_1c*cosAzim + theta_1s*sinAzim;

    int steps_y = 10;
    double delta_y = _b / (double)( steps_y );

    //std::cout << Units::rad2deg( azimuth ) << ": ";
    for ( int i = 0; i < steps_y; i++ )
    {
        double y = ( i ) * delta_y + 0.5 * delta_y; // center of segment
        double r = _e + cosBeta * y;

        Vector3 r_ras = psi2ras * Vector3( 0.0, _e, 0.0 ) + ibc2ras * Vector3( 0.0, y, 0.0 );
        Vector3 v_ibc = ras2ibc * ( vel_air_ras + ( omg_air_ras % r_ras ) );

        double vx = v_ibc.x() - omega   * r;
        double vz = v_ibc.z() + _beta_1 * y;

        // section angle of attack
        double angleOfAttack = Aerodynamics::getAngleOfAttack( -vx, -vz ) + theta;

        // section airspeed
        double airspeed = sqrt( vx*vx + vz*vz );

        // dynamic pressure
        double dynPress = 0.5 * airDensity * Misc::pow2( airspeed );

        double dD = dynPress * _cd.getValue( angleOfAttack ) * _c;
        double dL = dynPress * _cl.getValue( angleOfAttack ) * _c;

        double sinAlpha = sin( angleOfAttack );
        double cosAlpha = cos( angleOfAttack );

        double dX = cosAlpha * dD - sinAlpha * dL;
        double dZ = sinAlpha * dD + cosAlpha * dL;

        Vector3 d_for_ibc( dX, 0.0, dZ );
        Vector3 d_for_ras = ibc2ras * d_for_ibc;

        double dQ = r * dX;
        double dM = y * dZ;

        _hforce += delta_y * d_for_ras.x() * -1.0;
        _yforce += delta_y * d_for_ras.y();
        _thrust += delta_y * d_for_ras.z() * -1.0;
        _torque += delta_y * dQ;
        _moment += delta_y * dM;

//        dMom_FH = dMom_FH
//                - dBladeStMom * 9.81
//                - Pow2( dOmg ) * dBladeInMom * s_Beta;
    }
    //std::cout << std::endl;
}

////////////////////////////////////////////////////////////////////////////////

void Blade::integrate( double timeStep,
                       const Vector3 &vel_bas,
                       const Vector3 &omg_bas,
                       const Vector3 &acc_bas,
                       const Vector3 &eps_bas,
                       const Vector3 &grav_bas,
                       const Vector3 &vel_air_ras,
                       const Vector3 &omg_air_ras,
                       double omega,
                       double azimuth,
                       double airDensity,
                       double theta_0,
                       double theta_1c,
                       double theta_1s )
{
    computeForceAndMoment( vel_bas,
                           omg_bas,
                           acc_bas,
                           eps_bas,
                           grav_bas,
                           vel_air_ras,
                           omg_air_ras,
                           omega,
                           azimuth,
                           airDensity,
                           theta_0,
                           theta_1c,
                           theta_1s );

    double beta_0_prev = _beta_0;
    double beta_1_prev = _beta_1;

    // flaping angle time 2nd derivative
    _beta_2 = _moment / _ib;

    // flapping angle and flaping angle time derivative Euler integration
    _beta_1 += _beta_2 * timeStep;
    _beta_0 += _beta_1 * timeStep;

    // limiting flapping angle
    _beta_0 = Misc::satur( -_beta_max, _beta_max, _beta_0 );

    // back calculating flaping angle time derivatives
    if ( timeStep >= FDM_TIME_STEP_MIN )
    {
        _beta_1 = ( _beta_0 - beta_0_prev ) / timeStep;
        _beta_2 = ( _beta_1 - beta_1_prev ) / timeStep;
    }
}
