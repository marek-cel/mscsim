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
#ifndef F16C_FLCS_H
#define F16C_FLCS_H

////////////////////////////////////////////////////////////////////////////////

#include <fdmSys/fdm_Filter2.h>
#include <fdmSys/fdm_Lag.h>
#include <fdmSys/fdm_LeadLag.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief F-16 Flight Control System class.
 *
 * @see Marchand M.: Pitch Rate Flight Control for the F-16 Aircraft to improve Air-to-Air Combat, AD-A055-417, 1977
 * @see Nguyen L., et al.: Simulator Study of Stall/Post-Stall Characteristics of a Fighter Airplane With Relaxed Longitudinal Static Stability, NASA-TP-1538, 1979
 * @see Droste T., Walker J.: The General Dynamics Case Study on the F-16 Fly-by-Wire Flight Control System
 */
class F16C_FLCS
{
public:

    /** Loading category. */
    enum CAT
    {
        CAT_I   = 0,    ///< category I loadings
        CAT_III = 1     ///< category III loadings
    };

    /** FLCS gains. */
    enum Gains
    {
        Cruise  = 0,
        Landing = 1
    };

    /** Constructor. */
    F16C_FLCS();

    /** Destructor. */
    ~F16C_FLCS();

    /**
     * Updates model.
     */
    void update( double timeStep, double angleOfAttack,
                 double g_y, double g_z,
                 double rollRate, double pitchRate, double yawRate,
                 double ctrlLat, double trimLat,
                 double ctrlLon, double trimLon,
                 double ctrlYaw, double trimYaw,
                 double statPress, double dynPress,
                 bool alt_flaps_ext, bool refuel_door_open,
                 bool lg_handle_dn, bool touchdown );

    inline double getAilerons()     const { return m_ailerons;      }
    inline double getAileronsNorm() const { return m_ailerons_norm; }
    inline double getElevator()     const { return m_elevator;      }
    inline double getElevatorNorm() const { return m_elevator_norm; }
    inline double getElevons()      const { return m_elevons;       }
    inline double getRudder()       const { return m_rudder;        }
    inline double getRudderNorm()   const { return m_rudder_norm;   }
    inline double getFlapsLE()      const { return m_flaps_le;      }
    inline double getFlapsLENorm()  const { return m_flaps_le_norm; }
    inline double getFlapsTE()      const { return m_flaps_te;      }
    inline double getFlapsTENorm()  const { return m_flaps_te_norm; }

    void setAilerons_max( double ailerons_max );
    void setElevator_max( double elevator_max );
    void setRudder_max( double rudder_max );
    void setFlaps_le_max( double flaps_le_max );

private:

    double m_ailerons_max;              ///< [rad] ailerons deflection limit
    double m_elevator_max;              ///< [rad] elevator deflection limit
    double m_rudder_max;                ///< [rad] rudder  deflection limit
    double m_flaps_le_max;              ///< [rad] leading edgle flaps deflection limit

    double m_ailerons_max_deg;          ///< [deg] ailerons deflection limit
    double m_elevator_max_deg;          ///< [deg] elevator deflection limit
    double m_rudder_max_deg;            ///< [deg] rudder  deflection limit
    double m_flaps_le_max_deg;          ///< [deg] leading edgle flaps deflection limit

    double m_ailerons;                  ///< [rad] ailerons deflection
    double m_ailerons_norm;             ///< [-] normalized ailerons deflection
    double m_elevator;                  ///< [rad] elevator deflection
    double m_elevator_norm;             ///< [-] normalized elevator deflection
    double m_elevons;                   ///< [rad] elevons differential deflection
    double m_rudder;                    ///< [rad] rudder deflection
    double m_rudder_norm;               ///< [-] normalized rudder deflection
    double m_flaps_le;                  ///< [rad] leading edge flaps deflection
    double m_flaps_le_norm;             ///< [-] normalized leading edge flaps deflection
    double m_flaps_te;                  ///< [rad] trailing edge flaps deflection
    double m_flaps_te_norm;             ///< [-] normalized trailing edge flaps deflection

    double m_timeStep;                  ///< [s] time step

    CAT   m_cat;                        ///< loading category
    Gains m_gains;                      ///< FLCS gains mode

    // leading edge flaps

    LeadLag *m_alpha_lef;               ///<

    // trailing edge flaps

    double m_flaps_int;                 ///<
    double m_flaps_com;                 ///<

    // roll channel

    Lag     *m_stick_lat;               ///< lateral stick input lagged
    Lag     *m_p_com_lag;               ///< roll rate command
    LeadLag *m_p_com_pos;               ///< roll rate command "positive" feedback loop
    LeadLag *m_p_com_neg;               ///< roll rate command "negative" feedback loop
    Lag     *m_omg_p_lag;               ///< filtered roll rate (p_f)
    Filter2 *m_omg_p_fil;               ///<
    Lag     *m_delta_fl_lag;            ///<
    Lag     *m_delta_fr_lag;            ///<

    double m_delta_flc;                 ///< [deg] left flaperon deflection commanded by control system
    double m_delta_frc;                 ///< [deg] right flaperon deflection commanded by control system
    double m_delta_fl;                  ///< [deg] left flaperon deflection
    double m_delta_fr;                  ///< [deg] right flaperon deflection
    double m_delta_ac;                  ///< [deg] ailerons deflection
    double m_delta_a;                   ///< [deg] ailerons deflection

    // pitch channel

    Lag     *m_stick_lon;               ///< longitudinal stick input lagged
    Lag     *m_alpha_lag;               ///< [deg] filtered angle of attack (alpha_f)
    Lag     *m_g_com_lag;               ///<
    Lag     *m_omg_q_lag;               ///< filtered pitch rate (q_f)
    LeadLag *m_omg_q_fil;               ///< pitch rate filtered
    Lag     *m_g_z_input;               ///< filtered normal acceleration (Az_f)
    Lag     *m_sca_bias_1;              ///<
    Lag     *m_sca_bias_2;              ///<
    Lag     *m_sca_bias_3;              ///<
    LeadLag *m_u_sca_fil;               ///<
    Filter2 *m_u_sca_fil2;              ///<
    Lag     *m_actuator_l;              ///<
    Lag     *m_actuator_r;              ///<

    double m_pitch_int;                 ///< [deg] pitch integral
    double m_delta_htl;                 ///<
    double m_delta_htr;                 ///<
    double m_delta_h;                   ///< [deg] horizontal stabilator deflection
    double m_delta_d;                   ///< [deg] differential horizontal tail deflection

    // yaw channel

    Lag     *m_pedals;                  ///<
    Lag     *m_omg_r_lag;               ///< yaw rate filtered (r_f)
    Filter2 *m_omg_p_yaw;               ///<
    LeadLag *m_u_sum_ll1;               ///<
    LeadLag *m_u_sum_ll2;               ///<
    Filter2 *m_delta_r_fil;             ///<
    Lag     *m_delta_r_lag;             ///<

    double m_delta_r;                   ///< [deg] rudder deflection

    // gun compensation

    double m_gun_compensation;          ///<

    void updateLEF( double angleOfAttack, double q_p );

    void updateTEF( double q_p, bool alt_flaps_ext, bool lg_handle_dn );

    void updateLat( double ctrlLat, double trimLat,
                    double rollRate );

    void updateLon( double ctrlLon, double trimLon,
                    double pitchRate,
                    double angleOfAttack, double q_p, double dynPress, double g_z,
                    bool touchdown );

    void updateYaw( double ctrlYaw, double trimYaw,
                    double yawRate,
                    double q_p, double g_y );

    double getGainF2( double q_p );
    double getGainF3( double dynPress );
    double getGainF7( double q_p );
    double getGainF8( double q_p );
    double getGainF9( double q_p );
    double getGainF10( double q_p );

    double getSurfaceMaxRate( double d_old, double d_new, double delta_max );
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // F16C_FLCS_H
