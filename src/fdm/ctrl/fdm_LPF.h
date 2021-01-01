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
#ifndef FDM_LPF_H
#define FDM_LPF_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/ctrl/fdm_Lag.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief First-order low-pass filter (LPF) class.
 *
 * Transfer function:
 * G(s)  =  1 / ( Tc*s + 1 )  =  omega / ( s + omega )  =  1 / ( s/omega + 1 )
 *
 * First-order low-pass filter is based on a first-order lag element.
 */
class FDMEXPORT LPF : public Lag
{
public:

    /** @brief Constructor. */
    LPF();

    /**
     * @brief Constructor.
     * @param omega [rad/s] cutoff angular frequency
     * @param y initial output value
     */
    LPF( double omega, double y = 0.0 );

    /**
     * @brief Returns cutoff angular frequency.
     * @return cutoff angular frequency
     */
    inline double getOmega() const { return 1.0 / _tc; }

    /**
     * @brief Sets cutoff angular frequency.
     * @param omega [rad/s] cutoff angular frequency
     */
    void setOmega( double omega );

    /**
     * @brief Sets cutoff frequency.
     * @param freq [Hz] cutoff frequency
     */
    void setCutoffFreq( double freq );
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_LPF_H
