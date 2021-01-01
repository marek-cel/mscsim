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

#include <fdm/utils/fdm_Matrix6x6.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Matrix6x6::Matrix6x6() :
    Matrix< 6,6 >()
{}

////////////////////////////////////////////////////////////////////////////////

Matrix6x6::Matrix6x6( const Matrix6x6 &mtrx ) :
    Matrix< 6,6 >( mtrx )
{}

////////////////////////////////////////////////////////////////////////////////

Matrix6x6::Matrix6x6( const double items[] ) :
    Matrix< 6,6 >( items )
{}

////////////////////////////////////////////////////////////////////////////////

void Matrix6x6::transpose()
{
    Matrix6x6 temp( *this );

    for ( unsigned int r = 0; r < _rows; r++ )
    {
        for ( unsigned int c = 0; c < _cols; c++ )
        {
            _items[ c*_cols + r ] = temp._items[ r*_cols + c ];
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

Matrix6x6 Matrix6x6::getTransposed() const
{
    Matrix6x6 result( *this );

    result.transpose();

    return result;
}

////////////////////////////////////////////////////////////////////////////////

const Matrix6x6& Matrix6x6::operator= ( const Matrix6x6 &mtrx )
{
    setArray( mtrx._items );
    
    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

Matrix6x6 Matrix6x6::operator+ ( const Matrix6x6 &mtrx ) const
{
    Matrix6x6 result;
    
    for ( unsigned int i = 0; i < _size; i++ )
    {
        result._items[ i ] = _items[ i ] + mtrx._items[ i ];
    }
    
    return result;
}

////////////////////////////////////////////////////////////////////////////////

Matrix6x6 Matrix6x6::operator- ( const Matrix6x6 &mtrx ) const
{
    Matrix6x6 result;
    
    for ( unsigned int i = 0; i < _size; i++ )
    {
        result._items[ i ] = _items[ i ] - mtrx._items[ i ];
    }
    
    return result;
}

////////////////////////////////////////////////////////////////////////////////

Matrix6x6 Matrix6x6::operator* ( double value ) const
{
    Matrix6x6 result;
    
    for ( unsigned int i = 0; i < _size; i++ )
    {
        result._items[ i ] = _items[ i ] * value;
    }
    
    return result;
}

////////////////////////////////////////////////////////////////////////////////

Matrix6x6 Matrix6x6::operator* ( const Matrix6x6 &mtrx ) const
{
    Matrix6x6 result;

    for ( unsigned int r = 0; r < _rows; r++ )
    {
        for ( unsigned int c = 0; c < _cols; c++ )
        {
            result._items[ r*_cols + c ] = 0.0;

            for ( unsigned int i = 0; i < _cols; i++ )
            {
                result._items[ r*_cols + c ] += ( _items[ r*_cols + i ] * mtrx._items[ i*_cols + c ] );
            }
        }
    }
    
    return result;
}

////////////////////////////////////////////////////////////////////////////////

Vector6 Matrix6x6::operator* ( const Vector6 &vect ) const
{
    Vector6 result;

    for ( unsigned int r = 0; r < _rows; r++ )
    {
        result(r) = 0.0;

        for ( unsigned int c = 0; c < _cols; c++ )
        {
            result(r) += ( _items[ r*_cols + c ] * vect(c) );
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

Matrix6x6 Matrix6x6::operator/ ( double value ) const
{
    Matrix6x6 result;
    
    for ( unsigned int i = 0; i < _size; i++ )
    {
        result._items[ i ] = _items[ i ] / value;
    }
    
    return result;
}

////////////////////////////////////////////////////////////////////////////////

Matrix6x6& Matrix6x6::operator+= ( const Matrix6x6 &mtrx )
{
    for ( unsigned int i = 0; i < _size; i++ )
    {
        _items[ i ] += mtrx._items[ i ];
    }
    
    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

Matrix6x6& Matrix6x6::operator-= ( const Matrix6x6 &mtrx )
{
    for ( unsigned int i = 0; i < _size; i++ )
    {
        _items[ i ] -= mtrx._items[ i ];
    }
    
    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

Matrix6x6& Matrix6x6::operator*= ( double value )
{
    for ( unsigned int i = 0; i < _size; i++ )
    {
        _items[ i ] *= value;
    }
    
    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

Matrix6x6& Matrix6x6::operator/= ( double value )
{
    for ( unsigned int i = 0; i < _size; i++ )
    {
        _items[ i ] /= value;
    }
    
    return (*this);
}
