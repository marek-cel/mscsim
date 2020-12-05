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
#ifndef FDM_FUSELAGE_H
#define FDM_FUSELAGE_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/utils/fdm_Table1.h>
#include <fdm/utils/fdm_Vector3.h>

#include <fdm/xml/fdm_XmlNode.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Fuselage class.
 *
 * XML configuration file format:
 * @code
 * <fuselage>
 *   <aero_center> { [m] x-coordinate } { [m] y-coordinate } { [m] z-coordinate } </aero_center>
 *   <length> { [m] fuselage reference length } </length>
 *   <width> { [m] fuselage reference width } </width>
 *   <area> { [m^2] fuselage reference area } </area>
 *   <cx>
 *     { [rad] angle of attack } { [-] drag coefficient }
 *     ... { more entries }
 *   </cx>
 *   [<cy>
 *     { [rad] angle of sideslip } { [-] sideforce coefficient }
 *     ... { more entries }
 *   </cy>]
 *   [<cz>
 *     { [rad] angle of attack } { [-] lift coefficient }
 *     ... { more entries }
 *   </cz>]
 *   [<cl>
 *     { [rad] angle of sideslip } { [-] rolling moment coefficient }
 *     ... { more entries }
 *   </cl>]
 *   [<cm>
 *     { [rad] angle of attack } { [-] pitching moment coefficient }
 *     ... { more entries }
 *   </cm>]
 *   [<cn>
 *     { [rad] angle of sideslip } { [-] yawing moment coefficient }
 *     ... { more entries }
 *   </cn>]
 * </fuselage>
 * @endcode
 *
 * Optional elements: "cy", "cz", "cl", "cm", "cn"
 *
 * @see Talbot P., et al.: A Mathematical Model of a Single Main Rototr Helicopter for Piloted Simulation. NASA, TM-84281, 1982
 */
class FDMEXPORT Fuselage
{
public:

    /** @brief Constructor. */
    Fuselage();

    /** @brief Destructor. */
    virtual ~Fuselage();

    /**
     * @brief Reads data.
     * @param dataNode XML node
     */
    virtual void readData( XmlNode &dataNode );

    /**
     * @brief Computes force and moment.
     * @param vel_air_bas [m/s] aircraft linear velocity relative to the air expressed in BAS
     * @param omg_air_bas [rad/s] aircraft angular velocity relative to the air expressed in BAS
     * @param airDensity [kg/m^3] air density
     * @param inducedVelocity [m/s] rotor induced velocity
     * @param wakeSkewAngle [rad] rotor wake skew angle
     */
    virtual void computeForceAndMoment( const Vector3 &vel_air_bas,
                                        const Vector3 &omg_air_bas,
                                        double airDensity,
                                        double inducedVelocity = 0.0,
                                        double wakeSkewAngle = 0.0 );

    inline const Vector3& getFor_BAS() const { return _for_bas; }
    inline const Vector3& getMom_BAS() const { return _mom_bas; }

protected:

    Vector3 _for_bas;           ///< [N] total force vector expressed in BAS
    Vector3 _mom_bas;           ///< [N*m] total moment vector expressed in BAS

    Vector3 _r_ac_bas;          ///< [m] fuselage aerodynamic center expressed in BAS

    Table1 _cx;                 ///< [-] drag coefficient vs [rad] angle of attack
    Table1 _cy;                 ///< [-] sideforce coefficient vs [rad] angle of sideslip
    Table1 _cz;                 ///< [-] lift coefficient vs [rad] angle of attack
    Table1 _cl;                 ///< [-] rolling moment coefficient vs [rad] angle of sideslip
    Table1 _cm;                 ///< [-] pitching moment coefficient vs [rad] angle of attack
    Table1 _cn;                 ///< [-] yawing moment coefficient vs [rad] angle of sideslip

    double _length;             ///< [m] reference length
    double _area;               ///< [m^2] reference area

    double _sl;                 ///< [m^3] S*l where S is reference area and l is reference length

    double _angleOfAttack;      ///< [rad] angle of attack
    double _sideslipAngle;      ///< [rad] angle of sideslip

    /**
     * @brief Computes drag coefficient.
     * @param angleOfAttack [rad] angle of attack
     * @return [-] drag coefficient
     */
    virtual double getCx( double angleOfAttack ) const;

    /**
     * @brief Computes sideforce coefficient.
     * @param sideslipAngle [rad] angle of sideslip
     * @return [-] sideforce coefficient
     */
    virtual double getCy( double sideslipAngle ) const;

    /**
     * @brief Computes lift coefficient.
     * @param angleOfAttack [rad] angle of attack
     * @return [-] lift coefficient
     */
    virtual double getCz( double angleOfAttack ) const;

    /**
     * @brief Computes rolling moment coefficient.
     * @param sideslipAngle [rad] angle of sideslip
     * @return [-] rolling moment coefficient
     */
    virtual double getCl( double sideslipAngle ) const;

    /**
     * @brief Computes pitching moment coefficient.
     * @param angleOfAttack [rad] angle of attack
     * @return [-] pitching moment coefficient
     */
    virtual double getCm( double angleOfAttack ) const;

    /**
     * @brief Computes yawing moment coefficient.
     * @param sideslipAngle [rad] angle of sideslip
     * @return [-] yawing moment coefficient
     */
    virtual double getCn( double sideslipAngle ) const;
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_FUSELAGE_H
