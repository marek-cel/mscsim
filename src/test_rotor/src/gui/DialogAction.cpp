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
