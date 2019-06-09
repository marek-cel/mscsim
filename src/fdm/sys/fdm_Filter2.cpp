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

#include <fdm/sys/fdm_Filter2.h>

#include <algorithm>
#include <math.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Filter2::Filter2() :
    m_c1 ( 0.0 ),
    m_c2 ( 0.0 ),
    m_c3 ( 0.0 ),
    m_c4 ( 0.0 ),
    m_c5 ( 0.0 ),
    m_c6 ( 0.0 ),
    m_u_prev_1 ( 0.0 ),
    m_u_prev_2 ( 0.0 ),
    m_y_prev_1 ( 0.0 ),
    m_y_prev_2 ( 0.0 ),
    m_y ( 0.0 )
{}

////////////////////////////////////////////////////////////////////////////////

Filter2::Filter2( double c1, double c2, double c3, double c4, double c5, double c6,
                  double y ) :
    m_c1 ( c1 ),
    m_c2 ( c2 ),
    m_c3 ( c3 ),
    m_c4 ( c4 ),
    m_c5 ( c5 ),
    m_c6 ( c6 ),
    m_u_prev_1 ( 0.0 ),
    m_u_prev_2 ( 0.0 ),
    m_y_prev_1 ( y ),
    m_y_prev_2 ( y ),
    m_y ( y )
{}

////////////////////////////////////////////////////////////////////////////////

void Filter2::setValue( double y )
{
    m_y = y;
}

////////////////////////////////////////////////////////////////////////////////

void Filter2::setC1( double c1 )
{
    m_c1 = c1;
}

////////////////////////////////////////////////////////////////////////////////

void Filter2::setC2( double c2 )
{
    m_c2 = c2;
}

////////////////////////////////////////////////////////////////////////////////

void Filter2::setC3( double c3 )
{
    m_c3 = c3;
}

////////////////////////////////////////////////////////////////////////////////

void Filter2::setC4( double c4 )
{
    m_c4 = c4;
}

////////////////////////////////////////////////////////////////////////////////

void Filter2::setC5( double c5 )
{
    m_c5 = c5;
}

////////////////////////////////////////////////////////////////////////////////

void Filter2::setC6( double c6 )
{
    m_c6 = c6;
}

////////////////////////////////////////////////////////////////////////////////

void Filter2::update( double u, double dt )
{
    if ( dt > 0.0 )
    {
        double den = 4.0*m_c4 + 2.0*m_c5*dt + m_c6*dt*dt;

        double dt2 = dt * dt;

        double ca = ( 4.0 * m_c1       + 2.0 * m_c2 * dt + m_c3 * dt2 ) / den;
        double cb = ( 2.0 * m_c3 * dt2 - 8.0 * m_c1                   ) / den;
        double cc = ( 4.0 * m_c1       - 2.0 * m_c2 * dt + m_c3 * dt2 ) / den;
        double cd = ( 2.0 * m_c6 * dt2 - 8.0 * m_c4                   ) / den;
        double ce = ( 4.0 * m_c4       - 2.0 * m_c5 * dt + m_c6 * dt2 ) / den;

        m_y = u * ca + m_u_prev_1 * cb + m_u_prev_2 * cc
                     - m_y_prev_1 * cd - m_y_prev_2 * ce;

        m_u_prev_2 = m_u_prev_1;
        m_u_prev_1 = u;

        m_y_prev_2 = m_y_prev_1;
        m_y_prev_1 = m_y;
    }
}
