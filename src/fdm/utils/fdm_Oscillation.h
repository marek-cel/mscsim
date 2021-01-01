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
#ifndef FDM_OSCILLATION_H
#define FDM_OSCILLATION_H

////////////////////////////////////////////////////////////////////////////////

#include <vector>

#include <fdm/fdm_Defines.h>
#include <fdm/fdm_Types.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Oscillations analysis class.
 */
class FDMEXPORT Oscillation
{
public:

    /** @brief Constructor. */
    Oscillation();

    /** @brief Destructor. */
    virtual ~Oscillation();

    /**
     * @brief Adds single data point and updates parameters.
     * @param x argument
     * @param y value
     */
    void add( double x, double y );

    /**
     * @brief Adds multiple data points and updates parameters.
     * @param x arguments array
     * @param y values array
     * @param n arrays size
     */
    void add( double *x, double *y, unsigned int n );

    /** @brief Updates parameters. */
    void update();

    inline double getY_max() const { return _y_max; }
    inline double getY_min() const { return _y_min; }

    inline double getP_max() const { return _p_max; }
    inline double getP_min() const { return _p_min; }
    inline double getP_avg() const { return _p_avg; }

    inline double getA_max() const { return _a_max; }
    inline double getA_min() const { return _a_min; }
    inline double getA_avg() const { return _a_avg; }

private:

    struct Point
    {
        double x;       ///< argument
        double y;       ///< value
    };

    typedef std::vector< Point > Points;

    Points _points;     ///< data points

    double _x_last;     ///< last added argument

    double _y_max;      ///< maximum value
    double _y_min;      ///< minimum value

    double _p_max;      ///< maximal period
    double _p_min;      ///< minimal period
    double _p_avg;      ///< average period

    double _a_max;      ///< maximal amplitude
    double _a_min;      ///< minimal amplitude
    double _a_avg;      ///< average amplitude
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_OSCILLATION_H
