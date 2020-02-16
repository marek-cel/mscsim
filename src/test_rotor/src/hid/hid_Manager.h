/****************************************************************************//*
 * Copyright (C) 2020 Marek M. Cel
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

#ifndef HID_MANAGER_H
#define HID_MANAGER_H

////////////////////////////////////////////////////////////////////////////////

#include <string>

#include <hid/hid_Singleton.h>

#include "hid_Assignment.h"

////////////////////////////////////////////////////////////////////////////////

namespace hid
{

/** HID manager class. */
class Manager : public Singleton< Manager >
{
    friend class Singleton< Manager >;

public:

    static const std::string _actionNames[ HID_MAX_ACTIONS ];
    static const std::string _keysNames[ HID_MAX_KEYS ];

    /** */
    static Assignment::POVs getPOV( short pov_deg );

    /** */
    static bool isAxis( Assignment::Action action );

private:

    /** Using this constructor is forbidden. */
    Manager( const Manager & ) : Singleton< Manager >() {}

    /** Constructor. */
    Manager();

public:

    /** Destructor. */
    virtual ~Manager();

    /** */
    void init();

    /** */
    void update( double timeStep );

    inline float getCtrlRoll()   const { return _ctrlRoll;   }
    inline float getCtrlPitch()  const { return _ctrlPitch;  }
    inline float getCtrlYaw()    const { return _ctrlYaw;    }
    inline float getCollective() const { return _collective; }

    /** */
    void setAssingment( Assignment::Action action, const Assignment &assignment );

    /** */
    inline void setKeysState( const bool *keysState )
    {
        for ( int i = 0; i < HID_MAX_KEYS; i++ )
        {
            _keysState[ i ] = keysState[ i ];
        }
    }

private:

    Assignment _assignments[ HID_MAX_ACTIONS ];

    bool _keysState[ HID_MAX_KEYS ];

    float _speedControls;       ///< [1/s]
    float _speedCollective;     ///< [1/s]

    double _timeStep;           ///< [s]

    float _ctrlRoll;            ///< -1.0 ... 1.0
    float _ctrlPitch;           ///< -1.0 ... 1.0
    float _ctrlYaw;             ///< -1.0 ... 1.0
    float _collective;          ///<  0.0 ... 1.0

    /** */
    void getAxisValue( const Assignment &assignment, float &value, int absolute = 0 );

    /** */
    void getCtrlValue( Assignment::Action decreaseAction,
                       Assignment::Action increaseAction,
                       float &value,
                       float speed,
                       float min,
                       float max );

    /** */
    void getRealValue( Assignment::Action decreaseAction,
                       Assignment::Action increaseAction,
                       float &value,
                       float speed,
                       float min,
                       float max );

    /** */
    void getRealValue( Assignment::Action applyAction,
                       float &value,
                       float speed,
                       float min,
                       float max );

    /** */
    void getRealValue( Assignment::Action toggleAction,
                       bool  &togglePrev,
                       bool  &state,
                       float &value,
                       float speed,
                       float min,
                       float max );

    /** */
    bool getButtState( const Assignment &assignment );

    /** */
    void reset();

    /** */
    void updateAxisActions();

    /** */
    void updateMiscActions();
};

} // end of hid namepsace

////////////////////////////////////////////////////////////////////////////////

#endif // HID_MANAGER_H
