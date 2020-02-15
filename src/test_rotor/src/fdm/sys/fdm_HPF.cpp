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

#include <fdm/sys/fdm_HPF.h>

#include <algorithm>
#include <cmath>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

HPF::HPF() :
    _omega ( 1.0 ),
    _tc ( 1.0 ),
    _u_prev ( 0.0 ),
    _y ( 0.0 )
{}

////////////////////////////////////////////////////////////////////////////////

HPF::HPF( double omega, double y ) :
    _omega ( omega ),
    _tc ( 1.0 / _omega ),
    _u_prev ( 0.0 ),
    _y ( y )
{}

////////////////////////////////////////////////////////////////////////////////

void HPF::setValue( double y )
{
    _y = y;
}

////////////////////////////////////////////////////////////////////////////////

void HPF::setOmega( double omega )
{
    _omega = std::max( 0.0, omega );
    _tc = 1.0 / _omega;
}

////////////////////////////////////////////////////////////////////////////////

void HPF::setCutoffFreq( double freq )
{
    _omega = 2.0 * M_PI * std::max( 0.0, freq );
    _tc = 1.0 / _omega;
}

////////////////////////////////////////////////////////////////////////////////

void HPF::update( double u, double dt )
{
    if ( dt > 0.0 )
    {
        double u_dif = ( dt > 0.0 ) ? ( u - _u_prev ) / dt : 0.0;
        _y = _y + ( 1.0 - exp( -dt / _tc ) ) * ( _tc * u_dif - _y );
        _u_prev = u;
    }
}
