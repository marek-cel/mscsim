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

#include <fdmMain/fdm_MainRotor.h>
#include <fdmMain/fdm_Aerodynamics.h>

#include <fdmUtils/fdm_String.h>

#include <fdmXml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

MainRotor::MainRotor() :
    m_direction ( CW ),

    m_nb ( 0 ),

    m_r ( 0.0 ),
    m_c ( 0.0 ),
    m_e ( 0.0 ),
    m_a ( 0.0 ),
    m_b ( 0.0 ),

    m_beta_max ( 0.0 ),

    m_r2  ( 0.0 ),
    m_r3  ( 0.0 ),
    m_r4  ( 0.0 ),
    m_b2  ( 0.0 ),
    m_b3  ( 0.0 ),
    m_b4  ( 0.0 ),
    m_ad  ( 0.0 ),
    m_s   ( 0.0 ),
    m_s_b ( 0.0 ),
    m_i_b ( 0.0 ),

    m_omega ( 0.0 ),

    m_beta_0      ( 0.0 ),
    m_beta_1c_ras ( 0.0 ),
    m_beta_1s_ras ( 0.0 ),

    m_rotorDiskRoll  ( 0.0 ),
    m_rotorDiskPitch ( 0.0 ),

    m_thrust ( 0.0 ),
    m_torque ( 0.0 )
{
    m_bas2ras = Matrix3x3::createIdentityMatrix();
    m_ras2bas = Matrix3x3::createIdentityMatrix();
    m_ras2cas = Matrix3x3::createIdentityMatrix();
    m_cas2ras = Matrix3x3::createIdentityMatrix();
    m_cas2was = Matrix3x3::createIdentityMatrix();
    m_was2cas = Matrix3x3::createIdentityMatrix();
    m_bas2cas = Matrix3x3::createIdentityMatrix();
    m_bas2was = Matrix3x3::createIdentityMatrix();
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
            m_direction = CCW;
        }
        else
        {
            m_direction = CW;
        }

        double inclination = 0.0;
        double blade_mass  = 0.0;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_r_hub_bas, "hub_center" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, inclination, "inclination" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_nb, "number_of_blades" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, blade_mass, "blade_mass" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_r, "rotor_radius" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_c, "blades_chord" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_e, "hinge_offset" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_a, "lift_slope" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_b, "tip_losses" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_beta_max, "max_flapping" );

        if ( result == FDM_SUCCESS )
        {
            m_r2 = m_r * m_r;
            m_r3 = m_r * m_r2;
            m_r4 = m_r * m_r3;

            m_b2 = m_b * m_b;
            m_b3 = m_b * m_b2;
            m_b4 = m_b * m_b3;

            m_ad = M_PI * m_r2;
            m_s = ( (double)m_nb ) * m_c * m_r / m_ad;

            m_s_b = blade_mass * m_r / 2.0;
            m_i_b = blade_mass * m_r * m_r / 3.0;

            m_bas2ras = Matrix3x3( Angles( 0.0, -inclination, 0.0 ) );
            m_ras2bas = m_bas2ras.getTransposed();
        }
        else
        {
            Exception e;

            e.setType( Exception::FileReadingError );
            e.setInfo( "ERROR! Reading XML file failed. " + XmlUtils::getErrorInfo( dataNode ) );

            FDM_THROW( e );
        }
    }
    else
    {
        Exception e;

        e.setType( Exception::FileReadingError );
        e.setInfo( "ERROR! Reading XML file failed. " + XmlUtils::getErrorInfo( dataNode ) );

        FDM_THROW( e );
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
    double omega2 = m_omega * m_omega;
    double omegaR = m_omega * m_r;

    // Lock number
    double gamma = airDensity * m_a * m_c * m_r4 / m_i_b;

    // controls
    double theta_0      = collective;
    double theta_1c_ras = m_direction == CW ? cyclicLat : -cyclicLat;
    double theta_1s_ras = cyclicLon;

    // RAS <-> CAS
    m_ras2cas = Matrix3x3( Angles( cyclicLat, cyclicLon, 0.0 ) );

    m_cas2ras = m_ras2cas.getTransposed();

    // BAS -> CAS
    m_bas2cas = m_bas2ras * m_ras2cas;

    // velocity transformations
    Vector3 vel_air_cas = m_bas2cas * ( vel_air_bas + ( omg_air_bas ^ m_r_hub_bas ) );

    // sideslip angle
    double beta = Aerodynamics::getSideslipAngle( vel_air_cas );

    // CAS <-> WAS
    m_cas2was = Matrix3x3( Angles( 0.0, 0.0, beta ) );
    m_was2cas = m_cas2was.getTransposed();

    // BAS -> WAS
    m_bas2was = m_bas2cas * m_cas2was;

    // velocity transformations
    Vector3 vel_air_was = m_cas2was * vel_air_cas;
    Vector3 omg_air_was = m_bas2was * omg_air_bas;

    // acceleration
    Vector3 acc_hub_bas = acc_bas + ( omg_bas ^ vel_bas )
                        + ( omg_bas ^ ( omg_bas ^ m_r_hub_bas ) )
                        + ( eps_bas ^ m_r_hub_bas )
                        - grav_bas;
    Vector3 acc_hub_was = m_bas2was * acc_hub_bas;

    // angle of attack
    double alpha = Aerodynamics::getAngleOfAttack( omg_air_was );

    // induced velocity
    double beta_1c_was = 0.0;
    double beta_1s_was = 0.0;

    double lambda = 0.0;

    double p = omg_air_was( i_p );
    double q = omg_air_was( i_q );

    double airspeed = vel_air_was.getLength();

    // rotor advance ratio
    double mu   = airspeed * cos( alpha ) / omegaR;
    double mu2  = mu * mu;
    double mu_z = airspeed * sin( alpha ) / omegaR;

    double a_z = -acc_hub_was.z();

    // thrust coefficient
    double ct = 0.0;

    // rotor inflow
    double lambda_i = m_vel_i_bas.getLength() / omegaR;

    if ( fabs( lambda_i ) < 10e-14 ) lambda_i = 10e-14;

    // iteration loop
    for ( int i = 0; i < 100; i++ )
    {
        lambda = mu_z - lambda_i;

        // flapping coefficients
        if ( m_direction == CW )
        {
            m_beta_0 = ( gamma / 2.0 ) * ( m_b3 * lambda / 3.0 - m_b3 * p * mu / ( 6.0 * m_omega ) + m_b4 * theta_0 / 4.0 + m_b2 * theta_0 * mu2 / 4.0 )
                     - a_z * m_s_b / ( m_i_b * omega2 );

            beta_1c_was = 2.0 * mu * ( lambda + 4.0 * m_b * theta_0 / 3.0 ) / ( mu2 / 2.0 - m_b2 )
                    - ( m_b4 * p / m_omega + 16.0 * q / ( gamma * m_omega ) ) / ( m_b2 * ( mu2 / 2.0 - m_b2 ) );

            beta_1s_was = -4.0 * m_beta_0 * mu * m_b / ( mu2 / 2.0 + m_b2 ) / 3.0
                    + ( m_b4 * q / m_omega - 16.0 * p / ( gamma * m_omega ) ) / ( m_b2 * ( mu2 / 2.0 + m_b2 ) );
        }
        else
        {
            m_beta_0 = 0.0;

            beta_1c_was = 0.0;

            beta_1s_was = 0.0;
        }

        // limits
        m_beta_0    = Misc::satur( -m_beta_max, m_beta_max, m_beta_0    );
        beta_1c_was = Misc::satur( -m_beta_max, m_beta_max, beta_1c_was );
        beta_1s_was = Misc::satur( -m_beta_max, m_beta_max, beta_1s_was );

        // thrust coefficient
        ct = 0.5 * m_a * m_s * m_b * ( lambda * m_b / 2.0
                                     + theta_0 * ( m_b2 + 1.5 * mu2 ) / 3.0
                                     + m_b * mu * beta_1c_was / 4.0 );

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
        double lambda_i_new = lambda_i + f_j*h_j;

        if ( Misc::isValid( lambda_i_new ) ) lambda_i = lambda_i_new;
    }

    // drag coefficient
    double delta_0 = 0.001;
    double delta_2 = 0.001;
    double cd = delta_0 + delta_2 * ct*ct;

    // moment of resistance coefficient (Bramwell p.102)
    double cq = cd * m_s * ( 1.0 + 3.0 * mu2 ) / 8.0 - lambda * ct;

    // flapping in axes with sideslip
    double cosBeta = cos( beta );
    double sinBeta = sin( beta );

    double beta_1c_cas = beta_1c_was * cosBeta - beta_1s_was * sinBeta * ( m_direction == CW ? 1.0 : -1.0 );
    double beta_1s_cas = beta_1s_was * cosBeta + beta_1c_was * sinBeta * ( m_direction == CW ? 1.0 : -1.0 );

    // flapping coefficients
    m_beta_1c_ras = Misc::satur( -m_beta_max, m_beta_max, beta_1c_cas - theta_1s_ras );
    m_beta_1s_ras = Misc::satur( -m_beta_max, m_beta_max, beta_1s_cas + theta_1c_ras );

    m_rotorDiskPitch = -m_beta_1c_ras;
    m_rotorDiskRoll  = m_direction == CW ? m_beta_1s_ras : -m_beta_1s_ras;

    // DAS -> BAS
    m_das2bas = Matrix3x3( Angles( m_rotorDiskRoll, m_rotorDiskPitch, 0.0 ) ).getTransposed() * m_ras2bas;

    // induced velocity
    m_vel_i_bas = m_das2bas * Vector3( 0.0, 0.0, lambda_i * omegaR );

    m_thrust = airDensity * m_ad * m_r2 * omega2 * ct;
    m_torque = airDensity * m_ad * m_r3 * omega2 * cq;

    m_for_bas = m_das2bas * Vector3( 0.0, 0.0, -m_thrust );
    m_mom_bas = m_ras2bas * Vector3( 0.0, 0.0, m_direction == CW ? -m_torque : m_torque )
            + ( m_r_hub_bas ^ m_for_bas );

    if ( !m_for_bas.isValid() || !m_mom_bas.isValid() )
    {
        Exception e;

        e.setType( Exception::UnexpectedNaN );
        e.setInfo( "ERROR! NaN detected in the main rotor model." );

        FDM_THROW( e );
    }
}

