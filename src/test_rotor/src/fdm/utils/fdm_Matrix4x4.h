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
#ifndef FDM_MATRIX4X4_H
#define FDM_MATRIX4X4_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/utils/fdm_Matrix.h>
#include <fdm/utils/fdm_Vector4.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief 4 by 4 matrix class.
 */
class FDMEXPORT Matrix4x4 : public Matrix< 4,4 >
{
public:

    /** Constructor. */
    Matrix4x4();

    /** Copy constructor. */
    Matrix4x4( const Matrix4x4 &mtrx );

    /** Constructor. */
    Matrix4x4( const double items[] );

    /** Transposes matrix. */
    void transpose();

    /** Returns transposed matrix. */
    Matrix4x4 getTransposed() const;

    /** Assignment operator. */
    const Matrix4x4& operator= ( const Matrix4x4 &mtrx );

    /** Addition operator. */
    Matrix4x4 operator+ ( const Matrix4x4 &mtrx ) const;

    /** Subtraction operator. */
    Matrix4x4 operator- ( const Matrix4x4 &mtrx ) const;

    /** Multiplication operator (by scalar). */
    Matrix4x4 operator* ( double value ) const;

    /** Multiplication operator (by matrix). */
    Matrix4x4 operator* ( const Matrix4x4 &mtrx ) const;

    /** Multiplication operator (by vector). */
    Vector4 operator* ( const Vector4 &vect ) const;

    /** Division operator (by scalar). */
    Matrix4x4 operator/ ( double value ) const;

    /** Unary addition operator. */
    Matrix4x4& operator+= ( const Matrix4x4 &mtrx );

    /** Unary subtraction operator. */
    Matrix4x4& operator-= ( const Matrix4x4 &mtrx );

    /** Unary multiplication operator (by scalar). */
    Matrix4x4& operator*= ( double value );

    /** Unary division operator (by scalar). */
    Matrix4x4& operator/= ( double value );
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

/** Multiplication operator (by scalar). */
inline fdm::Matrix4x4 operator* ( double value, const fdm::Matrix4x4 &mtrx )
{
    return ( mtrx * value );
}

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_MATRIX4X4_H
