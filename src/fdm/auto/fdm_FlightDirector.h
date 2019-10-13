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

    /** Horizontal modes. */
    enum HorMode
    {
        HM_FD = 0,      ///< wing level
        HM_HDG,         ///< heading select
        HM_NAV,         ///< track navigation senson
        HM_APR,         ///< approach
        HM_BC           ///< back course approach
    };

    /** Vertical modes. */
    enum VerMode
    {
        VM_FD = 0,      ///< pitch attitude hold
        VM_ALT,         ///< altitude hold
        VM_IAS,         ///< indicated airspeed hold
        VM_VS,          ///< vertical speed
        VM_ARM,         ///< altitude select
        VM_GS           ///< approach glide slope
    };

    /** Constructor. */
    FlightDirector();

    /** Destructor. */
    virtual ~FlightDirector();

    /**
     * Reads data.
     * @param dataNode XML node
     */
    virtual void readData( XmlNode &dataNode );

    /**
     * Updates flight director.
     */
    virtual void update( double timeStep, double heading, double altitude,
                         double airspeed, double climbRate, double turnRate,
                         double deviation_hor, double deviation_ver );

    void disableHalfBank();
    void enableHalfBank();

    inline void disengage() { _engaged = false; }
    inline void engage()    { _engaged = true;  }

    inline HorMode getHorMode() const { return _hor_mode; }
    inline VerMode getVerMode() const { return _ver_mode; }

    inline double getCmdRoll()  const { return _cmd_roll;  }
    inline double getCmdPitch() const { return _cmd_pitch; }

    inline double getAltitude()  const { return _altitude;  }
    inline double getAirspeed()  const { return _airspeed;  }
    inline double getHeading()   const { return _heading;   }
    inline double getClimbRate() const { return _climbRate; }
    inline double getPitch()     const { return _pitch;     }

    inline bool isEngaged() const { return _engaged; }

    inline void setHorMode( HorMode hor_mode ) { _hor_mode = hor_mode; }
    inline void setVerMode( VerMode ver_mode ) { _ver_mode = ver_mode; }

    void setAltitude( double altitude );
    void setAirspeed( double airspeed );
    void setClimbRate( double climbRate );
    void setHeading( double heading );
    void setCourse( double course );
    void setPitch( double pitch );

    void toggleHorMode( HorMode hor_mode );
    void toggleVerMode( VerMode ver_mode );

protected:

    HorMode _hor_mode;      ///< horizontal mode
    VerMode _ver_mode;      ///< vertical mode

    PID _pid_alt;           ///<
    PID _pid_ias;           ///<
    PID _pid_vs;            ///<
    PID _pid_arm;           ///<
    PID _pid_hdg;           ///<
    PID _pid_trn;           ///<

    double _max_roll;       ///< [rad] max roll
    double _min_pitch;      ///< [rad] min pitch
    double _max_pitch;      ///< [rad] max pitch

    double _max_rate_roll;  ///< [rad/s] roll max rate
    double _max_rate_pitch; ///< [rad/s] pitch max rate
    double _max_rate_tr;    ///< [rad/s^2] turn rate max rate

    double _cmd_roll;       ///< roll command
    double _cmd_pitch;      ///< pitch command

    double _altitude;       ///< [m]   desired altitude
    double _airspeed;       ///< [m/s] desired airspeed
    double _heading;        ///< [rad] desired heading
    double _course;         ///< [rad] desired course
    double _climbRate;      ///< [m/s] desired climb rate
    double _pitch;          ///< [rad] desired pitch

    double _climbRate_act;  ///< [m/s] acting desired climb rate
    double _climbRate_tc;   ///< [s] acting desired climb rate time constant

    double _turnRate;       ///< [rad/s] turn rate

    double _min_dh_arm;     ///< [m] ARM mode min altitude difference

    bool _engaged;          ///< specify if flight director is engaged

    virtual void readMode( const fdm::XmlNode &dataNode, PID &pid, double min, double max );
    virtual void readPID( const fdm::XmlNode &dataNode, PID &pid, double min, double max );

    virtual void updateHorFD( double timeStep, double turnRate );
    virtual void updateHorHDG( double timeStep, double heading );
    virtual void updateHorNAV();
    virtual void updateHorAPR();
    virtual void updateHorBC();
    virtual void updateHorTRN( double timeStep, double turnRate );

    virtual void updateVerFD( double timeStep );
    virtual void updateVerALT( double timeStep, double altitude );
    virtual void updateVerIAS( double timeStep, double airspeed );
    virtual void updateVerVS( double timeStep, double climbRate );
    virtual void updateVerARM( double timeStep, double altitude, double climbRate );
    virtual void updateVerGS();
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_FLIGHTDIRECTOR_H
