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
#ifndef FDM_MISC_H
#define FDM_MISC_H

////////////////////////////////////////////////////////////////////////////////

#include <cmath>

#ifdef _MSC_VER
#   include <float.h>
#endif

#include <fdm/fdm_Defines.h>

#ifdef _MSC_VER
#   ifdef max
#       undef max
#   endif
#   ifdef min
#       undef min
#   endif
#endif

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Miscellaneous utilities.
 */
class FDMEXPORT Misc
{
public:

    /**
     * @brief Firt order inertia.
     * @param u desired value
     * @param y current value
     * @param dt [s] time step
     * @param tc [s] time constant
     * @return firt order inertia output
     */
    inline static double inertia( double u, double y, double dt, double tc )
    {
        return y + ( 1.0 - exp( -dt / tc ) ) * ( u - y );
    }

    /**
     * @brief Checks if given varaible is Infinite.
     * @param val double precision value to test
     * @return function returns TRUE if tested value is Infinite
     */
    inline static bool isInf( const double &val )
    {
#       ifdef _MSC_VER
        return !( _finite( val ) );
#       else
        return std::isinf( val );
#       endif
    }

    /**
     * @brief Checks if given varaible is NaN.
     * @param val double precision value to test
     * @return function returns TRUE if tested value is NaN
     */
    inline static bool isNaN( const double &val )
    {
        return ( val != val );
    }

    /**
     * @brief Checks if given varaible is Infinite or NaN.
     * @param val double precision value to test
     * @return function returns FALSE if tested value is Infinite or NaN
     */
    inline static bool isValid( const double &val )
    {
        return !( isNaN( val ) || isInf( val ) );
    }

    /**
     * @brief Checks if given array is Infinite or NaN.
     * @param array double precision array to test
     * @param size the size of given array
     * @return function returns FALSE if tested array is Infinite or NaN
     */
    inline static bool isValid( const double array[], unsigned int size )
    {
        for ( unsigned int i = 0; i < size; i++ )
        {
            if ( isNaN( array[ i ] ) || isInf( array[ i ] ) ) return false;
        }

        return true;
    }

    /**
     * @brief Checks if value is within the given range.
     * @param min minimum possible value
     * @param max maximum possible value
     * @param val variable to test
     * @return true if val greater than min and less than max, false otherwise
     */
    inline static bool isInside( const double &min, const double &max, const double &val )
    {
        return min <= val && val <= max;
    }

    /**
     * @brief Checks if value is outside the given range.
     * @param min minimum possible value
     * @param max maximum possible value
     * @param val variable to test
     * @return true if val greater than min and less than max, false otherwise
     */
    inline static bool isOutside( const double &min, const double &max, const double &val )
    {
        return min > val || val > max;
    }

    /**
     * @brief Maximum.
     * @param v1 first value to compare
     * @param v2 second value to campare
     * @return maximum value
     */
    inline static double max( const double &v1, const double &v2 )
    {
        return ( v1 > v2 ) ? v1 : v2;
    }

    /**
     * @brief Minimum.
     * @param v1 first value to compare
     * @param v2 second value to campare
     * @return minimum value
     */
    inline static double min( const double &v1, const double &v2 )
    {
        return ( v1 < v2 ) ? v1 : v2;
    }

    /**
     * @brief Power 2 (square) function.
     * @param val argument
     * @return power 2 (square)
     */
    inline static double pow2( const double &val )
    {
        return val * val;
    }

    /**
     * @brief Power 3 (cube) function.
     * @param val argument
     * @return power 3 (cube)
     */
    inline static double pow3( const double &val )
    {
        return val * val * val;
    }

    /**
     * @brief Power 4 function.
     * @param val argument
     * @return power 4
     */
    inline static double pow4( const double &val )
    {
        return val * val * val * val;
    }

    /**
     * @brief Power 5 function.
     * @param val argument
     * @return power 5
     */
    inline static double pow5( const double &val )
    {
        return val * val * val * val * val;
    }

    /**
     * @brief Maximum rate of change.
     * @param dt [s] time step
     * @param max [1/s] maximum rate of change
     * @param y current value
     * @param u desired value
     * @return output value
     */
    inline static double rate( double dt, double max, double y, double u )
    {
        double d_max = dt * max;
        return y + satur( -d_max, d_max, u - y );
    }

    /**
     * @brief Saturation function. Returns value limited to the given range.
     * @param min minimum possible value
     * @param max maximum possible value
     * @param val variable to test
     * @return min if val less than min, max if val larger than max, val if val larger than min and less than max
     */
    inline static double satur( const double &min, const double &max, const double &val )
    {
        if      ( val < min ) return min;
        else if ( val > max ) return max;

        return val;
    }

    /**
     * @brief Deadband function. Returns zero within the given deadband.
     * @param start deadband start
     * @param end deadband end
     * @param val variable to test
     * @return zero if value is greater than min and less than max
     */
    inline static double deadband( const double &start, const double &end, const double &val )
    {
        if      ( val > end   ) return val - end;
        else if ( val < start ) return val - start;

        return 0.0;
    }

    /**
     * @brief Returns Pacejka "Magic Formula" coefficient.
     * @see https://en.wikipedia.org/wiki/Hans_B._Pacejka#The_Pacejka_%22Magic_Formula%22_tire_models
     * @see https://www.mathworks.com/help/physmod/sdl/ref/tireroadinteractionmagicformula.html
     * @param kappa [-] slip parameter (v_slip/v_roll)
     * @param b b coefficient
     * @param c c coefficient
     * @param d d coefficient
     * @param e e coefficient
     * @return Pacejka "Magic Formula" coefficient
     */
    static double pacejka( double kappa,
                           double b = 10.0, double c = 1.9,
                           double d = 1.0,  double e = 0.97 )
    {
        return d * sin( c * atan( b*( 1.0 - e )*kappa + e*atan( b*kappa ) ) );
    }

    /**
     * @brief Signum function.
     * @param val input value
     * @return 1 if val is possitive, -1 when val is negative, 0 if val is zero
     */
    inline static double sign( const double &val )
    {
        if      ( val < 0.0 ) return -1.0;
        else if ( val > 0.0 ) return  1.0;

        return 0.0;
    }

    /**
     * @brief Sigmoid (logistic) function.
     * @see https://en.wikipedia.org/wiki/Sigmoid_function
     * @see https://en.wikipedia.org/wiki/Logistic_function
     * @param x argument value
     * @param x0 the sigmoid's midpoint
     * @param k the logistic growth rate or steepness of the curve
     * @param l the curve's maximum value
     * @return sigmoid function value
     */
    inline static double sigmoid( const double &x, const double &x0 = 0.0,
                                  const double &k = 1.0, const double &l = 1.0 )
    {
        return l / ( 1.0 + exp( -k * ( x - x0 ) ) );
    }

    /**
     * @brief Estimates standard deviation based on a sample.
     * @see https://en.wikipedia.org/wiki/Standard_deviation
     * @see https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance
     * @param sum sum of samples values
     * @param sumSq  sum of samples values squared
     * @param n number of samples
     * @return standard deviation
     */
    inline static double stDev( double sum, double sumSq, int n )
    {
        double coef = 1.0 / ( (double)n - 1.0 );
        double s2 = sumSq * coef - pow2( sum ) * coef / (double)n;

        return sqrt( s2 );
    }
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_MISC_H
