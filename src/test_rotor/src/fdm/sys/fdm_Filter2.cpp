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

#include <fdm/sys/fdm_Filter2.h>

#include <algorithm>
#include <cmath>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Filter2::Filter2() :
    _c1 ( 0.0 ),
    _c2 ( 0.0 ),
    _c3 ( 0.0 ),
    _c4 ( 0.0 ),
    _c5 ( 0.0 ),
    _c6 ( 0.0 ),
    _u_prev_1 ( 0.0 ),
    _u_prev_2 ( 0.0 ),
    _y_prev_1 ( 0.0 ),
    _y_prev_2 ( 0.0 ),
    _y ( 0.0 )
{}

////////////////////////////////////////////////////////////////////////////////

Filter2::Filter2( double c1, double c2, double c3, double c4, double c5, double c6,
                  double y ) :
    _c1 ( c1 ),
    _c2 ( c2 ),
    _c3 ( c3 ),
    _c4 ( c4 ),
    _c5 ( c5 ),
    _c6 ( c6 ),
    _u_prev_1 ( 0.0 ),
    _u_prev_2 ( 0.0 ),
    _y_prev_1 ( y ),
    _y_prev_2 ( y ),
    _y ( y )
{}

////////////////////////////////////////////////////////////////////////////////

void Filter2::setValue( double y )
{
    _y = y;
}

////////////////////////////////////////////////////////////////////////////////

void Filter2::setC1( double c1 )
{
    _c1 = c1;
}

////////////////////////////////////////////////////////////////////////////////

void Filter2::setC2( double c2 )
{
    _c2 = c2;
}

////////////////////////////////////////////////////////////////////////////////

void Filter2::setC3( double c3 )
{
    _c3 = c3;
}

////////////////////////////////////////////////////////////////////////////////

void Filter2::setC4( double c4 )
{
    _c4 = c4;
}

////////////////////////////////////////////////////////////////////////////////

void Filter2::setC5( double c5 )
{
    _c5 = c5;
}

////////////////////////////////////////////////////////////////////////////////

void Filter2::setC6( double c6 )
{
    _c6 = c6;
}

////////////////////////////////////////////////////////////////////////////////

void Filter2::update( double u, double dt )
{
    if ( dt > 0.0 )
    {
        double den = 4.0*_c4 + 2.0*_c5*dt + _c6*dt*dt;

        double dt2 = dt * dt;

        double ca = ( 4.0 * _c1       + 2.0 * _c2 * dt + _c3 * dt2 ) / den;
        double cb = ( 2.0 * _c3 * dt2 - 8.0 * _c1                  ) / den;
        double cc = ( 4.0 * _c1       - 2.0 * _c2 * dt + _c3 * dt2 ) / den;
        double cd = ( 2.0 * _c6 * dt2 - 8.0 * _c4                  ) / den;
        double ce = ( 4.0 * _c4       - 2.0 * _c5 * dt + _c6 * dt2 ) / den;

        _y = u * ca + _u_prev_1 * cb + _u_prev_2 * cc
                    - _y_prev_1 * cd - _y_prev_2 * ce;

        _u_prev_2 = _u_prev_1;
        _u_prev_1 = u;

        _y_prev_2 = _y_prev_1;
        _y_prev_1 = _y;
    }
}
