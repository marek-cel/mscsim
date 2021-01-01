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
#ifndef HID_ASSIGNMENT_H
#define HID_ASSIGNMENT_H

////////////////////////////////////////////////////////////////////////////////

#define HID_MAX_ACTIONS 54
#define HID_MAX_KEYS    69

////////////////////////////////////////////////////////////////////////////////

namespace hid
{

/** */
struct Assignment
{
    /** */
    enum Action
    {
        Trigger             = 0,    ///< 0:  Trigger
        AP_Disc             = 1,    ///< 1:  Autopilot Disconnect
        CWS                 = 2,    ///< 2:  Control Wheel Steering
        RollAxis            = 3,    ///< 3:  Roll (Axis)
        RollBankLeft        = 4,    ///< 4:  Roll: Bank Left
        RollBankRight       = 5,    ///< 5:  Roll: Bank Right
        PitchAxis           = 6,    ///< 6:  Pitch (Axis)
        PitchNoseUp         = 7,    ///< 7:  Pitch: Nose Up
        PitchNoseDown       = 8,    ///< 8:  Pitch: Nose Down
        YawAxis             = 9,    ///< 9:  Yaw (Axis)
        YawTurnLeft         = 10,   ///< 10: Yaw: Turn Left
        YawTurnRight        = 11,   ///< 11: Yaw: Turn Right
        TrimRollAxis        = 12,   ///< 12: Trim Roll (Axis)
        TrimRollBankLeft    = 13,   ///< 13: Trim Roll: Bank Left
        TrimRollBankRight   = 14,   ///< 14: Trim Roll: Bank Right
        TrimPitchAxis       = 15,   ///< 15: Trim Pitch (Axis)
        TrimPitchNoseUp     = 16,   ///< 16: Trim Pitch: Nose Up
        TrimPitchNoseDown   = 17,   ///< 17: Trim Pitch: Nose Down
        TrimYawAxis         = 18,   ///< 18: Trim Yaw (Axis)
        TrimYawTurnLeft     = 19,   ///< 19: Trim Yaw: Turn Left
        TrimYawTurnRight    = 20,   ///< 20: Trim Yaw: Turn Right
        TrimReset           = 21,   ///< 21: Trim Reset
        BrakeLeftAxis       = 22,   ///< 22: Wheel Brake Left (Axis)
        BrakeLeftApply      = 23,   ///< 23: Wheel Brake Left: Apply
        BrakeRightAxis      = 24,   ///< 24: Wheel Brake Right (Axis)
        BrakeRightApply     = 25,   ///< 25: Wheel Brake Right: Apply
        ParkingBrakeToggle  = 26,   ///< 26: Parking Brake: Toggle
        LandingGearToggle   = 27,   ///< 27: Landing Gear: Toggle
        FlapsExtend         = 28,   ///< 28: Flaps: Extend
        FlapsRetract        = 29,   ///< 29: Flaps: Retract
        AirbrakeExtend      = 30,   ///< 30: Airbrake: Extend
        AirbrakeRetract     = 31,   ///< 31: Airbrake: Retract
        SpoilersToggle      = 32,   ///< 32: Spoilers: Toggle
        CollectiveAxis      = 33,   ///< 33: Collective (Axis)
        CollectiveIncrease  = 34,   ///< 34: Collective: Increase
        CollectiveDecreade  = 35,   ///< 35: Collective: Decrease
        ThrottleAxis1       = 36,   ///< 36: Throttle 1 (Axis)
        ThrottleAxis2       = 37,   ///< 37; Throttle 2 (Axis)
        ThrottleAxis3       = 38,   ///< 38: Throttle 3 (Axis)
        ThrottleAxis4       = 39,   ///< 39: Throttle 4 (Axis)
        ThrottleIncrease    = 40,   ///< 40: Throttle: Increase
        ThrottleDecrease    = 41,   ///< 41: Throttle: Decrease
        MixtureAxis1        = 42,   ///< 42: Mixture 1 (Axis)
        MixtureAxis2        = 43,   ///< 43: Mixture 2 (Axis)
        MixtureAxis3        = 44,   ///< 44: Mixture 3 (Axis)
        MixtureAxis4        = 45,   ///< 45: Mixture 4 (Axis)
        MixtureRich         = 46,   ///< 46: Mixture: Rich
        MixtureLean         = 47,   ///< 47: Mixture: Lean
        PropellerAxis1      = 48,   ///< 48: Propeller 1 (Axis)
        PropellerAxis2      = 49,   ///< 49: Propeller 2 (Axis)
        PropellerAxis3      = 50,   ///< 50: Propeller 3 (Axis)
        PropellerAxis4      = 51,   ///< 51: Propeller 4 (Axis)
        PropellerFine       = 52,   ///< 52: Propeller: Fine
        PropellerCoarse     = 53    ///< 53: Propeller: Coarse
    };

#   if ( HID_MAX_ACTIONS != 54 )
#       error 'HID_MAX_ACTIONS' has been changed! Check code above this line!
#   endif

    /** */
    enum DeviceType
    {
        None     = 0,               ///< 0: none
        Keyboard = 1,               ///< 1: keyboard
        Joystick = 2                ///< 2: joystick
    };

    /** */
    enum Key
    {
        Key0 = 0,                   ///< 0:  "0"
        Key1,                       ///< 1:  "1"
        Key2,                       ///< 2:  "2"
        Key3,                       ///< 3:  "3"
        Key4,                       ///< 4:  "4"
        Key5,                       ///< 5:  "5"
        Key6,                       ///< 6:  "6"
        Key7,                       ///< 7:  "7"
        Key8,                       ///< 8:  "8"
        Key9,                       ///< 9:  "9"

        KeyA,                       ///< 10: "a"
        KeyB,                       ///< 11: "b"
        KeyC,                       ///< 12: "c"
        KeyD,                       ///< 13: "d"
        KeyE,                       ///< 14: "e"
        KeyF,                       ///< 15: "f"
        KeyG,                       ///< 16: "g"
        KeyH,                       ///< 17: "h"
        KeyI,                       ///< 18: "i"
        KeyJ,                       ///< 19: "j"
        KeyK,                       ///< 20: "k"
        KeyL,                       ///< 21: "l"
        KeyM,                       ///< 22: "m"
        KeyN,                       ///< 23: "n"
        KeyO,                       ///< 24: "o"
        KeyP,                       ///< 25: "p"
        KeyQ,                       ///< 26: "q"
        KeyR,                       ///< 27: "r"
        KeyS,                       ///< 28: "s"
        KeyT,                       ///< 29: "t"
        KeyU,                       ///< 30: "u"
        KeyV,                       ///< 31: "v"
        KeyW,                       ///< 32: "w"
        KeyX,                       ///< 33: "x"
        KeyY,                       ///< 34: "y"
        KeyZ,                       ///< 35: "z"

        KeyBackquote,               ///< 36: "`"
        KeyMinus,                   ///< 37: "-"
        KeyEquals,                  ///< 38: "="
        KeyLeftBracket,             ///< 39: "["
        KeyRightBracket,            ///< 40: "]"
        KeySemicolon,               ///< 41: ";"
        KeyQuote,                   ///< 42: "'"
        KeyComma,                   ///< 43: ","
        KeyPeriod,                  ///< 44: "."
        KeySlash,                   ///< 45: "/"

        KeySpace,                   ///< 46: space

        KeyLeft,                    ///< 47: left
        KeyRight,                   ///< 48: right
        KeyUp,                      ///< 49: up
        KeyDown,                    ///< 50: down

        KeyInsert,                  ///< 51: insert
        KeyDelete,                  ///< 52: delete
        KeyHome,                    ///< 53: home
        KeyEnd,                     ///< 54: end
        KeyPageUp,                  ///< 55: page up
        KeyPageDown,                ///< 56: page down

        KeyF1,                      ///< 57: F1
        KeyF2,                      ///< 58: F2
        KeyF3,                      ///< 59: F3
        KeyF4,                      ///< 60: F4
        KeyF5,                      ///< 61: F5
        KeyF6,                      ///< 62: F6
        KeyF7,                      ///< 63: F7
        KeyF8,                      ///< 64: F8
        KeyF9,                      ///< 65: F9
        KeyF10,                     ///< 66: F10
        KeyF11,                     ///< 67: F11
        KeyF12                      ///< 68: F12
    };

#   if ( HID_MAX_KEYS != 69 )
#       error 'HID_MAX_KEYS' has been changed! Check code following this line!
#   endif

    /** */
    enum POVs
    {
        Centered = 0x00,            ///<
        North    = 0x01,            ///<
        East     = 0x02,            ///<
        South    = 0x04,            ///<
        West     = 0x08,            ///<

        NorthEast = North | East,   ///<
        SouthEast = South | East,   ///<
        SouthWest = South | West,   ///<
        NorthWest = North | West    ///<
    };

    /** */
    struct KeyboardData
    {
        short keyId;                ///< key ID
    };

    /** */
    struct JoystickData
    {
        short joystickId;           ///< joystick ID
        short axisId;               ///< axis ID
        bool  inverted;             ///< is inverted
        short buttonId;             ///< button ID
        short povId;                ///< POV ID
        POVs  direction;            ///< POV direction
    };

    /** */
    union DeviceData
    {
        KeyboardData keyboard;      ///< keyboard data
        JoystickData joystick;      ///< joystick data
    };

    DeviceType type;                ///< input device type
    DeviceData data;                ///< input device data
};

} // end of hid namepsace

////////////////////////////////////////////////////////////////////////////////

#endif // HID_ASSIGNMENT_H
