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

#include <fdm/sys/fdm_Lag2.h>

#include <math.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Lag2::Lag2() :
    m_lag1 ( new Lag( 0.0, 0.0 ) ),
    m_tc2 ( 0.0 ),
    m_y ( 0.0 )
{}

////////////////////////////////////////////////////////////////////////////////

Lag2::Lag2( double tc1, double tc2, double y ) :
    m_lag1 ( new Lag( tc1, y ) ),
    m_tc2 ( tc2 ),
    m_y ( y )
{}

////////////////////////////////////////////////////////////////////////////////

Lag2::~Lag2()
{
    if ( m_lag1 ) delete m_lag1;
    m_lag1 = 0;
}

////////////////////////////////////////////////////////////////////////////////

void Lag2::setValue( double y )
{
    m_lag1->setValue( y );
    m_y = y;
}

////////////////////////////////////////////////////////////////////////////////

void Lag2::setTimeConstant1( double tc1 )
{
    m_lag1->setTimeConstant( tc1 );
}

////////////////////////////////////////////////////////////////////////////////

void Lag2::setTimeConstant2( double tc2 )
{
    m_tc2 = tc2;
}

////////////////////////////////////////////////////////////////////////////////

void Lag2::update( double u, double dt )
{
    if ( dt > 0.0 )
    {
        m_lag1->update( u, dt );
        m_y = Lag::update( m_lag1->getValue(), m_y, dt, m_tc2 );
    }
}
