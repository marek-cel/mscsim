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

#include <fdm/utils/fdm_Matrix4x4.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Matrix4x4::Matrix4x4() :
    Matrix< 4,4 >()
{}

////////////////////////////////////////////////////////////////////////////////

Matrix4x4::Matrix4x4( const Matrix4x4 &mtrx ) :
    Matrix< 4,4 >( mtrx )
{}

////////////////////////////////////////////////////////////////////////////////

Matrix4x4::Matrix4x4( const double items[] ) :
    Matrix< 4,4 >( items )
{}

////////////////////////////////////////////////////////////////////////////////

void Matrix4x4::transpose()
{
    Matrix4x4 temp( *this );

    for ( unsigned int r = 0; r < m_rows; r++ )
    {
        for ( unsigned int c = 0; c < m_cols; c++ )
        {
            m_items[ c*m_cols + r ] = temp.m_items[ r*m_cols + c ];
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

Matrix4x4 Matrix4x4::getTransposed() const
{
    Matrix4x4 result( *this );

    result.transpose();

    return result;
}
////////////////////////////////////////////////////////////////////////////////

const Matrix4x4& Matrix4x4::operator= ( const Matrix4x4 &mtrx )
{
    setArray( mtrx.m_items );
    
    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

Matrix4x4 Matrix4x4::operator+ ( const Matrix4x4 &mtrx ) const
{
    Matrix4x4 result;

    for ( unsigned int i = 0; i < m_size; i++ )
    {
        result.m_items[ i ] = m_items[ i ] + mtrx.m_items[ i ];
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

Matrix4x4 Matrix4x4::operator- ( const Matrix4x4 &mtrx ) const
{
    Matrix4x4 result;

    for ( unsigned int i = 0; i < m_size; i++ )
    {
        result.m_items[ i ] = m_items[ i ] - mtrx.m_items[ i ];
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

Matrix4x4 Matrix4x4::operator* ( double value ) const
{
    Matrix4x4 result;
    
    for ( unsigned int i = 0; i < m_size; i++ )
    {
        result.m_items[ i ] = m_items[ i ] * value;
    }
    
    return result;
}

////////////////////////////////////////////////////////////////////////////////

Matrix4x4 Matrix4x4::operator* ( const Matrix4x4 &mtrx ) const
{
    Matrix4x4 result;

    for ( unsigned int r = 0; r < m_rows; r++ )
    {
        for ( unsigned int c = 0; c < m_cols; c++ )
        {
            result.m_items[ r*m_cols + c ] = 0.0;

            for ( unsigned int i = 0; i < m_cols; i++ )
            {
                result.m_items[ r*m_cols + c ] += ( m_items[ r*m_cols + i ] * mtrx.m_items[ i*m_cols + c ] );
            }
        }
    }
    
    return result;
}

////////////////////////////////////////////////////////////////////////////////

Vector4 Matrix4x4::operator* ( const Vector4 &vect ) const
{
    Vector4 result;

    for ( unsigned int r = 0; r < m_rows; r++ )
    {
        result(r) = 0.0;

        for ( unsigned int c = 0; c < m_cols; c++ )
        {
            result(r) += ( m_items[ r*m_cols + c ] * vect(c) );
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

Matrix4x4 Matrix4x4::operator/ ( double value ) const
{
    Matrix4x4 result;
    
    for ( unsigned int i = 0; i < m_size; i++ )
    {
        result.m_items[ i ] = m_items[ i ] / value;
    }
    
    return result;
}

////////////////////////////////////////////////////////////////////////////////

Matrix4x4& Matrix4x4::operator+= ( const Matrix4x4 &mtrx )
{
    for ( unsigned int i = 0; i < m_size; i++ )
    {
        m_items[ i ] += mtrx.m_items[ i ];
    }
    
    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

Matrix4x4& Matrix4x4::operator-= ( const Matrix4x4 &mtrx )
{
    for ( unsigned int i = 0; i < m_size; i++ )
    {
        m_items[ i ] -= mtrx.m_items[ i ];
    }
    
    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

Matrix4x4& Matrix4x4::operator*= ( double value )
{
    for ( unsigned int i = 0; i < m_size; i++ )
    {
        m_items[ i ] *= value;
    }

    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

Matrix4x4& Matrix4x4::operator/= ( double value )
{
    for ( unsigned int i = 0; i < m_size; i++ )
    {
        m_items[ i ] /= value;
    }

    return (*this);
}
