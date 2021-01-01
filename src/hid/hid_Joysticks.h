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
#ifndef HID_JOYSTICKS_H
#define HID_JOYSTICKS_H

////////////////////////////////////////////////////////////////////////////////

#ifdef WIN32
#   define HID_WINMM_JOYSTICK
#else
#   ifdef _LINUX_
#       define HID_LINUX_JOYSTICK
#   endif
#endif

////////////////////////////////////////////////////////////////////////////////

#include <string>

#ifdef HID_LINUX_JOYSTICK
#   include <fcntl.h>
#   include <unistd.h>
#   include <linux/joystick.h>
#endif

#ifdef HID_WINMM_JOYSTICK
#   include <Windows.h>
#endif

#include <sim/Singleton.h>

////////////////////////////////////////////////////////////////////////////////

#ifdef HID_LINUX_JOYSTICK
#   define HID_MAX_JOYS 32
#   define HID_MAX_POVS 4
#   define HID_MAX_AXES 24
#   define HID_MAX_BUTT 32
#   define HID_AXIS_RANGE 32767.0f
#endif

#ifdef HID_WINMM_JOYSTICK
#   define HID_MAX_JOYS 2
#   define HID_MAX_POVS 1
#   define HID_MAX_AXES 6
#   define HID_MAX_BUTT 32
#   define HID_AXIS_RANGE 65535.0f
#endif

#ifndef HID_AXIS_RANGE
#   error 'HID_AXIS_RANGE' not defined!
#endif

////////////////////////////////////////////////////////////////////////////////

namespace hid
{

/**
 * @brief This class handles joystick like input devices.
 *
 * @see https://www.kernel.org/doc/html/v4.14/input/joydev/index.html
 * @see https://docs.microsoft.com/en-us/windows/desktop/multimedia/joysticks
 */
class Joysticks : public Singleton< Joysticks >
{
    friend class Singleton< Joysticks >;

public:

    /** */
    enum Axes
    {
        AxisX = 0,                  ///<  0: X-axis deflection
        AxisY,                      ///<  1: Y-axis deflection

#       ifdef HID_LINUX_JOYSTICK
        AxisZ,                      ///<  2: Z-axis deflection
        AxisRX,                     ///<  3: Rotation about X-axis
        AxisRY,                     ///<  4: Rotation about Y-axis
        AxisRZ,                     ///<  5: Rotation about Z-axis
        Throttle,                   ///<  6: Throttle
        Rudder,                     ///<  7: Rudder
        Gas,                        ///<  8: Gas
        Wheel,                      ///<  9: Wheel
        Brake,                      ///< 10: Brake
        Hat0X,                      ///< 11: Hat 0 X-axis
        Hat0Y,                      ///< 12: Hat 0 Y-axis
        Hat1X,                      ///< 13: Hat 1 X-axis
        Hat1Y,                      ///< 14: Hat 1 Y-axis
        Hat2X,                      ///< 15: Hat 2 X-axis
        Hat2Y,                      ///< 16: Hat 2 Y-axis
        Hat3X,                      ///< 17: Hat 3 X-axis
        Hat3Y,                      ///< 18: Hat 3 Y-axis
        Pressure,                   ///< 19:
        Distance,                   ///< 20:
        TiltX,                      ///< 21:
        TiltY,                      ///< 22:
        ToolWidth                   ///< 23:
#       endif

#       ifdef HID_WINMM_JOYSTICK
        AxisZ,                      ///<  2: Z-axis deflection
        AxisR,                      ///<  3:
        AxisU,                      ///<  4:
        AxisV                       ///<  5:
#       endif
    };

    /** */
    struct Data
    {
        std::string name;               ///< device name
        
        float axis[ HID_MAX_AXES ];     ///< -1.0 ... 1.0 normalized axis position
        bool  butt[ HID_MAX_BUTT ];     ///< false: released, true: pressed
        short povs[ HID_MAX_POVS ];     ///< [deg] POVs

        short axisCount;                ///< number of device axes
        short buttCount;                ///< number of devise buttons
        short povsCount;                ///< number of devices POVs

        bool active;                    ///< specifies active device

        bool hasAxis[ HID_MAX_AXES ];   ///< specifies if joystick has an axis

#       ifdef HID_LINUX_JOYSTICK
        bool hasPOV[ HID_MAX_POVS ];    ///< specifies if joystick has a POV

        Axes axesMap[ HID_MAX_AXES + 2*HID_MAX_POVS ];  ///< axes map
#       endif

#       ifdef HID_WINMM_JOYSTICK
        bool hasPOV;                    ///< specifies if joystick has a POV
#       endif
    };

    static const std::string _axisNames[ HID_MAX_AXES ];    ///< axes names

private:

    /**
     * You should use static function instance() due to get refernce
     * to Joytsicks class instance.
     */
    Joysticks();

    /** Using this constructor is forbidden. */
    Joysticks( const Joysticks & ) {}

public:

    /** @brief Destructor. */
    virtual ~Joysticks();

    /** @brief Initializes Joystick object. */
    void init();
    
    /** @brief Updates Joystick object. */
    void update();

    /** @return number of active joysticks */
    inline short getCount() const { return _count; }

    /**
     * @param joystick index number
     * @return joystick data
     */
    Joysticks::Data getData( short joyNum ) const;

private:

    short _count;                               ///< number of active joysticks

    Joysticks::Data _data[ HID_MAX_JOYS ];      ///< joysticks data

#   ifdef HID_LINUX_JOYSTICK
    int _fd[ HID_MAX_JOYS ];                    ///<
    js_event _event;                            ///< joystick event
#   endif

#   ifdef HID_WINMM_JOYSTICK
    DWORD _buttons[ HID_MAX_BUTT ];             ///< buttons state
#   endif
};

} // end of hid namepsace

////////////////////////////////////////////////////////////////////////////////

#endif // HID_JOYSTICKS_H
