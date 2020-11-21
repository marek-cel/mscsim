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

#include <fdm/ctrl/fdm_LeadLag.h>

#include <algorithm>
#include <cmath>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

LeadLag::LeadLag() :
    _c1 ( 0.0 ),
    _c2 ( 0.0 ),
    _c3 ( 0.0 ),
    _c4 ( 0.0 ),
    _u_prev ( 0.0 ),
    _y_prev ( 0.0 ),
    _y ( 0.0 )
{}

////////////////////////////////////////////////////////////////////////////////

LeadLag::LeadLag( double c1, double c2, double c3, double c4, double y ) :
    _c1 ( c1 ),
    _c2 ( c2 ),
    _c3 ( c3 ),
    _c4 ( c4 ),
    _u_prev ( 0.0 ),
    _y_prev ( y ),
    _y ( y )
{}

////////////////////////////////////////////////////////////////////////////////

void LeadLag::setValue( double y )
{
    _y = y;
}

////////////////////////////////////////////////////////////////////////////////

void LeadLag::setC1( double c1 )
{
    _c1 = c1;
}

////////////////////////////////////////////////////////////////////////////////

void LeadLag::setC2( double c2 )
{
    _c2 = c2;
}

////////////////////////////////////////////////////////////////////////////////

void LeadLag::setC3( double c3 )
{
    _c3 = c3;
}

////////////////////////////////////////////////////////////////////////////////

void LeadLag::setC4( double c4 )
{
    _c4 = c4;
}

////////////////////////////////////////////////////////////////////////////////

void LeadLag::update( double u, double dt )
{
    if ( dt > 0.0 )
    {
        double den = 2.0 * _c3 + dt * _c4;

        double ca = ( 2.0 * _c1 + dt  * _c2 ) / den;
        double cb = ( dt  * _c2 - 2.0 * _c1 ) / den;
        double cc = ( 2.0 * _c3 - dt  * _c4 ) / den;

        _y = u * ca + _u_prev * cb + _y_prev * cc;

        _u_prev = u;
        _y_prev = _y;
    }
}
