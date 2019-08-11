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
#ifndef P51_CONTROLS_H
#define P51_CONTROLS_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/main/fdm_Controls.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

class P51_Aircraft;    ///< aircraft class forward declaration

/**
 * @brief P-51 controls class.
 */
class P51_Controls : public Controls
{
public:

    /** Constructor. */
    P51_Controls( const P51_Aircraft *aircraft );

    /** Destructor. */
    ~P51_Controls();

    /** Initializes controls. */
    void init();

    /** Updates controls. */
    void update();

    inline double getAilerons()     const { return m_ailerons;      }
    inline double getElevator()     const { return m_elevator;      }
    inline double getRudder()       const { return m_rudder;        }
    inline double getElevatorTrim() const { return m_elevator_trim; }
    inline double getFlaps()        const { return m_flaps;         }
    inline double getBrakeL()       const { return m_brake_l;       }
    inline double getBrakeR()       const { return m_brake_r;       }

private:

    const P51_Aircraft *m_aircraft;     ///< aircraft model main object

    Channel *m_channelAilerons;         ///< ailerons channel
    Channel *m_channelElevator;         ///< elevator channel
    Channel *m_channelRudder;           ///< rudder channel
    Channel *m_channelElevatorTrim;     ///< elevator trim channel
    Channel *m_channelFlaps;            ///< flaps channel
    Channel *m_channelBrakeL;           ///< left brake channel
    Channel *m_channelBrakeR;           ///< right brake channel

    double m_ailerons;                  ///< [rad] ailerons deflection
    double m_elevator;                  ///< [rad] elevator deflection
    double m_rudder;                    ///< [rad] rudder deflection
    double m_elevator_trim;             ///< [rad] elevator trim deflection
    double m_flaps;                     ///< [rad] flaps deflection
    double m_brake_l;                   ///< [-] normalized left brake force
    double m_brake_r;                   ///< [-] normalized right brake force
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // P51_CONTROLS_H
