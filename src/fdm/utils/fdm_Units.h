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
     * SI units of certain values types:
     * - Angle: radian [rad]
     * - Length: meter [m]
     * - Velocity: meter per second [m/s]
     * - Angular Velocity: radian per second [rad/s]
     * - Mass: kilogram [kg]
     * - Force: newton [N]
     * - Pressure: pascal [Pa]
     * - Power: watt [W]
     * - Temperature: kelvin [K]
     * - Specific fuel consumption: kilograms per watt-second [kg/(W*s)]
     * - Thrust Specific fuel consumption: kilograms per newton-second [kg/(N*s)]
     *
     * Unit names of certain values types:
     * - Angle: deg (degree)
     * - Length: ft (feet), km (kilometer), nmi (nautical mile)
     * - Volume: cuin (cubic inches), l (litres)
     * - Velocity: fpm (feet per minute), fps (feet per second), kmh (kilemeter per hour), kts (knots)
     * - Angular Velocity: deg/s (degrees per second), rpm (revolutions per minute)
     * - Mass: lb (pound)
     * - Force: lbf (pound of force)
     * - Pressure: psi (pound per square inch), inHg (inch of mercury), mb (millibar)
     * - Power: PS (metric horsepower), hp (horsepower), kW (kilowatt)
     * - Temperature: degC (degree Celsius), degF (degree Fahrenheit)
     * - Specific fuel consumption: g/kWh (grams per kilowatt-hour)
     * - Thrust Specific fuel consumption: g/kNs (grams per kilonewton-socond)
     */
    static fptr getConverter( const char *name );

    // ANGLE

    /**
     * @brief Converts given angle from degrees to radians.
     * @param ang angle expressed in degrees
     * @return angle expressed in radians
     */
    static inline double deg2rad( double ang = 1.0 )
    {
        return ang * M_PI / 180.0;
    }

    /**
     * @brief Converts given angle from radians to degrees.
     * @param ang angle expressed in radians
     * @return angle expressed in degrees
     */
    static inline double rad2deg( double ang = 1.0 )
    {
        return ang * 180.0 / M_PI;
    }

    // LENGTH

    /**
     * @brief Converts given length from metres to feet.
     * @param len length expressed in metres
     * @return length expressed in feet
     */
    static inline double m2ft( double len = 1.0 )
    {
        return len * 3.2808399;
    }

    /**
     * @brief Converts given length from metres to miles.
     * @param len length expressed in metres
     * @return length expressed in miles
     */
    static inline double m2mi( double len = 1.0 )
    {
        return len * 0.000621371192;
    }

    /**
     * @brief Converts given length from metres to nautical miles.
     * @param len length expressed in metres
     * @return length expressed in nautical miles
     */
    static inline double m2nmi( double len = 1.0 )
    {
        return len * 0.000539956803;
    }

    /**
     * @brief Converts given length from metres to kilometres.
     * @param len length expressed in metres
     * @return length expressed in kilometres
     */
    static inline double m2km( double len = 1.0 )
    {
        return len * 0.001;
    }

    /**
     * @brief Converts given length from feet to metres.
     * @param len length expressed in feet
     * @return length expressed in metres
     */
    static inline double ft2m( double len = 1.0 )
    {
        return len * 0.3048;
    }

    /**
     * @brief Converts given length from feet to kilometres.
     * @param len length expressed in feet
     * @return length expressed in kilometres
     */
    static inline double ft2km( double len = 1.0 )
    {
        return len * 0.0003048;
    }

    /**
     * @brief Converts given length from feet to miles.
     * @param len length expressed in feet
     * @return length expressed in miles
     */
    static inline double ft2mi( double len = 1.0 )
    {
        return len * 0.000189393939;
    }

    /**
     * @brief Converts given length from feet to nautical miles.
     * @param len length expressed in feet
     * @return length expressed in nautical miles
     */
    static inline double ft2nmi( double len = 1.0 )
    {
        return len * 0.000164578834;
    }

    /**
     * @brief Converts given length from kilometres to metres.
     * @param len length expressed in kilometres
     * @return length expressed in metres
     */
    static inline double km2m( double len = 1.0 )
    {
        return len * 1000.0;
    }

    /**
     * @brief Converts given length from kilometres to feet.
     * @param len length expressed in kilometres
     * @return length expressed in feet
     */
    static inline double km2ft( double len = 1.0 )
    {
        return len * 3280.8399;
    }

    /**
     * @brief Converts given length from kilometres to miles.
     * @param len length expressed in kilometres
     * @return length expressed in miles
     */
    static inline double km2mi( double len = 1.0 )
    {
        return len * 0.621371192;
    }

    /**
     * @brief Converts given length from kilometres to nautical miles.
     * @param len length expressed in kilometres
     * @return length expressed in nautical miles
     */
    static inline double km2nmi( double len = 1.0 )
    {
        return len * 0.539956803;
    }

    /**
     * @brief Converts given length from miles to metres.
     * @param len length expressed in nautical miles
     * @return length expressed in metres
     */
    static inline double mi2m( double len = 1.0 )
    {
        return len * 1609.344;
    }

    /**
     * @brief Converts given length from miles to feet.
     * @param len length expressed in nautical miles
     * @return length expressed in feet
     */
    static inline double mi2ft( double len = 1.0 )
    {
        return len * 5280.0;
    }

    /**
     * @brief Converts given length from miles to kilometres.
     * @param len length expressed in nautical miles
     * @return length expressed in kilometres
     */
    static inline double mi2km( double len = 1.0 )
    {
        return len * 1.609344;
    }

    /**
     * @brief Converts given length from miles to nautical miles.
     * @param len length expressed in miles
     * @return length expressed in nautical miles
     */
    static inline double mi2nmi( double len = 1.0 )
    {
        return len * 0.868976242;
    }

    /**
     * @brief Converts given length from nautical miles to metres.
     * @param len length expressed in nautical miles
     * @return length expressed in metres
     */
    static inline double nmi2m( double len = 1.0 )
    {
        return len * 1852.0;
    }

    /**
     * @brief Converts given length from nautical miles to feet.
     * @param len length expressed in nautical miles
     * @return length expressed in feet
     */
    static inline double nmi2ft( double len = 1.0 )
    {
        return len * 6076.11549;
    }

    /**
     * @brief Converts given length from nautical miles to kilometres.
     * @param len length expressed in nautical miles
     * @return length expressed in kilometres
     */
    static inline double nmi2km( double len = 1.0 )
    {
        return len * 1.852;
    }

    /**
     * @brief Converts given length from nautical miles to miles.
     * @param len length expressed in nautical miles
     * @return length expressed in miles
     */
    static inline double nmi2mi( double len = 1.0 )
    {
        return len * 1.15077945;
    }

    // VOLUME

    /**
     * @brief Converts given volume from cubic inches to cubic metres.
     * @param vol volume expressed in cubic inches
     * @return volume expressed in cubic metres
     */
    static inline double cuin2m3( double vol = 1.0 )
    {
        return vol * 0.000016387064;
    }

    /**
     * @brief Converts given volume from litres to cubic metres.
     * @param vol volume expressed in litress
     * @return volume expressed in cubic metres
     */
    static inline double l2m3( double vol = 1.0 )
    {
        return vol / 1000.0;
    }

    // VELOCITY

    /**
     * @brief Converts given velocity from m/s to ft/min.
     * @param vel velocity expressed in m/s
     * @return velocity expressed in ft/min
     */
    static inline double mps2fpm( double vel = 1.0 )
    {
        return vel * 196.850394;
    }

    /**
     * @brief Converts given velocity from m/s to ft/s.
     * @param vel velocity expressed in m/s
     * @return velocity expressed in ft/s
     */
    static inline double mps2fps( double vel = 1.0 )
    {
        return vel * 3.2808399;
    }

    /**
     * @brief Converts given velocity from m/s to km/h.
     * @param vel velocity expressed in m/s
     * @return velocity expressed in km/h
     */
    static inline double mps2kmh( double vel = 1.0 )
    {
        return vel * 3.6;
    }

    /**
     * @brief Converts given velocity from m/s to knots.
     * @param vel velocity expressed in m/s
     * @return velocity expressed in knots
     */
    static inline double mps2kts( double vel = 1.0 )
    {
        return vel * 1.943844491;
    }

    /**
     * @brief Converts given velocity from m/s to miles per hour.
     * @param vel velocity expressed in m/s
     * @return velocity expressed in miles per hour
     */
    static inline double mps2mph( double vel = 1.0 )
    {
        return vel * 2.23693629;
    }

    /**
     * @brief Converts given velocity from ft/min to ft/s.
     * @param vel velocity expressed in ft/min
     * @return velocity expressed in ft/s
     */
    static inline double fpm2fps( double vel = 1.0 )
    {
        return vel * 0.0166666666667;
    }

    /**
     * @brief Converts given velocity from ft/min to km/h.
     * @param vel velocity expressed in ft/min
     * @return velocity expressed in km/h
     */
    static inline double fpm2kmh( double vel = 1.0 )
    {
        return vel * 0.018288;
    }

    /**
     * @brief Converts given velocity from ft/min to knots.
     * @param vel velocity expressed in ft/min
     * @return velocity expressed in knots
     */
    static inline double fpm2kts( double vel = 1.0 )
    {
        return vel * 0.00987473001;
    }

    /**
     * @brief Converts given velocity from ft/min to miles per hour.
     * @param vel velocity expressed in ft/min
     * @return velocity expressed in miles per hour
     */
    static inline double fpm2mph( double vel = 1.0 )
    {
        return vel * 0.0113636364;
    }

    /**
     * @brief Converts given velocity from ft/min to m/s.
     * @param vel velocity expressed in ft/min
     * @return velocity expressed in m/s
     */
    static inline double fpm2mps( double vel = 1.0 )
    {
        return vel * 0.00508;
    }

    /**
     * @brief Converts given velocity from ft/s to ft/min.
     * @param vel velocity expressed in ft/s
     * @return velocity expressed in ft/min
     */
    static inline double fps2fpm( double vel = 1.0 )
    {
        return vel * 60.0;
    }

    /**
     * @brief Converts given velocity from ft/s to km/h.
     * @param vel velocity expressed in ft/s
     * @return velocity expressed in km/h
     */
    static inline double fps2kmh( double vel = 1.0 )
    {
        return vel * 1.09728;
    }

    /**
     * @brief Converts given velocity from ft/s to knots.
     * @param vel velocity expressed in ft/s
     * @return velocity expressed in knots
     */
    static inline double fps2kts( double vel = 1.0 )
    {
        return vel * 0.592483801;
    }

    /**
     * @brief Converts given velocity from ft/s to miles per hour.
     * @param vel velocity expressed in ft/s
     * @return velocity expressed in miles per hour
     */
    static inline double fps2mph( double vel = 1.0 )
    {
        return vel * 0.681818182;
    }

    /**
     * @brief Converts given velocity from ft/s to m/s.
     * @param vel velocity expressed in ft/s
     * @return velocity expressed in m/s
     */
    static inline double fps2mps( double vel = 1.0 )
    {
        return vel * 0.3048;
    }

    /**
     * @brief Converts given velocity from km/h to ft/min.
     * @param vel velocity expressed in km/h
     * @return velocity expressed in ft/min
     */
    static inline double kmh2fpm( double vel = 1.0 )
    {
        return vel * 54.6806649;
    }

    /**
     * @brief Converts given velocity from km/h to ft/s.
     * @param vel velocity expressed in km/h
     * @return velocity expressed in ft/s
     */
    static inline double kmh2fps( double vel = 1.0 )
    {
        return vel * 0.911344415;
    }

    /**
     * @brief Converts given velocity from km/h to knots.
     * @param vel velocity expressed in km/h
     * @return velocity expressed in knots
     */
    static inline double kmh2kts( double vel = 1.0 )
    {
        return vel * 0.539956803;
    }

    /**
     * @brief Converts given velocity from km/h to miles per hour.
     * @param vel velocity expressed in km/h
     * @return velocity expressed in miles per hour
     */
    static inline double kmh2mph( double vel = 1.0 )
    {
        return vel * 0.621371192;
    }

    /**
     * @brief Converts given velocity from km/h to m/s.
     * @param vel velocity expressed in km/h
     * @return velocity expressed in m/s
     */
    static inline double kmh2mps( double vel = 1.0 )
    {
        return vel * 0.2777777777778;
    }

    /**
     * @brief Converts given velocity from kntos to ft/min.
     * @param vel velocity expressed in knots
     * @return velocity expressed in ft/min
     */
    static inline double kts2fpm( double vel = 1.0 )
    {
        return vel * 101.2685916;
    }

    /**
     * @brief Converts given velocity from kntos to ft/s.
     * @param vel velocity expressed in knots
     * @return velocity expressed in ft/s
     */
    static inline double kts2fps( double vel = 1.0 )
    {
        return vel * 1.68780986;
    }

    /**
     * @brief Converts given velocity from kntos to km/h.
     * @param vel velocity expressed in knots
     * @return velocity expressed in km/h
     */
    static inline double kts2kmh( double vel = 1.0 )
    {
        return vel * 1.852;
    }

    /**
     * @brief Converts given velocity from kntos to miles per hour.
     * @param vel velocity expressed in knots
     * @return velocity expressed in miles per hour
     */
    static inline double kts2mph( double vel = 1.0 )
    {
        return vel * 1.15077945;
    }

    /**
     * @brief Converts given velocity from kntos to m/s.
     * @param vel velocity expressed in knots
     * @return velocity expressed in m/s
     */
    static inline double kts2mps( double vel = 1.0 )
    {
        return vel * 0.5144444444444;
    }

    /**
     * @brief Converts given velocity from m/s to ft/min.
     * @param vel velocity expressed in m/s
     * @return velocity expressed in ft/min
     */
    static inline double mph2fpm( double vel = 1.0 )
    {
        return vel * 88.0;
    }

    /**
     * @brief Converts given velocity from m/s to ft/s.
     * @param vel velocity expressed in m/s
     * @return velocity expressed in ft/s
     */
    static inline double mph2fps( double vel = 1.0 )
    {
        return vel * 1.46666667;
    }

    /**
     * @brief Converts given velocity from m/s to km/h.
     * @param vel velocity expressed in m/s
     * @return velocity expressed in km/h
     */
    static inline double mph2kmh( double vel = 1.0 )
    {
        return vel * 1.609344;
    }

    /**
     * @brief Converts given velocity from m/s to knots.
     * @param vel velocity expressed in m/s
     * @return velocity expressed in knots
     */
    static inline double mph2kts( double vel = 1.0 )
    {
        return vel * 0.868976242;
    }

    /**
     * @brief Converts given velocity from m/s to miles per hour.
     * @param vel velocity expressed in m/s
     * @return velocity expressed in miles per hour
     */
    static inline double mph2mps( double vel = 1.0 )
    {
        return vel * 0.44704;
    }

    // ANGULAR VELOCITY

    /**
     * @brief Converts given angular velocity from revolutions per minute to radians per second.
     * @param ang_vel angular velocity expressed in revolutions per minute
     * @return velocity expressed in radians per second
     */
    static inline double rpm2rad_s( double ang_vel = 1.0 )
    {
        return M_PI * ang_vel / 30.0;
    }

    /**
     * @brief Converts given angular velocity from radians per second to revolutions per minute.
     * @param ang_vel angular velocity expressed in radians per second
     * @return velocity expressed in revolutions per minute
     */
    static inline double rad_s2rpm( double ang_vel = 1.0 )
    {
        return 30.0 * ang_vel / M_PI;
    }

    // MASS

    /**
     * @brief Converts given mass from kilograms to pounds.
     * @param mass mass expressed in kilograms
     * @return mass expressed in pounds
     */
    static inline double kg2lb( double mass = 1.0 )
    {
        return mass * 2.20462262;
    }

    /**
     * @brief Converts given mass from pounds to kilograms.
     * @param mass mass expressed in pounds
     * @return mass expressed in kilograms
     */
    static inline double lb2kg( double mass = 1.0 )
    {
        return mass * 0.45359237;
    }

    // FORCE

    /**
     * @brief Converts given force from newtons to pounds.
     * @param force force expressed in newtons
     * @return force expressed in pounds
     */
    static inline double n2lbf( double force = 1.0 )
    {
        return force * 0.224808943;
    }

    /**
     * @brief Converts given force from pounds to newtons.
     * @param force force expressed in pounds
     * @return force expressed in newtons
     */
    static inline double lbf2n( double force = 1.0 )
    {
        return force * 4.4482216152605;
    }

    // PRESSURE

    /**
     * @brief Converts given pressure from pascals to psi.
     * @param press pressure expressed in pascals
     * @return pressure expressed in psi
     */
    static inline double pa2psi( double press = 1.0 )
    {
        return press * 0.000145037738;
    }

    /**
     * @brief Converts given pressure from pascals to inches Hg.
     * @param press pressure expressed in pascals
     * @return pressure expressed in inches Hg
     */
    static inline double pa2inhg( double press = 1.0 )
    {
        return press * 0.000295333727;
    }

    /**
     * @brief Converts given pressure from pascals to millibars.
     * @param press pressure expressed in pascals
     * @return pressure expressed in millibars
     */
    static inline double pa2mb( double press = 1.0 )
    {
        return press * 0.01;
    }

    /**
     * @brief Converts given pressure from psi to pascals.
     * @param press pressure expressed in psi
     * @return pressure expressed in pascals
     */
    static inline double psi2pa( double press = 1.0 )
    {
        return press * 6894.75729;
    }

    /**
     * @brief Converts given pressure from psi to inches Hg.
     * @param press pressure expressed in psi
     * @return pressure expressed in inches Hg
     */
    static inline double psi2inhg( double press = 1.0 )
    {
        return press * 2.03625437;
    }

    /**
     * @brief Converts given pressure from psi to millibars.
     * @param press pressure expressed in psi
     * @return pressure expressed in millibars
     */
    static inline double psi2mb( double press = 1.0 )
    {
        return press * 68.9475729;
    }

    /**
     * @brief Converts given pressure from inches Hg to pascals.
     * @param press pressure expressed in inches Hg
     * @return pressure expressed in pascals
     */
    static inline double inhg2pa( double press = 1.0 )
    {
        return press * 3386.0;
    }

    /**
     * @brief Converts given pressure from inches Hg to psi.
     * @param press pressure expressed in inches Hg
     * @return pressure expressed in psi
     */
    static inline double inhg2psi( double press = 1.0 )
    {
        return press * 0.49109778;
    }

    /**
     * @brief Converts given pressure from inches Hg to millibars.
     * @param press pressure expressed in inches Hg
     * @return pressure expressed in millibars
     */
    static inline double inhg2mb( double press = 1.0 )
    {
        return press * 33.86;
    }

    /**
     * @brief Converts given pressure from millibars to pascals.
     * @param press pressure expressed in millibars
     * @return pressure expressed in pascals
     */
    static inline double mb2pa( double press = 1.0 )
    {
        return press * 100.0;
    }

    /**
     * @brief Converts given pressure from millibars to psi.
     * @param press pressure expressed in millibars
     * @return pressure expressed in psi
     */
    static inline double mb2psi( double press = 1.0 )
    {
        return press * 0.0145037738;
    }

    /**
     * @brief Converts given pressure from millibars to inches Hg.
     * @param press pressure expressed in millibars
     * @return pressure expressed in inches Hg
     */
    static inline double mb2inhg( double press = 1.0 )
    {
        return press * 0.0295333727;
    }

    // POWER

    /**
     * @brief Converts given power from (metric) horsepower to (mechanical) horsepower.
     * @param pwr power expressed in (metric) horsepower
     * @return power expressed in (mechanical) horsepower
     */
    static inline double ps2hp( double pwr = 1.0 )
    {
        return pwr * 0.98632005756;
    }

    /**
     * @brief Converts given power from (metric) horsepower to watts.
     * @param pwr power expressed in (metric) horsepower
     * @return power expressed in watts
     */
    static inline double ps2w( double pwr = 1.0 )
    {
        return pwr * 735.498749992;
    }

    /**
     * @brief Converts given power from (metric) horsepower to kilowatts.
     * @param pwr power expressed in (metric) horsepower
     * @return power expressed in kilowatts
     */
    static inline double ps2kw( double pwr = 1.0 )
    {
        return pwr * 0.735498749992;
    }

    /**
     * @brief Converts given power from (mechanical) horsepower to (metric) horsepower.
     * @param pwr power expressed in (mechanical) horsepower
     * @return power expressed in (metric) horsepower
     */
    static inline double hp2ps( double pwr = 1.0 )
    {
        return pwr * 1.01386967884;
    }

    /**
     * @brief Converts given power from (mechanical) horsepower to watts.
     * @param pwr power expressed in (mechanical) horsepower
     * @return power expressed in watts
     */
    static inline double hp2w( double pwr = 1.0 )
    {
        return pwr * 745.699881448;
    }

    /**
     * @brief Converts given power from (mechanical) horsepower to kilowatts.
     * @param pwr power expressed in (mechanical) horsepower
     * @return power expressed in kilowatts
     */
    static inline double hp2kw( double pwr = 1.0 )
    {
        return pwr * 0.745699881448;
    }

    /**
     * @brief Converts given power from watts to (metric) horsepower.
     * @param pwr power expressed in watts
     * @return power expressed in (metric) horsepower
     */
    static inline double w2ps( double pwr = 1.0 )
    {
        return pwr * 0.00135962161;
    }

    /**
     * @brief Converts given power from watts to (mechanical) horsepower.
     * @param pwr power expressed in watts
     * @return power expressed in (mechanical) horsepower
     */
    static inline double w2hp( double pwr = 1.0 )
    {
        return pwr * 0.00134102207;
    }

    /**
     * @brief Converts given power from watts to kilowatts.
     * @param pwr power expressed in watts
     * @return power expressed in kilowatts
     */
    static inline double w2kw( double pwr = 1.0 )
    {
        return pwr * 0.001;
    }

    /**
     * @brief Converts given power from kilowatts to (metric) horsepower.
     * @param pwr power expressed in kilowatts
     * @return power expressed in (metric) horsepower
     */
    static inline double kw2ps( double pwr = 1.0 )
    {
        return pwr * 1.35962161732;
    }

    /**
     * @brief Converts given power from kilowatts to (mechanical) horsepower.
     * @param pwr power expressed in kilowatts
     * @return power expressed in (mechanical) horsepower
     */
    static inline double kw2hp( double pwr = 1.0 )
    {
        return pwr * 1.34102207185;
    }

    /**
     * @brief Converts given power from kilowatts to watts.
     * @param pwr power expressed in kilowatts
     * @return power expressed in watts
     */
    static inline double kw2w( double pwr = 1.0 )
    {
        return pwr * 1000.0;
    }

    // TEMPERATURE

    /**
     * @brief Converts given temperature from degrees Celsius to Fahrenheit.
     * @param temp temperature expressed in degrees Celsius
     * @return temperature expressed in degrees Fehrenheit
     */
    static inline double c2f( double temp )
    {
        return ( 9.0 * temp / 5.0 ) + 32.0;
    }

    /**
     * @brief Converts given temperature from degrees Celsius to Kelvins.
     * @param temp temperature expressed in degrees Celsius
     * @return temperature expressed in Kelvins
     */
    static inline double c2k( double temp )
    {
        return temp + 273.15;
    }

    /**
     * @brief Converts given temperature from degrees Fahrenheit to Celsius.
     * @param temp temperature expressed in degrees Fahrenheit
     * @return temperature expressed in degrees Celsius
     */
    static inline double f2c( double temp )
    {
        return 5.0 * ( temp - 32.0 ) / 9.0;
    }

    /**
     * @brief Converts given temperature from degrees Fahrenheit to Kelvins.
     * @param temp temperature expressed in degrees Fahrenheit
     * @return temperature expressed in Kelvins
     */
    static inline double f2k( double temp )
    {
        return ( 5.0 * ( temp - 32.0 ) / 9.0 ) + 273.15;
    }

    /**
     * @brief Converts given temperature from Kelvins to degrees Celsius.
     * @param temp temperature expressed in Kelvins
     * @return temperature expressed in degrees Celsius
     */
    static inline double k2c( double temp )
    {
        return ( temp - 273.15 );
    }

    /**
     * @brief Converts given temperature from Kelvins to degrees Fahrenheit.
     * @param temp temperature expressed in Kelvins
     * @return temperature expressed in degrees Fahrenheit
     */
    static inline double k2f( double temp )
    {
        return ( 9.0 * ( temp - 273.15 ) / 5.0 ) + 32.0;
    }

    // SPECIFIC FUEL CONSUMPTION

    /**
     * @brief Converts given specific fuel consumption from g/kWh to kg/Ws.
     * @param sfc specific fuel consumption expressed in g/kWh
     * @return specific fuel consumption expressed in kg/Ws
     */
    static inline double g_kWh_2_kg_Ws( double sfc = 1.0 )
    {
         return sfc / ( 3.6e9 );
    }

    /**
     * @brief Converts given specific fuel consumption from g/kNs to kg/Ns.
     * @param sfc specific fuel consumption expressed in g/kNs
     * @return specific fuel consumption expressed in kg/Ns
     */
    static inline double g_kNs_2_kg_Ns( double sfc = 1.0 )
    {
        return sfc / ( 1.0e6 );
    }
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_UNITS_H
