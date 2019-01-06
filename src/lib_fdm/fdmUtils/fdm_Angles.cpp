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

#include <fdmUtils/fdm_Angles.h>

#include <sstream>

#include <fdmUtils/fdm_Misc.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Angles::Angles()
{
    m_phi = 0.0;
    m_tht = 0.0;
    m_psi = 0.0;
}

////////////////////////////////////////////////////////////////////////////////

Angles::Angles( const Angles &angl )
{
    set( angl.m_phi, angl.m_tht, angl.m_psi );
}

////////////////////////////////////////////////////////////////////////////////

Angles::Angles( double phi, double tht, double psi )
{
    set( phi, tht, psi );
}

////////////////////////////////////////////////////////////////////////////////

bool Angles::isValid() const
{
    return ( Misc::isValid( m_phi )
          && Misc::isValid( m_tht )
          && Misc::isValid( m_psi ) );
}

////////////////////////////////////////////////////////////////////////////////

void Angles::normalize()
{
    while ( m_tht >  M_PI_2 )
    {
        m_phi += M_PI;
        m_tht =  M_PI_2 - ( m_tht - M_PI_2 );
        m_psi += M_PI;
    }

    while ( m_tht < -M_PI_2 )
    {
        m_phi += M_PI;
        m_tht = -M_PI_2 - ( m_tht + M_PI_2 );
        m_psi += M_PI;
    }

    while ( m_phi >  M_PI ) m_phi -= 2.0 * M_PI;
    while ( m_phi < -M_PI ) m_phi += 2.0 * M_PI;

    while ( m_psi > 2.0 * M_PI ) m_psi -= 2.0 * M_PI;
    while ( m_psi < 0.0        ) m_psi += 2.0 * M_PI;
}

////////////////////////////////////////////////////////////////////////////////

void Angles::set( double phi, double tht, double psi )
{
    m_phi = phi;
    m_tht = tht;
    m_psi = psi;
}

////////////////////////////////////////////////////////////////////////////////

std::string Angles::toString() const
{
    std::stringstream ss;

    ss << m_phi << "," << m_tht << "," << m_psi;

    return ss.str();
}

////////////////////////////////////////////////////////////////////////////////

const Angles& Angles::operator= ( const Angles &angl )
{
    set( angl.m_phi, angl.m_tht, angl.m_psi );

    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

bool Angles::operator== ( const Angles &angl ) const
{
    return ( ( m_phi == angl.m_phi )
          && ( m_tht == angl.m_tht )
          && ( m_psi == angl.m_psi ) );
}

////////////////////////////////////////////////////////////////////////////////

bool Angles::operator!= ( const Angles &angl ) const
{
    return !( (*this) == angl );
}
