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
#ifndef FDM_WING_H
#define FDM_WING_H

////////////////////////////////////////////////////////////////////////////////

#include <fdmMain/fdm_Base.h>

#include <fdmUtils/fdm_Table.h>
#include <fdmUtils/fdm_Vector3.h>

#include <fdmXml/fdm_XmlNode.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Wing simple class.
 *
 * <p>Forces and moments are calculated, considering different airflow
 * conditions, separately for left and right half wings. Half wing aerodynamic
 * center is considered datum point for computing airflow conditions (airspeed,
 * angle of attack, etc.).</p>
 *
 * <h3>XML configuration file format:</h3>
 * @code
 * <wing>
 *   <ac_l> { [m] x-coordinate } { [m] y-coordinate } { [m] z-coordinate } </ac_l>
 *   <ac_r> { [m] x-coordinate } { [m] y-coordinate } { [m] z-coordinate } </ac_r>
 *   <mac> { [m] wing mean aerodynamic chord } </mac>
 *   <area> { [m^2] wing area } </area>
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
 * @see Torenbeek E: Synthesis of Subsonic Airplane Design, 1982, p.443
 * @see Raymer D: Aircraft Design: A Conceptual Approach, 1992, p.49
 */
class FDMEXPORT Wing : public Base
{
public:

    /** Constructor. */
    Wing();

    /** Destructor. */
    virtual ~Wing();

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
    void computeForceAndMoment( const Vector3 &vel_air_bas,
                                const Vector3 &omg_air_bas,
                                double airDensity );

    /**
     * Updates wing.
     * @param vel_air_bas [m/s] aircraft linear velocity relative to the air expressed in BAS
     * @param omg_air_bas [rad/s] aircraft angular velocity relative to the air expressed in BAS
     */
    void update( const Vector3 &vel_air_bas, const Vector3 &omg_air_bas );

    inline const Vector3& getFor_BAS() const { return m_for_bas; }
    inline const Vector3& getMom_BAS() const { return m_mom_bas; }

    inline bool getStall() const { return m_stall; }

protected:

    Vector3 m_for_bas;          ///< [N] total force vector expressed in BAS
    Vector3 m_mom_bas;          ///< [N*m] total moment vector expressed in BAS

    Vector3 m_vel_l_bas;        ///< [m/s] left half wing airspeed
    Vector3 m_vel_r_bas;        ///< [m/s] right half wing airspeed

    Vector3 m_ac_l_bas;         ///< [m] left half wing aerodynamic center expressed in BAS
    Vector3 m_ac_r_bas;         ///< [m] right half wing aerodynamic center expressed in BAS

    Table m_cx;                 ///< [-] drag coefficient vs [rad] angle of attack
    Table m_cz;                 ///< [-] lift coefficient vs [rad] angle of attack
    Table m_cm;                 ///< [-] pitching moment coefficient vs [rad] angle of attack

    double m_mac;               ///< [m] wing mean aerodynamic chord
    double m_area;              ///< [m^2] wing reference area

    double m_area_2;            ///< [m^2] half wing reference area
    double m_mac_s_2;           ///< [m^3] MAC*S/2 where MAC is mean aerodynamic chord and S is wing reference area

    double m_aoa_critical_neg;  ///< [rad] critical angle of attack (negative)
    double m_aoa_critical_pos;  ///< [rad] critical angle of attack (positive)

    double m_aoa_l;             ///< [rad] left half wing angle of attack
    double m_aoa_r;             ///< [rad] right half wing angle of attack

    bool m_stall;               ///< specifies if wing is stalled

    /**
     * Adds half wing force and moment to the total force and moment.
     * @param ac_bas [m] half wing aerodynamic center expressed in BAS
     * @param vel_air_bas [m/s] aircraft linear velocity relative to the air expressed in BAS
     * @param omg_air_bas [rad/s] aircraft angular velocity relative to the air expressed in BAS
     * @param airDensity [kg/m^3] air density
     */
    void addForceAndMoment( const Vector3 &ac_bas,
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
     * Computes side force coefficient.
     * @param angleOfAttack [rad] angle of attack
     * @return [-] side force coefficient
     */
    virtual double getCy( double angleOfAttack ) const;

    /**
     * Computes lift coefficient.
     * @param angleOfAttack [rad] angle of attack
     * @return [-] lift coefficient
     */
    virtual double getCz( double angleOfAttack ) const;

    /**
     * Computes rolling moment coefficient.
     * @param angleOfAttack [rad] angle of attack
     * @return [-] rolling moment coefficient
     */
    virtual double getCl( double angleOfAttack ) const;

    /**
     * Computes pitching moment coefficient.
     * @param angleOfAttack [rad] angle of attack
     * @return [-] pitching moment coefficient
     */
    virtual double getCm( double angleOfAttack ) const;

    /**
     * Computes yawing moment coefficient.
     * @param angleOfAttack [rad] angle of attack
     * @return [-] yawing moment coefficient
     */
    virtual double getCn( double angleOfAttack ) const;
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_WING_H
