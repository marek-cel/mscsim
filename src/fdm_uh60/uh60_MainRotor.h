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
#ifndef UH60_MAINROTOR_H
#define UH60_MAINROTOR_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/models/fdm_MainRotor.h>

#include <fdm/ctrl/fdm_Lag.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief UH-60 main rotor base class.
 */
class UH60_MainRotor : public MainRotor
{
public:

    /** Constructor. */
    UH60_MainRotor();

    /** Destructor. */
    virtual ~UH60_MainRotor();

    /**
     * @brief Updates main rotor model.
     * @param omega      [rad/s] rotor revolution speed
     * @param azimuth    [rad]   rotor azimuth
     * @param collective [rad]   collective pitch angle
     * @param cyclicLat  [rad]   cyclic lateral pitch angle
     * @param cyclicLon  [rad]   cyclic longitudinal pitch angle
     */
    void update( double omega,
                 double azimuth,
                 double collective,
                 double cyclicLat,
                 double cyclicLon );

private:

    Lag *_downwashLag;
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // UH60_MAINROTOR_H
