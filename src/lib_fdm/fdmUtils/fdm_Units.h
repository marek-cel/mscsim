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
#ifndef FDM_UNITS_H
#define FDM_UNITS_H

////////////////////////////////////////////////////////////////////////////////

#include <math.h>

#include <fdmMain/fdm_Defines.h>

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

    /**
     * @brief Returns scaling factor to the SI unit.
     *
     * @param name name of unit
     * @return SI unit scaling factor
     *
     * <h5>SI units of certain values types</h5>
     * <ul>
     * <li>Angle: radian [rad]</li>
     * <li>Length: meter [m]</li>
     * <li>Velocity: meter per second [m/s]</li>
     * <li>Mass: kilogram [kg]</li>
     * <li>Force: newton [N]</li>
     * <li>Pressure: pascal [Pa]</li>
     * <li>Power: watt [W]</li>
     * <li>Specific fuel consumption: kilograms per watt-second [kg/(W*s)]</li>
     * </ul>
     *
     * <h5>Unit names of certain values types</h5>
     * <ul>
     * <li>Angle: deg (degree)</li>
     * <li>Length: ft (feet), km (kilometer), nmi (nautical mile)</li>
     * <li>Velocity: ft_min (feet per minute), ft_s (feet per second), km_h (kilemeter per hour), kts (knots)</li>
     * <li>Mass: lb (pound)</li>
     * <li>Force: lbf (pound of force)</li>
     * <li>Pressure: psi (pound per square inch), inHg (inch of mercury)</li>
     * <li>Power: PS (metric horsepower), hp (horsepower), kW (kilowatt)</li>
     * <li>Specific fuel consumption: g_kWh (grams per kilowatt-hour)</li>
     * </ul>
     */
    static double factorSI( const char *name );

    /**
     * Converts given angle from degrees to radians.
     * @param angle angle expressed in degrees
     * @return angle expressed in radians
     */
    static inline double deg2rad( double angle = 1.0 )
    {
        return angle * M_PI / 180.0;
    }

    /**
     * Converts given angle from radians to degrees.
     * @param angle angle expressed in radians
     * @return angle expressed in degrees
     */
    static inline double rad2deg( double angle = 1.0 )
    {
        return angle * 180.0 / M_PI;
    }

    /**
     * Converts given length from metres to feet.
     * @param length length expressed in metres
     * @return length expressed in feet
     */
    static inline double m2ft( double length = 1.0 )
    {
        return length * 3.2808399;
    }

    /**
     * Converts given length from metres to nautical miles.
     * @param length length expressed in metres
     * @return length expressed in nautical miles
     */
    static inline double m2nmi( double length = 1.0 )
    {
        return length * 0.000539956803;
    }

    /**
     * Converts given length from metres to kilometres.
     * @param length length expressed in metres
     * @return length expressed in kilometres
     */
    static inline double m2km( double length = 1.0 )
    {
        return length * 0.001;
    }

    /**
     * Converts given length from feet to metres.
     * @param length length expressed in feet
     * @return length expressed in metres
     */
    static inline double ft2m( double length = 1.0 )
    {
        return length * 0.3048;
    }

    /**
     * Converts given length from feet to kilometres.
     * @param length length expressed in feet
     * @return length expressed in kilometres
     */
    static inline double ft2km( double length = 1.0 )
    {
        return length * 0.0003048;
    }

    /**
     * Converts given length from feet to nautical miles.
     * @param length length expressed in feet
     * @return length expressed in nautical miles
     */
    static inline double ft2nmi( double length = 1.0 )
    {
        return length * 0.000164578834;
    }

    /**
     * Converts given length from kilometres to metres.
     * @param length length expressed in kilometres
     * @return length expressed in metres
     */
    static inline double km2m( double length = 1.0 )
    {
        return length * 1000.0;
    }

    /**
     * Converts given length from kilometres to feet.
     * @param length length expressed in kilometres
     * @return length expressed in feet
     */
    static inline double km2ft( double length = 1.0 )
    {
        return length * 3280.8399;
    }

    /**
     * Converts given length from kilometres to nautical miles.
     * @param length length expressed in kilometres
     * @return length expressed in nautical miles
     */
    static inline double km2nmi( double length = 1.0 )
    {
        return length * 0.539956803;
    }

    /**
     * Converts given length from nautical miles to metres.
     * @param length length expressed in nautical miles
     * @return length expressed in metres
     */
    static inline double nmi2m( double length = 1.0 )
    {
        return length * 1852.0;
    }

    /**
     * Converts given length from nautical miles to feet.
     * @param length length expressed in nautical miles
     * @return length expressed in feet
     */
    static inline double nmi2ft( double length = 1.0 )
    {
        return length * 6076.11549;
    }

    /**
     * Converts given length from nautical miles to kilometres.
     * @param length length expressed in nautical miles
     * @return length expressed in kilometres
     */
    static inline double nmi2km( double length = 1.0 )
    {
        return length * 1.852;
    }

    /**
     * Converts given velocity from m/s to ft/min.
     * @param velocity velocity expressed in m/s
     * @return velocity expressed in ft/min
     */
    static inline double mps2fpm( double vel = 1.0 )
    {
        return vel * 196.850394;
    }

    /**
     * Converts given velocity from m/s to ft/s.
     * @param velocity velocity expressed in m/s
     * @return velocity expressed in ft/s
     */
    static inline double mps2fps( double vel = 1.0 )
    {
        return vel * 3.2808399;
    }

    /**
     * Converts given velocity from m/s to km/h.
     * @param velocity velocity expressed in m/s
     * @return velocity expressed in km/h
     */
    static inline double mps2kmh( double vel = 1.0 )
    {
        return vel * 3.6;
    }

    /**
     * Converts given velocity from m/s to knots.
     * @param velocity velocity expressed in m/s
     * @return velocity expressed in knots
     */
    static inline double mps2kts( double vel = 1.0 )
    {
        return vel * 1.943844491;
    }

    /**
     * Converts given velocity from ft/min to ft/s.
     * @param velocity velocity expressed in ft/min
     * @return velocity expressed in ft/s
     */
    static inline double fpm2fps( double vel = 1.0 )
    {
        return vel * 0.0166666666667;
    }

    /**
     * Converts given velocity from ft/min to km/h.
     * @param velocity velocity expressed in ft/min
     * @return velocity expressed in km/h
     */
    static inline double fpm2kmh( double vel = 1.0 )
    {
        return vel * 0.018288;
    }

    /**
     * Converts given velocity from ft/min to knots.
     * @param velocity velocity expressed in ft/min
     * @return velocity expressed in knots
     */
    static inline double fpm2kts( double vel = 1.0 )
    {
        return vel * 0.00987473001;
    }

    /**
     * Converts given velocity from ft/min to m/s.
     * @param velocity velocity expressed in ft/min
     * @return velocity expressed in m/s
     */
    static inline double fpm2mps( double vel = 1.0 )
    {
        return vel * 0.00508;
    }

    /**
     * Converts given velocity from ft/s to ft/min.
     * @param velocity velocity expressed in ft/s
     * @return velocity expressed in ft/min
     */
    static inline double fps2fpm( double vel = 1.0 )
    {
        return vel * 60.0;
    }

    /**
     * Converts given velocity from ft/s to km/h.
     * @param velocity velocity expressed in ft/s
     * @return velocity expressed in km/h
     */
    static inline double fps2kmh( double vel = 1.0 )
    {
        return vel * 1.09728;
    }

    /**
     * Converts given velocity from ft/s to knots.
     * @param velocity velocity expressed in ft/s
     * @return velocity expressed in knots
     */
    static inline double fps2kts( double vel = 1.0 )
    {
        return vel * 0.592483801;
    }

    /**
     * Converts given velocity from ft/s to m/s.
     * @param velocity velocity expressed in ft/s
     * @return velocity expressed in m/s
     */
    static inline double fps2mps( double vel = 1.0 )
    {
        return vel * 0.3048;
    }

    /**
     * Converts given velocity from km/h to ft/min.
     * @param velocity velocity expressed in km/h
     * @return velocity expressed in ft/min
     */
    static inline double kmh2fpm( double vel = 1.0 )
    {
        return vel * 54.6806649;
    }

    /**
     * Converts given velocity from km/h to ft/s.
     * @param velocity velocity expressed in km/h
     * @return velocity expressed in ft/s
     */
    static inline double kmh2fps( double vel = 1.0 )
    {
        return vel * 0.911344415;
    }

    /**
     * Converts given velocity from km/h to knots.
     * @param velocity velocity expressed in km/h
     * @return velocity expressed in knots
     */
    static inline double kmh2kts( double vel = 1.0 )
    {
        return vel * 0.539956803;
    }

    /**
     * Converts given velocity from km/h to m/s.
     * @param velocity velocity expressed in km/h
     * @return velocity expressed in m/s
     */
    static inline double kmh2mps( double vel = 1.0 )
    {
        return vel * 0.2777777777778;
    }

    /**
     * Converts given velocity from kntos to ft/min.
     * @param velocity velocity expressed in knots
     * @return velocity expressed in ft/min
     */
    static inline double kts2fpm( double vel = 1.0 )
    {
        return vel * 101.2685916;
    }

    /**
     * Converts given velocity from kntos to ft/s.
     * @param velocity velocity expressed in knots
     * @return velocity expressed in ft/s
     */
    static inline double kts2fps( double vel = 1.0 )
    {
        return vel * 1.68780986;
    }

    /**
     * Converts given velocity from kntos to km/h.
     * @param velocity velocity expressed in knots
     * @return velocity expressed in km/h
     */
    static inline double kts2kmh( double vel = 1.0 )
    {
        return vel * 1.852;
    }

    /**
     * Converts given velocity from kntos to m/s.
     * @param velocity velocity expressed in knots
     * @return velocity expressed in m/s
     */
    static inline double kts2mps( double vel = 1.0 )
    {
        return vel * 0.5144444444444;
    }

    /**
     * Converts given mass from kilograms to pounds.
     * @param mass mass expressed in kilograms
     * @return mass expressed in pounds
     */
    static inline double kg2lb( double mass = 1.0 )
    {
        return mass * 2.20462262;
    }

    /**
     * Converts given mass from pounds to kilograms.
     * @param mass mass expressed in pounds
     * @return mass expressed in kilograms
     */
    static inline double lb2kg( double mass = 1.0 )
    {
        return mass * 0.45359237;
    }

    /**
     * Converts given force from newtons to pounds.
     * @param force force expressed in newtons
     * @return force expressed in pounds
     */
    static inline double n2lbf( double force = 1.0 )
    {
        return force * 0.224808943;
    }

    /**
     * Converts given force from pounds to newtons.
     * @param force force expressed in pounds
     * @return force expressed in newtons
     */
    static inline double lbf2n( double force = 1.0 )
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
        return press * 145.04e-6;
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
     * Converts given power from (metric) horsepower to (mechanical) horsepower.
     * @param power power expressed in (metric) horsepower
     * @return power expressed in (mechanical) horsepower
     */
    static inline double ps2hp( double power = 1.0 )
    {
        return power * 0.98632005756;
    }

    /**
     * Converts given power from (metric) horsepower to watts.
     * @param power power expressed in (metric) horsepower
     * @return power expressed in watts
     */
    static inline double ps2w( double power = 1.0 )
    {
        return power * 735.498749992;
    }

    /**
     * Converts given power from (metric) horsepower to kilowatts.
     * @param power power expressed in (metric) horsepower
     * @return power expressed in kilowatts
     */
    static inline double ps2kw( double power = 1.0 )
    {
        return power * 0.735498749992;
    }

    /**
     * Converts given power from (mechanical) horsepower to (metric) horsepower.
     * @param power power expressed in (mechanical) horsepower
     * @return power expressed in (metric) horsepower
     */
    static inline double hp2ps( double power = 1.0 )
    {
        return power * 1.01386967884;
    }

    /**
     * Converts given power from (mechanical) horsepower to watts.
     * @param power power expressed in (mechanical) horsepower
     * @return power expressed in watts
     */
    static inline double hp2w( double power = 1.0 )
    {
        return power * 745.699881448;
    }

    /**
     * Converts given power from (mechanical) horsepower to kilowatts.
     * @param power power expressed in (mechanical) horsepower
     * @return power expressed in kilowatts
     */
    static inline double hp2kw( double power = 1.0 )
    {
        return power * 0.745699881448;
    }

    /**
     * Converts given power from watts to (metric) horsepower.
     * @param power power expressed in watts
     * @return power expressed in (metric) horsepower
     */
    static inline double w2ps( double power = 1.0 )
    {
        return power * 0.00135962161;
    }

    /**
     * Converts given power from watts to (mechanical) horsepower.
     * @param power power expressed in watts
     * @return power expressed in (mechanical) horsepower
     */
    static inline double w2hp( double power = 1.0 )
    {
        return power * 0.00134102207;
    }

    /**
     * Converts given power from watts to kilowatts.
     * @param power power expressed in watts
     * @return power expressed in kilowatts
     */
    static inline double w2kw( double power = 1.0 )
    {
        return power * 0.001;
    }

    /**
     * Converts given power from kilowatts to (metric) horsepower.
     * @param power power expressed in kilowatts
     * @return power expressed in (metric) horsepower
     */
    static inline double kw2ps( double power = 1.0 )
    {
        return power * 1.35962161732;
    }

    /**
     * Converts given power from kilowatts to (mechanical) horsepower.
     * @param power power expressed in kilowatts
     * @return power expressed in (mechanical) horsepower
     */
    static inline double kw2hp( double power = 1.0 )
    {
        return power * 1.34102207185;
    }

    /**
     * Converts given power from kilowatts to watts.
     * @param power power expressed in kilowatts
     * @return power expressed in watts
     */
    static inline double kw2w( double power = 1.0 )
    {
        return power * 1000.0;
    }

    /**
     * Converts given temperature from degrees Celsius to Fahrenheit.
     * @param temp temperature expressed in degrees Celsius
     * @return temperature expressed in degrees Fehrenheit
     */
    static inline double c2f( double temp )
    {
        return ( 9.0 * temp / 5.0 ) + 32.0;
    }

    /**
     * Converts given temperature from degrees Celsius to Kelvins.
     * @param temp temperature expressed in degrees Celsius
     * @return temperature expressed in Kelvins
     */
    static inline double c2k( double temp )
    {
        return temp + 273.15;
    }

    /**
     * Converts given temperature from degrees Fahrenheit to Celsius.
     * @param temp temperature expressed in degrees Fahrenheit
     * @return temperature expressed in degrees Celsius
     */
    static inline double f2c( double temp )
    {
        return 5.0 * ( temp - 32.0 ) / 9.0;
    }

    /**
     * Converts given temperature from degrees Fahrenheit to Kelvins.
     * @param temp temperature expressed in degrees Fahrenheit
     * @return temperature expressed in Kelvins
     */
    static inline double f2k( double temp )
    {
        return ( 5.0 * ( temp - 32.0 ) / 9.0 ) + 273.15;
    }

    /**
     * Converts given temperature from Kelvins to degrees Celsius.
     * @param temp temperature expressed in Kelvins
     * @return temperature expressed in degrees Celsius
     */
    static inline double k2c( double temp )
    {
        return ( temp - 273.15 );
    }

    /**
     * Converts given temperature from Kelvins to degrees Fahrenheit.
     * @param temp temperature expressed in Kelvins
     * @return temperature expressed in degrees Fahrenheit
     */
    static inline double k2f( double temp )
    {
        return ( 9.0 * ( temp - 273.15 ) / 5.0 ) + 32.0;
    }

    /**
     * Converts given specific fuel consumption from g/kWh to kg/Ws.
     * @param sfc specific fuel consumption expressed in g/kWh
     * @return specific fuel consumption expressed in kg/Ws
     */
    static inline double g_kWh_2_kg_Ws( double sfc = 1.0 )
    {
         return sfc / ( 1000.0 * 1000.0 * 3600.0 );
    }
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_UNITS_H
