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
#ifndef FDM_DATAINP_H
#define FDM_DATAINP_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_Defines.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Simulation input data.
 */
struct DataInp
{
    /** Aircraft type. */
    enum AircraftType
    {
        AW101 = 1,                          ///< AW101
        C130  = 2,                          ///< C-130
        C172  = 3,                          ///< Cessna 172
        F16   = 4,                          ///< F-16
        F35A  = 5,                          ///< F-35A
        P51   = 6,                          ///< P-51
        PW5   = 7,                          ///< PW-5
        R44   = 8,                          ///< R44
        UH60  = 9                           ///< UH-60

#       ifdef FDM_TEST
        ,XF = 901                           ///< eXperimental jet Fighter
        ,XH = 902                           ///< eXperimental Helicopter
#       endif

    };

    /** State input. */
    enum StateInp
    {
        Idle = 0,                           ///< idle
        Init,                               ///< initialize
        Work,                               ///< work
        Pause,                              ///< pause
        Stop                                ///< stop
    };

    /** Initial conditions. */
    struct Initial
    {
        double latitude;                    ///< [rad] geodetic latitude (positive north)
        double longitude;                   ///< [rad] geodetic longitude (positive east)
        double altitude_agl;                ///< [m] altitude above ground level
        double offset_x;                    ///< [m] initial position longitudinal offset
        double offset_y;                    ///< [m] initial position lateral offset
        double heading;                     ///< [rad] true heading
        double airspeed;                    ///< [m/s] airspeed

        bool engineOn;                      ///< specifies if engine is working at start
    };

    /** Environment data. */
    struct Environment
    {
        /** Turbulence intensity. */
        enum Turbulence
        {
            TurbulenceNone = 0,             ///< none
            TurbulenceMild,                 ///< mild
            TurbulenceModerate,             ///< moderate
            TurbulenceSevere,               ///< severe
            TurbulenceExtreme               ///< extreme
        };

        /** Wind shear types. */
        enum WindShear
        {
            WindShearNone = 0,              ///< none
            WindShearModel1,                ///< FAA Windshear Training Aid Vol. 2, Reference Wind Model #1, p. 4.3-146
            WindShearModel2,                ///< FAA Windshear Training Aid Vol. 2, Reference Wind Model #2, p. 4.3-148
            WindShearModel3,                ///< FAA Windshear Training Aid Vol. 2, Reference Wind Model #3, p. 4.3-150
            WindShearModel4                 ///< FAA Windshear Training Aid Vol. 2, Reference Wind Model #4, p. 4.3-152
        };

        double temperature_0;               ///< [K] sea level air temperature
        double pressure_0;                  ///< [Pa] sea level air pressure
        double wind_direction;              ///< [rad] wind direction
        double wind_speed;                  ///< [m/s] wind speed

        Turbulence turbulence;              ///< turbulence intensity
        WindShear  windShear;               ///< active wind shear model
    };

    /** Ground data. */
    struct Ground
    {
        double elevation;                   ///< [m] ground elevation above mean sea level

        double r_x_wgs;                     ///< [m] ground x coordinate expressed in WGS
        double r_y_wgs;                     ///< [m] ground y coordinate expressed in WGS
        double r_z_wgs;                     ///< [m] ground z coordinate expressed in WGS

        double n_x_wgs;                     ///< [-] vector normal to ground x coordinate expressed in WGS
        double n_y_wgs;                     ///< [-] vector normal to ground y coordinate expressed in WGS
        double n_z_wgs;                     ///< [-] vector normal to ground z coordinate expressed in WGS
    };

    /**
     * Controls data.
     * @see ISO 1151-4:1994
     * @see ISO 1151-6:1982
     */
    struct Controls
    {
        double roll;                        ///< [-1.0,1.0] roll control (positive in the port direction)
        double pitch;                       ///< [-1.0,1.0] pitch control (positive in the forward direction)
        double yaw;                         ///< [-1.0,1.0] yaw control (positive when the left foot moves in the forward direction)

        double trim_roll;                   ///< [-1.0,1.0] roll trim
        double trim_pitch;                  ///< [-1.0,1.0] pitch trim
        double trim_yaw;                    ///< [-1.0,1.0] yaw trim

        double brake_l;                     ///< [0.0,1.0] left brake
        double brake_r;                     ///< [0.0,1.0] right brake

        double landing_gear;                ///< [0.0,1.0] landing gear
        double wheel_nose;                  ///< [-1.0,1.0] nose wheel steering (positive when the left foot moves in the forward direction)

        double flaps;                       ///< [0.0,1.0] flaps
        double airbrake;                    ///< [0.0,1.0] airbrake
        double spoilers;                    ///< [0.0,1.0] spoilers

        double collective;                  ///< [0.0,1.0] collective control (positive in the upward direction)

        bool lgh;                           ///< landing gear handle
        bool nws;                           ///< nose wheel steering
        bool abs;                           ///< anti-skid braking system
    };

    /** Engine data. */
    struct Engine
    {
        double throttle;                    ///< [0.0,1.0] throttle
        double mixture;                     ///< [0.0,1.0] mixture lever
        double propeller;                   ///< [0.0,1.0] propeller lever

        bool fuel;                          ///< specifies if fuel is provided
        bool ignition;                      ///< specifies if ignition is enabled
        bool starter;                       ///< specifies if starter is enabled
    };

    /** Masses data. */
    struct Masses
    {
        double pilot[ FDM_MAX_PILOTS ];     ///< [kg] pilots
        double tank[ FDM_MAX_TANKS ];       ///< [kg] fuel tanks
        double cabin;                       ///< [kg] cabin
        double trunk;                       ///< [kg] cargo trunk
        double slung;                       ///< [kg] slung load
    };

    /** Recording data. */
    struct Recording
    {
        /** Recording modes. */
        enum Mode
        {
            Disabled = 0,                   ///< disabled
            Record,                         ///< record
            Replay                          ///< replay
        };

        Mode mode;                          ///< recording mode
        char file[ 4096 ];                  ///< recording file
    };

    Initial     initial;                    ///< initial conditions
    Environment environment;                ///< environment data
    Ground      ground;                     ///< ground data
    Controls    controls;                   ///< controls data
    Engine      engine[ FDM_MAX_ENGINES ];  ///< engines data
    Masses      masses;                     ///< masses data
    Recording   recording;                  ///< recording data

    AircraftType aircraftType;              ///< input aircraft type
    StateInp stateInp;                      ///< input state

    bool freezePosition;                    ///< specifies if aircraft position is to be frozen
    bool freezeAttitude;                    ///< specifies if aircraft attitude is to be frozen
    bool freezeVelocity;                    ///< specifies if aircraft velocity is to be frozen
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_DATAINP_H
