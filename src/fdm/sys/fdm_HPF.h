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
#ifndef FDM_HPF_H
#define FDM_HPF_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_Defines.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief First-order high-pass filter (HPF) or washout filter class.
 *
 * Transfer function:
 * G(s)  =  s / ( s + omega )  =  ( s / omega ) / ( s/omega + 1 )
 */
class FDMEXPORT HPF
{
public:

    HPF();

    HPF( double omega, double y = 0.0 );

    inline double getValue() const { return m_y;  }
    inline double getOmega() const { return m_omega; }

    void setValue( double y );
    void setOmega( double omega );

    void setCutoffFreq( double freq );

    void update( double u, double dt );

protected:

    double m_omega;     ///< [rad/s]
    double m_tc;        ///< time constant

    double m_u_prev;    ///<

    double m_y;         ///< current value
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_HPF_H
