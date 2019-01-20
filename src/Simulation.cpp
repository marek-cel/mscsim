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
#include <Simulation.h>
#include <Data.h>

#include <hid/hid_Manager.h>

////////////////////////////////////////////////////////////////////////////////

Data::DataBuf Data::m_data;

////////////////////////////////////////////////////////////////////////////////

Simulation::Simulation() :
    QObject( 0 ),

    m_nav ( 0 ),
    m_sim ( 0 ),
    m_win ( 0 ),

    m_timer ( 0 ),
    m_timerId ( 0 ),
    m_timeStep ( 0.0 )
{
    memset( &m_dataInp, 0, sizeof(fdm::DataInp) );
    memset( &m_dataOut, 0, sizeof(fdm::DataOut) );

    m_nav = new Navigation();
    m_sim = new fdm::Manager();
    m_win = new MainWindow();

    m_timer = new QElapsedTimer();
}

////////////////////////////////////////////////////////////////////////////////

Simulation::~Simulation()
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

void Simulation::init()
{
    hid::Manager::instance()->init();

    m_win->show();
    m_win->init();

    m_timerId = startTimer( 1000 * FDM_TIME_STEP );

    m_timer->start();
}

////////////////////////////////////////////////////////////////////////////////

void Simulation::timerEvent( QTimerEvent *event )
{
    /////////////////////////////
    QObject::timerEvent( event );
    /////////////////////////////

    m_timeStep = Data::get()->timeCoef * (double)m_timer->restart() / 1000.0;

    if ( Data::get()->phaseInp == fdm::DataInp::Idle )
    {
        hid::Manager::instance()->reset();
    }

    hid::Manager::instance()->update( m_timeStep );
    m_nav->update();

    updateDataInp();
    m_sim->step( m_timeStep, m_dataInp, m_dataOut );
    updateDataOut();
}

////////////////////////////////////////////////////////////////////////////////

void Simulation::updateDataInp()
{
    // environment
    m_dataInp.environment.temperature_0  = Data::get()->environment.temperature_0;
    m_dataInp.environment.pressure_0     = Data::get()->environment.pressure_0;
    m_dataInp.environment.wind_direction = Data::get()->environment.wind_direction;
    m_dataInp.environment.wind_speed     = Data::get()->environment.wind_speed;
    m_dataInp.environment.turbulence     = Data::get()->environment.turbulence;
    m_dataInp.environment.windShear      = Data::get()->environment.windShear;

    // initial conditions
    m_dataInp.initial.latitude     = Data::get()->initial.latitude;
    m_dataInp.initial.longitude    = Data::get()->initial.longitude;
    m_dataInp.initial.altitude_agl = Data::get()->initial.altitude_agl;
    m_dataInp.initial.heading      = Data::get()->initial.heading;
    m_dataInp.initial.airspeed     = Data::get()->initial.airspeed;
    m_dataInp.initial.engineOn     = Data::get()->initial.engineOn;

    // ground
    m_dataInp.ground.elevation = Data::get()->ground.elevation;
    m_dataInp.ground.r_x_wgs   = Data::get()->ground.r_x_wgs;
    m_dataInp.ground.r_y_wgs   = Data::get()->ground.r_y_wgs;
    m_dataInp.ground.r_z_wgs   = Data::get()->ground.r_z_wgs;
    m_dataInp.ground.n_x_wgs   = Data::get()->ground.n_x_wgs;
    m_dataInp.ground.n_y_wgs   = Data::get()->ground.n_y_wgs;
    m_dataInp.ground.n_z_wgs   = Data::get()->ground.n_z_wgs;

    // controls
    m_dataInp.controls.ctrl_roll    = -hid::Manager::instance()->getCtrlRoll();
    m_dataInp.controls.ctrl_pitch   = -hid::Manager::instance()->getCtrlPitch();
    m_dataInp.controls.ctrl_yaw     = -hid::Manager::instance()->getCtrlYaw();
    m_dataInp.controls.trim_roll    = -hid::Manager::instance()->getTrimRoll();
    m_dataInp.controls.trim_pitch   = -hid::Manager::instance()->getTrimPitch();
    m_dataInp.controls.trim_yaw     = -hid::Manager::instance()->getTrimYaw();
    m_dataInp.controls.brake_l      =  hid::Manager::instance()->getBrakeLeft();
    m_dataInp.controls.brake_r      =  hid::Manager::instance()->getBrakeRight();
    m_dataInp.controls.landing_gear =  hid::Manager::instance()->getLandingGear();
    m_dataInp.controls.nose_wheel   =  hid::Manager::instance()->getCtrlYaw();
    m_dataInp.controls.nw_steering  =  true;
    m_dataInp.controls.flaps        =  hid::Manager::instance()->getFlaps();
    m_dataInp.controls.airbrake     =  hid::Manager::instance()->getAirbrake();
    m_dataInp.controls.spoilers     =  hid::Manager::instance()->getSpoilers();
    m_dataInp.controls.collective   =  hid::Manager::instance()->getCollective();

    // engines
    for ( unsigned int i = 0; i < FDM_MAX_ENGINES; i++ )
    {
        m_dataInp.engine[ i ].throttle  = hid::Manager::instance()->getThrottle( i );
        m_dataInp.engine[ i ].mixture   = hid::Manager::instance()->getMixture( i );
        m_dataInp.engine[ i ].propeller = hid::Manager::instance()->getPropeller( i );
        m_dataInp.engine[ i ].fuel      = Data::get()->propulsion.engine[ i ].fuel;
        m_dataInp.engine[ i ].ignition  = Data::get()->propulsion.engine[ i ].ignition;
        m_dataInp.engine[ i ].starter   = Data::get()->propulsion.engine[ i ].starter;
    }

    // aircraft type
    m_dataInp.aircraftType = Data::get()->aircraftType;

    // input state
    m_dataInp.phaseInp = Data::get()->phaseInp;
}

////////////////////////////////////////////////////////////////////////////////

void Simulation::updateDataOut()
{
    // hud
    Data::get()->hud.roll    = m_dataOut.flight.roll;
    Data::get()->hud.pitch   = m_dataOut.flight.pitch;
    Data::get()->hud.heading = m_dataOut.flight.heading;

    Data::get()->hud.angleOfAttack = m_dataOut.flight.angleOfAttack;
    Data::get()->hud.sideslipAngle = m_dataOut.flight.sideslipAngle;

    Data::get()->hud.altitude  = m_dataOut.flight.altitude_asl;
    Data::get()->hud.climbRate = m_dataOut.flight.climbRate;
    Data::get()->hud.radioAlt  = m_dataOut.flight.altitude_agl;

    Data::get()->hud.airspeed   = m_dataOut.flight.airspeed;
    Data::get()->hud.machNumber = m_dataOut.flight.machNumber;
    Data::get()->hud.g_force    = m_dataOut.flight.g_force_z;

    Data::get()->hud.ils_visible      = Data::get()->navigation.ils_visible;
    Data::get()->hud.ils_gs_deviation = Data::get()->navigation.ils_gs_deviation;
    Data::get()->hud.ils_lc_deviation = Data::get()->navigation.ils_lc_deviation;

    Data::get()->hud.stall = m_dataOut.flight.stall;

    // ownship
    Data::get()->ownship.latitude  = m_dataOut.flight.latitude;
    Data::get()->ownship.longitude = m_dataOut.flight.longitude;

    Data::get()->ownship.altitude_asl = m_dataOut.flight.altitude_asl;
    Data::get()->ownship.altitude_agl = m_dataOut.flight.altitude_agl;

    Data::get()->ownship.roll    = m_dataOut.flight.roll;
    Data::get()->ownship.pitch   = m_dataOut.flight.pitch;
    Data::get()->ownship.heading = m_dataOut.flight.heading;

    Data::get()->ownship.angleOfAttack = m_dataOut.flight.angleOfAttack;
    Data::get()->ownship.sideslipAngle = m_dataOut.flight.sideslipAngle;

    Data::get()->ownship.course    = m_dataOut.flight.course;
    Data::get()->ownship.pathAngle = m_dataOut.flight.pathAngle;

    Data::get()->ownship.slipSkidAngle = m_dataOut.flight.slipSkidAngle;

    Data::get()->ownship.airspeed   = m_dataOut.flight.airspeed;
    Data::get()->ownship.machNumber = m_dataOut.flight.machNumber;
    Data::get()->ownship.climbRate  = m_dataOut.flight.climbRate;

    Data::get()->ownship.rollRate  = m_dataOut.flight.rollRate;
    Data::get()->ownship.pitchRate = m_dataOut.flight.pitchRate;
    Data::get()->ownship.yawRate   = m_dataOut.flight.yawRate;
    Data::get()->ownship.turnRate  = m_dataOut.flight.turnRate;

    Data::get()->ownship.g_force_x = m_dataOut.flight.g_force_x;
    Data::get()->ownship.g_force_y = m_dataOut.flight.g_force_y;
    Data::get()->ownship.g_force_z = m_dataOut.flight.g_force_z;

    Data::get()->ownship.pos_x_wgs = m_dataOut.flight.pos_x_wgs;
    Data::get()->ownship.pos_y_wgs = m_dataOut.flight.pos_y_wgs;
    Data::get()->ownship.pos_z_wgs = m_dataOut.flight.pos_z_wgs;

    Data::get()->ownship.att_e0_wgs = m_dataOut.flight.att_e0_wgs;
    Data::get()->ownship.att_ex_wgs = m_dataOut.flight.att_ex_wgs;
    Data::get()->ownship.att_ey_wgs = m_dataOut.flight.att_ey_wgs;
    Data::get()->ownship.att_ez_wgs = m_dataOut.flight.att_ez_wgs;

    Data::get()->ownship.ailerons    = -hid::Manager::instance()->getCtrlRoll();
    Data::get()->ownship.elevator    = -hid::Manager::instance()->getCtrlPitch();
    Data::get()->ownship.rudder      = -hid::Manager::instance()->getCtrlYaw();
    Data::get()->ownship.flaps       =  hid::Manager::instance()->getFlaps();
    Data::get()->ownship.airbrake    =  hid::Manager::instance()->getAirbrake();
    Data::get()->ownship.landingGear =  hid::Manager::instance()->getLandingGear();

    if ( m_dataOut.stateOut == fdm::DataOut::Working )
    {
        for ( signed int i = 0; i < FDM_MAX_ENGINES; i++ )
        {
            Data::get()->ownship.propeller[ i ] += m_timeStep * M_PI * m_dataOut.engine[ i ].rpm / 30.0;

            while ( Data::get()->ownship.propeller[ i ] > 2.0f * M_PI )
            {
                Data::get()->ownship.propeller[ i ] -= (float)( 2.0f * M_PI );
            }
        }
    }
    else if ( m_dataOut.stateOut == fdm::DataOut::Idle )
    {
        for ( signed int i = 0; i < FDM_MAX_ENGINES; i++ )
        {
            Data::get()->ownship.propeller[ i ] = 0.0;
        }

        Data::get()->ownship.mainRotor_psi = 0.0;
        Data::get()->ownship.tailRotor_psi = 0.0;
    }

    Data::get()->ownship.mainRotor_psi      = 0.0;
    Data::get()->ownship.mainRotor_beta_0   = 0.0;
    Data::get()->ownship.mainRotor_beta_1c  = 0.0;
    Data::get()->ownship.mainRotor_beta_1s  = 0.0;
    Data::get()->ownship.mainRotor_theta_0  = 0.0;
    Data::get()->ownship.mainRotor_theta_1c = 0.0;
    Data::get()->ownship.mainRotor_theta_1s = 0.0;
    Data::get()->ownship.tailRotor_psi      = 0.0;

    Data::get()->ownship.onGround = m_dataOut.flight.onGround;
    Data::get()->ownship.stall = m_dataOut.flight.stall;

    for ( int i = 0; i < FDM_MAX_ENGINES; i++ )
    {
        Data::get()->propulsion.engine[ i ].state = m_dataOut.engine[ i ].state;
        Data::get()->propulsion.engine[ i ].rpm   = m_dataOut.engine[ i ].rpm;
        Data::get()->propulsion.engine[ i ].prop  = m_dataOut.engine[ i ].prop;
        Data::get()->propulsion.engine[ i ].ng    = m_dataOut.engine[ i ].ng;
        Data::get()->propulsion.engine[ i ].n1    = m_dataOut.engine[ i ].n1;
        Data::get()->propulsion.engine[ i ].n2    = m_dataOut.engine[ i ].n2;
        Data::get()->propulsion.engine[ i ].trq   = m_dataOut.engine[ i ].trq;
        Data::get()->propulsion.engine[ i ].epr   = m_dataOut.engine[ i ].epr;
        Data::get()->propulsion.engine[ i ].map   = m_dataOut.engine[ i ].map;
        Data::get()->propulsion.engine[ i ].egt   = m_dataOut.engine[ i ].egt;
        Data::get()->propulsion.engine[ i ].itt   = m_dataOut.engine[ i ].itt;
        Data::get()->propulsion.engine[ i ].ff    = m_dataOut.engine[ i ].ff;
    }

    // output state
    Data::get()->stateOut = m_dataOut.stateOut;

    // time step
    Data::get()->timeStep = m_timeStep;
}
