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
#ifndef FDM_MATRIX3X3_H
#define FDM_MATRIX3X3_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/utils/fdm_Angles.h>
#include <fdm/utils/fdm_Matrix.h>
#include <fdm/utils/fdm_Quaternion.h>
#include <fdm/utils/fdm_Vector3.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief 3 by 3 matrix class.
 * @see Allerton D.: Principles of Flight Simulation, 2009, p.120
 * @see Zipfel P.: Modeling and Simulation of Aerospace Vehicle Dynamics, 2007, p.373
 * @see Bociek S., Gruszecki J.: Uklady sterowania automatycznego samolotem, 1999, p.23. [in Polish]
 * @see Sibilski K.: Modelowanie i symulacja dynamiki ruchu obiektow latajacych, 2004, p.34. [in Polish]
 * @see Matulewski J., et. al.: Grafika fizyka metody numeryczne, 2010, p.529. [in Polish]
 * @see https://en.wikipedia.org/wiki/Rotation_matrix
 * @see https://en.wikipedia.org/wiki/Euler_angles
 * @see https://en.wikipedia.org/wiki/Active_and_passive_transformation
 */
class FDMEXPORT Matrix3x3 : public Matrix< 3,3 >
{
public:

    /** Creates identity matrix. */
    static Matrix3x3 createIdentityMatrix();

    /** Constructor. */
    Matrix3x3();

    /** Copy constructor. */
    Matrix3x3( const Matrix3x3 &mtrx );

    /** Constructor. */
    Matrix3x3( const double items[] );

    /** Constructor. */
    Matrix3x3( double xx, double xy, double xz,
               double yx, double yy, double yz,
               double zx, double zy, double zz );

    /** Creates passive (alias) rotation matrix. */
    Matrix3x3( const Angles &angl );

    /** Creates passive (alias) rotation matrix. */
    Matrix3x3( const Quaternion &qtrn );

    /** Transposes matrix. */
    void transpose();

    /** Returns Bryant angles of rotation matrix. */
    Angles getAngles() const;

    /** Returns quaternion of rotation matrix. */
    Quaternion getQuaternion() const;

    /** Returns transposed matrix. */
    Matrix3x3 getTransposed() const;

    /** Assignment operator. */
    const Matrix3x3& operator= ( const Matrix3x3 &mtrx );

    /** Addition operator. */
    Matrix3x3 operator+ ( const Matrix3x3 &mtrx ) const;

    /** Subtraction operator. */
    Matrix3x3 operator- ( const Matrix3x3 &mtrx ) const;

    /** Multiplication operator (by scalar). */
    Matrix3x3 operator* ( double value ) const;

    /** Multiplication operator (by matrix). */
    Matrix3x3 operator* ( const Matrix3x3 &mtrx ) const;

    /** Multiplication operator (by vector). */
    Vector3 operator* ( const Vector3 &vect ) const;

    /** Division operator (by scalar). */
    Matrix3x3 operator/ ( double value ) const;

    /** Unary addition operator. */
    Matrix3x3& operator+= ( const Matrix3x3 &mtrx );

    /** Unary subtraction operator. */
    Matrix3x3& operator-= ( const Matrix3x3 &mtrx );

    /** Unary multiplication operator (by scalar). */
    Matrix3x3& operator*= ( double value );

    /** Unary division operator (by scalar). */
    Matrix3x3& operator/= ( double value );
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

/** Multiplication operator (by scalar). */
inline fdm::Matrix3x3 operator* ( double value, const fdm::Matrix3x3 &mtrx )
{
    return ( mtrx * value );
}

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_MATRIX3X3_H
