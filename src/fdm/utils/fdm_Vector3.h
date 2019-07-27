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
#ifndef FDM_VECTOR3_H
#define FDM_VECTOR3_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/utils/fdm_Quaternion.h>
#include <fdm/utils/fdm_Vector.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief 3 elements column vector class.
 */
class FDMEXPORT Vector3 : public Vector< 3 >
{
public:

    static const Vector3 m_ux;
    static const Vector3 m_uy;
    static const Vector3 m_uz;

    /** Constructor. */
    Vector3();

    /** Copy constructor. */
    Vector3( const Vector3 &vect );

    /** Constructor. */
    Vector3( const double items[] );

    /** Constructor. */
    Vector3( double x, double y, double z );

    /** @return normalized vector */
    Vector3 getNormalized() const;

    /** Performs vector passive (alias) rotation. */
    Vector3 rotate( const Quaternion quat );

    inline double  x() const { return m_x; }
    inline double  y() const { return m_y; }
    inline double  z() const { return m_z; }
    inline double& x()       { return m_x; }
    inline double& y()       { return m_y; }
    inline double& z()       { return m_z; }

    /** Sets vector values. */
    void set( double x, double y, double z );

    /** Assignment operator. */
    const Vector3& operator= ( const Vector3 &vect );

    /** Addition operator. */
    Vector3 operator+ ( const Vector3 &vect ) const;

    /** Negation operator. */
    Vector3 operator- () const;

    /** Subtraction operator. */
    Vector3 operator- ( const Vector3 &vect ) const;

    /** Multiplication operator (by scalar). */
    Vector3 operator* ( double val ) const;

    /** Division operator (by scalar). */
    Vector3 operator/ ( double val ) const;

    /** Dot product operator. */
    double operator* ( const Vector3 &vect ) const;

    /** Cross product operator. */
    Vector3 operator^ ( const Vector3 &vect ) const;

    /** Unary addition operator. */
    Vector3& operator+= ( const Vector3 &vect );

    /** Unary subtraction operator. */
    Vector3& operator-= ( const Vector3 &vect );

    /** Unary multiplication operator (by scalar). */
    Vector3& operator*= ( double val );

    /** Unary division operator (by scalar). */
    Vector3& operator/= ( double val );

    /** Unary cross product operator. */
    Vector3& operator^= ( const Vector3 &vect );

private:

    double &m_x;    ///<
    double &m_y;    ///<
    double &m_z;    ///<
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

/** Multiplication operator (by scalar). */
inline fdm::Vector3 operator* ( double val, const fdm::Vector3 &vect )
{
    return ( vect * val );
}

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_VECTOR3_H
