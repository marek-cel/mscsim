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

#include <fdm/models/fdm_PistonEngine.h>

#include <fdm/utils/fdm_Units.h>
#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

PistonEngine::PistonEngine() :
    _rpm_min      ( 0.0 ),
    _rpm_max      ( 0.0 ),
    _starter      ( 0.0 ),
    _displacement ( 0.0 ),
    _inertia      ( 0.0 ),
    _specFuelCons ( 0.0 ),

    _rpm      ( 0.0 ),
    _map      ( 0.0 ),
    _power    ( 0.0 ),
    _torque   ( 0.0 ),
    _airFlow  ( 0.0 ),
    _fuelFlow ( 0.0 )
{}

////////////////////////////////////////////////////////////////////////////////

PistonEngine::~PistonEngine() {}

////////////////////////////////////////////////////////////////////////////////

void PistonEngine::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _rpm_min      , "rpm_min"      );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _rpm_max      , "rpm_max"      );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _starter      , "starter"      );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _displacement , "displacement" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _inertia      , "inertia"      );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _specFuelCons , "sfc"          );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _power_rpm      , "power_rpm"      );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _power_throttle , "power_throttle" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _power_altitude , "power_altitude" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _mixture        , "mixture"        );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _power_factor   , "power_factor"   );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _map_throttle   , "map_throttle"   );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _map_rpm        , "map_rpm"        );

        if ( result != FDM_SUCCESS ) XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
    else
    {
        XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
}

////////////////////////////////////////////////////////////////////////////////

void PistonEngine::update( double throttleLever,
                           double mixtureLever,
                           double rpm,
                           double airPressure,
                           double airDensity,
                           double densityAlt,
                           bool fuel,
                           bool starter,
                           bool magneto_l,
                           bool magneto_r )
{
    double omega = M_PI * rpm / 30.0;

    _rpm = rpm;
    _map = getManifoldAbsolutePressure( throttleLever, _rpm, airPressure );
    _power = getNetPower( throttleLever, mixtureLever, _rpm, airDensity, densityAlt,
                          fuel, magneto_l, magneto_r );

    _airFlow = 0.5 * _displacement * airDensity * ( _rpm / 60.0 );
    _fuelFlow = Misc::max( 0.0, _power ) * _specFuelCons;

    // engine torque [N*m]
    _torque = ( omega > 1.0 ) ? _power / omega : _power;

    // state
    if ( _power > 0.0 || ( _rpm > _rpm_min && fuel && ( magneto_l || magneto_r ) ) )
    {
        _state = Running;
    }
    else
    {
        _state = Stopped;

        if ( starter )
        {
            _state = Starting;
            _torque += _starter;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void PistonEngine::setRPM( double rpm )
{
    _rpm = Misc::max( 0.0, rpm );
}

////////////////////////////////////////////////////////////////////////////////

double PistonEngine::getManifoldAbsolutePressure( double throttleLever,
                                                  double rpm, double airPressure )
{
    double map = airPressure
            * _map_throttle.getValue( throttleLever )
            * _map_rpm.getValue( rpm );

    map = Misc::max( 0.0, map );

    return map;
}

////////////////////////////////////////////////////////////////////////////////

double PistonEngine::getFuelToAirRatio( double mixture, double airDensity )
{
    return mixture * (1.225 / airDensity );
}

////////////////////////////////////////////////////////////////////////////////

double PistonEngine::getPowerFactor( double mixture, double airDensity, bool fuel,
                                     bool magneto_l, bool magneto_r )
{
    double fuelToAirRatio = getFuelToAirRatio( mixture, airDensity );

    // Allerton D.: Principles of Flight Simulation, p.130
    double powerFactor = _power_factor.getValue( fuelToAirRatio );

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
    else if ( ( magneto_l && !magneto_r ) || ( !magneto_l && magneto_r ) )
    {
        // 5% reduction in power caused by the reduced effectiveness of the combustion
        // Allerton D.: Principles of Flight Simulation, p.131
        powerFactor *= 0.95;
    }

    powerFactor = Misc::satur( 0.0, 1.0, powerFactor );

    return powerFactor;
}

////////////////////////////////////////////////////////////////////////////////

double PistonEngine::getNetPower( double throttleLever, double mixtureLever, double rpm,
                                  double airDensity, double densityAltitude,
                                  bool fuel, bool magneto_l, bool magneto_r )
{
    double power = _power_rpm.getValue( rpm );
    power *= _power_throttle.getValue( throttleLever );
    power *= _power_altitude.getValue( densityAltitude );
    power *= getPowerFactor( _mixture.getValue( mixtureLever ), airDensity,
                             fuel, magneto_l, magneto_r );

    if ( rpm < _rpm_min ) power = 0.0;

    return power;
}
