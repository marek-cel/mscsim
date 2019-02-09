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

#include <fdmSys/fdm_Inertia2.h>

#include <math.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Inertia2::Inertia2() :
    m_inertia1 ( new Inertia( 0.0, 0.0 ) ),
    m_tc2 ( 0.0 ),
    m_y ( 0.0 )
{}

////////////////////////////////////////////////////////////////////////////////

Inertia2::Inertia2( double tc1, double tc2, double y ) :
    m_inertia1 ( new Inertia( tc1, y ) ),
    m_tc2 ( tc2 ),
    m_y ( y )
{}

////////////////////////////////////////////////////////////////////////////////

Inertia2::~Inertia2()
{
    if ( m_inertia1 ) delete m_inertia1;
    m_inertia1 = 0;
}

////////////////////////////////////////////////////////////////////////////////

void Inertia2::setValue( double y )
{
    m_inertia1->setValue( y );
    m_y = y;
}

////////////////////////////////////////////////////////////////////////////////

void Inertia2::setTimeConstant1( double tc1 )
{
    m_inertia1->setTimeConstant( tc1 );
}

////////////////////////////////////////////////////////////////////////////////

void Inertia2::setTimeConstant2( double tc2 )
{
    m_tc2 = tc2;
}

////////////////////////////////////////////////////////////////////////////////

void Inertia2::update( double u, double dt )
{
    m_inertia1->update( u, dt );
    m_y = Inertia::update( m_inertia1->getValue(), m_y, dt, m_tc2 );
}
