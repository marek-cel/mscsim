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
        C130 = 0,                       ///< C-130
        C172 = 1,                       ///< Cessna 172
        F16  = 2,                       ///< F-16
        UH60 = 3                        ///< UH-60

    };

    /** State input. */
    enum PhaseInp
    {
        Idle = 0,                       ///< idle
        Init,                           ///< initialize
        Work,                           ///< work
        Pause,                          ///< pause
        Stop                            ///< stop
    };

    /** Initial conditions. */
    struct Initial
    {
        double latitude;                ///< [rad] geodetic latitude (positive north)
        double longitude;               ///< [rad] geodetic longitude (positive east)
        double altitude_agl;            ///< [m] altitude above ground level
        double heading;                 ///< [rad] true heading
        double airspeed;                ///< [m/s] airspeed

        bool engineOn;                  ///< specifies if engine is working at start
    };

    /** Environment data. */
    struct Environment
    {
        /** Wind shear types. */
        enum WindShear
        {
            None = 0,                   ///< none
            Model1,                     ///< FAA Windshear Training Aid Vol. 2, Reference Wind Model #1, p. 4.3-146
            Model2,                     ///< FAA Windshear Training Aid Vol. 2, Reference Wind Model #2, p. 4.3-148
            Model3,                     ///< FAA Windshear Training Aid Vol. 2, Reference Wind Model #3, p. 4.3-150
            Model4                      ///< FAA Windshear Training Aid Vol. 2, Reference Wind Model #4, p. 4.3-152
        };

        double temperature_0;           ///< [K] sea level air temperature
        double pressure_0;              ///< [Pa] sea level air pressure
        double wind_direction;          ///< [rad] wind direction
        double wind_speed;              ///< [m/s] wind speed
        double turbulence;              ///< [-] turbulence intensity

        WindShear windShear;            ///< active wind shear model
    };

    /** Ground data. */
    struct Ground
    {
        double elevation;               ///< [m] ground elevation above mean sea level

        double r_x_wgs;                 ///< [m] ground x coordinate expressed in WGS
        double r_y_wgs;                 ///< [m] ground y coordinate expressed in WGS
        double r_z_wgs;                 ///< [m] ground z coordinate expressed in WGS

        double n_x_wgs;                 ///< [-] vector normal to ground x coordinate expressed in WGS
        double n_y_wgs;                 ///< [-] vector normal to ground y coordinate expressed in WGS
        double n_z_wgs;                 ///< [-] vector normal to ground z coordinate expressed in WGS
    };

    /**
     * Controls data.
     * @see ISO 1151-4:1994
     * @see ISO 1151-6:1982
     */
    struct Controls
    {
        double roll;                    ///< [-1.0,1.0] roll control (positive in the port direction)
        double pitch;                   ///< [-1.0,1.0] pitch control (positive in the forward direction)
        double yaw;                     ///< [-1.0,1.0] yaw control (positive when the left foot moves in the forward direction)

        double trim_roll;               ///< [-1.0,1.0] roll trim
        double trim_pitch;              ///< [-1.0,1.0] pitch trim
        double trim_yaw;                ///< [-1.0,1.0] yaw trim

        double brake_l;                 ///< [0.0,1.0] left brake
        double brake_r;                 ///< [0.0,1.0] right brake

        double landing_gear;            ///< [0.0,1.0] landing gear
        double nose_wheel;              ///< [-1.0,1.0] nose wheel steering (positive when the left foot moves in the forward direction)

        double flaps;                   ///< [0.0,1.0] flaps
        double airbrake;                ///< [0.0,1.0] airbrake
        double spoilers;                ///< [0.0,1.0] spoilers

        double collective;              ///< [0.0,1.0] collective control (positive in the upward direction)

        bool lg_handle;                 ///< specifies if landing gear handle is down
        bool nw_steering;               ///< specifies if nose wheel seering is enabled
        bool antiskid;                  ///< specifies if antiskid system is enabled
    };

    /** Engine data. */
    struct Engine
    {
        double throttle;                ///< [0.0,1.0] throttle
        double mixture;                 ///< [0.0,1.0] mixture lever
        double propeller;               ///< [0.0,1.0] propeller lever

        bool fuel;                      ///< specifies if fuel is provided
        bool ignition;                  ///< specifies if ignition is enabled
        bool starter;                   ///< specifies if starter is enabled
    };

    /** Masses data. */
    struct Masses
    {
        double pilot_1;                 ///< [kg] pilot 1
        double pilot_2;                 ///< [kg] pilot 2
        double fuel_tank_1;             ///< [kg] fuel tank 1
        double fuel_tank_2;             ///< [kg] fuel tank 2
        double fuel_tank_3;             ///< [kg] fuel tank 3
        double fuel_tank_4;             ///< [kg] fuel tank 4
        double cabin;                   ///< [kg] cabin
        double trunk;                   ///< [kg] cargo trunk
    };

    Initial     initial;                ///< initial conditions
    Environment environment;            ///< environment data
    Ground      ground;                 ///< ground data
    Controls    controls;               ///< controls data
    Engine engine[ FDM_MAX_ENGINES ];   ///< engines data
    Masses      masses;                 ///< masses data

    AircraftType aircraftType;          ///< input aircraft type
    PhaseInp phaseInp;                  ///< input phase
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_DATAINP_H
