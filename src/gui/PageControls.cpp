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

#include <gui/PageControls.h>
#include <ui_PageControls.h>

#include <QSettings>

#include <Common.h>

#include <hid/hid_Manager.h>
#include <hid/hid_Joysticks.h>

#include <gui/DialogAction.h>
#include <gui/gui_Defines.h>

////////////////////////////////////////////////////////////////////////////////

PageControls::PageControls( QWidget *parent ) :
    QWidget ( parent ),
    _ui ( new Ui::PageControls ),

    _defaultAssignmentsInited ( false ),
    _comboDeviceInited ( false )
{
    _actionNames[ hid::Assignment::Trigger             ] = "trigger";              // 0
    _actionNames[ hid::Assignment::AP_Disc             ] = "ap_disc";              // 1
    _actionNames[ hid::Assignment::CWS                 ] = "cws";                  // 2
    _actionNames[ hid::Assignment::RollAxis            ] = "roll_axis";            // 3
    _actionNames[ hid::Assignment::RollBankLeft        ] = "roll_bank_left";       // 4
    _actionNames[ hid::Assignment::RollBankRight       ] = "roll_bank_right";      // 5
    _actionNames[ hid::Assignment::PitchAxis           ] = "pitch_axis";           // 6
    _actionNames[ hid::Assignment::PitchNoseUp         ] = "pitch_nose_up";        // 7
    _actionNames[ hid::Assignment::PitchNoseDown       ] = "pitch_nose_down";      // 8
    _actionNames[ hid::Assignment::YawAxis             ] = "yaw_axis";             // 9
    _actionNames[ hid::Assignment::YawTurnLeft         ] = "yaw_turn_left";        // 10
    _actionNames[ hid::Assignment::YawTurnRight        ] = "yaw_turn_right";       // 11
    _actionNames[ hid::Assignment::TrimRollAxis        ] = "trim_roll_axis";       // 12
    _actionNames[ hid::Assignment::TrimRollBankLeft    ] = "trim_roll_bank_left";  // 13
    _actionNames[ hid::Assignment::TrimRollBankRight   ] = "trim_roll_bank_right"; // 14
    _actionNames[ hid::Assignment::TrimPitchAxis       ] = "trim_pitch_axis";      // 15
    _actionNames[ hid::Assignment::TrimPitchNoseUp     ] = "trim_pitch_nose_up";   // 16
    _actionNames[ hid::Assignment::TrimPitchNoseDown   ] = "trim_pitch_nose_down"; // 17
    _actionNames[ hid::Assignment::TrimYawAxis         ] = "trim_yaw_axis";        // 18
    _actionNames[ hid::Assignment::TrimYawTurnLeft     ] = "trim_yaw_turn_left";   // 19
    _actionNames[ hid::Assignment::TrimYawTurnRight    ] = "trim_yaw_turn_right";  // 20
    _actionNames[ hid::Assignment::TrimReset           ] = "trim_reset";           // 21
    _actionNames[ hid::Assignment::BrakeLeftAxis       ] = "brake_left_axis";      // 22
    _actionNames[ hid::Assignment::BrakeLeftApply      ] = "brake_left_apply";     // 23
    _actionNames[ hid::Assignment::BrakeRightAxis      ] = "brake_right_axis";     // 24
    _actionNames[ hid::Assignment::BrakeRightApply     ] = "brake_right_apply";    // 25
    _actionNames[ hid::Assignment::ParkingBrakeToggle  ] = "parking_brake_toggle"; // 26
    _actionNames[ hid::Assignment::LandingGearToggle   ] = "landing_gear_toggle";  // 27
    _actionNames[ hid::Assignment::FlapsExtend         ] = "flaps_extend";         // 28
    _actionNames[ hid::Assignment::FlapsRetract        ] = "flaps_retract";        // 29
    _actionNames[ hid::Assignment::AirbrakeExtend      ] = "airbrake_extend";      // 30
    _actionNames[ hid::Assignment::AirbrakeRetract     ] = "airbrake_retract";     // 31
    _actionNames[ hid::Assignment::SpoilersToggle      ] = "spoilers_toggle";      // 32
    _actionNames[ hid::Assignment::CollectiveAxis      ] = "collective_axis";      // 33
    _actionNames[ hid::Assignment::CollectiveIncrease  ] = "collective_increase";  // 34
    _actionNames[ hid::Assignment::CollectiveDecreade  ] = "collective_decrease";  // 35
    _actionNames[ hid::Assignment::ThrottleAxis1       ] = "throttle_axis_1";      // 36
    _actionNames[ hid::Assignment::ThrottleAxis2       ] = "throttle_axis_2";      // 37
    _actionNames[ hid::Assignment::ThrottleAxis3       ] = "throttle_axis_3";      // 38
    _actionNames[ hid::Assignment::ThrottleAxis4       ] = "throttle_axis_4";      // 39
    _actionNames[ hid::Assignment::ThrottleIncrease    ] = "throttle_increase";    // 40
    _actionNames[ hid::Assignment::ThrottleDecrease    ] = "throttle_decrease";    // 41
    _actionNames[ hid::Assignment::MixtureAxis1        ] = "mixture_axis_1";       // 42
    _actionNames[ hid::Assignment::MixtureAxis2        ] = "mixture_axis_2";       // 43
    _actionNames[ hid::Assignment::MixtureAxis3        ] = "mixture_axis_3";       // 44
    _actionNames[ hid::Assignment::MixtureAxis4        ] = "mixture_axis_4";       // 45
    _actionNames[ hid::Assignment::MixtureRich         ] = "mixture_rich";         // 46
    _actionNames[ hid::Assignment::MixtureLean         ] = "mixture_lean";         // 47
    _actionNames[ hid::Assignment::PropellerAxis1      ] = "propeller_axis_1";     // 48
    _actionNames[ hid::Assignment::PropellerAxis2      ] = "propeller_axis_2";     // 49
    _actionNames[ hid::Assignment::PropellerAxis3      ] = "propeller_axis_3";     // 50
    _actionNames[ hid::Assignment::PropellerAxis4      ] = "propeller_axis_4";     // 51
    _actionNames[ hid::Assignment::PropellerFine       ] = "propeller_fine";       // 52
    _actionNames[ hid::Assignment::PropellerCoarse     ] = "propeller_coarse";     // 53

#   if ( HID_MAX_ACTIONS != 54 )
#       error 'HID_MAX_ACTIONS' has been changed! Check code above this line!
#   endif

    _ui->setupUi( this );

    for ( short i = 0; i < HID_MAX_ACTIONS; i++ )
    {
        memset( &(_assignments[ i ]), 0, sizeof(hid::Assignment) );
        _assignments[ i ].type = hid::Assignment::None;
    }

    createTableControls();

    if ( _ui->tableControls->rowCount() != HID_MAX_ACTIONS )
    {
        Log::e() << "Wrong actions count: " << HID_MAX_ACTIONS << std::endl;

#       ifdef SIM_EXITONERROR
        exit( EXIT_FAILURE );
#       endif
    }
}

////////////////////////////////////////////////////////////////////////////////

PageControls::~PageControls()
{
    DELPTR( _ui );
}

////////////////////////////////////////////////////////////////////////////////

void PageControls::getAssignments( hid::Assignment assignments[] )
{
    for ( short i = 0; i < HID_MAX_ACTIONS; i++ )
    {
        assignments[ i ] = _assignments[ i ];
    }
}

////////////////////////////////////////////////////////////////////////////////

void PageControls::readData()
{
    if ( !_comboDeviceInited )
    {
        short joysCount = hid::Joysticks::instance()->getCount();

        for ( short i = 0; i < joysCount; i++ )
        {
            hid::Joysticks::Data joyData = hid::Joysticks::instance()->getData( i );

            _ui->comboDevice->addItem( QString( joyData.name.c_str() ) );
        }

        _comboDeviceInited = true;
    }

    assignmentsRead();

    updateTableControls();
}

////////////////////////////////////////////////////////////////////////////////

void PageControls::saveData()
{
    assignmentsSave();
}

////////////////////////////////////////////////////////////////////////////////

void PageControls::updateAssignments()
{
    for ( int i = 0; i < HID_MAX_ACTIONS; i++ )
    {
        hid::Manager::instance()->setAssingment( (hid::Assignment::Action)i, _assignments[ i ] );
    }
}

////////////////////////////////////////////////////////////////////////////////

bool PageControls::isButtonAssignmentClearEnabled( int action )
{
    if ( action >= 0 && action < HID_MAX_ACTIONS )
    {
        if ( _assignments[ action ].type == hid::Assignment::None )
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool PageControls::isButtonAssignmentChangeEnabled( int action, int device )
{
    if ( action >= 0 && action < HID_MAX_ACTIONS )
    {
        if ( device == 0 )
        {
            if ( hid::Manager::isAxis( (hid::Assignment::Action)action ) )
            {
                return false;
            }
            else
            {
                return true;
            }
        }
        else
        {
            return true;
        }
    }
    else

    return false;
}

////////////////////////////////////////////////////////////////////////////////

void PageControls::changeAssignment( int action )
{
    if ( action >= 0 && action < HID_MAX_ACTIONS )
    {
        hid::Assignment newAssignment;

        if ( _ui->comboDevice->currentIndex() < 1 )
        {
            newAssignment = DialogAction::getKeyAssignment( this, _assignments[ action ] );
        }
        else
        {
            int joystickId = _ui->comboDevice->currentIndex() - 1;

            if ( hid::Manager::isAxis( (hid::Assignment::Action)action ) )
            {
                newAssignment = DialogAction::getJoyAxisAssignment( this, _assignments[ action ], joystickId );
            }
            else
            {
                newAssignment = DialogAction::getJoyButtAssignment( this, _assignments[ action ], joystickId );
            }
        }

        if ( newAssignment.type != hid::Assignment::None )
        {
            _assignments[ action ] = newAssignment;
        }

        updateTableControls();

        setButtonsState( _ui->tableControls->currentRow(), _ui->comboDevice->currentIndex() );
    }
    else
    {
        Log::e() << "Wrong action number: " << action << std::endl;

#       ifdef SIM_EXITONERROR
        exit( EXIT_FAILURE );
#       endif
    }
}

////////////////////////////////////////////////////////////////////////////////

void PageControls::clearAssignment( int action )
{
    if ( action >= 0 && action < HID_MAX_ACTIONS )
    {
        memset( &(_assignments[ action ]), 0, sizeof(hid::Assignment) );

        _assignments[ action ].type = hid::Assignment::None;

        updateTableControls();
        setButtonsState( _ui->tableControls->currentRow(), _ui->comboDevice->currentIndex() );
    }
    else
    {
        Log::e() << "Wrong action number: " << action << std::endl;

#       ifdef SIM_EXITONERROR
        exit( EXIT_FAILURE );
#       endif
    }
}

////////////////////////////////////////////////////////////////////////////////

void PageControls::createTableControls()
{
    if ( _ui->tableControls->rowCount() < HID_MAX_ACTIONS )
    {
        _ui->tableControls->setRowCount( HID_MAX_ACTIONS );
    }

    for ( int i = 0; i < HID_MAX_ACTIONS; i++ )
    {
        QTableWidgetItem *tableRowHeader = new QTableWidgetItem();
        tableRowHeader->setText( hid::Manager::_actionNames[ i ].c_str() );
        _ui->tableControls->setVerticalHeaderItem( i, tableRowHeader );

        QTableWidgetItem *tableItem = new QTableWidgetItem();
        _ui->tableControls->setItem( i, 0, tableItem );
    }
}

////////////////////////////////////////////////////////////////////////////////

void PageControls::updateTableControls()
{
    for ( int i = 0; i < HID_MAX_ACTIONS; i++ )
    {
        QString itemText( "" );

        hid::Joysticks::Data joyData;

        switch ( _assignments[ i ].type )
        {
        case hid::Assignment::Keyboard:
            itemText += "Keyboard: ";
            if ( _assignments[ i ].data.keyboard.keyId >= 0
              && _assignments[ i ].data.keyboard.keyId < HID_MAX_KEYS )
            {
                itemText += "\"";
                itemText += hid::Manager::_keysNames[ _assignments[ i ].data.keyboard.keyId ].c_str();
                itemText += "\"";
            }
            else
            {
                itemText += "NONE";
            }
            break;

        case hid::Assignment::Joystick:
            joyData = hid::Joysticks::instance()->getData( _assignments[ i ].data.joystick.joystickId );

            itemText += joyData.name.c_str();
            itemText += ": ";
            if ( hid::Manager::isAxis( (hid::Assignment::Action)i ) )
            {
                itemText += hid::Joysticks::_axisNames[ _assignments[ i ].data.joystick.axisId ].c_str();
                if ( _assignments[ i ].data.joystick.inverted ) itemText += " (Inverted)";
            }
            else
            {
                if ( _assignments[ i ].data.joystick.buttonId >= 0
                  && _assignments[ i ].data.joystick.buttonId < joyData.buttCount )
                {
                    itemText += "Button ";
                    itemText += QString::number( _assignments[ i ].data.joystick.buttonId + 1 );
                }
                else if ( _assignments[ i ].data.joystick.povId >= 0
                       && _assignments[ i ].data.joystick.povId < joyData.povsCount )
                {
                    itemText += "POV ";
                    itemText += QString::number( _assignments[ i ].data.joystick.povId + 1 );

                    switch ( _assignments[ i ].data.joystick.direction )
                    {
                        case hid::Assignment::North: itemText += " - N";  break;
                        case hid::Assignment::East:  itemText += " - E";  break;
                        case hid::Assignment::South: itemText += " - S";  break;
                        case hid::Assignment::West:  itemText += " - W";  break;
                        default:  itemText += " - NONE"; break;
                    }
                }
            }
            break;

        default:
            break;
        }

        _ui->tableControls->item( i, 0 )->setText( itemText );
    }
}

////////////////////////////////////////////////////////////////////////////////

void PageControls::setButtonsState( int action, int device )
{
    _ui->buttonAssignmentClear->setEnabled( isButtonAssignmentClearEnabled( action ) );
    _ui->buttonAssignmentChange->setEnabled( isButtonAssignmentChangeEnabled( action, device ) );
}

////////////////////////////////////////////////////////////////////////////////

void PageControls::initDefaultAssignments()
{
    if ( !_defaultAssignmentsInited )
    {
        QSettings settings( SIM_ORG_NAME, SIM_APP_NAME );

        settings.beginGroup( "page_controls" );
        {
            _defaultAssignmentsInited = settings.value( "default_assignments_inited", 0 ).toBool();
        }
        settings.endGroup(); // page_controls
    }

    if ( !_defaultAssignmentsInited )
    {
        QSettings settings( SIM_ORG_NAME, SIM_APP_NAME );

        settings.beginGroup( "page_controls" );
        {
            settings.setValue( "default_assignments_inited" , (int)true );
        }
        settings.endGroup(); // page_controls

        _assignments[ hid::Assignment::Trigger ].type = hid::Assignment::Keyboard;
        _assignments[ hid::Assignment::Trigger ].data.keyboard.keyId = hid::Assignment::KeySpace;

        _assignments[ hid::Assignment::AP_Disc ].type = hid::Assignment::Keyboard;
        _assignments[ hid::Assignment::AP_Disc ].data.keyboard.keyId = hid::Assignment::KeyQ;

        _assignments[ hid::Assignment::CWS ].type = hid::Assignment::Keyboard;
        _assignments[ hid::Assignment::CWS ].data.keyboard.keyId = hid::Assignment::KeyW;

        int joysCount = hid::Joysticks::instance()->getCount();

        if ( joysCount > 0 )
        {
            _assignments[ hid::Assignment::RollAxis ].type = hid::Assignment::Joystick;
            _assignments[ hid::Assignment::RollAxis ].data.joystick.joystickId = 0;
            _assignments[ hid::Assignment::RollAxis ].data.joystick.axisId     = hid::Joysticks::AxisX;
            _assignments[ hid::Assignment::RollAxis ].data.joystick.inverted   = false;
            _assignments[ hid::Assignment::RollAxis ].data.joystick.buttonId   = -1;
            _assignments[ hid::Assignment::RollAxis ].data.joystick.povId      = -1;
            _assignments[ hid::Assignment::RollAxis ].data.joystick.direction  = hid::Assignment::Centered;

            _assignments[ hid::Assignment::PitchAxis ].type = hid::Assignment::Joystick;
            _assignments[ hid::Assignment::PitchAxis ].data.joystick.joystickId = 0;
            _assignments[ hid::Assignment::PitchAxis ].data.joystick.axisId     = hid::Joysticks::AxisY;
            _assignments[ hid::Assignment::PitchAxis ].data.joystick.inverted   = false;
            _assignments[ hid::Assignment::PitchAxis ].data.joystick.buttonId   = -1;
            _assignments[ hid::Assignment::PitchAxis ].data.joystick.povId      = -1;
            _assignments[ hid::Assignment::PitchAxis ].data.joystick.direction  = hid::Assignment::Centered;
        }
        else
        {
            _assignments[ hid::Assignment::RollBankLeft ].type = hid::Assignment::Keyboard;
            _assignments[ hid::Assignment::RollBankLeft ].data.keyboard.keyId = hid::Assignment::KeyLeft;

            _assignments[ hid::Assignment::RollBankRight ].type = hid::Assignment::Keyboard;
            _assignments[ hid::Assignment::RollBankRight ].data.keyboard.keyId = hid::Assignment::KeyRight;

            _assignments[ hid::Assignment::PitchNoseUp ].type = hid::Assignment::Keyboard;
            _assignments[ hid::Assignment::PitchNoseUp ].data.keyboard.keyId = hid::Assignment::KeyDown;

            _assignments[ hid::Assignment::PitchNoseDown ].type = hid::Assignment::Keyboard;
            _assignments[ hid::Assignment::PitchNoseDown ].data.keyboard.keyId = hid::Assignment::KeyUp;
        }

        _assignments[ hid::Assignment::YawTurnLeft ].type = hid::Assignment::Keyboard;
        _assignments[ hid::Assignment::YawTurnLeft ].data.keyboard.keyId = hid::Assignment::KeyComma;

        _assignments[ hid::Assignment::YawTurnRight ].type = hid::Assignment::Keyboard;
        _assignments[ hid::Assignment::YawTurnRight ].data.keyboard.keyId = hid::Assignment::KeyPeriod;

        _assignments[ hid::Assignment::TrimRollBankLeft ].type = hid::Assignment::Keyboard;
        _assignments[ hid::Assignment::TrimRollBankLeft ].data.keyboard.keyId = hid::Assignment::Key8;

        _assignments[ hid::Assignment::TrimRollBankRight ].type = hid::Assignment::Keyboard;
        _assignments[ hid::Assignment::TrimRollBankRight ].data.keyboard.keyId = hid::Assignment::Key0;

        _assignments[ hid::Assignment::TrimPitchNoseUp ].type = hid::Assignment::Keyboard;
        _assignments[ hid::Assignment::TrimPitchNoseUp ].data.keyboard.keyId = hid::Assignment::KeyRightBracket;

        _assignments[ hid::Assignment::TrimPitchNoseDown ].type = hid::Assignment::Keyboard;
        _assignments[ hid::Assignment::TrimPitchNoseDown ].data.keyboard.keyId = hid::Assignment::KeyLeftBracket;

        _assignments[ hid::Assignment::TrimYawTurnLeft ].type = hid::Assignment::Keyboard;
        _assignments[ hid::Assignment::TrimYawTurnLeft ].data.keyboard.keyId = hid::Assignment::Key5;

        _assignments[ hid::Assignment::TrimYawTurnRight ].type = hid::Assignment::Keyboard;
        _assignments[ hid::Assignment::TrimYawTurnRight ].data.keyboard.keyId = hid::Assignment::Key7;

        _assignments[ hid::Assignment::TrimReset ].type = hid::Assignment::Keyboard;
        _assignments[ hid::Assignment::TrimReset ].data.keyboard.keyId = hid::Assignment::Key9;

        _assignments[ hid::Assignment::BrakeLeftApply ].type = hid::Assignment::Keyboard;
        _assignments[ hid::Assignment::BrakeLeftApply ].data.keyboard.keyId = hid::Assignment::KeySpace;

        _assignments[ hid::Assignment::BrakeRightApply ].type = hid::Assignment::Keyboard;
        _assignments[ hid::Assignment::BrakeRightApply ].data.keyboard.keyId = hid::Assignment::KeySpace;

        _assignments[ hid::Assignment::ParkingBrakeToggle ].type = hid::Assignment::Keyboard;
        _assignments[ hid::Assignment::ParkingBrakeToggle ].data.keyboard.keyId = hid::Assignment::KeyB;

        _assignments[ hid::Assignment::LandingGearToggle ].type = hid::Assignment::Keyboard;
        _assignments[ hid::Assignment::LandingGearToggle ].data.keyboard.keyId = hid::Assignment::KeyG;

        _assignments[ hid::Assignment::FlapsExtend ].type = hid::Assignment::Keyboard;
        _assignments[ hid::Assignment::FlapsExtend ].data.keyboard.keyId = hid::Assignment::Key2;

        _assignments[ hid::Assignment::FlapsRetract ].type = hid::Assignment::Keyboard;
        _assignments[ hid::Assignment::FlapsRetract ].data.keyboard.keyId = hid::Assignment::Key1;

        _assignments[ hid::Assignment::AirbrakeExtend ].type = hid::Assignment::Keyboard;
        _assignments[ hid::Assignment::AirbrakeExtend ].data.keyboard.keyId = hid::Assignment::Key4;

        _assignments[ hid::Assignment::AirbrakeRetract ].type = hid::Assignment::Keyboard;
        _assignments[ hid::Assignment::AirbrakeRetract ].data.keyboard.keyId = hid::Assignment::Key3;

        _assignments[ hid::Assignment::SpoilersToggle ].type = hid::Assignment::Keyboard;
        _assignments[ hid::Assignment::SpoilersToggle ].data.keyboard.keyId = hid::Assignment::KeyS;

        _assignments[ hid::Assignment::CollectiveIncrease ].type = hid::Assignment::Keyboard;
        _assignments[ hid::Assignment::CollectiveIncrease ].data.keyboard.keyId = hid::Assignment::KeyF10;

        _assignments[ hid::Assignment::CollectiveDecreade ].type = hid::Assignment::Keyboard;
        _assignments[ hid::Assignment::CollectiveDecreade ].data.keyboard.keyId = hid::Assignment::KeyF9;

        _assignments[ hid::Assignment::ThrottleIncrease ].type = hid::Assignment::Keyboard;
        _assignments[ hid::Assignment::ThrottleIncrease ].data.keyboard.keyId = hid::Assignment::KeyF2;

        _assignments[ hid::Assignment::ThrottleDecrease ].type = hid::Assignment::Keyboard;
        _assignments[ hid::Assignment::ThrottleDecrease ].data.keyboard.keyId = hid::Assignment::KeyF1;

        _assignments[ hid::Assignment::MixtureRich ].type = hid::Assignment::Keyboard;
        _assignments[ hid::Assignment::MixtureRich ].data.keyboard.keyId = hid::Assignment::KeyF6;

        _assignments[ hid::Assignment::MixtureLean ].type = hid::Assignment::Keyboard;
        _assignments[ hid::Assignment::MixtureLean ].data.keyboard.keyId = hid::Assignment::KeyF5;

        _assignments[ hid::Assignment::PropellerFine ].type = hid::Assignment::Keyboard;
        _assignments[ hid::Assignment::PropellerFine ].data.keyboard.keyId = hid::Assignment::KeyF4;

        _assignments[ hid::Assignment::PropellerCoarse ].type = hid::Assignment::Keyboard;
        _assignments[ hid::Assignment::PropellerCoarse ].data.keyboard.keyId = hid::Assignment::KeyF3;

#       if ( HID_MAX_ACTIONS != 54 )
#           error 'HID_MAX_ACTIONS' has been changed! Check code above this line!
#       endif

        assignmentsSave();
    }

    _defaultAssignmentsInited = true;
}

////////////////////////////////////////////////////////////////////////////////

void PageControls::assignmentsRead()
{
    initDefaultAssignments();

    for ( int i = 0; i < HID_MAX_ACTIONS; i++ )
    {
        assignmentRead( _assignments[ i ], (hid::Assignment::Action)i );
    }
}

////////////////////////////////////////////////////////////////////////////////

void PageControls::assignmentsSave()
{
    for ( int i = 0; i < HID_MAX_ACTIONS; i++ )
    {
        assignmentSave( _assignments[ i ], (hid::Assignment::Action)i );
    }
}

////////////////////////////////////////////////////////////////////////////////

void PageControls::assignmentRead( hid::Assignment &assignment, hid::Assignment::Action action )
{
    QSettings settings( SIM_ORG_NAME, SIM_APP_NAME );

    settings.beginGroup( "page_controls" );
    {
        settings.beginGroup( "controls" );
        {
            settings.beginGroup( _actionNames[ action ].c_str() );
            {
                int joysCount = hid::Joysticks::instance()->getCount();

                int tempType       = 0;
                int tempKeyId      = 0;
                int tempJoystickId = 0;
                int tempAxisId     = 0;
                int tempInverted   = 0;
                int tempButtonId   = 0;
                int tempPovId      = 0;
                int tempDirection  = 0;

                tempType = settings.value( "type", hid::Assignment::None ).toInt();

                switch ( tempType )
                {
                case hid::Assignment::Keyboard:
                    tempKeyId = settings.value( "key_id", -1 ).toInt();

                    assignment.type = hid::Assignment::Keyboard;
                    assignment.data.keyboard.keyId = (short)tempKeyId;
                    break;

                case hid::Assignment::Joystick:
                    tempJoystickId = settings.value( "joystick_id" , -1 ).toInt();
                    tempAxisId     = settings.value( "axis_id"     , -1 ).toInt();
                    tempInverted   = settings.value( "inverted"    , -1 ).toInt();
                    tempButtonId   = settings.value( "button_id"   , -1 ).toInt();
                    tempPovId      = settings.value( "pov_id"      , -1 ).toInt();
                    tempDirection  = settings.value( "direction"   , -1 ).toInt();

                    if ( tempJoystickId < joysCount )
                    {
                        assignment.type = hid::Assignment::Joystick;
                        assignment.data.joystick.joystickId = (short)tempJoystickId;
                        assignment.data.joystick.axisId     = (short)tempAxisId;
                        assignment.data.joystick.inverted   = (bool)tempInverted;
                        assignment.data.joystick.buttonId   = (short)tempButtonId;
                        assignment.data.joystick.povId      = (short)tempPovId;
                        assignment.data.joystick.direction  = (hid::Assignment::POVs)tempDirection;
                    }
                    else
                    {
                        assignment.type = hid::Assignment::None;
                        memset( &(assignment.data), 0, sizeof(hid::Assignment::DeviceData) );
                    }
                    break;

                default:
                    assignment.type = hid::Assignment::None;
                    break;
                }
            }
            settings.endGroup();
        }
        settings.endGroup(); // controls
    }
    settings.endGroup(); // page_controls
}

////////////////////////////////////////////////////////////////////////////////

void PageControls::assignmentSave( hid::Assignment &assignment, hid::Assignment::Action action )
{
    QSettings settings( SIM_ORG_NAME, SIM_APP_NAME );

    settings.beginGroup( "page_controls" );
    {
        settings.beginGroup( "controls" );
        {
            settings.beginGroup( _actionNames[ action ].c_str() );
            {
                switch ( assignment.type )
                {
                case hid::Assignment::Keyboard:
                    settings.setValue( "type", hid::Assignment::Keyboard );
                    settings.setValue( "key_id", (int)assignment.data.keyboard.keyId );
                    break;

                case hid::Assignment::Joystick:
                    settings.setValue( "type", hid::Assignment::Joystick );
                    settings.setValue( "joystick_id" , (int)assignment.data.joystick.joystickId );
                    settings.setValue( "axis_id"     , (int)assignment.data.joystick.axisId     );
                    settings.setValue( "inverted"    , (int)assignment.data.joystick.inverted   );
                    settings.setValue( "button_id"   , (int)assignment.data.joystick.buttonId   );
                    settings.setValue( "pov_id"      , (int)assignment.data.joystick.povId      );
                    settings.setValue( "direction"   , (int)assignment.data.joystick.direction  );
                    break;

                default:
                    settings.setValue( "type", hid::Assignment::None );
                    settings.setValue( "key_id"      , 0 );
                    settings.setValue( "joystick_id" , 0 );
                    settings.setValue( "axis_id"     , 0 );
                    settings.setValue( "inverted"    , 0 );
                    settings.setValue( "button_id"   , 0 );
                    settings.setValue( "pov_id"      , 0 );
                    settings.setValue( "direction"   , 0 );
                    break;
                }
            }
            settings.endGroup();

        }
        settings.endGroup(); // controls
    }
    settings.endGroup(); // page_controls
}

////////////////////////////////////////////////////////////////////////////////

void PageControls::on_buttonAssignmentClear_clicked()
{
    clearAssignment( _ui->tableControls->currentRow() );
}

////////////////////////////////////////////////////////////////////////////////

void PageControls::on_buttonAssignmentChange_clicked()
{
    changeAssignment( _ui->tableControls->currentRow() );
}

////////////////////////////////////////////////////////////////////////////////

void PageControls::on_comboDevice_currentIndexChanged( int index )
{
    setButtonsState( _ui->tableControls->currentRow(), index );
}

////////////////////////////////////////////////////////////////////////////////

void PageControls::on_tableControls_cellDoubleClicked( int row, int )
{
    if ( isButtonAssignmentChangeEnabled( row, _ui->comboDevice->currentIndex() ) )
    {
        changeAssignment( row );
    }
}

////////////////////////////////////////////////////////////////////////////////

void PageControls::on_tableControls_currentCellChanged( int row, int, int, int )
{
    setButtonsState( row, _ui->comboDevice->currentIndex() );
}
