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

#include <fdmUtils/fdm_Table.h>
#include <fdmUtils/fdm_Table2D.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

class F16C_Aircraft; ///< aircraft class forward declaration

/**
 * @brief F-16 aerodynamics class.
 *
 * @see Nguyen L., et al.: Simulator Study of Stall/Post-Stall Characteristics of a Fighter Airplane With Relaxed Longitudinal Static Stability, NASA-TP-1538
 * @see Gilbert W., et al.: Simulator Study of the Effectiveness of an Automatic Control System Designed to Improve the High-Angle-of-Attack Characteristics of a Fighter Airplane, NASA-TN-D-8176
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

    Table2D m_cx_dh_n25;        ///< [-] body x-force coefficient vs angle of attack and sideslip (delta_h=-25)
    Table2D m_cx_dh_n10;        ///< [-] body x-force coefficient vs angle of attack and sideslip (delta_h=-10)
    Table2D m_cx_dh_0;          ///< [-] body x-force coefficient vs angle of attack and sideslip (delta_h=0)
    Table2D m_cx_dh_p10;        ///< [-] body x-force coefficient vs angle of attack and sideslip (delta_h=10)
    Table2D m_cx_dh_p25;        ///< [-] body x-force coefficient vs angle of attack and sideslip (delta_h=25)
    Table2D m_cx_lef;           ///<
    Table m_delta_cx_sb;        ///<
    Table m_cx_q;               ///<
    Table m_delta_cx_q_lef;     ///<

    Table2D m_cy;               ///< [-] sideforce coefficient vs angle of attack and sideslip
    Table2D m_cy_lef;           ///<
    Table2D m_cy_da_20;         ///<
    Table2D m_cy_da_20_lef;     ///<
    Table2D m_cy_dr_30;         ///<
    Table m_cy_r;               ///<
    Table m_delta_cy_r_lef;     ///<
    Table m_cy_p;               ///<
    Table m_delta_cy_p_lef;     ///<

    Table2D m_cz_dh_n25;        ///< [-] body z-force coefficient vs angle of attack and sideslip (delta_h=-25)
    Table2D m_cz_dh_n10;        ///< [-] body z-force coefficient vs angle of attack and sideslip (delta_h=-10)
    Table2D m_cz_dh_0;          ///< [-] body z-force coefficient vs angle of attack and sideslip (delta_h=0)
    Table2D m_cz_dh_p10;        ///< [-] body z-force coefficient vs angle of attack and sideslip (delta_h=10)
    Table2D m_cz_dh_p25;        ///< [-] body z-force coefficient vs angle of attack and sideslip (delta_h=25)
    Table2D m_cz_lef;           ///<
    Table m_delta_cz_sb;        ///<
    Table m_cz_q;               ///<
    Table m_delta_cz_q_lef;     ///<
    Table m_eta_delta_h;        ///< horizontal stabilator effectiveness factor

    Table2D m_cl_dh_n25;        ///<
    Table2D m_cl_dh_0;          ///<
    Table2D m_cl_dh_p25;        ///<
    Table2D m_cl_lef;           ///<
    Table2D m_cl_da_20;         ///<
    Table2D m_cl_da_20_lef;     ///<
    Table2D m_cl_dr_30;         ///<
    Table m_cl_r;               ///<
    Table m_delta_cl_beta;      ///<
    Table m_delta_cl_r_lef;     ///<
    Table m_cl_p;               ///<
    Table m_delta_cl_p_lef;     ///<

    Table2D m_cm_dh_n25;        ///< [-] body pitching moment coefficient vs angle of attack and sideslip (delta_h=-25)
    Table2D m_cm_dh_n10;        ///< [-] body pitching moment coefficient vs angle of attack and sideslip (delta_h=-10)
    Table2D m_cm_dh_0;          ///< [-] body pitching moment coefficient vs angle of attack and sideslip (delta_h=0)
    Table2D m_cm_dh_p10;        ///< [-] body pitching moment coefficient vs angle of attack and sideslip (delta_h=10)
    Table2D m_cm_dh_p25;        ///< [-] body pitching moment coefficient vs angle of attack and sideslip (delta_h=25)
    Table2D m_cm_lef;           ///<
    Table m_delta_cm_sb;        ///<
    Table m_cm_q;               ///<
    Table m_delta_cm_q_lef;     ///<
    Table m_delta_cm;           ///<
    Table2D m_delta_cm_ds;      ///< (deep stall)

    Table2D m_cn_dh_n25;        ///< [-] body yawing moment coefficient vs angle of attack and sideslip (delta_h=-25)
    Table2D m_cn_dh_0;          ///< [-] body yawing moment coefficient vs angle of attack and sideslip (delta_h=0)
    Table2D m_cn_dh_p25;        ///< [-] body yawing moment coefficient vs angle of attack and sideslip (delta_h=25)
    Table2D m_cn_lef;           ///<
    Table2D m_cn_da_20;         ///<
    Table2D m_cn_da_20_lef;     ///<
    Table2D m_cn_dr_30;         ///<
    Table m_cn_r;               ///<
    Table m_delta_cn_beta;      ///<
    Table m_delta_cn_r_lef;     ///<
    Table m_cn_p;               ///<
    Table m_delta_cn_p_lef;     ///<

    double m_span;              ///< [m] wing span
    double m_mac;               ///< [m] wing mean aerodynamic chord
    double m_area;              ///< [m^2] wing reference area

    double m_mac_s;             ///< [m^3] MAC*S where MAC is mean aerodynamic chord and S is wing area

    double m_alpha;             ///< [rad] angle of attack
    double m_alpha_deg;         ///< [deg] angle of attack
    double m_beta;              ///< [rad] angle of sideslip
    double m_beta_deg;          ///< [deg] angle of sideslip

    double m_b_2v;              ///< [s] b/(2*V) where b is a wing span and V is an airspeed
    double m_c_2v;              ///< [s] c/(2*V) where c is a mean chord and V is an airspeed

    double m_cx_delta_h;        ///< [-] body x-force coefficient due to angle of attack, sideslip and elevator deflection
    double m_cz_delta_h;        ///< [-] body z-force coefficient due to angle of attack, sideslip and elevator deflection
    double m_cl_delta_h;
    double m_cm_delta_h;        ///< [-] body pitching moment coefficient due to angle of attack, sideslip and elevator deflection
    double m_cn_delta_h;

    double m_lef_factor;        ///< [-] leading edge flaps factor

    double getCx() const;
    double getCy() const;
    double getCz() const;

    double getCl() const;
    double getCm() const;
    double getCn() const;

    void updateCoefsDueToElevator();
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // F16C_AERODYNAMICS_H
