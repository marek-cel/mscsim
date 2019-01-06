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

#include <gui/DialogAction.h>
#include <ui_DialogAction.h>

#include <iostream>

#include <hid/hid_Manager.h>
#include <hid/hid_Joysticks.h>

#include <gui/gui_Defines.h>

////////////////////////////////////////////////////////////////////////////////

DialogAction::Assignment DialogAction::getJoyAxisAssignment( QWidget *parent, Assignment assignment, short joystickId )
{
    DialogAction *dialog = 0;

    dialog = new DialogAction( parent, joystickId );

    dialog->m_ui->stackedWidget->setCurrentIndex( 0 );

    if ( assignment.type == Assignment::Joystick )
    {
        if ( joystickId == assignment.data.joystick.joystickId )
        {
            hid::Joysticks::Data joyData = hid::Joysticks::instance()->getData( assignment.data.joystick.joystickId );

            int absentAxesCount = 0;

            for ( short i = 0; i < HID_MAX_AXES; i++ )
            {
                if ( !joyData.hasAxis[ i ] ) absentAxesCount++;

                if ( assignment.data.joystick.axisId == i ) break;
            }

            dialog->m_ui->comboAxis->setCurrentIndex( assignment.data.joystick.axisId - absentAxesCount );

            dialog->m_ui->checkAxisInverted->setChecked( assignment.data.joystick.inverted );
        }
    }

    if ( dialog->exec() == QDialog::Accepted )
    {
        if ( dialog->m_joystickId > -1 && dialog->m_axisId > -1 )
        {
            assignment.type = Assignment::Joystick;

            assignment.data.joystick.joystickId = dialog->m_joystickId;
            assignment.data.joystick.axisId     = dialog->m_axisId;
            assignment.data.joystick.inverted   = dialog->m_inverted;
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

DialogAction::Assignment DialogAction::getJoyButtAssignment( QWidget *parent, Assignment assignment, short joystickId )
{
    DialogAction *dialog = 0;

    dialog = new DialogAction( parent, joystickId );

    dialog->m_ui->stackedWidget->setCurrentIndex( 1 );

    if ( assignment.type == Assignment::Joystick )
    {
        hid::Joysticks::Data joyData = hid::Joysticks::instance()->getData( joystickId );

        if ( joystickId == assignment.data.joystick.joystickId )
        {
            if ( assignment.data.joystick.buttonId >= 0
              && assignment.data.joystick.buttonId < joyData.buttCount )
            {
                dialog->m_ui->comboButt->setCurrentIndex( assignment.data.joystick.buttonId );
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

                dialog->m_ui->comboButt->setCurrentIndex( joyData.buttCount + 4 * assignment.data.joystick.povId + dir );
            }
        }
    }

    if ( dialog->exec() == QDialog::Accepted )
    {
        if ( dialog->m_joystickId > -1 )
        {
            assignment.type = Assignment::Joystick;

            assignment.data.joystick.joystickId = dialog->m_joystickId;
            assignment.data.joystick.axisId     = -1;
            assignment.data.joystick.inverted   = false;

            if ( dialog->m_buttonId > -1 )
            {
                assignment.data.joystick.buttonId   = dialog->m_buttonId;
                assignment.data.joystick.povId      = -1;
                assignment.data.joystick.direction  = hid::Assignment::Centered;
            }
            else if ( dialog->m_povId > -1 )
            {
                assignment.data.joystick.buttonId   = -1;
                assignment.data.joystick.povId      = dialog->m_povId;
                assignment.data.joystick.direction  = dialog->m_povDir;
            }
        }
    }

    if ( dialog ) delete dialog;
    dialog = 0;

    return assignment;
}

////////////////////////////////////////////////////////////////////////////////

DialogAction::Assignment DialogAction::getKeyAssignment( QWidget *parent, Assignment assignment )
{
    DialogAction *dialog = 0;

    dialog = new DialogAction( parent );

    dialog->m_ui->stackedWidget->setCurrentIndex( 2 );

    if ( dialog->exec() == QDialog::Accepted )
    {
        if ( dialog->m_keyId > -1 )
        {
            assignment.type = Assignment::Keyboard;
            assignment.data.keyboard.keyId = dialog->m_keyId;
        }
    }

    if ( dialog ) delete dialog;
    dialog = 0;

    return assignment;
}

////////////////////////////////////////////////////////////////////////////////

DialogAction::DialogAction( QWidget *parent, short joystickId ) :
    QDialog( parent ),
    m_ui( new Ui::DialogAction ),

    m_timerId ( 0 ),

    m_keyId ( -1 ),

    m_axisCount ( 0 ),
    m_buttCount ( 0 ),
    m_povsCount ( 0 ),

    m_joystickId ( -1 ),
    m_axisId     ( -1 ),
    m_buttonId   ( -1 ),
    m_povId      ( -1 ),
    m_povDir     ( hid::Assignment::Centered ),
    m_inverted ( false )
{
    m_ui->setupUi( this );

    short joysticksCount = hid::Joysticks::instance()->getCount();

    if ( joystickId >= 0 && joystickId < joysticksCount )
    {
        m_joystickId = joystickId;

        hid::Joysticks::Data joyData = hid::Joysticks::instance()->getData( m_joystickId );

        m_ui->labelAxisDeviceName->setText( joyData.name.c_str() );
        m_ui->labelButtDeviceName->setText( joyData.name.c_str() );

        m_axisCount = joyData.axisCount;
        for ( short i = 0; i < HID_MAX_AXES; i++ )
        {
            if ( joyData.hasAxis[ i ] )
            {
                m_ui->comboAxis->addItem( hid::Joysticks::m_axisNames[ i ].c_str() );
            }
        }

        m_buttCount = joyData.buttCount;
        for ( short i = 0; i < m_buttCount; i++ )
        {
            m_ui->comboButt->addItem( QString::number( i + 1 ) );
        }

        m_povsCount = joyData.povsCount;
        for ( short i = 0; i < m_povsCount; i++ )
        {
            QString povText = "POV " + QString::number( i + 1 );

            m_ui->comboButt->addItem( povText + " - N" );
            m_ui->comboButt->addItem( povText + " - E" );
            m_ui->comboButt->addItem( povText + " - S" );
            m_ui->comboButt->addItem( povText + " - W" );
        }
    }

    for ( short i = 0; i < HID_MAX_KEYS; i++ )
    {
        m_ui->comboKeys->addItem( hid::Manager::m_keysNames[ i ].c_str() );
    }

    m_ui->comboAxis->setCurrentIndex( -1 );
    m_ui->comboButt->setCurrentIndex( -1 );
    m_ui->comboKeys->setCurrentIndex( -1 );

    m_timerId = startTimer( GUI_TIME_STEP );
}

////////////////////////////////////////////////////////////////////////////////

DialogAction::~DialogAction()
{
    if ( m_timerId ) killTimer( m_timerId );

    if ( m_ui ) delete m_ui;
    m_ui = 0;
}

////////////////////////////////////////////////////////////////////////////////

void DialogAction::timerEvent( QTimerEvent *event )
{
    /////////////////////////////
    QDialog::timerEvent( event );
    /////////////////////////////

    m_ui->buttonButt->setChecked( false );

    if ( m_joystickId >= 0 && m_joystickId < HID_MAX_JOYS )
    {
        float axisValue = hid::Joysticks::instance()->getData( m_joystickId ).axis[ m_axisId ];

        if ( m_inverted ) m_ui->sliderAxis->setValue( -100.0 * axisValue );
        else              m_ui->sliderAxis->setValue(  100.0 * axisValue );

        if ( m_buttonId >= 0 && m_buttonId < m_buttCount )
        {
            if ( hid::Joysticks::instance()->getData( m_joystickId ).butt[ m_buttonId ] )
            {
                m_ui->buttonButt->setChecked( true );
            }
        }
        else if ( m_povId >= 0 && m_povId < 4*m_povsCount )
        {
            if ( m_povDir != hid::Assignment::Centered )
            {
                if ( hid::Manager::getPOV( hid::Joysticks::instance()->getData( m_joystickId ).povs[ m_povId ] ) & m_povDir )
                {
                    m_ui->buttonButt->setChecked( true );
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void DialogAction::on_comboAxis_currentIndexChanged( int index )
{
    if ( index >= 0 && index < m_axisCount )
    {
        int indexTemp = -1;

        hid::Joysticks::Data joyData = hid::Joysticks::instance()->getData( m_joystickId );

        for ( short i = 0; i < HID_MAX_AXES; i++ )
        {
            if ( joyData.hasAxis[ i ] ) indexTemp++;

            if ( indexTemp == index )
            {
                m_axisId = i;
                break;
            }
        }

        m_ui->checkAxisInverted->setEnabled( true );
        m_ui->sliderAxis->setEnabled( true );
    }
    else
    {
        m_ui->checkAxisInverted->setEnabled( false );
        m_ui->checkAxisInverted->setChecked( false );
        m_ui->sliderAxis->setEnabled( false );
    }
}

////////////////////////////////////////////////////////////////////////////////

void DialogAction::on_comboButt_currentIndexChanged( int index )
{
    if ( index >= 0 && index < m_buttCount )
    {
        m_buttonId = (short)index;

        m_povDir = hid::Assignment::Centered;
        m_povId  = -1;

        m_ui->buttonButt->setText( QString("BUTTON ") + QString::number( m_buttonId + 1 ) );
    }
    else if ( index >= m_buttCount && index < m_buttCount + 4*m_povsCount )
    {
        m_buttonId = -1;

        short dir = ( index - m_buttCount ) % 4;
        m_povId = (short)( index - m_buttCount - dir );

        QString povText = "POV " + QString::number( m_povId + 1 );

        switch ( dir )
        {
        case 0:
            m_ui->buttonButt->setText( povText + " - N" );
            m_povDir = hid::Assignment::North;
            break;

        case 1:
            m_ui->buttonButt->setText( povText + " - E" );
            m_povDir = hid::Assignment::East;
            break;

        case 2:
            m_ui->buttonButt->setText( povText + " - S" );
            m_povDir = hid::Assignment::South;
            break;

        case 3:
            m_ui->buttonButt->setText( povText + " - W" );
            m_povDir = hid::Assignment::West;
            break;

        default:
            m_ui->buttonButt->setText( povText + " - NONE" );
            m_povDir = hid::Assignment::Centered;
            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void DialogAction::on_comboKeys_currentIndexChanged( int index )
{
    if ( index >= 0 && index < HID_MAX_KEYS )
    {
        m_keyId = (short)index;
    }
}

////////////////////////////////////////////////////////////////////////////////

void DialogAction::on_checkAxisInverted_toggled( bool checked )
{
    m_inverted = checked;
}
