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

#include <Simulation.h>

#include <sim/Log.h>

////////////////////////////////////////////////////////////////////////////////

Simulation::Simulation() :
    QThread ( NULLPTR ),

    _timeoutTimer ( NULLPTR ),
    _elapsedTimer ( NULLPTR ),

    _fdm ( NULLPTR ),

    _timeStep ( 0.0 ),
    _timeCoef ( 1.0 ),

    _timerId ( 0 )
{
    memset( &_dataInp, 0, sizeof(fdm::DataInp) );
    memset( &_dataOut, 0, sizeof(fdm::DataOut) );

    _fdm = new fdm::Manager( &_dataInp, &_dataOut );

#   ifdef SIM_USE_THREADS
    moveToThread( this );
#   endif
}

////////////////////////////////////////////////////////////////////////////////

Simulation::~Simulation()
{
#   ifndef SIM_USE_THREADS
    if ( _timerId ) killTimer( _timerId );
#   endif

    DELPTR( _timeoutTimer );
    DELPTR( _elapsedTimer );

    DELPTR( _fdm );
}

////////////////////////////////////////////////////////////////////////////////

void Simulation::init()
{
#   ifdef SIM_USE_THREADS
    start();
    setPriority( QThread::HighPriority );
#   else
    _elapsedTimer = new QElapsedTimer();
    _timerId = startTimer( 1000.0 * FDM_TIME_STEP );
#   endif
}

////////////////////////////////////////////////////////////////////////////////

void Simulation::run()
{
    _timeoutTimer = new QTimer();
    _elapsedTimer = new QElapsedTimer();

    connect( _timeoutTimer, SIGNAL(timeout()), this, SLOT(update()) );

    _timeoutTimer->start( 1000.0 * FDM_TIME_STEP );
    _elapsedTimer->start();

    ///////////////
    QThread::run();
    ///////////////

    disconnect( _timeoutTimer, SIGNAL(timeout()), 0, 0 );

    _timeoutTimer->stop();
}

////////////////////////////////////////////////////////////////////////////////

void Simulation::onDataInpUpdated( const Data::DataBuf *data )
{
    _timeCoef = data->timeCoef;

    // environment
    _dataInp.environment.temperature_0  = data->environment.temperature_0;
    _dataInp.environment.pressure_0     = data->environment.pressure_0;
    _dataInp.environment.wind_direction = data->environment.wind_direction;
    _dataInp.environment.wind_speed     = data->environment.wind_speed;
    _dataInp.environment.turbulence     = data->environment.turbulence;
    _dataInp.environment.windShear      = data->environment.windShear;

    // initial conditions
    _dataInp.initial.latitude     = data->initial.latitude;
    _dataInp.initial.longitude    = data->initial.longitude;
    _dataInp.initial.altitude_agl = data->initial.altitude_agl;
    _dataInp.initial.offset_x     = data->initial.offset_x;
    _dataInp.initial.offset_y     = data->initial.offset_y;
    _dataInp.initial.heading      = data->initial.heading;
    _dataInp.initial.airspeed     = data->initial.airspeed;
    _dataInp.initial.engineOn     = data->initial.engineOn;

    // ground
    _dataInp.ground.elevation = data->ground.elevation;
    _dataInp.ground.r_x_wgs   = data->ground.r_x_wgs;
    _dataInp.ground.r_y_wgs   = data->ground.r_y_wgs;
    _dataInp.ground.r_z_wgs   = data->ground.r_z_wgs;
    _dataInp.ground.n_x_wgs   = data->ground.n_x_wgs;
    _dataInp.ground.n_y_wgs   = data->ground.n_y_wgs;
    _dataInp.ground.n_z_wgs   = data->ground.n_z_wgs;

    // controls
    _dataInp.controls.roll         = data->controls.roll;
    _dataInp.controls.pitch        = data->controls.pitch;
    _dataInp.controls.yaw          = data->controls.yaw;
    _dataInp.controls.trim_roll    = data->controls.trim_roll;
    _dataInp.controls.trim_pitch   = data->controls.trim_pitch;
    _dataInp.controls.trim_yaw     = data->controls.trim_yaw;
    _dataInp.controls.brake_l      = data->controls.brake_l;
    _dataInp.controls.brake_r      = data->controls.brake_r;
    _dataInp.controls.wheel_brake  = data->controls.wheel_brake;
    _dataInp.controls.landing_gear = data->controls.landing_gear;
    _dataInp.controls.wheel_nose   = data->controls.wheel_nose;
    _dataInp.controls.flaps        = data->controls.flaps;
    _dataInp.controls.airbrake     = data->controls.airbrake;
    _dataInp.controls.spoilers     = data->controls.spoilers;
    _dataInp.controls.collective   = data->controls.collective;

    _dataInp.controls.lgh = data->controls.lgh;
    _dataInp.controls.nws = data->controls.nws;
    _dataInp.controls.abs = data->controls.abs;

    // engines
    for ( unsigned int i = 0; i < FDM_MAX_ENGINES; i++ )
    {
        _dataInp.engine[ i ].throttle  = data->propulsion.engine[ i ].throttle;
        _dataInp.engine[ i ].mixture   = data->propulsion.engine[ i ].mixture;
        _dataInp.engine[ i ].propeller = data->propulsion.engine[ i ].propeller;
        _dataInp.engine[ i ].fuel      = data->propulsion.engine[ i ].fuel;
        _dataInp.engine[ i ].ignition  = data->propulsion.engine[ i ].ignition;
        _dataInp.engine[ i ].starter   = data->propulsion.engine[ i ].starter;
    }

    // masses
    for ( unsigned int i = 0; i < FDM_MAX_PILOTS; i++ )
    {
        _dataInp.masses.pilot[ i ] = data->masses.pilot[ i ];
    }

    for ( unsigned int i = 0; i < FDM_MAX_TANKS; i++ )
    {
        _dataInp.masses.tank[ i ] = data->masses.tank[ i ];
    }

    _dataInp.masses.cabin = data->masses.cabin;
    _dataInp.masses.trunk = data->masses.trunk;
    _dataInp.masses.slung = data->masses.slung;

    // recording
    _dataInp.recording.mode = data->recording.mode;
    strncpy( _dataInp.recording.file, data->recording.file, 4096 );

    // aircraft type
    _dataInp.aircraftType = data->aircraftType;

    // input state
    _dataInp.stateInp = data->stateInp;

    // freezes
    _dataInp.freezePosition = data->freezePosition;
    _dataInp.freezeAttitude = data->freezeAttitude;
    _dataInp.freezeVelocity = data->freezeVelocity;
}

////////////////////////////////////////////////////////////////////////////////

void Simulation::timerEvent( QTimerEvent *event )
{
    /////////////////////////////
    QThread::timerEvent( event );
    /////////////////////////////

#   ifndef SIM_USE_THREADS
    update();
#   endif
}

////////////////////////////////////////////////////////////////////////////////

void Simulation::update()
{
    _timeStep = _timeCoef * static_cast<double>( _elapsedTimer->restart() ) / 1000.0;

    _fdm->step( _timeStep );

    ////////////////////////////////
    emit dataOutUpdated( _dataOut );
    ////////////////////////////////
}
