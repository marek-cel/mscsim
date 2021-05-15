/****************************************************************************//*
 * Copyright (C) 2021 Marek M. Cel
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
#ifndef F16_AERODYNAMICS_H
#define F16_AERODYNAMICS_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_Aerodynamics.h>

#include <fdm/utils/fdm_Table1.h>
#include <fdm/utils/fdm_Table2.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

class F16_Aircraft;     // aircraft class forward declaration

/**
 * @brief F-16 aerodynamics class.
 *
 * @see Nguyen L., et al.: Simulator Study of Stall/Post-Stall Characteristics of a Fighter Airplane With Relaxed Longitudinal Static Stability, NASA-TP-1538, 1979
 * @see Gilbert W., et al.: Simulator Study of the Effectiveness of an Automatic Control System Designed to Improve the High-Angle-of-Attack Characteristics of a Fighter Airplane, NASA-TN-D-8176, 1976
 */
class F16_Aerodynamics : public Aerodynamics
{
public:

    /** Constructor. */
    F16_Aerodynamics( const F16_Aircraft *aircraft, Input *input );

    /** Destructor. */
    virtual ~F16_Aerodynamics();

    /**
     * Reads data.
     * @param dataNode XML node
     */
    void readData( XmlNode &dataNode );

    /** Computes force and moment. */
    void computeForceAndMoment();

    /** Updates model. */
    void update();

    /**
     * Returns true if aircraft is stalling, otherwise returns false.
     * @return true if aircraft is stalling, false otherwise
     */
    inline bool getStall() const { return false; /* TODO */ }

private:

    const F16_Aircraft *_aircraft;  ///< aircraft model main object

    Table2 _cx_dh_n25;              ///< [-] body x-force coefficient vs angle of attack and sideslip (delta_h=-25)
    Table2 _cx_dh_n10;              ///< [-] body x-force coefficient vs angle of attack and sideslip (delta_h=-10)
    Table2 _cx_dh_0;                ///< [-] body x-force coefficient vs angle of attack and sideslip (delta_h=0)
    Table2 _cx_dh_p10;              ///< [-] body x-force coefficient vs angle of attack and sideslip (delta_h=10)
    Table2 _cx_dh_p25;              ///< [-] body x-force coefficient vs angle of attack and sideslip (delta_h=25)
    Table2 _cx_lef;                 ///<
    Table1 _delta_cx_sb;            ///<
    Table1 _cx_q;                   ///<
    Table1 _delta_cx_q_lef;         ///<
    Table1 _delta_cx_tef;           ///<
    Table1 _delta_cx_gear;          ///<

    Table2 _cy;                     ///< [-] sideforce coefficient vs angle of attack and sideslip
    Table2 _cy_lef;                 ///<
    Table2 _cy_da_20;               ///<
    Table2 _cy_da_20_lef;           ///<
    Table2 _cy_dr_30;               ///<
    Table1 _cy_r;                   ///<
    Table1 _delta_cy_r_lef;         ///<
    Table1 _cy_p;                   ///<
    Table1 _delta_cy_p_lef;         ///<

    Table2 _cz_dh_n25;              ///< [-] body z-force coefficient vs angle of attack and sideslip (delta_h=-25)
    Table2 _cz_dh_n10;              ///< [-] body z-force coefficient vs angle of attack and sideslip (delta_h=-10)
    Table2 _cz_dh_0;                ///< [-] body z-force coefficient vs angle of attack and sideslip (delta_h=0)
    Table2 _cz_dh_p10;              ///< [-] body z-force coefficient vs angle of attack and sideslip (delta_h=10)
    Table2 _cz_dh_p25;              ///< [-] body z-force coefficient vs angle of attack and sideslip (delta_h=25)
    Table2 _cz_lef;                 ///<
    Table1 _delta_cz_sb;            ///<
    Table1 _cz_q;                   ///<
    Table1 _delta_cz_q_lef;         ///<
    Table1 _delta_cz_tef;           ///<
    Table1 _delta_cz_gear;          ///<

    Table2 _cl_dh_n25;              ///<
    Table2 _cl_dh_0;                ///<
    Table2 _cl_dh_p25;              ///<
    Table2 _cl_lef;                 ///<
    Table2 _cl_da_20;               ///<
    Table2 _cl_da_20_lef;           ///<
    Table2 _cl_dr_30;               ///<
    Table1 _cl_r;                   ///<
    Table1 _delta_cl_beta;          ///<
    Table1 _delta_cl_r_lef;         ///<
    Table1 _cl_p;                   ///<
    Table1 _delta_cl_p_lef;         ///<

    Table2 _cm_dh_n25;              ///< [-] body pitching moment coefficient vs angle of attack and sideslip (delta_h=-25)
    Table2 _cm_dh_n10;              ///< [-] body pitching moment coefficient vs angle of attack and sideslip (delta_h=-10)
    Table2 _cm_dh_0;                ///< [-] body pitching moment coefficient vs angle of attack and sideslip (delta_h=0)
    Table2 _cm_dh_p10;              ///< [-] body pitching moment coefficient vs angle of attack and sideslip (delta_h=10)
    Table2 _cm_dh_p25;              ///< [-] body pitching moment coefficient vs angle of attack and sideslip (delta_h=25)
    Table2 _cm_lef;                 ///<
    Table1 _delta_cm_sb;            ///<
    Table1 _cm_q;                   ///<
    Table1 _delta_cm_q_lef;         ///<
    Table1 _delta_cm;               ///<
    Table2 _delta_cm_ds;            ///< (deep stall)
    Table1 _delta_cm_tef;           ///<
    Table1 _eta_delta_h;            ///< horizontal stabilator effectiveness factor

    Table2 _cn_dh_n25;              ///< [-] body yawing moment coefficient vs angle of attack and sideslip (delta_h=-25)
    Table2 _cn_dh_0;                ///< [-] body yawing moment coefficient vs angle of attack and sideslip (delta_h=0)
    Table2 _cn_dh_p25;              ///< [-] body yawing moment coefficient vs angle of attack and sideslip (delta_h=25)
    Table2 _cn_lef;                 ///<
    Table2 _cn_da_20;               ///<
    Table2 _cn_da_20_lef;           ///<
    Table2 _cn_dr_30;               ///<
    Table1 _cn_r;                   ///<
    Table1 _delta_cn_beta;          ///<
    Table1 _delta_cn_r_lef;         ///<
    Table1 _cn_p;                   ///<
    Table1 _delta_cn_p_lef;         ///<

    Table1 _wave_drag;              ///< wave drag coefficient

    double _span;                   ///< [m] wing span
    double _mac;                    ///< [m] wing mean aerodynamic chord
    double _area;                   ///< [m^2] wing reference area

    double _mac_s;                  ///< [m^3] MAC*S where MAC is mean aerodynamic chord and S is wing area

    double _alpha;                  ///< [rad] angle of attack
    double _alpha_deg;              ///< [deg] angle of attack
    double _beta;                   ///< [rad] angle of sideslip
    double _beta_deg;               ///< [deg] angle of sideslip

    double _b_2v;                   ///< [s] b/(2*V) where b is a wing span and V is an airspeed
    double _c_2v;                   ///< [s] c/(2*V) where c is a mean chord and V is an airspeed

    double _cx_delta_h;             ///< [-] body x-force coefficient due to angle of attack, sideslip and elevator deflection
    double _cz_delta_h;             ///< [-] body z-force coefficient due to angle of attack, sideslip and elevator deflection
    double _cl_delta_h;             ///<
    double _cm_delta_h;             ///< [-] body pitching moment coefficient due to angle of attack, sideslip and elevator deflection
    double _cn_delta_h;             ///<

    double _lef_factor;             ///< [-] leading edge flaps factor

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

#endif // F16_AERODYNAMICS_H
