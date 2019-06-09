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
#ifndef FDM_VECTOR6_H
#define FDM_VECTOR6_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/utils/fdm_Vector.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief 6 elements column vector class.
 */
class FDMEXPORT Vector6 : public Vector< 6 >
{
public:

    /** Constructor. */
    Vector6();

    /** Copy constructor. */
    Vector6( const Vector6 &vect );

    /** Constructor. */
    Vector6( const double items[] );

    /** @return normalized vector */
    Vector6 getNormalized() const;

    /** Assignment operator. */
    const Vector6& operator= ( const Vector6 &vect );

    /** Addition operator. */
    Vector6 operator+ ( const Vector6 &vect ) const;

    /** Negation operator. */
    Vector6 operator- () const;

    /** Subtraction operator. */
    Vector6 operator- ( const Vector6 &vect ) const;

    /** Multiplication operator (by scalar). */
    Vector6 operator* ( double val ) const;

    /** Division operator (by scalar). */
    Vector6 operator/ ( double val ) const;

    /** Unary addition operator. */
    Vector6& operator+= ( const Vector6 &vect );

    /** Unary subtraction operator. */
    Vector6& operator-= ( const Vector6 &vect );

    /** Unary multiplication operator (by scalar). */
    Vector6& operator*= ( double val );

    /** Unary division operator (by scalar). */
    Vector6& operator/= ( double val );

};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

/** Multiplication operator (by scalar). */
inline fdm::Vector6 operator* ( double val, const fdm::Vector6 & vect )
{
    return ( vect * val );
}

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_VECTOR6_H
