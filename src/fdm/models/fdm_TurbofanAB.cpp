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

#include <fdm/models/fdm_TurbofanAB.h>

#include <fdm/utils/fdm_Misc.h>
#include <fdm/utils/fdm_Units.h>
#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

TurbofanAB::TurbofanAB() :
    m_thrust_mil ( 0.0 ),
    m_thrust_ab  ( 0.0 ),

    m_tc_tit ( 1.0 ),

    m_tsfc    ( 0.0 ),
    m_tsfc_ab ( 0.0 ),

    m_n1_idle ( 0.0 ),
    m_n2_idle ( 0.0 ),
    m_n1_ab   ( 0.0 ),
    m_n2_ab   ( 0.0 ),
    m_n1_max  ( 0.0 ),
    m_n2_max  ( 0.0 ),

    m_n1_setpoint ( 0.0 ),
    m_n2_setpoint ( 0.0 ),

    m_tit_setpoint ( 0.0 ),

    m_pow_command ( 0.0 ),
    m_pow ( 0.0 ),

    m_thrust_tc_inv ( 1.0 ),
    m_tit_tc_actual ( 1.0 ),

    m_temperature ( 15.0 ),

    m_n1 ( 0.0 ),
    m_n2 ( 0.0 ),
    m_tit ( 0.0 ),
    m_fuelFlow ( 0.0 ),
    m_thrust ( 0.0 ),

    m_afterburner ( false )
{}

////////////////////////////////////////////////////////////////////////////////

TurbofanAB::~TurbofanAB() {}

////////////////////////////////////////////////////////////////////////////////

void TurbofanAB::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        m_n1_idle = 0.0;
        m_n2_idle = 0.0;
        m_n1_ab   = 0.0;
        m_n2_ab   = 0.0;
        m_n1_max  = 0.0;
        m_n2_max  = 0.0;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_pos_bas, "position" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_thrust_mil , "thrust_mil" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_thrust_ab  , "thrust_ab"  );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_ab_threshold, "ab_threshold" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_tc_n1, "time_constant_n1" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_tc_n2, "time_constant_n2" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_tc_thrust, "time_constant_thrust" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_tc_tit, "time_constant_tit" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_tsfc    , "tsfc"    );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_tsfc_ab , "tsfc_ab" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_n1_throttle, "n1_throttle" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_n2_throttle, "n2_throttle" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_tit_n2, "tit_n2" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_tf_idle , "thrust_factor_idle" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_tf_mil  , "thrust_factor_mil"  );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_tf_ab   , "thrust_factor_ab"   );

        if ( result == FDM_SUCCESS )
        {
            m_n1_idle = m_n1_throttle.getValueMin();
            m_n2_idle = m_n2_throttle.getValueMin();

            m_n1_ab = m_n1_throttle.getValue( m_ab_threshold );
            m_n2_ab = m_n2_throttle.getValue( m_ab_threshold );

            m_n1_max = m_n1_throttle.getValueMax();
            m_n2_max = m_n2_throttle.getValueMax();
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

void TurbofanAB::initialize( bool engineOn )
{
    if ( engineOn )
    {
        m_n1 = m_n1_setpoint = m_n1_idle;
        m_n2 = m_n2_setpoint = m_n2_idle;

        m_tit = m_tit_n2.getValue( m_n2_idle );
    }
    else
    {
        m_n1 = m_n1_setpoint = 0.0;
        m_n2 = m_n2_setpoint = 0.0;

        m_tit = m_temperature;
    }
}

////////////////////////////////////////////////////////////////////////////////

void TurbofanAB::computeThrust( double machNumber, double densityAltitude )
{
    if ( m_state == Running )
    {
        // (NASA-TP-1538, p.218)
        // (NASA-TN-D-8176, p.152)
        if ( m_afterburner )
        {
            double t_mil = m_thrust_mil * m_tf_mil .getValue( machNumber, densityAltitude );
            double t_ab  = m_thrust_ab  * m_tf_ab  .getValue( machNumber, densityAltitude );

            m_thrust = t_mil + ( t_ab - t_mil ) * ( m_pow - 0.5 ) / 0.5;
        }
        else
        {
            double t_idle = m_thrust_mil * m_tf_idle .getValue( machNumber, densityAltitude );
            double t_mil  = m_thrust_mil * m_tf_mil  .getValue( machNumber, densityAltitude );

            m_thrust = t_idle + ( t_mil - t_idle ) * std::min( 0.5, m_pow ) / 0.5;
        }
    }
    else
    {
        m_thrust = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////

void TurbofanAB::integrate( double timeStep )
{
    double pow_deriv = m_thrust_tc_inv * ( m_pow_command - m_pow );
    m_pow = m_pow + timeStep * pow_deriv;

    double epsilon_n1 = 0.01 * ( m_n1 < m_n1_idle ? m_n1_idle : m_n1_max );
    double epsilon_n2 = 0.01 * ( m_n2 < m_n2_idle ? m_n2_idle : m_n2_max );

    double n1_tc = m_tc_n1.getValue( m_n1_setpoint - m_n1 );
    double n2_tc = m_tc_n1.getValue( m_n2_setpoint - m_n2 );

    m_n1 = fabs( m_n1 - m_n1_setpoint ) < epsilon_n1 ? m_n1_setpoint : Misc::inertia( m_n1_setpoint, m_n1, timeStep, n1_tc );
    m_n2 = fabs( m_n2 - m_n2_setpoint ) < epsilon_n2 ? m_n2_setpoint : Misc::inertia( m_n2_setpoint, m_n2, timeStep, n2_tc );

    m_tit = Misc::inertia( m_tit_setpoint, m_tit, timeStep, m_tit_tc_actual );
}

////////////////////////////////////////////////////////////////////////////////

void TurbofanAB::update( double throttle, double temperature,
                         double machNumber, double densityAltitude,
                         bool fuel, bool starter )
{
    computeThrust( machNumber, densityAltitude );

    m_temperature = temperature;

    if ( m_n1 >= m_n1_idle && m_n2 >= m_n2_idle && fuel )
    {
        m_state = Running;

        if ( throttle < m_ab_threshold )
        {
            // military
            if ( m_pow < 0.5 )
            {
                m_pow_command = 0.5 * throttle / m_ab_threshold;
                m_thrust_tc_inv = 1.0 / m_tc_thrust.getValue( m_pow_command - m_pow );

                m_n1_setpoint = m_n1_throttle.getValue( throttle );
                m_n2_setpoint = m_n2_throttle.getValue( throttle );
            }
            else
            {
                m_pow_command = 0.4;
                m_thrust_tc_inv = 5.0;

                m_n1_setpoint = m_n1_ab;
                m_n2_setpoint = m_n2_ab;
            }
        }
        else
        {
            // afterburner
            if ( m_pow < 0.5 )
            {
                m_pow_command = 0.6;
                m_thrust_tc_inv = 1.0 / m_tc_thrust.getValue( m_pow_command - m_pow );

                m_n1_setpoint = m_n1 < m_n2_ab ? 1.2 * m_n2_ab : m_n1_throttle.getValue( throttle );
                m_n2_setpoint = m_n2 < m_n2_ab ? 1.2 * m_n2_ab : m_n2_throttle.getValue( throttle );
            }
            else
            {
                m_pow_command = 0.5 + 0.5 * ( throttle - m_ab_threshold ) / ( 1.0 - m_ab_threshold );
                m_thrust_tc_inv = 5.0;

                m_n1_setpoint = m_n1_throttle.getValue( throttle );
                m_n2_setpoint = m_n2_throttle.getValue( throttle );
            }
        }

        m_tit_tc_actual = m_tc_tit;
        m_tit_setpoint = m_tit_n2.getValue( m_n2 );

        m_afterburner = m_pow >= 0.5 && m_n1 >= m_n1_ab && m_n2 >= m_n2_ab;

        if ( m_afterburner )
        {
            m_fuelFlow = m_thrust * m_tsfc_ab;
        }
        else
        {
            m_fuelFlow = m_thrust * m_tsfc;
        }
    }
    else
    {
        if ( starter )
        {
            m_state = Starting;

            m_pow_command = 0.0; // TODO

            m_n1_setpoint = 1.2 * m_n1_idle;
            m_n2_setpoint = 1.2 * m_n2_idle;

            m_tit_setpoint = m_temperature; // TODO

            m_fuelFlow = 0.0; // TODO
        }
        else
        {
            m_state = Stopped;

            m_pow_command = 0.0;
            m_pow = 0.0;

            m_n1_setpoint = 0.0;
            m_n2_setpoint = 0.0;

            m_tit_setpoint = m_temperature;

            m_fuelFlow = 0.0;
        }

        m_tit_tc_actual = 20.0;

        m_afterburner = false;
    }
}
