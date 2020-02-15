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

#include <gui/MainWindow.h>
#include <ui_MainWindow.h>

#include <iostream>

#include <QSettings>

#include <defs.h>

////////////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow),

    _dockCtrl ( nullptr ),
    _dockData ( nullptr ),
    _dockMain ( nullptr ),
    _dockTest ( nullptr ),

    _timer ( nullptr ),
    _timeStep ( 0.0 ),

    _timerId ( 0 )
{
    _ui->setupUi( this );

    setCorner( Qt::TopLeftCorner     , Qt::LeftDockWidgetArea  );
    setCorner( Qt::BottomLeftCorner  , Qt::LeftDockWidgetArea  );
    setCorner( Qt::TopRightCorner    , Qt::RightDockWidgetArea );
    setCorner( Qt::BottomRightCorner , Qt::RightDockWidgetArea );

    _dockCtrl = new DockWidgetCtrl( this );
    _dockData = new DockWidgetData( this );
    _dockMain = new DockWidgetMain( this );
    _dockTest = new DockWidgetTest( this );

    _dockMain->setDockTest( _dockTest );

    addDockWidget( Qt::LeftDockWidgetArea  , _dockMain );
    addDockWidget( Qt::LeftDockWidgetArea  , _dockCtrl );

    addDockWidget( Qt::RightDockWidgetArea , _dockData );
    addDockWidget( Qt::RightDockWidgetArea , _dockTest );

    settingsRead();

    _timer = new QElapsedTimer();
    _timer->start();

    _timerId = startTimer( 10 ); // 100 Hz
}

////////////////////////////////////////////////////////////////////////////////

MainWindow::~MainWindow()
{
    killTimer( _timerId );

    settingsSave();

    if ( _dockCtrl ) delete _dockCtrl;
    _dockCtrl = nullptr;

    if ( _dockData ) delete _dockData;
    _dockData = nullptr;

    if ( _dockMain ) delete _dockMain;
    _dockMain = nullptr;

    if ( _dockTest ) delete _dockTest;
    _dockTest = nullptr;

    if ( _timer ) delete _timer;
    _timer = nullptr;

    if ( _ui ) delete _ui;
    _ui = 0;
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::timerEvent( QTimerEvent *event )
{
    /////////////////////////////////
    QMainWindow::timerEvent( event );
    /////////////////////////////////

    _ui->widgetCGI->update();

    _timeStep = (double)_timer->restart() / 1000.0;

    updateDataBlades();

    _dockCtrl->update( _timeStep );
    //_dockData->update( _timeStep );
    _dockMain->update( _timeStep );
    _dockTest->update( _timeStep );

//    std::cout << Data::get()->rotor.collective << std::endl;
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

////////////////////////////////////////////////////////////////////////////////

void MainWindow::updateDataBlades()
{
    Data::get()->rotor.omega      = _dockCtrl->getOmega();
    Data::get()->rotor.azimuth    = _dockCtrl->getAzimuth();
    Data::get()->rotor.collective = _dockCtrl->getCollective();
    Data::get()->rotor.cyclicLon  = _dockCtrl->getCyclicLon();
    Data::get()->rotor.cyclicLat  = _dockCtrl->getCyclicLat();

    bool ccw = _dockMain->getCCW();

    double beta_0  =  Data::get()->rotor.coningAngle;
    double beta_1c = -Data::get()->rotor.diskPitch;
    double beta_1s = ( ccw ? -1.0 : 1.0 ) * Data::get()->rotor.diskRoll;

    double theta_0  = Data::get()->rotor.collective;
    double theta_1c = ( ccw ? -1.0 : 1.0 ) * Data::get()->rotor.cyclicLat;
    double theta_1s = Data::get()->rotor.cyclicLon;

    unsigned int bladesCount = 4;
    double psiStep = 2.0*M_PI / (float)bladesCount;

    for ( unsigned int i = 0; i < bladesCount; i++ )
    {
        double psi = Data::get()->rotor.azimuth + (double)(i*psiStep);

        double cosPsi = cos( psi );
        double sinPsi = sin( psi );

        Data::get()->blade[ i ].beta  =  beta_0 +  beta_1c * cosPsi +  beta_1s * sinPsi;
        Data::get()->blade[ i ].theta = theta_0 + theta_1c * cosPsi + theta_1s * sinPsi;
    }
}
