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

#include <gui/DialogAction.h>
#include <ui_DialogAction.h>

#include <defs.h>

#include <hid/hid_Manager.h>
#include <hid/hid_Joysticks.h>

////////////////////////////////////////////////////////////////////////////////

hid::Assignment DialogAction::getJoyAxisAssignment( QWidget *parent, hid::Assignment assignment, short joystickId )
{
    DialogAction *dialog = 0;

    dialog = new DialogAction( parent, joystickId );

    dialog->_ui->stackedWidget->setCurrentIndex( 0 );

    if ( assignment.type == hid::Assignment::Joystick )
    {
        if ( joystickId == assignment.data.joystick.joystickId )
        {
            hid::Joysticks::Data joyData = hid::Joysticks::instance()->getJoyData( assignment.data.joystick.joystickId );

            int absentAxesCount = 0;

            for ( short i = 0; i < HID_MAX_AXES; i++ )
            {
                if ( !joyData.hasAxis[ i ] ) absentAxesCount++;

                if ( assignment.data.joystick.axisId == i ) break;
            }

            dialog->_ui->comboAxis->setCurrentIndex( assignment.data.joystick.axisId - absentAxesCount );

            dialog->_ui->checkAxisInverted->setChecked( assignment.data.joystick.inverted );
        }
    }

    if ( dialog->exec() == QDialog::Accepted )
    {
        if ( dialog->_joystickId > -1 && dialog->_axisId > -1 )
        {
            assignment.type = hid::Assignment::Joystick;

            assignment.data.joystick.joystickId = dialog->_joystickId;
            assignment.data.joystick.axisId     = dialog->_axisId;
            assignment.data.joystick.inverted   = dialog->_inverted;
            assignment.data.joystick.buttonId   = -1;
            assignment.data.joystick.povId      = -1;
            assignment.data.joystick.direction  = hid::Assignment::Centered;
        }
    }

    if ( dialog ) delete dialog;
    dialog = 0;

    return assignment;
}

////////////////////////////////////////////////////////////////////////////////

hid::Assignment DialogAction::getJoyButtAssignment( QWidget *parent, hid::Assignment assignment, short joystickId )
{
    DialogAction *dialog = 0;

    dialog = new DialogAction( parent, joystickId );

    dialog->_ui->stackedWidget->setCurrentIndex( 1 );

    if ( assignment.type == hid::Assignment::Joystick )
    {
        hid::Joysticks::Data joyData = hid::Joysticks::instance()->getJoyData( joystickId );

        if ( joystickId == assignment.data.joystick.joystickId )
        {
            if ( assignment.data.joystick.buttonId >= 0
              && assignment.data.joystick.buttonId < joyData.buttCount )
            {
                dialog->_ui->comboButt->setCurrentIndex( assignment.data.joystick.buttonId );
            }
            else if ( assignment.data.joystick.povId >= 0
                   && assignment.data.joystick.povId < joyData.povsCount )
            {
                short dir = -100;

                switch ( assignment.data.joystick.direction )
                {
                    case hid::Assignment::North: dir = 0; break;
                    case hid::Assignment::East:  dir = 1; break;
                    case hid::Assignment::South: dir = 2; break;
                    case hid::Assignment::West:  dir = 3; break;
                    default: dir = -100; break;
                }

                dialog->_ui->comboButt->setCurrentIndex( joyData.buttCount + 4 * assignment.data.joystick.povId + dir );
            }
        }
    }

    if ( dialog->exec() == QDialog::Accepted )
    {
        if ( dialog->_joystickId > -1 )
        {
            assignment.type = hid::Assignment::Joystick;

            assignment.data.joystick.joystickId = dialog->_joystickId;
            assignment.data.joystick.axisId     = -1;
            assignment.data.joystick.inverted   = false;

            if ( dialog->_buttonId > -1 )
            {
                assignment.data.joystick.buttonId   = dialog->_buttonId;
                assignment.data.joystick.povId      = -1;
                assignment.data.joystick.direction  = hid::Assignment::Centered;
            }
            else if ( dialog->_povId > -1 )
            {
                assignment.data.joystick.buttonId   = -1;
                assignment.data.joystick.povId      = dialog->_povId;
                assignment.data.joystick.direction  = dialog->_povDir;
            }
        }
    }

    if ( dialog ) delete dialog;
    dialog = 0;

    return assignment;
}

////////////////////////////////////////////////////////////////////////////////

hid::Assignment DialogAction::getKeyAssignment( QWidget *parent, hid::Assignment assignment )
{
    DialogAction *dialog = 0;

    dialog = new DialogAction( parent );

    dialog->_ui->stackedWidget->setCurrentIndex( 2 );

    if ( dialog->exec() == QDialog::Accepted )
    {
        if ( dialog->_keyId > -1 )
        {
            assignment.type = hid::Assignment::Keyboard;
            assignment.data.keyboard.keyId = dialog->_keyId;
        }
    }

    if ( dialog ) delete dialog;
    dialog = 0;

    return assignment;
}

////////////////////////////////////////////////////////////////////////////////

DialogAction::DialogAction( QWidget *parent, short joystickId ) :
    QDialog( parent ),
    _ui( new Ui::DialogAction ),

    _timerId ( 0 ),

    _keyId ( -1 ),

    _axisCount ( 0 ),
    _buttCount ( 0 ),
    _povsCount ( 0 ),

    _joystickId ( -1 ),
    _axisId     ( -1 ),
    _buttonId   ( -1 ),
    _povId      ( -1 ),
    _povDir     ( hid::Assignment::Centered ),
    _inverted ( false )
{
    _ui->setupUi( this );

    short joysticksCount = hid::Joysticks::instance()->getJoysCount();

    if ( joystickId >= 0 && joystickId < joysticksCount )
    {
        _joystickId = joystickId;

        hid::Joysticks::Data joyData = hid::Joysticks::instance()->getJoyData( _joystickId );

        _ui->labelAxisDeviceName->setText( joyData.name.c_str() );
        _ui->labelButtDeviceName->setText( joyData.name.c_str() );

        _axisCount = joyData.axisCount;
        for ( short i = 0; i < HID_MAX_AXES; i++ )
        {
            if ( joyData.hasAxis[ i ] )
            {
                _ui->comboAxis->addItem( hid::Joysticks::_axisNames[ i ].c_str() );
            }
        }

        _buttCount = joyData.buttCount;
        for ( short i = 0; i < _buttCount; i++ )
        {
            _ui->comboButt->addItem( QString::number( i + 1 ) );
        }

        _povsCount = joyData.povsCount;
        for ( short i = 0; i < _povsCount; i++ )
        {
            QString povText = "POV " + QString::number( i + 1 );

            _ui->comboButt->addItem( povText + " - N" );
            _ui->comboButt->addItem( povText + " - E" );
            _ui->comboButt->addItem( povText + " - S" );
            _ui->comboButt->addItem( povText + " - W" );
        }
    }

    for ( short i = 0; i < HID_MAX_KEYS; i++ )
    {
        _ui->comboKeys->addItem( hid::Manager::_keysNames[ i ].c_str() );
    }

    _ui->comboAxis->setCurrentIndex( -1 );
    _ui->comboButt->setCurrentIndex( -1 );
    _ui->comboKeys->setCurrentIndex( -1 );

    _timerId = startTimer( 1000.0f * 1.0f / 60.0f );
}

////////////////////////////////////////////////////////////////////////////////

DialogAction::~DialogAction()
{
    if ( _timerId ) killTimer( _timerId );

    if ( _ui ) delete _ui;
    _ui = nullptr;
}

////////////////////////////////////////////////////////////////////////////////

void DialogAction::timerEvent( QTimerEvent *event )
{
    /////////////////////////////
    QDialog::timerEvent( event );
    /////////////////////////////

    _ui->buttonButt->setChecked( false );

    if ( _joystickId >= 0 && _joystickId < HID_MAX_JOYS )
    {
        float axisValue = hid::Joysticks::instance()->getJoyData( _joystickId ).axis[ _axisId ];

        if ( _inverted ) _ui->sliderAxis->setValue( -100.0 * axisValue );
        else             _ui->sliderAxis->setValue(  100.0 * axisValue );

        if ( _buttonId >= 0 && _buttonId < _buttCount )
        {
            if ( hid::Joysticks::instance()->getJoyData( _joystickId ).butt[ _buttonId ] )
            {
                _ui->buttonButt->setChecked( true );
            }
        }
        else if ( _povId >= 0 && _povId < 4*_povsCount )
        {
            if ( _povDir != hid::Assignment::Centered )
            {
                if ( hid::Manager::getPOV( hid::Joysticks::instance()->getJoyData( _joystickId ).povs[ _povId ] ) & _povDir )
                {
                    _ui->buttonButt->setChecked( true );
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void DialogAction::on_comboAxis_currentIndexChanged( int index )
{
    if ( index >= 0 && index < _axisCount )
    {
        int indexTemp = -1;

        hid::Joysticks::Data joyData = hid::Joysticks::instance()->getJoyData( _joystickId );

        for ( short i = 0; i < HID_MAX_AXES; i++ )
        {
            if ( joyData.hasAxis[ i ] ) indexTemp++;

            if ( indexTemp == index )
            {
                _axisId = i;
                break;
            }
        }

        _ui->checkAxisInverted->setEnabled( true );
        _ui->sliderAxis->setEnabled( true );
    }
    else
    {
        _ui->checkAxisInverted->setEnabled( false );
        _ui->checkAxisInverted->setChecked( false );
        _ui->sliderAxis->setEnabled( false );
    }
}

////////////////////////////////////////////////////////////////////////////////

void DialogAction::on_comboButt_currentIndexChanged( int index )
{
    if ( index >= 0 && index < _buttCount )
    {
        _buttonId = (short)index;

        _povDir = hid::Assignment::Centered;
        _povId  = -1;

        _ui->buttonButt->setText( QString("BUTTON ") + QString::number( _buttonId + 1 ) );
    }
    else if ( index >= _buttCount && index < _buttCount + 4*_povsCount )
    {
        _buttonId = -1;

        short dir = ( index - _buttCount ) % 4;
        _povId = (short)( index - _buttCount - dir );

        QString povText = "POV " + QString::number( _povId + 1 );

        switch ( dir )
        {
        case 0:
            _ui->buttonButt->setText( povText + " - N" );
            _povDir = hid::Assignment::North;
            break;

        case 1:
            _ui->buttonButt->setText( povText + " - E" );
            _povDir = hid::Assignment::East;
            break;

        case 2:
            _ui->buttonButt->setText( povText + " - S" );
            _povDir = hid::Assignment::South;
            break;

        case 3:
            _ui->buttonButt->setText( povText + " - W" );
            _povDir = hid::Assignment::West;
            break;

        default:
            _ui->buttonButt->setText( povText + " - NONE" );
            _povDir = hid::Assignment::Centered;
            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void DialogAction::on_comboKeys_currentIndexChanged( int index )
{
    if ( index >= 0 && index < HID_MAX_KEYS )
    {
        _keyId = (short)index;
    }
}

////////////////////////////////////////////////////////////////////////////////

void DialogAction::on_checkAxisInverted_toggled( bool checked )
{
    _inverted = checked;
}
