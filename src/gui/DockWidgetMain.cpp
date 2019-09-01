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

#include <fdm/utils/fdm_Units.h>

#include <gui/gui_Defines.h>

////////////////////////////////////////////////////////////////////////////////

DockWidgetMain::DockWidgetMain( QWidget *parent ) :
    QDockWidget ( parent ),
    _ui ( new Ui::DockWidgetMain ),

    _phaseInp ( fdm::DataInp::Idle ),
    _stateOut ( fdm::DataOut::Idle ),

    _timerId ( 0 ),

    _blink ( false )
{
    _ui->setupUi( this );

    _timerId = startTimer( 400 );
}

////////////////////////////////////////////////////////////////////////////////

DockWidgetMain::~DockWidgetMain()
{
    if ( _ui ) delete _ui;
    _ui = 0;
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::setPhaseInp( fdm::DataInp::PhaseInp phaseInp )
{
    _phaseInp = phaseInp;

    switch ( _phaseInp )
    {
        case fdm::DataInp::Idle:  _ui->buttonPhaseInpIdle->setChecked( true );  break;
        case fdm::DataInp::Init:  _ui->buttonPhaseInpInit->setChecked( true );  break;
        case fdm::DataInp::Work:  _ui->buttonPhaseInpWork->setChecked( true );  break;
        case fdm::DataInp::Pause: _ui->buttonPhaseInpPause->setChecked( true ); break;
        case fdm::DataInp::Stop:  _ui->buttonPhaseInpStop->setChecked( true );  break;
        default:                  _ui->buttonPhaseInpIdle->setChecked( true );  break;
    }
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::setStateOut( fdm::DataOut::StateOut stateOut )
{
    if ( _stateOut != stateOut ) _blink = false;

    _stateOut = stateOut;

    switch ( _stateOut )
    {
    default:
    case fdm::DataOut::Idle:
        _ui->buttonStateOutIdle->setChecked( true );
        _ui->buttonStateOutReady->setChecked( false );
        _ui->buttonStateOutWorking->setChecked( false );
        _ui->buttonStateOutPaused->setChecked( false );
        _ui->buttonStateOutStopped->setChecked( false );

        _ui->buttonPhaseInpIdle->setEnabled( true );
        _ui->buttonPhaseInpInit->setEnabled( true );
        _ui->buttonPhaseInpWork->setEnabled( false );
        _ui->buttonPhaseInpPause->setEnabled( false );
        _ui->buttonPhaseInpStop->setEnabled( false );
        break;

    case fdm::DataOut::Initializing:
        _ui->buttonStateOutIdle->setChecked( false );
        _ui->buttonStateOutReady->setChecked( _blink );
        _ui->buttonStateOutWorking->setChecked( false );
        _ui->buttonStateOutPaused->setChecked( false );
        _ui->buttonStateOutStopped->setChecked( false );

        _ui->buttonPhaseInpIdle->setEnabled( false );
        _ui->buttonPhaseInpInit->setEnabled( true );
        _ui->buttonPhaseInpWork->setEnabled( false );
        _ui->buttonPhaseInpPause->setEnabled( false );
        _ui->buttonPhaseInpStop->setEnabled( true );
        break;

    case fdm::DataOut::Ready:
        _ui->buttonStateOutIdle->setChecked( false );
        _ui->buttonStateOutReady->setChecked( true );
        _ui->buttonStateOutWorking->setChecked( false );
        _ui->buttonStateOutPaused->setChecked( false );
        _ui->buttonStateOutStopped->setChecked( false );

        _ui->buttonPhaseInpIdle->setEnabled( false );
        _ui->buttonPhaseInpInit->setEnabled( true );
        _ui->buttonPhaseInpWork->setEnabled( true );
        _ui->buttonPhaseInpPause->setEnabled( true );
        _ui->buttonPhaseInpStop->setEnabled( true );
        break;

    case fdm::DataOut::Working:
        _ui->buttonStateOutIdle->setChecked( false );
        _ui->buttonStateOutReady->setChecked( false );
        _ui->buttonStateOutWorking->setChecked( true );
        _ui->buttonStateOutPaused->setChecked( false );
        _ui->buttonStateOutStopped->setChecked( false );

        _ui->buttonPhaseInpIdle->setEnabled( false );
        _ui->buttonPhaseInpInit->setEnabled( false );
        _ui->buttonPhaseInpWork->setEnabled( true );
        _ui->buttonPhaseInpPause->setEnabled( true );
        _ui->buttonPhaseInpStop->setEnabled( true );
        break;

    case fdm::DataOut::Paused:
        _ui->buttonStateOutIdle->setChecked( false );
        _ui->buttonStateOutReady->setChecked( false );
        _ui->buttonStateOutWorking->setChecked( false );
        _ui->buttonStateOutPaused->setChecked( true );
        _ui->buttonStateOutStopped->setChecked( false );

        _ui->buttonPhaseInpIdle->setEnabled( false );
        _ui->buttonPhaseInpInit->setEnabled( false );
        _ui->buttonPhaseInpWork->setEnabled( true );
        _ui->buttonPhaseInpPause->setEnabled( true );
        _ui->buttonPhaseInpStop->setEnabled( true );
        break;

    case fdm::DataOut::Stopped:
        _ui->buttonStateOutIdle->setChecked( false );
        _ui->buttonStateOutReady->setChecked( false );
        _ui->buttonStateOutWorking->setChecked( false );
        _ui->buttonStateOutPaused->setChecked( false );
        _ui->buttonStateOutStopped->setChecked( true );

        _ui->buttonPhaseInpIdle->setEnabled( true );
        _ui->buttonPhaseInpInit->setEnabled( false );
        _ui->buttonPhaseInpWork->setEnabled( false );
        _ui->buttonPhaseInpPause->setEnabled( false );
        _ui->buttonPhaseInpStop->setEnabled( true );
        break;
    }
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::setTimeStep( double timeStep )
{
    _ui->spinStep->setValue( 1000 * timeStep );
    _ui->spinFreq->setValue( ( timeStep > 0.0 ) ? 1.0 / timeStep : 0.0 );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::setFlightTime( QTime time )
{
    _ui->timeFlightTime->setTime( time );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::closeEvent( QCloseEvent *event )
{
    /////////////////////////////////
    QDockWidget::closeEvent( event );
    /////////////////////////////////

    emit closed();
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::timerEvent( QTimerEvent *event )
{
    /////////////////////////////////
    QDockWidget::timerEvent( event );
    /////////////////////////////////

    if ( _stateOut == fdm::DataOut::Initializing )
    {
        _blink = !_blink;

        _ui->buttonStateOutReady->setChecked( _blink );
    }
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::on_buttonPhaseInpIdle_clicked()
{
    _phaseInp = fdm::DataInp::Idle;
    emit phaseInpChanged( _phaseInp );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::on_buttonPhaseInpInit_clicked()
{
    _phaseInp = fdm::DataInp::Init;
    emit phaseInpChanged( _phaseInp );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::on_buttonPhaseInpWork_clicked()
{
    _phaseInp = fdm::DataInp::Work;
    emit phaseInpChanged( _phaseInp );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::on_buttonPhaseInpPause_clicked()
{
    _phaseInp = fdm::DataInp::Pause;
    emit phaseInpChanged( _phaseInp );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::on_buttonPhaseInpStop_clicked()
{
    _phaseInp = fdm::DataInp::Stop;
    emit phaseInpChanged( _phaseInp );
}
