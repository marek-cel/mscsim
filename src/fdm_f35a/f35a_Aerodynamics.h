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
#ifndef F35A_AERODYNAMICS_H
#define F35A_AERODYNAMICS_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/main/fdm_Aerodynamics.h>

#include <fdm/utils/fdm_Table.h>
#include <fdm/utils/fdm_Table2D.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

class F35A_Aircraft;    ///< aircraft class forward declaration

/**
 * @brief F-35A aerodynamics class.
 */
class F35A_Aerodynamics : public Aerodynamics
{
public:

    /** Constructor. */
    F35A_Aerodynamics( const F35A_Aircraft *aircraft, DataNode *rootNode );

    /** Destructor. */
    ~F35A_Aerodynamics();

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

    const F35A_Aircraft *_aircraft; ///< aircraft model main object

    Table2D _cx_dh_n25;             ///< [-] body x-force coefficient vs angle of attack and sideslip (delta_h=-25)
    Table2D _cx_dh_n10;             ///< [-] body x-force coefficient vs angle of attack and sideslip (delta_h=-10)
    Table2D _cx_dh_0;               ///< [-] body x-force coefficient vs angle of attack and sideslip (delta_h=0)
    Table2D _cx_dh_p10;             ///< [-] body x-force coefficient vs angle of attack and sideslip (delta_h=10)
    Table2D _cx_dh_p25;             ///< [-] body x-force coefficient vs angle of attack and sideslip (delta_h=25)
    Table2D _cx_lef;                ///<
    Table _delta_cx_sb;             ///<
    Table _cx_q;                    ///<
    Table _delta_cx_q_lef;          ///<
    Table _delta_cx_tef;            ///<
    Table _delta_cx_gear;           ///<

    Table2D _cy;                    ///< [-] sideforce coefficient vs angle of attack and sideslip
    Table2D _cy_lef;                ///<
    Table2D _cy_da_20;              ///<
    Table2D _cy_da_20_lef;          ///<
    Table2D _cy_dr_30;              ///<
    Table _cy_r;                    ///<
    Table _delta_cy_r_lef;          ///<
    Table _cy_p;                    ///<
    Table _delta_cy_p_lef;          ///<

    Table2D _cz_dh_n25;             ///< [-] body z-force coefficient vs angle of attack and sideslip (delta_h=-25)
    Table2D _cz_dh_n10;             ///< [-] body z-force coefficient vs angle of attack and sideslip (delta_h=-10)
    Table2D _cz_dh_0;               ///< [-] body z-force coefficient vs angle of attack and sideslip (delta_h=0)
    Table2D _cz_dh_p10;             ///< [-] body z-force coefficient vs angle of attack and sideslip (delta_h=10)
    Table2D _cz_dh_p25;             ///< [-] body z-force coefficient vs angle of attack and sideslip (delta_h=25)
    Table2D _cz_lef;                ///<
    Table _delta_cz_sb;             ///<
    Table _cz_q;                    ///<
    Table _delta_cz_q_lef;          ///<
    Table _delta_cz_tef;            ///<
    Table _delta_cz_gear;           ///<

    Table2D _cl_dh_n25;             ///<
    Table2D _cl_dh_0;               ///<
    Table2D _cl_dh_p25;             ///<
    Table2D _cl_lef;                ///<
    Table2D _cl_da_20;              ///<
    Table2D _cl_da_20_lef;          ///<
    Table2D _cl_dr_30;              ///<
    Table _cl_r;                    ///<
    Table _delta_cl_beta;           ///<
    Table _delta_cl_r_lef;          ///<
    Table _cl_p;                    ///<
    Table _delta_cl_p_lef;          ///<

    Table2D _cm_dh_n25;             ///< [-] body pitching moment coefficient vs angle of attack and sideslip (delta_h=-25)
    Table2D _cm_dh_n10;             ///< [-] body pitching moment coefficient vs angle of attack and sideslip (delta_h=-10)
    Table2D _cm_dh_0;               ///< [-] body pitching moment coefficient vs angle of attack and sideslip (delta_h=0)
    Table2D _cm_dh_p10;             ///< [-] body pitching moment coefficient vs angle of attack and sideslip (delta_h=10)
    Table2D _cm_dh_p25;             ///< [-] body pitching moment coefficient vs angle of attack and sideslip (delta_h=25)
    Table2D _cm_lef;                ///<
    Table _delta_cm_sb;             ///<
    Table _cm_q;                    ///<
    Table _delta_cm_q_lef;          ///<
    Table _delta_cm;                ///<
    Table2D _delta_cm_ds;           ///< (deep stall)
    Table _delta_cm_tef;            ///<
    Table _eta_delta_h;             ///< horizontal stabilator effectiveness factor

    Table2D _cn_dh_n25;             ///< [-] body yawing moment coefficient vs angle of attack and sideslip (delta_h=-25)
    Table2D _cn_dh_0;               ///< [-] body yawing moment coefficient vs angle of attack and sideslip (delta_h=0)
    Table2D _cn_dh_p25;             ///< [-] body yawing moment coefficient vs angle of attack and sideslip (delta_h=25)
    Table2D _cn_lef;                ///<
    Table2D _cn_da_20;              ///<
    Table2D _cn_da_20_lef;          ///<
    Table2D _cn_dr_30;              ///<
    Table _cn_r;                    ///<
    Table _delta_cn_beta;           ///<
    Table _delta_cn_r_lef;          ///<
    Table _cn_p;                    ///<
    Table _delta_cn_p_lef;          ///<

    Table _wave_drag;               ///< wave drag coefficient

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

#endif // F35A_AERODYNAMICS_H
