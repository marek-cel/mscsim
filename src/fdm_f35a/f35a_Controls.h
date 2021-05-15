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
#ifndef F35A_CONTROLS_H
#define F35A_CONTROLS_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_Controls.h>

#include <fdm_f35a/f35a_FLCS.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

class F35A_Aircraft;    // aircraft class forward declaration

/**
 * @brief F-35A controls class.
 */
class F35A_Controls : public Controls
{
public:

    /** Constructor. */
    F35A_Controls( const F35A_Aircraft *aircraft, Input *input );

    /** Destructor. */
    virtual ~F35A_Controls();

    /**
     * Reads data.
     * @param dataNode XML node
     */
    void readData( XmlNode &dataNode );

    /** Initializes controls. */
    void initialize();

    /** Updates model. */
    void update();

    inline const F35A_FLCS* getFLCS() const { return _flcs; }

    inline double getAilerons()  const { return _ailerons;   }
    inline double getElevator()  const { return _elevator;   }
    inline double getRudder()    const { return _rudder;     }
    inline double getFlapsLE()   const { return _flaps_le;   }
    inline double getFlapsTE()   const { return _flaps_te;   }
    inline double getAirbrake()  const { return _airbrake;   }
    inline double getBrakeL()    const { return _brake_l;    }
    inline double getBrakeR()    const { return _brake_r;    }
    inline double getNoseWheel() const { return _nose_wheel; }

private:

    const F35A_Aircraft *_aircraft;     ///< aircraft model main object

    Channel *_channelRoll;              ///< roll channel
    Channel *_channelPitch;             ///< pitch channel
    Channel *_channelYaw;               ///< yaw channel
    Channel *_channelRollTrim;          ///< roll trim channel
    Channel *_channelPitchTrim;         ///< pitch trim channel
    Channel *_channelYawTrim;           ///< yaw trim channel
    Channel *_channelAirbrake;          ///< airbrake channel
    Channel *_channelBrakeLeft;         ///< left brake channel
    Channel *_channelBrakeRight;        ///< right brake channel
    Channel *_channelNoseWheel;         ///< nose wheel channel

    DataRef _inputLGH;                  ///< LGH input data reference

    F35A_FLCS *_flcs;                   ///< Fligt Control System

    double _ailerons_max;               ///< [rad]
    double _elevator_max;               ///< [rad]
    double _rudder_max;                 ///< [rad]
    double _flaps_le_max;               ///< [rad]
    double _flaps_te_max;               ///< [rad]

    double _ailerons;                   ///< [rad]
    double _elevator;                   ///< [rad]
    double _rudder;                     ///< [rad]
    double _flaps_le;                   ///< [rad]
    double _flaps_te;                   ///< [rad]
    double _airbrake;                   ///< [-] normalized airbrake deflection
    double _brake_l;                    ///< [-] normalized left brake force
    double _brake_r;                    ///< [-] normalized right brake force
    double _nose_wheel;                 ///< [rad] nose wheel turn angle
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // F35A_CONTROLS_H
