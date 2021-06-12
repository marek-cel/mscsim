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

#include <hid/hid_Joysticks.h>

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <string>

#include <sim/Log.h>

////////////////////////////////////////////////////////////////////////////////

using namespace hid;

////////////////////////////////////////////////////////////////////////////////

#ifdef HID_LINUX_JOYSTICK
const std::string Joysticks::_axisNames[] = { "X", "Y", "Z",
                                              "RX", "RY", "RZ",
                                              "Throttle",
                                              "Rudder",
                                              "Wheel",
                                              "Gas",
                                              "Brake",
                                              "Hat0X", "Hat0Y",
                                              "Hat1X", "Hat1Y",
                                              "Hat2X", "Hat2Y",
                                              "Hat3X", "Hat3Y",
                                              "Pressure",
                                              "Distance",
                                              "Tilt X", "Tilt Y",
                                              "Tool Width"
                                            };
#endif

#ifdef HID_WINMM_JOYSTICK
const std::string Joysticks::_axisNames[] = { "X", "Y", "Z",
                                              "R", "U", "V" };
#endif

////////////////////////////////////////////////////////////////////////////////

Joysticks::Joysticks() :
    _count ( 0 )
{
#   ifdef HID_WINMM_JOYSTICK
    _buttons[  0 ] = JOY_BUTTON1;
    _buttons[  1 ] = JOY_BUTTON2;
    _buttons[  2 ] = JOY_BUTTON3;
    _buttons[  3 ] = JOY_BUTTON4;
    _buttons[  4 ] = JOY_BUTTON5;
    _buttons[  5 ] = JOY_BUTTON6;
    _buttons[  6 ] = JOY_BUTTON7;
    _buttons[  7 ] = JOY_BUTTON8;
    _buttons[  8 ] = JOY_BUTTON9;
    _buttons[  9 ] = JOY_BUTTON10;
    _buttons[ 10 ] = JOY_BUTTON11;
    _buttons[ 11 ] = JOY_BUTTON12;
    _buttons[ 12 ] = JOY_BUTTON13;
    _buttons[ 13 ] = JOY_BUTTON14;
    _buttons[ 14 ] = JOY_BUTTON15;
    _buttons[ 15 ] = JOY_BUTTON16;
    _buttons[ 16 ] = JOY_BUTTON17;
    _buttons[ 17 ] = JOY_BUTTON18;
    _buttons[ 18 ] = JOY_BUTTON19;
    _buttons[ 19 ] = JOY_BUTTON20;
    _buttons[ 20 ] = JOY_BUTTON21;
    _buttons[ 21 ] = JOY_BUTTON22;
    _buttons[ 22 ] = JOY_BUTTON23;
    _buttons[ 23 ] = JOY_BUTTON24;
    _buttons[ 24 ] = JOY_BUTTON25;
    _buttons[ 25 ] = JOY_BUTTON26;
    _buttons[ 26 ] = JOY_BUTTON27;
    _buttons[ 27 ] = JOY_BUTTON28;
    _buttons[ 28 ] = JOY_BUTTON29;
    _buttons[ 29 ] = JOY_BUTTON30;
    _buttons[ 30 ] = JOY_BUTTON31;
    _buttons[ 31 ] = JOY_BUTTON32;

#       if ( HID_MAX_BUTT != 32 )
#           error 'HID_MAX_BUTT' has been changed! Check code following this line!
#       endif
#   endif

    for ( int i = 0; i < HID_MAX_JOYS; i++ )
    {
        _data[ i ].name = "";

        for ( int a = 0; a < HID_MAX_AXES; a++ ) _data[ i ].axis[ a ] = 0.0f;
        for ( int b = 0; b < HID_MAX_BUTT; b++ ) _data[ i ].butt[ b ] = false;
        for ( int p = 0; p < HID_MAX_POVS; p++ ) _data[ i ].povs[ p ] = -1;

        _data[ i ].axisCount = 0;
        _data[ i ].buttCount = 0;
        _data[ i ].povsCount = 0;

        _data[ i ].active = false;

        for ( int a = 0; a < HID_MAX_AXES; a++ ) _data[ i ].hasAxis[ a ] = false;

#       ifdef HID_LINUX_JOYSTICK
        for ( short i_pov = 0; i_pov < HID_MAX_POVS; i_pov++ )
        {
            _data[ i ].hasPOV[ i_pov ] = false;
        }

        _fd[ i ] = 0;
#       endif

#       ifdef HID_WINMM_JOYSTICK
        _data[ i ].hasPOV = false;
#       endif
    }
}

////////////////////////////////////////////////////////////////////////////////

Joysticks::~Joysticks() {}

////////////////////////////////////////////////////////////////////////////////

void Joysticks::init()
{
#   ifdef HID_LINUX_JOYSTICK
    for ( int i = 0; i < HID_MAX_JOYS; i++ )
    {
        std::stringstream ss;

        ss << "/dev/input/js" << i;

        int joyFD = open( ss.str().c_str(), O_NONBLOCK );

        if ( joyFD > 0 )
        {
            _fd[ _count ] = joyFD;

            char tempAxisCount = 0;
            char tempButtCount = 0;
            char tempName[ 512 ] = "";
            char tempAxesMap[ HID_MAX_AXES + 2*HID_MAX_POVS ] = { 0 };

            ioctl( _fd[ _count ], JSIOCGAXES     , &tempAxisCount );
            ioctl( _fd[ _count ], JSIOCGBUTTONS  , &tempButtCount );
            ioctl( _fd[ _count ], JSIOCGNAME(64) , tempName );
            ioctl( _fd[ _count ], JSIOCGAXMAP    , tempAxesMap );

            _data[ _count ].name = tempName;

            _data[ _count ].axisCount = std::min( (int)tempAxisCount, HID_MAX_AXES );
            _data[ _count ].buttCount = std::min( (int)tempButtCount, HID_MAX_BUTT );
            _data[ _count ].povsCount = 0;

            _data[ _count ].active = true;

            for ( int j = 0; j < HID_MAX_AXES + 2 * HID_MAX_POVS; j++ )
            {
                switch ( tempAxesMap[ j ] )
                {
                case ABS_X:
                    _data[ _count ].hasAxis[ AxisX ] = true;
                    _data[ _count ].axesMap[ j ] = AxisX;
                    break;

                case ABS_Y:
                    _data[ _count ].hasAxis[ AxisY ] = true;
                    _data[ _count ].axesMap[ j ] = AxisY;
                    break;

                case ABS_Z:
                    _data[ _count ].hasAxis[ AxisZ ] = true;
                    _data[ _count ].axesMap[ j ] = AxisZ;
                    break;

                case ABS_RX:
                    _data[ _count ].hasAxis[ AxisRX ] = true;
                    _data[ _count ].axesMap[ j ] = AxisRX;
                    break;

                case ABS_RY:
                    _data[ _count ].hasAxis[ AxisRY ] = true;
                    _data[ _count ].axesMap[ j ] = AxisRY;
                    break;

                case ABS_RZ:
                    _data[ _count ].hasAxis[ AxisRZ ] = true;
                    _data[ _count ].axesMap[ j ] = AxisRZ;
                    break;

                case ABS_THROTTLE:
                    _data[ _count ].hasAxis[ Throttle ] = true;
                    _data[ _count ].axesMap[ j ] = Throttle;
                    break;

                case ABS_RUDDER:
                    _data[ _count ].hasAxis[ Rudder ] = true;
                    _data[ _count ].axesMap[ j ] = Rudder;
                    break;

                case ABS_WHEEL:
                    _data[ _count ].hasAxis[ Wheel ] = true;
                    _data[ _count ].axesMap[ j ] = Wheel;
                    break;

                case ABS_GAS:
                    _data[ _count ].hasAxis[ Gas ] = true;
                    _data[ _count ].axesMap[ j ] = Gas;
                    break;

                case ABS_BRAKE:
                    _data[ _count ].hasAxis[ Brake ] = true;
                    _data[ _count ].axesMap[ j ] = Brake;
                    break;

                case ABS_HAT0X:
                    _data[ _count ].hasAxis[ Hat0X ] = true;
                    _data[ _count ].axesMap[ j ] = Hat0X;
                    break;

                case ABS_HAT0Y:
                    _data[ _count ].hasAxis[ Hat0Y ] = true;
                    _data[ _count ].axesMap[ j ] = Hat0Y;
                    break;

                case ABS_HAT1X:
                    _data[ _count ].hasAxis[ Hat1X ] = true;
                    _data[ _count ].axesMap[ j ] = Hat1X;
                    break;

                case ABS_HAT1Y:
                    _data[ _count ].hasAxis[ Hat1Y ] = true;
                    _data[ _count ].axesMap[ j ] = Hat1Y;
                    break;

                case ABS_HAT2X:
                    _data[ _count ].hasAxis[ Hat2X ] = true;
                    _data[ _count ].axesMap[ j ] = Hat2X;
                    break;

                case ABS_HAT2Y:
                    _data[ _count ].hasAxis[ Hat2Y ] = true;
                    _data[ _count ].axesMap[ j ] = Hat2Y;
                    break;

                case ABS_HAT3X:
                    _data[ _count ].hasAxis[ Hat3X ] = true;
                    _data[ _count ].axesMap[ j ] = Hat3X;
                    break;

                case ABS_HAT3Y:
                    _data[ _count ].hasAxis[ Hat3Y ] = true;
                    _data[ _count ].axesMap[ j ] = Hat3Y;
                    break;

                case ABS_PRESSURE:
                    _data[ _count ].hasAxis[ Pressure ] = true;
                    _data[ _count ].axesMap[ j ] = Pressure;
                    break;

                case ABS_DISTANCE:
                    _data[ _count ].hasAxis[ Distance ] = true;
                    _data[ _count ].axesMap[ j ] = Distance;
                    break;

                case ABS_TILT_X:
                    _data[ _count ].hasAxis[ TiltX ] = true;
                    _data[ _count ].axesMap[ j ] = TiltX;
                    break;

                case ABS_TILT_Y:
                    _data[ _count ].hasAxis[ TiltY ] = true;
                    _data[ _count ].axesMap[ j ] = TiltY;
                    break;

                case ABS_TOOL_WIDTH:
                    _data[ _count ].hasAxis[ ToolWidth ] = true;
                    _data[ _count ].axesMap[ j ] = ToolWidth;
                    break;
                }
            }

            if ( _data[ _count ].hasAxis[ Hat0X ]
              || _data[ _count ].hasAxis[ Hat0Y ] )
            {
                _data[ _count ].hasPOV[ 0 ] = true;
                _data[ _count ].povsCount++;
            }

            if ( _data[ _count ].hasAxis[ Hat1X ]
              || _data[ _count ].hasAxis[ Hat1Y ] )
            {
                _data[ _count ].hasPOV[ 1 ] = true;
                _data[ _count ].povsCount++;
            }

            if ( _data[ _count ].hasAxis[ Hat2X ]
              || _data[ _count ].hasAxis[ Hat2Y ] )
            {
                _data[ _count ].hasPOV[ 2 ] = true;
                _data[ _count ].povsCount++;
            }

            if ( _data[ _count ].hasAxis[ Hat3X ]
              || _data[ _count ].hasAxis[ Hat3Y ] )
            {
                _data[ _count ].hasPOV[ 3 ] = true;
                _data[ _count ].povsCount++;
            }

            _count++;
        }
    }
#   endif

#   ifdef HID_WINMM_JOYSTICK
    UINT joyIdTemp = JOYSTICKID1;
    JOYCAPS joyCaps;

    for ( int i = 0; i < HID_MAX_JOYS; i++ )
    {
        if ( _count > (short)joyGetNumDevs() ) break;

        if ( joyGetDevCaps( joyIdTemp, &joyCaps, sizeof(JOYCAPS) ) == JOYERR_NOERROR )
        {
            char tempNameChar[ MAXPNAMELEN ];

            for ( int j = 0; j < MAXPNAMELEN; j++ )
            {
                tempNameChar[ j ] = (char)joyCaps.szPname[ j ];
                if ( tempNameChar[ j ] == '\0' ) break;
            }

            //if ( joySetCapture( winId, joyIdTemp, 10, false ) == JOYERR_NOERROR )
            {
                _data[ _count ].name = tempNameChar;

                _data[ _count ].axisCount = joyCaps.wNumAxes;
                _data[ _count ].buttCount = joyCaps.wNumButtons;
                _data[ _count ].povsCount = 0;

                _data[ _count ].active = true;

                _data[ _count ].hasAxis[ AxisX ] = true;
                _data[ _count ].hasAxis[ AxisY ] = true;

                if ( joyCaps.wCaps & JOYCAPS_HASZ ) _data[ _count ].hasAxis[ AxisZ ] = true;
                if ( joyCaps.wCaps & JOYCAPS_HASR ) _data[ _count ].hasAxis[ AxisR ] = true;
                if ( joyCaps.wCaps & JOYCAPS_HASU ) _data[ _count ].hasAxis[ AxisU ] = true;
                if ( joyCaps.wCaps & JOYCAPS_HASV ) _data[ _count ].hasAxis[ AxisZ ] = true;

                if ( joyCaps.wCaps & JOYCAPS_HASPOV )
                {
                    _data[ _count ].hasPOV = true;
                    _data[ _count ].povsCount = 1;
                }

                _count++;
            }
        }

        if ( joyIdTemp == JOYSTICKID2 ) break;
        if ( joyIdTemp == JOYSTICKID1 ) joyIdTemp = JOYSTICKID2;
    }
#   endif
}

////////////////////////////////////////////////////////////////////////////////

void Joysticks::update()
{
#   ifdef HID_LINUX_JOYSTICK
    for( short i = 0; i < _count && i < HID_MAX_JOYS; i++ )
    {
        while( read( _fd[ i ], &_event, sizeof(js_event) ) > 0 )
        {
            // buttons
            if ( _event.type == JS_EVENT_BUTTON )
            {
                if ( _event.number < HID_MAX_BUTT )
                {
                    _data[ i ].butt[ _event.number ] = ( _event.value ) ? 1 : 0;
                }
            }

            // axes
            if ( _event.type == JS_EVENT_AXIS )
            {
                float value = _event.value / (double)HID_AXIS_RANGE;

                switch ( _data[ i ].axesMap[ _event.number ] )
                {
                    case AxisX:  _data[ i ].axis[ AxisX  ] = value; break;
                    case AxisY:  _data[ i ].axis[ AxisY  ] = value; break;
                    case AxisZ:  _data[ i ].axis[ AxisZ  ] = value; break;
                    case AxisRX: _data[ i ].axis[ AxisRX ] = value; break;
                    case AxisRY: _data[ i ].axis[ AxisRY ] = value; break;
                    case AxisRZ: _data[ i ].axis[ AxisRZ ] = value; break;

                    case Throttle: _data[ i ].axis[ Throttle ] = value; break;
                    case Rudder:   _data[ i ].axis[ Rudder   ] = value; break;
                    case Gas:      _data[ i ].axis[ Gas      ] = value; break;
                    case Wheel:    _data[ i ].axis[ Wheel    ] = value; break;
                    case Brake:    _data[ i ].axis[ Brake    ] = value; break;

                    case Hat0X: _data[ i ].axis[ Hat0X ] = value; break;
                    case Hat0Y: _data[ i ].axis[ Hat0Y ] = value; break;
                    case Hat1X: _data[ i ].axis[ Hat1X ] = value; break;
                    case Hat1Y: _data[ i ].axis[ Hat1Y ] = value; break;
                    case Hat2X: _data[ i ].axis[ Hat2X ] = value; break;
                    case Hat2Y: _data[ i ].axis[ Hat2Y ] = value; break;
                    case Hat3X: _data[ i ].axis[ Hat3X ] = value; break;
                    case Hat3Y: _data[ i ].axis[ Hat3Y ] = value; break;

                    case Pressure:  _data[ i ].axis[ Pressure  ] = value; break;
                    case Distance:  _data[ i ].axis[ Distance  ] = value; break;
                    case TiltX:     _data[ i ].axis[ TiltX     ] = value; break;
                    case TiltY:     _data[ i ].axis[ TiltY     ] = value; break;
                    case ToolWidth: _data[ i ].axis[ ToolWidth ] = value; break;
                }

                for ( short i_pov = 0; i_pov < HID_MAX_POVS; i_pov++ )
                {
                    _data[ i ].povs[ i_pov ] = -1;

                    if ( _data[ i ].hasPOV[ i_pov ] )
                    {
                        if ( 0 )
                        {
                            // multidirectional POV
                            short angle_deg = 360 * _data[ i ].axis[ Hat0X + i_pov ];

                            while ( angle_deg <   0 ) angle_deg += 360;
                            while ( angle_deg > 360 ) angle_deg -= 360;

                            _data[ i ].povs[ i_pov ] = angle_deg;
                        }
                        else
                        {
                            // default model
                            if ( _data[ i ].axis[ Hat0X + i_pov ] != 0.0f
                              || _data[ i ].axis[ Hat0Y + i_pov ] != 0.0f )
                            {
                                float angle_rad = atan2( _data[ i ].axis[ Hat0X + i_pov ],
                                                        -_data[ i ].axis[ Hat0Y + i_pov ] );

                                short angle_deg = 180 * angle_rad / M_PI;

                                while ( angle_deg <   0 ) angle_deg += 360;
                                while ( angle_deg > 360 ) angle_deg -= 360;

                                _data[ i ].povs[ i_pov ] = angle_deg;
                            }
                        }
                    }
                }
            }
        }
    }
#   endif

#   ifdef HID_WINMM_JOYSTICK
    JOYINFOEX joyInfoEx;

    joyInfoEx.dwSize = sizeof(JOYINFOEX);
    joyInfoEx.dwFlags = JOY_RETURNALL;

    UINT joyIdTemp = JOYSTICKID1;

    for ( int i = 0; i < _count; i++ )
    {
        if ( joyGetPosEx( joyIdTemp, &joyInfoEx ) == JOYERR_NOERROR )
        {
            for ( int j = 0; j < _data[ i ].buttCount; j++ )
            {
                _data[ i ].butt[ j ] = ( _buttons[ j ] & joyInfoEx.dwButtons ) ? 1 : 0;
            }
        }

        _data[ i ].axis[ AxisX ] = 2.0f * ( (float)joyInfoEx.dwXpos / HID_AXIS_RANGE ) - 1.0f;
        _data[ i ].axis[ AxisY ] = 2.0f * ( (float)joyInfoEx.dwYpos / HID_AXIS_RANGE ) - 1.0f;

        if ( _data[ i ].hasAxis[ AxisZ ] ) _data[ i ].axis[ AxisZ ] = 2.0f * ( (float)joyInfoEx.dwZpos / HID_AXIS_RANGE ) - 1.0f;
        if ( _data[ i ].hasAxis[ AxisR ] ) _data[ i ].axis[ AxisR ] = 2.0f * ( (float)joyInfoEx.dwRpos / HID_AXIS_RANGE ) - 1.0f;
        if ( _data[ i ].hasAxis[ AxisU ] ) _data[ i ].axis[ AxisU ] = 2.0f * ( (float)joyInfoEx.dwUpos / HID_AXIS_RANGE ) - 1.0f;
        if ( _data[ i ].hasAxis[ AxisV ] ) _data[ i ].axis[ AxisV ] = 2.0f * ( (float)joyInfoEx.dwVpos / HID_AXIS_RANGE ) - 1.0f;

        _data[ i ].povs[ 0 ] = -1;

        if ( _data[ i ].hasPOV && joyInfoEx.dwPOV != JOY_POVCENTERED )
        {
            _data[ i ].povs[ 0 ] = (short)( joyInfoEx.dwPOV / 100 );
        }

        if ( joyIdTemp == JOYSTICKID2 ) break;
        if ( joyIdTemp == JOYSTICKID1 ) joyIdTemp = JOYSTICKID2;
    }
#   endif
}

////////////////////////////////////////////////////////////////////////////////

Joysticks::Data Joysticks::getData( short joyNum ) const
{
    if ( joyNum < 0 && joyNum >= HID_MAX_JOYS )
    {
        Log::e() << "Wrong joystick index number." << std::endl;

#       ifdef SIM_EXITONERROR
        exit( EXIT_FAILURE );
#       endif
    }

    return _data[ joyNum ];
}
