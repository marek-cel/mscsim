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
#ifndef FDM_DATAOUT_H
#define FDM_DATAOUT_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_Defines.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Simulation output data.
 */
struct DataOut
{
    /** State output. */
    enum StateOut
    {
        Idle = 0,                       ///< idle
        Initializing,                   ///< initializing
        Ready,                          ///< ready
        Working,                        ///< working
        Frozen,                         ///< frozen
        Paused,                         ///< paused
        Stopped                         ///< stopped
    };

    /** Aircraft crash condition. */
    enum Crash
    {
        NoCrash = 0,                    ///< no crash
        Collision,                      ///< collision with terrain or obstacle
        Overspeed,                      ///< airspeed too high
        Overstressed                    ///< load factor too high
    };

    /** Flight data. */
    struct Flight
    {
        double latitude;                ///< [rad] geodetic latitude (positive north)
        double longitude;               ///< [rad] geodetic longitude (positive east)

        double altitude_asl;            ///< [m] altitude above mean sea level
        double altitude_agl;            ///< [m] altitude above ground level

        double roll;                    ///< [rad] roll angle
        double pitch;                   ///< [rad] pitch angle
        double heading;                 ///< [rad] true heading

        double angleOfAttack;           ///< [rad] angle of attack
        double sideslipAngle;           ///< [rad] angle of sideslip

        double climbAngle;              ///< [rad] climb angle
        double trackAngle;              ///< [rad] track angle

        double slipSkidAngle;           ///< [rad] slip/skid angle

        double airspeed;                ///< [m/s] airspeed
        double machNumber;              ///< [-]   Mach number
        double climbRate;               ///< [m/s] climb rate

        double rollRate;                ///< [rad/s] roll rate  (angular velcoity p component expressed in BAS)
        double pitchRate;               ///< [rad/s] pitch rate (angular velcoity q component expressed in BAS)
        double yawRate;                 ///< [rad/s] yaw rate   (angular velcoity r component expressed in BAS)
        double turnRate;                ///< [rad/s] turn rate  (heading change rate)

        double pos_x_wgs;               ///< [m] postion x coordinate expressed in WGS
        double pos_y_wgs;               ///< [m] postion y coordinate expressed in WGS
        double pos_z_wgs;               ///< [m] postion z coordinate expressed in WGS

        double att_e0_wgs;              ///< [-] attitude quaternion e0 component expressed as rotation from WGS to BAS
        double att_ex_wgs;              ///< [-] attitude quaternion ex component expressed as rotation from WGS to BAS
        double att_ey_wgs;              ///< [-] attitude quaternion ey component expressed as rotation from WGS to BAS
        double att_ez_wgs;              ///< [-] attitude quaternion ez component expressed as rotation from WGS to BAS

        double vel_u_bas;               ///< [m/s] velcoity u component expressed in BAS
        double vel_v_bas;               ///< [m/s] velcoity v component expressed in BAS
        double vel_w_bas;               ///< [m/s] velcoity w component expressed in BAS

        double omg_p_bas;               ///< [rad/s] angular velcoity p component expressed in BAS
        double omg_q_bas;               ///< [rad/s] angular velcoity q component expressed in BAS
        double omg_r_bas;               ///< [rad/s] angular velcoity r component expressed in BAS

        double phi_wgs;                 ///< [rad] angle of rotation about x axis from WGS to BAS expressed in zyx convention (Bryant angles)
        double tht_wgs;                 ///< [rad] angle of rotation about y axis from WGS to BAS expressed in zyx convention (Bryant angles)
        double psi_wgs;                 ///< [rad] angle of rotation about z axis from WGS to BAS expressed in zyx convention (Bryant angles)

        double tas_u_bas;               ///< [m/s] velcoity relative to airflow u component expressed in BAS
        double tas_v_bas;               ///< [m/s] velcoity relative to airflow v component expressed in BAS
        double tas_w_bas;               ///< [m/s] velcoity relative to airflow w component expressed in BAS

        double vel_north;               ///< [m/s] north velcoity u component expressed in BAS
        double vel_east;                ///< [m/s] east velcoity v component expressed in BAS

        double acc_x_bas;               ///< [m/s^2] linear acceleration x component expressed in BAS
        double acc_y_bas;               ///< [m/s^2] linear acceleration y component expressed in BAS
        double acc_z_bas;               ///< [m/s^2] linear acceleration z component expressed in BAS

        double g_force_x;               ///< [-] x component of G-Force vector expressed in BAS (aircraft)
        double g_force_y;               ///< [-] y component of G-Force vector expressed in BAS (aircraft)
        double g_force_z;               ///< [-] z component of G-Force vector expressed in BAS (aircraft)

        double g_pilot_x;               ///< [-] x component of G-Force vector expressed in BAS (pilot)
        double g_pilot_y;               ///< [-] y component of G-Force vector expressed in BAS (aircraft)
        double g_pilot_z;               ///< [-] z component of G-Force vector expressed in BAS (aircraft)

        bool onGround;                  ///< specifies if aircraft is on ground
        bool stall;                     ///< specifies if aircraft is stalling
    };

    /** Controls data. */
    struct Controls
    {
        double ailerons;                ///< [rad] ailerons deflection (positive left aileron in the upward direction)
        double elevator;                ///< [rad] elevator deflection (positive in the downward direction)
        double elevons;                 ///< [rad] elevons differential deflection
        double rudder;                  ///< [rad] rudder deflection (positive in the port direction)
        double flaps;                   ///< [rad] flaps deflection
        double flaperons;               ///< [rad] flaperons differtial deflection
        double lef;                     ///< [rad] leading edge flaps deflection
        double airbrake;                ///< [rad] airbrake deflection
    };

    /** Engine data. */
    struct Engine
    {
        bool state;                     ///< specifies if engine is working
        bool afterburner;               ///< afterburner

        double rpm;                     ///< [rpm]
        double prop;                    ///< [rpm]
        double ng;                      ///< [%]
        double n1;                      ///< [%]
        double n2;                      ///< [%]
        double trq;                     ///< [%]
        double epr;                     ///< [-]
        double map;                     ///< [Pa]
        double egt;                     ///< [deg C]
        double itt;                     ///< [deg C]
        double tit;                     ///< [deg C]

        double fuelFlow;                ///< [kg/s] fuel flow
    };

    /** Rotor data. */
    struct Rotor
    {
        double mainRotor_azimuth;       ///< [rad] main rotor rotation angle (azimuth)
        double mainRotor_coningAngle;   ///< [rad] main rotor coning angle
        double mainRotor_diskRoll;      ///< [rad] main rotor disk roll angle
        double mainRotor_diskPitch;     ///< [rad] main rotor disk pitch angle
        double mainRotor_collective;    ///< [rad] main rotor collective pitch angle
        double mainRotor_cyclicLon;     ///< [rad] main rotor longitudinal cyclic pitch angle
        double mainRotor_cyclicLat;     ///< [rad] main rotor lateral cyclic pitch angle
        double tailRotor_azimuth;       ///< [rad] tail rotor rotation angle
    };

    Flight flight;                      ///< flight data
    Controls controls;                  ///< controls data
    Engine engine[ FDM_MAX_ENGINES ];   ///< engines data
    Rotor  rotor;                       ///< rotor data
    Crash crash;                        ///< crash cause
    StateOut stateOut;                  ///< output state
    double landing_gear;                ///< [0.0,1.0] landing gear
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_DATAOUT_H
