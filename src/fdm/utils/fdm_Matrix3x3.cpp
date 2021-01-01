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

#include <fdm/utils/fdm_Matrix3x3.h>

#ifdef _MSC_VER
#   include <algorithm>
#endif

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Matrix3x3 Matrix3x3::identityMatrix()
{
    return Matrix3x3( 1.0, 0.0, 0.0,
                      0.0, 1.0, 0.0,
                      0.0, 0.0, 1.0 );
}

////////////////////////////////////////////////////////////////////////////////

Matrix3x3::Matrix3x3() :
    Matrix< 3,3 >(),

    _xx ( _items[ 0 ] ),
    _xy ( _items[ 1 ] ),
    _xz ( _items[ 2 ] ),
    _yx ( _items[ 3 ] ),
    _yy ( _items[ 4 ] ),
    _yz ( _items[ 5 ] ),
    _zx ( _items[ 6 ] ),
    _zy ( _items[ 7 ] ),
    _zz ( _items[ 8 ] )
{}

////////////////////////////////////////////////////////////////////////////////

Matrix3x3::Matrix3x3( const Matrix3x3 &mtrx ) :
    Matrix< 3,3 >( mtrx ),

    _xx ( _items[ 0 ] ),
    _xy ( _items[ 1 ] ),
    _xz ( _items[ 2 ] ),
    _yx ( _items[ 3 ] ),
    _yy ( _items[ 4 ] ),
    _yz ( _items[ 5 ] ),
    _zx ( _items[ 6 ] ),
    _zy ( _items[ 7 ] ),
    _zz ( _items[ 8 ] )
{}

////////////////////////////////////////////////////////////////////////////////

Matrix3x3::Matrix3x3( const double items[] ) :
    Matrix< 3,3 >( items ),

    _xx ( _items[ 0 ] ),
    _xy ( _items[ 1 ] ),
    _xz ( _items[ 2 ] ),
    _yx ( _items[ 3 ] ),
    _yy ( _items[ 4 ] ),
    _yz ( _items[ 5 ] ),
    _zx ( _items[ 6 ] ),
    _zy ( _items[ 7 ] ),
    _zz ( _items[ 8 ] )
{}

////////////////////////////////////////////////////////////////////////////////

Matrix3x3::Matrix3x3( double xx, double xy, double xz,
                      double yx, double yy, double yz,
                      double zx, double zy, double zz ) :
    Matrix< 3,3 >(),

    _xx ( _items[ 0 ] ),
    _xy ( _items[ 1 ] ),
    _xz ( _items[ 2 ] ),
    _yx ( _items[ 3 ] ),
    _yy ( _items[ 4 ] ),
    _yz ( _items[ 5 ] ),
    _zx ( _items[ 6 ] ),
    _zy ( _items[ 7 ] ),
    _zz ( _items[ 8 ] )
{
    _xx = xx;
    _xy = xy;
    _xz = xz;

    _yx = yx;
    _yy = yy;
    _yz = yz;

    _zx = zx;
    _zy = zy;
    _zz = zz;
}

////////////////////////////////////////////////////////////////////////////////

Matrix3x3::Matrix3x3( const Angles &angl ) :
    Matrix< 3,3 >(),

    _xx ( _items[ 0 ] ),
    _xy ( _items[ 1 ] ),
    _xz ( _items[ 2 ] ),
    _yx ( _items[ 3 ] ),
    _yy ( _items[ 4 ] ),
    _yz ( _items[ 5 ] ),
    _zx ( _items[ 6 ] ),
    _zy ( _items[ 7 ] ),
    _zz ( _items[ 8 ] )
{
    double sinPhi = sin( angl.phi() );
    double cosPhi = cos( angl.phi() );

    double sinTht = sin( angl.tht() );
    double cosTht = cos( angl.tht() );

    double sinPsi = sin( angl.psi() );
    double cosPsi = cos( angl.psi() );

    double sinPhiSinTht = sinPhi * sinTht;
    double cosPhiSinTht = cosPhi * sinTht;

    _xx =  cosTht * cosPsi;
    _xy =  cosTht * sinPsi;
    _xz = -sinTht;

    _yx = -( cosPhi * sinPsi ) + ( sinPhiSinTht * cosPsi );
    _yy =  ( cosPhi * cosPsi ) + ( sinPhiSinTht * sinPsi );
    _yz =  ( sinPhi * cosTht );

    _zx =  ( sinPhi * sinPsi ) + ( cosPhiSinTht * cosPsi );
    _zy = -( sinPhi * cosPsi ) + ( cosPhiSinTht * sinPsi );
    _zz =  ( cosPhi * cosTht );
}

////////////////////////////////////////////////////////////////////////////////

Matrix3x3::Matrix3x3( const Quaternion &qtrn ) :
    Matrix< 3,3 >(),

    _xx ( _items[ 0 ] ),
    _xy ( _items[ 1 ] ),
    _xz ( _items[ 2 ] ),
    _yx ( _items[ 3 ] ),
    _yy ( _items[ 4 ] ),
    _yz ( _items[ 5 ] ),
    _zx ( _items[ 6 ] ),
    _zy ( _items[ 7 ] ),
    _zz ( _items[ 8 ] )
{
    double e0 = qtrn.e0();
    double ex = qtrn.ex();
    double ey = qtrn.ey();
    double ez = qtrn.ez();

    double e02 = e0*e0;
    double ex2 = ex*ex;
    double ey2 = ey*ey;
    double ez2 = ez*ez;

    _xx = e02 + ex2 - ey2 - ez2;
    _xy = 2.0 * ( e0*ez + ex*ey );
    _xz = 2.0 * ( ex*ez - e0*ey );

    _yx = 2.0 * ( ex*ey - e0*ez );
    _yy = e02 - ex2 + ey2 - ez2;
    _yz = 2.0 * ( e0*ex + ey*ez );

    _zx = 2.0 * ( e0*ey + ex*ez );
    _zy = 2.0 * ( ey*ez - e0*ex );
    _zz = e02 - ex2 - ey2 + ez2;
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

    double sinTht = -_xz;
    double cosTht = sqrt( 1.0 - std::min( 1.0, sinTht*sinTht ) );

    result.tht() = atan2( sinTht, cosTht );

    if ( cosTht > 0.0 )
    {
        result.phi() = atan2( _yz,  _zz );
        result.psi() = atan2( _xy,  _xx );
    }
    else
    {
        result.phi() = atan2( _yx, -_zx );
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

    tr[ 0 ] = 1.0 + _xx + _yy + _zz;
    tr[ 1 ] = 1.0 + _xx - _yy - _zz;
    tr[ 2 ] = 1.0 - _xx + _yy - _zz;
    tr[ 3 ] = 1.0 - _xx - _yy + _zz;

    int index = 0;
    for ( int i = 1; i < 4; i++ ) index = ( tr[ i ] > tr[ index ] ) ? i : index;

    if ( index == 0 )
    {
        result.e0() = tr[ 0 ];
        result.ex() = _yz - _zy;
        result.ey() = _zx - _xz;
        result.ez() = _xy - _yx;
    }
    else if ( index == 1 )
    {
        result.e0() = _yz - _zy;
        result.ex() = tr[ 1 ];
        result.ey() = _xy + _yx;
        result.ez() = _zx + _xz;
    }
    else if ( index == 2 )
    {
        result.e0() = _zx - _xz;
        result.ex() = _xy + _yx;
        result.ey() = tr[ 2 ];
        result.ez() = _yz + _zy;
    }
    else
    {
        result.e0() = _xy - _yx;
        result.ex() = _zx + _xz;
        result.ey() = _yz + _zy;
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
