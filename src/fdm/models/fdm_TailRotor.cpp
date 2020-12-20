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

#include <fdm/models/fdm_TailRotor.h>
#include <fdm/main/fdm_Aerodynamics.h>

#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

TailRotor::TailRotor() :
    _nb ( 0 ),

    _r ( 0.0 ),
    _c ( 0.0 ),
    _a ( 0.0 ),
    _b ( 0.0 ),

    _delta_0 ( 0.0 ),
    _delta_2 ( 0.0 ),

    _ct_max ( DBL_MAX ),
    _cq_max ( DBL_MAX ),

    _thrust_factor ( 1.0 ),
    _torque_factor ( 1.0 ),
    _vel_i_factor  ( 1.0 ),

    _r2  ( 0.0 ),
    _r3  ( 0.0 ),
    _ad  ( 0.0 ),
    _s   ( 0.0 ),
    _i_b ( 0.0 ),

    _omega ( 0.0 ),

    _theta ( 0.0 ),

    _thrust ( 0.0 ),
    _torque ( 0.0 )
{
    _bas2ras = Matrix3x3::identityMatrix();
    _ras2bas = Matrix3x3::identityMatrix();
}

////////////////////////////////////////////////////////////////////////////////

TailRotor::~TailRotor() {}

////////////////////////////////////////////////////////////////////////////////

void TailRotor::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        double cant_angle = 0.0;
        double blade_mass = 0.0;

        _ct_max = DBL_MAX;
        _cq_max = DBL_MAX;

        _thrust_factor = 1.0;
        _torque_factor = 1.0;
        _vel_i_factor  = 1.0;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _r_hub_bas, "hub_center" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, cant_angle, "cant_angle" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _nb, "number_of_blades" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, blade_mass, "blade_mass" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _r, "rotor_radius" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _c, "blade_chord"  );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _a, "lift_slope" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _b, "tip_losses" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _delta_0, "delta_0" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _delta_2, "delta_2" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _ct_max, "ct_max", true );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _cq_max, "cq_max", true );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _thrust_factor , "thrust_factor" , true );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _torque_factor , "torque_factor" , true );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _vel_i_factor  , "vel_i_factor"  , true );

        if ( result == FDM_SUCCESS )
        {
            _r2 = _r * _r;
            _r3 = _r * _r2;

            _ad = M_PI * _r2;
            _s = ( (double)_nb ) * _c * _r / _ad;

            _i_b = blade_mass * _r * _r / 3.0;

            // TODO
            _bas2ras = Matrix3x3( Angles( cant_angle, 0.0, 0.0 ) );
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

void TailRotor::computeForceAndMoment( const Vector3 &vel_air_bas,
                                       const Vector3 &omg_air_bas,
                                       double airDensity )
{
    double omega2 = _omega * _omega;
    double omegaR = _omega * _r;

    // velocity transformations
    Vector3 vel_air_ras = _bas2ras * ( vel_air_bas + ( omg_air_bas % _r_hub_bas ) );

    // angle of attack
    double alpha = Aerodynamics::getAngleOfAttack( vel_air_ras );

    // induced velocity
    double lambda = 0.0;

    const double airspeed = vel_air_ras.getLength();

    // rotor advance ratio
    const double mu   = airspeed * cos( alpha ) / omegaR;
    const double mu2  = mu * mu;
    const double mu_z = airspeed * sin( alpha ) / omegaR;

    // thrust coefficient
    double ct = 0.0;

    // rotor inflow
    double lambda_i = _vel_i_bas.getLength() / omegaR;

    if ( fabs( lambda_i ) < 10e-14 ) lambda_i = 10e-14;

    // iteration loopp
    for ( int i = 0; i < 100; i++ )
    {
        lambda = mu_z - lambda_i;

        // thrust coefficient
        ct = 0.5 * _a * _s  * _b * ( lambda * _b / 2.0 + _theta * ( _b + 1.5 * mu2 ) / 3.0 );
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

    // induced velocity
    _vel_i_bas = _ras2bas * Vector3( 0.0, 0.0, _vel_i_factor * lambda_i * omegaR );

    _thrust = _thrust_factor * airDensity * _ad * _r2 * omega2 * ct;
    _torque = _torque_factor * airDensity * _ad * _r3 * omega2 * cq;

    _for_bas = _ras2bas * Vector3( 0.0, 0.0, -_thrust );
    _mom_bas = _r_hub_bas % _for_bas;

    if ( !_for_bas.isValid() || !_mom_bas.isValid() )
    {
        Exception e;

        e.setType( Exception::UnexpectedNaN );
        e.setInfo( "NaN detected in the tail rotor model." );

        FDM_THROW( e );
    }
}

////////////////////////////////////////////////////////////////////////////////

void TailRotor::update( double omega, double theta )
{
    _omega = omega;
    _theta = theta;
}
