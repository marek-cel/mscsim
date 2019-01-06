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
#ifndef C172_WING_H
#define C172_WING_H

////////////////////////////////////////////////////////////////////////////////

#include <fdmMain/fdm_Wing.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Cessna 172 wing class.
 *
 * <h3>XML configuration file format:</h3>
 * @code
 * <wing>
 *   <ac_l> { [m] x-coordinate } { [m] y-coordinate } { [m] z-coordinate } </ac_l>
 *   <ac_r> { [m] x-coordinate } { [m] y-coordinate } { [m] z-coordinate } </ac_r>
 *   <mac> { [m] wing mean aerodynamic chord } </mac>
 *   <area> { [m^2] wing area } </area>
 *   <dcl_dailerons> { [1/rad] rolling moment coefficient due to ailerons deflection } </dcl_dailerons>
 *   <dcx_dflaps> { [1/rad] drag coefficient due to flaps deflection } </dcx_dflaps>
 *   <dcz_dflaps> { [1/rad] lift coefficient due to flaps deflection } </dcz_dflaps>
 *   <dcm_dflaps> { [1/rad] pitching moment coefficient due to flaps deflection } </dcm_dflaps>
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
 */
class C172_Wing : public Wing
{
public:

    /** Constructor. */
    C172_Wing();

    /** Destructor. */
    ~C172_Wing();

    /**
     * Reads data.
     * @param dataNode XML node
     */
    void readData( fdm::XmlNode &dataNode );

    /**
     * Computes force and moment.
     * @param vel_air_bas [m/s] aircraft linear velocity relative to the air expressed in BAS
     * @param omg_air_bas [rad/s] aircraft angular velocity relative to the air expressed in BAS
     * @param airDensity [kg/m^3] air density
     * @param ailerons [rad] ailerons deflection
     * @param flaps [rad] flaps deflection
     */
    void computeForceAndMoment(const fdm::Vector3 &vel_air_bas,
                                const fdm::Vector3 &omg_air_bas,
                                double airDensity ,
                                double ailerons,
                                double flaps );

    /**
     * Updates model.
     * @param vel_air_bas [m/s] aircraft linear velocity relative to the air expressed in BAS
     * @param omg_air_bas [rad/s] aircraft angular velocity relative to the air expressed in BAS
     */
    void update( const fdm::Vector3 &vel_air_bas, const fdm::Vector3 &omg_air_bas );

private:

    double m_ailerons;              ///< [rad] ailerons deflection
    double m_flaps;                 ///< [rad] flaps deflection

    double m_dcl_dailerons;         ///< [1/rad]

    double m_dcx_dflaps;            ///< [1/rad]
    double m_dcz_dflaps;            ///< [1/rad]
    double m_dcm_dflaps;            ///< [1/rad]

    /**
     * Computes drag coefficient.
     * @param angleOfAttack [rad] angle of attack
     * @return [-] drag coefficient
     */
    double getCx( double angleOfAttack ) const;

    /**
     * Computes side force coefficient.
     * @param angleOfAttack [rad] angle of attack
     * @return [-] side force coefficient
     */
    double getCy( double angleOfAttack ) const;

    /**
     * Computes lift coefficient.
     * @param angleOfAttack [rad] angle of attack
     * @return [-] lift coefficient
     */
    double getCz( double angleOfAttack ) const;

    /**
     * Computes rolling moment coefficient.
     * @param angleOfAttack [rad] angle of attack
     * @return [-] rolling moment coefficient
     */
    double getCl( double angleOfAttack ) const;

    /**
     * Computes pitching moment coefficient.
     * @param angleOfAttack [rad] angle of attack
     * @return [-] pitching moment coefficient
     */
    double getCm( double angleOfAttack ) const;

    /**
     * Computes yawing moment coefficient.
     * @param angleOfAttack [rad] angle of attack
     * @return [-] yawing moment coefficient
     */
    double getCn( double angleOfAttack ) const;
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // C172_WING_H
