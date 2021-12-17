/****************************************************************************//*
 * Copyright (C) 2021 Marek M. Cel
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

#include <Manager.h>

#include <QApplication>
#include <QSplashScreen>

#include <Data.h>

#include <fdm/utils/fdm_Units.h>

#include <gui/Aircrafts.h>

#include <hid/hid_AxisTune.h>
#include <hid/hid_Manager.h>

#include <sim/Log.h>

////////////////////////////////////////////////////////////////////////////////

Data::DataBuf Data::_data;

////////////////////////////////////////////////////////////////////////////////

Manager::Manager() :
    QObject( NULLPTR ),

    _ap  ( NULLPTR ),
    _nav ( NULLPTR ),
    _sfx ( NULLPTR ),
    _sim ( NULLPTR ),
    _win ( NULLPTR ),

    _g1000_ifd ( NULLPTR ),

    _timerSim ( NULLPTR ),
    _timerOut ( NULLPTR ),

    _timeStep ( 0.0 ),

    _timerId ( 0 )
{
    _ap  = new Autopilot();
    _nav = new nav::Manager();
    _sfx = new sfx::Thread();
    _sim = new Simulation();
    _win = new MainWindow();

    _g1000_ifd = new g1000::IFD();
    memset( &_g1000_input, 0, sizeof( g1000::Input ) );

    _timerSim = new QElapsedTimer();
    _timerOut = new QElapsedTimer();
}

////////////////////////////////////////////////////////////////////////////////

Manager::~Manager()
{
    if ( _timerId != 0 ) killTimer( _timerId );

    if ( _sfx )
    {
        while ( _sfx->isRunning() )
        {
            _sfx->quit();
        }
    }

    if ( _sim )
    {
        while ( _sim->isRunning() )
        {
            _sim->quit();
        }
    }

    DELPTR( _ap  );
    DELPTR( _nav );
    DELPTR( _sfx );
    DELPTR( _sim );
    DELPTR( _win );

    DELPTR( _g1000_ifd );

    DELPTR( _timerSim );
    DELPTR( _timerOut );
}

////////////////////////////////////////////////////////////////////////////////

void Manager::init()
{
    QPixmap pixmap(":/gui/images/splashscreen.png");
    QSplashScreen splash( pixmap );
    splash.show();
    splash.showMessage( QString( SIM_APP_NAME ) + " " + QString( SIM_APP_VER ) );
    qApp->processEvents();
#   ifdef SIM_TEST
    QTimer::singleShot(  500, &splash, SLOT(close()) );
#   else
    QTimer::singleShot( 2000, &splash, SLOT(close()) );
#   endif
    while ( splash.isVisible() )
    {
        qApp->processEvents();
    }

    qRegisterMetaType< Data::DataBuf >( "Data::DataBuf" );
    qRegisterMetaType< fdm::DataOut  >( "fdm::DataOut"  );

    connect( this, SIGNAL(dataInpUpdated(const Data::DataBuf*)), _sim, SLOT(onDataInpUpdated(const Data::DataBuf*)), Qt::QueuedConnection );
    connect( this, SIGNAL(dataInpUpdated(const Data::DataBuf*)), _sfx, SLOT(onDataInpUpdated(const Data::DataBuf*)), Qt::QueuedConnection );

    connect( _sim, SIGNAL(dataOutUpdated(const fdm::DataOut&)), this, SLOT(onDataOutUpdated(const fdm::DataOut&)), Qt::QueuedConnection );

    hid::Manager::instance()->init();

    _sfx->init();
    _sim->init();

    _win->setup( _ap, _g1000_ifd );
    _win->show();
    splash.finish( _win );
    //qApp->processEvents();

    _win->init();

    _timerId = startTimer( 1000.0 * FDM_TIME_STEP );

    _timerSim->start();
    _timerOut->start();
}

////////////////////////////////////////////////////////////////////////////////

void Manager::timerEvent( QTimerEvent *event )
{
    /////////////////////////////
    QObject::timerEvent( event );
    /////////////////////////////

    _timeStep = Data::get()->timeCoef * (double)_timerSim->restart() / 1000.0;

    if ( Data::get()->stateInp == fdm::DataInp::Idle )
    {
        hid::Manager::instance()->reset( Data::get()->initial.altitude_agl < FDM_MIN_INIT_ALTITUDE );
    }
    else if ( Data::get()->stateInp == fdm::DataInp::Work )
    {
        // TODO
    }

    hid::Manager::instance()->update( _timeStep );

    _ap->update( _timeStep );

    updatedInputG1000();
    _g1000_ifd->update( _timeStep, _g1000_input );

    _nav->setCourse( fdm::Units::deg2rad( _win->getCourse() ) );
    _nav->setFreqNAV( 1000 * _win->getFreqNav() );
    _nav->update();

    // controls
    double r_rv = hid::Manager::instance()->getCtrlRoll();
    double p_rv = hid::Manager::instance()->getCtrlPitch();
    double y_rv = hid::Manager::instance()->getCtrlYaw();

    Aircrafts::Aircraft aircraft = _win->getCurrentAircraft();

    double r_d = aircraft.axes.roll.deadzone;
    double r_c = aircraft.axes.roll.curvature;
    double r_s = aircraft.axes.roll.scale;

    double p_d = aircraft.axes.pitch.deadzone;
    double p_c = aircraft.axes.pitch.curvature;
    double p_s = aircraft.axes.pitch.scale;

    double y_d = aircraft.axes.yaw.deadzone;
    double y_c = aircraft.axes.yaw.curvature;
    double y_s = aircraft.axes.yaw.scale;

    Data::get()->controls.roll         = -hid::AxisTune::getAxisTune( r_rv, r_d, r_c, r_s );
    Data::get()->controls.pitch        = -hid::AxisTune::getAxisTune( p_rv, p_d, p_c, p_s );
    Data::get()->controls.yaw          = -hid::AxisTune::getAxisTune( y_rv, y_d, y_c, y_s );
    Data::get()->controls.trim_roll    = -hid::Manager::instance()->getTrimRoll();
    Data::get()->controls.trim_pitch   = -hid::Manager::instance()->getTrimPitch();
    Data::get()->controls.trim_yaw     = -hid::Manager::instance()->getTrimYaw();
    Data::get()->controls.brake_l      =  hid::Manager::instance()->getBrakeLeft();
    Data::get()->controls.brake_r      =  hid::Manager::instance()->getBrakeRight();
    Data::get()->controls.wheel_brake  =  std::max( hid::Manager::instance()->getBrakeLeft(), hid::Manager::instance()->getBrakeRight() );
    Data::get()->controls.landing_gear =  hid::Manager::instance()->getLandingGear();
    Data::get()->controls.wheel_nose   =  hid::Manager::instance()->getCtrlYaw();
    Data::get()->controls.flaps        =  hid::Manager::instance()->getFlaps();
    Data::get()->controls.airbrake     =  hid::Manager::instance()->getAirbrake();
    Data::get()->controls.spoilers     =  hid::Manager::instance()->getSpoilers();
    Data::get()->controls.collective   =  hid::Manager::instance()->getCollective();

    Data::get()->controls.lgh = hid::Manager::instance()->isLgHandleDown();
    Data::get()->controls.nws = _win->getNWS();
    Data::get()->controls.abs = _win->getABS();

    if ( _ap->isActiveAP() && _ap->isActiveFD() && !hid::Manager::instance()->getCWS() )
    {
        Data::get()->controls.roll  -= _ap->getCtrlRoll();
        Data::get()->controls.pitch -= _ap->getCtrlPitch();

        Data::get()->controls.roll  = std::max( -1.0, std::min( 1.0, Data::get()->controls.roll  ) );
        Data::get()->controls.pitch = std::max( -1.0, std::min( 1.0, Data::get()->controls.pitch ) );
    }

    if ( _ap->isActiveYD() )
    {
        Data::get()->controls.yaw -= _ap->getCtrlYaw();
        Data::get()->controls.yaw = std::max( -1.0, std::min( 1.0, Data::get()->controls.yaw ) );
    }

    // engines
    for ( unsigned int i = 0; i < FDM_MAX_ENGINES; i++ )
    {
        Data::get()->propulsion.engine[ i ].throttle  = hid::Manager::instance()->getThrottle  ( i );
        Data::get()->propulsion.engine[ i ].mixture   = hid::Manager::instance()->getMixture   ( i );
        Data::get()->propulsion.engine[ i ].propeller = hid::Manager::instance()->getPropeller ( i );
    }

    ///////////////////////////////////
    emit dataInpUpdated( Data::get() );
    ///////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updatedInputG1000()
{
    _g1000_input.ref_pressure = 101325.0;

    _g1000_input.fd_visible = false;
    _g1000_input.fd_pitch = 0.0;
    _g1000_input.fd_roll  = 0.0;

    _g1000_input.sel_course    = 0.0;
    _g1000_input.sel_heading   = 0.0;
    _g1000_input.sel_airspeed  = 0.0;
    _g1000_input.sel_altitude  = 0.0;
    _g1000_input.sel_climbRate = 0.0;

    _g1000_input.com_1_act = 118.000;
    _g1000_input.com_1_sby = 118.000;
    _g1000_input.com_2_act = 136.975;
    _g1000_input.com_2_sby = 136.975;

    _g1000_input.nav_1_act = 108.00;
    _g1000_input.nav_1_sby = 108.00;
    _g1000_input.nav_2_act = 117.95;
    _g1000_input.nav_2_sby = 117.95;
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updatedInputG1000( const fdm::DataOut &dataOut )
{
    _g1000_input.roll    = dataOut.flight.roll;
    _g1000_input.pitch   = dataOut.flight.pitch;
    _g1000_input.heading = dataOut.flight.heading;

    _g1000_input.airspeed_u_bas = dataOut.flight.airspeed_u_bas;
    _g1000_input.airspeed_v_bas = dataOut.flight.airspeed_v_bas;
    _g1000_input.airspeed_w_bas = dataOut.flight.airspeed_w_bas;

    _g1000_input.air_pressure    = dataOut.environment.air_pressure;
    _g1000_input.air_density     = dataOut.environment.air_density;
    _g1000_input.air_temperature = dataOut.environment.air_temperature;
}

////////////////////////////////////////////////////////////////////////////////

void Manager::onDataOutUpdated( const fdm::DataOut &dataOut )
{
    double dt = Data::get()->timeCoef * (double)_timerOut->restart() / 1000.0;

    updatedInputG1000( dataOut );

    // hud
    Data::get()->cgi.hud.roll    = dataOut.flight.roll;
    Data::get()->cgi.hud.pitch   = dataOut.flight.pitch;
    Data::get()->cgi.hud.heading = dataOut.flight.heading;

    Data::get()->cgi.hud.angleOfAttack = dataOut.flight.angleOfAttack;
    Data::get()->cgi.hud.sideslipAngle = dataOut.flight.sideslipAngle;

    Data::get()->cgi.hud.altitude  = dataOut.flight.altitude_asl;
    Data::get()->cgi.hud.climbRate = dataOut.flight.climbRate;
    Data::get()->cgi.hud.radioAlt  = dataOut.flight.altitude_agl;

    Data::get()->cgi.hud.airspeed   = dataOut.flight.ias;
    Data::get()->cgi.hud.machNumber = dataOut.flight.machNumber;
    Data::get()->cgi.hud.g_force    = dataOut.flight.g_force_z;

    Data::get()->cgi.hud.ils_visible      = Data::get()->navigation.ils_visible;
    Data::get()->cgi.hud.ils_gs_deviation = Data::get()->navigation.ils_gs_norm;
    Data::get()->cgi.hud.ils_lc_deviation = Data::get()->navigation.ils_lc_norm;

    Data::get()->cgi.hud.stall = dataOut.flight.stall;

    // ownship
    Data::get()->ownship.mainRotor.omega       = dataOut.rotor.mainRotor_omega;
    Data::get()->ownship.mainRotor.azimuth     = dataOut.rotor.mainRotor_azimuth;
    Data::get()->ownship.mainRotor.coningAngle = dataOut.rotor.mainRotor_coningAngle;
    Data::get()->ownship.mainRotor.diskRoll    = dataOut.rotor.mainRotor_diskRoll;
    Data::get()->ownship.mainRotor.diskPitch   = dataOut.rotor.mainRotor_diskPitch;
    Data::get()->ownship.mainRotor.collective  = dataOut.rotor.mainRotor_collective;
    Data::get()->ownship.mainRotor.cyclicLon   = dataOut.rotor.mainRotor_cyclicLon;
    Data::get()->ownship.mainRotor.cyclicLat   = dataOut.rotor.mainRotor_cyclicLat;

    for ( int i = 0; i < FDM_MAX_BLADES; i++ )
    {
        Data::get()->ownship.mainRotor.blade[ i ].flapping   = dataOut.blade[ i ].flapping;
        Data::get()->ownship.mainRotor.blade[ i ].feathering = dataOut.blade[ i ].feathering;
    }

    Data::get()->ownship.tailRotor.azimuth     = dataOut.rotor.tailRotor_azimuth;

    Data::get()->ownship.latitude  = dataOut.flight.latitude;
    Data::get()->ownship.longitude = dataOut.flight.longitude;

    Data::get()->ownship.altitude_asl = dataOut.flight.altitude_asl;
    Data::get()->ownship.altitude_agl = dataOut.flight.altitude_agl;

    Data::get()->ownship.roll    = dataOut.flight.roll;
    Data::get()->ownship.pitch   = dataOut.flight.pitch;
    Data::get()->ownship.heading = dataOut.flight.heading;

    Data::get()->ownship.angleOfAttack = dataOut.flight.angleOfAttack;
    Data::get()->ownship.sideslipAngle = dataOut.flight.sideslipAngle;

    Data::get()->ownship.climbAngle = dataOut.flight.climbAngle;
    Data::get()->ownship.trackAngle = dataOut.flight.trackAngle;

    Data::get()->ownship.slipSkidAngle = dataOut.flight.slipSkidAngle;

    Data::get()->ownship.airspeed    = dataOut.flight.airspeed;
    Data::get()->ownship.ias         = dataOut.flight.ias;
    Data::get()->ownship.tas         = dataOut.flight.tas;
    Data::get()->ownship.groundSpeed = dataOut.flight.groundSpeed;
    Data::get()->ownship.machNumber  = dataOut.flight.machNumber;
    Data::get()->ownship.climbRate   = dataOut.flight.climbRate;

    Data::get()->ownship.rollRate  = dataOut.flight.rollRate;
    Data::get()->ownship.pitchRate = dataOut.flight.pitchRate;
    Data::get()->ownship.yawRate   = dataOut.flight.yawRate;
    Data::get()->ownship.turnRate  = dataOut.flight.turnRate;

    Data::get()->ownship.g_force_x = dataOut.flight.g_force_x;
    Data::get()->ownship.g_force_y = dataOut.flight.g_force_y;
    Data::get()->ownship.g_force_z = dataOut.flight.g_force_z;

    Data::get()->ownship.pos_x_wgs = dataOut.flight.pos_x_wgs;
    Data::get()->ownship.pos_y_wgs = dataOut.flight.pos_y_wgs;
    Data::get()->ownship.pos_z_wgs = dataOut.flight.pos_z_wgs;

    Data::get()->ownship.att_e0_wgs = dataOut.flight.att_e0_wgs;
    Data::get()->ownship.att_ex_wgs = dataOut.flight.att_ex_wgs;
    Data::get()->ownship.att_ey_wgs = dataOut.flight.att_ey_wgs;
    Data::get()->ownship.att_ez_wgs = dataOut.flight.att_ez_wgs;

    Data::get()->ownship.vel_north = dataOut.flight.vel_north;
    Data::get()->ownship.vel_east  = dataOut.flight.vel_east;

    Data::get()->ownship.ailerons    = dataOut.controls.ailerons;
    Data::get()->ownship.elevator    = dataOut.controls.elevator;
    Data::get()->ownship.elevons     = dataOut.controls.elevons;
    Data::get()->ownship.rudder      = dataOut.controls.rudder;
    Data::get()->ownship.flaps       = dataOut.controls.flaps;
    Data::get()->ownship.flaperons   = dataOut.controls.flaperons;
    Data::get()->ownship.lef         = dataOut.controls.lef;
    Data::get()->ownship.airbrake    = dataOut.controls.airbrake;

    Data::get()->ownship.norm_airbrake    = hid::Manager::instance()->getAirbrake();
    Data::get()->ownship.norm_flaps       = hid::Manager::instance()->getFlaps();
    Data::get()->ownship.norm_landingGear = hid::Manager::instance()->getLandingGear();

    if ( dataOut.stateOut == fdm::DataOut::Working )
    {
        for ( signed int i = 0; i < FDM_MAX_ENGINES; i++ )
        {
            Data::get()->ownship.propeller[ i ] += dt * M_PI * dataOut.engine[ i ].rpm / 30.0;

            while ( Data::get()->ownship.propeller[ i ] > 2.0f * M_PI )
            {
                Data::get()->ownship.propeller[ i ] -= 2.0 * M_PI;
            }

            Data::get()->ownship.afterburner[ i ] =
                    fdm::Misc::inertia( dataOut.engine[ i ].afterburner ? 1.0 : 0.0,
                                        Data::get()->ownship.afterburner[ i ],
                                        dt, 0.5 );
        }
    }
    else if ( dataOut.stateOut == fdm::DataOut::Idle )
    {
        for ( signed int i = 0; i < FDM_MAX_ENGINES; i++ )
        {
            Data::get()->ownship.propeller   [ i ] = 0.0;
            Data::get()->ownship.afterburner [ i ] = 0.0;
        }
    }

    Data::get()->ownship.onGround = dataOut.flight.onGround;
    Data::get()->ownship.stall = dataOut.flight.stall;
    Data::get()->ownship.crash = dataOut.crash != fdm::DataOut::NoCrash;

    for ( int i = 0; i < FDM_MAX_ENGINES; i++ )
    {
        Data::get()->propulsion.engine[ i ].state       = dataOut.engine[ i ].state;

        Data::get()->propulsion.engine[ i ].rpm  = dataOut.engine[ i ].rpm;
        Data::get()->propulsion.engine[ i ].prop = dataOut.engine[ i ].prop;
        Data::get()->propulsion.engine[ i ].ng   = dataOut.engine[ i ].ng;
        Data::get()->propulsion.engine[ i ].n1   = dataOut.engine[ i ].n1;
        Data::get()->propulsion.engine[ i ].n2   = dataOut.engine[ i ].n2;
        Data::get()->propulsion.engine[ i ].trq  = dataOut.engine[ i ].trq;
        Data::get()->propulsion.engine[ i ].epr  = dataOut.engine[ i ].epr;
        Data::get()->propulsion.engine[ i ].map  = dataOut.engine[ i ].map;
        Data::get()->propulsion.engine[ i ].egt  = dataOut.engine[ i ].egt;
        Data::get()->propulsion.engine[ i ].cht  = dataOut.engine[ i ].cht;
        Data::get()->propulsion.engine[ i ].itt  = dataOut.engine[ i ].itt;
        Data::get()->propulsion.engine[ i ].tit  = dataOut.engine[ i ].tit;

        Data::get()->propulsion.engine[ i ].afterburner = dataOut.engine[ i ].afterburner;

        Data::get()->propulsion.engine[ i ].fuelFlow = dataOut.engine[ i ].fuelFlow;
    }

    // SFX
    // TODO

    // output state
    Data::get()->stateOut = dataOut.stateOut;

    // time step
    Data::get()->timeStep = _timeStep;
}
