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

    _stateInp ( fdm::DataInp::Idle ),
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
    DELETE( _ui );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::setStateInp( fdm::DataInp::StateInp stateInp )
{
    _stateInp = stateInp;

    switch ( _stateInp )
    {
        case fdm::DataInp::Idle:   _ui->buttonStateInpIdle->setChecked( true );   break;
        case fdm::DataInp::Init:   _ui->buttonStateInpInit->setChecked( true );   break;
        case fdm::DataInp::Work:   _ui->buttonStateInpWork->setChecked( true );   break;
        case fdm::DataInp::Freeze: _ui->buttonStateInpFreeze->setChecked( true ); break;
        case fdm::DataInp::Pause:  _ui->buttonStateInpPause->setChecked( true );  break;
        case fdm::DataInp::Stop:   _ui->buttonStateInpStop->setChecked( true );   break;
        default:                   _ui->buttonStateInpIdle->setChecked( true );   break;
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
        _ui->buttonStateOutIdle    ->setChecked( true  );
        _ui->buttonStateOutReady   ->setChecked( false );
        _ui->buttonStateOutWorking ->setChecked( false );
        _ui->buttonStateOutFrozen  ->setChecked( false );
        _ui->buttonStateOutPaused  ->setChecked( false );
        _ui->buttonStateOutStopped ->setChecked( false );

        _ui->buttonStateInpIdle   ->setEnabled( true  );
        _ui->buttonStateInpInit   ->setEnabled( true  );
        _ui->buttonStateInpWork   ->setEnabled( false );
        _ui->buttonStateInpFreeze ->setEnabled( false );
        _ui->buttonStateInpPause  ->setEnabled( false );
        _ui->buttonStateInpStop   ->setEnabled( false );
        break;

    case fdm::DataOut::Initializing:
        _ui->buttonStateOutIdle    ->setChecked( false );
        _ui->buttonStateOutReady   ->setChecked( _blink );
        _ui->buttonStateOutWorking ->setChecked( false );
        _ui->buttonStateOutFrozen  ->setChecked( false );
        _ui->buttonStateOutPaused  ->setChecked( false );
        _ui->buttonStateOutStopped ->setChecked( false );

        _ui->buttonStateInpIdle   ->setEnabled( false );
        _ui->buttonStateInpInit   ->setEnabled( true  );
        _ui->buttonStateInpWork   ->setEnabled( false );
        _ui->buttonStateInpFreeze ->setEnabled( false );
        _ui->buttonStateInpPause  ->setEnabled( false );
        _ui->buttonStateInpStop   ->setEnabled( true  );
        break;

    case fdm::DataOut::Ready:
        _ui->buttonStateOutIdle    ->setChecked( false );
        _ui->buttonStateOutReady   ->setChecked( true  );
        _ui->buttonStateOutWorking ->setChecked( false );
        _ui->buttonStateOutFrozen  ->setChecked( false );
        _ui->buttonStateOutPaused  ->setChecked( false );
        _ui->buttonStateOutStopped ->setChecked( false );

        _ui->buttonStateInpIdle   ->setEnabled( false );
        _ui->buttonStateInpInit   ->setEnabled( true  );
        _ui->buttonStateInpWork   ->setEnabled( true  );
        _ui->buttonStateInpFreeze ->setEnabled( true  );
        _ui->buttonStateInpPause  ->setEnabled( true  );
        _ui->buttonStateInpStop   ->setEnabled( true  );
        break;

    case fdm::DataOut::Working:
        _ui->buttonStateOutIdle    ->setChecked( false );
        _ui->buttonStateOutReady   ->setChecked( false );
        _ui->buttonStateOutWorking ->setChecked( true  );
        _ui->buttonStateOutFrozen  ->setChecked( false );
        _ui->buttonStateOutPaused  ->setChecked( false );
        _ui->buttonStateOutStopped ->setChecked( false );

        _ui->buttonStateInpIdle   ->setEnabled( false );
        _ui->buttonStateInpInit   ->setEnabled( false );
        _ui->buttonStateInpWork   ->setEnabled( true  );
        _ui->buttonStateInpFreeze ->setEnabled( true  );
        _ui->buttonStateInpPause  ->setEnabled( true  );
        _ui->buttonStateInpStop   ->setEnabled( true  );
        break;

    case fdm::DataOut::Frozen:
        _ui->buttonStateOutIdle    ->setChecked( false );
        _ui->buttonStateOutReady   ->setChecked( false );
        _ui->buttonStateOutWorking ->setChecked( false );
        _ui->buttonStateOutFrozen  ->setChecked( true  );
        _ui->buttonStateOutPaused  ->setChecked( false );
        _ui->buttonStateOutStopped ->setChecked( false );

        _ui->buttonStateInpIdle   ->setEnabled( false );
        _ui->buttonStateInpInit   ->setEnabled( false );
        _ui->buttonStateInpWork   ->setEnabled( true  );
        _ui->buttonStateInpFreeze ->setEnabled( true  );
        _ui->buttonStateInpPause  ->setEnabled( true  );
        _ui->buttonStateInpStop   ->setEnabled( true  );
        break;

    case fdm::DataOut::Paused:
        _ui->buttonStateOutIdle    ->setChecked( false );
        _ui->buttonStateOutReady   ->setChecked( false );
        _ui->buttonStateOutWorking ->setChecked( false );
        _ui->buttonStateOutFrozen  ->setChecked( false );
        _ui->buttonStateOutPaused  ->setChecked( true  );
        _ui->buttonStateOutStopped ->setChecked( false );

        _ui->buttonStateInpIdle   ->setEnabled( false );
        _ui->buttonStateInpInit   ->setEnabled( false );
        _ui->buttonStateInpWork   ->setEnabled( true  );
        _ui->buttonStateInpFreeze ->setEnabled( true  );
        _ui->buttonStateInpPause  ->setEnabled( true  );
        _ui->buttonStateInpStop   ->setEnabled( true  );
        break;

    case fdm::DataOut::Stopped:
        _ui->buttonStateOutIdle    ->setChecked( false );
        _ui->buttonStateOutReady   ->setChecked( false );
        _ui->buttonStateOutWorking ->setChecked( false );
        _ui->buttonStateOutFrozen  ->setChecked( false );
        _ui->buttonStateOutPaused  ->setChecked( false );
        _ui->buttonStateOutStopped ->setChecked( true  );

        _ui->buttonStateInpIdle   ->setEnabled( true  );
        _ui->buttonStateInpInit   ->setEnabled( false );
        _ui->buttonStateInpWork   ->setEnabled( false );
        _ui->buttonStateInpFreeze ->setEnabled( false );
        _ui->buttonStateInpPause  ->setEnabled( false );
        _ui->buttonStateInpStop   ->setEnabled( true  );
        break;
    }
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::setTimeStep( double timeStep )
{
    _ui->spinStep->setValue( 1000.0 * timeStep );
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

void DockWidgetMain::on_buttonStateInpIdle_clicked()
{
    _stateInp = fdm::DataInp::Idle;
    emit stateInpChanged( _stateInp );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::on_buttonStateInpInit_clicked()
{
    _stateInp = fdm::DataInp::Init;
    emit stateInpChanged( _stateInp );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::on_buttonStateInpWork_clicked()
{
    _stateInp = fdm::DataInp::Work;
    emit stateInpChanged( _stateInp );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::on_buttonStateInpFreeze_clicked()
{
    _stateInp = fdm::DataInp::Freeze;
    emit stateInpChanged( _stateInp );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::on_buttonStateInpPause_clicked()
{
    _stateInp = fdm::DataInp::Pause;
    emit stateInpChanged( _stateInp );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::on_buttonStateInpStop_clicked()
{
    _stateInp = fdm::DataInp::Stop;
    emit stateInpChanged( _stateInp );
}
