/****************************************************************************//*
 * Copyright (C) 2021 Marek M. Cel
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
#include <fdm/utils/fdm_Units.h>

#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_Test.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

MainRotor::MainRotor() :
    _ccw ( false ),

    _nb ( 0 ),

    _r ( 0.0 ),
    _c ( 0.0 ),
    _e ( 0.0 ),

    _a ( 0.0 ),
    _b ( 0.0 ),

    _delta_0 ( 0.0 ),
    _delta_2 ( 0.0 ),

    _beta_max ( 0.0 ),

    _ct_max ( DBL_MAX ),
    _ch_max ( DBL_MAX ),
    _cq_max ( DBL_MAX ),

    _thrust_factor ( 1.0 ),
    _hforce_factor ( 1.0 ),
    _torque_factor ( 1.0 ),
    _vel_i_factor  ( 1.0 ),

    _r2 ( 0.0 ),
    _r3 ( 0.0 ),
    _r4 ( 0.0 ),
    _b2 ( 0.0 ),
    _b3 ( 0.0 ),
    _b4 ( 0.0 ),
    _ad ( 0.0 ),
    _s  ( 0.0 ),
    _sb ( 0.0 ),
    _ib ( 0.0 ),
    _cd ( 0.0 ),

    _omega   ( 0.0 ),
    _azimuth ( 0.0 ),

    _beta_0  ( 0.0 ),
    _beta_1c ( 0.0 ),
    _beta_1s ( 0.0 ),

    _theta_0  ( 0.0 ),
    _theta_1c ( 0.0 ),
    _theta_1s ( 0.0 ),

    _coningAngle ( 0.0 ),
    _diskRoll    ( 0.0 ),
    _diskPitch   ( 0.0 ),

    _ct ( 0.0 ),
    _ch ( 0.0 ),
    _cq ( 0.0 ),

    _thrust ( 0.0 ),
    _hforce ( 0.0 ),
    _torque ( 0.0 ),

    _vel_i ( 0.0 ),

    _wakeSkew ( 0.0 )
{
    _bas2ras  = Matrix3x3::identityMatrix();
    _ras2bas  = Matrix3x3::identityMatrix();
    _ras2cas  = Matrix3x3::identityMatrix();
    _cas2ras  = Matrix3x3::identityMatrix();
    _bas2cas  = Matrix3x3::identityMatrix();
    _ras2rwas = Matrix3x3::identityMatrix();
    _rwas2ras = Matrix3x3::identityMatrix();
    _cas2cwas = Matrix3x3::identityMatrix();
    _cwas2cas = Matrix3x3::identityMatrix();
    _bas2cwas = Matrix3x3::identityMatrix();
    _cwas2bas = Matrix3x3::identityMatrix();
}

////////////////////////////////////////////////////////////////////////////////

MainRotor::~MainRotor() {}

////////////////////////////////////////////////////////////////////////////////

void MainRotor::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        _ccw = String::toBool( dataNode.getAttribute( "counter-clockwise" ), false );

        double blade_mass  = 0.0;
        double inclination = 0.0;

        _ct_max = DBL_MAX;
        _ch_max = DBL_MAX;
        _cq_max = DBL_MAX;

        _thrust_factor = 1.0;
        _hforce_factor = 1.0;
        _torque_factor = 1.0;
        _vel_i_factor  = 1.0;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_r_hub_bas, "hub_center" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &inclination, "inclination" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_nb, "number_of_blades" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &blade_mass, "blade_mass" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_r, "rotor_radius" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_c, "blade_chord"  );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_e, "hinge_offset" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_a, "lift_slope" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_b, "tip_losses" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_delta_0, "delta_0" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_delta_2, "delta_2" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_beta_max, "beta_max" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_ct_max, "ct_max", true );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_ch_max, "ch_max", true );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_cq_max, "cq_max", true );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_thrust_factor , "thrust_factor" , true );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_hforce_factor , "hforce_factor" , true );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_torque_factor , "torque_factor" , true );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_vel_i_factor  , "vel_i_factor"  , true );

        if ( result == FDM_SUCCESS )
        {
            _bas2ras = Matrix3x3( Angles( 0.0, -inclination, 0.0 ) );
            _ras2bas = _bas2ras.getTransposed();

            _r2 = _r * _r;
            _r3 = _r * _r2;
            _r4 = _r * _r3;

            _b2 = _b * _b;
            _b3 = _b * _b2;
            _b4 = _b * _b3;

            _ad = M_PI * _r2;
            _s = ( (double)_nb ) * _c * _r / _ad;

            _sb = blade_mass * _r / 2.0;
            _ib = blade_mass * _r2 / 3.0;

            _cd = _ccw ? 1.0 : -1.0;
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
                                       const Vector3 &vel_air_bas,
                                       const Vector3 &omg_air_bas,
                                       const Vector3 &grav_bas,
                                       double airDensity )
{
    double omega2 = _omega * _omega;
    double omegaR = _omega * _r;

    // Lock number
    double gamma = airDensity * _a * _c * _r4 / _ib;

    // RAS <-> CAS
    _ras2cas = Matrix3x3( Angles( _theta_1c, _theta_1s, 0.0 ) );

    _cas2ras = _ras2cas.getTransposed();

    // BAS -> CAS
    _bas2cas = _bas2ras * _ras2cas;

    // velocity transformations
    Vector3 vel_air_ras = _bas2ras * ( vel_air_bas + ( omg_air_bas % _r_hub_bas ) );
    Vector3 vel_air_cas = _ras2cas * vel_air_ras;

    // sideslip angle
    double beta_cas = Aerodynamics::getSideslipAngle( vel_air_cas );
    double beta_ras = Aerodynamics::getSideslipAngle( vel_air_ras );

    //std::cout << Units::rad2deg( beta_cas )  << "  " << vel_air_cas.toString() << std::endl;

    // RAS -> RWAS
    _ras2rwas = Matrix3x3( Angles( 0.0, 0.0, beta_ras ) );
    _rwas2ras = _ras2rwas.getTransposed();

    // CAS <-> CWAS
    _cas2cwas = Matrix3x3( Angles( 0.0, 0.0, beta_cas ) );
    _cwas2cas = _cas2cwas.getTransposed();

    // BAS -> CWAS
    _bas2cwas = _bas2cas * _cas2cwas;
    _cwas2bas = _bas2cwas.getTransposed();

    // velocity transformations
    Vector3 vel_air_rwas = _ras2rwas * vel_air_ras;
    Vector3 vel_air_cwas = _cas2cwas * vel_air_cas;
    Vector3 omg_air_cwas = _bas2cwas * omg_air_bas;

    // acceleration
    Vector3 acc_hub_bas = acc_bas
                        + ( omg_bas % ( omg_bas % _r_hub_bas ) )
                        + ( eps_bas % _r_hub_bas )
                        - grav_bas;
    Vector3 acc_hub_cwas = _bas2cwas * acc_hub_bas;

    // angle of attack
    double alpha = Aerodynamics::getAngleOfAttack( vel_air_rwas );

    // flapping coefficients
    double beta_1c_cwas = 0.0;
    double beta_1s_cwas = 0.0;

    // induced velocity
    double lambda = 0.0;

    const double p = omg_air_cwas.p();
    const double q = omg_air_cwas.q();

    const double airspeed = vel_air_cwas.getLength();

    // rotor advance ratio
    const double mu   = airspeed * cos( alpha ) / omegaR;
    const double mu2  = mu * mu;
    const double mu_z = airspeed * sin( alpha ) / omegaR;

    const double a_z = -acc_hub_cwas.z();

    // thrust coefficient
    _ct = 0.0;

    // rotor inflow
    double lambda_i = _vel_i / omegaR;

    if ( fabs( lambda_i ) < 10e-14 ) lambda_i = 10e-14;

    // TODO Young approximation

    // iteration loop
    for ( int i = 0; i < 100; i++ )
    {
        lambda = mu_z - lambda_i;

        // flapping coefficients
        _beta_0 = ( gamma / 2.0 ) * ( _b3 * lambda / 3.0 + _cd * _b3 * p * mu / ( 6.0 * _omega ) + _b4 * _theta_0 / 4.0 + _b2 * _theta_0 * mu2 / 4.0 )
                - a_z * _sb / ( _ib * omega2 );

        beta_1c_cwas = 2.0 * mu * ( lambda + 4.0 * _b * _theta_0 / 3.0 ) / ( mu2 / 2.0 - _b2 )
                + _cd * ( _b4 * p / _omega - _cd * 16.0 * q / ( gamma * _omega ) ) / ( _b2 * ( mu2 / 2.0 - _b2 ) );

        beta_1s_cwas = -4.0 * _beta_0 * mu * _b / ( mu2 / 2.0 + _b2 ) / 3.0
                + ( _b4 * q / _omega + _cd * 16.0 * p / ( gamma * _omega ) ) / ( _b2 * ( mu2 / 2.0 + _b2 ) );

        //if ( _ccw )
        //{
        //    _beta_0 = ( gamma / 2.0 ) * ( _b3 * lambda / 3.0 + _b3 * p * mu / ( 6.0 * _omega ) + _b4 * _theta_0 / 4.0 + _b2 * _theta_0 * mu2 / 4.0 )
        //            - a_z * _sb / ( _ib * omega2 );
        //
        //    beta_1c_cwas = 2.0 * mu * ( lambda + 4.0 * _b * _theta_0 / 3.0 ) / ( mu2 / 2.0 - _b2 )
        //            + ( _b4 * p / _omega - 16.0 * q / ( gamma * _omega ) ) / ( _b2 * ( mu2 / 2.0 - _b2 ) );
        //
        //    beta_1s_cwas = -4.0 * _beta_0 * mu * _b / ( mu2 / 2.0 + _b2 ) / 3.0
        //            + ( _b4 * q / _omega + 16.0 * p / ( gamma * _omega ) ) / ( _b2 * ( mu2 / 2.0 + _b2 ) );
        //}
        //else
        //{
        //    _beta_0 = ( gamma / 2.0 ) * ( _b3 * lambda / 3.0 - _b3 * p * mu / ( 6.0 * _omega ) + _b4 * _theta_0 / 4.0 + _b2 * _theta_0 * mu2 / 4.0 )
        //            - a_z * _sb / ( _ib * omega2 );
        //
        //    beta_1c_cwas = 2.0 * mu * ( lambda + 4.0 * _b * _theta_0 / 3.0 ) / ( mu2 / 2.0 - _b2 )
        //            - ( _b4 * p / _omega + 16.0 * q / ( gamma * _omega ) ) / ( _b2 * ( mu2 / 2.0 - _b2 ) );
        //
        //    beta_1s_cwas = -4.0 * _beta_0 * mu * _b / ( mu2 / 2.0 + _b2 ) / 3.0
        //            + ( _b4 * q / _omega - 16.0 * p / ( gamma * _omega ) ) / ( _b2 * ( mu2 / 2.0 + _b2 ) );
        //}

        // limits
        _beta_0      = Misc::satur( -_beta_max, _beta_max, _beta_0      );
        beta_1c_cwas = Misc::satur( -_beta_max, _beta_max, beta_1c_cwas );
        beta_1s_cwas = Misc::satur( -_beta_max, _beta_max, beta_1s_cwas );

        // thrust coefficient
        _ct = 0.5 * _a * _s * _b * ( lambda * _b / 2.0
                                  + _theta_0 * ( _b2 + 1.5 * mu2 ) / 3.0
                                  + _b * mu * beta_1c_cwas / 4.0 );
        if ( _ct > _ct_max ) _ct = _ct_max;

        // zero function (Padfield p.124)
        double lambda_d = mu2 + lambda * lambda;
        double g_0 = lambda_i - _ct / ( 2.0 * sqrt( lambda_d ) );

        // break condition
        if ( fabs( g_0 ) < 1.0e-6 ) break;

        // (Padfield p.124)
        double h_j = -( 2.0 * lambda_i * sqrt( lambda_d ) - _ct ) * lambda_d
                / ( 2*pow( lambda_d, 2.0/3.0 ) + _a * _s * lambda_d / 4.0 - _ct * lambda );

        // (Padfield p.124)
        double f_j = 1.0;
        if ( fabs( mu_z ) < 0.05 ) f_j = 0.6;

        // (Padfield p.124)
        double lambda_i_new = lambda_i + f_j * h_j;

        if ( Misc::isValid( lambda_i_new ) ) lambda_i = lambda_i_new;
    }

    // flapping in axes with sideslip
    double cosBetaCAS = cos( beta_cas );
    double sinBetaCAS = sin( beta_cas );

    double beta_1c_cas = beta_1c_cwas * cosBetaCAS - beta_1s_cwas * sinBetaCAS * ( -1.0 * _cd );
    double beta_1s_cas = beta_1s_cwas * cosBetaCAS + beta_1c_cwas * sinBetaCAS * ( -1.0 * _cd );

    // flapping coefficients
    _beta_1c = Misc::satur( -_beta_max, _beta_max, beta_1c_cas - _theta_1s );
    _beta_1s = Misc::satur( -_beta_max, _beta_max, beta_1s_cas + _theta_1c );

    _coningAngle =  _beta_0;
    _diskRoll    = -_beta_1s * _cd;
    _diskPitch   = -_beta_1c;

    // DAS <-> BAS
    _das2bas = Matrix3x3( Angles( _diskRoll, _diskPitch, 0.0 ) ).getTransposed() * _ras2bas;
    _bas2das = _das2bas.getTransposed();

    // drag coefficient (Padfield p.98)
    double cd = _delta_0 + _delta_2 * _ct*_ct;

    // H-force coefficient (Bramwell p.100)
    _ch = 0.5 * _a * _s * ( 0.5 * mu * cd / _a
                          + beta_1c_cwas * _theta_0 / 3.0
                          + 0.75 * lambda * beta_1c_cwas
                          - 0.5 * mu * _theta_0 * lambda
                          + 0.25 * mu * beta_1c_cwas * beta_1c_cwas );

//    std::cout << Units::rad2deg( beta_cas ) << std::endl;

//    std::cout << _ch;
//    std::cout << " / " << ( 0.5 * _a * _s );
//    std::cout << " = " << ( 0.5 * mu * cd / _a );
//    std::cout << " + " << ( beta_1c_cwas * _theta_0 / 3.0 );
//    std::cout << " + " << ( 0.75 * lambda * beta_1c_cwas );
//    std::cout << " - " << ( 0.5 * mu * _theta_0 * lambda );
//    std::cout << " + " << ( 0.25 * mu * beta_1c_cwas * beta_1c_cwas );
//    std::cout << std::endl;

    // moment of resistance coefficient (Bramwell p.102)
    _cq = cd * _s * ( 1.0 + 3.0 * mu2 ) / 8.0 - lambda * _ct - mu * _ch;
    if ( _cq > _cq_max ) _cq = _cq_max;

    //Vector3 vel_air_das = _bas2das * ( vel_air_bas + ( omg_air_bas % _r_hub_bas ) );
    //double beta_das = Aerodynamics::getSideslipAngle( vel_air_das );
    //Matrix3x3 dwas2das = Matrix3x3( Angles( 0.0, 0.0, beta_das ) ).getTransposed();

    // induced velocity (Padfield p.117)
    _vel_i = _vel_i_factor * lambda_i * omegaR;

    // rotor wake skew angle (Padfield p.121)
    _wakeSkew = atan2( mu, lambda_i - mu_z );

    _thrust = _thrust_factor * airDensity * _ad * _r2 * omega2 * _ct;
    _hforce = _hforce_factor * airDensity * _ad * _r2 * omega2 * _ch;
    _torque = _torque_factor * airDensity * _ad * _r3 * omega2 * _cq;

//    Vector3 hforce_bas = _cwas2bas * Vector3( _hforce, 0.0, 0.0 );

//    Vector3 b = _r_hub_bas;
//    Vector3 v = hforce_bas;
//    v.normalize();
//    Test::setVector( b, 5.0 * v );

    //std::cout << _hforce << "   " << hforce_ras.toString() << std::endl;

    _for_bas = _das2bas * Vector3( 0.0, 0.0, -_thrust );
             //+ _ras2bas * hforce_ras;
    _mom_bas = ( _r_hub_bas % _for_bas )
             + _ras2bas * Vector3( 0.0, 0.0, _cd * _torque );

    if ( !_for_bas.isValid() || !_mom_bas.isValid() )
    {
        Exception e;

        e.setType( Exception::UnexpectedNaN );
        e.setInfo( "NaN detected in the main rotor model." );

        FDM_THROW( e );
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainRotor::update( double omega,
                        double azimuth,
                        double collective,
                        double cyclicLat,
                        double cyclicLon )
{
    _omega   = omega;
    _azimuth = azimuth;

    _theta_0  =  collective;
    _theta_1c = -cyclicLat * _cd;
    _theta_1s =  cyclicLon;
}

