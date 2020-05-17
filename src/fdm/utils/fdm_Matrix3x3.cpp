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

#include <fdm/utils/fdm_Matrix3x3.h>

#ifdef _MSC_VER
#   include <algorithm>
#endif

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Matrix3x3 Matrix3x3::createIdentityMatrix()
{
    return Matrix3x3( 1.0, 0.0, 0.0,
                      0.0, 1.0, 0.0,
                      0.0, 0.0, 1.0 );
}

////////////////////////////////////////////////////////////////////////////////

Matrix3x3::Matrix3x3() :
    Matrix< 3,3 >()
{}

////////////////////////////////////////////////////////////////////////////////

Matrix3x3::Matrix3x3( const Matrix3x3 &mtrx ) :
    Matrix< 3,3 >( mtrx )
{}

////////////////////////////////////////////////////////////////////////////////

Matrix3x3::Matrix3x3( const double items[] ) :
    Matrix< 3,3 >( items )
{}

////////////////////////////////////////////////////////////////////////////////

Matrix3x3::Matrix3x3( double xx, double xy, double xz,
                      double yx, double yy, double yz,
                      double zx, double zy, double zz ) :
    Matrix< 3,3 >()
{
    _items[ 0 ] = xx;
    _items[ 1 ] = xy;
    _items[ 2 ] = xz;

    _items[ 3 ] = yx;
    _items[ 4 ] = yy;
    _items[ 5 ] = yz;

    _items[ 6 ] = zx;
    _items[ 7 ] = zy;
    _items[ 8 ] = zz;
}

////////////////////////////////////////////////////////////////////////////////

Matrix3x3::Matrix3x3( const Angles &angl ) :
    Matrix< 3,3 >()
{
    double sinPhi = sin( angl.phi() );
    double cosPhi = cos( angl.phi() );

    double sinTht = sin( angl.tht() );
    double cosTht = cos( angl.tht() );

    double sinPsi = sin( angl.psi() );
    double cosPsi = cos( angl.psi() );

    double sinPhiSinTht = sinPhi * sinTht;
    double cosPhiSinTht = cosPhi * sinTht;

    _items[ 0 ] =  cosTht * cosPsi;
    _items[ 1 ] =  cosTht * sinPsi;
    _items[ 2 ] = -sinTht;

    _items[ 3 ] = -( cosPhi * sinPsi ) + ( sinPhiSinTht * cosPsi );
    _items[ 4 ] =  ( cosPhi * cosPsi ) + ( sinPhiSinTht * sinPsi );
    _items[ 5 ] =  ( sinPhi * cosTht );

    _items[ 6 ] =  ( sinPhi * sinPsi ) + ( cosPhiSinTht * cosPsi );
    _items[ 7 ] = -( sinPhi * cosPsi ) + ( cosPhiSinTht * sinPsi );
    _items[ 8 ] =  ( cosPhi * cosTht );
}

////////////////////////////////////////////////////////////////////////////////

Matrix3x3::Matrix3x3( const Quaternion &qtrn ) :
    Matrix< 3,3 >()
{
    double e0 = qtrn.e0();
    double ex = qtrn.ex();
    double ey = qtrn.ey();
    double ez = qtrn.ez();

    double e02 = e0*e0;
    double ex2 = ex*ex;
    double ey2 = ey*ey;
    double ez2 = ez*ez;

    _items[ 0 ] = e02 + ex2 - ey2 - ez2;
    _items[ 1 ] = 2.0 * ( e0*ez + ex*ey );
    _items[ 2 ] = 2.0 * ( ex*ez - e0*ey );

    _items[ 3 ] = 2.0 * ( ex*ey - e0*ez );
    _items[ 4 ] = e02 - ex2 + ey2 - ez2;
    _items[ 5 ] = 2.0 * ( e0*ex + ey*ez );

    _items[ 6 ] = 2.0 * ( e0*ey + ex*ez );
    _items[ 7 ] = 2.0 * ( ey*ez - e0*ex );
    _items[ 8 ] = e02 - ex2 - ey2 + ez2;
}

////////////////////////////////////////////////////////////////////////////////

void Matrix3x3::transpose()
{
    Matrix3x3 temp( *this );

    for ( unsigned int r = 0; r < _rows; r++ )
    {
        for ( unsigned int c = 0; c < _cols; c++ )
        {
            _items[ c*_cols + r ] = temp._items[ r*_cols + c ];
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

Angles Matrix3x3::getAngles() const
{
    Angles result;

    double sinTht = -_items[ 2 ];
    double cosTht = sqrt( 1.0 - std::min( 1.0, sinTht*sinTht ) );

    result.tht() = atan2( sinTht, cosTht );

    if ( cosTht > 0.0 )
    {
        result.phi() = atan2( _items[ 5 ],  _items[ 8 ] );
        result.psi() = atan2( _items[ 1 ],  _items[ 0 ] );
    }
    else
    {
        result.phi() = atan2( _items[ 3 ], -_items[ 6 ] );
        result.psi() = 0.0;
    }

    result.normalize();

    return result;
}

////////////////////////////////////////////////////////////////////////////////

Quaternion Matrix3x3::getQuaternion() const
{
    Quaternion result;

    // traces
    double tr[ 4 ];

    tr[ 0 ] = 1.0 + _items[ 0 ] + _items[ 4 ] + _items[ 8 ];
    tr[ 1 ] = 1.0 + _items[ 0 ] - _items[ 4 ] - _items[ 8 ];
    tr[ 2 ] = 1.0 - _items[ 0 ] + _items[ 4 ] - _items[ 8 ];
    tr[ 3 ] = 1.0 - _items[ 0 ] - _items[ 4 ] + _items[ 8 ];

    int index = 0;
    for ( int i = 1; i < 4; i++ ) index = ( tr[ i ] > tr[ index ] ) ? i : index;

    if ( index == 0 )
    {
        result.e0() = tr[ 0 ];
        result.ex() = _items[ 5 ] - _items[ 7 ];
        result.ey() = _items[ 6 ] - _items[ 2 ];
        result.ez() = _items[ 1 ] - _items[ 3 ];
    }
    else if ( index == 1 )
    {
        result.e0() = _items[ 5 ] - _items[ 7 ];
        result.ex() = tr[ 1 ];
        result.ey() = _items[ 1 ] + _items[ 3 ];
        result.ez() = _items[ 6 ] + _items[ 2 ];
    }
    else if ( index == 2 )
    {
        result.e0() = _items[ 6 ] - _items[ 2 ];
        result.ex() = _items[ 1 ] + _items[ 3 ];
        result.ey() = tr[ 2 ];
        result.ez() = _items[ 5 ] + _items[ 7 ];
    }
    else
    {
        result.e0() = _items[ 1 ] - _items[ 3 ];
        result.ex() = _items[ 6 ] + _items[ 2 ];
        result.ey() = _items[ 5 ] + _items[ 7 ];
        result.ez() = tr[ 3 ];
    }

    result *= sqrt( 0.25 / tr[ index ] );

    result.normalize();

    return result;
}

////////////////////////////////////////////////////////////////////////////////

Matrix3x3 Matrix3x3::getTransposed() const
{
    Matrix3x3 result( *this );

    result.transpose();

    return result;
}

////////////////////////////////////////////////////////////////////////////////

const Matrix3x3& Matrix3x3::operator= ( const Matrix3x3 &mtrx )
{
    setArray( mtrx._items );
    
    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

Matrix3x3 Matrix3x3::operator+ ( const Matrix3x3 &mtrx ) const
{
    Matrix3x3 result;

    for ( unsigned int i = 0; i < _size; i++ )
    {
        result._items[ i ] = _items[ i ] + mtrx._items[ i ];
    }
    
    return result;
}

////////////////////////////////////////////////////////////////////////////////

Matrix3x3 Matrix3x3::operator- ( const Matrix3x3 &mtrx ) const
{
    Matrix3x3 result;
    
    for ( unsigned int i = 0; i < _size; i++ )
    {
        result._items[ i ] = _items[ i ] - mtrx._items[ i ];
    }
    
    return result;
}

////////////////////////////////////////////////////////////////////////////////

Matrix3x3 Matrix3x3::operator* ( double value ) const
{
    Matrix3x3 result;
    
    for ( unsigned int i = 0; i < _size; i++ )
    {
        result._items[ i ] = _items[ i ] * value;
    }
    
    return result;
}

////////////////////////////////////////////////////////////////////////////////

Matrix3x3 Matrix3x3::operator* ( const Matrix3x3 &mtrx ) const
{
    Matrix3x3 result;

    for ( unsigned int r = 0; r < _rows; r++ )
    {
        for ( unsigned int c = 0; c < _cols; c++ )
        {
            result(r,c) = 0.0;

            for ( unsigned int i = 0; i < _cols; i++ )
            {
                result(r,c) += ( _items[ r*_cols + i ] * mtrx(i,c) );
            }
        }
    }
    
    return result;
}

////////////////////////////////////////////////////////////////////////////////

Vector3 Matrix3x3::operator* ( const Vector3 &vect ) const
{
    Vector3 result;

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

Matrix3x3 Matrix3x3::operator/ ( double value ) const
{
    Matrix3x3 result;
    
    for ( unsigned int i = 0; i < _size; i++ )
    {
        result._items[ i ] = _items[ i ] / value;
    }
    
    return result;
}

////////////////////////////////////////////////////////////////////////////////

Matrix3x3& Matrix3x3::operator+= ( const Matrix3x3 &mtrx )
{
    for ( unsigned int i = 0; i < _size; i++ )
    {
        _items[ i ] += mtrx._items[ i ];
    }
    
    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

Matrix3x3& Matrix3x3::operator-= ( const Matrix3x3 &mtrx )
{
    for ( unsigned int i = 0; i < _size; i++ )
    {
        _items[ i ] -= mtrx._items[ i ];
    }
    
    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

Matrix3x3& Matrix3x3::operator*= ( double value )
{
    for ( unsigned int i = 0; i < _size; i++ )
    {
        _items[ i ] *= value;
    }
    
    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

Matrix3x3& Matrix3x3::operator/= ( double value )
{
    for ( unsigned int i = 0; i < _size; i++ )
    {
        _items[ i ] /= value;
    }
    
    return (*this);
}
