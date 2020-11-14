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
#ifndef FDM_LEAD_H
#define FDM_LEAD_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_Defines.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief First-order lead class.
 *
 * Transfer function:
 * G(s)  =  Tc*s + 1
 *
 * @see Boulet B.: Fundamentals of Signals and Systems, 2006, p.298
 * @see Kaczorek T.: Teoria ukladow regulacji automatycznej, 1970, p.224. [in Polish]
 */
class FDMEXPORT Lead
{
public:

    /** Constructor. */
    Lead();

    Lead( double tc, double y = 0.0 );

    inline double getValue() const { return _y;  }
    inline double getTimeConst() const { return _tc; }

    /**
     * Sets output value
     * @param youtput value
     */
    void setValue( double y );

    /**
     * Sets time constant.
     * @param tc time constant
     */
    void setTimeConst( double tc );

    /**
     * Updates element due to time step and input value
     * @param u input value
     * @param dt [s] time step
     */
    void update( double u, double dt );

protected:

    double _tc;             ///< time constant

    double _u;              ///< current input
    double _y;              ///< current value
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_LEAD_H
