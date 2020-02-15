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
#ifndef FDM_SCHRENK_H
#define FDM_SCHRENK_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/utils/fdm_Table.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Wing Schrenk approximation class.
 *
 * @see A Simple Approximation Method for Obtaining the Spanwise Lift Distribution, NACA, TM-948
 * @see Galinski C.: Wybrane zagadnienia projektowania samolotow, 2016, p.118. [in Polish]
 */
class FDMEXPORT Schrenk
{
public:

    /**
     * Returns mean aerodynamic chord.
     * @see Raymer D.: Aircraft Design: A Conceptual Approach, 1992, p.49
     * @param cr [m] chord at wing root
     * @param ct [m] chord at wing tip
     * @return [m] mean aerodynamic chord
     */
    static double getMeanAerodynamicChord( double cr, double ct );

    /**
     * Returns mean aerodynamic chord.
     * @param chord [m] wing chord vs [m] spanwise coordinate
     * @return [m] mean aerodynamic chord
     */
    static double getMeanAerodynamicChord( const Table &chord );

    /** Constructor. */
    Schrenk();

    /** Destructor. */
    virtual ~Schrenk();

    /**
     * Computes simple approximation of normalized spanwise drag coefficient
     * distribution.
     * @param y [m] spanwise coordinate where 0: wing root (plane of symmetry)
     * @return [-] spanwise drag coefficient
     */
    virtual double getDragCoefDist( double y ) const;

    /**
     * Computes Schrenk approximation of normalized spanwise lift coefficient
     * distribution.
     * @param y [m] spanwise coordinate where 0: wing root (plane of symmetry)
     * @return [-] spanwise lift coefficient
     */
    virtual double getLiftCoefDist( double y ) const;

    /**
     * Sets wing area.
     * @param area [m^2] wing area
     */
    void setArea( double area );

    /**
     * Sets wing span.
     * @param span [m] wing span
     */
    void setSpan( double span );

    /**
     * Sets wing chord.
     * @param chord [m] wing chord vs [m] spanwise coordinate
     */
    void setChord( const Table &chord );

protected:

    double _area;               ///< [m^2] wing area
    double _span;               ///< [m] wing span

    Table _chord;               ///< [m] wing chord vs [m] spanwise coordinate

    double _4S_bpi;             ///< [m] 4*S/(b*pi) where S is wing area and b is wing span
    double _2_b;                ///< [1/m] 2/b where b is wing span

    /** */
    void updateAxiliaryParameters();
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_SCHRENK_H
