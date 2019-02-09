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
#ifndef FDM_INERTIA_H
#define FDM_INERTIA_H

////////////////////////////////////////////////////////////////////////////////

#include <fdmMain/fdm_Defines.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Inertia class.
 * G(s) = 1 / ( 1 + s*Tc )
 */
class FDMEXPORT Inertia
{
public:

    /**
     * @brief update
     * @param u input
     * @param y current valuye
     * @param dt [s] time step
     * @param tc [s] time constant
     * @return
     */
    static double update( double u, double y, double dt, double tc );

    Inertia();

    Inertia( double tc, double y = 0.0 );

    inline double getValue() const { return m_y;  }
    inline double getTimeConstant() const { return m_tc; }

    void setValue( double y );
    void setTimeConstant( double tc );

    void update( double u, double dt );

private:

    double m_tc;    ///< time constant
    double m_y;     ///< current value

};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_INERTIA_H
