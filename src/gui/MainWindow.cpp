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

#include <QCloseEvent>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

#ifdef WIN32
#   include <QDir>
#endif

#include <fdm/utils/fdm_Units.h>

#include <hid/hid_Manager.h>
#include <gui/gui_Defines.h>

#include <gui/Keys.h>
#include <gui/ScreenSaver.h>

////////////////////////////////////////////////////////////////////////////////

QString getTempFile()
{
    QString result;

#   ifdef _LINUX_
    result = "/tmp/";
#   endif

#   ifdef WIN32
    result = QDir::homePath() + "/";
#   endif

    result += "fdm_temp_rec.csv";

    return result;
}

////////////////////////////////////////////////////////////////////////////////

const QString MainWindow::_tmp_file = getTempFile();

////////////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow( QWidget *parent ) :
    QMainWindow ( parent ),
    _ui ( new Ui::MainWindow ),

    _ap ( NULLPTR ),

    _dateTime( QDateTime::currentDateTimeUtc() ),
    
    _dialogConf ( NULLPTR ),
    _dialogEnvr ( NULLPTR ),
    _dialogInit ( NULLPTR ),
    _dialogMass ( NULLPTR ),

    _dockAuto ( NULLPTR ),
    _dockCtrl ( NULLPTR ),
    _dockData ( NULLPTR ),
    _dockEFIS ( NULLPTR ),
    _dockMain ( NULLPTR ),
    _dockMap  ( NULLPTR ),
    _dockProp ( NULLPTR ),

    _widgetPFD ( NULLPTR ),

    _scCycleViews ( NULLPTR ),
    _scToggleHud  ( NULLPTR ),
    _scFullScreen ( NULLPTR ),
    _scTimeFaster ( NULLPTR ),
    _scTimeSlower ( NULLPTR ),

    _viewType ( Data::CGI::ViewPilot ),

    _showHUD ( true ),
    _showTraces ( false ),

    _showRotorBlur   ( false ),
    _showBladesDatum ( false ),
    _showBladesPaths ( false ),

    _timeCoef ( 1.0 ),

    _timerId ( 0 ),

    _typeIndex ( 0 ),

    _stateInp ( fdm::DataInp::Idle ),
    _stateOut ( fdm::DataOut::Idle )
{
    _ui->setupUi( this );

    _dialogConf = new DialogConf( this );
    _dialogEnvr = new DialogEnvr( this );
    _dialogInit = new DialogInit( this );
    _dialogMass = new DialogMass( this );

    setWidescreenDockLayout( _dialogConf->getWidescreen() );

    _dockAuto = new DockWidgetAuto( this );
    _dockCtrl = new DockWidgetCtrl( this );
    _dockData = new DockWidgetData( this );
    _dockEFIS = new DockWidgetEFIS( this );
    _dockMain = new DockWidgetMain( this );
    _dockMap  = new DockWidgetMap( this );
    _dockProp = new DockWidgetProp( this );

    _widgetPFD = new WidgetPFD();

    _dockAuto->setObjectName( "DockAuto" );
    _dockCtrl->setObjectName( "DockCtrl" );
    _dockData->setObjectName( "DockData" );
    _dockEFIS->setObjectName( "DockEFIS" );
    _dockMain->setObjectName( "DockMain" );
    _dockMap->setObjectName( "DockMap" );
    _dockProp->setObjectName( "DockProp" );

    addDockWidget( Qt::TopDockWidgetArea    , _dockAuto );
    addDockWidget( Qt::BottomDockWidgetArea , _dockCtrl );
    addDockWidget( Qt::RightDockWidgetArea  , _dockData );
    addDockWidget( Qt::BottomDockWidgetArea , _dockEFIS );
    addDockWidget( Qt::LeftDockWidgetArea   , _dockMain );
    addDockWidget( Qt::BottomDockWidgetArea , _dockMap  );
    addDockWidget( Qt::LeftDockWidgetArea   , _dockProp );

    _dockAuto->setVisible( false );
    _dockCtrl->setVisible( false );
    _dockData->setVisible( false );
    _dockEFIS->setVisible( false );
    _dockMain->setVisible( false );
    _dockMap->setVisible( false );
    _dockProp->setVisible( false );

    _scCycleViews = new QShortcut( QKeySequence(Qt::CTRL + Qt::Key_V), this, SLOT(shorcutCycleViews_activated()) );
    _scToggleHud  = new QShortcut( QKeySequence(Qt::CTRL + Qt::Key_H), this, SLOT(shorcutToggleHud_activated()) );
    _scFullScreen = new QShortcut( QKeySequence(Qt::CTRL + Qt::Key_F), this, SLOT(shorcutFullScreen_activated()) );
    _scTimeFaster = new QShortcut( QKeySequence(Qt::CTRL + Qt::Key_Equal), this, SLOT(on_actionTimeFaster_triggered()) );
    _scTimeSlower = new QShortcut( QKeySequence(Qt::CTRL + Qt::Key_Minus), this, SLOT(on_actionTimeSlower_triggered()) );

    connect( _dialogInit, SIGNAL(typeIndexChanged(int)), this, SLOT(dialogInit_typeIndexChanged(int)) );
    connect( _dockMain, SIGNAL(stateInpChanged(fdm::DataInp::StateInp)), this, SLOT(dockMain_stateInpChanged(fdm::DataInp::StateInp)) );

    connect( _dockAuto , SIGNAL(closed()), this, SLOT(dockAuto_closed()) );
    connect( _dockCtrl , SIGNAL(closed()), this, SLOT(dockCtrl_closed()) );
    connect( _dockData , SIGNAL(closed()), this, SLOT(dockData_closed()) );
    connect( _dockEFIS , SIGNAL(closed()), this, SLOT(dockEFIS_closed()) );
    connect( _dockMain , SIGNAL(closed()), this, SLOT(dockMain_closed()) );
    connect( _dockMap  , SIGNAL(closed()), this, SLOT(dockMap_closed())  );
    connect( _dockProp , SIGNAL(closed()), this, SLOT(dockProp_closed()) );

    settingsRead();

    setAircraftType( _dialogInit->getTypeIndex() );

    setStateIdle();
}

////////////////////////////////////////////////////////////////////////////////

MainWindow::~MainWindow()
{
    if ( _timerId ) killTimer( _timerId );

    settingsSave();

    DELPTR( _dialogConf );
    DELPTR( _dialogEnvr );
    DELPTR( _dialogInit );
    DELPTR( _dialogMass );

    DELPTR( _dockAuto );
    DELPTR( _dockCtrl );
    DELPTR( _dockData );
    DELPTR( _dockEFIS );
    DELPTR( _dockMain );
    DELPTR( _dockMap  );
    DELPTR( _dockProp );

    DELPTR( _widgetPFD );

    DELPTR( _scCycleViews );
    DELPTR( _scToggleHud  );
    DELPTR( _scFullScreen );
    DELPTR( _scTimeFaster );
    DELPTR( _scTimeSlower );

    DELPTR( _ui );
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::setup( Autopilot *ap, g1000::IFD *ifd )
{
    _ap = ap;

    _widgetPFD->setup( _ap, ifd );

    _dockAuto->setAutopilot( _ap );

}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::init()
{
    _dialogConf->readData();
    _dialogInit->readData();
    _dialogMass->readData();

    _dialogConf->updateAssignments();

    _widgetPFD->init();

    updateOutputData();

    _timerId = startTimer( 1000.0 * GUI_TIME_STEP );
}

////////////////////////////////////////////////////////////////////////////////

Aircrafts::Aircraft MainWindow::getCurrentAircraft() const
{
    return Aircrafts::instance()->getAircraft( _typeIndex );
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::keyPressEvent( QKeyEvent *event )
{
    ////////////////////////////////////
    QMainWindow::keyPressEvent( event );
    ////////////////////////////////////

    if ( !event->isAutoRepeat() )
    {
        _ui->widgetOTW->keyDn( Keys::getKey( event->key() ) );
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
        _ui->widgetOTW->keyUp( Keys::getKey( event->key() ) );
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::closeEvent( QCloseEvent *event )
{
    QString title = windowTitle();
    QString text = tr( "Do you really want to quit?" );

    QMessageBox::StandardButton result = QMessageBox::question( this, title, text,
                                                                QMessageBox::Yes | QMessageBox::No,
                                                                QMessageBox::No );

    if ( result == QMessageBox::Yes )
    {
        /////////////////////////////////
        QMainWindow::closeEvent( event );
        /////////////////////////////////

        _widgetPFD->close();
    }
    else
    {
        event->ignore();
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::timerEvent( QTimerEvent *event )
{
    /////////////////////////////////
    QMainWindow::timerEvent( event );
    /////////////////////////////////

    _stateOut = Data::get()->stateOut;

    if ( _stateOut == fdm::DataOut::Stopped )
    {
        if ( _stateInp != fdm::DataInp::Idle && _stateInp != fdm::DataInp::Stop )
        {
            setStateStop();
        }
    }

    updateDateTime();

    updateDockMain();
    updateDockData();
    updateDockCtrl();
    updateDockProp();
    updateDockEFIS();
    updateDockAuto();

    updateMenu();
    updateStatusBar();

    updateOutputData();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::flightRecordOpen()
{
    QString caption = tr( "Open flight..." );
    QString filter = tr( "CSV (*.csv)" );

    QString fileName = QFileDialog::getOpenFileName( this, caption, QDir::homePath(),
                                                     filter, &filter );

    if ( fileName.length() > 0 )
    {
        _rec_file = fileName;
        setStateInit();
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::flightRecordSave()
{
    QString caption = tr( "Save flight as..." );
    QString filter = tr( "CSV (*.csv)" );

    QString fileName = QFileDialog::getSaveFileName( this, caption, QDir::homePath(),
                                                     filter, &filter );

    if ( fileName.length() > 0 )
    {
        if ( !fileName.endsWith( ".csv" ) )
        {
            fileName += ".csv";
        }

        if ( !QFile::copy( _tmp_file, fileName ) )
        {
            QString title = windowTitle();
            QString text = tr( "Error while saving file!" );

            QMessageBox::critical( this, title, text, QMessageBox::Close );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::setStateIdle()
{
    _stateInp = fdm::DataInp::Idle;
    _dockMain->setStateInp( _stateInp );

    _rec_file = "";
    _timeCoef = 1.0;
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::setStateInit()
{
    _stateInp = fdm::DataInp::Init;
    _dockMain->setStateInp( _stateInp );
    _ap->init();

    ScreenSaver::disable();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::setStateWork()
{
    _stateInp = fdm::DataInp::Work;
    _dockMain->setStateInp( _stateInp );
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::setStateFreeze()
{
    _stateInp = fdm::DataInp::Freeze;
    _dockMain->setStateInp( _stateInp );
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::setStatePause()
{
    _stateInp = fdm::DataInp::Pause;
    _dockMain->setStateInp( _stateInp );
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::setStateStop()
{
    _stateInp = fdm::DataInp::Stop;
    _dockMain->setStateInp( _stateInp );
    _ap->stop();

    ScreenSaver::enable();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::setViewChase()
{
    _viewType = Data::CGI::ViewChase;
    _ui->stackedMain->setCurrentIndex( 1 );
    _ui->widgetOTW->setCameraManipulatorChase();
    _ui->widgetOTW->setDistanceDef( Aircrafts::instance()->getAircraft( _typeIndex ).distance_def );
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::setViewOrbit()
{
    _viewType = Data::CGI::ViewOrbit;
    _ui->stackedMain->setCurrentIndex( 1 );
    _ui->widgetOTW->setCameraManipulatorOrbit();
    _ui->widgetOTW->setDistanceDef( Aircrafts::instance()->getAircraft( _typeIndex ).distance_def );
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::setViewPilot()
{
    _viewType = Data::CGI::ViewPilot;
    _ui->stackedMain->setCurrentIndex( 1 );
    _ui->widgetOTW->setCameraManipulatorPilot();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::setViewWorld()
{
    _viewType = Data::CGI::ViewWorld;
    _ui->stackedMain->setCurrentIndex( 1 );
    _ui->widgetOTW->setCameraManipulatorWorld();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::setAircraftType( int typeIndex )
{
    _typeIndex = typeIndex;

    _dialogMass->setAircraftType( typeIndex );

    _dockCtrl->setAircraftType( typeIndex );
    _dockProp->setAircraftType( typeIndex );

    _ui->widgetOTW->setDistanceDef( Aircrafts::instance()->getAircraft( typeIndex ).distance_def );
    _ui->widgetOTW->setDistanceMin( Aircrafts::instance()->getAircraft( typeIndex ).distance_min );

    std::vector< double > notches =
            Aircrafts::instance()->getAircraft( typeIndex ).controls.notches.toVector().toStdVector();
    hid::Manager::instance()->setNotches( notches );
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::setSideDockAreasFullHeight( bool left, bool rght )
{
    if ( left )
    {
        setCorner( Qt::TopLeftCorner    , Qt::LeftDockWidgetArea );
        setCorner( Qt::BottomLeftCorner , Qt::LeftDockWidgetArea );
    }
    else
    {
        setCorner( Qt::TopLeftCorner    , Qt::TopDockWidgetArea    );
        setCorner( Qt::BottomLeftCorner , Qt::BottomDockWidgetArea );
    }

    if ( rght )
    {
        setCorner( Qt::TopRightCorner    , Qt::RightDockWidgetArea );
        setCorner( Qt::BottomRightCorner , Qt::RightDockWidgetArea );
    }
    else
    {
        setCorner( Qt::TopRightCorner    , Qt::TopDockWidgetArea    );
        setCorner( Qt::BottomRightCorner , Qt::BottomDockWidgetArea );
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::setWidescreenDockLayout( bool enabled )
{
    setSideDockAreasFullHeight( enabled, enabled );
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::settingsRead()
{
    QSettings settings( SIM_ORG_NAME, SIM_APP_NAME );
    
    settings.beginGroup( "main_window" );

    restoreState( settings.value( "state" ).toByteArray() );
    restoreGeometry( settings.value( "geometry" ).toByteArray() );

    bool visibleAuto = settings.value( "dock_auto_visible" , 0 ).toBool();
    bool visibleCtrl = settings.value( "dock_ctrl_visible" , 0 ).toBool();
    bool visibleData = settings.value( "dock_data_visible" , 0 ).toBool();
    bool visibleEFIS = settings.value( "dock_efis_visible" , 0 ).toBool();
    bool visibleMain = settings.value( "dock_main_visible" , 0 ).toBool();
    bool visibleMap  = settings.value( "dock_map_visible"  , 0 ).toBool();
    bool visibleProp = settings.value( "dock_prop_visible" , 0 ).toBool();

    _ui->actionDockAuto->setChecked( visibleAuto );
    _ui->actionDockCtrl->setChecked( visibleCtrl );
    _ui->actionDockData->setChecked( visibleData );
    _ui->actionDockEFIS->setChecked( visibleEFIS );
    _ui->actionDockMain->setChecked( visibleMain );
    _ui->actionDockMap->setChecked( visibleMap );
    _ui->actionDockProp->setChecked( visibleProp );

    settingsRead_Airport( settings );
    settingsRead_View( settings );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::settingsRead_Airport( QSettings &settings )
{
    settings.beginGroup( "airport" );

    bool lightsHELI = settings.value( "lightsHELI", 1 ).toBool();
    bool lightsRALS = settings.value( "lightsRALS", 1 ).toBool();
    bool lightsRCLS = settings.value( "lightsRCLS", 1 ).toBool();
    bool lightsRELS = settings.value( "lightsRELS", 1 ).toBool();
    bool lightsTDZL = settings.value( "lightsTDZL", 1 ).toBool();
    bool lightsTELS = settings.value( "lightsTELS", 1 ).toBool();
    bool lightsTWRL = settings.value( "lightsTWRL", 1 ).toBool();
    bool lightsVGSI = settings.value( "lightsVGSI", 1 ).toBool();
    bool gatesRwy18 = settings.value( "gatesRwy18", 0 ).toBool();
    bool gatesRwy36 = settings.value( "gatesRwy36", 0 ).toBool();

    _ui->actionAirportLightsHELI->setChecked( lightsHELI );
    _ui->actionAirportLightsRALS->setChecked( lightsRALS );
    _ui->actionAirportLightsRCLS->setChecked( lightsRCLS );
    _ui->actionAirportLightsRELS->setChecked( lightsRELS );
    _ui->actionAirportLightsTDZL->setChecked( lightsTDZL );
    _ui->actionAirportLightsTELS->setChecked( lightsTELS );
    _ui->actionAirportLightsTWRL->setChecked( lightsTWRL );
    _ui->actionAirportLightsVGSI->setChecked( lightsVGSI );
    _ui->actionAirportGatesRwy18->setChecked( gatesRwy18 );
    _ui->actionAirportGatesRwy36->setChecked( gatesRwy36 );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::settingsRead_View( QSettings &settings )
{
    settings.beginGroup( "view" );

    int viewType = settings.value( "view_type", Data::CGI::ViewPilot ).toInt();

    ////////////////////////////////////////////////////////////////////////
    if ( viewType == Data::CGI::ViewWorld ) viewType = Data::CGI::ViewOrbit;
    ////////////////////////////////////////////////////////////////////////

    switch ( viewType )
    {
    case Data::CGI::ViewChase:
        _viewType = Data::CGI::ViewChase;
        _ui->widgetOTW->setCameraManipulatorChase();
        break;

    default:
    case Data::CGI::ViewPilot:
        _viewType = Data::CGI::ViewPilot;
        _ui->widgetOTW->setCameraManipulatorPilot();
        break;

    case Data::CGI::ViewOrbit:
        _viewType = Data::CGI::ViewOrbit;
        _ui->widgetOTW->setCameraManipulatorOrbit();
        break;

    case Data::CGI::ViewWorld:
        _viewType = Data::CGI::ViewWorld;
        _ui->widgetOTW->setCameraManipulatorWorld();
        break;
    }

    _showHUD    = settings.value( "show_hud"    , 1 ).toBool();
    _showTraces = settings.value( "show_traces" , 0 ).toBool();

    _ui->actionShowHUD->setChecked( _showHUD );
    _ui->actionShowTraces->setChecked( _showTraces );

    {
        settings.beginGroup( "rotor" );


        _showRotorBlur   = settings.value( "show_rotor_blur"   , 0 ).toBool();
        _showBladesDatum = settings.value( "show_blades_datum" , 0 ).toBool();
        _showBladesPaths = settings.value( "show_blades_paths" , 0 ).toBool();

        _ui->actionShowRotorBlur->setChecked( _showRotorBlur );
        _ui->actionShowBladesDatum->setChecked( _showBladesDatum );
        _ui->actionShowBladesPaths->setChecked( _showBladesPaths );

        settings.endGroup();
    }

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::settingsSave()
{
    QSettings settings( SIM_ORG_NAME, SIM_APP_NAME );
    
    settings.beginGroup( "main_window" );

    settings.setValue( "state", saveState() );
    settings.setValue( "geometry", saveGeometry() );

    settingsSave_Airport( settings );
    settingsSave_View( settings );

    settings.setValue( "dock_auto_visible" , _ui->actionDockAuto->isChecked() ? 1 : 0 );
    settings.setValue( "dock_ctrl_visible" , _ui->actionDockCtrl->isChecked() ? 1 : 0 );
    settings.setValue( "dock_data_visible" , _ui->actionDockData->isChecked() ? 1 : 0 );
    settings.setValue( "dock_efis_visible" , _ui->actionDockEFIS->isChecked() ? 1 : 0 );
    settings.setValue( "dock_main_visible" , _ui->actionDockMain->isChecked() ? 1 : 0 );
    settings.setValue( "dock_map_visible"  , _ui->actionDockMap->isChecked()  ? 1 : 0 );
    settings.setValue( "dock_prop_visible" , _ui->actionDockProp->isChecked() ? 1 : 0 );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::settingsSave_Airport( QSettings &settings )
{
    settings.beginGroup( "airport" );

    settings.setValue( "lightsHELI", (int)_ui->actionAirportLightsHELI->isChecked() );
    settings.setValue( "lightsRALS", (int)_ui->actionAirportLightsRALS->isChecked() );
    settings.setValue( "lightsRCLS", (int)_ui->actionAirportLightsRCLS->isChecked() );
    settings.setValue( "lightsRELS", (int)_ui->actionAirportLightsRELS->isChecked() );
    settings.setValue( "lightsTDZL", (int)_ui->actionAirportLightsTDZL->isChecked() );
    settings.setValue( "lightsTELS", (int)_ui->actionAirportLightsTELS->isChecked() );
    settings.setValue( "lightsTWRL", (int)_ui->actionAirportLightsTWRL->isChecked() );
    settings.setValue( "lightsVGSI", (int)_ui->actionAirportLightsVGSI->isChecked() );
    settings.setValue( "gatesRwy18", (int)_ui->actionAirportGatesRwy18->isChecked() );
    settings.setValue( "gatesRwy36", (int)_ui->actionAirportGatesRwy36->isChecked() );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::settingsSave_View( QSettings &settings )
{
    settings.beginGroup( "view" );

    settings.setValue( "view_type"   , (int)_viewType   );
    settings.setValue( "show_hud"    , (int)_showHUD    );
    settings.setValue( "show_traces" , (int)_showTraces );

    {
        settings.beginGroup( "rotor" );

        settings.setValue( "show_rotor_blur"   , (int)_showRotorBlur   );
        settings.setValue( "show_blades_datum" , (int)_showBladesDatum );
        settings.setValue( "show_blades_paths" , (int)_showBladesPaths );

        settings.endGroup();
    }

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::updateDateTime()
{
    if ( _stateOut == fdm::DataOut::Idle )
    {
        _dateTime = _dialogInit->getDateTime();
        _flightTime.setHMS( 0, 0, 0 );
    }

    if ( _stateOut == fdm::DataOut::Working )
    {
        double timeStep_ms = _timeCoef * 1000.0 * GUI_TIME_STEP;
        _dateTime   = _dateTime.addMSecs( timeStep_ms );
        _flightTime = _flightTime.addMSecs( timeStep_ms );
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::updateDockAuto() {}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::updateDockCtrl()
{
    if ( _dockCtrl->isVisible() )
    {
        _dockCtrl->setCollective( hid::Manager::instance()->getCollective() );

        _dockCtrl->setCtrlStick( hid::Manager::instance()->getCtrlRoll(),
                                 hid::Manager::instance()->getCtrlPitch() );

        _dockCtrl->setTrimStick( hid::Manager::instance()->getTrimRoll(),
                                 hid::Manager::instance()->getTrimPitch() );

        _dockCtrl->setCtrlPedals( hid::Manager::instance()->getCtrlYaw() );
        _dockCtrl->setTrimPedals( hid::Manager::instance()->getTrimYaw() );

        _dockCtrl->setBrakes( Data::get()->controls.brake_l,
                              Data::get()->controls.brake_r );

        _dockCtrl->setGear( Data::get()->controls.landing_gear );
        _dockCtrl->setFlaps( Data::get()->controls.flaps );

        _dockCtrl->setThrottle( Data::get()->propulsion.engine[ 0 ].throttle,
                                Data::get()->propulsion.engine[ 1 ].throttle,
                                Data::get()->propulsion.engine[ 2 ].throttle,
                                Data::get()->propulsion.engine[ 3 ].throttle );

        _dockCtrl->setMixture( Data::get()->propulsion.engine[ 0 ].mixture,
                               Data::get()->propulsion.engine[ 1 ].mixture,
                               Data::get()->propulsion.engine[ 2 ].mixture,
                               Data::get()->propulsion.engine[ 3 ].mixture );

        _dockCtrl->setPropeller( Data::get()->propulsion.engine[ 0 ].propeller,
                                 Data::get()->propulsion.engine[ 1 ].propeller,
                                 Data::get()->propulsion.engine[ 2 ].propeller,
                                 Data::get()->propulsion.engine[ 3 ].propeller );

        _dockCtrl->setAirbrake( Data::get()->controls.airbrake );
        _dockCtrl->setSpoilers( Data::get()->controls.spoilers );
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::updateDockData()
{
    if ( _dockData->isVisible() )
    {
        if ( _stateOut == fdm::DataOut::Idle )
        {
            _dockData->reset();
        }
        else
        {
            _dockData->setAltitudeASL( Data::get()->ownship.altitude_asl );
            _dockData->setAltitudeAGL( Data::get()->ownship.altitude_agl );

            _dockData->setAirspeed( Data::get()->ownship.airspeed );
            _dockData->setIAS( Data::get()->ownship.ias );
            _dockData->setClimbRate( Data::get()->ownship.climbRate );
            _dockData->setMachNumber( Data::get()->ownship.machNumber );

            _dockData->setRollAngle( Data::get()->ownship.roll );
            _dockData->setPitchAngle( Data::get()->ownship.pitch );
            _dockData->setHeading( Data::get()->ownship.heading );

            _dockData->setAngleOfAttack( Data::get()->ownship.angleOfAttack );
            _dockData->setSideslipAngle( Data::get()->ownship.sideslipAngle );
            _dockData->setClimbAngle( Data::get()->ownship.climbAngle );
            _dockData->setTrackAngle( Data::get()->ownship.trackAngle );

            _dockData->setRollRate( Data::get()->ownship.rollRate );
            _dockData->setPitchRate( Data::get()->ownship.pitchRate );
            _dockData->setYawRate( Data::get()->ownship.yawRate );
            _dockData->setTurnRate( Data::get()->ownship.turnRate );

            _dockData->setGx( Data::get()->ownship.g_force_x );
            _dockData->setGy( Data::get()->ownship.g_force_y );
            _dockData->setGz( Data::get()->ownship.g_force_z );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::updateDockEFIS()
{
    if ( _dockEFIS->isVisible() )
    {
        const double coef_alt = fdm::Units::m2ft();
        const double coef_ias = fdm::Units::mps2kts();
        const double coef_vsi = fdm::Units::mps2fpm();

        _dockEFIS->setRoll( fdm::Units::rad2deg( Data::get()->ownship.roll ) );
        _dockEFIS->setPitch( fdm::Units::rad2deg( Data::get()->ownship.pitch ) );

        _dockEFIS->setSlipSkid( Data::get()->ownship.slipSkidAngle / fdm::Units::deg2rad( 9.0 ) );
        _dockEFIS->setTurnRate( Data::get()->ownship.turnRate      / fdm::Units::deg2rad( 6.0 ) );

        _dockEFIS->setDots( Data::get()->navigation.ils_lc_norm,
                            Data::get()->navigation.ils_gs_norm,
                            Data::get()->navigation.ils_lc_visible,
                            Data::get()->navigation.ils_gs_visible );

        _dockEFIS->setFD( fdm::Units::rad2deg( _ap->getCmdRoll() ),
                          fdm::Units::rad2deg( _ap->getCmdPitch() ),
                          _ap->isActiveFD() );

        _dockEFIS->setStall( Data::get()->ownship.stall );

        _dockEFIS->setAltitude( coef_alt * Data::get()->ownship.altitude_asl );
        _dockEFIS->setAirspeed( coef_ias * Data::get()->ownship.ias );

        _dockEFIS->setMachNo( Data::get()->ownship.machNumber );

        _dockEFIS->setHeading( fdm::Units::rad2deg( Data::get()->ownship.heading ) );

        _dockEFIS->setClimbRate( coef_vsi * Data::get()->ownship.climbRate / 1000.0 );

        _dockEFIS->setCourse( _dockAuto->getCourse() );

        GraphicsEHSI::CDI cdi = GraphicsEHSI::NONE;

        if ( Data::get()->navigation.nav_cdi == Data::Navigation::TO   ) cdi = GraphicsEHSI::TO;
        if ( Data::get()->navigation.nav_cdi == Data::Navigation::FROM ) cdi = GraphicsEHSI::FROM;

        _dockEFIS->setDistance( fdm::Units::m2nmi( Data::get()->navigation.dme_distance ),
                                Data::get()->navigation.dme_visible );
        _dockEFIS->setBearing( fdm::Units::rad2deg( Data::get()->navigation.adf_bearing ),
                               Data::get()->navigation.adf_visible );
        _dockEFIS->setDeviation( Data::get()->navigation.nav_norm, cdi );

        GraphicsEADI::FlightMode flightMode = GraphicsEADI::FM_OFF;
        if ( _ap->isActiveFD() )
        {
            flightMode = GraphicsEADI::FM_FD;

            if ( _ap->isActiveAP() ) flightMode = GraphicsEADI::FM_CMD;
        }
        _dockEFIS->setFlightMode( flightMode );

        GraphicsEADI::LNAV lnav = GraphicsEADI::LNAV_OFF;
        if      ( _ap->isActiveHDG() ) lnav = GraphicsEADI::LNAV_HDG;
        else if ( _ap->isActiveNAV() ) lnav = GraphicsEADI::LNAV_NAV;
        else if ( _ap->isActiveAPR() ) lnav = GraphicsEADI::LNAV_APR;
        else if ( _ap->isActiveBC()  ) lnav = GraphicsEADI::LNAV_BC;
        _dockEFIS->setLNAV( lnav );

        GraphicsEADI::VNAV vnav = GraphicsEADI::VNAV_OFF;
        if      ( _ap->isActiveALT() ) vnav = GraphicsEADI::VNAV_ALT;
        else if ( _ap->isActiveIAS() ) vnav = GraphicsEADI::VNAV_IAS;
        else if ( _ap->isActiveVS()  ) vnav = GraphicsEADI::VNAV_VS;
        else if ( _ap->isActiveARM() ) vnav = GraphicsEADI::VNAV_ALT_SEL;
        else if ( _ap->isActiveGS()  ) vnav = GraphicsEADI::VNAV_GS;
        _dockEFIS->setVNAV( vnav );

        _dockEFIS->setAirspeedSel( coef_ias * _ap->getAirspeed() );
        _dockEFIS->setAltitudeSel( coef_alt * _ap->getAltitude() );
        _dockEFIS->setHeadingSel( fdm::Units::rad2deg( _ap->getHeading() ) );

        _dockEFIS->setVfe( coef_ias * Aircrafts::instance()->getAircraft( _dialogInit->getTypeIndex() ).vfe );
        _dockEFIS->setVne( coef_ias * Aircrafts::instance()->getAircraft( _dialogInit->getTypeIndex() ).vne );
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::updateDockMain()
{
    if ( _dockMain->isVisible() )
    {
        _dockMain->setStateInp( _stateInp );
        _dockMain->setStateOut( _stateOut );

        if ( _stateOut == fdm::DataOut::Working )
        {
            _dockMain->setTimeStep( Data::get()->timeStep );
        }
        else
        {
            _dockMain->setTimeStep( FDM_TIME_STEP );
        }

        _dockMain->setFlightTime( _flightTime );
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::updateDockProp()
{
    if ( _dockProp->isVisible() )
    {
        for ( unsigned int i = 0; i < FDM_MAX_ENGINES; i++ )
        {
            _dockProp->setState ( i, Data::get()->propulsion.engine[ i ].state );
            _dockProp->setAB    ( i, Data::get()->propulsion.engine[ i ].afterburner );
            _dockProp->setRPM   ( i, M_PI * Data::get()->propulsion.engine[ i ].rpm  / 30.0f );
            _dockProp->setPROP  ( i, M_PI * Data::get()->propulsion.engine[ i ].prop / 30.0f );
            _dockProp->setNG    ( i, Data::get()->propulsion.engine[ i ].ng  / 100.0f );
            _dockProp->setN1    ( i, Data::get()->propulsion.engine[ i ].n1  / 100.0f );
            _dockProp->setN2    ( i, Data::get()->propulsion.engine[ i ].n2  / 100.0f );
            _dockProp->setTRQ   ( i, Data::get()->propulsion.engine[ i ].trq / 100.0f );
            _dockProp->setEPR   ( i, Data::get()->propulsion.engine[ i ].epr );
            _dockProp->setMAP   ( i, Data::get()->propulsion.engine[ i ].map );
            _dockProp->setEGT   ( i, fdm::Units::c2k( Data::get()->propulsion.engine[ i ].egt ) );
            _dockProp->setTIT   ( i, fdm::Units::c2k( Data::get()->propulsion.engine[ i ].tit ) );
            _dockProp->setTOT   ( i, fdm::Units::c2k( Data::get()->propulsion.engine[ i ].egt ) );
            _dockProp->setITT   ( i, fdm::Units::c2k( Data::get()->propulsion.engine[ i ].itt ) );
            _dockProp->setFF    ( i, Data::get()->propulsion.engine[ i ].fuelFlow );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::updateMenu()
{
    switch ( _stateOut )
    {
    default:
    case fdm::DataOut::Idle:
        _ui->actionStateInpIdle   ->setEnabled( true  );
        _ui->actionStateInpInit   ->setEnabled( true  );
        _ui->actionStateInpWork   ->setEnabled( false );
        _ui->actionStateInpFreeze ->setEnabled( false );
        _ui->actionStateInpPause  ->setEnabled( false );
        _ui->actionStateInpStop   ->setEnabled( false );
        break;

    case fdm::DataOut::Ready:
        _ui->actionStateInpIdle   ->setEnabled( false );
        _ui->actionStateInpInit   ->setEnabled( true  );
        _ui->actionStateInpWork   ->setEnabled( true  );
        _ui->actionStateInpFreeze ->setEnabled( true  );
        _ui->actionStateInpPause  ->setEnabled( true  );
        _ui->actionStateInpStop   ->setEnabled( true  );
        break;

    case fdm::DataOut::Working:
        _ui->actionStateInpIdle   ->setEnabled( false );
        _ui->actionStateInpInit   ->setEnabled( false );
        _ui->actionStateInpWork   ->setEnabled( true  );
        _ui->actionStateInpFreeze ->setEnabled( true  );
        _ui->actionStateInpPause  ->setEnabled( true  );
        _ui->actionStateInpStop   ->setEnabled( true  );
        break;

    case fdm::DataOut::Frozen:
        _ui->actionStateInpIdle   ->setEnabled( false );
        _ui->actionStateInpInit   ->setEnabled( false );
        _ui->actionStateInpWork   ->setEnabled( true  );
        _ui->actionStateInpFreeze ->setEnabled( true  );
        _ui->actionStateInpPause  ->setEnabled( true  );
        _ui->actionStateInpStop   ->setEnabled( true  );
        break;

    case fdm::DataOut::Paused:
        _ui->actionStateInpIdle   ->setEnabled( false );
        _ui->actionStateInpInit   ->setEnabled( false );
        _ui->actionStateInpWork   ->setEnabled( true  );
        _ui->actionStateInpFreeze ->setEnabled( true  );
        _ui->actionStateInpPause  ->setEnabled( true  );
        _ui->actionStateInpStop   ->setEnabled( true  );
        break;

    case fdm::DataOut::Stopped:
        _ui->actionStateInpIdle   ->setEnabled( true  );
        _ui->actionStateInpInit   ->setEnabled( false );
        _ui->actionStateInpWork   ->setEnabled( false );
        _ui->actionStateInpFreeze ->setEnabled( false );
        _ui->actionStateInpPause  ->setEnabled( false );
        _ui->actionStateInpStop   ->setEnabled( true  );
        break;
    }

    if ( _stateInp == fdm::DataInp::Idle && _stateOut == fdm::DataOut::Idle )
    {
        _ui->actionDialogInit->setEnabled( true );
        _ui->actionDialogMass->setEnabled( true );

        _ui->actionFlightOpen->setEnabled( true );
    }
    else
    {
        _ui->actionDialogInit->setEnabled( false );
        _ui->actionDialogMass->setEnabled( false );

        _ui->actionFlightOpen->setEnabled( false );
    }

    if ( _stateInp == fdm::DataInp::Stop && _stateOut == fdm::DataOut::Stopped && _rec_file.length() == 0 )
    {
        _ui->actionFlightSave->setEnabled( true );
    }
    else
    {
        _ui->actionFlightSave->setEnabled( false );
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::updateStatusBar()
{
    double frameRate = 1.0 / FDM_TIME_STEP;

    if ( _stateOut == fdm::DataOut::Working || _stateOut == fdm::DataOut::Frozen )
    {
        frameRate = 1.0 / Data::get()->timeStep;
    }

    QString text = "";

    text += tr( "Time Coef: " ) + QString::number( _timeCoef, 'd', 1 );
    text += "   ";
    text += tr( "Frame Rate: " ) + QString::number( frameRate, 'd', 2 );

    _ui->statusBar->showMessage( text );
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::updateOutputData()
{
    Aircrafts::Aircraft aircraft = Aircrafts::instance()->getAircraft( _dialogInit->getTypeIndex() );

    // CGI - airport
    Data::get()->cgi.airport.lightsHELI = _ui->actionAirportLightsHELI->isChecked();
    Data::get()->cgi.airport.lightsRALS = _ui->actionAirportLightsRALS->isChecked();
    Data::get()->cgi.airport.lightsRCLS = _ui->actionAirportLightsRCLS->isChecked();
    Data::get()->cgi.airport.lightsRELS = _ui->actionAirportLightsRELS->isChecked();
    Data::get()->cgi.airport.lightsTDZL = _ui->actionAirportLightsTDZL->isChecked();
    Data::get()->cgi.airport.lightsTELS = _ui->actionAirportLightsTELS->isChecked();
    Data::get()->cgi.airport.lightsTWRL = _ui->actionAirportLightsTWRL->isChecked();
    Data::get()->cgi.airport.lightsVGSI = _ui->actionAirportLightsVGSI->isChecked();
    Data::get()->cgi.airport.gatesRwy18 = _ui->actionAirportGatesRwy18->isChecked();
    Data::get()->cgi.airport.gatesRwy36 = _ui->actionAirportGatesRwy36->isChecked();

    // CGI - camera
    Data::get()->cgi.camera.offset_x = aircraft.offset_x;
    Data::get()->cgi.camera.offset_y = aircraft.offset_y;
    Data::get()->cgi.camera.offset_z = aircraft.offset_z;

    // CGI - environment
    Data::get()->cgi.environment.clouds.type = _dialogEnvr->getCloudsType();
    if ( _dialogEnvr->getCloudsType() == Data::CGI::Environment::Clouds::Block )
    {
        Data::get()->cgi.environment.clouds.data.block = _dialogEnvr->getBlockClouds();
    }
    else if ( _dialogEnvr->getCloudsType() == Data::CGI::Environment::Clouds::Layer )
    {
        Data::get()->cgi.environment.clouds.data.layer = _dialogEnvr->getLayerClouds();
    }

    Data::get()->cgi.environment.visibility = _dialogEnvr->getVisibility();

    // CGI - HUD
    Data::get()->cgi.hud.enabled = _showHUD;
    Data::get()->cgi.hud.color_r = _dialogConf->getHudColorR();
    Data::get()->cgi.hud.color_g = _dialogConf->getHudColorG();
    Data::get()->cgi.hud.color_b = _dialogConf->getHudColorB();
    Data::get()->cgi.hud.opacity = (double)_dialogConf->getHudOpacity() / 100.0;
    Data::get()->cgi.hud.factor_alt = (double)_dialogConf->getHudFactorAlt();
    Data::get()->cgi.hud.factor_vel = (double)_dialogConf->getHudFactorVel();

    // CGI - show
    Data::get()->cgi.show_traces = _showTraces;

    // CGI - show - rotor
    Data::get()->cgi.show_rotor_blur = _showRotorBlur;
    Data::get()->cgi.show_blades_datum = _showBladesDatum;
    Data::get()->cgi.show_blades_paths = _showBladesPaths;

    // date time
    Data::get()->dateTime.year   = (unsigned short)_dateTime.date().year();
    Data::get()->dateTime.month  = (unsigned short)_dateTime.date().month();
    Data::get()->dateTime.day    = (unsigned short)_dateTime.date().day();
    Data::get()->dateTime.hour   = (unsigned short)_dateTime.time().hour();
    Data::get()->dateTime.minute = (unsigned short)_dateTime.time().minute();
    Data::get()->dateTime.second = (unsigned short)_dateTime.time().second();

    // environment
    Data::get()->environment.temperature_0  = _dialogEnvr->getTemperatureSL();
    Data::get()->environment.pressure_0     = _dialogEnvr->getPressureSL();
    Data::get()->environment.wind_direction = _dialogEnvr->getWindDirection();
    Data::get()->environment.wind_speed     = _dialogEnvr->getWindSpeed();
    Data::get()->environment.turbulence     = _dialogEnvr->getTurbulence();
    Data::get()->environment.windShear      = _dialogEnvr->getWindShear();

    // initial conditions
    Data::get()->initial.latitude     = _dialogInit->getLat();
    Data::get()->initial.longitude    = _dialogInit->getLon();
    Data::get()->initial.altitude_agl = _dialogInit->getAlt();
    Data::get()->initial.offset_x     = 0.0; // TODO
    Data::get()->initial.offset_y     = 0.0; // TODO
    Data::get()->initial.heading      = _dialogInit->getPsi();
    Data::get()->initial.airspeed     = _dialogInit->getIAS();
    Data::get()->initial.engineOn     = _dialogInit->getEngine();

    // ownship
    strcpy( Data::get()->ownship.aircraftFile, aircraft.file.toStdString().c_str() );

    Data::get()->ownship.mainRotor.coef = aircraft.mainRotorCoef;
    Data::get()->ownship.tailRotor.coef = aircraft.tailRotorCoef;

    // propulsion
    for ( unsigned int i = 0; i < FDM_MAX_ENGINES; i++ )
    {
        Data::get()->propulsion.engine[ i ].fuel     = _dockProp->getFuel( i );
        Data::get()->propulsion.engine[ i ].ignition = _dockProp->getIgnition( i );
        Data::get()->propulsion.engine[ i ].starter  = _dockProp->getStarter( i );
    }

    // masses
    Data::get()->masses.pilot[ 0 ] = _dialogMass->getPilot1();
    Data::get()->masses.pilot[ 1 ] = _dialogMass->getPilot2();
    Data::get()->masses.tank[ 0 ]  = _dialogMass->getFuelTank1();
    Data::get()->masses.tank[ 1 ]  = _dialogMass->getFuelTank2();
    Data::get()->masses.tank[ 2 ]  = _dialogMass->getFuelTank3();
    Data::get()->masses.tank[ 3 ]  = _dialogMass->getFuelTank4();
    Data::get()->masses.tank[ 4 ]  = _dialogMass->getFuelTank5();
    Data::get()->masses.tank[ 5 ]  = _dialogMass->getFuelTank6();
    Data::get()->masses.tank[ 6 ]  = _dialogMass->getFuelTank7();
    Data::get()->masses.tank[ 7 ]  = _dialogMass->getFuelTank8();
    Data::get()->masses.cabin      = _dialogMass->getCabin();
    Data::get()->masses.trunk      = _dialogMass->getTrunk();
    Data::get()->masses.slung      = 0.0;

    // recording
    if ( _rec_file.length() > 0 )
    {
        Data::get()->recording.mode = fdm::DataInp::Recording::Replay;
        strncpy( Data::get()->recording.file, _rec_file.toLocal8Bit().data(), 4095 );
    }
    else
    {
        Data::get()->recording.mode = fdm::DataInp::Recording::Record;
        strncpy( Data::get()->recording.file, _tmp_file.toLocal8Bit().data(), 4095 );
    }

    Data::get()->sfx.volume = (double)_dialogConf->getSoundVolume() / 100.0;

    // aircraft type
    Data::get()->aircraftType = (fdm::DataInp::AircraftType)aircraft.type;

    // FDM (input state)
    Data::get()->stateInp = _stateInp;

    // time coefficient
    Data::get()->timeCoef = _timeCoef;
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionDialogConf_triggered()
{
    _dialogConf->readData();

    if ( _dialogConf->exec() == QDialog::Accepted )
    {
        _dialogConf->saveData();
        _dialogConf->updateAssignments();
        setWidescreenDockLayout( _dialogConf->getWidescreen() );
    }
    else
    {
        _dialogConf->readData();
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionDialogEnvr_triggered()
{
    _dialogEnvr->readData();

    if ( _dialogEnvr->exec() == QDialog::Accepted )
    {
        _dialogEnvr->saveData();
    }
    else
    {
        _dialogEnvr->readData();
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionDialogInit_triggered()
{
    _dialogInit->readData();

    if ( _dialogInit->exec() == QDialog::Accepted )
    {
        _dialogInit->saveData();
    }
    else
    {
        _dialogInit->readData();
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionDialogMass_triggered()
{
    _dialogMass->readData();

    if ( _dialogMass->exec() == QDialog::Accepted )
    {
        _dialogMass->saveData();
    }
    else
    {
        _dialogMass->readData();
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionDockAuto_toggled( bool checked )
{
    _dockAuto->setVisible( checked );
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

void MainWindow::on_actionDockEFIS_toggled( bool checked )
{
    _dockEFIS->setVisible( checked );
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionDockMain_toggled( bool checked )
{
    _dockMain->setVisible( checked );
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionDockMap_toggled( bool checked )
{
    _dockMap->setVisible( checked );
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionDockProp_toggled( bool checked )
{
    _dockProp->setVisible( checked );
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionShowPFD_triggered()
{
    _widgetPFD->show();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionFlightOpen_triggered()
{
    flightRecordOpen();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionFlightSave_triggered()
{
    flightRecordSave();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionStateInpIdle_triggered()
{
    setStateIdle();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionStateInpInit_triggered()
{
    setStateInit();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionStateInpWork_triggered()
{
    setStateWork();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionStateInpFreeze_triggered()
{
    setStateFreeze();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionStateInpPause_triggered()
{
    setStatePause();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionStateInpStop_triggered()
{
    setStateStop();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionQuit_triggered()
{
    close();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionViewChase_triggered()
{
    setViewChase();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionViewOrbit_triggered()
{
    setViewOrbit();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionViewPilot_triggered()
{
    setViewPilot();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionViewWorld_triggered()
{
    setViewWorld();
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionShowHUD_triggered( bool checked )
{
    _showHUD = checked;
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionShowTraces_triggered( bool checked )
{
    _showTraces = checked;
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionShowRotorBlur_triggered( bool checked )
{
    _showRotorBlur = checked;
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionShowBladesDatum_triggered( bool checked )
{
    _showBladesDatum = checked;
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionShowBladesPaths_triggered( bool checked )
{
    _showBladesPaths = checked;
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionTimeFaster_triggered()
{
    int timeCoef10 = floor( 10.0 * _timeCoef + 0.5 );

    if ( timeCoef10 > 9 )
    {
        if ( timeCoef10 < 20 )
        {
            timeCoef10 += 5;
        }
        else if ( timeCoef10 < 100 )
        {
            timeCoef10 += 10;
        }
        else
        {
            timeCoef10 = 100;
        }
    }
    else
    {
        timeCoef10 += 1;
    }

    if ( timeCoef10 > 100 ) timeCoef10 = 100;

    _timeCoef = 0.1 * (double)timeCoef10;
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_actionTimeSlower_triggered()
{
    int timeCoef10 = floor( 10.0 * _timeCoef + 0.5 );

    if ( timeCoef10 > 20 )
    {
        timeCoef10 -= 10;
    }
    else if ( timeCoef10 > 10 )
    {
        timeCoef10 -= 5;
    }
    else
    {
        if ( timeCoef10 > 1 )
        {
            timeCoef10 -= 1;
        }
        else
        {
            timeCoef10 = 0;
        }
    }

    if ( timeCoef10 < 1 ) timeCoef10 = 1;

    _timeCoef = 0.1 * (double)timeCoef10;
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::shorcutCycleViews_activated()
{
    ViewType viewType = Data::CGI::ViewChase;

    if ( _viewType + 1 < Data::CGI::ViewWorld ) // sic!
    {
        viewType = (ViewType)( _viewType + 1 );
    }

    switch ( viewType )
    {
        case Data::CGI::ViewChase: setViewChase(); break;
        case Data::CGI::ViewOrbit: setViewOrbit(); break;
        case Data::CGI::ViewPilot: setViewPilot(); break;
        case Data::CGI::ViewWorld: setViewWorld(); break;
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::shorcutToggleHud_activated()
{
    _showHUD = !_showHUD;
    _ui->actionShowHUD->setChecked( _showHUD );
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::shorcutFullScreen_activated()
{
    if ( isFullScreen() )
    {
        showNormal();
        _ui->menuBar->show();
        _ui->statusBar->show();
    }
    else
    {
        showFullScreen();
        _ui->menuBar->hide();
        _ui->statusBar->hide();
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::dialogInit_typeIndexChanged( int typeIndex )
{
    setAircraftType( typeIndex );
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::dockMain_stateInpChanged( fdm::DataInp::StateInp stateInp )
{
    switch ( stateInp )
    {
        case fdm::DataInp::Idle:   setStateIdle();   break;
        case fdm::DataInp::Init:   setStateInit();   break;
        case fdm::DataInp::Work:   setStateWork();   break;
        case fdm::DataInp::Freeze: setStateFreeze(); break;
        case fdm::DataInp::Pause:  setStatePause();  break;
        case fdm::DataInp::Stop:   setStateStop();   break;
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::dockAuto_closed()
{
    _ui->actionDockAuto->setChecked( false );
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

void MainWindow::dockEFIS_closed()
{
    _ui->actionDockEFIS->setChecked( false );
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::dockMain_closed()
{
    _ui->actionDockMain->setChecked( false );
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::dockMap_closed()
{
    _ui->actionDockMap->setChecked( false );
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::dockProp_closed()
{
    _ui->actionDockProp->setChecked( false );
}
