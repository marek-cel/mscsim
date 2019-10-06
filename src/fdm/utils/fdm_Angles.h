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
#ifndef FDM_ANGLES_H
#define FDM_ANGLES_H

////////////////////////////////////////////////////////////////////////////////

#include <string>

#include <fdm/fdm_Defines.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Bryant angles class.
 *
 * @see https://en.wikipedia.org/wiki/Euler_angles
 */
class FDMEXPORT Angles
{
public:

    /** Constructor. */
    Angles();

    /** Copy constructor. */
    Angles( const Angles &angl );

    /**
     * Constructor.
     * @param [rad] angle of rotation along x-axis
     * @param [rad] angle of rotation along y-axis
     * @param [rad] angle of rotation along z-axis
     */
    Angles( double phi, double tht, double psi );

    /** @return 1 if all items are valid */
    bool isValid() const;

    /** Normalizes angles. */
    void normalize();

    inline double   phi() const { return _phi; }
    inline double   tht() const { return _tht; }
    inline double   psi() const { return _psi; }
    inline double & phi()       { return _phi; }
    inline double & tht()       { return _tht; }
    inline double & psi()       { return _psi; }

    /** Sets angles values. */
    void set( double phi, double tht, double psi );

    /** This function returns string represtation of the angles. */
    std::string toString() const;

    /** Assignment operator. */
    const Angles& operator= ( const Angles &angl );

    /** Equality operator. */
    bool operator== ( const Angles &angl ) const;

    /** Inequality operator. */
    bool operator!= ( const Angles &angl ) const;

private:

    double _phi;    ///< [rad] angle of rotation about x-axis
    double _tht;    ///< [rad] angle of rotation about y-axis
    double _psi;    ///< [rad] angle of rotation about z-axis
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_ANGLES_H
