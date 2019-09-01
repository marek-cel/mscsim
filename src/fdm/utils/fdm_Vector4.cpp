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

#include <fdm/utils/fdm_Vector4.h>

#include <math.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Vector4::Vector4() :
    Vector< 4 >()
{}

////////////////////////////////////////////////////////////////////////////////

Vector4::Vector4( const Vector4 &vect ) :
    Vector< 4 >( vect )
{}

////////////////////////////////////////////////////////////////////////////////

Vector4::Vector4( const double items[] ) :
    Vector< 4 >( items )
{}

////////////////////////////////////////////////////////////////////////////////

Vector4 Vector4::getNormalized() const
{
    Vector4 result( *this );

    result.normalize();

    return result;
}

////////////////////////////////////////////////////////////////////////////////

const Vector4& Vector4::operator= ( const Vector4 &vect )
{
    setArray( vect._items );

    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

Vector4 Vector4::operator+ ( const Vector4 &vect ) const
{
    Vector4 result;

    for ( unsigned int i = 0; i < _size; i++ )
    {
        result._items[ i ] = _items[ i ] + vect._items[ i ];
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

Vector4 Vector4::operator- () const
{
    Vector4 result;

    for ( unsigned int i = 0; i < _size; i++ )
    {
        result._items[ i ] = -_items[ i ];
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

Vector4 Vector4::operator- ( const Vector4 &vect ) const
{
    Vector4 result;

    for ( unsigned int i = 0; i < _size; i++ )
    {
        result._items[ i ] = _items[ i ] - vect._items[ i ];
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

Vector4 Vector4::operator* ( double val ) const
{
    Vector4 result;

    for ( unsigned int i = 0; i < _size; i++ )
    {
        result._items[ i ] = _items[ i ] * val;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

Vector4 Vector4::operator/ ( double val ) const
{
    Vector4 result;

    for ( unsigned int i = 0; i < _size; i++ )
    {
        result._items[ i ] = _items[ i ] / val;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

Vector4& Vector4::operator+= ( const Vector4 &vect )
{
    for ( unsigned int i = 0; i < _size; i++ )
    {
        _items[ i ] += vect._items[ i ];
    }

    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

Vector4& Vector4::operator-= ( const Vector4 &vect )
{
    for ( unsigned int i = 0; i < _size; i++ )
    {
        _items[ i ] -= vect._items[ i ];
    }

    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

Vector4& Vector4::operator*= ( double val )
{
    for ( unsigned int i = 0; i < _size; i++ )
    {
        _items[ i ] *= val;
    }

    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

Vector4& Vector4::operator/= ( double val )
{
    for ( unsigned int i = 0; i < _size; i++ )
    {
        _items[ i ] /= val;
    }

    return (*this);
}
