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
#ifndef DATA_H
#define DATA_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_DataInp.h>
#include <fdm/fdm_DataOut.h>

////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Simulation data access class.
 */
class Data
{
public:

    /** Airport data. */
    struct Airport
    {
        bool lightsHELI;                    ///< helipad lights
        bool lightsRALS;                    ///< Runway Approach Lighting System
        bool lightsRCLS;                    ///< Runway Centerline Lighting System
        bool lightsRELS;                    ///< Runway Edge Light System
        bool lightsTDZL;                    ///< Touchdown Zone Lights
        bool lightsTELS;                    ///< Taxiway Edge Lights System
        bool lightsTWRL;                    ///< Tower Lights
        bool lightsVGSI;                    ///< Visual Glide Slope Indicator

        bool gatesRwy18;                    ///<
        bool gatesRwy36;                    ///<
    };

    /** Camera data. */
    struct Camera
    {
        /** Camera manipulator types. */
        enum ViewType
        {
            ViewChase = 1,                  ///< chase view camera manipulator
            ViewPilot = 2,                  ///< pilot view camera manipulator
            ViewOrbit = 3,                  ///< orbit view camera manipulator
            ViewWorld = 4                   ///< world view camera manipulator
        };

        double latitude;                    ///< [rad] camera latitude
        double longitude;                   ///< [rad] camera longitude
        double altitude_agl;                ///< [m] camera altitude above ground level
        double altitude_asl;                ///< [m] camera altitude above sea level

        ViewType viewType;                  ///< camera manipulator type
    };

    /** Date and Time. */
    struct DateTime
    {
        unsigned short year;                ///< year
        unsigned short month;               ///< 1...12 month
        unsigned short day;                 ///< 1...31 day

        unsigned short hour;                ///< 0...23 hour
        unsigned short minute;              ///< 0...59 minute
        unsigned short second;              ///< 0...59 second
    };

    /** Environment data. */
    struct Environment
    {
        typedef fdm::DataInp::Environment::WindShear WindShear;

        float visibility;                   ///< [m] visibility due haze, fog, etc

        double temperature_0;               ///< [K] sea level air temperature
        double pressure_0;                  ///< [Pa] sea level air pressure

        double wind_direction;              ///< [rad] wind direction
        double wind_speed;                  ///< [m/s] wind speed
        double turbulence;                  ///< [-] turbulence intensity

        WindShear windShear;                ///< wind shear model type
    };

    /** HUD data. */
    struct HUD
    {
        bool enabled;                       ///< specifies if HUD is enabled

        float color_r;                      ///< [-] HUD color red component
        float color_g;                      ///< [-] HUD color green component
        float color_b;                      ///< [-] HUD color blue component

        float opacity;                      ///< [-] HUD opacity

        float roll;                         ///< [rad] roll angle
        float pitch;                        ///< [rad] pitch angle
        float heading;                      ///< [rad] true heading

        float angleOfAttack;                ///< [rad] angle of attack
        float sideslipAngle;                ///< [rad] angle of sideslip

        float altitude;                     ///< [m]   baro altitude
        float climbRate;                    ///< [m/s] climb rate
        float radioAlt;                     ///< [m]   radio altitude

        float airspeed;                     ///< [m/s] airspeed
        float machNumber;                   ///< [-]   Mach number
        float g_force;                      ///< [-] G-Force

        bool  ils_visible;                  ///< specifies if ILS is visible
        float ils_gs_deviation;             ///< [-1.0,1.0] ILS Glide Slope deviation
        float ils_lc_deviation;             ///< [-1.0,1.0] ILS Localizer deviation

        bool stall;                         ///< stall flag
    };

    /** Navigation data. */
    struct Navigation
    {
        bool  adf_visible;                  ///<
        float adf_bearing;                  ///< [rad]

        bool  ils_visible;                  ///<
        bool  ils_gs_visible;               ///<
        bool  ils_lc_visible;               ///<
        float ils_gs_deviation;             ///< [1.0,1.0] horizontal deviation
        float ils_lc_deviation;             ///< [1.0,1.0] vertical deviation

        bool  nav_visible;                  ///<
        float nav_deviation;                ///<
        float nav_distance;                 ///<
    };

    /** Ownship data. */
    struct Ownship
    {
        char aircraftFile[ 1024 ];          ///< ownship model file path

        double latitude;                    ///< [rad] geodetic latitude
        double longitude;                   ///< [rad] geodetic longitude

        double altitude_asl;                ///< [m] altitude above mean sea level
        double altitude_agl;                ///< [m] altitude above ground level

        double roll;                        ///< [rad] roll angle
        double pitch;                       ///< [rad] pitch angle
        double heading;                     ///< [rad] true heading

        double angleOfAttack;               ///< [rad] angle of attack
        double sideslipAngle;               ///< [rad] angle of sideslip

        double course;                      ///< [rad] course
        double pathAngle;                   ///< [rad] path angle

        double slipSkidAngle;               ///< [rad] slip/skid angle

        double airspeed;                    ///< [m/s] airspeed
        double machNumber;                  ///< [-]   Mach number
        double climbRate;                   ///< [m/s] climb rate

        double rollRate;                    ///< [rad/s] roll rate  (angular velcoity p component expressed in BAS)
        double pitchRate;                   ///< [rad/s] pitch rate (angular velcoity q component expressed in BAS)
        double yawRate;                     ///< [rad/s] yaw rate   (angular velcoity r component expressed in BAS)
        double turnRate;                    ///< [rad/s] turn rate  (heading change rate)

        double g_force_x;                   ///< [-] x component of G-Force vector expressed in BAS (aircraft)
        double g_force_y;                   ///< [-] y component of G-Force vector expressed in BAS (aircraft)
        double g_force_z;                   ///< [-] z component of G-Force vector expressed in BAS (aircraft)

        double pos_x_wgs;                   ///< [m] postion x coordinate expressed in WGS
        double pos_y_wgs;                   ///< [m] postion y coordinate expressed in WGS
        double pos_z_wgs;                   ///< [m] postion z coordinate expressed in WGS

        double att_e0_wgs;                  ///< [-] attitude quaternion e0 component expressed as rotation from WGS to BAS
        double att_ex_wgs;                  ///< [-] attitude quaternion ex component expressed as rotation from WGS to BAS
        double att_ey_wgs;                  ///< [-] attitude quaternion ey component expressed as rotation from WGS to BAS
        double att_ez_wgs;                  ///< [-] attitude quaternion ez component expressed as rotation from WGS to BAS

        float ailerons;                     ///< [rad] ailerons deflection (positive left aileron in the upward direction)
        float elevator;                     ///< [rad] elevator deflection (positive in the downward direction)
        float elevons;                      ///< [rad] elevons differential deflection
        float rudder;                       ///< [rad] rudder deflection (positive in the port direction)
        float flaps;                        ///< [rad] flaps deflection
        float flaperons;                    ///< [rad] flaperons differtial deflection
        float lef;                          ///< [rad] leading edge flaps deflection
        float airbrake;                     ///< [rad] airbrake deflection
        float landingGear;                  ///< [0.0,1.0] landing gear normalized deflection

        float propeller[ FDM_MAX_ENGINES ]; ///< [rad] propeller angle

        float mainRotor_azimuth;            ///< [rad] main rotor rotation angle (azimuth)
        float mainRotor_coningAngle;        ///< [rad] main rotor coning angle
        float mainRotor_diskRoll;           ///< [rad] main rotor disk roll angle
        float mainRotor_diskPitch;          ///< [rad] main rotor disk pitch angle
        float mainRotor_collective;         ///< [rad] main rotor collective pitch angle
        float mainRotor_cyclicLon;          ///< [rad] main rotor longitudinal cyclic pitch angle
        float mainRotor_cyclicLat;          ///< [rad] main rotor lateral cyclic pitch angle
        float tailRotor_azimuth;            ///< [rad] tail rotor rotation angle
        float mainRotor_coef;               ///< [-] main rotor rotation coefficient (sign)
        float tailRotor_coef;               ///< [-] tail rotor rotation coefficient (sign)

        bool onGround;                      ///< specifies if aircraft is on ground
        bool stall;                         ///< specifies if aircraft is stalling
    };

    /** Propulsion data. */
    struct Propulsion
    {
        struct Engine
        {
            bool state;                     ///< specifies if engine is working
            bool afterburner;               ///<

            float rpm;                      ///< [rpm]
            float prop;                     ///< [rpm]
            float ng;                       ///< [%]
            float n1;                       ///< [%]
            float n2;                       ///< [%]
            float trq;                      ///< [%]
            float epr;                      ///< [-]
            float map;                      ///< [Pa]
            float egt;                      ///< [deg C]
            float itt;                      ///< [deg C]
            float tit;                      ///< [deg C]
            float ff;                       ///< [kg/s]

            float throttle;                 ///< [0.0,1.0] throttle
            float mixture;                  ///< [0.0,1.0] mixture lever
            float propeller;                ///< [0.0,1.0] propeller lever

            bool fuel;                      ///<
            bool ignition;                  ///<
            bool starter;                   ///<
        };

        Engine engine[ FDM_MAX_ENGINES ];   ///< engines data
    };

    /** Sky dome data. */
    struct SkyDome
    {
        float skyScale;                     ///< [-] sky dome scaling factor
        float sunAlpha;                     ///< [rad] Sun right ascension
        float sunDelta;                     ///< [rad] Sun declination
        float sunElev;                      ///< [rad] Sun elevation
        float sunAzim;                      ///< [rad] Sun azimuth
    };

    /** Simulation data struct. */
    struct DataBuf
    {
        typedef fdm::DataInp::AircraftType AircraftType;
        typedef fdm::DataInp::PhaseInp PhaseInp;
        typedef fdm::DataOut::StateOut StateOut;

        typedef fdm::DataInp::Controls Controls;
        typedef fdm::DataInp::Ground   Ground;
        typedef fdm::DataInp::Initial  Initial;
        typedef fdm::DataInp::Masses   Masses;

        Airport     airport;                ///< airport data
        Camera      camera;                 ///< camera data
        Controls    controls;               ///< controls data
        DateTime    dateTime;               ///< date time data
        Environment environment;            ///< environment data
        Ground      ground;                 ///< ground data
        HUD         hud;                    ///< HUD data
        Initial     initial;                ///< initial conditions
        Masses      masses;                 ///< masses data
        Navigation  navigation;             ///< navigation data
        Ownship     ownship;                ///< ownship data
        Propulsion  propulsion;             ///< propulsion data
        SkyDome     skyDome;                ///< sky dome data

        AircraftType aircraftType;          ///< input aircraft type

        PhaseInp phaseInp;                  ///< simulation input phase
        StateOut stateOut;                  ///< simulation output state

        double timeCoef;                    ///< [-] time coefficient
        double timeStep;                    ///< [s] simulation time step
    };

private:

    /**
     * You should use static function instance() due to get refernce
     * to Data class instance.
     */
    Data() {}

    /** Using this constructor is forbidden. */
    Data( const Data & ) {}

public:

    /** Returns pointer to the common data object. */
    static inline DataBuf* get() { return &m_data; }

    /** Destructor. */
    virtual ~Data() {}

private:

    static DataBuf m_data;  ///< simulation data
};

////////////////////////////////////////////////////////////////////////////////

#endif // DATA_H
