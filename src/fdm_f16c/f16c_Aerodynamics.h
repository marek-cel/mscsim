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
#ifndef F16C_AERODYNAMICS_H
#define F16C_AERODYNAMICS_H

////////////////////////////////////////////////////////////////////////////////

#include <fdmMain/fdm_Aerodynamics.h>

#include <fdmMain/fdm_MainRotor.h>
#include <fdmMain/fdm_TailRotor.h>
#include <fdmMain/fdm_Stabilizer.h>

#include <fdm_uh60/uh60_Fuselage.h>
#include <fdm_uh60/uh60_StabilizerHor.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

class F16C_Aircraft; ///< aircraft class forward declaration

/**
 * @brief F-16 aerodynamics class.
 */
class F16C_Aerodynamics : public Aerodynamics
{
public:

    /** Constructor. */
    F16C_Aerodynamics( const F16C_Aircraft *aircraft );

    /** Destructor. */
    ~F16C_Aerodynamics();

    /**
     * Reads data.
     * @param dataNode XML node
     */
    void readData( XmlNode &dataNode );

    /**
     * Computes force and moment.
     */
    void computeForceAndMoment();

    /**
     * Updates model.
     */
    void update();

    /**
     * Returns true if aircraft is stalling, otherwise returns false.
     * @return true if aircraft is stalling, false otherwise
     */
    inline bool getStall() const { return false; /* TODO */ }

private:

    const F16C_Aircraft *m_aircraft;    ///< aircraft model main object

    Table m_cx;                 ///< [-] drag coefficient vs [rad] angle of attack
    Table m_cy;                 ///< [-] sideforce coefficient vs [rad] angle of sideslip
    Table m_cz;                 ///< [-] lift coefficient vs [rad] angle of attack

    Table m_cl;                 ///< [-] rolling moment coefficient vs [rad] angle of sideslip
    Table m_cl_p;               ///< [1/rad] rolling moment coefficient due to roll rate
    Table m_cl_delta_a;         ///< [1/rad] rolling moment coefficient due to ailerons deflection

    Table m_cm;                 ///< [-] pitching moment coefficient vs [rad] angle of attack
    Table m_cm_q;               ///< [1/rad] pitching moment coefficient due to pitch rate
    Table m_cm_delta_h;         ///< [1/rad] pitching moment coefficient due to elevator deflection

    Table m_cn;                 ///< [-] yawing moment coefficient vs [rad] angle of sideslip
    Table m_cn_r;               ///< [1/rad] yawing moment coefficient due to yaw rate
    Table m_cn_delta_r;         ///< [1/rad] yawing moment coefficient due to rudder deflection

    double m_span;              ///< [m] wing span
    double m_mac;               ///< [m] wing mean aerodynamic chord
    double m_area;              ///< [m^2] wing reference area

    double m_mac_s;             ///< [m^3] MAC*S where MAC is mean aerodynamic chord and S is wing area

    double m_b_2v;              ///< [s] b/(2*V) where b is a wing span and V is an airspeed
    double m_c_2v;              ///< [s] c/(2*V) where c is a mean chord and V is an airspeed

    double getCx() const;
    double getCy() const;
    double getCz() const;

    double getCl() const;
    double getCm() const;
    double getCn() const;
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // F16C_AERODYNAMICS_H
