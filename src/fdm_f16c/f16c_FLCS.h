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

namespace fdm
{

/**
 * @brief F-16 Flight Control System class.
 *
 * @see Nguyen L., et al.: Simulator Study of Stall/Post-Stall Characteristics of a Fighter Airplane With Relaxed Longitudinal Static Stability, NASA-TP-1538
 * @see Droste T., Walker J.: The General Dynamics Case Study on the F-16 Fly-by-Wire Flight Control System
 * @see Marchand M.: Pitch Rate Flight Control for the F-16 Aircraft to improve Air-to-Air Combat, 1977
 * @see Kassan M.: F-16 Simulator for Man-in-the-Loop Testing of Aircraft Control Systems, 1987
 * @see Dameron G.: A Real-Time Simulator for Man-in-the-Loop Testing of Aircraft Control System, 1988
 * @see Flight Manual USAF/EPAF Series Aircraft F-16A/B Blocks 10 and 15. T.O. 1F-16A-1, 2003
 * @see Flight Manual HAF Series Aircraft F-16C/D Blocks 50 and 52+. T.O. GR1F-16CJ-1, 2003
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
        Takeoff = 1
    };

    /** Constructor. */
    F16C_FLCS();

    /** Destructor. */
    ~F16C_FLCS();

    /**
     * Updates model.
     */
    void update( double timeStep, double angleOfAttack, double g_z,
                 double rollRate, double pitchRate,
                 double stickLat, double trimLat,
                 double stickLon, double trimLon,
                 double statPress, double dynPress );

    inline double getAilerons()     const { return m_ailerons;      }
    inline double getAileronsNorm() const { return m_ailerons_norm; }
    inline double getElevator()     const { return m_elevator;      }
    inline double getElevatorNorm() const { return m_elevator_norm; }
    inline double getRudder()       const { return m_rudder;        }
    inline double getRudderNorm()   const { return m_rudder_norm;   }
    inline double getFlapsLE()      const { return m_flaps_le;      }
    inline double getFlapsLENorm()  const { return m_flaps_le_norm; }

    void setAilerons_max ( double ailerons_max ) { m_ailerons_max = ailerons_max; }
    void setElevator_max ( double elevator_max ) { m_elevator_max = elevator_max; }
    void setRudder_max   ( double rudder_max   ) { m_rudder_max   = rudder_max;   }
    void setFlaps_le_max ( double flaps_le_max ) { m_flaps_le_max = flaps_le_max; }

private:

    double m_ailerons_max;              ///< [rad] ailerons deflection limit
    double m_elevator_max;              ///< [rad] elevator deflection limit
    double m_rudder_max;                ///< [rad] rudder  deflection limit
    double m_flaps_le_max;              ///< [rad] leading edgle flaps deflection limit

    double m_ailerons;                  ///< [rad] ailerons deflection
    double m_ailerons_norm;             ///< [-] normalized ailerons deflection
    double m_elevator;                  ///< [rad] elevator deflection
    double m_elevator_norm;             ///< [-] normalized elevator deflection
    double m_rudder;                    ///< [rad] rudder deflection
    double m_rudder_norm;               ///< [-] normalized rudder deflection
    double m_flaps_le;                  ///< [rad] leading edge flaps deflection
    double m_flaps_le_norm;             ///< [-] normalized leading edge flaps deflection

    double m_timeStep;                  ///< [s] time step

    CAT   m_cat;                        ///< loading category
    Gains m_gains;                      ///< FLCS gains mode

    double m_alpha_deg;                 ///< [deg] angle of attack
    double m_alpha_lag;                 ///< [deg] angle of attack angle of attack (lagged)
    double m_alpha_dif;                 ///< [deg/s] angle of attack differential

    double m_aoa_lef_1;                 ///<
    double m_aoa_lef_2;                 ///<

    double m_omg_q_lag;                 ///< [deg/s] pitch rate (lagged)
    double m_omg_q_dif;                 ///< [deg/^2] pitch rate differential
    double m_stick_lon;                 ///< [-] longitudinal stick input (lagged)
    double m_g_response;                ///< [-] Gz response (lagged)
    double m_g_command;                 ///< [-] Gz command (lagged)
    double m_sca;                       ///<
    double m_sca_1_dif;                 ///<
    double m_sca_1_lag;                 ///<
    double m_sca_2_lag;                 ///<
    double m_pitch_int;                 ///< [deg] pitch integral
    double m_pitch_nfl;                 ///< [deg] pitch negative feedback loop
    double m_elevator_1;                ///<
    double m_elevator_2;                ///<

    void updateLEF( double statPress, double dynPress );

    void updatePitch( double pitchRate, double g_z,
                      double stickLon, double trimLon,
                      double dynPress );

    void updateRoll( double rollRate, double stickLat, double trimLat, double dynPress);

    void updateYaw();

    double getElevatorMaxRate( double elevator_old, double elevator_new,
                               double delta_max );

    /**
     * @brief getGCommandCAT_I
     * @param stickLon [-]
     * @param trimLon [-]
     * @param q_dif_gained [deg/s^2]
     * @return [-] g command
     */
    double getGCommand( double stickLon, double trimLon, double dynPress,
                        double q_dif_gained );

    double getMaxG( double q_dif_gained );

    double getPitchRateGain( double dynPress );

    double getPitchLoopGain( double dynPress );

    /**
     * @brief getRollRateCommand
     * @param delta_h [deg] horizontal stabilator deflection commanded by the control system
     * @return [rad/s] roll rate command
     */
    double getRollRateCommand( double stickLat, double trimLat, double dynPress,
                               double delta_h );
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // F16C_FLCS_H
