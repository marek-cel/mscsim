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
#ifndef UH60_CONTROLS_H
#define UH60_CONTROLS_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_Controls.h>

#include <fdm_uh60/uh60_AFCS.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

class UH60_Aircraft;    // aircraft class forward declaration

/**
 * @brief UH-60 controls class.
 */
class UH60_Controls : public Controls
{
public:

    /** Constructor. */
    UH60_Controls( const UH60_Aircraft *aircraft, Input *input );

    /** Destructor. */
    virtual ~UH60_Controls();

    /** Initializes controls. */
    void initialize();

    /** Updates model. */
    void update();

    inline double getCyclicLat()  const { return _cyclic_lat; }
    inline double getCyclicLon()  const { return _cyclic_lon; }
    inline double getCollective() const { return _collective; }
    inline double getTailPitch()  const { return _tail_pitch; }
    inline double getElevator()   const { return _elevator;   }
    inline double getBrakeL()     const { return _brake_l;    }
    inline double getBrakeR()     const { return _brake_r;    }

private:

    const UH60_Aircraft *_aircraft;     ///< aircraft model main object

    Channel *_channelCyclicLat;         ///<
    Channel *_channelCyclicLon;         ///<
    Channel *_channelCollective;        ///<
    Channel *_channelTailPitch;         ///<
    Channel *_channelElevator;          ///<
    Channel *_channelBrakeLeft;         ///< left brake channel
    Channel *_channelBrakeRight;        ///< right brake channel

    UH60_AFCS *_afcs;                   ///< Automatic Flight Control System

    double _cyclic_lat;                 ///< [rad]
    double _cyclic_lon;                 ///< [rad]
    double _collective;                 ///< [rad]
    double _tail_pitch;                 ///< [rad]
    double _elevator;                   ///< [rad]
    double _brake_l;                    ///< [-]
    double _brake_r;                    ///< [-]
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // UH60_CONTROLS_H
