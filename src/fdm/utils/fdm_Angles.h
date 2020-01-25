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
#ifndef FDM_ANGLES_H
#define FDM_ANGLES_H

////////////////////////////////////////////////////////////////////////////////

#include <string>

#include <fdm/fdm_Defines.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Tait-Bryant angles class.
 *
 * This class represents three angles of rotation about three axes in ZYX
 * convention.
 *
 * @see https://en.wikipedia.org/wiki/Euler_angles
 */
class FDMEXPORT Angles
{
public:

    /** Degree-Minute-Second subdivision data struct. */
    struct DegMinSec
    {
        int    deg;     ///< degree part
        int    min;     ///< minute part
        double sec;     ///< second part
    };

    /**
     * Normalizes angle within [min,min+2*pi] range.
     * @param val angle to be normalized
     * @param min minimum value
     * @return normalized angle
     */
    static double normalize( double val, double min = 0.0 );

    /**
     * @brief Returns angle represented as Degree-Minute-Second.
     * @param val [rad] given angle
     * @return Degree-Minute-Second representation
     */
    static DegMinSec toDegMinSec( double val );

    /** Constructor. */
    Angles();

    /** Copy constructor. */
    Angles( const Angles &angl );

    /**
     * Constructor.
     * @param [rad] angle of rotation about x-axis
     * @param [rad] angle of rotation about y-axis
     * @param [rad] angle of rotation about z-axis
     */
    Angles( double phi, double tht, double psi );

    /** @return 1 if all items are valid */
    bool isValid() const;

    /**
     * @brief Normalizes angles.
     * Limits phi to [-pi,pi] theta to [-pi/2,pi/2] and psi to [0,2*pi].
     */
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
