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

////////////////////////////////////////////////////////////////////////////////

Simulation::Simulation() :
    QThread ( 0 ),

    m_timeoutTimer ( 0 ),
    m_elapsedTimer ( 0 ),

    m_fdm ( 0 ),

    m_timeStep ( 0.0 ),
    m_timeCoef ( 1.0 ),

    m_timerId ( 0 )
{
    m_fdm = new fdm::Manager();

    memset( &m_dataInp, 0, sizeof(fdm::DataInp) );
    memset( &m_dataOut, 0, sizeof(fdm::DataOut) );

#   ifdef SIM_USETHREADS
    moveToThread( this );
#   endif
}

////////////////////////////////////////////////////////////////////////////////

Simulation::~Simulation()
{
    if ( m_timerId ) killTimer( m_timerId );

    if ( m_timeoutTimer ) delete m_timeoutTimer;
    m_timeoutTimer = 0;

    if ( m_elapsedTimer ) delete m_elapsedTimer;
    m_elapsedTimer = 0;

    if ( m_fdm ) delete m_fdm;
    m_fdm = 0;
}

////////////////////////////////////////////////////////////////////////////////

void Simulation::init()
{
#   ifdef SIM_USETHREADS
    start();
    setPriority( QThread::HighPriority );
#   else
    m_elapsedTimer = new QElapsedTimer();
    m_timerId = startTimer( 1000 * FDM_TIME_STEP );
#   endif
}

////////////////////////////////////////////////////////////////////////////////

void Simulation::run()
{
    m_timeoutTimer = new QTimer();
    m_elapsedTimer = new QElapsedTimer();

    connect( m_timeoutTimer, SIGNAL(timeout()), this, SLOT(update()) );

    m_timeoutTimer->start( 1000 * FDM_TIME_STEP );
    m_elapsedTimer->start();

    ///////////////
    QThread::run();
    ///////////////

    disconnect( m_timeoutTimer, SIGNAL(timeout()), 0, 0 );

    m_timeoutTimer->stop();
}

////////////////////////////////////////////////////////////////////////////////

void Simulation::onDataInpUpdated( const Data::DataBuf *data )
{
    m_timeCoef = data->timeCoef;

    // environment
    m_dataInp.environment.temperature_0  = data->environment.temperature_0;
    m_dataInp.environment.pressure_0     = data->environment.pressure_0;
    m_dataInp.environment.wind_direction = data->environment.wind_direction;
    m_dataInp.environment.wind_speed     = data->environment.wind_speed;
    m_dataInp.environment.turbulence     = data->environment.turbulence;
    m_dataInp.environment.windShear      = data->environment.windShear;

    // initial conditions
    m_dataInp.initial.latitude     = data->initial.latitude;
    m_dataInp.initial.longitude    = data->initial.longitude;
    m_dataInp.initial.altitude_agl = data->initial.altitude_agl;
    m_dataInp.initial.heading      = data->initial.heading;
    m_dataInp.initial.airspeed     = data->initial.airspeed;
    m_dataInp.initial.engineOn     = data->initial.engineOn;

    // ground
    m_dataInp.ground.elevation = data->ground.elevation;
    m_dataInp.ground.r_x_wgs   = data->ground.r_x_wgs;
    m_dataInp.ground.r_y_wgs   = data->ground.r_y_wgs;
    m_dataInp.ground.r_z_wgs   = data->ground.r_z_wgs;
    m_dataInp.ground.n_x_wgs   = data->ground.n_x_wgs;
    m_dataInp.ground.n_y_wgs   = data->ground.n_y_wgs;
    m_dataInp.ground.n_z_wgs   = data->ground.n_z_wgs;

    // controls
    m_dataInp.controls.ctrl_roll    = data->controls.ctrl_roll;
    m_dataInp.controls.ctrl_pitch   = data->controls.ctrl_pitch;
    m_dataInp.controls.ctrl_yaw     = data->controls.ctrl_yaw;
    m_dataInp.controls.trim_roll    = data->controls.trim_roll;
    m_dataInp.controls.trim_pitch   = data->controls.trim_pitch;
    m_dataInp.controls.trim_yaw     = data->controls.trim_yaw;
    m_dataInp.controls.brake_l      = data->controls.brake_l;
    m_dataInp.controls.brake_r      = data->controls.brake_r;
    m_dataInp.controls.landing_gear = data->controls.landing_gear;
    m_dataInp.controls.nose_wheel   = data->controls.nose_wheel;
    m_dataInp.controls.lg_handle    = data->controls.lg_handle;
    m_dataInp.controls.nw_steering  = data->controls.nw_steering;
    m_dataInp.controls.flaps        = data->controls.flaps;
    m_dataInp.controls.airbrake     = data->controls.airbrake;
    m_dataInp.controls.spoilers     = data->controls.spoilers;
    m_dataInp.controls.collective   = data->controls.collective;

    // engines
    for ( unsigned int i = 0; i < FDM_MAX_ENGINES; i++ )
    {
        m_dataInp.engine[ i ].throttle  = data->propulsion.engine[ i ].throttle;
        m_dataInp.engine[ i ].mixture   = data->propulsion.engine[ i ].mixture;
        m_dataInp.engine[ i ].propeller = data->propulsion.engine[ i ].propeller;
        m_dataInp.engine[ i ].fuel      = data->propulsion.engine[ i ].fuel;
        m_dataInp.engine[ i ].ignition  = data->propulsion.engine[ i ].ignition;
        m_dataInp.engine[ i ].starter   = data->propulsion.engine[ i ].starter;
    }

    // masses
    m_dataInp.masses.pilot   = data->masses.pilot;
    m_dataInp.masses.pilot_l = data->masses.pilot_l;
    m_dataInp.masses.pilot_r = data->masses.pilot_r;
    m_dataInp.masses.pilot_f = data->masses.pilot_f;
    m_dataInp.masses.pilot_a = data->masses.pilot_a;
    m_dataInp.masses.fuel    = data->masses.fuel;
    m_dataInp.masses.fuel_l  = data->masses.fuel_l;
    m_dataInp.masses.fuel_r  = data->masses.fuel_r;
    m_dataInp.masses.fuel_f  = data->masses.fuel_f;
    m_dataInp.masses.fuel_a  = data->masses.fuel_a;
    m_dataInp.masses.cabin   = data->masses.cabin;
    m_dataInp.masses.trunk   = data->masses.trunk;

    // aircraft type
    m_dataInp.aircraftType = data->aircraftType;

    // input state
    m_dataInp.phaseInp = data->phaseInp;
}

////////////////////////////////////////////////////////////////////////////////

void Simulation::timerEvent( QTimerEvent *event )
{
    /////////////////////////////
    QThread::timerEvent( event );
    /////////////////////////////

#   ifndef SIM_USETHREADS
    update();
#   endif
}

////////////////////////////////////////////////////////////////////////////////

void Simulation::update()
{
    m_timeStep = m_timeCoef * (double)m_elapsedTimer->restart() / 1000.0;

    m_fdm->step( m_timeStep, m_dataInp, m_dataOut );

    /////////////////////////////////
    emit dataOutUpdated( m_dataOut );
    /////////////////////////////////
}
