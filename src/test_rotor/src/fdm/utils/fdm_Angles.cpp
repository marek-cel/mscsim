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

#include <fdm/utils/fdm_Angles.h>

#include <sstream>

#include <fdm/utils/fdm_Misc.h>
#include <fdm/utils/fdm_Units.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

double Angles::normalize( double val, double min )
{
    double tmp = val;
    double max = min + 2.0 * M_PI;

    while ( tmp < min ) tmp += 2.0 * M_PI;
    while ( tmp > max ) tmp -= 2.0 * M_PI;

    return tmp;
}

////////////////////////////////////////////////////////////////////////////////

Angles::DegMinSec Angles::toDegMinSec( double val )
{
    DegMinSec result;

    double deg_abs = fabs( Units::rad2deg( val ) );

    result.deg = floor( deg_abs );
    result.min = floor( ( deg_abs - result.deg ) * 60 );
    result.sec = ( deg_abs - result.deg - result.min / 60.0 ) * 3600.0;

    if ( val < 0.0 ) result.deg *= -1;

    return result;
}

////////////////////////////////////////////////////////////////////////////////

Angles::Angles()
{
    _phi = 0.0;
    _tht = 0.0;
    _psi = 0.0;
}

////////////////////////////////////////////////////////////////////////////////

Angles::Angles( const Angles &angl )
{
    set( angl._phi, angl._tht, angl._psi );
}

////////////////////////////////////////////////////////////////////////////////

Angles::Angles( double phi, double tht, double psi )
{
    set( phi, tht, psi );
}

////////////////////////////////////////////////////////////////////////////////

bool Angles::isValid() const
{
    return ( Misc::isValid( _phi )
          && Misc::isValid( _tht )
          && Misc::isValid( _psi ) );
}

////////////////////////////////////////////////////////////////////////////////

void Angles::normalize()
{
    while ( _tht >  M_PI_2 )
    {
        _phi += M_PI;
        _tht =  M_PI_2 - ( _tht - M_PI_2 );
        _psi += M_PI;
    }

    while ( _tht < -M_PI_2 )
    {
        _phi += M_PI;
        _tht = -M_PI_2 - ( _tht + M_PI_2 );
        _psi += M_PI;
    }

    while ( _phi >  M_PI ) _phi -= 2.0 * M_PI;
    while ( _phi < -M_PI ) _phi += 2.0 * M_PI;

    while ( _psi > 2.0 * M_PI ) _psi -= 2.0 * M_PI;
    while ( _psi < 0.0        ) _psi += 2.0 * M_PI;
}

////////////////////////////////////////////////////////////////////////////////

void Angles::set( double phi, double tht, double psi )
{
    _phi = phi;
    _tht = tht;
    _psi = psi;
}

////////////////////////////////////////////////////////////////////////////////

std::string Angles::toString() const
{
    std::stringstream ss;

    ss << _phi << "," << _tht << "," << _psi;

    return ss.str();
}

////////////////////////////////////////////////////////////////////////////////

const Angles& Angles::operator= ( const Angles &angl )
{
    set( angl._phi, angl._tht, angl._psi );

    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

bool Angles::operator== ( const Angles &angl ) const
{
    return ( ( _phi == angl._phi )
          && ( _tht == angl._tht )
          && ( _psi == angl._psi ) );
}

////////////////////////////////////////////////////////////////////////////////

bool Angles::operator!= ( const Angles &angl ) const
{
    return !( (*this) == angl );
}
