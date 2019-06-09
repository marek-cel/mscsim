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

#include <fdm/sys/fdm_HPF.h>

#include <algorithm>
#include <math.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

HPF::HPF() :
    m_omega ( 1.0 ),
    m_tc ( 1.0 ),
    m_u_prev ( 0.0 ),
    m_y ( 0.0 )
{}

////////////////////////////////////////////////////////////////////////////////

HPF::HPF( double omega, double y ) :
    m_omega ( omega ),
    m_tc ( 1.0 / m_omega ),
    m_u_prev ( 0.0 ),
    m_y ( y )
{}

////////////////////////////////////////////////////////////////////////////////

void HPF::setValue( double y )
{
    m_y = y;
}

////////////////////////////////////////////////////////////////////////////////

void HPF::setOmega( double omega )
{
    m_omega = std::max( 0.0, omega );
    m_tc = 1.0 / m_omega;
}

////////////////////////////////////////////////////////////////////////////////

void HPF::setCutoffFreq( double freq )
{
    m_omega = 2.0 * M_PI * std::max( 0.0, freq );
    m_tc = 1.0 / m_omega;
}

////////////////////////////////////////////////////////////////////////////////

void HPF::update( double u, double dt )
{
    if ( dt > 0.0 )
    {
        double u_dif = ( dt > 0.0 ) ? ( u - m_u_prev ) / dt : 0.0;
        m_y = m_y + ( 1.0 - exp( -dt / m_tc ) ) * ( m_tc * u_dif - m_y );
        m_u_prev = u;
    }
}
