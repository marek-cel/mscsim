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

#include <fdm_f16c/f16c_FLCS.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

class F16C_Aircraft; ///< aircraft class forward declaration

/**
 * @brief F-16 controls class.
 */
class F16C_Controls : public Controls
{
public:

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

    inline double getAilerons()     const { return m_flcs->getAilerons();     }
    inline double getAileronsNorm() const { return m_flcs->getAileronsNorm(); }
    inline double getElevator()     const { return m_flcs->getElevator();     }
    inline double getElevatorNorm() const { return m_flcs->getElevatorNorm(); }
    inline double getRudder()       const { return m_flcs->getRudder();       }
    inline double getRudderNorm()   const { return m_flcs->getRudderNorm();   }
    inline double getFlapsLE()      const { return m_flcs->getFlapsLE();      }
    inline double getFlapsLENorm()  const { return m_flcs->getFlapsLENorm();  }
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

    F16C_FLCS *m_flcs;                  ///< Fligt Control System

    double m_airbrake_max;              ///< [rad] airbrake deflection limit

    double m_airbrake;                  ///< [rad] airbrake deflection
    double m_airbrake_norm;             ///< [-] normalized airbrake deflection
    double m_brake_l;                   ///< [-] normalized left brake force
    double m_brake_r;                   ///< [-] normalized right brake force
    double m_nose_wheel;                ///< [rad] nose wheel turn angle

    bool m_nwSteering;                  ///< nose wheel steering

    DataRef m_drNwSteering;             ///< nose wheel steering data refernce

    double m_angleOfAttack;
    double m_gz;
    double m_rollRate;
    double m_pitchRate;
    double m_stickLat;
    double m_stickLon;
    double m_trimLat;
    double m_trimLon;
    double m_staticPress;
    double m_dynPress;

    DataRef m_outputAilerons;           ///<
    DataRef m_outputElevator;           ///<
    DataRef m_outputRudder;             ///<
    DataRef m_outputFlaps;              ///<
    DataRef m_outputFlapsLE;            ///<
    DataRef m_outputAirbrake;           ///<
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // F16C_CONTROLS_H
