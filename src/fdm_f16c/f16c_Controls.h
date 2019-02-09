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
#ifndef F16C_CONTROLS_H
#define F16C_CONTROLS_H

////////////////////////////////////////////////////////////////////////////////

#include <fdmMain/fdm_Controls.h>

#include <fdmSys/fdm_PID.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

class F16C_Aircraft; ///< aircraft class forward declaration

/**
 * @brief F-16 controls class.
 *
 * @see Nguyen L., et al.: Simulator Study of Stall/Post-Stall Characteristics of a Fighter Airplane With Relaxed Longitudinal Static Stability, NASA-TP-1538
 * @see Droste T., Walker J.: The General Dynamics Case Study on the F-16 Fly-by-Wire Flight Control System
 * @see Marchand M.: Pitch Rate Flight Control for the F-16 Aircraft to improve Air-to-Air Combat, 1977
 * @see Kassan M.: F-16 Simulator for Man-in-the-Loop Testing of Aircraft Control Systems, 1987
 * @see Dameron G.: A Real-Time Simulator for Man-in-the-Loop Testing of Aircraft Control System, 1988
 * @see Flight Manual USAF/EPAF Series Aircraft F-16A/B Blocks 10 and 15. T.O. 1F-16A-1, 2003
 * @see Flight Manual HAF Series Aircraft F-16C/D Blocks 50 and 52+. T.O. GR1F-16CJ-1, 2003
 */
class F16C_Controls : public Controls
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
    F16C_Controls( const F16C_Aircraft *aircraft );

    /** Destructor. */
    ~F16C_Controls();

    /**
     * Reads data.
     * @param dataNode XML node
     */
    void readData( XmlNode &dataNode );

    /**
     * Initializes data referneces.
     */
    void initDataRefs();

    /**
     * Updates model.
     */
    void update();

    inline double getAilerons()     const { return m_ailerons;      }
    inline double getAileronsNorm() const { return m_ailerons_norm; }
    inline double getElevator()     const { return m_elevator;      }
    inline double getElevatorNorm() const { return m_elevator_norm; }
    inline double getRudder()       const { return m_rudder;        }
    inline double getRudderNorm()   const { return m_rudder_norm;   }
    inline double getFlapsLE()      const { return m_flaps_le;      }
    inline double getFlapsLENorm()  const { return m_flaps_le_norm; }
    inline double getAirbrake()     const { return m_airbrake;      }
    inline double getAirbrakeNorm() const { return m_airbrake_norm; }
    inline double getBrakeL()       const { return m_brake_l;       }
    inline double getBrakeR()       const { return m_brake_r;       }
    inline double getNoseWheel()    const { return m_nose_wheel;    }
    inline bool   getNwSteering()   const { return m_nwSteering;    }

private:

    const F16C_Aircraft *m_aircraft;    ///< aircraft model main object

    Channel *m_channelRoll;             ///< roll channel
    Channel *m_channelPitch;            ///< pitch channel
    Channel *m_channelYaw;              ///< yaw channel
    Channel *m_channelRollTrim;         ///< roll trim channel
    Channel *m_channelPitchTrim;        ///< pitch trim channel
    Channel *m_channelYawTrim;          ///< yaw trim channel
    Channel *m_channelAirbrake;         ///< airbrake channel
    Channel *m_channelBrakeL;           ///< left brake channel
    Channel *m_channelBrakeR;           ///< right brake channel
    Channel *m_channelNoseWheel;        ///< nose wheel channel

    double m_ailerons_max;
    double m_elevator_max;              ///< [rad] elevator deflection limit
    double m_rudder_max;
    double m_flaps_le_max;              ///< [rad] leading edgle flaps deflection limit
    double m_airbrake_max;

    double m_ailerons;                  ///< [rad] ailerons deflection
    double m_ailerons_norm;             ///< [-] normalized ailerons deflection
    double m_elevator;                  ///< [rad] elevator deflection
    double m_elevator_norm;             ///< [-] normalized elevator deflection
    double m_rudder;                    ///< [rad] rudder deflection
    double m_rudder_norm;               ///< [-] normalized rudder deflection
    double m_flaps_le;                  ///< [rad] leading edge flaps deflection
    double m_flaps_le_norm;             ///< [-] normalized leading edge flaps deflection
    double m_airbrake;                  ///< [rad] airbrake deflection
    double m_airbrake_norm;             ///< [-] normalized airbrake deflection
    double m_brake_l;                   ///< [-] normalized left brake force
    double m_brake_r;                   ///< [-] normalized right brake force
    double m_nose_wheel;                ///< [rad] nose wheel turn angle

    bool m_nwSteering;                  ///< nose wheel steering

    DataRef m_drNwSteering;             ///< nose wheel steering data refernce

    CAT   m_cat;                        ///< loading category
    Gains m_gains;                      ///< FLCS gains mode

    double m_alpha_deg;                 ///< [deg] angle of attack
    double m_pitch_int;                 ///< [rad] pitch integral

    DataRef m_outputAilerons;           ///<
    DataRef m_outputElevator;           ///<
    DataRef m_outputRudder;             ///<
    DataRef m_outputFlaps;              ///<
    DataRef m_outputFlapsLE;            ///<
    DataRef m_outputAirbrake;           ///<

    void updateAFCS();
    void updateAFCS_Flaps();
    void updateAFCS_Pitch();
    void updateAFCS_Roll();
    void updateAFCS_Yaw();

    /**
     * @brief getGCommandCAT_I
     * @param alpha [deg] angle of attack
     * @return [-] g command
     */
    double getGCommand();

    double getMaxG();

    double getPitchRateGain();

    double getPitchLoopGain();

    /**
     * @brief getRollRateCommand
     * @param delta_h [deg] horizontal stabilator deflection commanded by the control system
     * @return [rad/s] roll rate command
     */
    double getRollRateCommand( double delta_h );
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // F16C_CONTROLS_H
