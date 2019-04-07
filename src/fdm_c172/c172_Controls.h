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
#ifndef C172_CONTROLS_H
#define C172_CONTROLS_H

////////////////////////////////////////////////////////////////////////////////

#include <fdmMain/fdm_Controls.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

class C172_Aircraft; ///< aircraft class forward declaration

/**
 * @brief Cessna 172 controls class.
 */
class C172_Controls : public Controls
{
public:

    /** Constructor. */
    C172_Controls( const C172_Aircraft *aircraft );

    /** Destructor. */
    ~C172_Controls();

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
    inline double getNoseWheel()    const { return m_nose_wheel;    }
    inline bool   getNwSteering()   const { return m_nwSteering;    }

private:

    const C172_Aircraft *m_aircraft;    ///< aircraft model main object

    Channel *m_channelAilerons;         ///< ailerons channel
    Channel *m_channelElevator;         ///< elevator channel
    Channel *m_channelRudder;           ///< rudder channel
    Channel *m_channelElevatorTrim;     ///< elevator trim channel
    Channel *m_channelFlaps;            ///< flaps channel
    Channel *m_channelBrakeL;           ///< left brake channel
    Channel *m_channelBrakeR;           ///< right brake channel
    Channel *m_channelNoseWheel;        ///< nose wheel channel

    double m_ailerons;                  ///< [rad] ailerons deflection
    double m_elevator;                  ///< [rad] elevator deflection
    double m_rudder;                    ///< [rad] rudder deflection
    double m_elevator_trim;             ///< [rad] elevator trim deflection
    double m_flaps;                     ///< [rad] flaps deflection
    double m_brake_l;                   ///< [-] normalized left brake force
    double m_brake_r;                   ///< [-] normalized right brake force
    double m_nose_wheel;                ///< [rad] nose wheel turn angle

    bool m_nwSteering;                  ///< nose wheel steering

    DataRef m_drNwSteering;             ///< nose wheel steering data refernce

    DataRef m_outputAilerons;           ///<
    DataRef m_outputElevator;           ///<
    DataRef m_outputRudder;             ///<
    DataRef m_outputFlaps;              ///<
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // C172_CONTROLS_H
