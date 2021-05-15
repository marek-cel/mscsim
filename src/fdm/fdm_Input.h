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
#ifndef FDM_INPUT_H
#define FDM_INPUT_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/utils/fdm_DataRef.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Input class.
 */
class FDMEXPORT Input : public DataNode
{
public:

    /** Data references. */
    struct DataRefs
    {
        /** */
        struct Controls
        {
            DataRef roll;                       ///< roll controls data reference
            DataRef pitch;                      ///< pitch control data reference
            DataRef yaw;                        ///< yaw control data reference

            DataRef trim_roll;                  ///< roll trim data reference
            DataRef trim_pitch;                 ///< pitch trim data reference
            DataRef trim_yaw;                   ///< yaw trim data reference

            DataRef brake_l;                    ///< left brake data reference
            DataRef brake_r;                    ///< right brake data reference
            DataRef wheel_brake;                ///< wheel brake data reference

            DataRef landing_gear;               ///< landing gear data reference
            DataRef wheel_nose;                 ///< nose wheel steering data reference

            DataRef flaps;                      ///< flaps data reference
            DataRef airbrake;                   ///< airbrake data reference
            DataRef spoilers;                   ///< spoilers data reference

            DataRef collective;                 ///< collective data reference

            DataRef lgh;                        ///< landing gear handle data reference
            DataRef nws;                        ///< nose wheel steering data reference
            DataRef abs;                        ///< anti-skid braking system data reference
        };

        /** */
        struct Engine
        {
            DataRef  throttle;                  ///< throttle data reference
            DataRef  mixture;                   ///< mixture lever data reference
            DataRef  propeller;                 ///< propeller lever data reference

            DataRef  fuel;                      ///< fuel state data reference
            DataRef  ignition;                  ///< ignition state data reference
            DataRef  starter;                   ///< starter state data reference
        };

        /** */
        struct Masses
        {
            DataRef pilot [ FDM_MAX_PILOTS ];   ///< pilots data reference
            DataRef tank  [ FDM_MAX_TANKS ];    ///< fuel tanks data reference
            DataRef cabin;                      ///< cabin data reference
            DataRef trunk;                      ///< cargo trunk data reference
            DataRef slung;                      ///< slung load data reference
        };

        Controls controls;                      ///< controls data
        Engine   engine[ FDM_MAX_ENGINES ];     ///< engines data
        Masses   masses;                        ///< masses data
    };

    /** @brief Constructor. */
    Input();

    /** @brief Destructor. */
    virtual ~Input();

private:

    void init();
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_INPUT_H
