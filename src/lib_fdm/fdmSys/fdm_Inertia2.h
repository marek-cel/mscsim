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
#ifndef FDM_INERTIA2_H
#define FDM_INERTIA2_H

////////////////////////////////////////////////////////////////////////////////

#include <fdmSys/fdm_Inertia.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief 2nd order inertia class.
 * G(s) = 1 / ( 1 + s*Tc1 )( 1 + s*Tc2 )
 */
class FDMEXPORT Inertia2
{
public:

    Inertia2();

    Inertia2( double tc1, double tc2, double y = 0.0 );

    virtual ~Inertia2();

    inline double getValue() const { return m_y; }

    void setValue( double y );
    void setTimeConstant1( double tc1 );
    void setTimeConstant2( double tc2 );

    void update( double u, double dt );

private:

    Inertia *m_inertia1;

    double m_tc2;   ///< time constant
    double m_y;     ///< current value
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_INERTIA2_H
