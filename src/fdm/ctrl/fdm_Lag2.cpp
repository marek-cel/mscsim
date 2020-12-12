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

#include <fdm/ctrl/fdm_Lag2.h>

#include <cmath>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Lag2::Lag2() :
    _lag1 ( new Lag( 0.0, 0.0 ) ),
    _tc2 ( 0.0 ),
    _y ( 0.0 )
{}

////////////////////////////////////////////////////////////////////////////////

Lag2::Lag2( double tc1, double tc2, double y ) :
    _lag1 ( new Lag( tc1, y ) ),
    _tc2 ( tc2 ),
    _y ( y )
{}

////////////////////////////////////////////////////////////////////////////////

Lag2::~Lag2()
{
    FDM_DELPTR( _lag1 );
}

////////////////////////////////////////////////////////////////////////////////

void Lag2::setValue( double y )
{
    _lag1->setValue( y );
    _y = y;
}

////////////////////////////////////////////////////////////////////////////////

void Lag2::setTimeConst1( double tc1 )
{
    _lag1->setTimeConst( tc1 );
}

////////////////////////////////////////////////////////////////////////////////

void Lag2::setTimeConst2( double tc2 )
{
    _tc2 = tc2;
}

////////////////////////////////////////////////////////////////////////////////

void Lag2::update( double dt, double u )
{
    if ( dt > 0.0 )
    {
        _lag1->update( dt, u );
        _y = Lag::update( _lag1->getValue(), _y, dt, _tc2 );
    }
}
