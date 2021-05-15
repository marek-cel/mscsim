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
#ifndef C130_CONTROLS_H
#define C130_CONTROLS_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_Controls.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

class C130_Aircraft;    // aircraft class forward declaration

/**
 * @brief C-130 controls class.
 */
class C130_Controls : public Controls
{
public:

    /** Constructor. */
    C130_Controls( const C130_Aircraft *aircraft, Input *input );

    /** Destructor. */
    virtual ~C130_Controls();

    /** Initializes controls. */
    void initialize();

    /** Updates controls. */
    void update();

    inline double getAilerons()     const { return _ailerons;      }
    inline double getElevator()     const { return _elevator;      }
    inline double getRudder()       const { return _rudder;        }
    inline double getElevatorTrim() const { return _elevator_trim; }
    inline double getFlaps()        const { return _flaps;         }
    inline double getBrakeL()       const { return _brake_l;       }
    inline double getBrakeR()       const { return _brake_r;       }

private:

    const C130_Aircraft *_aircraft;     ///< aircraft model main object

    Channel *_channelAilerons;          ///< ailerons channel
    Channel *_channelElevator;          ///< elevator channel
    Channel *_channelRudder;            ///< rudder channel
    Channel *_channelElevatorTrim;      ///< elevator trim channel
    Channel *_channelFlaps;             ///< flaps channel
    Channel *_channelBrakeLeft;         ///< left brake channel
    Channel *_channelBrakeRight;        ///< right brake channel

    double _ailerons;                   ///< [rad] ailerons deflection
    double _elevator;                   ///< [rad] elevator deflection
    double _rudder;                     ///< [rad] rudder deflection
    double _elevator_trim;              ///< [rad] elevator trim deflection
    double _flaps;                      ///< [rad] flaps deflection
    double _brake_l;                    ///< [-] normalized left brake force
    double _brake_r;                    ///< [-] normalized right brake force
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // C130_CONTROLS_H
