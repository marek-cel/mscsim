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

#include <fdm/utils/fdm_Vector3.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

const Vector3 Vector3::_ex = Vector3( 1.0, 0.0, 0.0 );
const Vector3 Vector3::_ey = Vector3( 0.0, 1.0, 0.0 );
const Vector3 Vector3::_ez = Vector3( 0.0, 0.0, 1.0 );

////////////////////////////////////////////////////////////////////////////////

Vector3::Vector3() :
    Vector< 3 >(),

    _x ( _items[ 0 ] ),
    _y ( _items[ 1 ] ),
    _z ( _items[ 2 ] )
{}

////////////////////////////////////////////////////////////////////////////////

Vector3::Vector3( const Vector3 &vect ) :
    Vector< 3 >( vect ),

    _x ( _items[ 0 ] ),
    _y ( _items[ 1 ] ),
    _z ( _items[ 2 ] )
{}

////////////////////////////////////////////////////////////////////////////////

Vector3::Vector3( const double items[] ) :
    Vector< 3 >( items ),

    _x ( _items[ 0 ] ),
    _y ( _items[ 1 ] ),
    _z ( _items[ 2 ] )
{}

////////////////////////////////////////////////////////////////////////////////

Vector3::Vector3( double x, double y, double z ) :
    Vector< 3 >(),

    _x ( _items[ 0 ] ),
    _y ( _items[ 1 ] ),
    _z ( _items[ 2 ] )
{
    _x = x;
    _y = y;
    _z = z;
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
    Quaternion qv( 0.0, _x, _y, _z );
    Quaternion qr = quat.getConjugated() * qv * quat;

    return Vector3( qr.ex(), qr.ey(), qr.ez() );
}

////////////////////////////////////////////////////////////////////////////////

void Vector3::set( double x, double y, double z )
{
    _x = x;
    _y = y;
    _z = z;
}

////////////////////////////////////////////////////////////////////////////////

const Vector3& Vector3::operator= ( const Vector3 &vect )
{
    _x = vect._x;
    _y = vect._y;
    _z = vect._z;
    
    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

Vector3 Vector3::operator+ ( const Vector3 &vect ) const
{
    Vector3 result;
    
    result._x = _x + vect._x;
    result._y = _y + vect._y;
    result._z = _z + vect._z;
    
    return result;
}

////////////////////////////////////////////////////////////////////////////////

Vector3 Vector3::operator- () const
{
    Vector3 result;
    
    result._x = -_x;
    result._y = -_y;
    result._z = -_z;
    
    return result;
}

////////////////////////////////////////////////////////////////////////////////

Vector3 Vector3::operator- ( const Vector3 &vect ) const
{
    Vector3 result;
    
    result._x = _x - vect._x;
    result._y = _y - vect._y;
    result._z = _z - vect._z;
    
    return result;
}

////////////////////////////////////////////////////////////////////////////////

Vector3 Vector3::operator* ( double val ) const
{
    Vector3 result;
    
    result._x = _x * val;
    result._y = _y * val;
    result._z = _z * val;
    
    return result;
}

////////////////////////////////////////////////////////////////////////////////

Vector3 Vector3::operator/ ( double val ) const
{
    Vector3 result;
    
    result._x = _x / val;
    result._y = _y / val;
    result._z = _z / val;
    
    return result;
}

////////////////////////////////////////////////////////////////////////////////

double Vector3::operator* ( const Vector3 &vect ) const
{
    return ( _x*vect._x + _y*vect._y + _z*vect._z );
}

////////////////////////////////////////////////////////////////////////////////

Vector3 Vector3::operator^ ( const Vector3 &vect ) const
{
    Vector3 result;
    
    result._x = _y * vect._z - _z * vect._y;
    result._y = _z * vect._x - _x * vect._z;
    result._z = _x * vect._y - _y * vect._x;
    
    return result;
}

////////////////////////////////////////////////////////////////////////////////

Vector3& Vector3::operator+= ( const Vector3 &vect )
{
    _x += vect._x;
    _y += vect._y;
    _z += vect._z;
    
    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

Vector3& Vector3::operator-= ( const Vector3 &vect )
{
    _x -= vect._x;
    _y -= vect._y;
    _z -= vect._z;
    
    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

Vector3& Vector3::operator*= ( double val )
{
    _x *= val;
    _y *= val;
    _z *= val;
    
    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

Vector3& Vector3::operator/= ( double val )
{
    _x /= val;
    _y /= val;
    _z /= val;
    
    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

Vector3& Vector3::operator^= ( const Vector3 &vect )
{
    (*this) = (*this) ^ vect;
    
    return (*this);
}
