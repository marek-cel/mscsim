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
#ifndef FDM_AUTOPILOT_H
#define FDM_AUTOPILOT_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_Defines.h>

#include <fdm/ctrl/fdm_PID.h>
#include <fdm/utils/fdm_Table1.h>
#include <fdm/xml/fdm_XmlNode.h>

#include <fdm/auto/fdm_FlightDirector.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Autopilot class.
 *
 * @see McLean D.: Automatic Flight Control Systems, 1990
 * @see Roskam J.: Airplane Flight Dynamics and Automatic Flight Controls, 2001
 * @see Bociek S., Gruszecki J.: Uklady sterowania automatycznego samolotem, 1999 [in Polish]
 * @see Polak Z., Rypulak A.: Awionika przyrzady i systemy pokladowe, 2002 [in Polish]
 */
class FDMEXPORT Autopilot
{
public:

    /** @brief Constructor. */
    Autopilot( FlightDirector *fd );

    /** @brief Destructor. */
    virtual ~Autopilot();

    /**
     * @brief Reads data.
     * @param dataNode XML node
     */
    virtual void readData( XmlNode &dataNode );

    /** @brief Initializes autopilot. */
    virtual void initialize();

    /**
     * @brief Updates autopilot.
     * @param timeStep [s]
     * @param roll [rad]
     * @param pitch [rad]
     * @param heading [rad]
     * @param altitude [m]
     * @param airspeed [m/s]
     * @param turn_rate [rad/s]
     * @param yaw_rate [rad/s]
     * @param climb_rate [m/s]
     * @param dme_distance [m]
     * @param nav_deviation
     * @param nav_active
     * @param loc_deviation
     * @param loc_active
     * @param gs_deviation
     * @param gs_active
     */
    virtual void update( double timeStep,
                         double roll, double pitch, double heading,
                         double altitude, double airspeed,
                         double turn_rate, double yaw_rate, double climb_rate,
                         double dme_distance,
                         double nav_deviation, bool nav_active,
                         double loc_deviation, bool loc_active,
                         double gs_deviation,  bool gs_active );

    inline void disengage() { _engaged = false; }
    inline void engage()    { _engaged = true;  }

    inline double getMinAltitude()  const { return _min_alt; }
    inline double getMaxAltitude()  const { return _max_alt; }
    inline double getMinClimbRate() const { return _min_vs;  }
    inline double getMaxClimbRate() const { return _max_vs;  }

    inline double getAltitude()  const { return _fd->getAltitude();  }
    inline double getAirspeed()  const { return _fd->getAirspeed();  }
    inline double getHeading()   const { return _fd->getHeading();   }
    inline double getClimbRate() const { return _fd->getClimbRate(); }
    inline double getPitch()     const { return _fd->getPitch();     }

    inline double getCmdRoll()  const { return _fd->getCmdRoll();  }
    inline double getCmdPitch() const { return _fd->getCmdPitch(); }

    inline double getCtrlRoll()  const { return _ctrl_roll;  }
    inline double getCtrlPitch() const { return _ctrl_pitch; }
    inline double getCtrlYaw()   const { return _ctrl_yaw;   }

    inline bool isActiveAP() const { return _engaged; }
    inline bool isActiveYD() const { return _yaw_damper; }

    inline bool isActiveFD() const { return _fd->isEngaged(); }

    void setAltitude( double altitude );
    void setAirspeed( double airspeed );
    void setClimbRate( double climb_rate );
    void setHeading( double heading );
    void setCourse( double course );
    void setRoll( double roll );
    void setPitch( double pitch );

protected:

    FlightDirector *_fd;    ///< flight director

    PID _pid_r;             ///< roll PID controller
    PID _pid_p;             ///< pitch PID controller
    PID _pid_y;             ///< yaw PID controller

    Table1 _gain_ias_r;     ///< roll gain schedule due to airspeed
    Table1 _gain_ias_p;     ///< pitch gain schedule due to airspeed
    Table1 _gain_ias_y;     ///< yaw gain schedule due to airspeed

    double _max_rate_roll;  ///< [1/s] max control roll rate
    double _max_rate_pitch; ///< [1/s] max control pitch rate
    double _max_rate_yaw;   ///< [1/s] max control yaw rate

    double _min_roll;       ///< [rad] min roll
    double _max_roll;       ///< [rad] max roll
    double _min_pitch;      ///< [rad] min pitch
    double _max_pitch;      ///< [rad] max pitch
    double _min_alt;        ///< [m]   min altitude
    double _max_alt;        ///< [m]   max altitude
    double _min_ias;        ///< [m/s] min airspeed
    double _max_ias;        ///< [m/s] max airspeed
    double _min_vs;         ///< [m/s] min climb rate
    double _max_vs;         ///< [m/s] max climb rate

    double _ctrl_roll;      ///< roll control command
    double _ctrl_pitch;     ///< pitch control command
    double _ctrl_yaw;       ///< yaw control command

    bool _yaw_damper;       ///< specifies if yaw damper is engaged

    bool _testing;          ///< specifies if test is active
    bool _engaged;          ///< specifies if autopilot is engaged

    virtual void readChannel( const XmlNode &dataNode, double *max_rate,
                              PID *pid, Table1 *gain_ias );
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_AUTOPILOT_H
