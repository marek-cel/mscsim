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

#include <fdm/models/fdm_TailRotor.h>
#include <fdm/main/fdm_Aerodynamics.h>

#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

TailRotor::TailRotor() :
    m_nb ( 0 ),

    m_r ( 0.0 ),
    m_c ( 0.0 ),
    m_a ( 0.0 ),
    m_b ( 0.0 ),

    m_delta_0 ( 0.0 ),
    m_delta_2 ( 0.0 ),

    m_ct_max ( 0.0 ),
    m_cq_max ( 0.0 ),

    m_thrust_factor ( 1.0 ),
    m_torque_factor ( 1.0 ),
    m_vel_i_factor  ( 1.0 ),

    m_r2  ( 0.0 ),
    m_r3  ( 0.0 ),
    m_ad  ( 0.0 ),
    m_s   ( 0.0 ),
    m_i_b ( 0.0 ),

    m_omega ( 0.0 ),

    m_thrust ( 0.0 ),
    m_torque ( 0.0 )
{
    m_bas2ras = Matrix3x3::createIdentityMatrix();
    m_ras2bas = Matrix3x3::createIdentityMatrix();
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

        m_thrust_factor = 1.0;
        m_torque_factor = 1.0;
        m_vel_i_factor  = 1.0;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_r_hub_bas, "hub_center" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, cant_angle, "cant_angle" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_nb, "number_of_blades" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, blade_mass, "blade_mass" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_r, "rotor_radius" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_c, "blades_chord" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_a, "lift_slope" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_b, "tip_losses" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_delta_0, "delta_0" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_delta_2, "delta_2" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_ct_max, "ct_max" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_cq_max, "cq_max" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_thrust_factor , "thrust_factor" , true );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_torque_factor , "torque_factor" , true );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_vel_i_factor  , "vel_i_factor"  , true );

        if ( result == FDM_SUCCESS )
        {
            m_r2 = m_r * m_r;
            m_r3 = m_r * m_r2;

            m_ad = M_PI * m_r2;
            m_s = ( (double)m_nb ) * m_c * m_r / m_ad;

            m_i_b = blade_mass * m_r * m_r / 3.0;

            // TODO
            m_bas2ras = Matrix3x3( Angles( cant_angle, 0.0, 0.0 ) );
            m_ras2bas = m_bas2ras.getTransposed();
        }
        else
        {
            Exception e;

            e.setType( Exception::FileReadingError );
            e.setInfo( "Reading XML file failed. " + XmlUtils::getErrorInfo( dataNode ) );

            FDM_THROW( e );
        }
    }
    else
    {
        Exception e;

        e.setType( Exception::FileReadingError );
        e.setInfo( "Reading XML file failed. " + XmlUtils::getErrorInfo( dataNode ) );

        FDM_THROW( e );
    }
}

////////////////////////////////////////////////////////////////////////////////

void TailRotor::computeForceAndMoment( const Vector3 &vel_air_bas,
                                       const Vector3 &omg_air_bas,
                                       double airDensity,
                                       double collective )
{
    double omega2 = m_omega * m_omega;
    double omegaR = m_omega * m_r;

    // controls
    double theta_0 = collective;

    // velocity transformations
    Vector3 vel_air_ras = m_bas2ras * ( vel_air_bas + ( omg_air_bas ^ m_r_hub_bas ) );

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
    double lambda_i = m_vel_i_bas.getLength() / omegaR;

    if ( fabs( lambda_i ) < 10e-14 ) lambda_i = 10e-14;

    // iteration loopp
    for ( int i = 0; i < 100; i++ )
    {
        lambda = mu_z - lambda_i;

        // thrust coefficient
        ct = 0.5 * m_a * m_s  * m_b * ( lambda * m_b / 2.0 + theta_0 * ( m_b + 1.5 * mu2 ) / 3.0 );
        if ( ct > m_ct_max ) ct = m_ct_max;

        // zero function (Padfield p.124)
        double lambda_d = mu2 + lambda * lambda;
        double g_0 = lambda_i - ct / ( 2.0 * sqrt( lambda_d ) );

        // break condition
        if ( fabs( g_0 ) < 1.0e-6 ) break;

        // (Padfield p.124)
        double h_j = -( 2.0 * lambda_i * sqrt( lambda_d ) - ct ) * lambda_d
                / ( 2*pow( lambda_d, 2.0/3.0 ) + m_a * m_s * lambda_d / 4.0 - ct * lambda );

        // (Padfield p.124)
        double f_j = 1.0;
        if ( fabs( mu_z ) < 0.05 ) f_j = 0.6;

        // (Padfield p.124)
        double lambda_i_new = lambda_i + f_j * h_j;

        if ( Misc::isValid( lambda_i_new ) ) lambda_i = lambda_i_new;
    }

    // drag coefficient
    double cd = m_delta_0 + m_delta_2 * ct*ct;

    // moment of resistance coefficient (Bramwell p.102)
    double cq = cd * m_s * ( 1.0 + 3.0 * mu2 ) / 8.0 - lambda * ct;
    if ( cq > m_cq_max ) cq = m_cq_max;

    // induced velocity
    m_vel_i_bas = m_ras2bas * Vector3( 0.0, 0.0, m_vel_i_factor * lambda_i * omegaR );

    m_thrust = m_thrust_factor * airDensity * m_ad * m_r2 * omega2 * ct;
    m_torque = m_torque_factor * airDensity * m_ad * m_r3 * omega2 * cq;

    m_for_bas = m_ras2bas * Vector3( 0.0, 0.0, -m_thrust );
    m_mom_bas = m_r_hub_bas ^ m_for_bas;

    if ( !m_for_bas.isValid() || !m_mom_bas.isValid() )
    {
        Exception e;

        e.setType( Exception::UnexpectedNaN );
        e.setInfo( "NaN detected in the tail rotor model." );

        FDM_THROW( e );
    }
}

////////////////////////////////////////////////////////////////////////////////

void TailRotor::update( double omega )
{
    m_omega = omega;
}
