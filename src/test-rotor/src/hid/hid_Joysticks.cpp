/****************************************************************************//*
 * Copyright (C) 2020 Marek M. Cel
 *
 * Creative Commons Legal Code
 *
 * CC0 1.0 Universal
 *
 *     CREATIVE COMMONS CORPORATION IS NOT A LAW FIRM AND DOES NOT PROVIDE
 *     LEGAL SERVICES. DISTRIBUTION OF THIS DOCUMENT DOES NOT CREATE AN
 *     ATTORNEY-CLIENT RELATIONSHIP. CREATIVE COMMONS PROVIDES THIS
 *     INFORMATION ON AN "AS-IS" BASIS. CREATIVE COMMONS MAKES NO WARRANTIES
 *     REGARDING THE USE OF THIS DOCUMENT OR THE INFORMATION OR WORKS
 *     PROVIDED HEREUNDER, AND DISCLAIMS LIABILITY FOR DAMAGES RESULTING FROM
 *     THE USE OF THIS DOCUMENT OR THE INFORMATION OR WORKS PROVIDED
 *     HEREUNDER.
 *
 * Statement of Purpose
 *
 * The laws of most jurisdictions throughout the world automatically confer
 * exclusive Copyright and Related Rights (defined below) upon the creator
 * and subsequent owner(s) (each and all, an "owner") of an original work of
 * authorship and/or a database (each, a "Work").
 *
 * Certain owners wish to permanently relinquish those rights to a Work for
 * the purpose of contributing to a commons of creative, cultural and
 * scientific works ("Commons") that the public can reliably and without fear
 * of later claims of infringement build upon, modify, incorporate in other
 * works, reuse and redistribute as freely as possible in any form whatsoever
 * and for any purposes, including without limitation commercial purposes.
 * These owners may contribute to the Commons to promote the ideal of a free
 * culture and the further production of creative, cultural and scientific
 * works, or to gain reputation or greater distribution for their Work in
 * part through the use and efforts of others.
 *
 * For these and/or other purposes and motivations, and without any
 * expectation of additional consideration or compensation, the person
 * associating CC0 with a Work (the "Affirmer"), to the extent that he or she
 * is an owner of Copyright and Related Rights in the Work, voluntarily
 * elects to apply CC0 to the Work and publicly distribute the Work under its
 * terms, with knowledge of his or her Copyright and Related Rights in the
 * Work and the meaning and intended legal effect of CC0 on those rights.
 *
 * 1. Copyright and Related Rights. A Work made available under CC0 may be
 * protected by copyright and related or neighboring rights ("Copyright and
 * Related Rights"). Copyright and Related Rights include, but are not
 * limited to, the following:
 *
 *   i. the right to reproduce, adapt, distribute, perform, display,
 *      communicate, and translate a Work;
 *  ii. moral rights retained by the original author(s) and/or performer(s);
 * iii. publicity and privacy rights pertaining to a person's image or
 *      likeness depicted in a Work;
 *  iv. rights protecting against unfair competition in regards to a Work,
 *      subject to the limitations in paragraph 4(a), below;
 *   v. rights protecting the extraction, dissemination, use and reuse of data
 *      in a Work;
 *  vi. database rights (such as those arising under Directive 96/9/EC of the
 *      European Parliament and of the Council of 11 March 1996 on the legal
 *      protection of databases, and under any national implementation
 *      thereof, including any amended or successor version of such
 *      directive); and
 * vii. other similar, equivalent or corresponding rights throughout the
 *      world based on applicable law or treaty, and any national
 *      implementations thereof.
 *
 * 2. Waiver. To the greatest extent permitted by, but not in contravention
 * of, applicable law, Affirmer hereby overtly, fully, permanently,
 * irrevocably and unconditionally waives, abandons, and surrenders all of
 * Affirmer's Copyright and Related Rights and associated claims and causes
 * of action, whether now known or unknown (including existing as well as
 * future claims and causes of action), in the Work (i) in all territories
 * worldwide, (ii) for the maximum duration provided by applicable law or
 * treaty (including future time extensions), (iii) in any current or future
 * medium and for any number of copies, and (iv) for any purpose whatsoever,
 * including without limitation commercial, advertising or promotional
 * purposes (the "Waiver"). Affirmer makes the Waiver for the benefit of each
 * member of the public at large and to the detriment of Affirmer's heirs and
 * successors, fully intending that such Waiver shall not be subject to
 * revocation, rescission, cancellation, termination, or any other legal or
 * equitable action to disrupt the quiet enjoyment of the Work by the public
 * as contemplated by Affirmer's express Statement of Purpose.
 *
 * 3. Public License Fallback. Should any part of the Waiver for any reason
 * be judged legally invalid or ineffective under applicable law, then the
 * Waiver shall be preserved to the maximum extent permitted taking into
 * account Affirmer's express Statement of Purpose. In addition, to the
 * extent the Waiver is so judged Affirmer hereby grants to each affected
 * person a royalty-free, non transferable, non sublicensable, non exclusive,
 * irrevocable and unconditional license to exercise Affirmer's Copyright and
 * Related Rights in the Work (i) in all territories worldwide, (ii) for the
 * maximum duration provided by applicable law or treaty (including future
 * time extensions), (iii) in any current or future medium and for any number
 * of copies, and (iv) for any purpose whatsoever, including without
 * limitation commercial, advertising or promotional purposes (the
 * "License"). The License shall be deemed effective as of the date CC0 was
 * applied by Affirmer to the Work. Should any part of the License for any
 * reason be judged legally invalid or ineffective under applicable law, such
 * partial invalidity or ineffectiveness shall not invalidate the remainder
 * of the License, and in such case Affirmer hereby affirms that he or she
 * will not (i) exercise any of his or her remaining Copyright and Related
 * Rights in the Work or (ii) assert any associated claims and causes of
 * action with respect to the Work, in either case contrary to Affirmer's
 * express Statement of Purpose.
 *
 * 4. Limitations and Disclaimers.
 *
 *  a. No trademark or patent rights held by Affirmer are waived, abandoned,
 *     surrendered, licensed or otherwise affected by this document.
 *  b. Affirmer offers the Work as-is and makes no representations or
 *     warranties of any kind concerning the Work, express, implied,
 *     statutory or otherwise, including without limitation warranties of
 *     title, merchantability, fitness for a particular purpose, non
 *     infringement, or the absence of latent or other defects, accuracy, or
 *     the present or absence of errors, whether or not discoverable, all to
 *     the greatest extent permissible under applicable law.
 *  c. Affirmer disclaims responsibility for clearing rights of other persons
 *     that may apply to the Work or any use thereof, including without
 *     limitation any person's Copyright and Related Rights in the Work.
 *     Further, Affirmer disclaims responsibility for obtaining any necessary
 *     consents, permissions or other rights required for any use of the
 *     Work.
 *  d. Affirmer understands and acknowledges that Creative Commons is not a
 *     party to this document and has no duty or obligation with respect to
 *     this CC0 or use of the Work.
 *
 ******************************************************************************/

#include <hid/hid_Joysticks.h>

#include <cmath>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>

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
    _joysCount ( 0 )
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
        _joyData[ i ].name = "";

        for ( int a = 0; a < HID_MAX_AXES; a++ ) _joyData[ i ].axis[ a ] = 0.0f;
        for ( int b = 0; b < HID_MAX_BUTT; b++ ) _joyData[ i ].butt[ b ] = false;
        for ( int p = 0; p < HID_MAX_POVS; p++ ) _joyData[ i ].povs[ p ] = -1;

        _joyData[ i ].axisCount = 0;
        _joyData[ i ].buttCount = 0;
        _joyData[ i ].povsCount = 0;

        _joyData[ i ].active   = false;
        _joyData[ i ].feedback = false;

        for ( int a = 0; a < HID_MAX_AXES; a++ ) _joyData[ i ].hasAxis[ a ] = false;

#       ifdef HID_LINUX_JOYSTICK
        for ( short i_pov = 0; i_pov < HID_MAX_POVS; i_pov++ )
        {
            _joyData[ i ].hasPOV[ i_pov ] = false;
        }

        _joysFD[ i ] = 0;
#       endif

#       ifdef HID_WINMM_JOYSTICK
        _joyData[ i ].hasPOV = false;
#       endif
    }
}

////////////////////////////////////////////////////////////////////////////////

Joysticks::~Joysticks() {}

////////////////////////////////////////////////////////////////////////////////

#ifdef HID_LINUX_JOYSTICK
void Joysticks::init()
{
    for ( int i = 0; i < HID_MAX_JOYS; i++ )
    {
        char tempDev[ 255 ];

        sprintf( tempDev, "/dev/input/js%d", i );

        int joyFD = open( tempDev, O_NONBLOCK );

        if ( joyFD > 0 )
        {
            _joysFD[ _joysCount ] = joyFD;

            char tempAxisCount = 0;
            char tempButtCount = 0;
            char tempName[ 512 ];
            char tempAxesMap[ HID_MAX_AXES + 2 * HID_MAX_POVS ];

            ioctl( _joysFD[ _joysCount ], JSIOCGAXES     , &tempAxisCount );
            ioctl( _joysFD[ _joysCount ], JSIOCGBUTTONS  , &tempButtCount );
            ioctl( _joysFD[ _joysCount ], JSIOCGNAME(64) , tempName );
            ioctl( _joysFD[ _joysCount ], JSIOCGAXMAP    , tempAxesMap );

            _joyData[ _joysCount ].name = tempName;

            _joyData[ _joysCount ].axisCount = std::min( (int)tempAxisCount, HID_MAX_AXES );
            _joyData[ _joysCount ].buttCount = std::min( (int)tempButtCount, HID_MAX_BUTT );
            _joyData[ _joysCount ].povsCount = 0;

            _joyData[ _joysCount ].active   = true;
            _joyData[ _joysCount ].feedback = false;

            for ( int j = 0; j < HID_MAX_AXES + 2 * HID_MAX_POVS; j++ )
            {
                switch ( tempAxesMap[ j ] )
                {
                case ABS_X:
                    _joyData[ _joysCount ].hasAxis[ AxisX ] = true;
                    _joyData[ _joysCount ].axesMap[ j ] = AxisX;
                    break;

                case ABS_Y:
                    _joyData[ _joysCount ].hasAxis[ AxisY ] = true;
                    _joyData[ _joysCount ].axesMap[ j ] = AxisY;
                    break;

                case ABS_Z:
                    _joyData[ _joysCount ].hasAxis[ AxisZ ] = true;
                    _joyData[ _joysCount ].axesMap[ j ] = AxisZ;
                    break;

                case ABS_RX:
                    _joyData[ _joysCount ].hasAxis[ AxisRX ] = true;
                    _joyData[ _joysCount ].axesMap[ j ] = AxisRX;
                    break;

                case ABS_RY:
                    _joyData[ _joysCount ].hasAxis[ AxisRY ] = true;
                    _joyData[ _joysCount ].axesMap[ j ] = AxisRY;
                    break;

                case ABS_RZ:
                    _joyData[ _joysCount ].hasAxis[ AxisRZ ] = true;
                    _joyData[ _joysCount ].axesMap[ j ] = AxisRZ;
                    break;

                case ABS_THROTTLE:
                    _joyData[ _joysCount ].hasAxis[ Throttle ] = true;
                    _joyData[ _joysCount ].axesMap[ j ] = Throttle;
                    break;

                case ABS_RUDDER:
                    _joyData[ _joysCount ].hasAxis[ Rudder ] = true;
                    _joyData[ _joysCount ].axesMap[ j ] = Rudder;
                    break;

                case ABS_WHEEL:
                    _joyData[ _joysCount ].hasAxis[ Wheel ] = true;
                    _joyData[ _joysCount ].axesMap[ j ] = Wheel;
                    break;

                case ABS_GAS:
                    _joyData[ _joysCount ].hasAxis[ Gas ] = true;
                    _joyData[ _joysCount ].axesMap[ j ] = Gas;
                    break;

                case ABS_BRAKE:
                    _joyData[ _joysCount ].hasAxis[ Brake ] = true;
                    _joyData[ _joysCount ].axesMap[ j ] = Brake;
                    break;

                case ABS_HAT0X:
                    _joyData[ _joysCount ].hasAxis[ Hat0X ] = true;
                    _joyData[ _joysCount ].axesMap[ j ] = Hat0X;
                    break;

                case ABS_HAT0Y:
                    _joyData[ _joysCount ].hasAxis[ Hat0Y ] = true;
                    _joyData[ _joysCount ].axesMap[ j ] = Hat0Y;
                    break;

                case ABS_HAT1X:
                    _joyData[ _joysCount ].hasAxis[ Hat1X ] = true;
                    _joyData[ _joysCount ].axesMap[ j ] = Hat1X;
                    break;

                case ABS_HAT1Y:
                    _joyData[ _joysCount ].hasAxis[ Hat1Y ] = true;
                    _joyData[ _joysCount ].axesMap[ j ] = Hat1Y;
                    break;

                case ABS_HAT2X:
                    _joyData[ _joysCount ].hasAxis[ Hat2X ] = true;
                    _joyData[ _joysCount ].axesMap[ j ] = Hat2X;
                    break;

                case ABS_HAT2Y:
                    _joyData[ _joysCount ].hasAxis[ Hat2Y ] = true;
                    _joyData[ _joysCount ].axesMap[ j ] = Hat2Y;
                    break;

                case ABS_HAT3X:
                    _joyData[ _joysCount ].hasAxis[ Hat3X ] = true;
                    _joyData[ _joysCount ].axesMap[ j ] = Hat3X;
                    break;

                case ABS_HAT3Y:
                    _joyData[ _joysCount ].hasAxis[ Hat3Y ] = true;
                    _joyData[ _joysCount ].axesMap[ j ] = Hat3Y;
                    break;

                case ABS_PRESSURE:
                    _joyData[ _joysCount ].hasAxis[ Pressure ] = true;
                    _joyData[ _joysCount ].axesMap[ j ] = Pressure;
                    break;

                case ABS_DISTANCE:
                    _joyData[ _joysCount ].hasAxis[ Distance ] = true;
                    _joyData[ _joysCount ].axesMap[ j ] = Distance;
                    break;

                case ABS_TILT_X:
                    _joyData[ _joysCount ].hasAxis[ TiltX ] = true;
                    _joyData[ _joysCount ].axesMap[ j ] = TiltX;
                    break;

                case ABS_TILT_Y:
                    _joyData[ _joysCount ].hasAxis[ TiltY ] = true;
                    _joyData[ _joysCount ].axesMap[ j ] = TiltY;
                    break;

                case ABS_TOOL_WIDTH:
                    _joyData[ _joysCount ].hasAxis[ ToolWidth ] = true;
                    _joyData[ _joysCount ].axesMap[ j ] = ToolWidth;
                    break;
                }
            }

            if ( _joyData[ _joysCount ].hasAxis[ Hat0X ]
              || _joyData[ _joysCount ].hasAxis[ Hat0Y ] )
            {
                _joyData[ _joysCount ].hasPOV[ 0 ] = true;
                _joyData[ _joysCount ].povsCount++;
            }

            if ( _joyData[ _joysCount ].hasAxis[ Hat1X ]
              || _joyData[ _joysCount ].hasAxis[ Hat1Y ] )
            {
                _joyData[ _joysCount ].hasPOV[ 1 ] = true;
                _joyData[ _joysCount ].povsCount++;
            }

            if ( _joyData[ _joysCount ].hasAxis[ Hat2X ]
              || _joyData[ _joysCount ].hasAxis[ Hat2Y ] )
            {
                _joyData[ _joysCount ].hasPOV[ 2 ] = true;
                _joyData[ _joysCount ].povsCount++;
            }

            if ( _joyData[ _joysCount ].hasAxis[ Hat3X ]
              || _joyData[ _joysCount ].hasAxis[ Hat3Y ] )
            {
                _joyData[ _joysCount ].hasPOV[ 3 ] = true;
                _joyData[ _joysCount ].povsCount++;
            }

            _joysCount++;
        }
    }
}
#endif

////////////////////////////////////////////////////////////////////////////////

#ifdef HID_WINMM_JOYSTICK
void Joysticks::init()
{
    UINT joyIdTemp = JOYSTICKID1;
    JOYCAPS joyCaps;

    for ( int i = 0; i < HID_MAX_JOYS; i++ )
    {
        if ( _joysCount > (short)joyGetNumDevs() ) break;

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
                _joyData[ _joysCount ].name = tempNameChar;

                _joyData[ _joysCount ].axisCount = joyCaps.wNumAxes;
                _joyData[ _joysCount ].buttCount = joyCaps.wNumButtons;
                _joyData[ _joysCount ].povsCount = 0;

                _joyData[ _joysCount ].active   = true;
                _joyData[ _joysCount ].feedback = false;

                _joyData[ _joysCount ].hasAxis[ AxisX ] = true;
                _joyData[ _joysCount ].hasAxis[ AxisY ] = true;

                if ( joyCaps.wCaps & JOYCAPS_HASZ ) _joyData[ _joysCount ].hasAxis[ AxisZ ] = true;
                if ( joyCaps.wCaps & JOYCAPS_HASR ) _joyData[ _joysCount ].hasAxis[ AxisR ] = true;
                if ( joyCaps.wCaps & JOYCAPS_HASU ) _joyData[ _joysCount ].hasAxis[ AxisU ] = true;
                if ( joyCaps.wCaps & JOYCAPS_HASV ) _joyData[ _joysCount ].hasAxis[ AxisZ ] = true;

                if ( joyCaps.wCaps & JOYCAPS_HASPOV )
                {
                    _joyData[ _joysCount ].hasPOV = true;
                    _joyData[ _joysCount ].povsCount = 1;
                }

                _joysCount++;
            }
        }

        if ( joyIdTemp == JOYSTICKID2 ) break;
        if ( joyIdTemp == JOYSTICKID1 ) joyIdTemp = JOYSTICKID2;
    }
}
#endif

////////////////////////////////////////////////////////////////////////////////

#ifdef HID_LINUX_JOYSTICK
void Joysticks::update()
{
    for( short i = 0; i < _joysCount && i < HID_MAX_JOYS; i++ )
    {
        while( read( _joysFD[ i ], &_joyEvent, sizeof(js_event) ) > 0 )
        {
            // buttons
            if ( _joyEvent.type == JS_EVENT_BUTTON )
            {
                if ( _joyEvent.number < HID_MAX_BUTT )
                {
                    _joyData[ i ].butt[ _joyEvent.number ] = ( _joyEvent.value ) ? 1 : 0;
                }
            }

            // axes
            if ( _joyEvent.type == JS_EVENT_AXIS )
            {
                float value = _joyEvent.value / (double)HID_AXIS_RANGE;

                switch ( _joyData[ i ].axesMap[ _joyEvent.number ] )
                {
                    case AxisX:  _joyData[ i ].axis[ AxisX  ] = value; break;
                    case AxisY:  _joyData[ i ].axis[ AxisY  ] = value; break;
                    case AxisZ:  _joyData[ i ].axis[ AxisZ  ] = value; break;
                    case AxisRX: _joyData[ i ].axis[ AxisRX ] = value; break;
                    case AxisRY: _joyData[ i ].axis[ AxisRY ] = value; break;
                    case AxisRZ: _joyData[ i ].axis[ AxisRZ ] = value; break;

                    case Throttle: _joyData[ i ].axis[ Throttle ] = value; break;
                    case Rudder:   _joyData[ i ].axis[ Rudder   ] = value; break;
                    case Gas:      _joyData[ i ].axis[ Gas      ] = value; break;
                    case Wheel:    _joyData[ i ].axis[ Wheel    ] = value; break;
                    case Brake:    _joyData[ i ].axis[ Brake    ] = value; break;

                    case Hat0X: _joyData[ i ].axis[ Hat0X ] = value; break;
                    case Hat0Y: _joyData[ i ].axis[ Hat0Y ] = value; break;
                    case Hat1X: _joyData[ i ].axis[ Hat1X ] = value; break;
                    case Hat1Y: _joyData[ i ].axis[ Hat1Y ] = value; break;
                    case Hat2X: _joyData[ i ].axis[ Hat2X ] = value; break;
                    case Hat2Y: _joyData[ i ].axis[ Hat2Y ] = value; break;
                    case Hat3X: _joyData[ i ].axis[ Hat3X ] = value; break;
                    case Hat3Y: _joyData[ i ].axis[ Hat3Y ] = value; break;

                    case Pressure:  _joyData[ i ].axis[ Pressure  ] = value; break;
                    case Distance:  _joyData[ i ].axis[ Distance  ] = value; break;
                    case TiltX:     _joyData[ i ].axis[ TiltX     ] = value; break;
                    case TiltY:     _joyData[ i ].axis[ TiltY     ] = value; break;
                    case ToolWidth: _joyData[ i ].axis[ ToolWidth ] = value; break;
                }

                for ( short i_pov = 0; i_pov < HID_MAX_POVS; i_pov++ )
                {
                    _joyData[ i ].povs[ i_pov ] = -1;

                    if ( _joyData[ i ].hasPOV[ i_pov ]
                         && (
                                _joyData[ i ].axis[ Hat0X + i_pov ] != 0.0f
                             || _joyData[ i ].axis[ Hat0Y + i_pov ] != 0.0f
                            )
                       )
                    {
                        float angle_rad = atan2( _joyData[ i ].axis[ Hat0X + i_pov ],
                                                -_joyData[ i ].axis[ Hat0Y + i_pov ] );

                        short angle_deg = 180 * angle_rad / M_PI;

                        while ( angle_deg <   0 ) angle_deg += 360;
                        while ( angle_deg > 360 ) angle_deg -= 360;

                        _joyData[ i ].povs[ i_pov ] = angle_deg;
                    }
                }
            }
        }
    }
}
#endif

////////////////////////////////////////////////////////////////////////////////

#ifdef HID_WINMM_JOYSTICK
void Joysticks::update()
{
    JOYINFOEX joyInfoEx;

    joyInfoEx.dwSize = sizeof(JOYINFOEX);
    joyInfoEx.dwFlags = JOY_RETURNALL;

    UINT joyIdTemp = JOYSTICKID1;

    for ( int i = 0; i < _joysCount; i++ )
    {
        if ( joyGetPosEx( joyIdTemp, &joyInfoEx ) == JOYERR_NOERROR )
        {
            for ( int j = 0; j < _joyData[ i ].buttCount; j++ )
            {
                _joyData[ i ].butt[ j ] = ( _buttons[ j ] & joyInfoEx.dwButtons ) ? 1 : 0;
            }
        }

        _joyData[ i ].axis[ AxisX ] = 2.0f * ( (float)joyInfoEx.dwXpos / HID_AXIS_RANGE ) - 1.0f;
        _joyData[ i ].axis[ AxisY ] = 2.0f * ( (float)joyInfoEx.dwYpos / HID_AXIS_RANGE ) - 1.0f;

        if ( _joyData[ i ].hasAxis[ AxisZ ] ) _joyData[ i ].axis[ AxisZ ] = 2.0f * ( (float)joyInfoEx.dwZpos / HID_AXIS_RANGE ) - 1.0f;
        if ( _joyData[ i ].hasAxis[ AxisR ] ) _joyData[ i ].axis[ AxisR ] = 2.0f * ( (float)joyInfoEx.dwRpos / HID_AXIS_RANGE ) - 1.0f;
        if ( _joyData[ i ].hasAxis[ AxisU ] ) _joyData[ i ].axis[ AxisU ] = 2.0f * ( (float)joyInfoEx.dwUpos / HID_AXIS_RANGE ) - 1.0f;
        if ( _joyData[ i ].hasAxis[ AxisV ] ) _joyData[ i ].axis[ AxisV ] = 2.0f * ( (float)joyInfoEx.dwVpos / HID_AXIS_RANGE ) - 1.0f;

        _joyData[ i ].povs[ 0 ] = -1;

        if ( _joyData[ i ].hasPOV && joyInfoEx.dwPOV != JOY_POVCENTERED )
        {
            _joyData[ i ].povs[ 0 ] = (short)( joyInfoEx.dwPOV / 100 );
        }

        if ( joyIdTemp == JOYSTICKID2 ) break;
        if ( joyIdTemp == JOYSTICKID1 ) joyIdTemp = JOYSTICKID2;
    }
}
#endif

////////////////////////////////////////////////////////////////////////////////

short Joysticks::getJoysCount() const
{
    return _joysCount;
}

////////////////////////////////////////////////////////////////////////////////

Joysticks::Data Joysticks::getJoyData( short joyNum ) const
{
    if ( joyNum < 0 && joyNum >= HID_MAX_JOYS )
    {
        std::cerr << "ERROR! Wrong joystick index number." << std::endl;

#       ifdef HID_EXITONERROR
        exit( EXIT_FAILURE );
#       endif
    }

    return _joyData[ joyNum ];
}
