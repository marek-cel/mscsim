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
#ifndef PW5_CONTROLS_H
#define PW5_CONTROLS_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_Controls.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

class PW5_Aircraft;    // aircraft class forward declaration

/**
 * @brief PW-5 controls class.
 */
class PW5_Controls : public Controls
{
public:

    /** Constructor. */
    PW5_Controls( const PW5_Aircraft *aircraft, Input *input );

    /** Destructor. */
    virtual ~PW5_Controls();

    /** Initializes controls. */
    void initialize();

    /** Updates controls. */
    void update();

    inline double getAilerons()     const { return _ailerons;      }
    inline double getElevator()     const { return _elevator;      }
    inline double getRudder()       const { return _rudder;        }
    inline double getElevatorTrim() const { return _elevator_trim; }
    inline double getAirbrake()     const { return _airbrake;      }
    inline double getWheelBrake()   const { return _wheelBrake;    }

private:

    const PW5_Aircraft *_aircraft;      ///< aircraft model main object

    Channel *_channelAilerons;          ///< ailerons channel
    Channel *_channelElevator;          ///< elevator channel
    Channel *_channelRudder;            ///< rudder channel
    Channel *_channelElevatorTrim;      ///< elevator trim channel
    Channel *_channelAirbrake;          ///< airbrake channel
    Channel *_channelWheelBrake;        ///< wheel brake channel

    double _ailerons;                   ///< [rad] ailerons deflection
    double _elevator;                   ///< [rad] elevator deflection
    double _rudder;                     ///< [rad] rudder deflection
    double _elevator_trim;              ///< [rad] elevator trim deflection
    double _airbrake;                   ///< [-] normalized airbrake deflection
    double _wheelBrake;                 ///< [-] normalized wheel brake force
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // PW5_CONTROLS_H
