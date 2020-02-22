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

#include <QKeyEvent>
#include <QSettings>

#include <hid/hid_Manager.h>

#include <fdm/utils/fdm_Units.h>

#include <defs.h>

#include <gui/Keys.h>

////////////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow),

    _dialogCtrl ( nullptr ),

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

    _dialogCtrl = new DialogCtrl( this );

    _dockCtrl = new DockWidgetCtrl( this );
    _dockData = new DockWidgetData( this );
    _dockMain = new DockWidgetMain( this );
    _dockTest = new DockWidgetTest( this );

    _dockMain->setDockTest( _dockTest );
    _dockMain->setWidgetCGI( _ui->widgetCGI );

    addDockWidget( Qt::LeftDockWidgetArea  , _dockMain );
    addDockWidget( Qt::LeftDockWidgetArea  , _dockCtrl );

    addDockWidget( Qt::RightDockWidgetArea , _dockData );
    addDockWidget( Qt::RightDockWidgetArea , _dockTest );

    connect( _dockCtrl, SIGNAL(closed()), this, SLOT(dockCtrl_closed()) );
    connect( _dockData, SIGNAL(closed()), this, SLOT(dockData_closed()) );
    connect( _dockMain, SIGNAL(closed()), this, SLOT(dockMain_closed()) );
    connect( _dockTest, SIGNAL(closed()), this, SLOT(dockTest_closed()) );

    settingsRead();

    hid::Manager::instance()->init();

    _dialogCtrl->readData();
    _dialogCtrl->updateAssignments();

    _timer = new QElapsedTimer();
    _timer->start();

    _timerId = startTimer( 1000.0 * SIM_TIME_STEP );
}

////////////////////////////////////////////////////////////////////////////////

MainWindow::~MainWindow()
{
    killTimer( _timerId );

    settingsSave();

    if ( _dialogCtrl ) delete _dialogCtrl;
    _dialogCtrl = nullptr;

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

void MainWindow::keyPressEvent( QKeyEvent *event )
{
    ////////////////////////////////////
    QMainWindow::keyPressEvent( event );
    ////////////////////////////////////

    if ( !event->isAutoRepeat() )
    {
        _ui->widgetCGI->keyDn( Keys::getKey( event->key() ) );
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::keyReleaseEvent( QKeyEvent *event )
{
    //////////////////////////////////////
    QMainWindow::keyReleaseEvent( event );
    //////////////////////////////////////

    if ( !event->isAutoRepeat() )
    {
        _ui->widgetCGI->keyUp( Keys::getKey( event->key() ) );
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::timerEvent( QTimerEvent *event )
{
    /////////////////////////////////
    QMainWindow::timerEvent( event );
    /////////////////////////////////

    double timeStep = (double)_timer->restart() / 1000.0;
    double timeCoef = _dockMain->getTimeCoef();

    _timeStep = timeCoef * timeStep;

    hid::Manager::instance()->update( timeStep );

    updateDataBlades();

    _dockCtrl->update( _timeStep );
    //_dockData->update( _timeStep );
    _dockMain->update( _timeStep );
    _dockTest->update( _timeStep );

    Data::get()->time_step = timeStep;
    Data::get()->time_coef = timeCoef;

//    std::cout << Data::get()->rotor.collective << std::endl;
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::settingsRead()
{
    QSettings settings( TEST_ORG_NAME, TEST_APP_NAME );

    settings.beginGroup( "main_window" );

    restoreState( settings.value( "state" ).toByteArray() );
    restoreGeometry( settings.value( "geometry" ).toByteArray() );

    bool show_vectors_main = settings.value( "show_vectors_main", 0 ).toBool();
    bool show_vectors_span = settings.value( "show_vectors_span", 0 ).toBool();
    bool show_blades_datum = settings.value( "show_blades_datum", 0 ).toBool();
    bool show_blades_trace = settings.value( "show_blades_trace", 0 ).toBool();

    _ui->actionShowVectorsSpan->setChecked( show_vectors_span );
    _ui->actionShowVectorsMain->setChecked( show_vectors_main );
    _ui->actionShowBladesDatum->setChecked( show_blades_datum );
    _ui->actionShowBladesTrace->setChecked( show_blades_trace );

    Data::get()->other.show_vectors_span = show_vectors_span;
    Data::get()->other.show_vectors_main = show_vectors_main;
    Data::get()->other.show_blades_datum = show_blades_datum;
    Data::get()->other.show_blades_trace = show_blades_trace;

    bool visibleCtrl = settings.value( "dock_ctrl_visible", 1 ).toBool();
    bool visibleData = settings.value( "dock_data_visible", 1 ).toBool();
    bool visibleMain = settings.value( "dock_main_visible", 1 ).toBool();
    bool visibleTest = settings.value( "dock_test_visible", 0 ).toBool();

    _ui->actionDockCtrl->setChecked( visibleCtrl );
    _ui->actionDockData->setChecked( visibleData );
    _ui->actionDockMain->setChecked( visibleMain );
    _ui->actionDockTest->setChecked( visibleTest );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::settingsSave()
{
    QSettings settings( TEST_ORG_NAME, TEST_APP_NAME );

    settings.beginGroup( "main_window" );

    settings.setValue( "state", saveState() );
    settings.setValue( "geometry", saveGeometry() );

    settings.setValue( "show_vectors_span", _ui->actionShowVectorsSpan->isChecked() );
    settings.setValue( "show_vectors_main", _ui->actionShowVectorsMain->isChecked() );
    settings.setValue( "show_blades_datum", _ui->actionShowBladesDatum->isChecked() );
    settings.setValue( "show_blades_trace", _ui->actionShowBladesTrace->isChecked() );

    settings.setValue( "dock_ctrl_visible", _ui->actionDockCtrl->isChecked() ? 1 : 0 );
    settings.setValue( "dock_data_visible", _ui->actionDockData->isChecked() ? 1 : 0 );
    settings.setValue( "dock_main_visible", _ui->actionDockMain->isChecked() ? 1 : 0 );
    settings.setValue( "dock_test_visible", _ui->actionDockTest->isChecked() ? 1 : 0 );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::updateDataBlades()
{
    Data::get()->rotor.omega      = _dockCtrl->getOmega();
    Data::get()->rotor.azimuth    = _dockCtrl->getAzimuth();
    //Data::get()->rotor.collective = _dockCtrl->getCollective();
    //Data::get()->rotor.cyclicLon  = _dockCtrl->getCyclicLon();
    //Data::get()->rotor.cyclicLat  = _dockCtrl->getCyclicLat();

    const double max_collective = fdm::Units::deg2rad( MAX_COLLECTIVE_DEG );
    const double max_cyclic     = fdm::Units::deg2rad( MAX_CYCLIC_DEG );
    Data::get()->rotor.collective = max_collective * hid::Manager::instance()->getCollective();
    Data::get()->rotor.cyclicLon  = max_cyclic * hid::Manager::instance()->getCtrlPitch();
    Data::get()->rotor.cyclicLat  = max_cyclic * hid::Manager::instance()->getCtrlRoll();

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

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionControls_triggered()
{
    _dialogCtrl->readData();

    if ( _dialogCtrl->exec() == QDialog::Accepted )
    {
        _dialogCtrl->saveData();
        _dialogCtrl->updateAssignments();
    }
    else
    {
        _dialogCtrl->readData();
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionDockCtrl_toggled( bool checked )
{
    _dockCtrl->setVisible( checked );
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionDockData_toggled( bool checked )
{
    _dockData->setVisible( checked );
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionDockMain_toggled( bool checked )
{
    _dockMain->setVisible( checked );
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionDockTest_toggled( bool checked )
{
    _dockTest->setVisible( checked );
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionViewAft_triggered()
{
    _ui->widgetCGI->setViewAft();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionViewFwd_triggered()
{
    _ui->widgetCGI->setViewFwd();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionViewLft_triggered()
{
    _ui->widgetCGI->setViewLft();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionViewRgt_triggered()
{
    _ui->widgetCGI->setViewRgt();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionViewTop_triggered()
{
    _ui->widgetCGI->setViewTop();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionRotationStart_triggered()
{
    _ui->widgetCGI->rotationStart();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionRotationStop_triggered()
{
    _ui->widgetCGI->rotationStop();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionShowVectorsSpan_toggled(bool arg1)
{
    Data::get()->other.show_vectors_span = arg1;
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionShowVectorsMain_toggled(bool arg1)
{
    Data::get()->other.show_vectors_main = arg1;
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionShowBladesDatum_toggled(bool arg1)
{
    Data::get()->other.show_blades_datum = arg1;
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionShowBladesTrace_toggled(bool arg1)
{
    Data::get()->other.show_blades_trace = arg1;
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::dockCtrl_closed()
{
    _ui->actionDockCtrl->setChecked( false );
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::dockData_closed()
{
    _ui->actionDockData->setChecked( false );
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::dockMain_closed()
{
    _ui->actionDockMain->setChecked( false );
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::dockTest_closed()
{
    _ui->actionDockTest->setChecked( false );
}
