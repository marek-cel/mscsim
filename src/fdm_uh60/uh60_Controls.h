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
#ifndef UH60_CONTROLS_H
#define UH60_CONTROLS_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/main/fdm_Controls.h>

#include <fdm_uh60/uh60_AFCS.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

class UH60_Aircraft;    ///< aircraft class forward declaration

/**
 * @brief UH-60 controls class.
 */
class UH60_Controls : public Controls
{
public:

    /** Constructor. */
    UH60_Controls( const UH60_Aircraft *aircraft );

    /** Destructor. */
    ~UH60_Controls();

    /** Initializes controls. */
    void init();

    /** Updates model. */
    void update();

    inline double getCyclicLat()  const { return m_cyclic_lat; }
    inline double getCyclicLon()  const { return m_cyclic_lon; }
    inline double getCollective() const { return m_collective; }
    inline double getTailPitch()  const { return m_tail_pitch; }
    inline double getElevator()   const { return m_elevator;   }
    inline double getBrakeL()     const { return m_brake_l;    }
    inline double getBrakeR()     const { return m_brake_r;    }

private:

    const UH60_Aircraft *m_aircraft;    ///< aircraft model main object

    Channel *m_channelCyclicLat;        ///<
    Channel *m_channelCyclicLon;        ///<
    Channel *m_channelCollective;       ///<
    Channel *m_channelTailPitch;        ///<
    Channel *m_channelElevator;         ///<
    Channel *m_channelBrakeL;           ///<
    Channel *m_channelBrakeR;           ///<

    UH60_AFCS *m_afcs;                  ///< Automatic Flight Control System

    double m_cyclic_lat;                ///< [rad]
    double m_cyclic_lon;                ///< [rad]
    double m_collective;                ///< [rad]
    double m_tail_pitch;                ///< [rad]
    double m_elevator;                  ///< [rad]
    double m_brake_l;                   ///< [-]
    double m_brake_r;                   ///< [-]
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // UH60_CONTROLS_H
