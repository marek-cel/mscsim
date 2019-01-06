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
#ifndef FDM_STABILIZERHOR_H
#define FDM_STABILIZERHOR_H

////////////////////////////////////////////////////////////////////////////////

#include <fdmMain/fdm_Stabilizer.h>
#include <fdmUtils/fdm_Table.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Horizontal stabilizer class.
 *
 * <h5>XML configuration file format:</h5>
 * @code
 * <horizontal_stabilizer>
 *   <position> { [m] x-coordinate } { [m] y-coordinate } { [m] z-coordinate } </position>
 *   <area> { [m^2] stabilizer area } </area>
 *   <incidence> { [deg] stabilizer incidence } </incidence>
 *   <downwash> { [-] coefficient of downwash due to the wing angle of attack } </downwash>
 *   <cx>
 *     { [deg] stabilizer angle of attack } { [-] stabilizer drag coefficient }
 *     ... { more entries }
 *   </cx>
 *   <cz>
 *     { [deg] stabilizer angle of attack } { [-] stabilizer lift coefficient }
 *     ... { more entries }
 *   </cz>
 * </horizontal_stabilizer>
 * @endcode
 *
 * @see Etkin B.: Dynamics of Atmosferic Flight, 1972, p.210
 * @see Raymer D.: Aircraft Design: A Conceptual Approach, 1992, p.426
 * @see Paturski Z.: Przewodnik po projektach z Mechaniki Lotu, Projekt nr 9: Rownowaga podluzna samolotu i sily na sterownicy wysokosci, p.IX-4. [in Polish]
 */
class FDMEXPORT StabilizerHor : public Stabilizer
{
public:

    /** Constructor. */
    StabilizerHor();

    /** Destructor. */
    virtual ~StabilizerHor();

    /**
     * Reads data.
     * @param dataNode XML node
     */
    virtual void readData( XmlNode &dataNode );

protected:

    double m_incidence;         ///< [rad] stabilizer incidence angle
    double m_downwash;          ///< [-] coefficient of downwash due to the wing angle of attack

    Table m_cx;                 ///< [-] drag coefficient vs angle of attach
    Table m_cz;                 ///< [-] lift coefficient vs angle of attach

    /**
     * Computes stabilizer angle of attack.
     * @param vel_air_bas [m/s] stabilizer linear velocity relative to the air expressed in BAS
     * @param angleOfAttackWing [rad] wing angle of attach
     * @return [rad] stabilizer angle of attack
     */
    virtual double getAngleOfAttack( const Vector3 &vel_air_bas, double angleOfAttackWing );
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_STABILIZERHOR_H
