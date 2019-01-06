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

#include <hid/hid_Joysticks.h>

#include <math.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

////////////////////////////////////////////////////////////////////////////////

using namespace hid;

////////////////////////////////////////////////////////////////////////////////

Joysticks* Joysticks::m_instance = 0;

////////////////////////////////////////////////////////////////////////////////

#ifdef HID_LINUX_JOYSTICK
const std::string Joysticks::m_axisNames[] = { "X", "Y", "Z",
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
const std::string Joysticks::m_axisNames[] = { "X", "Y", "Z",
                                               "R", "U", "V" };
#endif

////////////////////////////////////////////////////////////////////////////////

Joysticks::Joysticks() :
    m_count ( 0 )
{
#   ifdef HID_WINMM_JOYSTICK
    m_buttons[  0 ] = JOY_BUTTON1;
    m_buttons[  1 ] = JOY_BUTTON2;
    m_buttons[  2 ] = JOY_BUTTON3;
    m_buttons[  3 ] = JOY_BUTTON4;
    m_buttons[  4 ] = JOY_BUTTON5;
    m_buttons[  5 ] = JOY_BUTTON6;
    m_buttons[  6 ] = JOY_BUTTON7;
    m_buttons[  7 ] = JOY_BUTTON8;
    m_buttons[  8 ] = JOY_BUTTON9;
    m_buttons[  9 ] = JOY_BUTTON10;
    m_buttons[ 10 ] = JOY_BUTTON11;
    m_buttons[ 11 ] = JOY_BUTTON12;
    m_buttons[ 12 ] = JOY_BUTTON13;
    m_buttons[ 13 ] = JOY_BUTTON14;
    m_buttons[ 14 ] = JOY_BUTTON15;
    m_buttons[ 15 ] = JOY_BUTTON16;
    m_buttons[ 16 ] = JOY_BUTTON17;
    m_buttons[ 17 ] = JOY_BUTTON18;
    m_buttons[ 18 ] = JOY_BUTTON19;
    m_buttons[ 19 ] = JOY_BUTTON20;
    m_buttons[ 20 ] = JOY_BUTTON21;
    m_buttons[ 21 ] = JOY_BUTTON22;
    m_buttons[ 22 ] = JOY_BUTTON23;
    m_buttons[ 23 ] = JOY_BUTTON24;
    m_buttons[ 24 ] = JOY_BUTTON25;
    m_buttons[ 25 ] = JOY_BUTTON26;
    m_buttons[ 26 ] = JOY_BUTTON27;
    m_buttons[ 27 ] = JOY_BUTTON28;
    m_buttons[ 28 ] = JOY_BUTTON29;
    m_buttons[ 29 ] = JOY_BUTTON30;
    m_buttons[ 30 ] = JOY_BUTTON31;
    m_buttons[ 31 ] = JOY_BUTTON32;

#       if ( HID_MAX_BUTT != 32 )
#           error 'HID_MAX_BUTT' has been changed! Check code following this line!
#       endif
#   endif

    for ( int i = 0; i < HID_MAX_JOYS; i++ )
    {
        m_data[ i ].name = "";

        for ( int a = 0; a < HID_MAX_AXES; a++ ) m_data[ i ].axis[ a ] = 0.0f;
        for ( int b = 0; b < HID_MAX_BUTT; b++ ) m_data[ i ].butt[ b ] = false;
        for ( int p = 0; p < HID_MAX_POVS; p++ ) m_data[ i ].povs[ p ] = -1;

        m_data[ i ].axisCount = 0;
        m_data[ i ].buttCount = 0;
        m_data[ i ].povsCount = 0;

        m_data[ i ].active = false;

        for ( int a = 0; a < HID_MAX_AXES; a++ ) m_data[ i ].hasAxis[ a ] = false;

#       ifdef HID_LINUX_JOYSTICK
        for ( short i_pov = 0; i_pov < HID_MAX_POVS; i_pov++ )
        {
            m_data[ i ].hasPOV[ i_pov ] = false;
        }

        m_fd[ i ] = 0;
#       endif

#       ifdef HID_WINMM_JOYSTICK
        m_data[ i ].hasPOV = false;
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
        char tempDev[ 255 ];

        sprintf( tempDev, "/dev/input/js%d", i );

        int joyFD = open( tempDev, O_NONBLOCK );

        if ( joyFD > 0 )
        {
            m_fd[ m_count ] = joyFD;

            char tempAxisCount = 0;
            char tempButtCount = 0;
            char tempName[ 512 ];
            char tempAxesMap[ HID_MAX_AXES + 2*HID_MAX_POVS ];

            ioctl( m_fd[ m_count ], JSIOCGAXES     , &tempAxisCount );
            ioctl( m_fd[ m_count ], JSIOCGBUTTONS  , &tempButtCount );
            ioctl( m_fd[ m_count ], JSIOCGNAME(64) , tempName );
            ioctl( m_fd[ m_count ], JSIOCGAXMAP    , tempAxesMap );

            m_data[ m_count ].name = tempName;

            m_data[ m_count ].axisCount = std::min( (int)tempAxisCount, HID_MAX_AXES );
            m_data[ m_count ].buttCount = std::min( (int)tempButtCount, HID_MAX_BUTT );
            m_data[ m_count ].povsCount = 0;

            m_data[ m_count ].active = true;

            for ( int j = 0; j < HID_MAX_AXES + 2 * HID_MAX_POVS; j++ )
            {
                switch ( tempAxesMap[ j ] )
                {
                case ABS_X:
                    m_data[ m_count ].hasAxis[ AxisX ] = true;
                    m_data[ m_count ].axesMap[ j ] = AxisX;
                    break;

                case ABS_Y:
                    m_data[ m_count ].hasAxis[ AxisY ] = true;
                    m_data[ m_count ].axesMap[ j ] = AxisY;
                    break;

                case ABS_Z:
                    m_data[ m_count ].hasAxis[ AxisZ ] = true;
                    m_data[ m_count ].axesMap[ j ] = AxisZ;
                    break;

                case ABS_RX:
                    m_data[ m_count ].hasAxis[ AxisRX ] = true;
                    m_data[ m_count ].axesMap[ j ] = AxisRX;
                    break;

                case ABS_RY:
                    m_data[ m_count ].hasAxis[ AxisRY ] = true;
                    m_data[ m_count ].axesMap[ j ] = AxisRY;
                    break;

                case ABS_RZ:
                    m_data[ m_count ].hasAxis[ AxisRZ ] = true;
                    m_data[ m_count ].axesMap[ j ] = AxisRZ;
                    break;

                case ABS_THROTTLE:
                    m_data[ m_count ].hasAxis[ Throttle ] = true;
                    m_data[ m_count ].axesMap[ j ] = Throttle;
                    break;

                case ABS_RUDDER:
                    m_data[ m_count ].hasAxis[ Rudder ] = true;
                    m_data[ m_count ].axesMap[ j ] = Rudder;
                    break;

                case ABS_WHEEL:
                    m_data[ m_count ].hasAxis[ Wheel ] = true;
                    m_data[ m_count ].axesMap[ j ] = Wheel;
                    break;

                case ABS_GAS:
                    m_data[ m_count ].hasAxis[ Gas ] = true;
                    m_data[ m_count ].axesMap[ j ] = Gas;
                    break;

                case ABS_BRAKE:
                    m_data[ m_count ].hasAxis[ Brake ] = true;
                    m_data[ m_count ].axesMap[ j ] = Brake;
                    break;

                case ABS_HAT0X:
                    m_data[ m_count ].hasAxis[ Hat0X ] = true;
                    m_data[ m_count ].axesMap[ j ] = Hat0X;
                    break;

                case ABS_HAT0Y:
                    m_data[ m_count ].hasAxis[ Hat0Y ] = true;
                    m_data[ m_count ].axesMap[ j ] = Hat0Y;
                    break;

                case ABS_HAT1X:
                    m_data[ m_count ].hasAxis[ Hat1X ] = true;
                    m_data[ m_count ].axesMap[ j ] = Hat1X;
                    break;

                case ABS_HAT1Y:
                    m_data[ m_count ].hasAxis[ Hat1Y ] = true;
                    m_data[ m_count ].axesMap[ j ] = Hat1Y;
                    break;

                case ABS_HAT2X:
                    m_data[ m_count ].hasAxis[ Hat2X ] = true;
                    m_data[ m_count ].axesMap[ j ] = Hat2X;
                    break;

                case ABS_HAT2Y:
                    m_data[ m_count ].hasAxis[ Hat2Y ] = true;
                    m_data[ m_count ].axesMap[ j ] = Hat2Y;
                    break;

                case ABS_HAT3X:
                    m_data[ m_count ].hasAxis[ Hat3X ] = true;
                    m_data[ m_count ].axesMap[ j ] = Hat3X;
                    break;

                case ABS_HAT3Y:
                    m_data[ m_count ].hasAxis[ Hat3Y ] = true;
                    m_data[ m_count ].axesMap[ j ] = Hat3Y;
                    break;

                case ABS_PRESSURE:
                    m_data[ m_count ].hasAxis[ Pressure ] = true;
                    m_data[ m_count ].axesMap[ j ] = Pressure;
                    break;

                case ABS_DISTANCE:
                    m_data[ m_count ].hasAxis[ Distance ] = true;
                    m_data[ m_count ].axesMap[ j ] = Distance;
                    break;

                case ABS_TILT_X:
                    m_data[ m_count ].hasAxis[ TiltX ] = true;
                    m_data[ m_count ].axesMap[ j ] = TiltX;
                    break;

                case ABS_TILT_Y:
                    m_data[ m_count ].hasAxis[ TiltY ] = true;
                    m_data[ m_count ].axesMap[ j ] = TiltY;
                    break;

                case ABS_TOOL_WIDTH:
                    m_data[ m_count ].hasAxis[ ToolWidth ] = true;
                    m_data[ m_count ].axesMap[ j ] = ToolWidth;
                    break;
                }
            }

            if ( m_data[ m_count ].hasAxis[ Hat0X ]
              || m_data[ m_count ].hasAxis[ Hat0Y ] )
            {
                m_data[ m_count ].hasPOV[ 0 ] = true;
                m_data[ m_count ].povsCount++;
            }

            if ( m_data[ m_count ].hasAxis[ Hat1X ]
              || m_data[ m_count ].hasAxis[ Hat1Y ] )
            {
                m_data[ m_count ].hasPOV[ 1 ] = true;
                m_data[ m_count ].povsCount++;
            }

            if ( m_data[ m_count ].hasAxis[ Hat2X ]
              || m_data[ m_count ].hasAxis[ Hat2Y ] )
            {
                m_data[ m_count ].hasPOV[ 2 ] = true;
                m_data[ m_count ].povsCount++;
            }

            if ( m_data[ m_count ].hasAxis[ Hat3X ]
              || m_data[ m_count ].hasAxis[ Hat3Y ] )
            {
                m_data[ m_count ].hasPOV[ 3 ] = true;
                m_data[ m_count ].povsCount++;
            }

            m_count++;
        }
    }
#   endif

#   ifdef HID_WINMM_JOYSTICK
    UINT joyIdTemp = JOYSTICKID1;
    JOYCAPS joyCaps;

    for ( int i = 0; i < HID_MAX_JOYS; i++ )
    {
        if ( m_count > (short)joyGetNumDevs() ) break;

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
                m_data[ m_count ].name = tempNameChar;

                m_data[ m_count ].axisCount = joyCaps.wNumAxes;
                m_data[ m_count ].buttCount = joyCaps.wNumButtons;
                m_data[ m_count ].povsCount = 0;

                m_data[ m_count ].active = true;

                m_data[ m_count ].hasAxis[ AxisX ] = true;
                m_data[ m_count ].hasAxis[ AxisY ] = true;

                if ( joyCaps.wCaps & JOYCAPS_HASZ ) m_data[ m_count ].hasAxis[ AxisZ ] = true;
                if ( joyCaps.wCaps & JOYCAPS_HASR ) m_data[ m_count ].hasAxis[ AxisR ] = true;
                if ( joyCaps.wCaps & JOYCAPS_HASU ) m_data[ m_count ].hasAxis[ AxisU ] = true;
                if ( joyCaps.wCaps & JOYCAPS_HASV ) m_data[ m_count ].hasAxis[ AxisZ ] = true;

                if ( joyCaps.wCaps & JOYCAPS_HASPOV )
                {
                    m_data[ m_count ].hasPOV = true;
                    m_data[ m_count ].povsCount = 1;
                }

                m_count++;
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
    for( short i = 0; i < m_count && i < HID_MAX_JOYS; i++ )
    {
        while( read( m_fd[ i ], &m_event, sizeof(js_event) ) > 0 )
        {
            // buttons
            if ( m_event.type == JS_EVENT_BUTTON )
            {
                if ( m_event.number < HID_MAX_BUTT )
                {
                    m_data[ i ].butt[ m_event.number ] = ( m_event.value ) ? 1 : 0;
                }
            }

            // axes
            if ( m_event.type == JS_EVENT_AXIS )
            {
                float value = m_event.value / (double)HID_AXIS_RANGE;

                switch ( m_data[ i ].axesMap[ m_event.number ] )
                {
                    case AxisX:  m_data[ i ].axis[ AxisX  ] = value; break;
                    case AxisY:  m_data[ i ].axis[ AxisY  ] = value; break;
                    case AxisZ:  m_data[ i ].axis[ AxisZ  ] = value; break;
                    case AxisRX: m_data[ i ].axis[ AxisRX ] = value; break;
                    case AxisRY: m_data[ i ].axis[ AxisRY ] = value; break;
                    case AxisRZ: m_data[ i ].axis[ AxisRZ ] = value; break;

                    case Throttle: m_data[ i ].axis[ Throttle ] = value; break;
                    case Rudder:   m_data[ i ].axis[ Rudder   ] = value; break;
                    case Gas:      m_data[ i ].axis[ Gas      ] = value; break;
                    case Wheel:    m_data[ i ].axis[ Wheel    ] = value; break;
                    case Brake:    m_data[ i ].axis[ Brake    ] = value; break;

                    case Hat0X: m_data[ i ].axis[ Hat0X ] = value; break;
                    case Hat0Y: m_data[ i ].axis[ Hat0Y ] = value; break;
                    case Hat1X: m_data[ i ].axis[ Hat1X ] = value; break;
                    case Hat1Y: m_data[ i ].axis[ Hat1Y ] = value; break;
                    case Hat2X: m_data[ i ].axis[ Hat2X ] = value; break;
                    case Hat2Y: m_data[ i ].axis[ Hat2Y ] = value; break;
                    case Hat3X: m_data[ i ].axis[ Hat3X ] = value; break;
                    case Hat3Y: m_data[ i ].axis[ Hat3Y ] = value; break;

                    case Pressure:  m_data[ i ].axis[ Pressure  ] = value; break;
                    case Distance:  m_data[ i ].axis[ Distance  ] = value; break;
                    case TiltX:     m_data[ i ].axis[ TiltX     ] = value; break;
                    case TiltY:     m_data[ i ].axis[ TiltY     ] = value; break;
                    case ToolWidth: m_data[ i ].axis[ ToolWidth ] = value; break;
                }

                for ( short i_pov = 0; i_pov < HID_MAX_POVS; i_pov++ )
                {
                    m_data[ i ].povs[ i_pov ] = -1;

                    if ( m_data[ i ].hasPOV[ i_pov ]
                         && (
                                m_data[ i ].axis[ Hat0X + i_pov ] != 0.0f
                             || m_data[ i ].axis[ Hat0Y + i_pov ] != 0.0f
                            )
                       )
                    {
                        float angle_rad = atan2( m_data[ i ].axis[ Hat0X + i_pov ],
                                                -m_data[ i ].axis[ Hat0Y + i_pov ] );

                        short angle_deg = 180 * angle_rad / M_PI;

                        while ( angle_deg <   0 ) angle_deg += 360;
                        while ( angle_deg > 360 ) angle_deg -= 360;

                        m_data[ i ].povs[ i_pov ] = angle_deg;
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

    for ( int i = 0; i < m_count; i++ )
    {
        if ( joyGetPosEx( joyIdTemp, &joyInfoEx ) == JOYERR_NOERROR )
        {
            for ( int j = 0; j < m_data[ i ].buttCount; j++ )
            {
                m_data[ i ].butt[ j ] = ( m_buttons[ j ] & joyInfoEx.dwButtons ) ? 1 : 0;
            }
        }

        m_data[ i ].axis[ AxisX ] = 2.0f * ( (float)joyInfoEx.dwXpos / HID_AXIS_RANGE ) - 1.0f;
        m_data[ i ].axis[ AxisY ] = 2.0f * ( (float)joyInfoEx.dwYpos / HID_AXIS_RANGE ) - 1.0f;

        if ( m_data[ i ].hasAxis[ AxisZ ] ) m_data[ i ].axis[ AxisZ ] = 2.0f * ( (float)joyInfoEx.dwZpos / HID_AXIS_RANGE ) - 1.0f;
        if ( m_data[ i ].hasAxis[ AxisR ] ) m_data[ i ].axis[ AxisR ] = 2.0f * ( (float)joyInfoEx.dwRpos / HID_AXIS_RANGE ) - 1.0f;
        if ( m_data[ i ].hasAxis[ AxisU ] ) m_data[ i ].axis[ AxisU ] = 2.0f * ( (float)joyInfoEx.dwUpos / HID_AXIS_RANGE ) - 1.0f;
        if ( m_data[ i ].hasAxis[ AxisV ] ) m_data[ i ].axis[ AxisV ] = 2.0f * ( (float)joyInfoEx.dwVpos / HID_AXIS_RANGE ) - 1.0f;

        m_data[ i ].povs[ 0 ] = -1;

        if ( m_data[ i ].hasPOV && joyInfoEx.dwPOV != JOY_POVCENTERED )
        {
            m_data[ i ].povs[ 0 ] = (short)( joyInfoEx.dwPOV / 100 );
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
        std::cerr << "ERROR! Wrong joystick index number." << std::endl;

#       ifdef SIM_EXITONERROR
        exit( EXIT_FAILURE );
#       endif
    }

    return m_data[ joyNum ];
}
