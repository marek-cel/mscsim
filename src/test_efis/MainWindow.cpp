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

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <iostream>

#include <QSettings>

#include <test_efis/defs.h>

////////////////////////////////////////////////////////////////////////////////

using namespace std;

////////////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow( QWidget *parent ) :
    QMainWindow( parent ),
    _ui( new Ui::MainWindow ),

    _dockEFIS ( NULLPTR ),
    _dockTest ( NULLPTR ),

    _timer ( NULLPTR ),

    _timerId ( 0 ),
    _steps   ( 0 ),

    _realTime ( 0.0 )
{
    _ui->setupUi( this );

    _dockEFIS = new DockWidgetEFIS( this );
    _dockTest = new DockWidgetTest( this );

    _timer = new QElapsedTimer();

    addDockWidget( Qt::RightDockWidgetArea , _dockEFIS );
    addDockWidget( Qt::LeftDockWidgetArea  , _dockTest );

    settingsRead();

    _timer->start();
    _timerId  = startTimer( 25 );
}

////////////////////////////////////////////////////////////////////////////////

MainWindow::~MainWindow()
{
    std::cout << "Average time step: " << ( (double)_realTime ) / ( (double)_steps ) << " s" << std::endl;

    if ( _timerId ) killTimer( _timerId );

    settingsSave();

    DELPTR( _dockEFIS );
    DELPTR( _dockTest );

    DELPTR( _timer );

    DELPTR( _ui );
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::timerEvent( QTimerEvent *event )
{
    /////////////////////////////////
    QMainWindow::timerEvent( event );
    /////////////////////////////////

    float timeStep = (double)_timer->restart() / 1000.0;

    _realTime += timeStep;

    _dockTest->update( _realTime );

    double alpha     = _dockTest->getAlpha();
    double beta      = _dockTest->getBeta();
    double roll      = _dockTest->getRoll();
    double pitch     = _dockTest->getPitch();
    double heading   = _dockTest->getHeading();
    double slipSkid  = _dockTest->getSlipSkid();
    double turnRate  = _dockTest->getTurnRate();
    double course    = _dockTest->getCourse();
    double devH      = _dockTest->getDevHor();
    double devV      = _dockTest->getDevVer();
    double airspeed  = _dockTest->getAirspeed();
    double altitude  = _dockTest->getAltitude();
    double pressure  = _dockTest->getPressure();
    double climbRate = _dockTest->getClimbRate();
    double machNo    = _dockTest->getMachNo();
    double adf       = _dockTest->getADF();
    double dme       = _dockTest->getDME();

    _dockEFIS->setRoll( roll );
    _dockEFIS->setPitch( pitch );

    _dockEFIS->setSlipSkid( slipSkid );
    _dockEFIS->setTurnRate( turnRate );

    _dockEFIS->setDots( devH, devV, true, true );

    _dockEFIS->setFD( 0.0, 0.0, false );

    _dockEFIS->setStall( false );

    _dockEFIS->setAltitude( altitude );
    _dockEFIS->setAirspeed( airspeed );

    _dockEFIS->setMachNo( machNo );

    _dockEFIS->setHeading( heading );

    _dockEFIS->setClimbRate( climbRate / 100.0f );

    _dockEFIS->setCourse( course );

    _dockEFIS->setDistance( dme, true );
    _dockEFIS->setBearing( adf, true );
    _dockEFIS->setDeviation( devH, true );

    _dockEFIS->update();

    _steps++;
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::settingsRead()
{
    QSettings settings( TEST_ORG_NAME, TEST_APP_NAME );

    settings.beginGroup( "main_window" );

    restoreState( settings.value( "state" ).toByteArray() );
    restoreGeometry( settings.value( "geometry" ).toByteArray() );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::settingsSave()
{
    QSettings settings( TEST_ORG_NAME, TEST_APP_NAME );

    settings.beginGroup( "main_window" );

    settings.setValue( "state", saveState() );
    settings.setValue( "geometry", saveGeometry() );

    settings.endGroup();
}
