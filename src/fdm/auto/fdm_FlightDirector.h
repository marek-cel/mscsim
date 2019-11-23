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
#ifndef FDM_FLIGHTDIRECTOR_H
#define FDM_FLIGHTDIRECTOR_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_Defines.h>

#include <fdm/sys/fdm_PID.h>

#include <fdm/xml/fdm_XmlNode.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Flight director class.
 */
class FDMEXPORT FlightDirector
{
public:

    /** Constructor. */
    FlightDirector();

    /** Destructor. */
    virtual ~FlightDirector();

    /**
     * Reads data.
     * @param dataNode XML node
     */
    virtual void readData( XmlNode &dataNode ) = 0;

    /**
     * Updates flight director.
     */
    virtual void update( double timeStep,
                         double heading,
                         double altitude, double airspeed,
                         double turnRate, double climbRate,
                         double dme_distance,
                         double nav_deviation, bool nav_active,
                         double loc_deviation, bool loc_active,
                         double gs_deviation,  bool gs_active ) = 0;

    inline void disengage() { _engaged = false; }
    inline void engage()    { _engaged = true;  }

    inline double getCmdRoll()  const { return _cmd_roll;  }
    inline double getCmdPitch() const { return _cmd_pitch; }

    inline double getAltitude()  const { return _altitude;  }
    inline double getAirspeed()  const { return _airspeed;  }
    inline double getHeading()   const { return _heading;   }
    inline double getClimbRate() const { return _climbRate; }
    inline double getPitch()     const { return _pitch;     }

    inline bool isEngaged() const { return _engaged; }

    void setAltitude( double altitude );
    void setAirspeed( double airspeed );
    void setClimbRate( double climbRate );
    void setHeading( double heading );
    void setCourse( double course );
    void setPitch( double pitch );

protected:

    double _cmd_roll;       ///< roll command
    double _cmd_pitch;      ///< pitch command

    double _altitude;       ///< [m]   desired altitude
    double _airspeed;       ///< [m/s] desired airspeed
    double _heading;        ///< [rad] desired heading
    double _course;         ///< [rad] desired course
    double _climbRate;      ///< [m/s] desired climb rate
    double _pitch;          ///< [rad] desired pitch

    bool _engaged;          ///< specify if flight director is engaged
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_FLIGHTDIRECTOR_H
