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

#include <gui/DockWidgetMain.h>
#include <ui_DockWidgetMain.h>

#include <fdmUtils/fdm_Units.h>

#include <gui/gui_Defines.h>

////////////////////////////////////////////////////////////////////////////////

DockWidgetMain::DockWidgetMain( QWidget *parent ) :
    QDockWidget ( parent ),
    m_ui ( new Ui::DockWidgetMain ),

    m_phaseInp ( fdm::DataInp::Idle ),
    m_stateOut ( fdm::DataOut::Idle ),

    m_timerId ( 0 ),

    m_blink ( false )
{
    m_ui->setupUi( this );

    m_timerId = startTimer( 400 );
}

////////////////////////////////////////////////////////////////////////////////

DockWidgetMain::~DockWidgetMain()
{
    if ( m_ui ) delete m_ui;
    m_ui = 0;
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::setPhaseInp( fdm::DataInp::PhaseInp phaseInp )
{
    m_phaseInp = phaseInp;

    switch ( m_phaseInp )
    {
        case fdm::DataInp::Idle:  m_ui->buttonPhaseInpIdle->setChecked( true );  break;
        case fdm::DataInp::Init:  m_ui->buttonPhaseInpInit->setChecked( true );  break;
        case fdm::DataInp::Work:  m_ui->buttonPhaseInpWork->setChecked( true );  break;
        case fdm::DataInp::Pause: m_ui->buttonPhaseInpPause->setChecked( true ); break;
        case fdm::DataInp::Stop:  m_ui->buttonPhaseInpStop->setChecked( true );  break;
        default:                  m_ui->buttonPhaseInpIdle->setChecked( true );  break;
    }
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::setStateOut( fdm::DataOut::StateOut stateOut )
{
    if ( m_stateOut != stateOut ) m_blink = false;

    m_stateOut = stateOut;

    switch ( m_stateOut )
    {
    default:
    case fdm::DataOut::Idle:
        m_ui->buttonStateOutIdle->setChecked( true );
        m_ui->buttonStateOutReady->setChecked( false );
        m_ui->buttonStateOutWorking->setChecked( false );
        m_ui->buttonStateOutPaused->setChecked( false );
        m_ui->buttonStateOutStopped->setChecked( false );

        m_ui->buttonPhaseInpIdle->setEnabled( true );
        m_ui->buttonPhaseInpInit->setEnabled( true );
        m_ui->buttonPhaseInpWork->setEnabled( false );
        m_ui->buttonPhaseInpPause->setEnabled( false );
        m_ui->buttonPhaseInpStop->setEnabled( false );
        break;

    case fdm::DataOut::Initializing:
        m_ui->buttonStateOutIdle->setChecked( false );
        m_ui->buttonStateOutReady->setChecked( m_blink );
        m_ui->buttonStateOutWorking->setChecked( false );
        m_ui->buttonStateOutPaused->setChecked( false );
        m_ui->buttonStateOutStopped->setChecked( false );

        m_ui->buttonPhaseInpIdle->setEnabled( false );
        m_ui->buttonPhaseInpInit->setEnabled( true );
        m_ui->buttonPhaseInpWork->setEnabled( false );
        m_ui->buttonPhaseInpPause->setEnabled( false );
        m_ui->buttonPhaseInpStop->setEnabled( true );
        break;

    case fdm::DataOut::Ready:
        m_ui->buttonStateOutIdle->setChecked( false );
        m_ui->buttonStateOutReady->setChecked( true );
        m_ui->buttonStateOutWorking->setChecked( false );
        m_ui->buttonStateOutPaused->setChecked( false );
        m_ui->buttonStateOutStopped->setChecked( false );

        m_ui->buttonPhaseInpIdle->setEnabled( false );
        m_ui->buttonPhaseInpInit->setEnabled( true );
        m_ui->buttonPhaseInpWork->setEnabled( true );
        m_ui->buttonPhaseInpPause->setEnabled( true );
        m_ui->buttonPhaseInpStop->setEnabled( true );
        break;

    case fdm::DataOut::Working:
        m_ui->buttonStateOutIdle->setChecked( false );
        m_ui->buttonStateOutReady->setChecked( false );
        m_ui->buttonStateOutWorking->setChecked( true );
        m_ui->buttonStateOutPaused->setChecked( false );
        m_ui->buttonStateOutStopped->setChecked( false );

        m_ui->buttonPhaseInpIdle->setEnabled( false );
        m_ui->buttonPhaseInpInit->setEnabled( false );
        m_ui->buttonPhaseInpWork->setEnabled( true );
        m_ui->buttonPhaseInpPause->setEnabled( true );
        m_ui->buttonPhaseInpStop->setEnabled( true );
        break;

    case fdm::DataOut::Paused:
        m_ui->buttonStateOutIdle->setChecked( false );
        m_ui->buttonStateOutReady->setChecked( false );
        m_ui->buttonStateOutWorking->setChecked( false );
        m_ui->buttonStateOutPaused->setChecked( true );
        m_ui->buttonStateOutStopped->setChecked( false );

        m_ui->buttonPhaseInpIdle->setEnabled( false );
        m_ui->buttonPhaseInpInit->setEnabled( false );
        m_ui->buttonPhaseInpWork->setEnabled( true );
        m_ui->buttonPhaseInpPause->setEnabled( true );
        m_ui->buttonPhaseInpStop->setEnabled( true );
        break;

    case fdm::DataOut::Stopped:
        m_ui->buttonStateOutIdle->setChecked( false );
        m_ui->buttonStateOutReady->setChecked( false );
        m_ui->buttonStateOutWorking->setChecked( false );
        m_ui->buttonStateOutPaused->setChecked( false );
        m_ui->buttonStateOutStopped->setChecked( true );

        m_ui->buttonPhaseInpIdle->setEnabled( true );
        m_ui->buttonPhaseInpInit->setEnabled( false );
        m_ui->buttonPhaseInpWork->setEnabled( false );
        m_ui->buttonPhaseInpPause->setEnabled( false );
        m_ui->buttonPhaseInpStop->setEnabled( true );
        break;
    }
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::setTimeStep( double timeStep )
{
    m_ui->spinStep->setValue( 1000 * timeStep );
    m_ui->spinFreq->setValue( ( timeStep > 0.0 ) ? 1.0 / timeStep : 0.0 );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::setFlightTime( QTime time )
{
    m_ui->timeFlightTime->setTime( time );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::timerEvent( QTimerEvent *event )
{
    /////////////////////////////////
    QDockWidget::timerEvent( event );
    /////////////////////////////////

    if ( m_stateOut == fdm::DataOut::Initializing )
    {
        m_blink = !m_blink;

        m_ui->buttonStateOutReady->setChecked( m_blink );
    }
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::on_buttonPhaseInpIdle_clicked()
{
    m_phaseInp = fdm::DataInp::Idle;
    emit phaseInpChanged( m_phaseInp );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::on_buttonPhaseInpInit_clicked()
{
    m_phaseInp = fdm::DataInp::Init;
    emit phaseInpChanged( m_phaseInp );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::on_buttonPhaseInpWork_clicked()
{
    m_phaseInp = fdm::DataInp::Work;
    emit phaseInpChanged( m_phaseInp );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::on_buttonPhaseInpPause_clicked()
{
    m_phaseInp = fdm::DataInp::Pause;
    emit phaseInpChanged( m_phaseInp );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::on_buttonPhaseInpStop_clicked()
{
    m_phaseInp = fdm::DataInp::Stop;
    emit phaseInpChanged( m_phaseInp );
}
