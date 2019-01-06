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
#ifndef FDM_SCHRENKWING_H
#define FDM_SCHRENKWING_H

////////////////////////////////////////////////////////////////////////////////

#include <fdmMain/fdm_Wing.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Wing Schrenk approximation class.
 *
 * <h3>XML configuration file format:</h3>
 * @code
 * <wing>
 *   <ac_l> { [m] x-coordinate } { [m] y-coordinate } { [m] z-coordinate } </ac_l>
 *   <ac_r> { [m] x-coordinate } { [m] y-coordinate } { [m] z-coordinate } </ac_r>
 *   <span> { [m] wing span } </span>
 *   <mac> { [m] wing mean aerodynamic chord } </mac>
 *   <area> { [m^2] wing area } </area>
 *   <chord>
 *     { [m] spanwise coordinate } { [m] wing chord }
 *     ... { more entries }
 *   </chord>
 *   <cx>
 *     { [deg] wing angle of attack } { [-] wing drag coefficient }
 *     ... { more entries }
 *   </cx>
 *   <cz>
 *     { [deg] wing angle of attack } { [-] wing lift coefficient }
 *     ... { more entries }
 *   </cz>
 *   <cm>
 *     { [deg] wing angle of attack } { [-] wing pitching moment coefficient }
 *     ... { more entries }
 *   </cm>
 * </wing>
 * @endcode
 *
 * @see A Simple Approximation Method for Obtaining the Spanwise Lift Distribution, NACA, TM-948
 * @see Galinski C.: Wybrane zagadnienia projektowania samolotow, 2016, p.118. [in Polish]
 */
class FDMEXPORT SchrenkWing : public Wing
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
    SchrenkWing();

    /** Destructor. */
    virtual ~SchrenkWing();

    /**
     * Reads data.
     * @param dataNode XML node
     */
    virtual void readData( XmlNode &dataNode );

protected:

    double m_span;              ///< [m] wing span

    Table m_chord;              ///< [m] wing chord vs [m] spanwise coordinate

    double m_4S_bpi;            ///< [m] 4*S/(b*pi) where S is wing area and b is wing span
    double m_2_b;               ///< [1/m] 2/b where b is wing span

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
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_SCHRENKWING_H
