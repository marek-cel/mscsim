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

#include <fdm/utils/fdm_Vector3.h>

#include <math.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

const Vector3 Vector3::m_ux = Vector3( 1.0, 0.0, 0.0 );
const Vector3 Vector3::m_uy = Vector3( 0.0, 1.0, 0.0 );
const Vector3 Vector3::m_uz = Vector3( 0.0, 0.0, 1.0 );

////////////////////////////////////////////////////////////////////////////////

Vector3::Vector3() :
    Vector< 3 >(),

    m_x ( m_items[ 0 ] ),
    m_y ( m_items[ 1 ] ),
    m_z ( m_items[ 2 ] )
{}

////////////////////////////////////////////////////////////////////////////////

Vector3::Vector3( const Vector3 &vect ) :
    Vector< 3 >( vect ),

    m_x ( m_items[ 0 ] ),
    m_y ( m_items[ 1 ] ),
    m_z ( m_items[ 2 ] )
{}

////////////////////////////////////////////////////////////////////////////////

Vector3::Vector3( const double items[] ) :
    Vector< 3 >( items ),

    m_x ( m_items[ 0 ] ),
    m_y ( m_items[ 1 ] ),
    m_z ( m_items[ 2 ] )
{}

////////////////////////////////////////////////////////////////////////////////

Vector3::Vector3( double x, double y, double z ) :
    Vector< 3 >(),

    m_x ( m_items[ 0 ] ),
    m_y ( m_items[ 1 ] ),
    m_z ( m_items[ 2 ] )
{
    m_x = x;
    m_y = y;
    m_z = z;
}

////////////////////////////////////////////////////////////////////////////////

Vector3 Vector3::getNormalized() const
{
    Vector3 result( *this );

    result.normalize();

    return result;
}

////////////////////////////////////////////////////////////////////////////////

Vector3 Vector3::rotate( const Quaternion quat )
{
    // passive rotation
    Quaternion qv( 0.0, m_x, m_y, m_z );
    Quaternion qr = quat.getConjugated() * qv * quat;

    return Vector3( qr.ex(), qr.ey(), qr.ez() );
}

////////////////////////////////////////////////////////////////////////////////

void Vector3::set( double x, double y, double z )
{
    m_x = x;
    m_y = y;
    m_z = z;
}

////////////////////////////////////////////////////////////////////////////////

const Vector3& Vector3::operator= ( const Vector3 &vect )
{
    m_x = vect.m_x;
    m_y = vect.m_y;
    m_z = vect.m_z;
    
    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

Vector3 Vector3::operator+ ( const Vector3 &vect ) const
{
    Vector3 result;
    
    result.m_x = m_x + vect.m_x;
    result.m_y = m_y + vect.m_y;
    result.m_z = m_z + vect.m_z;
    
    return result;
}

////////////////////////////////////////////////////////////////////////////////

Vector3 Vector3::operator- () const
{
    Vector3 result;
    
    result.m_x = -m_x;
    result.m_y = -m_y;
    result.m_z = -m_z;
    
    return result;
}

////////////////////////////////////////////////////////////////////////////////

Vector3 Vector3::operator- ( const Vector3 &vect ) const
{
    Vector3 result;
    
    result.m_x = m_x - vect.m_x;
    result.m_y = m_y - vect.m_y;
    result.m_z = m_z - vect.m_z;
    
    return result;
}

////////////////////////////////////////////////////////////////////////////////

Vector3 Vector3::operator* ( double val ) const
{
    Vector3 result;
    
    result.m_x = m_x * val;
    result.m_y = m_y * val;
    result.m_z = m_z * val;
    
    return result;
}

////////////////////////////////////////////////////////////////////////////////

Vector3 Vector3::operator/ ( double val ) const
{
    Vector3 result;
    
    result.m_x = m_x / val;
    result.m_y = m_y / val;
    result.m_z = m_z / val;
    
    return result;
}

////////////////////////////////////////////////////////////////////////////////

double Vector3::operator* ( const Vector3 &vect ) const
{
    return ( m_x*vect.m_x + m_y*vect.m_y + m_z*vect.m_z );
}

////////////////////////////////////////////////////////////////////////////////

Vector3 Vector3::operator^ ( const Vector3 &vect ) const
{
    Vector3 result;
    
    result.m_x = m_y * vect.m_z - m_z * vect.m_y;
    result.m_y = m_z * vect.m_x - m_x * vect.m_z;
    result.m_z = m_x * vect.m_y - m_y * vect.m_x;
    
    return result;
}

////////////////////////////////////////////////////////////////////////////////

Vector3& Vector3::operator+= ( const Vector3 &vect )
{
    m_x += vect.m_x;
    m_y += vect.m_y;
    m_z += vect.m_z;
    
    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

Vector3& Vector3::operator-= ( const Vector3 &vect )
{
    m_x -= vect.m_x;
    m_y -= vect.m_y;
    m_z -= vect.m_z;
    
    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

Vector3& Vector3::operator*= ( double val )
{
    m_x *= val;
    m_y *= val;
    m_z *= val;
    
    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

Vector3& Vector3::operator/= ( double val )
{
    m_x /= val;
    m_y /= val;
    m_z /= val;
    
    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

Vector3& Vector3::operator^= ( const Vector3 &vect )
{
    (*this) = (*this) ^ vect;
    
    return (*this);
}
