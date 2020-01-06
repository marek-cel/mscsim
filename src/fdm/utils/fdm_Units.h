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
#ifndef FDM_UNITS_H
#define FDM_UNITS_H

////////////////////////////////////////////////////////////////////////////////

#include <cmath>
#include <string>

#include <fdm/fdm_Defines.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Units conversion class.
 *
 * @see The International System of Units: Physical Constants and Conversion Factors, NASA-SP-7012
 */
class FDMEXPORT Units
{
public:

    typedef double (*fptr)(double);

    /**
     * @brief Returns converter function pointer.
     *
     * @param name name of the unit
     * @return conversion function pointer
     *
     * <h5>SI units of certain values types</h5>
     * <ul>
     * <li>Angle: radian [rad]</li>
     * <li>Length: meter [m]</li>
     * <li>Velocity: meter per second [m/s]</li>
     * <li>Angular Velocity: radian per second [rad/s]</li>
     * <li>Mass: kilogram [kg]</li>
     * <li>Force: newton [N]</li>
     * <li>Pressure: pascal [Pa]</li>
     * <li>Power: watt [W]</li>
     * <li>Temperature: kelvin [K]</li>
     * <li>Specific fuel consumption: kilograms per watt-second [kg/(W*s)]</li>
     * <li>Thrust Specific fuel consumption: kilograms per newton-second [kg/(N*s)]</li>
     * </ul>
     *
     * <h5>Unit names of certain values types</h5>
     * <ul>
     * <li>Angle: deg (degree)</li>
     * <li>Length: ft (feet), km (kilometer), nmi (nautical mile)</li>
     * <li>Volume: cuin (cubic inches), l (litres)
     * <li>Velocity: fpm (feet per minute), fps (feet per second), kmh (kilemeter per hour), kts (knots)</li>
     * <li>Angular Velocity: deg_s (degrees per second), rpm (revolutions per minute)</li>
     * <li>Mass: lb (pound)</li>
     * <li>Force: lbf (pound of force)</li>
     * <li>Pressure: psi (pound per square inch), inHg (inch of mercury), mb (millibar)</li>
     * <li>Power: PS (metric horsepower), hp (horsepower), kW (kilowatt)</li>
     * <li>Temperature: degC (degree Celsius), degF (degree Fahrenheit)</li>
     * <li>Specific fuel consumption: g_kWh (grams per kilowatt-hour)</li>
     * <li>Thrust Specific fuel consumption: g_kNs (grams per kilonewton-socond)</li>
     * </ul>
     */
    static fptr getConverter( const char *name );

    /**
     * Converts given angle from degrees to radians.
     * @param ang angle expressed in degrees
     * @return angle expressed in radians
     */
    FDM_CONSTEXPR static inline double deg2rad( double ang = 1.0 )
    {
        return ang * M_PI / 180.0;
    }

    /**
     * Converts given angle from radians to degrees.
     * @param ang angle expressed in radians
     * @return angle expressed in degrees
     */
    FDM_CONSTEXPR static inline double rad2deg( double ang = 1.0 )
    {
        return ang * 180.0 / M_PI;
    }

    /**
     * Converts given length from metres to feet.
     * @param len length expressed in metres
     * @return length expressed in feet
     */
    FDM_CONSTEXPR static inline double m2ft( double len = 1.0 )
    {
        return len * 3.2808399;
    }

    /**
     * Converts given length from metres to miles.
     * @param len length expressed in metres
     * @return length expressed in miles
     */
    FDM_CONSTEXPR static inline double m2mi( double len = 1.0 )
    {
        return len * 0.000621371192;
    }

    /**
     * Converts given length from metres to nautical miles.
     * @param len length expressed in metres
     * @return length expressed in nautical miles
     */
    FDM_CONSTEXPR static inline double m2nmi( double len = 1.0 )
    {
        return len * 0.000539956803;
    }

    /**
     * Converts given length from metres to kilometres.
     * @param len length expressed in metres
     * @return length expressed in kilometres
     */
    FDM_CONSTEXPR static inline double m2km( double len = 1.0 )
    {
        return len * 0.001;
    }

    /**
     * Converts given length from feet to metres.
     * @param len length expressed in feet
     * @return length expressed in metres
     */
    FDM_CONSTEXPR static inline double ft2m( double len = 1.0 )
    {
        return len * 0.3048;
    }

    /**
     * Converts given length from feet to kilometres.
     * @param len length expressed in feet
     * @return length expressed in kilometres
     */
    FDM_CONSTEXPR static inline double ft2km( double len = 1.0 )
    {
        return len * 0.0003048;
    }

    /**
     * Converts given length from feet to miles.
     * @param len length expressed in feet
     * @return length expressed in miles
     */
    FDM_CONSTEXPR static inline double ft2mi( double len = 1.0 )
    {
        return len * 0.000189393939;
    }

    /**
     * Converts given length from feet to nautical miles.
     * @param len length expressed in feet
     * @return length expressed in nautical miles
     */
    FDM_CONSTEXPR static inline double ft2nmi( double len = 1.0 )
    {
        return len * 0.000164578834;
    }

    /**
     * Converts given length from kilometres to metres.
     * @param len length expressed in kilometres
     * @return length expressed in metres
     */
    FDM_CONSTEXPR static inline double km2m( double len = 1.0 )
    {
        return len * 1000.0;
    }

    /**
     * Converts given length from kilometres to feet.
     * @param len length expressed in kilometres
     * @return length expressed in feet
     */
    FDM_CONSTEXPR static inline double km2ft( double len = 1.0 )
    {
        return len * 3280.8399;
    }

    /**
     * Converts given length from kilometres to miles.
     * @param len length expressed in kilometres
     * @return length expressed in miles
     */
    FDM_CONSTEXPR static inline double km2mi( double len = 1.0 )
    {
        return len * 0.621371192;
    }

    /**
     * Converts given length from kilometres to nautical miles.
     * @param len length expressed in kilometres
     * @return length expressed in nautical miles
     */
    FDM_CONSTEXPR static inline double km2nmi( double len = 1.0 )
    {
        return len * 0.539956803;
    }

    /**
     * Converts given length from miles to metres.
     * @param len length expressed in nautical miles
     * @return length expressed in metres
     */
    FDM_CONSTEXPR static inline double mi2m( double len = 1.0 )
    {
        return len * 1609.344;
    }

    /**
     * Converts given length from miles to feet.
     * @param len length expressed in nautical miles
     * @return length expressed in feet
     */
    FDM_CONSTEXPR static inline double mi2ft( double len = 1.0 )
    {
        return len * 5280.0;
    }

    /**
     * Converts given length from miles to kilometres.
     * @param len length expressed in nautical miles
     * @return length expressed in kilometres
     */
    FDM_CONSTEXPR static inline double mi2km( double len = 1.0 )
    {
        return len * 1.609344;
    }

    /**
     * Converts given length from miles to nautical miles.
     * @param len length expressed in miles
     * @return length expressed in nautical miles
     */
    FDM_CONSTEXPR static inline double mi2nmi( double len = 1.0 )
    {
        return len * 0.868976242;
    }

    /**
     * Converts given length from nautical miles to metres.
     * @param len length expressed in nautical miles
     * @return length expressed in metres
     */
    FDM_CONSTEXPR static inline double nmi2m( double len = 1.0 )
    {
        return len * 1852.0;
    }

    /**
     * Converts given length from nautical miles to feet.
     * @param len length expressed in nautical miles
     * @return length expressed in feet
     */
    FDM_CONSTEXPR static inline double nmi2ft( double len = 1.0 )
    {
        return len * 6076.11549;
    }

    /**
     * Converts given length from nautical miles to kilometres.
     * @param len length expressed in nautical miles
     * @return length expressed in kilometres
     */
    FDM_CONSTEXPR static inline double nmi2km( double len = 1.0 )
    {
        return len * 1.852;
    }

    /**
     * Converts given length from nautical miles to miles.
     * @param len length expressed in nautical miles
     * @return length expressed in miles
     */
    FDM_CONSTEXPR static inline double nmi2mi( double len = 1.0 )
    {
        return len * 1.15077945;
    }

    /**
     * Converts given volume from cubic inches to cubic metres.
     * @param vol volume expressed in cubic inches
     * @return volume expressed in cubic metres
     */
    FDM_CONSTEXPR static inline double cuin2m3( double vol = 1.0 )
    {
        return vol * 0.000016387064;
    }

    /**
     * Converts given volume from litres to cubic metres.
     * @param vol volume expressed in litress
     * @return volume expressed in cubic metres
     */
    FDM_CONSTEXPR static inline double l2m3( double vol = 1.0 )
    {
        return vol / 1000.0;
    }

    /**
     * Converts given velocity from m/s to ft/min.
     * @param vel velocity expressed in m/s
     * @return velocity expressed in ft/min
     */
    FDM_CONSTEXPR static inline double mps2fpm( double vel = 1.0 )
    {
        return vel * 196.850394;
    }

    /**
     * Converts given velocity from m/s to ft/s.
     * @param vel velocity expressed in m/s
     * @return velocity expressed in ft/s
     */
    FDM_CONSTEXPR static inline double mps2fps( double vel = 1.0 )
    {
        return vel * 3.2808399;
    }

    /**
     * Converts given velocity from m/s to km/h.
     * @param vel velocity expressed in m/s
     * @return velocity expressed in km/h
     */
    FDM_CONSTEXPR static inline double mps2kmh( double vel = 1.0 )
    {
        return vel * 3.6;
    }

    /**
     * Converts given velocity from m/s to knots.
     * @param vel velocity expressed in m/s
     * @return velocity expressed in knots
     */
    FDM_CONSTEXPR static inline double mps2kts( double vel = 1.0 )
    {
        return vel * 1.943844491;
    }

    /**
     * Converts given velocity from m/s to miles per hour.
     * @param vel velocity expressed in m/s
     * @return velocity expressed in miles per hour
     */
    FDM_CONSTEXPR static inline double mps2mph( double vel = 1.0 )
    {
        return vel * 2.23693629;
    }

    /**
     * Converts given velocity from ft/min to ft/s.
     * @param vel velocity expressed in ft/min
     * @return velocity expressed in ft/s
     */
    FDM_CONSTEXPR static inline double fpm2fps( double vel = 1.0 )
    {
        return vel * 0.0166666666667;
    }

    /**
     * Converts given velocity from ft/min to km/h.
     * @param vel velocity expressed in ft/min
     * @return velocity expressed in km/h
     */
    FDM_CONSTEXPR static inline double fpm2kmh( double vel = 1.0 )
    {
        return vel * 0.018288;
    }

    /**
     * Converts given velocity from ft/min to knots.
     * @param vel velocity expressed in ft/min
     * @return velocity expressed in knots
     */
    FDM_CONSTEXPR static inline double fpm2kts( double vel = 1.0 )
    {
        return vel * 0.00987473001;
    }

    /**
     * Converts given velocity from ft/min to miles per hour.
     * @param vel velocity expressed in ft/min
     * @return velocity expressed in miles per hour
     */
    FDM_CONSTEXPR static inline double fpm2mph( double vel = 1.0 )
    {
        return vel * 0.0113636364;
    }

    /**
     * Converts given velocity from ft/min to m/s.
     * @param vel velocity expressed in ft/min
     * @return velocity expressed in m/s
     */
    FDM_CONSTEXPR static inline double fpm2mps( double vel = 1.0 )
    {
        return vel * 0.00508;
    }

    /**
     * Converts given velocity from ft/s to ft/min.
     * @param vel velocity expressed in ft/s
     * @return velocity expressed in ft/min
     */
    FDM_CONSTEXPR static inline double fps2fpm( double vel = 1.0 )
    {
        return vel * 60.0;
    }

    /**
     * Converts given velocity from ft/s to km/h.
     * @param vel velocity expressed in ft/s
     * @return velocity expressed in km/h
     */
    FDM_CONSTEXPR static inline double fps2kmh( double vel = 1.0 )
    {
        return vel * 1.09728;
    }

    /**
     * Converts given velocity from ft/s to knots.
     * @param vel velocity expressed in ft/s
     * @return velocity expressed in knots
     */
    FDM_CONSTEXPR static inline double fps2kts( double vel = 1.0 )
    {
        return vel * 0.592483801;
    }

    /**
     * Converts given velocity from ft/s to miles per hour.
     * @param vel velocity expressed in ft/s
     * @return velocity expressed in miles per hour
     */
    FDM_CONSTEXPR static inline double fps2mph( double vel = 1.0 )
    {
        return vel * 0.681818182;
    }

    /**
     * Converts given velocity from ft/s to m/s.
     * @param vel velocity expressed in ft/s
     * @return velocity expressed in m/s
     */
    FDM_CONSTEXPR static inline double fps2mps( double vel = 1.0 )
    {
        return vel * 0.3048;
    }

    /**
     * Converts given velocity from km/h to ft/min.
     * @param vel velocity expressed in km/h
     * @return velocity expressed in ft/min
     */
    FDM_CONSTEXPR static inline double kmh2fpm( double vel = 1.0 )
    {
        return vel * 54.6806649;
    }

    /**
     * Converts given velocity from km/h to ft/s.
     * @param vel velocity expressed in km/h
     * @return velocity expressed in ft/s
     */
    FDM_CONSTEXPR static inline double kmh2fps( double vel = 1.0 )
    {
        return vel * 0.911344415;
    }

    /**
     * Converts given velocity from km/h to knots.
     * @param vel velocity expressed in km/h
     * @return velocity expressed in knots
     */
    FDM_CONSTEXPR static inline double kmh2kts( double vel = 1.0 )
    {
        return vel * 0.539956803;
    }

    /**
     * Converts given velocity from km/h to miles per hour.
     * @param vel velocity expressed in km/h
     * @return velocity expressed in miles per hour
     */
    FDM_CONSTEXPR static inline double kmh2mph( double vel = 1.0 )
    {
        return vel * 0.621371192;
    }

    /**
     * Converts given velocity from km/h to m/s.
     * @param vel velocity expressed in km/h
     * @return velocity expressed in m/s
     */
    FDM_CONSTEXPR static inline double kmh2mps( double vel = 1.0 )
    {
        return vel * 0.2777777777778;
    }

    /**
     * Converts given velocity from kntos to ft/min.
     * @param vel velocity expressed in knots
     * @return velocity expressed in ft/min
     */
    FDM_CONSTEXPR static inline double kts2fpm( double vel = 1.0 )
    {
        return vel * 101.2685916;
    }

    /**
     * Converts given velocity from kntos to ft/s.
     * @param vel velocity expressed in knots
     * @return velocity expressed in ft/s
     */
    FDM_CONSTEXPR static inline double kts2fps( double vel = 1.0 )
    {
        return vel * 1.68780986;
    }

    /**
     * Converts given velocity from kntos to km/h.
     * @param vel velocity expressed in knots
     * @return velocity expressed in km/h
     */
    FDM_CONSTEXPR static inline double kts2kmh( double vel = 1.0 )
    {
        return vel * 1.852;
    }

    /**
     * Converts given velocity from kntos to miles per hour.
     * @param vel velocity expressed in knots
     * @return velocity expressed in miles per hour
     */
    FDM_CONSTEXPR static inline double kts2mph( double vel = 1.0 )
    {
        return vel * 1.15077945;
    }

    /**
     * Converts given velocity from kntos to m/s.
     * @param vel velocity expressed in knots
     * @return velocity expressed in m/s
     */
    FDM_CONSTEXPR static inline double kts2mps( double vel = 1.0 )
    {
        return vel * 0.5144444444444;
    }

    /**
     * Converts given velocity from m/s to ft/min.
     * @param vel velocity expressed in m/s
     * @return velocity expressed in ft/min
     */
    FDM_CONSTEXPR static inline double mph2fpm( double vel = 1.0 )
    {
        return vel * 88.0;
    }

    /**
     * Converts given velocity from m/s to ft/s.
     * @param vel velocity expressed in m/s
     * @return velocity expressed in ft/s
     */
    FDM_CONSTEXPR static inline double mph2fps( double vel = 1.0 )
    {
        return vel * 1.46666667;
    }

    /**
     * Converts given velocity from m/s to km/h.
     * @param vel velocity expressed in m/s
     * @return velocity expressed in km/h
     */
    FDM_CONSTEXPR static inline double mph2kmh( double vel = 1.0 )
    {
        return vel * 1.609344;
    }

    /**
     * Converts given velocity from m/s to knots.
     * @param vel velocity expressed in m/s
     * @return velocity expressed in knots
     */
    FDM_CONSTEXPR static inline double mph2kts( double vel = 1.0 )
    {
        return vel * 0.868976242;
    }

    /**
     * Converts given velocity from m/s to miles per hour.
     * @param vel velocity expressed in m/s
     * @return velocity expressed in miles per hour
     */
    FDM_CONSTEXPR static inline double mph2mps( double vel = 1.0 )
    {
        return vel * 0.44704;
    }

    /**
     * Converts given angular velocity from revolutions per minute to radians per second.
     * @param ang_vel angular velocity expressed in revolutions per minute
     * @return velocity expressed in radians per second
     */
    FDM_CONSTEXPR static inline double rpm2rad_s( double ang_vel = 1.0 )
    {
        return 120.0 * M_PI * ang_vel;
    }

    /**
     * Converts given mass from kilograms to pounds.
     * @param mass mass expressed in kilograms
     * @return mass expressed in pounds
     */
    FDM_CONSTEXPR static inline double kg2lb( double mass = 1.0 )
    {
        return mass * 2.20462262;
    }

    /**
     * Converts given mass from pounds to kilograms.
     * @param mass mass expressed in pounds
     * @return mass expressed in kilograms
     */
    FDM_CONSTEXPR static inline double lb2kg( double mass = 1.0 )
    {
        return mass * 0.45359237;
    }

    /**
     * Converts given force from newtons to pounds.
     * @param force force expressed in newtons
     * @return force expressed in pounds
     */
    FDM_CONSTEXPR static inline double n2lbf( double force = 1.0 )
    {
        return force * 0.224808943;
    }

    /**
     * Converts given force from pounds to newtons.
     * @param force force expressed in pounds
     * @return force expressed in newtons
     */
    FDM_CONSTEXPR static inline double lbf2n( double force = 1.0 )
    {
        return force * 4.4482216152605;
    }

    /**
     * Converts given pressure from pascals to psi.
     * @param press pressure expressed in pascals
     * @return pressure expressed in psi
     */
    static inline double pa2psi( double press = 1.0 )
    {
        return press * 0.000145037738;
    }

    /**
     * Converts given pressure from pascals to inches Hg.
     * @param press pressure expressed in pascals
     * @return pressure expressed in inches Hg
     */
    static inline double pa2inhg( double press = 1.0 )
    {
        return press * 0.000295333727;
    }

    /**
     * Converts given pressure from pascals to millibars.
     * @param press pressure expressed in pascals
     * @return pressure expressed in millibars
     */
    static inline double pa2mb( double press = 1.0 )
    {
        return press * 0.01;
    }

    /**
     * Converts given pressure from psi to pascals.
     * @param press pressure expressed in psi
     * @return pressure expressed in pascals
     */
    static inline double psi2pa( double press = 1.0 )
    {
        return press * 6894.75729;
    }

    /**
     * Converts given pressure from psi to inches Hg.
     * @param press pressure expressed in psi
     * @return pressure expressed in inches Hg
     */
    static inline double psi2inhg( double press = 1.0 )
    {
        return press * 2.03625437;
    }

    /**
     * Converts given pressure from psi to millibars.
     * @param press pressure expressed in psi
     * @return pressure expressed in millibars
     */
    static inline double psi2mb( double press = 1.0 )
    {
        return press * 68.9475729;
    }

    /**
     * Converts given pressure from inches Hg to pascals.
     * @param press pressure expressed in inches Hg
     * @return pressure expressed in pascals
     */
    static inline double inhg2pa( double press = 1.0 )
    {
        return press * 3386.0;
    }

    /**
     * Converts given pressure from inches Hg to psi.
     * @param press pressure expressed in inches Hg
     * @return pressure expressed in psi
     */
    static inline double inhg2psi( double press = 1.0 )
    {
        return press * 0.49109778;
    }

    /**
     * Converts given pressure from inches Hg to millibars.
     * @param press pressure expressed in inches Hg
     * @return pressure expressed in millibars
     */
    static inline double inhg2mb( double press = 1.0 )
    {
        return press * 33.86;
    }

    /**
     * Converts given pressure from millibars to pascals.
     * @param press pressure expressed in millibars
     * @return pressure expressed in pascals
     */
    static inline double mb2pa( double press = 1.0 )
    {
        return press * 100.0;
    }

    /**
     * Converts given pressure from millibars to psi.
     * @param press pressure expressed in millibars
     * @return pressure expressed in psi
     */
    static inline double mb2psi( double press = 1.0 )
    {
        return press * 0.0145037738;
    }

    /**
     * Converts given pressure from millibars to inches Hg.
     * @param press pressure expressed in millibars
     * @return pressure expressed in inches Hg
     */
    static inline double mb2inhg( double press = 1.0 )
    {
        return press * 0.0295333727;
    }

    /**
     * Converts given power from (metric) horsepower to (mechanical) horsepower.
     * @param pwr power expressed in (metric) horsepower
     * @return power expressed in (mechanical) horsepower
     */
    FDM_CONSTEXPR static inline double ps2hp( double pwr = 1.0 )
    {
        return pwr * 0.98632005756;
    }

    /**
     * Converts given power from (metric) horsepower to watts.
     * @param pwr power expressed in (metric) horsepower
     * @return power expressed in watts
     */
    FDM_CONSTEXPR static inline double ps2w( double pwr = 1.0 )
    {
        return pwr * 735.498749992;
    }

    /**
     * Converts given power from (metric) horsepower to kilowatts.
     * @param pwr power expressed in (metric) horsepower
     * @return power expressed in kilowatts
     */
    FDM_CONSTEXPR static inline double ps2kw( double pwr = 1.0 )
    {
        return pwr * 0.735498749992;
    }

    /**
     * Converts given power from (mechanical) horsepower to (metric) horsepower.
     * @param pwr power expressed in (mechanical) horsepower
     * @return power expressed in (metric) horsepower
     */
    FDM_CONSTEXPR static inline double hp2ps( double pwr = 1.0 )
    {
        return pwr * 1.01386967884;
    }

    /**
     * Converts given power from (mechanical) horsepower to watts.
     * @param pwr power expressed in (mechanical) horsepower
     * @return power expressed in watts
     */
    FDM_CONSTEXPR static inline double hp2w( double pwr = 1.0 )
    {
        return pwr * 745.699881448;
    }

    /**
     * Converts given power from (mechanical) horsepower to kilowatts.
     * @param pwr power expressed in (mechanical) horsepower
     * @return power expressed in kilowatts
     */
    FDM_CONSTEXPR static inline double hp2kw( double pwr = 1.0 )
    {
        return pwr * 0.745699881448;
    }

    /**
     * Converts given power from watts to (metric) horsepower.
     * @param pwr power expressed in watts
     * @return power expressed in (metric) horsepower
     */
    FDM_CONSTEXPR static inline double w2ps( double pwr = 1.0 )
    {
        return pwr * 0.00135962161;
    }

    /**
     * Converts given power from watts to (mechanical) horsepower.
     * @param pwr power expressed in watts
     * @return power expressed in (mechanical) horsepower
     */
    FDM_CONSTEXPR static inline double w2hp( double pwr = 1.0 )
    {
        return pwr * 0.00134102207;
    }

    /**
     * Converts given power from watts to kilowatts.
     * @param pwr power expressed in watts
     * @return power expressed in kilowatts
     */
    FDM_CONSTEXPR static inline double w2kw( double pwr = 1.0 )
    {
        return pwr * 0.001;
    }

    /**
     * Converts given power from kilowatts to (metric) horsepower.
     * @param pwr power expressed in kilowatts
     * @return power expressed in (metric) horsepower
     */
    FDM_CONSTEXPR static inline double kw2ps( double pwr = 1.0 )
    {
        return pwr * 1.35962161732;
    }

    /**
     * Converts given power from kilowatts to (mechanical) horsepower.
     * @param pwr power expressed in kilowatts
     * @return power expressed in (mechanical) horsepower
     */
    FDM_CONSTEXPR static inline double kw2hp( double pwr = 1.0 )
    {
        return pwr * 1.34102207185;
    }

    /**
     * Converts given power from kilowatts to watts.
     * @param pwr power expressed in kilowatts
     * @return power expressed in watts
     */
    FDM_CONSTEXPR static inline double kw2w( double pwr = 1.0 )
    {
        return pwr * 1000.0;
    }

    /**
     * Converts given temperature from degrees Celsius to Fahrenheit.
     * @param temp temperature expressed in degrees Celsius
     * @return temperature expressed in degrees Fehrenheit
     */
    FDM_CONSTEXPR static inline double c2f( double temp )
    {
        return ( 9.0 * temp / 5.0 ) + 32.0;
    }

    /**
     * Converts given temperature from degrees Celsius to Kelvins.
     * @param temp temperature expressed in degrees Celsius
     * @return temperature expressed in Kelvins
     */
    FDM_CONSTEXPR static inline double c2k( double temp )
    {
        return temp + 273.15;
    }

    /**
     * Converts given temperature from degrees Fahrenheit to Celsius.
     * @param temp temperature expressed in degrees Fahrenheit
     * @return temperature expressed in degrees Celsius
     */
    FDM_CONSTEXPR static inline double f2c( double temp )
    {
        return 5.0 * ( temp - 32.0 ) / 9.0;
    }

    /**
     * Converts given temperature from degrees Fahrenheit to Kelvins.
     * @param temp temperature expressed in degrees Fahrenheit
     * @return temperature expressed in Kelvins
     */
    FDM_CONSTEXPR static inline double f2k( double temp )
    {
        return ( 5.0 * ( temp - 32.0 ) / 9.0 ) + 273.15;
    }

    /**
     * Converts given temperature from Kelvins to degrees Celsius.
     * @param temp temperature expressed in Kelvins
     * @return temperature expressed in degrees Celsius
     */
    FDM_CONSTEXPR static inline double k2c( double temp )
    {
        return ( temp - 273.15 );
    }

    /**
     * Converts given temperature from Kelvins to degrees Fahrenheit.
     * @param temp temperature expressed in Kelvins
     * @return temperature expressed in degrees Fahrenheit
     */
    FDM_CONSTEXPR static inline double k2f( double temp )
    {
        return ( 9.0 * ( temp - 273.15 ) / 5.0 ) + 32.0;
    }

    /**
     * Converts given specific fuel consumption from g/kWh to kg/Ws.
     * @param sfc specific fuel consumption expressed in g/kWh
     * @return specific fuel consumption expressed in kg/Ws
     */
    FDM_CONSTEXPR static inline double g_kWh_2_kg_Ws( double sfc = 1.0 )
    {
         return sfc / ( 3.6e9 );
    }

    /**
     * Converts given specific fuel consumption from g/kNs to kg/Ns.
     * @param sfc specific fuel consumption expressed in g/kNs
     * @return specific fuel consumption expressed in kg/Ns
     */
    FDM_CONSTEXPR static inline double g_kNs_2_kg_Ns( double sfc = 1.0 )
    {
        return sfc / ( 1.0e6 );
    }
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_UNITS_H
