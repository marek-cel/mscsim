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
#ifndef FDM_TAILOFF_H
#define FDM_TAILOFF_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_Base.h>

#include <fdm/utils/fdm_Table.h>
#include <fdm/utils/fdm_Vector3.h>

#include <fdm/xml/fdm_XmlNode.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Tail-off aircraft aerodynamics class.
 *
 * <p>Forces and moments are calculated, considering different airflow
 * conditions, separately for left and right half wings. Half wing aerodynamic
 * center is considered datum point for computing airflow conditions (airspeed,
 * angle of attack, etc.).</p>
 *
 * <h3>XML configuration file format:</h3>
 * @code
 * <tail_off>
 *   <aero_center_l> { [m] x-coordinate } { [m] y-coordinate } { [m] z-coordinate } </aero_center_l>
 *   <aero_center_r> { [m] x-coordinate } { [m] y-coordinate } { [m] z-coordinate } </aero_center_r>
 *   <mac> { [m] wing mean aerodynamic chord } </mac>
 *   <area> { [m^2] wing area } </area>
 *   <cx>
 *     { [rad] angle of attack } { [-] drag coefficient }
 *     ... { more entries }
 *   </cx>
 *   <cy>
 *     { [rad] angle of sideslip } { [-] sideforce coefficient }
 *     ... { more entries }
 *   </cy>
 *   <cz>
 *     { [rad] angle of attack } { [-] lift coefficient }
 *     ... { more entries }
 *   </cz>
 *   <cl>
 *     { [rad] angle of sideslip } { [-] rolling moment coefficient }
 *     ... { more entries }
 *   </cl>
 *   <cm>
 *     { [rad] angle of attack } { [-] pitching moment coefficient }
 *     ... { more entries }
 *   </cm>
 *   <cn>
 *     { [rad] angle of sideslip } { [-] yawing moment coefficient }
 *     ... { more entries }
 *   </cn>
 * </tail_off>
 * @endcode
 *
 * <p>Optional elements: "cy", "cl", "cn"</p>
 */
class FDMEXPORT TailOff : public Base
{
public:

    /** Constructor. */
    TailOff();

    /** Destructor. */
    virtual ~TailOff();

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
     */
    virtual void computeForceAndMoment( const Vector3 &vel_air_bas,
                                        const Vector3 &omg_air_bas,
                                        double airDensity );

    /**
     * Updates wing.
     * @param vel_air_bas [m/s] aircraft linear velocity relative to the air expressed in BAS
     * @param omg_air_bas [rad/s] aircraft angular velocity relative to the air expressed in BAS
     */
    virtual void update( const Vector3 &vel_air_bas, const Vector3 &omg_air_bas );

    inline const Vector3& getFor_BAS() const { return _for_bas; }
    inline const Vector3& getMom_BAS() const { return _mom_bas; }

    inline bool getStall() const { return _stall; }

protected:

    Vector3 _for_bas;           ///< [N] total force vector expressed in BAS
    Vector3 _mom_bas;           ///< [N*m] total moment vector expressed in BAS

    Vector3 _vel_l_bas;         ///< [m/s] left half wing airspeed
    Vector3 _vel_r_bas;         ///< [m/s] right half wing airspeed

    Vector3 _r_ac_l_bas;        ///< [m] left half wing aerodynamic center expressed in BAS
    Vector3 _r_ac_r_bas;        ///< [m] right half wing aerodynamic center expressed in BAS

    Table _cx;                  ///< [-] drag coefficient vs [rad] angle of attack
    Table _cy;                  ///< [-] sideforce coefficient vs [rad] angle of sideslip
    Table _cz;                  ///< [-] lift coefficient vs [rad] angle of attack
    Table _cl;                  ///< [-] rolling moment coefficient vs [rad] angle of sideslip
    Table _cm;                  ///< [-] pitching moment coefficient vs [rad] angle of attack
    Table _cn;                  ///< [-] yawing moment coefficient vs [rad] angle of sideslip

    double _mac;                ///< [m] wing mean aerodynamic chord
    double _area;               ///< [m^2] wing reference area

    double _area_2;             ///< [m^2] half wing reference area
    double _mac_s_2;            ///< [m^3] MAC*S/2 where MAC is mean aerodynamic chord and S is reference area

    double _aoa_critical_neg;   ///< [rad] critical angle of attack (negative)
    double _aoa_critical_pos;   ///< [rad] critical angle of attack (positive)

    double _aoa_l;              ///< [rad] left half wing angle of attack
    double _aoa_r;              ///< [rad] right half wing angle of attack

    bool _stall;                ///< specifies if wing is stalled

    /**
     * Adds half wing force and moment to the total force and moment.
     * @param r_ac_bas [m] half wing aerodynamic center expressed in BAS
     * @param vel_air_bas [m/s] aircraft linear velocity relative to the air expressed in BAS
     * @param omg_air_bas [rad/s] aircraft angular velocity relative to the air expressed in BAS
     * @param airDensity [kg/m^3] air density
     */
    virtual void addForceAndMoment( const Vector3 &r_ac_bas,
                                    const Vector3 &vel_air_bas,
                                    const Vector3 &omg_air_bas,
                                    double airDensity );

    /**
     * Computes drag coefficient.
     * @param angleOfAttack [rad] angle of attack
     * @return [-] drag coefficient
     */
    virtual double getCx( double angleOfAttack ) const;

    /**
     * Computes sideforce coefficient.
     * @param sideslipAngle [rad] angle of sideslip
     * @return [-] sideforce coefficient
     */
    virtual double getCy( double sideslipAngle ) const;

    /**
     * Computes lift coefficient.
     * @param angleOfAttack [rad] angle of attack
     * @return [-] lift coefficient
     */
    virtual double getCz( double angleOfAttack ) const;

    /**
     * Computes rolling moment coefficient.
     * @param sideslipAngle [rad] angle of sideslip
     * @return [-] rolling moment coefficient
     */
    virtual double getCl( double sideslipAngle ) const;

    /**
     * Computes pitching moment coefficient.
     * @param angleOfAttack [rad] angle of attack
     * @return [-] pitching moment coefficient
     */
    virtual double getCm( double angleOfAttack ) const;

    /**
     * Computes yawing moment coefficient.
     * @param sideslipAngle [rad] angle of sideslip
     * @return [-] yawing moment coefficient
     */
    virtual double getCn( double sideslipAngle ) const;
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_TAILOFF_H
