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

#include <gui/DialogCtrl.h>
#include <ui_DialogCtrl.h>

#include <iostream>

#include <defs.h>

#include <hid/hid_Joysticks.h>
#include <gui/DialogAction.h>

////////////////////////////////////////////////////////////////////////////////

DialogCtrl::DialogCtrl( QWidget *parent ) :
    QDialog ( parent ),
    _ui ( new Ui::DialogCtrl ),

    _defaultAssignmentsInited ( false ),
    _comboDeviceInited ( false )
{
    _actionNames[ hid::Assignment::RollAxis           ] = "roll_axis";            // 0
    _actionNames[ hid::Assignment::RollBankLeft       ] = "roll_bank_left";       // 1
    _actionNames[ hid::Assignment::RollBankRight      ] = "roll_bank_right";      // 2
    _actionNames[ hid::Assignment::PitchAxis          ] = "pitch_axis";           // 3
    _actionNames[ hid::Assignment::PitchNoseUp        ] = "pitch_nose_up";        // 4
    _actionNames[ hid::Assignment::PitchNoseDown      ] = "pitch_nose_down";      // 5
    _actionNames[ hid::Assignment::YawAxis            ] = "yaw_axis";             // 6
    _actionNames[ hid::Assignment::YawTurnLeft        ] = "yaw_turn_left";        // 7
    _actionNames[ hid::Assignment::YawTurnRight       ] = "yaw_turn_right";       // 8
    _actionNames[ hid::Assignment::CollectiveAxis     ] = "collective_axis";      // 9
    _actionNames[ hid::Assignment::CollectiveIncrease ] = "collective_increase";  // 10
    _actionNames[ hid::Assignment::CollectiveDecrease ] = "collective_decrease";  // 11

#   if ( HID_MAX_ACTIONS != 12 )
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
        std::cerr << "ERROR! Wrong actions count: " << HID_MAX_ACTIONS << std::endl;

#       ifdef SIM_EXITONERROR
        exit( EXIT_FAILURE );
#       endif
    }

    settingsRead();
}

////////////////////////////////////////////////////////////////////////////////

DialogCtrl::~DialogCtrl()
{
    settingsSave();
    
    if ( _ui ) delete _ui;
    _ui = nullptr;
}

////////////////////////////////////////////////////////////////////////////////

void DialogCtrl::getAssignments( hid::Assignment assignments[] )
{
    for ( short i = 0; i < HID_MAX_ACTIONS; i++ )
    {
        assignments[ i ] = _assignments[ i ];
    }
}

////////////////////////////////////////////////////////////////////////////////

void DialogCtrl::readData()
{
    if ( !_comboDeviceInited )
    {
        short joysCount = hid::Joysticks::instance()->getJoysCount();

        for ( short i = 0; i < joysCount; i++ )
        {
            hid::Joysticks::Data joyData = hid::Joysticks::instance()->getJoyData( i );

            _ui->comboDevice->addItem( QString( joyData.name.c_str() ) );
        }

        _comboDeviceInited = true;
    }

    settingsRead();

    updateTableControls();
}

////////////////////////////////////////////////////////////////////////////////

void DialogCtrl::saveData()
{
    settingsSave();
}

////////////////////////////////////////////////////////////////////////////////

void DialogCtrl::updateAssignments()
{
    for ( int i = 0; i < HID_MAX_ACTIONS; i++ )
    {
        hid::Manager::instance()->setAssingment( (hid::Assignment::Action)i, _assignments[ i ] );
    }
}

////////////////////////////////////////////////////////////////////////////////

bool DialogCtrl::isButtonAssignmentClearEnabled( int action )
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

bool DialogCtrl::isButtonAssignmentChangeEnabled( int action, int device )
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

void DialogCtrl::changeAssignment( int action )
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
        std::cerr << "ERROR! Wrong action number: " << action << std::endl;

#       ifdef SIM_EXITONERROR
        exit( EXIT_FAILURE );
#       endif
    }
}

////////////////////////////////////////////////////////////////////////////////

void DialogCtrl::clearAssignment( int action )
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
        std::cerr << "ERROR! Wrong action number: " << action << std::endl;

#       ifdef SIM_EXITONERROR
        exit( EXIT_FAILURE );
#       endif
    }
}

////////////////////////////////////////////////////////////////////////////////

void DialogCtrl::createTableControls()
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

void DialogCtrl::updateTableControls()
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
            joyData = hid::Joysticks::instance()->getJoyData( _assignments[ i ].data.joystick.joystickId );

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

void DialogCtrl::setButtonsState( int action, int device )
{
    _ui->buttonAssignmentClear->setEnabled( isButtonAssignmentClearEnabled( action ) );
    _ui->buttonAssignmentChange->setEnabled( isButtonAssignmentChangeEnabled( action, device ) );
}

////////////////////////////////////////////////////////////////////////////////

void DialogCtrl::settingsRead()
{
    QSettings settings( TEST_ORG_NAME, TEST_APP_NAME );

    settings.beginGroup( "dialog_conf" );

    restoreGeometry( settings.value( "geometry" ).toByteArray() );

    assignmentsRead( settings );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DialogCtrl::settingsSave()
{
    QSettings settings( TEST_ORG_NAME, TEST_APP_NAME );

    settings.beginGroup( "dialog_conf" );

    settings.setValue( "geometry" , saveGeometry() );

    assignmentsSave( settings );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DialogCtrl::initDefaultAssignments()
{
    if ( !_defaultAssignmentsInited )
    {
        QSettings settings( TEST_ORG_NAME, TEST_APP_NAME );

        settings.beginGroup( "page_controls" );
        {
            _defaultAssignmentsInited = settings.value( "default_assignments_inited", 0 ).toBool();
        }
        settings.endGroup(); // page_controls
    }

    if ( !_defaultAssignmentsInited )
    {
        QSettings settings( TEST_ORG_NAME, TEST_APP_NAME );

        settings.beginGroup( "page_controls" );
        {
            settings.setValue( "default_assignments_inited" , (int)true );
        }
        settings.endGroup(); // page_controls

        int joysCount = hid::Joysticks::instance()->getJoysCount();

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

        _assignments[ hid::Assignment::CollectiveIncrease ].type = hid::Assignment::Keyboard;
        _assignments[ hid::Assignment::CollectiveIncrease ].data.keyboard.keyId = hid::Assignment::KeyEquals;

        _assignments[ hid::Assignment::CollectiveDecrease ].type = hid::Assignment::Keyboard;
        _assignments[ hid::Assignment::CollectiveDecrease ].data.keyboard.keyId = hid::Assignment::KeyMinus;

#       if ( HID_MAX_ACTIONS != 12 )
#           error 'HID_MAX_ACTIONS' has been changed! Check code above this line!
#       endif

        settingsSave();
    }

    _defaultAssignmentsInited = true;
}

////////////////////////////////////////////////////////////////////////////////

void DialogCtrl::assignmentsRead( QSettings &settings )
{
    initDefaultAssignments();

    for ( int i = 0; i < HID_MAX_ACTIONS; i++ )
    {
        assignmentRead( settings, _assignments[ i ], (hid::Assignment::Action)i );
    }
}

////////////////////////////////////////////////////////////////////////////////

void DialogCtrl::assignmentsSave( QSettings &settings )
{
    for ( int i = 0; i < HID_MAX_ACTIONS; i++ )
    {
        assignmentSave( settings, _assignments[ i ], (hid::Assignment::Action)i );
    }
}

////////////////////////////////////////////////////////////////////////////////

void DialogCtrl::assignmentRead( QSettings &settings, hid::Assignment &assignment, hid::Assignment::Action action )
{
    settings.beginGroup( "controls" );
    {
        settings.beginGroup( _actionNames[ action ].c_str() );
        {
            int joysCount = hid::Joysticks::instance()->getJoysCount();

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

////////////////////////////////////////////////////////////////////////////////

void DialogCtrl::assignmentSave( QSettings &settings, hid::Assignment &assignment, hid::Assignment::Action action )
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

////////////////////////////////////////////////////////////////////////////////

void DialogCtrl::on_buttonAssignmentClear_clicked()
{
    clearAssignment( _ui->tableControls->currentRow() );
}

////////////////////////////////////////////////////////////////////////////////

void DialogCtrl::on_buttonAssignmentChange_clicked()
{
    changeAssignment( _ui->tableControls->currentRow() );
}

////////////////////////////////////////////////////////////////////////////////

void DialogCtrl::on_comboDevice_currentIndexChanged( int index )
{
    setButtonsState( _ui->tableControls->currentRow(), index );
}

////////////////////////////////////////////////////////////////////////////////

void DialogCtrl::on_tableControls_cellDoubleClicked( int row, int )
{
    if ( isButtonAssignmentChangeEnabled( row, _ui->comboDevice->currentIndex() ) )
    {
        changeAssignment( row );
    }
}

////////////////////////////////////////////////////////////////////////////////

void DialogCtrl::on_tableControls_currentCellChanged( int row, int, int, int )
{
    setButtonsState( row, _ui->comboDevice->currentIndex() );
}
