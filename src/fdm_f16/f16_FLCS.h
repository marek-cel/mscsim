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
#ifndef F16_FLCS_H
#define F16_FLCS_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/ctrl/fdm_Filter2.h>
#include <fdm/ctrl/fdm_Lag.h>
#include <fdm/ctrl/fdm_LeadLag.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief F-16 Flight Control System class.
 *
 * @see Marchand M.: Pitch Rate Flight Control for the F-16 Aircraft to improve Air-to-Air Combat, AD-A055-417, 1977
 * @see Nguyen L., et al.: Simulator Study of Stall/Post-Stall Characteristics of a Fighter Airplane With Relaxed Longitudinal Static Stability, NASA-TP-1538, 1979
 * @see Droste T., Walker J.: The General Dynamics Case Study on the F-16 Fly-by-Wire Flight Control System
 * @see Dameron G.: A Real-Time Simulator for Man-In-The-Loop Testing of Aircraft Control Systems, AD-A202-599, 1988
 */
class F16_FLCS
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
    F16_FLCS();

    /** Destructor. */
    virtual ~F16_FLCS();

    /** Updates model. */
    void update( double timeStep, double angleOfAttack,
                 double g_y, double g_z,
                 double rollRate, double pitchRate, double yawRate,
                 double ctrlLat, double trimLat,
                 double ctrlLon, double trimLon,
                 double ctrlYaw, double trimYaw,
                 double statPress, double dynPress,
                 bool alt_flaps_ext, bool refuel_door_open,
                 bool lg_handle_dn, bool touchdown );

    inline double getAilerons()     const { return _ailerons;      }
    inline double getAileronsNorm() const { return _ailerons_norm; }
    inline double getElevator()     const { return _elevator;      }
    inline double getElevatorNorm() const { return _elevator_norm; }
    inline double getElevons()      const { return _elevons;       }
    inline double getRudder()       const { return _rudder;        }
    inline double getRudderNorm()   const { return _rudder_norm;   }
    inline double getFlapsLE()      const { return _flaps_le;      }
    inline double getFlapsLENorm()  const { return _flaps_le_norm; }
    inline double getFlapsTE()      const { return _flaps_te;      }
    inline double getFlapsTENorm()  const { return _flaps_te_norm; }

    void setAilerons_max( double ailerons_max );
    void setElevator_max( double elevator_max );
    void setRudder_max( double rudder_max );
    void setFlaps_le_max( double flaps_le_max );

private:

    double _ailerons_max;               ///< [rad] ailerons deflection limit
    double _elevator_max;               ///< [rad] elevator deflection limit
    double _rudder_max;                 ///< [rad] rudder  deflection limit
    double _flaps_le_max;               ///< [rad] leading edgle flaps deflection limit

    double _ailerons_max_deg;           ///< [deg] ailerons deflection limit
    double _elevator_max_deg;           ///< [deg] elevator deflection limit
    double _rudder_max_deg;             ///< [deg] rudder  deflection limit
    double _flaps_le_max_deg;           ///< [deg] leading edgle flaps deflection limit

    double _ailerons;                   ///< [rad] ailerons deflection
    double _ailerons_norm;              ///< [-] normalized ailerons deflection
    double _elevator;                   ///< [rad] elevator deflection
    double _elevator_norm;              ///< [-] normalized elevator deflection
    double _elevons;                    ///< [rad] elevons differential deflection
    double _rudder;                     ///< [rad] rudder deflection
    double _rudder_norm;                ///< [-] normalized rudder deflection
    double _flaps_le;                   ///< [rad] leading edge flaps deflection
    double _flaps_le_norm;              ///< [-] normalized leading edge flaps deflection
    double _flaps_te;                   ///< [rad] trailing edge flaps deflection
    double _flaps_te_norm;              ///< [-] normalized trailing edge flaps deflection

    double _timeStep;                   ///< [s] time step

    CAT   _cat;                         ///< loading category
    Gains _gains;                       ///< FLCS gains mode

    // leading edge flaps

    LeadLag *_alpha_lef;                ///<

    // trailing edge flaps

    double _flaps_int;                  ///<
    double _flaps_com;                  ///<

    // roll channel

    Lag     *_stick_lat;                ///< lateral stick input lagged
    Lag     *_p_com_lag;                ///< roll rate command
    LeadLag *_p_com_pos;                ///< roll rate command "positive" feedback loop
    LeadLag *_p_com_neg;                ///< roll rate command "negative" feedback loop
    Lag     *_omg_p_lag;                ///< filtered roll rate (p_f)
    Filter2 *_omg_p_fil;                ///<
    Lag     *_delta_fl_lag;             ///<
    Lag     *_delta_fr_lag;             ///<

    double _delta_flc;                  ///< [deg] left flaperon deflection commanded by control system
    double _delta_frc;                  ///< [deg] right flaperon deflection commanded by control system
    double _delta_fl;                   ///< [deg] left flaperon deflection
    double _delta_fr;                   ///< [deg] right flaperon deflection
    double _delta_ac;                   ///< [deg] ailerons deflection
    double _delta_a;                    ///< [deg] ailerons deflection

    // pitch channel

    Lag     *_stick_lon;                ///< longitudinal stick input lagged
    Lag     *_alpha_lag;                ///< [deg] filtered angle of attack (alpha_f)
    Lag     *_g_com_lag;                ///<
    Lag     *_omg_q_lag;                ///< filtered pitch rate (q_f)
    LeadLag *_omg_q_fil;                ///< pitch rate filtered
    Lag     *_g_z_input;                ///< filtered normal acceleration (Az_f)
    Lag     *_sca_bias_1;               ///<
    Lag     *_sca_bias_2;               ///<
    Lag     *_sca_bias_3;               ///<
    LeadLag *_u_sca_fil;                ///<
    Filter2 *_u_sca_fil2;               ///<
    Lag     *_actuator_l;               ///<
    Lag     *_actuator_r;               ///<

    double _pitch_int;                  ///< [deg] pitch integral
    double _delta_htl;                  ///<
    double _delta_htr;                  ///<
    double _delta_h;                    ///< [deg] horizontal stabilator deflection
    double _delta_d;                    ///< [deg] differential horizontal tail deflection

    // yaw channel

    Lag     *_pedals;                   ///<
    Lag     *_omg_r_lag;                ///< yaw rate filtered (r_f)
    Filter2 *_omg_p_yaw;                ///<
    LeadLag *_u_sum_ll1;                ///<
    LeadLag *_u_sum_ll2;                ///<
    Filter2 *_delta_r_fil;              ///<
    Lag     *_delta_r_lag;              ///<

    double _delta_r;                    ///< [deg] rudder deflection

    // gun compensation

    double _gun_compensation;           ///<

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

#endif // F16_FLCS_H
