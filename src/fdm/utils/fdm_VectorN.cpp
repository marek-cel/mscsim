/****************************************************************************//*
 * Copyright (C) 2021 Marek M. Cel
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

#include <fdm/utils/fdm_VectorN.h>

#include <limits>

#include <fdm/utils/fdm_Misc.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

VectorN::VectorN() :
    _size ( 0 ),
    _items ( FDM_NULLPTR )
{}

////////////////////////////////////////////////////////////////////////////////

VectorN::VectorN( unsigned int size ) :
    _size ( size ),
    _items ( FDM_NULLPTR )
{
    _items = new double[ _size ];

    zeroize();
}

////////////////////////////////////////////////////////////////////////////////

VectorN::VectorN( const VectorN &vect ) :
    _size ( vect._size ),
    _items ( FDM_NULLPTR )
{
    _items = new double[ _size ];

    setArray( vect._items );
}

////////////////////////////////////////////////////////////////////////////////

VectorN::~VectorN()
{
    FDM_DELTAB( _items );
}

////////////////////////////////////////////////////////////////////////////////

bool VectorN::isValid() const
{
    return Misc::isValid( _items, _size );
}

////////////////////////////////////////////////////////////////////////////////

double VectorN::getLength() const
{
    double length2 = 0.0;

    for ( unsigned int i = 0; i < _size; i++ )
    {
        length2 += ( _items[ i ] * _items[ i ] );
    }

    return sqrt( length2 );
}

////////////////////////////////////////////////////////////////////////////////

void VectorN::normalize()
{
    double length = getLength();

    if ( length > 0.0 )
    {
        double length_inv = 1.0 / length;

        for ( unsigned int i = 0; i < _size; i++ )
        {
            _items[ i ] *= length_inv;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void VectorN::getArray( double items[] ) const
{
    for ( unsigned int i = 0; i < _size; i++ )
    {
        items[i] = _items[i];
    }
}

////////////////////////////////////////////////////////////////////////////////

double VectorN::getItem( unsigned int index ) const
{
    if ( index < _size )
    {
        return _items[ index ];
    }
    else
    {
        Exception e;

        e.setType( Exception::ArrayIndexOverLimit );
        e.setInfo( "Index over limit." );

        FDM_THROW( e );
    }

    return std::numeric_limits< double >::quiet_NaN();
}

////////////////////////////////////////////////////////////////////////////////

void VectorN::setArray( const double items[] )
{
    for ( unsigned int i = 0; i < _size; i++ )
    {
        _items[i] = items[i];
    }
}

////////////////////////////////////////////////////////////////////////////////

void VectorN::setItem( unsigned int index, double val )
{
    if ( index < _size )
    {
        _items[ index ] = val;
    }
    else
    {
        Exception e;

        e.setType( Exception::ArrayIndexOverLimit );
        e.setInfo( "Index over limit." );

        FDM_THROW( e );
    }
}

////////////////////////////////////////////////////////////////////////////////

void VectorN::setValue( double val )
{
    for ( unsigned int i = 0; i < _size; i++ )
    {
        _items[i] = val;
    }
}

////////////////////////////////////////////////////////////////////////////////

std::string VectorN::toString() const
{
    std::stringstream ss;

    for ( unsigned int i = 0; i < _size; i++ )
    {
        if ( i != 0 ) ss << ",";

        ss << _items[ i ];
    }

    return ss.str();
}

////////////////////////////////////////////////////////////////////////////////

void VectorN::resize( unsigned int size )
{
    if ( _size != size )
    {
        FDM_DELTAB( _items );

        _size = size;
        _items = new double[ _size ];
    }
}

////////////////////////////////////////////////////////////////////////////////

void VectorN::zeroize()
{
    for ( unsigned int i = 0; i < _size; i++ )
    {
        _items[ i ] = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////

const VectorN& VectorN::operator= ( const VectorN &vect )
{
    resize( vect._size );
    setArray( vect._items );

    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

VectorN VectorN::operator+ ( const VectorN &vect ) const
{
    VectorN result( _size );

    if ( _size == vect._size )
    {
        for ( unsigned int i = 0; i < _size; i++ )
        {
            result._items[ i ] = _items[ i ] + vect._items[ i ];
        }
    }
    else
    {
        result.setValue( std::numeric_limits< double >::quiet_NaN() );
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

VectorN VectorN::operator- () const
{
    VectorN result( _size );

    for ( unsigned int i = 0; i < _size; i++ )
    {
        result._items[ i ] = - _items[ i ];
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

VectorN VectorN::operator- ( const VectorN &vect ) const
{
    VectorN result( _size );

    if ( _size == vect._size )
    {
        for ( unsigned int i = 0; i < _size; i++ )
        {
            result._items[ i ] = _items[ i ] - vect._items[ i ];
        }
    }
    else
    {
        result.setValue( std::numeric_limits< double >::quiet_NaN() );
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

VectorN VectorN::operator* ( double val ) const
{
    VectorN result( _size );

    for ( unsigned int i = 0; i < _size; i++ )
    {
        result._items[ i ] = _items[ i ] * val;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

VectorN VectorN::operator/ ( double val ) const
{
    VectorN result( _size );

    for ( unsigned int i = 0; i < _size; i++ )
    {
        result._items[ i ] = _items[ i ] / val;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

VectorN& VectorN::operator+= ( const VectorN &vect )
{
    if ( _size == vect._size )
    {
        for ( unsigned int i = 0; i < _size; i++ )
        {
            _items[ i ] += vect._items[ i ];
        }
    }
    else
    {
        setValue( std::numeric_limits< double >::quiet_NaN() );
    }

    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

VectorN& VectorN::operator-= ( const VectorN &vect )
{
    if ( _size == vect._size )
    {
        for ( unsigned int i = 0; i < _size; i++ )
        {
            _items[ i ] -= vect._items[ i ];
        }
    }
    else
    {
        setValue( std::numeric_limits< double >::quiet_NaN() );
    }

    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

VectorN& VectorN::operator*= ( double val )
{
    for ( unsigned int i = 0; i < _size; i++ )
    {
        _items[ i ] *= val;
    }

    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

VectorN& VectorN::operator/= ( double val )
{
    for ( unsigned int i = 0; i < _size; i++ )
    {
        _items[ i ] /= val;
    }

    return (*this);
}

