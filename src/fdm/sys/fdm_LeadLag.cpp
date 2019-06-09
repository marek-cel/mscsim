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

#include <fdm/sys/fdm_LeadLag.h>

#include <algorithm>
#include <math.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

LeadLag::LeadLag() :
    m_c1 ( 0.0 ),
    m_c2 ( 0.0 ),
    m_c3 ( 0.0 ),
    m_c4 ( 0.0 ),
    m_u_prev ( 0.0 ),
    m_y_prev ( 0.0 ),
    m_y ( 0.0 )
{}

////////////////////////////////////////////////////////////////////////////////

LeadLag::LeadLag( double c1, double c2, double c3, double c4, double y ) :
    m_c1 ( c1 ),
    m_c2 ( c2 ),
    m_c3 ( c3 ),
    m_c4 ( c4 ),
    m_u_prev ( 0.0 ),
    m_y_prev ( y ),
    m_y ( y )
{}

////////////////////////////////////////////////////////////////////////////////

void LeadLag::setValue( double y )
{
    m_y = y;
}

////////////////////////////////////////////////////////////////////////////////

void LeadLag::setC1( double c1 )
{
    m_c1 = c1;
}

////////////////////////////////////////////////////////////////////////////////

void LeadLag::setC2( double c2 )
{
    m_c2 = c2;
}

////////////////////////////////////////////////////////////////////////////////

void LeadLag::setC3( double c3 )
{
    m_c3 = c3;
}

////////////////////////////////////////////////////////////////////////////////

void LeadLag::setC4( double c4 )
{
    m_c4 = c4;
}

////////////////////////////////////////////////////////////////////////////////

void LeadLag::update( double u, double dt )
{
    if ( dt > 0.0 )
    {
        double den = 2.0 * m_c3 + dt * m_c4;

        double ca = ( 2.0 * m_c1 + dt  * m_c2 ) / den;
        double cb = ( dt  * m_c2 - 2.0 * m_c1 ) / den;
        double cc = ( 2.0 * m_c3 - dt  * m_c4 ) / den;

        m_y = u * ca + m_u_prev * cb + m_y_prev * cc;

        m_u_prev = u;
        m_y_prev = m_y;
    }
}
