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
#ifndef FDM_LAG2_H
#define FDM_LAG2_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/ctrl/fdm_Lag.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Second-order lag class.
 *
 * Transfer function:
 * G(s)  =  1 / ( Tc1*s + 1 )( Tc2*s + 1 )
 */
class FDMEXPORT Lag2
{
public:

    /** Constructor. */
    Lag2();

    Lag2( double tc1, double tc2, double y = 0.0 );

    virtual ~Lag2();

    inline double getValue() const { return _y; }

    /**
     * Sets output value
     * @param youtput value
     */
    void setValue( double y );

    /**
     * Sets time constant tc1.
     * @param tc1 time constant tc1
     */
    void setTimeConst1( double tc1 );

    /**
     * Sets time constant tc2.
     * @param tc2 time constant tc2
     */
    void setTimeConst2( double tc2 );

    /**
     * Updates element due to time step and input value
     * @param u input value
     * @param dt [s] time step
     */
    void update( double u, double dt );

protected:

    Lag *_lag1;             ///< first-order lag element

    double _tc2;            ///< time constant
    double _y;              ///< current value
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_LAG2_H
