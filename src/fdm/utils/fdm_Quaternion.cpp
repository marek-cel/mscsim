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

#include <fdm/utils/fdm_Quaternion.h>

#include <cmath>

#include <fdm/utils/fdm_Matrix3x3.h>
#include <fdm/utils/fdm_Misc.h>
#include <fdm/utils/fdm_Vector3.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Quaternion::Quaternion()
{
    set( 0.0, 0.0, 0.0, 0.0 );
}

////////////////////////////////////////////////////////////////////////////////

Quaternion::Quaternion( const Quaternion &quat )
{
    _e0 = quat._e0;
    _ex = quat._ex;
    _ey = quat._ey;
    _ez = quat._ez;
}

////////////////////////////////////////////////////////////////////////////////

Quaternion::Quaternion( double e0, double ex, double ey, double ez )
{
    set( e0, ex, ey, ez );
}

////////////////////////////////////////////////////////////////////////////////

Quaternion::Quaternion( const Angles &angl )
{
    double phi2 = angl.phi() / 2.0;
    double tht2 = angl.tht() / 2.0;
    double psi2 = angl.psi() / 2.0;

    double sinPhi2 = sin( phi2 );
    double cosPhi2 = cos( phi2 );

    double sinTht2 = sin( tht2 );
    double cosTht2 = cos( tht2 );

    double sinPsi2 = sin( psi2 );
    double cosPsi2 = cos( psi2 );

    double cosPhi2CosPsi2 = cosPhi2 * cosPsi2;
    double cosPhi2SinPsi2 = cosPhi2 * sinPsi2;
    double sinPhi2SinPsi2 = sinPhi2 * sinPsi2;
    double sinPhi2CosPsi2 = sinPhi2 * cosPsi2;

    _e0 = ( cosPhi2CosPsi2 * cosTht2 ) + ( sinPhi2SinPsi2 * sinTht2 );
    _ex = ( sinPhi2CosPsi2 * cosTht2 ) - ( cosPhi2SinPsi2 * sinTht2 );
    _ey = ( cosPhi2CosPsi2 * sinTht2 ) + ( sinPhi2SinPsi2 * cosTht2 );
    _ez = ( cosPhi2SinPsi2 * cosTht2 ) - ( sinPhi2CosPsi2 * sinTht2 );

    normalize();
}

////////////////////////////////////////////////////////////////////////////////

Quaternion::Quaternion( double angl, const Vector3 &vect )
{
    double len_inv = 1.0 / vect.getLength();

    double cosAngl2 = cos( 0.5 * angl );
    double sinAngl2 = cos( 0.5 * angl );

    _e0 = cosAngl2;
    _ex = sinAngl2 * vect.x() * len_inv;
    _ey = sinAngl2 * vect.y() * len_inv;
    _ez = sinAngl2 * vect.z() * len_inv;
}

////////////////////////////////////////////////////////////////////////////////

bool Quaternion::isValid() const
{
    return Misc::isValid( _e0 )
        && Misc::isValid( _ex )
        && Misc::isValid( _ey )
        && Misc::isValid( _ez );
}

////////////////////////////////////////////////////////////////////////////////

void Quaternion::conjugate()
{
    _ex = -_ex;
    _ey = -_ey;
    _ez = -_ez;
}

////////////////////////////////////////////////////////////////////////////////

void Quaternion::invert()
{
    conjugate();
    normalize();
}

////////////////////////////////////////////////////////////////////////////////

double Quaternion::getLength2() const
{
    return _e0*_e0 + _ex*_ex + _ey*_ey + _ez*_ez;
}

////////////////////////////////////////////////////////////////////////////////

double Quaternion::getLength() const
{
    return sqrt( getLength2() );
}

////////////////////////////////////////////////////////////////////////////////

void Quaternion::normalize()
{
    double length = getLength();

    if ( length > 0.0 )
    {
        _e0 = _e0 / length;
        _ex = _ex / length;
        _ey = _ey / length;
        _ez = _ez / length;
    }
}

////////////////////////////////////////////////////////////////////////////////

Angles Quaternion::getAngles() const
{
    Angles result;

    double sinTht2 = _e0*_ey - _ex*_ez;

    if( sinTht2 >= 0.5 )
    {
        result.phi() =  2.0 * asin( _ex / cos( M_PI_4 ) );
        result.tht() =  M_PI_2;
        result.psi() =  0.0;
    }
    else if ( sinTht2 <= -0.5 )
    {
        result.phi() =  2.0 * asin( _ex / cos( M_PI_4 ) );
        result.tht() = -M_PI_2;
        result.psi() =  0.0;
    }
    else
    {
        result.phi() = atan2( 2.0*( _e0*_ex + _ey*_ez ), 1.0 - 2.0*( _ex*_ex + _ey*_ey ) );
        result.tht() =  asin( 2.0*sinTht2 );
        result.psi() = atan2( 2.0*( _e0*_ez + _ex*_ey ), 1.0 - 2.0*( _ey*_ey + _ez*_ez ) );
    }

    result.normalize();

    return result;
}

////////////////////////////////////////////////////////////////////////////////

Quaternion Quaternion::getConjugated() const
{
    Quaternion result( *this );

    result.conjugate();

    return result;
}

////////////////////////////////////////////////////////////////////////////////

Quaternion Quaternion::getDerivative( const Vector3 &omega ) const
{
    Quaternion result;

    // | e0_dot |          |  0  P  Q  R | | e0 |   | -0.5 * ( ez*R + ey*Q + ex*P ) |
    // | ex_dot |          | -P  0 -R  Q | | ex |   | -0.5 * ( ez*Q - ey*R - e0*P ) |
    // | ey_dot | = -0.5 * | -Q  R  0 -P | | ey | = | -0.5 * ( ex*R - e0*Q - ez*P ) |
    // | ez_dot |          | -R -Q  P  0 | | ez |   | -0.5 * ( ey*P - e0*R - ex*Q ) |

    result._e0 = -0.5 * ( _ez * omega.z()
                        + _ey * omega.y()
                        + _ex * omega.x() );

    result._ex = -0.5 * ( _ez * omega.y()
                        - _ey * omega.z()
                        - _e0 * omega.x() );

    result._ey = -0.5 * ( _ex * omega.z()
                        - _e0 * omega.y()
                        - _ez * omega.x() );

    result._ez = -0.5 * ( _ey * omega.x()
                        - _e0 * omega.z()
                        - _ex * omega.y() );

    return result;
}

////////////////////////////////////////////////////////////////////////////////

Quaternion Quaternion::getInverted() const
{
    Quaternion result( *this );

    result.invert();

    return result;
}

////////////////////////////////////////////////////////////////////////////////

Quaternion Quaternion::getNormalized() const
{
    Quaternion result( *this );

    result.normalize();

    return result;
}

////////////////////////////////////////////////////////////////////////////////

void Quaternion::set( double e0, double ex, double ey, double ez )
{
    _e0 = e0;
    _ex = ex;
    _ey = ey;
    _ez = ez;
}

////////////////////////////////////////////////////////////////////////////////

std::string Quaternion::toString() const
{
    std::stringstream ss;

    ss << _e0 <<  "," << _ex <<  "," << _ey <<  "," << _ez;

    return ss.str();
}

////////////////////////////////////////////////////////////////////////////////

const Quaternion& Quaternion::operator= ( const Quaternion &quat )
{
    _e0 = quat._e0;
    _ex = quat._ex;
    _ey = quat._ey;
    _ez = quat._ez;

    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

Quaternion Quaternion::operator+ ( const Quaternion &quat ) const
{
    Quaternion result;

    result._e0 = _e0 + quat._e0;
    result._ex = _ex + quat._ex;
    result._ey = _ey + quat._ey;
    result._ez = _ez + quat._ez;

    return result;
}

////////////////////////////////////////////////////////////////////////////////

Quaternion Quaternion::operator- ( const Quaternion &quat ) const
{
    Quaternion result;

    result._e0 = _e0 - quat._e0;
    result._ex = _ex - quat._ex;
    result._ey = _ey - quat._ey;
    result._ez = _ez - quat._ez;

    return result;
}

////////////////////////////////////////////////////////////////////////////////

Quaternion Quaternion::operator* ( double val ) const
{
    Quaternion result;

    result._e0 = _e0 * val;
    result._ex = _ex * val;
    result._ey = _ey * val;
    result._ez = _ez * val;

    return result;
}

////////////////////////////////////////////////////////////////////////////////

Quaternion Quaternion::operator* ( const Quaternion &quat ) const
{
    Quaternion result;

    result._e0 = _e0 * quat._e0
               - _ex * quat._ex
               - _ey * quat._ey
               - _ez * quat._ez;

    result._ex = _e0 * quat._ex
               + _ex * quat._e0
               + _ey * quat._ez
               - _ez * quat._ey;

    result._ey = _e0 * quat._ey
               - _ex * quat._ez
               + _ey * quat._e0
               + _ez * quat._ex;

    result._ez = _e0 * quat._ez
               + _ex * quat._ey
               - _ey * quat._ex
               + _ez * quat._e0;

    return result;
}

////////////////////////////////////////////////////////////////////////////////

Quaternion Quaternion::operator/ ( double val ) const
{
    Quaternion result;

    result._e0 = _e0 / val;
    result._ex = _ex / val;
    result._ey = _ey / val;
    result._ez = _ez / val;

    return result;
}

////////////////////////////////////////////////////////////////////////////////

Quaternion& Quaternion::operator+= ( const Quaternion &quat )
{
    _e0 += quat._e0;
    _ex += quat._ex;
    _ey += quat._ey;
    _ez += quat._ez;

    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

Quaternion& Quaternion::operator-= ( const Quaternion &quat )
{
    _e0 -= quat._e0;
    _ex -= quat._ex;
    _ey -= quat._ey;
    _ez -= quat._ez;

    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

Quaternion& Quaternion::operator*= ( double val )
{
    _e0 *= val;
    _ex *= val;
    _ey *= val;
    _ez *= val;

    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

Quaternion& Quaternion::operator/= ( double val )
{
    _e0 /= val;
    _ex /= val;
    _ey /= val;
    _ez /= val;

    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

bool Quaternion::operator== ( const Quaternion &quat ) const
{
    return ( _e0 == quat._e0 )
        && ( _ex == quat._ex )
        && ( _ey == quat._ey )
        && ( _ez == quat._ez );
}

////////////////////////////////////////////////////////////////////////////////

bool Quaternion::operator!= ( const Quaternion &quat ) const
{
    return !( (*this) == quat );
}
