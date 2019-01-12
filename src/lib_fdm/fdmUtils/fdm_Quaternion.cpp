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

#include <fdmUtils/fdm_Quaternion.h>

#include <math.h>

#include <fdmUtils/fdm_Matrix3x3.h>
#include <fdmUtils/fdm_Misc.h>
#include <fdmUtils/fdm_Vector3.h>

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
    m_e0 = quat.m_e0;
    m_ex = quat.m_ex;
    m_ey = quat.m_ey;
    m_ez = quat.m_ez;
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

    m_e0 = ( cosPhi2CosPsi2 * cosTht2 ) + ( sinPhi2SinPsi2 * sinTht2 );
    m_ex = ( sinPhi2CosPsi2 * cosTht2 ) - ( cosPhi2SinPsi2 * sinTht2 );
    m_ey = ( cosPhi2CosPsi2 * sinTht2 ) + ( sinPhi2SinPsi2 * cosTht2 );
    m_ez = ( cosPhi2SinPsi2 * cosTht2 ) - ( sinPhi2CosPsi2 * sinTht2 );

    normalize();
}

////////////////////////////////////////////////////////////////////////////////

Quaternion::Quaternion( double angl, const Vector3 &vect )
{
    double len_inv = 1.0 / vect.getLength();

    double cosAngl2 = cos( 0.5 * angl );
    double sinAngl2 = cos( 0.5 * angl );

    m_e0 = cosAngl2;
    m_ex = sinAngl2 * vect.x() * len_inv;
    m_ey = sinAngl2 * vect.y() * len_inv;
    m_ez = sinAngl2 * vect.z() * len_inv;
}

////////////////////////////////////////////////////////////////////////////////

bool Quaternion::isValid() const
{
    return Misc::isValid( m_e0 )
        && Misc::isValid( m_ex )
        && Misc::isValid( m_ey )
        && Misc::isValid( m_ez );
}

////////////////////////////////////////////////////////////////////////////////

void Quaternion::conjugate()
{
    m_ex = -m_ex;
    m_ey = -m_ey;
    m_ez = -m_ez;
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
    return m_e0*m_e0 + m_ex*m_ex + m_ey*m_ey + m_ez*m_ez;
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
        m_e0 = m_e0 / length;
        m_ex = m_ex / length;
        m_ey = m_ey / length;
        m_ez = m_ez / length;
    }
}

////////////////////////////////////////////////////////////////////////////////

Angles Quaternion::getAngles() const
{
    Angles result;

    double sinTht2 = m_e0*m_ey - m_ex*m_ez;

    if( sinTht2 >= 0.5 )
    {
        result.phi() =  2.0 * asin( m_ex / cos( M_PI_4 ) );
        result.tht() =  M_PI_2;
        result.psi() =  0.0;
    }
    else if ( sinTht2 <= -0.5 )
    {
        result.phi() =  2.0 * asin( m_ex / cos( M_PI_4 ) );
        result.tht() = -M_PI_2;
        result.psi() =  0.0;
    }
    else
    {
        result.phi() = atan2( 2.0*( m_e0*m_ex + m_ey*m_ez ), 1.0 - 2.0*( m_ex*m_ex + m_ey*m_ey ) );
        result.tht() =  asin( 2.0*sinTht2 );
        result.psi() = atan2( 2.0*( m_e0*m_ez + m_ex*m_ey ), 1.0 - 2.0*( m_ey*m_ey + m_ez*m_ez ) );
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

    result.m_e0 = -0.5 * ( m_ez * omega.z()
                         + m_ey * omega.y()
                         + m_ex * omega.x() );

    result.m_ex = -0.5 * ( m_ez * omega.y()
                         - m_ey * omega.z()
                         - m_e0 * omega.x() );

    result.m_ey = -0.5 * ( m_ex * omega.z()
                         - m_e0 * omega.y()
                         - m_ez * omega.x() );

    result.m_ez = -0.5 * ( m_ey * omega.x()
                         - m_e0 * omega.z()
                         - m_ex * omega.y() );

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
    m_e0 = e0;
    m_ex = ex;
    m_ey = ey;
    m_ez = ez;
}

////////////////////////////////////////////////////////////////////////////////

std::string Quaternion::toString() const
{
    std::stringstream ss;

    ss << m_e0 <<  "," << m_ex <<  "," << m_ey <<  "," << m_ez;

    return ss.str();
}

////////////////////////////////////////////////////////////////////////////////

const Quaternion& Quaternion::operator= ( const Quaternion &quat )
{
    m_e0 = quat.m_e0;
    m_ex = quat.m_ex;
    m_ey = quat.m_ey;
    m_ez = quat.m_ez;

    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

Quaternion Quaternion::operator+ ( const Quaternion &quat ) const
{
    Quaternion result;

    result.m_e0 = m_e0 + quat.m_e0;
    result.m_ex = m_ex + quat.m_ex;
    result.m_ey = m_ey + quat.m_ey;
    result.m_ez = m_ez + quat.m_ez;

    return result;
}

////////////////////////////////////////////////////////////////////////////////

Quaternion Quaternion::operator- ( const Quaternion &quat ) const
{
    Quaternion result;

    result.m_e0 = m_e0 - quat.m_e0;
    result.m_ex = m_ex - quat.m_ex;
    result.m_ey = m_ey - quat.m_ey;
    result.m_ez = m_ez - quat.m_ez;

    return result;
}

////////////////////////////////////////////////////////////////////////////////

Quaternion Quaternion::operator* ( double val ) const
{
    Quaternion result;

    result.m_e0 = m_e0 * val;
    result.m_ex = m_ex * val;
    result.m_ey = m_ey * val;
    result.m_ez = m_ez * val;

    return result;
}

////////////////////////////////////////////////////////////////////////////////

Quaternion Quaternion::operator* ( const Quaternion &quat ) const
{
    Quaternion result;

    result.m_e0 = m_e0 * quat.m_e0
                - m_ex * quat.m_ex
                - m_ey * quat.m_ey
                - m_ez * quat.m_ez;

    result.m_ex = m_e0 * quat.m_ex
                + m_ex * quat.m_e0
                + m_ey * quat.m_ez
                - m_ez * quat.m_ey;

    result.m_ey = m_e0 * quat.m_ey
                - m_ex * quat.m_ez
                + m_ey * quat.m_e0
                + m_ez * quat.m_ex;

    result.m_ez = m_e0 * quat.m_ez
                + m_ex * quat.m_ey
                - m_ey * quat.m_ex
                + m_ez * quat.m_e0;

    return result;
}

////////////////////////////////////////////////////////////////////////////////

Quaternion Quaternion::operator/ ( double val ) const
{
    Quaternion result;

    result.m_e0 = m_e0 / val;
    result.m_ex = m_ex / val;
    result.m_ey = m_ey / val;
    result.m_ez = m_ez / val;

    return result;
}

////////////////////////////////////////////////////////////////////////////////

Quaternion& Quaternion::operator+= ( const Quaternion &quat )
{
    m_e0 += quat.m_e0;
    m_ex += quat.m_ex;
    m_ey += quat.m_ey;
    m_ez += quat.m_ez;

    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

Quaternion& Quaternion::operator-= ( const Quaternion &quat )
{
    m_e0 -= quat.m_e0;
    m_ex -= quat.m_ex;
    m_ey -= quat.m_ey;
    m_ez -= quat.m_ez;

    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

Quaternion& Quaternion::operator*= ( double val )
{
    m_e0 *= val;
    m_ex *= val;
    m_ey *= val;
    m_ez *= val;

    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

Quaternion& Quaternion::operator/= ( double val )
{
    m_e0 /= val;
    m_ex /= val;
    m_ey /= val;
    m_ez /= val;

    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

bool Quaternion::operator== ( const Quaternion &quat ) const
{
    return ( m_e0 == quat.m_e0 )
        && ( m_ex == quat.m_ex )
        && ( m_ey == quat.m_ey )
        && ( m_ez == quat.m_ez );
}

////////////////////////////////////////////////////////////////////////////////

bool Quaternion::operator!= ( const Quaternion &quat ) const
{
    return !( (*this) == quat );
}
