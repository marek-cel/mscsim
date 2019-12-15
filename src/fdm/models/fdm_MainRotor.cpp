/****************************************************************************//*
 * Copyright (C) 2019 Marek M. Cel
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

#include <fdm/models/fdm_MainRotor.h>
#include <fdm/main/fdm_Aerodynamics.h>

#include <fdm/utils/fdm_String.h>

#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

MainRotor::MainRotor() :
    _direction ( CW ),

    _nb ( 0 ),

    _r ( 0.0 ),
    _c ( 0.0 ),
    _e ( 0.0 ),
    _a ( 0.0 ),
    _b ( 0.0 ),

    _delta_0 ( 0.0 ),
    _delta_2 ( 0.0 ),

    _beta_max ( 0.0 ),

    _ct_max ( 0.0 ),
    _cq_max ( 0.0 ),

    _thrust_factor ( 1.0 ),
    _torque_factor ( 1.0 ),
    _vel_i_factor  ( 1.0 ),

    _r2  ( 0.0 ),
    _r3  ( 0.0 ),
    _r4  ( 0.0 ),
    _b2  ( 0.0 ),
    _b3  ( 0.0 ),
    _b4  ( 0.0 ),
    _ad  ( 0.0 ),
    _s   ( 0.0 ),
    _s_b ( 0.0 ),
    _i_b ( 0.0 ),

    _omega ( 0.0 ),

    _beta_0      ( 0.0 ),
    _beta_1c_ras ( 0.0 ),
    _beta_1s_ras ( 0.0 ),

    _rotorDiskRoll  ( 0.0 ),
    _rotorDiskPitch ( 0.0 ),

    _thrust ( 0.0 ),
    _torque ( 0.0 )
{
    _bas2ras = Matrix3x3::createIdentityMatrix();
    _ras2bas = Matrix3x3::createIdentityMatrix();
    _ras2cas = Matrix3x3::createIdentityMatrix();
    _cas2ras = Matrix3x3::createIdentityMatrix();

    _bas2cas = Matrix3x3::createIdentityMatrix();

    _ras2rwas = Matrix3x3::createIdentityMatrix();
    _rwas2ras = Matrix3x3::createIdentityMatrix();

    _cas2cwas = Matrix3x3::createIdentityMatrix();
    _cwas2cas = Matrix3x3::createIdentityMatrix();

    _bas2cwas = Matrix3x3::createIdentityMatrix();
}

////////////////////////////////////////////////////////////////////////////////

MainRotor::~MainRotor() {}

////////////////////////////////////////////////////////////////////////////////

void MainRotor::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        if ( String::toBool( dataNode.getAttribute( "counter-clockwise" ), false ) )
        {
            _direction = CCW;
        }
        else
        {
            _direction = CW;
        }

        double inclination = 0.0;
        double blade_mass  = 0.0;

        _thrust_factor = 1.0;
        _torque_factor = 1.0;
        _vel_i_factor  = 1.0;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _r_hub_bas, "hub_center" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, inclination, "inclination" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _nb, "number_of_blades" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, blade_mass, "blade_mass" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _r, "rotor_radius" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _c, "blade_chord"  );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _e, "hinge_offset" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _a, "lift_slope" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _b, "tip_losses" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _delta_0, "delta_0" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _delta_2, "delta_2" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _beta_max, "beta_max" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _ct_max, "ct_max" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _cq_max, "cq_max" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _thrust_factor , "thrust_factor" , true );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _torque_factor , "torque_factor" , true );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _vel_i_factor  , "vel_i_factor"  , true );

        if ( result == FDM_SUCCESS )
        {
            _r2 = _r * _r;
            _r3 = _r * _r2;
            _r4 = _r * _r3;

            _b2 = _b * _b;
            _b3 = _b * _b2;
            _b4 = _b * _b3;

            _ad = M_PI * _r2;
            _s = ( (double)_nb ) * _c * _r / _ad;

            _s_b = blade_mass * _r / 2.0;
            _i_b = blade_mass * _r2 / 3.0;

            _bas2ras = Matrix3x3( Angles( 0.0, -inclination, 0.0 ) );
            _ras2bas = _bas2ras.getTransposed();
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

void MainRotor::computeForceAndMoment( const Vector3 &vel_bas,
                                       const Vector3 &omg_bas,
                                       const Vector3 &acc_bas,
                                       const Vector3 &eps_bas,
                                       const Vector3 &grav_bas,
                                       const Vector3 &vel_air_bas,
                                       const Vector3 &omg_air_bas,
                                       double airDensity,
                                       double collective,
                                       double cyclicLat,
                                       double cyclicLon )
{
    double omega2 = _omega * _omega;
    double omegaR = _omega * _r;

    // Lock number
    double gamma = airDensity * _a * _c * _r4 / _i_b;

    // controls
    double theta_0      = collective;
    double theta_1c_ras = _direction == CW ? cyclicLat : -cyclicLat;
    double theta_1s_ras = cyclicLon; // ????????????????????????????????????????????????????

    // RAS <-> CAS
    _ras2cas = Matrix3x3( Angles( theta_1c_ras, theta_1s_ras, 0.0 ) );

    _cas2ras = _ras2cas.getTransposed();

    // BAS -> CAS
    _bas2cas = _bas2ras * _ras2cas;

    // velocity transformations
    Vector3 vel_air_ras = _bas2ras * ( vel_air_bas + ( omg_air_bas ^ _r_hub_bas ) );
    Vector3 vel_air_cas = _ras2cas * vel_air_ras;

    // sideslip angle
    double beta_cas = Aerodynamics::getSideslipAngle( vel_air_cas );
    double beta_ras = Aerodynamics::getSideslipAngle( vel_air_ras );

    // RAS -> RWAS
    _ras2rwas = Matrix3x3( Angles( 0.0, 0.0, beta_ras ) );
    _rwas2ras = _ras2rwas.getTransposed();

    // CAS <-> CWAS
    _cas2cwas = Matrix3x3( Angles( 0.0, 0.0, beta_cas ) );
    _cwas2cas = _cas2cwas.getTransposed();

    // BAS -> CWAS
    _bas2cwas = _bas2cas * _cas2cwas;

    // velocity transformations
    Vector3 vel_air_rwas = _ras2rwas * vel_air_ras;
    Vector3 vel_air_cwas = _cas2cwas * vel_air_cas;
    Vector3 omg_air_cwas = _bas2cwas * omg_air_bas;

    // acceleration
    Vector3 acc_hub_bas = acc_bas + ( omg_bas ^ vel_bas )
                        + ( omg_bas ^ ( omg_bas ^ _r_hub_bas ) )
                        + ( eps_bas ^ _r_hub_bas )
                        - grav_bas;
    Vector3 acc_hub_cwas = _bas2cwas * acc_hub_bas;

    // angle of attack
    double alpha = Aerodynamics::getAngleOfAttack( vel_air_rwas );

    // flapping coefficients
    double beta_1c_cwas = 0.0;
    double beta_1s_cwas = 0.0;

    // induced velocity
    double lambda = 0.0;

    const double p = omg_air_cwas( _ip );
    const double q = omg_air_cwas( _iq );

    const double airspeed = vel_air_cwas.getLength();

    // rotor advance ratio
    const double mu   = airspeed * cos( alpha ) / omegaR;
    const double mu2  = mu * mu;
    const double mu_z = airspeed * sin( alpha ) / omegaR;

    const double a_z = -acc_hub_cwas.z();

    // thrust coefficient
    double ct = 0.0;

    // rotor inflow
    double lambda_i = _vel_i_bas.getLength() / omegaR;

    if ( fabs( lambda_i ) < 10e-14 ) lambda_i = 10e-14;

    // TODO Young approximation

    // iteration loop
    for ( int i = 0; i < 100; i++ )
    {
        lambda = mu_z - lambda_i;

        // flapping coefficients
        if ( _direction == CW )
        {
            _beta_0 = ( gamma / 2.0 ) * ( _b3 * lambda / 3.0 - _b3 * p * mu / ( 6.0 * _omega ) + _b4 * theta_0 / 4.0 + _b2 * theta_0 * mu2 / 4.0 )
                    - a_z * _s_b / ( _i_b * omega2 );

            beta_1c_cwas = 2.0 * mu * ( lambda + 4.0 * _b * theta_0 / 3.0 ) / ( mu2 / 2.0 - _b2 )
                    - ( _b4 * p / _omega + 16.0 * q / ( gamma * _omega ) ) / ( _b2 * ( mu2 / 2.0 - _b2 ) );

            beta_1s_cwas = -4.0 * _beta_0 * mu * _b / ( mu2 / 2.0 + _b2 ) / 3.0
                    + ( _b4 * q / _omega - 16.0 * p / ( gamma * _omega ) ) / ( _b2 * ( mu2 / 2.0 + _b2 ) );
        }
        else
        {
            _beta_0 = ( gamma / 2.0 ) * ( _b3 * lambda / 3.0 + _b3 * p * mu / ( 6.0 * _omega ) + _b4 * theta_0 / 4.0 + _b2 * theta_0 * mu2 / 4.0 )
                    - a_z * _s_b / ( _i_b * omega2 );

            beta_1c_cwas = 2.0 * mu * ( lambda + 4.0 * _b * theta_0 / 3.0 ) / ( mu2 / 2.0 - _b2 )
                    + ( _b4 * p / _omega - 16.0 * q / ( gamma * _omega ) ) / ( _b2 * ( mu2 / 2.0 - _b2 ) );

            beta_1s_cwas = - 4.0 * _beta_0 * mu * _b / ( mu2 / 2.0 + _b2 ) / 3.0
                    + ( _b4 * q / _omega + 16.0 * p / ( gamma * _omega ) ) / ( _b2 * ( mu2 / 2.0 + _b2 ) );
        }

        // limits
        _beta_0      = Misc::satur( -_beta_max, _beta_max, _beta_0      );
        beta_1c_cwas = Misc::satur( -_beta_max, _beta_max, beta_1c_cwas );
        beta_1s_cwas = Misc::satur( -_beta_max, _beta_max, beta_1s_cwas );

        // thrust coefficient
        ct = 0.5 * _a * _s * _b * ( lambda * _b / 2.0
                                  + theta_0 * ( _b2 + 1.5 * mu2 ) / 3.0
                                  + _b * mu * beta_1c_cwas / 4.0 );
        if ( ct > _ct_max ) ct = _ct_max;

        // zero function (Padfield p.124)
        double lambda_d = mu2 + lambda * lambda;
        double g_0 = lambda_i - ct / ( 2.0 * sqrt( lambda_d ) );

        // break condition
        if ( fabs( g_0 ) < 1.0e-6 ) break;

        // (Padfield p.124)
        double h_j = -( 2.0 * lambda_i * sqrt( lambda_d ) - ct ) * lambda_d
                / ( 2*pow( lambda_d, 2.0/3.0 ) + _a * _s * lambda_d / 4.0 - ct * lambda );

        // (Padfield p.124)
        double f_j = 1.0;
        if ( fabs( mu_z ) < 0.05 ) f_j = 0.6;

        // (Padfield p.124)
        double lambda_i_new = lambda_i + f_j * h_j;

        if ( Misc::isValid( lambda_i_new ) ) lambda_i = lambda_i_new;
    }

    // drag coefficient
    double cd = _delta_0 + _delta_2 * ct*ct;

    // moment of resistance coefficient (Bramwell p.102)
    double cq = cd * _s * ( 1.0 + 3.0 * mu2 ) / 8.0 - lambda * ct;
    if ( cq > _cq_max ) cq = _cq_max;

    // flapping in axes with sideslip
    double cosBeta = cos( beta_cas );
    double sinBeta = sin( beta_cas );

    double beta_1c_cas = beta_1c_cwas * cosBeta - beta_1s_cwas * sinBeta * ( _direction == CW ? 1.0 : -1.0 );
    double beta_1s_cas = beta_1s_cwas * cosBeta + beta_1c_cwas * sinBeta * ( _direction == CW ? 1.0 : -1.0 );

    // flapping coefficients
    _beta_1c_ras = Misc::satur( -_beta_max, _beta_max, beta_1c_cas - theta_1s_ras );
    _beta_1s_ras = Misc::satur( -_beta_max, _beta_max, beta_1s_cas + theta_1c_ras );

    _rotorDiskPitch = -_beta_1c_ras;
    _rotorDiskRoll = _direction == CW ? _beta_1s_ras : -_beta_1s_ras;

    // DAS <-> BAS
    _das2bas = Matrix3x3( Angles( _rotorDiskRoll, _rotorDiskPitch, 0.0 ) ).getTransposed() * _ras2bas;
    _bas2das = _das2bas.getTransposed();

    Vector3 vel_air_das = _bas2das * ( vel_air_bas + ( omg_air_bas ^ _r_hub_bas ) );
    double beta_das = Aerodynamics::getSideslipAngle( vel_air_das );
    Matrix3x3 dwas2das = Matrix3x3( Angles( 0.0, 0.0, beta_das ) ).getTransposed();

    // induced velocity (Padfield p.121)
    double chi = atan2( mu, lambda_i - mu_z );
    double vel_i = _vel_i_factor * lambda_i * omegaR;

    _vel_i_bas = _das2bas * ( dwas2das * Vector3( -vel_i * sin( chi ), 0.0, vel_i * cos( chi ) ) );

    _thrust = _thrust_factor * airDensity * _ad * _r2 * omega2 * ct;
    _torque = _torque_factor * airDensity * _ad * _r3 * omega2 * cq;

    _for_bas = _das2bas * Vector3( 0.0, 0.0, -_thrust );
    _mom_bas = ( _r_hub_bas ^ _for_bas )
             + _ras2bas * Vector3( 0.0, 0.0, _direction == CW ? -_torque : _torque );

    if ( !_for_bas.isValid() || !_mom_bas.isValid() )
    {
        Exception e;

        e.setType( Exception::UnexpectedNaN );
        e.setInfo( "NaN detected in the main rotor model." );

        FDM_THROW( e );
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainRotor::update( double omega )
{
    _omega = omega;
}
