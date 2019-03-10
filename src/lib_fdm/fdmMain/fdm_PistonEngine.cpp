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

#include <fdmMain/fdm_PistonEngine.h>

#include <fdmUtils/fdm_Units.h>
#include <fdmXml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

PistonEngine::PistonEngine() :
    m_power_max ( 0.0 ),
    m_rpm_min ( 0.0 ),
    m_rpm_max ( 0.0 ),
    m_specFuelCons ( 0.0 ),
    m_inertia ( 0.0 ),
    m_map ( 0.0 ),
    m_power ( 0.0 ),
    m_torque ( 0.0 ),
    m_fuelFlow ( 0.0 )
{}

////////////////////////////////////////////////////////////////////////////////

PistonEngine::~PistonEngine() {}

////////////////////////////////////////////////////////////////////////////////

void PistonEngine::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_power_max    , "power_max"    );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_starter      , "starter"      );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_rpm_min      , "rpm_min"      );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_rpm_max      , "rpm_max"      );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_specFuelCons , "sfc"          );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_inertia      , "inertia"      );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_mixture      , "mixture"      );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_throttle     , "throttle"     );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_powerFactor  , "power_factor" );

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

void PistonEngine::update( double throttleLever, double mixtureLever, double rpm,
                           double airPressure, double airDensity,
                           bool fuel, bool starter,
                           bool magneto_l, bool magneto_r )
{
    double omega = M_PI * rpm / 30.0;

    m_rpm = rpm;
    m_map = getManifoldAbsolutePressure( m_throttle.getValue( throttleLever ), m_rpm, airPressure );

    double powerFactor = getPowerFactor( m_mixture.getValue( mixtureLever ), airDensity, fuel, magneto_l, magneto_r );
    double staticPower = getStaticPower( m_rpm, m_map );
    double powerLosses = getPowerLosses( m_rpm );
    double termalPower = staticPower * powerFactor;

    if ( m_rpm < m_rpm_min ) termalPower = 0.0;

    // net power [W]
    // Allerton D.: Principles of Flight Simulation, p.130
    m_power = termalPower - powerLosses;

    m_fuelFlow = Misc::max( 0.0, m_power ) * m_specFuelCons;

    // engine torque [N*m]
    m_torque = ( omega > 1.0 ) ? m_power / omega : m_power;

    // state
    if ( m_power > 0.0 || ( m_rpm > m_rpm_min && fuel && ( magneto_l || magneto_r ) ) )
    {
        m_state = Running;
    }
    else
    {
        m_state = Stopped;

        if ( starter )
        {
            m_state = Starting;
            m_torque += m_starter;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

double PistonEngine::getManifoldAbsolutePressure( double throttle, double rpm,
                                                  double airPressure )
{
    // Allerton D.: Principles of Flight Simulation, p.129
    double map = airPressure + ( 156.9411 * throttle - 158.8034 ) * rpm;
    map = Misc::max( 0.0, map );

    return map;
}

////////////////////////////////////////////////////////////////////////////////

double PistonEngine::getPowerFactor( double mixture, double airDensity, bool fuel,
                                     bool magneto_l, bool magneto_r )
{
    // Allerton D.: Principles of Flight Simulation, p.130
    double fuelToAirRatio = mixture * (1.225 / airDensity ) * 0.1;

    // Allerton D.: Principles of Flight Simulation, p.130
    double powerFactor = m_powerFactor.getValue( fuelToAirRatio );

    if ( !fuel )
    {
        // no fuel - engine starving
        powerFactor = 0.0;
    }
    else if ( !magneto_l && !magneto_r )
    {
        // both magnetos disabled
        powerFactor = 0.0;
    }
    else if ( (  magneto_l && !magneto_r )
           || ( !magneto_l &&  magneto_r ) )
    {
        // 5% reduction in power caused by the reduced effectiveness of the combustion
        // Allerton D.: Principles of Flight Simulation, p.131
        powerFactor *= 0.95;
    }

    powerFactor = Misc::satur( 0.0, 1.0, powerFactor );

    return powerFactor;
}

////////////////////////////////////////////////////////////////////////////////

double PistonEngine::getPowerLosses( double rpm )
{
    // Power losses of a 160 HP (horsepower) engine proportional to the square
    // of RPM are given by:
    // fpow = 0.0413 * n^2 / n_max
    // Allerton D.: Principles of Flight Simulation, p.130
    // 1/160 = 0.00625
    // 0.00625 * 0.0413 = 0.000258125
    double powerLosses = m_power_max * 2.58125e-4 * ( rpm * rpm ) / m_rpm_max;
    powerLosses = Misc::max( 0.0, powerLosses );

    return powerLosses;
}

////////////////////////////////////////////////////////////////////////////////

double PistonEngine::getStaticPower( double rpm, double map )
{
    // For a model of a 160 HP (horsepower) engine, the static horsepower shp
    // (the power produced by combustion) is computed as:
    // shp = mp * (0.0039 * n - 1)
    // Allerton D.: Principles of Flight Simulation, p.130
    // 1/160 = 0.00625
    // 0.00625 * 0.0039 =
    //double staticPower = m_power_max * 0.00625 * Units::pa2inhg( map ) * ( 0.0039 * rpm - 1.0 );
    double staticPower = m_power_max * map * ( 7.198759595625e-9 * rpm - 1.84583579375e-6 );
    staticPower = Misc::max( 0.0, staticPower );

    return staticPower;
}
