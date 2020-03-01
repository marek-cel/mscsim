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
#ifndef FDM_STABILIZER_H
#define FDM_STABILIZER_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_Base.h>
#include <fdm/utils/fdm_Vector3.h>
#include <fdm/xml/fdm_XmlNode.h>

#include <fdm/utils/fdm_Table.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Generic stabilizer base class.
 *
 * This is generalized stabilizator class intended to represent both
 * horizontal and vertical stabilizers. For verical stabilizer angle of attack
 * becomes angle of sideslip.
 *
 * Type of stabilizer is determined by XML tag name "horizontal_stabilizer"
 * or "vertical_stabilizer".
 *
 * XML configuration file format:
 * @code
 * <stabilizer type="[horizontal|vertical]">
 *   <aerodynamic_center> { [m] x-coordinate } { [m] y-coordinate } { [m] z-coordinate } </aerodynamic_center>
 *   <area> { [m^2] area } </area>
 *   [<incidence> { [rad] incidence } </incidence>]
 *   [<downwash>
 *     { [rad] wing angle of attack } { [rad] downwash angle }
 *     ... { more entries }
 *   </downwash>]
 *   <cx>
 *     { [rad] angle } { [-] drag coefficient }
 *     ... { more entries }
 *   </cx>
 *   <cy>
 *     { [rad] angle } { [-] sideforce coefficient }
 *     ... { more entries }
 *   </cy>
 *   <cz>
 *     { [rad] angle } { [-] lift coefficient }
 *     ... { more entries }
 *   </cz>
 * </stabilizer>
 * @endcode
 *
 * Specify "cy" for vertical and "cz" for horizontal stabilizer.
 *
 * Optional elements: "incidence", "downwash", "cy" for horizontal stabilizer,
 * "cz" for vertical stabilizer
 */
class FDMEXPORT Stabilizer : public Base
{
public:

    /** Stabilizer type. */
    enum Type
    {
        Horizontal = 0,     ///< horizontal stabilizer
        Vertical   = 1      ///< vertical stabilizer
    };

    /**
     * Constructor.
     * @param type stabilizer type
     */
    Stabilizer( Type type = Horizontal );

    /** Destructor. */
    virtual ~Stabilizer();

    /**
     * Reads data.
     * @param dataNode XML node
     */
    virtual void readData( XmlNode &dataNode );

    /**
     * Computes force and moment.
     * @param vel_air_bas [m/s] aircraft linear velocity relative to the air expressed in BAS
     * @param omg_air_bas [rad/s] aircraft angular velocity relative to the air expressed in BAS
     * @param airDensity [kg/m^3] air density
     * @param wingAngleOfAttack [rad] wing angle of attack
     */
    virtual void computeForceAndMoment( const Vector3 &vel_air_bas,
                                        const Vector3 &omg_air_bas,
                                        double airDensity,
                                        double wingAngleOfAttack = 0.0 );

    inline const Vector3& getFor_BAS() const { return _for_bas; }
    inline const Vector3& getMom_BAS() const { return _mom_bas; }

protected:

    const Type _type;           ///< stabilizer type

    Vector3 _for_bas;           ///< [N] total force vector expressed in BAS
    Vector3 _mom_bas;           ///< [N*m] total moment vector expressed in BAS

    Vector3 _r_ac_bas;          ///< [m] stabilizer aerodynamic center expressed in BAS

    Table _cx;                  ///< [-] drag coefficient vs "angle of attack"
    Table _cy;                  ///< [-] sideforce coefficient vs "angle of attack"
    Table _cz;                  ///< [-] lift coefficient vs "angle of attack"

    Table _downwash;            ///< [rad] downwash angle vs wing angle of attack

    double _area;               ///< [m^2] stabilizer reference area

    double _incidence;          ///< [rad] stabilizer incidence angle

    /**
     * Computes stabilizer angle of attack.
     * @see Etkin B.: Dynamics of Atmosferic Flight, 1972, p.210
     * @see Raymer D.: Aircraft Design: A Conceptual Approach, 1992, p.426
     * @see Paturski Z.: Przewodnik po projektach z Mechaniki Lotu, Projekt nr 9: Rownowaga podluzna samolotu i sily na sterownicy wysokosci, p.IX-4. [in Polish]
     * @param vel_air_bas [m/s] stabilizer linear velocity relative to the air expressed in BAS
     * @param wingAngleOfAttack [rad] wing angle of attack
     * @return [rad] stabilizer angle of attack
     */
    virtual double getAngleOfAttack( const Vector3 &vel_air_bas,
                                     double wingAngleOfAttack );

    /**
     * Computes drag coefficient.
     * @param angle [rad] "angle of attack"
     * @return [-] drag coefficient
     */
    virtual double getCx( double angle ) const;

    /**
     * Computes sideforce coefficient.
     * @param angle [rad] "angle of attack"
     * @return [-] sideforce coefficient
     */
    virtual double getCy( double angle ) const;

    /**
     * Computes lift coefficient.
     * @param angle [rad] "angle of attack"
     * @return [-] lift coefficient
     */
    virtual double getCz( double angle ) const;
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_STABILIZERHOR_H
