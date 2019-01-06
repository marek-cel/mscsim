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
#ifndef FDM_MATRIX6X6_H
#define FDM_MATRIX6X6_H

////////////////////////////////////////////////////////////////////////////////

#include <fdmUtils/fdm_Matrix.h>
#include <fdmUtils/fdm_Vector6.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief 6 by 6 matrix class.
 */
class FDMEXPORT Matrix6x6 : public Matrix< 6,6 >
{
public:

    /** Constructor. */
    Matrix6x6();

    /** Copy constructor. */
    Matrix6x6( const Matrix6x6 &mtrx );

    /** Constructor. */
    Matrix6x6( const double items[] );

    /** Transposes matrix. */
    void transpose();

    /** Returns transposed matrix. */
    Matrix6x6 getTransposed() const;

    /** Assignment operator. */
    const Matrix6x6& operator= ( const Matrix6x6 &mtrx );

    /** Addition operator. */
    Matrix6x6 operator+ ( const Matrix6x6 &mtrx ) const;

    /** Subtraction operator. */
    Matrix6x6 operator- ( const Matrix6x6 &mtrx ) const;

    /** Multiplication operator (by scalar). */
    Matrix6x6 operator* ( double value ) const;

    /** Multiplication operator (by matrix). */
    Matrix6x6 operator* ( const Matrix6x6 &mtrx ) const;

    /** Multiplication operator (by vector). */
    Vector6 operator* ( const Vector6 &vect ) const;

    /** Division operator (by scalar). */
    Matrix6x6 operator/ ( double value ) const;

    /** Unary addition operator. */
    Matrix6x6& operator+= ( const Matrix6x6 &mtrx );

    /** Unary subtraction operator. */
    Matrix6x6& operator-= ( const Matrix6x6 &mtrx );

    /** Unary multiplication operator (by scalar). */
    Matrix6x6& operator*= ( double value );

    /** Unary division operator (by scalar). */
    Matrix6x6& operator/= ( double value );
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

/** Binary multiplication by scalar operator. */
inline fdm::Matrix6x6 operator* ( double value, const fdm::Matrix6x6 &mtrx )
{
    return ( mtrx * value );
}

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_MATRIX6X6_H
