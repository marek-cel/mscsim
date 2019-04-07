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

#include <Manager.h>
#include <Data.h>

#include <hid/hid_Manager.h>

////////////////////////////////////////////////////////////////////////////////

Data::DataBuf Data::m_data;

////////////////////////////////////////////////////////////////////////////////

Manager::Manager() :
    QObject( 0 ),

    m_nav ( 0 ),
    m_sim ( 0 ),
    m_win ( 0 ),

    m_timer ( 0 ),
    m_timerId ( 0 ),
    m_timeStep ( 0.0 )
{
    m_nav = new Navigation();
    m_sim = new Simulation();
    m_win = new MainWindow();

    m_timer = new QElapsedTimer();
}

////////////////////////////////////////////////////////////////////////////////

Manager::~Manager()
{
    if ( m_timerId != 0 ) killTimer( m_timerId );

    if ( m_timer ) delete m_timer;
    m_timer = 0;

    if ( m_nav ) delete m_nav;
    m_nav = 0;

    if ( m_sim ) delete m_sim;
    m_sim = 0;

    if ( m_win ) delete m_win;
    m_win = 0;
}

////////////////////////////////////////////////////////////////////////////////

void Manager::init()
{
    qRegisterMetaType< Data::DataBuf >( "Data::DataBuf" );
    qRegisterMetaType< fdm::DataOut  >( "fdm::DataOut"  );

    connect( this  , SIGNAL(dataInpUpdated(const Data::DataBuf *)) , m_sim , SLOT(onDataInpUpdated(const Data::DataBuf *)) );
    connect( m_sim , SIGNAL(dataOutUpdated(const fdm::DataOut &))  , this  , SLOT(onDataOutUpdated(const fdm::DataOut &))  );

    hid::Manager::instance()->init();

    m_sim->init();

    m_win->show();
    m_win->init();

    m_timerId = startTimer( 1000 * FDM_TIME_STEP );

    m_timer->start();
}

////////////////////////////////////////////////////////////////////////////////

void Manager::timerEvent( QTimerEvent *event )
{
    /////////////////////////////
    QObject::timerEvent( event );
    /////////////////////////////

    m_timeStep = Data::get()->timeCoef * (double)m_timer->restart() / 1000.0;

    if ( Data::get()->phaseInp == fdm::DataInp::Idle )
    {
        hid::Manager::instance()->reset();
    }
    else if ( Data::get()->phaseInp == fdm::DataInp::Work )
    {
        // TODO
    }

    hid::Manager::instance()->update( m_timeStep );
    m_nav->update();

    // controls
    Data::get()->controls.ctrl_roll    = -hid::Manager::instance()->getCtrlRoll();
    Data::get()->controls.ctrl_pitch   = -hid::Manager::instance()->getCtrlPitch();
    Data::get()->controls.ctrl_yaw     = -hid::Manager::instance()->getCtrlYaw();
    Data::get()->controls.trim_roll    = -hid::Manager::instance()->getTrimRoll();
    Data::get()->controls.trim_pitch   = -hid::Manager::instance()->getTrimPitch();
    Data::get()->controls.trim_yaw     = -hid::Manager::instance()->getTrimYaw();
    Data::get()->controls.brake_l      =  hid::Manager::instance()->getBrakeLeft();
    Data::get()->controls.brake_r      =  hid::Manager::instance()->getBrakeRight();
    Data::get()->controls.landing_gear =  hid::Manager::instance()->getLandingGear();
    Data::get()->controls.nose_wheel   =  hid::Manager::instance()->getCtrlYaw();
    Data::get()->controls.lg_handle    =  hid::Manager::instance()->isLgHandleDown();
    Data::get()->controls.nw_steering  =  true; // TODO
    Data::get()->controls.flaps        =  hid::Manager::instance()->getFlaps();
    Data::get()->controls.airbrake     =  hid::Manager::instance()->getAirbrake();
    Data::get()->controls.spoilers     =  hid::Manager::instance()->getSpoilers();
    Data::get()->controls.collective   =  hid::Manager::instance()->getCollective();

    // engines
    for ( unsigned int i = 0; i < FDM_MAX_ENGINES; i++ )
    {
        Data::get()->propulsion.engine[ i ].throttle  = hid::Manager::instance()->getThrottle( i );
        Data::get()->propulsion.engine[ i ].mixture   = hid::Manager::instance()->getMixture( i );
        Data::get()->propulsion.engine[ i ].propeller = hid::Manager::instance()->getPropeller( i );
    }

    ///////////////////////////////////
    emit dataInpUpdated( Data::get() );
    ///////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////

void Manager::onDataOutUpdated( const fdm::DataOut &dataOut )
{
    // hud
    Data::get()->hud.roll    = dataOut.flight.roll;
    Data::get()->hud.pitch   = dataOut.flight.pitch;
    Data::get()->hud.heading = dataOut.flight.heading;

    Data::get()->hud.angleOfAttack = dataOut.flight.angleOfAttack;
    Data::get()->hud.sideslipAngle = dataOut.flight.sideslipAngle;

    Data::get()->hud.altitude  = dataOut.flight.altitude_asl;
    Data::get()->hud.climbRate = dataOut.flight.climbRate;
    Data::get()->hud.radioAlt  = dataOut.flight.altitude_agl;

    Data::get()->hud.airspeed   = dataOut.flight.airspeed;
    Data::get()->hud.machNumber = dataOut.flight.machNumber;
    Data::get()->hud.g_force    = dataOut.flight.g_force_z;

    Data::get()->hud.ils_visible      = Data::get()->navigation.ils_visible;
    Data::get()->hud.ils_gs_deviation = Data::get()->navigation.ils_gs_deviation;
    Data::get()->hud.ils_lc_deviation = Data::get()->navigation.ils_lc_deviation;

    Data::get()->hud.stall = dataOut.flight.stall;

    // ownship
    Data::get()->ownship.latitude  = dataOut.flight.latitude;
    Data::get()->ownship.longitude = dataOut.flight.longitude;

    Data::get()->ownship.altitude_asl = dataOut.flight.altitude_asl;
    Data::get()->ownship.altitude_agl = dataOut.flight.altitude_agl;

    Data::get()->ownship.roll    = dataOut.flight.roll;
    Data::get()->ownship.pitch   = dataOut.flight.pitch;
    Data::get()->ownship.heading = dataOut.flight.heading;

    Data::get()->ownship.angleOfAttack = dataOut.flight.angleOfAttack;
    Data::get()->ownship.sideslipAngle = dataOut.flight.sideslipAngle;

    Data::get()->ownship.course    = dataOut.flight.course;
    Data::get()->ownship.pathAngle = dataOut.flight.pathAngle;

    Data::get()->ownship.slipSkidAngle = dataOut.flight.slipSkidAngle;

    Data::get()->ownship.airspeed   = dataOut.flight.airspeed;
    Data::get()->ownship.machNumber = dataOut.flight.machNumber;
    Data::get()->ownship.climbRate  = dataOut.flight.climbRate;

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

    Data::get()->ownship.ailerons    = dataOut.controls.ailerons;
    Data::get()->ownship.elevator    = dataOut.controls.elevator;
    Data::get()->ownship.elevons     = dataOut.controls.elevons;
    Data::get()->ownship.rudder      = dataOut.controls.rudder;
    Data::get()->ownship.flaps       = dataOut.controls.flaps;
    Data::get()->ownship.flaperons   = dataOut.controls.flaperons;
    Data::get()->ownship.lef         = dataOut.controls.lef;
    Data::get()->ownship.airbrake    = dataOut.controls.airbrake;
    Data::get()->ownship.landingGear =  hid::Manager::instance()->getLandingGear();

    if ( dataOut.stateOut == fdm::DataOut::Working )
    {
        for ( signed int i = 0; i < FDM_MAX_ENGINES; i++ )
        {
            Data::get()->ownship.propeller[ i ] += m_timeStep * M_PI * dataOut.engine[ i ].rpm / 30.0;

            while ( Data::get()->ownship.propeller[ i ] > 2.0f * M_PI )
            {
                Data::get()->ownship.propeller[ i ] -= (float)( 2.0f * M_PI );
            }
        }
    }
    else if ( dataOut.stateOut == fdm::DataOut::Idle )
    {
        for ( signed int i = 0; i < FDM_MAX_ENGINES; i++ )
        {
            Data::get()->ownship.propeller[ i ] = 0.0;
        }
    }

    Data::get()->ownship.mainRotor_azimuth     = dataOut.rotor.mainRotor_azimuth;
    Data::get()->ownship.mainRotor_coningAngle = dataOut.rotor.mainRotor_coningAngle;
    Data::get()->ownship.mainRotor_diskRoll    = dataOut.rotor.mainRotor_diskRoll;
    Data::get()->ownship.mainRotor_diskPitch   = dataOut.rotor.mainRotor_diskPitch;
    Data::get()->ownship.mainRotor_collective  = dataOut.rotor.mainRotor_collective;
    Data::get()->ownship.mainRotor_cyclicLon   = dataOut.rotor.mainRotor_cyclicLon;
    Data::get()->ownship.mainRotor_cyclicLat   = dataOut.rotor.mainRotor_cyclicLat;
    Data::get()->ownship.tailRotor_azimuth     = dataOut.rotor.tailRotor_azimuth;

    Data::get()->ownship.onGround = dataOut.flight.onGround;
    Data::get()->ownship.stall = dataOut.flight.stall;

    for ( int i = 0; i < FDM_MAX_ENGINES; i++ )
    {
        Data::get()->propulsion.engine[ i ].state       = dataOut.engine[ i ].state;
        Data::get()->propulsion.engine[ i ].afterburner = dataOut.engine[ i ].afterburner;

        Data::get()->propulsion.engine[ i ].rpm  = dataOut.engine[ i ].rpm;
        Data::get()->propulsion.engine[ i ].prop = dataOut.engine[ i ].prop;
        Data::get()->propulsion.engine[ i ].ng   = dataOut.engine[ i ].ng;
        Data::get()->propulsion.engine[ i ].n1   = dataOut.engine[ i ].n1;
        Data::get()->propulsion.engine[ i ].n2   = dataOut.engine[ i ].n2;
        Data::get()->propulsion.engine[ i ].trq  = dataOut.engine[ i ].trq;
        Data::get()->propulsion.engine[ i ].epr  = dataOut.engine[ i ].epr;
        Data::get()->propulsion.engine[ i ].map  = dataOut.engine[ i ].map;
        Data::get()->propulsion.engine[ i ].egt  = dataOut.engine[ i ].egt;
        Data::get()->propulsion.engine[ i ].itt  = dataOut.engine[ i ].itt;
        Data::get()->propulsion.engine[ i ].tit  = dataOut.engine[ i ].tit;
        Data::get()->propulsion.engine[ i ].ff   = dataOut.engine[ i ].ff;
    }

    // output state
    Data::get()->stateOut = dataOut.stateOut;

    // time step
    Data::get()->timeStep = m_timeStep;
}
