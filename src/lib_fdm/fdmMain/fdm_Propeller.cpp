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

#include <fdmMain/fdm_Propeller.h>

#include <fdmSys/fdm_Inertia.h>
#include <fdmUtils/fdm_String.h>
#include <fdmXml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Propeller::Propeller() :
    m_direction ( CW ),
    m_gearRatio ( 1.0 ),
    m_diameter ( 0.0 ),
    m_inertia ( 0.0 ),
    m_pitch ( 0.0 ),
    m_omega ( 0.0 ),
    m_speed_rps ( 0.0 ),
    m_speed_rpm ( 0.0 ),
    m_thrust ( 0.0 ),
    m_torqueAvailable ( 0.0 ),
    m_torqueRequired ( 0.0 )
{}

////////////////////////////////////////////////////////////////////////////////

Propeller::~Propeller() {}

////////////////////////////////////////////////////////////////////////////////

void Propeller::readData( XmlNode &dataNode )
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

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_pos_bas   , "position"   );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_gearRatio , "gear_ratio" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_diameter  , "diameter"   );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_inertia   , "inertia"    );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_propPitch, "pitch" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_coefThrust , "thrust_coef" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_coefPower  , "power_coef"  );

        if ( result != FDM_SUCCESS )
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

void Propeller::computeThrust( double airspeed, double airDensity )
{
    if ( m_speed_rps > 0.0 )
    {
        double advance = airspeed / ( m_diameter * m_speed_rps );
        double coefThrust = m_coefThrust.getValue( advance, m_pitch );

        m_thrust = coefThrust * airDensity * Misc::pow2( m_speed_rps ) * Misc::pow4( m_diameter );
    }
    else
    {
        m_thrust = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////

void Propeller::integrate( double timeStep, double engineInertia )
{
    // integrating propeller omega
    m_omega += ( ( m_torqueAvailable - m_torqueRequired ) / ( m_inertia + engineInertia ) ) * timeStep;

    m_speed_rps = Misc::max( 0.0, m_omega / ( 2.0 * M_PI ) );

    // engine friction stops propeller
    if ( m_torqueAvailable < m_torqueRequired && m_speed_rps < 1.0 )
    {
        m_speed_rps = m_speed_rps < 0.1 ? 0.0 : Inertia::update( 0.0, m_speed_rps, timeStep, 0.1 );
        m_omega = 2.0 * M_PI * m_speed_rps;
    }

    m_speed_rpm = 60.0 * m_speed_rps;
}

////////////////////////////////////////////////////////////////////////////////

void Propeller::update( double propellerLever, double engineTorque,
                        double airspeed, double airDensity )
{
    m_pitch = m_propPitch.getValue( propellerLever );

    double advance = airspeed / ( m_diameter * ( m_speed_rps > 0.1 ? m_speed_rps : 0.1 ) );
    double coefPower = m_coefPower.getValue( advance, m_pitch );
    double powerRequired = coefPower * airDensity * Misc::pow3( m_speed_rps ) * Misc::pow5( m_diameter );

    m_torqueRequired  = powerRequired / ( m_omega > 1.0 ? m_omega : 1.0 );
    m_torqueAvailable = engineTorque / m_gearRatio;
}

////////////////////////////////////////////////////////////////////////////////

void Propeller::setRPM( double rpm )
{
    m_speed_rpm = Misc::max( 0.0, rpm );
    m_speed_rps = m_speed_rpm / 60.0;
    m_omega = 2.0 * M_PI * m_speed_rps;
}
