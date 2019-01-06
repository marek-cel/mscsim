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
#ifndef C172_STABILIZERHOR_H
#define C172_STABILIZERHOR_H

////////////////////////////////////////////////////////////////////////////////

#include <fdmMain/fdm_StabilizerHor.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Cessna 172 horizontal stabilizer class.
 *
 * <h5>XML configuration file format:</h5>
 * @code
 * <horizontal_stabilizer>
 *   <position> { [m] x-coordinate } { [m] y-coordinate } { [m] z-coordinate } </position>
 *   <area> { [m^2] stabilizer area } </area>
 *   <incidence> { [deg] stabilizer incidence } </incidence>
 *   <downwash> { [-] coefficient of downwash due to the wing angle of attack } </downwash>
 *   <dcx_delevator> { [1/rad] drag coefficient due to elevator deflection } </dcx_delevator>
 *   <dcz_delevator> { [1/rad] lift coefficient due to elevator deflection } </dcz_delevator>
 *   <dcz_delevator_trim> { [1/rad] lift coefficient due to elevator trim deflection } </dcz_delevator_trim>
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
 */
class C172_StabilizerHor : public StabilizerHor
{
public:

    /** Constructor. */
    C172_StabilizerHor();

    /** Destructor. */
    ~C172_StabilizerHor();

    /**
     * Reads data.
     * @param dataNode XML node
     */
    void readData( XmlNode &dataNode );

    /**
     * Computes force and moment.
     * @param vel_air_bas [m/s] aircraft linear velocity relative to the air expressed in BAS
     * @param omg_air_bas [rad/s] aircraft angular velocity relative to the air expressed in BAS
     * @param airDensity [kg/m^3] air density
     * @param elevator [rad] elevator deflection
     * @param elevatorTrim [rad] elevator trim deflection
     * @param angleOfAttackWing [rad] wing angle of attach
     */
    void computeForceAndMoment( const Vector3 &vel_air_bas,
                                const Vector3 &omg_air_bas,
                                double airDensity,
                                double elevator,
                                double elevatorTrim,
                                double angleOfAttackWing );

private:

    double m_dcx_delevator;         ///< [1/rad] drag coefficient due to elevator deflection
    double m_dcz_delevator;         ///< [1/rad] lift coefficient due to elevator deflection

    double m_dcz_delevator_trim;    ///< [1/rad] lift coefficient due to elevator trim deflection
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // C172_STABILIZERHOR_H
