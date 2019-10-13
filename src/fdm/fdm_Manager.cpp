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

#include <fdm/fdm_Manager.h>

#include <memory.h>

#include <fdm/fdm_Log.h>

#include <fdm/utils/fdm_String.h>
#include <fdm/utils/fdm_Units.h>

#include <fdm_c130/c130_Aircraft.h>
#include <fdm_c172/c172_Aircraft.h>
#include <fdm_f16/f16_Aircraft.h>
#include <fdm_p51/p51_Aircraft.h>
#include <fdm_uh60/uh60_Aircraft.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Manager::Manager() :
    _aircraft ( FDM_NULLPTR ),
    _recorder ( FDM_NULLPTR ),

    _stateInp ( DataInp::Idle ),
    _stateOut ( DataOut::Idle ),

    _initStep ( 0 ),

    _init_phi ( 0.0 ),
    _init_tht ( 0.0 ),
    _init_alt ( 0.0 ),

    _timeStep ( 0.0 ),
    _realTime ( 0.0 ),

    _timeSteps ( 0 ),

    _verbose ( true )
{
    memset( &_dataInp, 0, sizeof(DataInp) );
    memset( &_dataOut, 0, sizeof(DataOut) );
}

////////////////////////////////////////////////////////////////////////////////

Manager::~Manager()
{
    FDM_DELPTR( _aircraft );
    FDM_DELPTR( _recorder );
}

////////////////////////////////////////////////////////////////////////////////

void Manager::step( double timeStep, const DataInp &dataInp, DataOut &dataOut )
{
    _timeStep = timeStep;

    if ( _timeStep < FDM_TIME_STEP_MIN ) _timeStep = FDM_TIME_STEP_MIN;
    if ( _timeStep > FDM_TIME_STEP_MAX ) _timeStep = FDM_TIME_STEP_MAX;

    _dataInp = dataInp;

    updateStateInp();

    if ( _stateOut == DataOut::Idle )
    {
        _aircraftType = _dataInp.aircraftType;
    }

    switch ( _stateInp )
    {
        case DataInp::Idle:   updateStateIdle();   break;
        case DataInp::Init:   updateStateInit();   break;
        case DataInp::Work:   updateStateWork();   break;
        case DataInp::Freeze: updateStateFreeze(); break;
        case DataInp::Pause:  updateStatePause();  break;
        default:
        case DataInp::Stop:   updateStateStop();   break;
    }

    _dataOut.stateOut = _stateOut;

    dataOut = _dataOut;
}

////////////////////////////////////////////////////////////////////////////////

Aircraft* Manager::createAircraft( AircraftType aircraftType )
{
    Aircraft *aircraft = 0;

    switch ( aircraftType )
    {
    case DataInp::C130:
        aircraft = new C130_Aircraft( &_dataInp, &_dataOut );
        break;

    case DataInp::C172:
        aircraft = new C172_Aircraft( &_dataInp, &_dataOut );
        break;

    case DataInp::F16:
        aircraft = new F16_Aircraft( &_dataInp, &_dataOut );
        break;

    case DataInp::P51:
        aircraft = new P51_Aircraft( &_dataInp, &_dataOut );
        break;

    case DataInp::UH60:
        aircraft = new UH60_Aircraft( &_dataInp, &_dataOut );
        break;
    }

    return aircraft;
}

////////////////////////////////////////////////////////////////////////////////

void Manager::initInFlight()
{
    if ( _stateOut != DataOut::Ready )
    {
        updateInitialPositionAndAttitude();

        StateVector stateVector;

        stateVector( _is_x  ) = _init_pos_wgs.x();
        stateVector( _is_y  ) = _init_pos_wgs.y();
        stateVector( _is_z  ) = _init_pos_wgs.z();
        stateVector( _is_e0 ) = _init_att_wgs.e0();
        stateVector( _is_ex ) = _init_att_wgs.ex();
        stateVector( _is_ey ) = _init_att_wgs.ey();
        stateVector( _is_ez ) = _init_att_wgs.ez();
        stateVector( _is_u  ) = _dataInp.initial.airspeed;
        stateVector( _is_v  ) = 0.0;
        stateVector( _is_w  ) = 0.0;
        stateVector( _is_p  ) = 0.0;
        stateVector( _is_q  ) = 0.0;
        stateVector( _is_r  ) = 0.0;

        // TODO
        _stateOut = DataOut::Ready;

        _aircraft->setStateVector( stateVector );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Manager::initOnGround()
{
    if ( _stateOut != DataOut::Ready )
    {
        if ( _initStep == 0 )
        {
            _init_phi =  0.0;
            _init_tht =  0.0;
            _init_alt = FDM_MIN_INIT_ALTITUDE + _dataInp.ground.elevation;
        }

        for ( UInt32 i = 0; i < 1000 && _initStep < FDM_MAX_INIT_STEPS && _stateOut != DataOut::Ready; i++  )
        {
            Vector3 n_wgs = _aircraft->getIsect()->getNormal( _dataInp.initial.latitude,
                                                              _dataInp.initial.longitude );

            WGS84::Geo pos_geo;

            pos_geo.lat = _dataInp.initial.latitude;
            pos_geo.lon = _dataInp.initial.longitude;
            pos_geo.alt = _init_alt;

            Quaternion ned2bas( Angles( _init_phi,
                                        _init_tht,
                                        _dataInp.initial.heading ) );

            WGS84 wgs( pos_geo );

            _init_pos_wgs = wgs.getPos_WGS();
            _init_att_wgs = wgs.getWGS2BAS( ned2bas );

            StateVector stateVector;
            StateVector derivVector;

            stateVector( _is_x  ) = _init_pos_wgs.x();
            stateVector( _is_y  ) = _init_pos_wgs.y();
            stateVector( _is_z  ) = _init_pos_wgs.z();
            stateVector( _is_e0 ) = _init_att_wgs.e0();
            stateVector( _is_ex ) = _init_att_wgs.ex();
            stateVector( _is_ey ) = _init_att_wgs.ey();
            stateVector( _is_ez ) = _init_att_wgs.ez();
            stateVector( _is_u  ) = 0.0;
            stateVector( _is_v  ) = 0.0;
            stateVector( _is_w  ) = 0.0;
            stateVector( _is_p  ) = 0.0;
            stateVector( _is_q  ) = 0.0;
            stateVector( _is_r  ) = 0.0;

            _aircraft->setStateVector( stateVector );
            derivVector = _aircraft->getDerivVect();

            const double coef_p = 0.001;
            const double coef_q = 0.001;
            const double coef_n = 0.01;

            double dp_dt = derivVector( _is_p );
            double dq_dt = derivVector( _is_q );
            double dn_dt = ( _aircraft->getWGS2BAS() * n_wgs )
                         * Vector3( derivVector( _is_u ),
                                    derivVector( _is_v ),
                                    derivVector( _is_w ) );

            // attitude updating only on the ground
            if ( _aircraft->getGear()->getFor_BAS().getLength2() > 0.0 )
            {
                _init_phi += dp_dt * coef_p;
                _init_tht += dq_dt * coef_q;
                _init_alt += dn_dt * coef_n;
            }
            else
            {
                _init_alt += dn_dt * coef_n;
            }

            if ( _init_alt > 0.0
              && fabs( dp_dt ) < 1.0e-3
              && fabs( dq_dt ) < 1.0e-3
              && fabs( dn_dt ) < 1.0e-3 )
            {
                _stateOut = DataOut::Ready;

                if ( _verbose )
                {
                    Log::i() << "On-ground initialization finished in " << _initStep << " steps" << std::endl;
                    printState();
                }
            }
            else
            {
                _initStep++;
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Manager::initRecorder()
{
    // state
    _recorder->addVariable( new Recorder::Variable< double >( "state_x"  , &( _aircraft->getStateVect()( _is_x  ) ), 3 ) );
    _recorder->addVariable( new Recorder::Variable< double >( "state_y"  , &( _aircraft->getStateVect()( _is_y  ) ), 3 ) );
    _recorder->addVariable( new Recorder::Variable< double >( "state_z"  , &( _aircraft->getStateVect()( _is_z  ) ), 3 ) );
    _recorder->addVariable( new Recorder::Variable< double >( "state_e0" , &( _aircraft->getStateVect()( _is_e0 ) ), 8 ) );
    _recorder->addVariable( new Recorder::Variable< double >( "state_ex" , &( _aircraft->getStateVect()( _is_ex ) ), 8 ) );
    _recorder->addVariable( new Recorder::Variable< double >( "state_ey" , &( _aircraft->getStateVect()( _is_ey ) ), 8 ) );
    _recorder->addVariable( new Recorder::Variable< double >( "state_ez" , &( _aircraft->getStateVect()( _is_ez ) ), 8 ) );
    _recorder->addVariable( new Recorder::Variable< double >( "state_u"  , &( _aircraft->getStateVect()( _is_u  ) ), 3 ) );
    _recorder->addVariable( new Recorder::Variable< double >( "state_v"  , &( _aircraft->getStateVect()( _is_v  ) ), 3 ) );
    _recorder->addVariable( new Recorder::Variable< double >( "state_w"  , &( _aircraft->getStateVect()( _is_w  ) ), 3 ) );
    _recorder->addVariable( new Recorder::Variable< double >( "state_p"  , &( _aircraft->getStateVect()( _is_p  ) ), 8 ) );
    _recorder->addVariable( new Recorder::Variable< double >( "state_q"  , &( _aircraft->getStateVect()( _is_q  ) ), 8 ) );
    _recorder->addVariable( new Recorder::Variable< double >( "state_r"  , &( _aircraft->getStateVect()( _is_r  ) ), 8 ) );

    // initial conditions
    _recorder->addVariable( new Recorder::Variable< bool >( "engine_on" , &( _dataInp.initial.engineOn ) ) );

    // flight data
    _recorder->addVariable( new Recorder::Variable< double >( "latitude"        , &( _dataOut.flight.latitude      ), 8 ) );
    _recorder->addVariable( new Recorder::Variable< double >( "longitude"       , &( _dataOut.flight.longitude     ), 8 ) );
    _recorder->addVariable( new Recorder::Variable< double >( "altitude_agl"    , &( _dataOut.flight.altitude_agl  ), 3 ) );
    _recorder->addVariable( new Recorder::Variable< double >( "altitude_asl"    , &( _dataOut.flight.altitude_asl  ), 3 ) );
    _recorder->addVariable( new Recorder::Variable< double >( "roll"            , &( _dataOut.flight.roll          ), 6 ) );
    _recorder->addVariable( new Recorder::Variable< double >( "pitch"           , &( _dataOut.flight.pitch         ), 6 ) );
    _recorder->addVariable( new Recorder::Variable< double >( "heading"         , &( _dataOut.flight.heading       ), 6 ) );
    _recorder->addVariable( new Recorder::Variable< double >( "angle_of_attack" , &( _dataOut.flight.angleOfAttack ), 6 ) );
    _recorder->addVariable( new Recorder::Variable< double >( "sideslip_angle"  , &( _dataOut.flight.sideslipAngle ), 6 ) );
    _recorder->addVariable( new Recorder::Variable< double >( "climb_angle"     , &( _dataOut.flight.climbAngle    ), 6 ) );
    _recorder->addVariable( new Recorder::Variable< double >( "track_angle"     , &( _dataOut.flight.trackAngle    ), 6 ) );
    _recorder->addVariable( new Recorder::Variable< double >( "slip_skid_angle" , &( _dataOut.flight.slipSkidAngle ), 6 ) );
    _recorder->addVariable( new Recorder::Variable< double >( "airspeed"        , &( _dataOut.flight.airspeed      ), 3 ) );
    _recorder->addVariable( new Recorder::Variable< double >( "mach_number"     , &( _dataOut.flight.machNumber    ), 3 ) );
    _recorder->addVariable( new Recorder::Variable< double >( "climb_rate"      , &( _dataOut.flight.climbRate     ), 3 ) );

    // controls
    _recorder->addVariable( new Recorder::Variable< double >( "controls_roll"  , &( _dataInp.controls.roll         ), 3 ) );
    _recorder->addVariable( new Recorder::Variable< double >( "controls_pitch" , &( _dataInp.controls.pitch        ), 3 ) );
    _recorder->addVariable( new Recorder::Variable< double >( "controls_yaw"   , &( _dataInp.controls.yaw          ), 3 ) );
    _recorder->addVariable( new Recorder::Variable< double >( "trim_roll"      , &( _dataInp.controls.trim_roll    ), 3 ) );
    _recorder->addVariable( new Recorder::Variable< double >( "trim_pitch"     , &( _dataInp.controls.trim_pitch   ), 3 ) );
    _recorder->addVariable( new Recorder::Variable< double >( "trim_yaw"       , &( _dataInp.controls.trim_yaw     ), 3 ) );
    _recorder->addVariable( new Recorder::Variable< double >( "brake_l"        , &( _dataInp.controls.brake_l      ), 3 ) );
    _recorder->addVariable( new Recorder::Variable< double >( "brake_r"        , &( _dataInp.controls.brake_r      ), 3 ) );
    _recorder->addVariable( new Recorder::Variable< double >( "landing_gear"   , &( _dataInp.controls.landing_gear ), 3 ) );
    _recorder->addVariable( new Recorder::Variable< double >( "nose_wheel"     , &( _dataInp.controls.nose_wheel   ), 3 ) );
    _recorder->addVariable( new Recorder::Variable< double >( "flaps"          , &( _dataInp.controls.flaps        ), 3 ) );
    _recorder->addVariable( new Recorder::Variable< double >( "airbrake"       , &( _dataInp.controls.airbrake     ), 3 ) );
    _recorder->addVariable( new Recorder::Variable< double >( "spoilers"       , &( _dataInp.controls.spoilers     ), 3 ) );
    _recorder->addVariable( new Recorder::Variable< double >( "collective"     , &( _dataInp.controls.collective   ), 3 ) );

    _recorder->addVariable( new Recorder::Variable< bool >( "lgh" , &( _dataInp.controls.lgh ) ) );
    _recorder->addVariable( new Recorder::Variable< bool >( "nws" , &( _dataInp.controls.nws ) ) );
    _recorder->addVariable( new Recorder::Variable< bool >( "abs" , &( _dataInp.controls.abs ) ) );

    // engines
    for ( unsigned int i = 0; i < FDM_MAX_ENGINES; i++ )
    {
        std::string num = "_" + String::toString( (int)i );

        _recorder->addVariable( new Recorder::Variable< double >( "throttle"  + num, &( _dataInp.engine[ i ].throttle  ), 3 ) );
        _recorder->addVariable( new Recorder::Variable< double >( "mixture"   + num, &( _dataInp.engine[ i ].mixture   ), 3 ) );
        _recorder->addVariable( new Recorder::Variable< double >( "propeller" + num, &( _dataInp.engine[ i ].propeller ), 3 ) );

        _recorder->addVariable( new Recorder::Variable< bool >( "fuel"     + num, &( _dataInp.engine[ i ].fuel     ) ) );
        _recorder->addVariable( new Recorder::Variable< bool >( "ignition" + num, &( _dataInp.engine[ i ].ignition ) ) );
        _recorder->addVariable( new Recorder::Variable< bool >( "starter"  + num, &( _dataInp.engine[ i ].starter  ) ) );
    }

    _recorder->init( _dataInp.recording.mode, _dataInp.recording.file );
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updateInitialPositionAndAttitude()
{
    double altitude_asl = _dataInp.initial.altitude_agl + _dataInp.ground.elevation;

    if ( _dataInp.initial.altitude_agl < FDM_MIN_INIT_ALTITUDE )
    {
        altitude_asl = 1.0 + _dataInp.ground.elevation;
    }

    WGS84::Geo pos_geo;

    pos_geo.lat = _dataInp.initial.latitude;
    pos_geo.lon = _dataInp.initial.longitude;
    pos_geo.alt = altitude_asl;

    Quaternion ned2bas( Angles( 0.0, 0.0, _dataInp.initial.heading ) );

    WGS84 wgs( pos_geo );

    _init_pos_wgs = wgs.getPos_WGS();
    _init_att_wgs = wgs.getWGS2BAS( ned2bas );
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updateStateInp()
{
    switch ( _dataInp.stateInp )
    {
    case DataInp::Idle:
        if ( _stateOut == DataOut::Idle
          || _stateOut == DataOut::Stopped )
        {
            _stateInp = DataInp::Idle;
        }
        else
        {
            _stateInp = DataInp::Stop;
        }
        break;

    case DataInp::Init:
        if ( _stateOut == DataOut::Idle
          || _stateOut == DataOut::Initializing
          || _stateOut == DataOut::Ready )
        {
            _stateInp = DataInp::Init;
        }
        else
        {
            _stateInp = DataInp::Stop;
        }
        break;

    case DataInp::Work:
        if ( _stateOut == DataOut::Ready
          || _stateOut == DataOut::Working
          || _stateOut == DataOut::Frozen
          || _stateOut == DataOut::Paused )
        {
            _stateInp = DataInp::Work;
        }
        else if ( _stateOut == DataOut::Idle )
        {
            _stateInp = DataInp::Init;
        }
        else if ( _stateOut == DataOut::Stopped )
        {
            _stateInp = DataInp::Stop;
        }
        else
        {
            _stateInp = DataInp::Idle;
        }
        break;

    case DataInp::Freeze:
        if ( _stateOut == DataOut::Ready
          || _stateOut == DataOut::Working
          || _stateOut == DataOut::Frozen
          || _stateOut == DataOut::Paused )
        {
            _stateInp = DataInp::Freeze;
        }
        else if ( _stateOut == DataOut::Idle )
        {
            _stateInp = DataInp::Init;
        }
        else
        {
            _stateInp = DataInp::Idle;
        }
        break;

    case DataInp::Pause:
        if ( _stateOut == DataOut::Ready
          || _stateOut == DataOut::Working
          || _stateOut == DataOut::Frozen
          || _stateOut == DataOut::Paused )
        {
            _stateInp = DataInp::Pause;
        }
        else if ( _stateOut == DataOut::Idle )
        {
            _stateInp = DataInp::Init;
        }
        else
        {
            _stateInp = DataInp::Idle;
        }
        break;

    default:
        _stateInp = DataInp::Stop;
    }
}
////////////////////////////////////////////////////////////////////////////////

void Manager::updateStateIdle()
{
    updateInitialPositionAndAttitude();

    Angles angles_wgs = _init_att_wgs.getAngles();

    double altitude_asl = _dataInp.initial.altitude_agl + _dataInp.ground.elevation;
    double altitude_agl = _dataInp.initial.altitude_agl;
    bool onGround = false;

    if ( _dataInp.initial.altitude_agl < FDM_MIN_INIT_ALTITUDE )
    {
        altitude_asl = WGS84( _init_pos_wgs ).getPos_Geo().alt;
        altitude_agl = altitude_asl - _dataInp.ground.elevation;
        onGround = true;
    }

    // flight data
    _dataOut.flight.latitude  = _dataInp.initial.latitude;
    _dataOut.flight.longitude = _dataInp.initial.longitude;

    _dataOut.flight.altitude_asl = altitude_asl;
    _dataOut.flight.altitude_agl = altitude_agl;

    _dataOut.flight.roll    = 0.0;
    _dataOut.flight.pitch   = 0.0;
    _dataOut.flight.heading = _dataInp.initial.heading;

    _dataOut.flight.angleOfAttack = 0.0;
    _dataOut.flight.sideslipAngle = 0.0;

    _dataOut.flight.climbAngle = 0.0;
    _dataOut.flight.trackAngle = _dataInp.initial.heading;

    _dataOut.flight.slipSkidAngle = 0.0;

    _dataOut.flight.airspeed   = _dataInp.initial.airspeed;
    _dataOut.flight.machNumber = _dataInp.initial.airspeed / 340.29;
    _dataOut.flight.climbRate  = 0.0;

    _dataOut.flight.rollRate  = 0.0;
    _dataOut.flight.pitchRate = 0.0;
    _dataOut.flight.yawRate   = 0.0;
    _dataOut.flight.turnRate  = 0.0;

    _dataOut.flight.pos_x_wgs = _init_pos_wgs.x();
    _dataOut.flight.pos_y_wgs = _init_pos_wgs.y();
    _dataOut.flight.pos_z_wgs = _init_pos_wgs.z();

    _dataOut.flight.att_e0_wgs = _init_att_wgs.e0();
    _dataOut.flight.att_ex_wgs = _init_att_wgs.ex();
    _dataOut.flight.att_ey_wgs = _init_att_wgs.ey();
    _dataOut.flight.att_ez_wgs = _init_att_wgs.ez();

    _dataOut.flight.vel_u_bas = _dataInp.initial.airspeed;
    _dataOut.flight.vel_v_bas = 0.0;
    _dataOut.flight.vel_w_bas = 0.0;

    _dataOut.flight.omg_p_bas = 0.0;
    _dataOut.flight.omg_q_bas = 0.0;
    _dataOut.flight.omg_r_bas = 0.0;

    _dataOut.flight.phi_wgs = angles_wgs.phi();
    _dataOut.flight.tht_wgs = angles_wgs.tht();
    _dataOut.flight.psi_wgs = angles_wgs.psi();

    _dataOut.flight.tas_u_bas = _dataInp.initial.airspeed;
    _dataOut.flight.tas_v_bas = 0.0;
    _dataOut.flight.tas_w_bas = 0.0;

    _dataOut.flight.vel_north = 0.0;
    _dataOut.flight.vel_east  = 0.0;

    _dataOut.flight.acc_x_bas = 0.0;
    _dataOut.flight.acc_y_bas = 0.0;
    _dataOut.flight.acc_z_bas = 0.0;

    _dataOut.flight.g_force_x = 0.0;
    _dataOut.flight.g_force_y = 0.0;
    _dataOut.flight.g_force_z = 1.0;

    _dataOut.flight.g_pilot_x = 0.0;
    _dataOut.flight.g_pilot_y = 0.0;
    _dataOut.flight.g_pilot_z = 1.0;

    _dataOut.flight.onGround = onGround;
    _dataOut.flight.stall = false;

    // controls
    _dataOut.controls.ailerons  = 0.0;
    _dataOut.controls.elevator  = 0.0;
    _dataOut.controls.elevons   = 0.0;
    _dataOut.controls.rudder    = 0.0;
    _dataOut.controls.flaps     = 0.0;
    _dataOut.controls.flaperons = 0.0;
    _dataOut.controls.lef       = 0.0;
    _dataOut.controls.airbrake  = 0.0;

    // propulsion
    for ( int i = 0; i < FDM_MAX_ENGINES; i++ )
    {
        _dataOut.engine[ i ].state = false;
        _dataOut.engine[ i ].afterburner = false;
        _dataOut.engine[ i ].rpm  = 0.0;
        _dataOut.engine[ i ].prop = 0.0;
        _dataOut.engine[ i ].ng   = 0.0;
        _dataOut.engine[ i ].n1   = 0.0;
        _dataOut.engine[ i ].n2   = 0.0;
        _dataOut.engine[ i ].trq  = 0.0;
        _dataOut.engine[ i ].epr  = 0.0;
        _dataOut.engine[ i ].map  = 0.0;
        _dataOut.engine[ i ].egt  = 0.0;
        _dataOut.engine[ i ].itt  = 0.0;
        _dataOut.engine[ i ].tit  = 0.0;
        _dataOut.engine[ i ].fuelFlow = 0.0;
    }

    // rotor
    _dataOut.rotor.mainRotor_azimuth     = 0.0;
    _dataOut.rotor.mainRotor_coningAngle = 0.0;
    _dataOut.rotor.mainRotor_diskRoll    = 0.0;
    _dataOut.rotor.mainRotor_diskPitch   = 0.0;
    _dataOut.rotor.mainRotor_collective  = 0.0;
    _dataOut.rotor.mainRotor_cyclicLon   = 0.0;
    _dataOut.rotor.mainRotor_cyclicLat   = 0.0;
    _dataOut.rotor.tailRotor_azimuth     = 0.0;

    // crash
    _dataOut.crash = DataOut::NoCrash;

    // landing gear
    _dataOut.landing_gear = 0.0;

    _stateOut = DataOut::Idle;
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updateStateInit()
{
    if ( _stateOut != DataOut::Ready )
    {
        try
        {
            if ( _aircraft == 0 )
            {
                _stateOut = DataOut::Initializing;
                _initStep = 0;

                _aircraft = createAircraft( _aircraftType );
                _recorder = new Recorder( 0.1 );

                if ( _aircraft != 0 )
                {
                    initRecorder();
                    _aircraft->init( _dataInp.initial.engineOn );

                    if ( _dataInp.recording.mode == DataInp::Recording::Replay )
                    {
                        _stateOut = DataOut::Ready;
                        printState();
                    }
                }
            }

            if ( _aircraft != 0 )
            {
                if ( _dataInp.initial.altitude_agl < FDM_MIN_INIT_ALTITUDE )
                    initOnGround();
                else
                    initInFlight();

                _aircraft->updateOutputData();
            }
        }
        catch ( Exception &e )
        {
            Log::e() << e.getInfo() << std::endl;

            while ( e.hasCause() )
            {
                e = e.getCause();
                Log::e() << e.getInfo() << std::endl;
            }

            _stateOut = DataOut::Stopped;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updateStateWork()
{
    if ( _aircraft != 0 )
    {
        try
        {
            _recorder->step( _timeStep );

            if ( _recorder->isReplaying() )
                _aircraft->disableIntegration();
            else
                _aircraft->enableIntegration();

            if ( _dataInp.recording.mode != DataInp::Recording::Replay || _recorder->isReplaying() )
            {
                _aircraft->step( _timeStep );
                _aircraft->updateOutputData();
            }

            //double lat = Units::rad2deg( _aircraft->getWGS().getPos_Geo().lat );
            //double lon = Units::rad2deg( _aircraft->getWGS().getPos_Geo().lon );
            //printf( "%lf %lf\n", lat , lon );

            _realTime += _timeStep;
            _timeSteps++;

            if ( DataOut::NoCrash == _aircraft->getCrash() || _recorder->isReplaying() )
            {
                if ( _dataInp.recording.mode == DataInp::Recording::Replay && !_recorder->isReplaying() )
                    _stateOut = DataOut::Stopped;
                else
                    _stateOut = DataOut::Working;
            }
            else
            {
                _stateOut = DataOut::Stopped;

                if ( _verbose )
                {
                    switch ( _aircraft->getCrash() )
                    {
                    case fdm::DataOut::Collision:
                        Log::i() << "Crash: Collision with terrain or obstacle." << std::endl;
                        break;

                    case fdm::DataOut::Overspeed:
                        Log::i() << "Crash: Airspeed too high. Airspeed= " << _aircraft->getAirspeed() << " [m/s]" << std::endl;
                        break;

                    case fdm::DataOut::Overstressed:
                        Log::i() << "Crash: Load factor too high. Gz= " << _aircraft->getGForce().z() << std::endl;
                        break;

                    default:
                        Log::i() << "Crash: Unknown crash cause." << std::endl;
                        break;
                    }

                    printState();
                }
            }
        }
        catch ( Exception &e )
        {
            Log::e() << e.getInfo() << std::endl;

            while ( e.hasCause() )
            {
                e = e.getCause();
                Log::e() << e.getInfo() << std::endl;
            }

            _stateOut = DataOut::Stopped;
        }
    }
    else
    {
        _stateOut = DataOut::Stopped;
    }
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updateStateFreeze()
{
    if ( _aircraft != 0 )
    {
        try
        {
            _aircraft->disableIntegration();
            _aircraft->step( _timeStep );
            _aircraft->updateOutputData();

            _realTime += _timeStep;
            _timeSteps++;

            _stateOut = DataOut::Frozen;
        }
        catch ( Exception &e )
        {
            Log::e() << e.getInfo() << std::endl;

            while ( e.hasCause() )
            {
                e = e.getCause();
                Log::e() << e.getInfo() << std::endl;
            }

            _stateOut = DataOut::Stopped;
        }
    }
    else
    {
        _stateOut = DataOut::Stopped;
    }
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updateStatePause()
{
    _stateOut = DataOut::Paused;
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updateStateStop()
{
    if ( _verbose )
    {
        if ( _timeSteps > 0 && _stateOut != DataOut::Stopped )
        {
            double meanStep = _realTime / (double)_timeSteps;
            double meanFreq = 1.0 / meanStep;
            Log::i() << "Mean time step: " << meanStep << " s"  << std::endl;
            Log::i() << "Mean frequency: " << meanFreq << " Hz" << std::endl;
        }
    }

    _realTime = 0.0;
    _timeSteps = 0;

    FDM_DELPTR( _aircraft );
    FDM_DELPTR( _recorder );

    _stateOut = DataOut::Stopped;
}

////////////////////////////////////////////////////////////////////////////////

void Manager::printState()
{
    if ( _aircraft )
    {
        Log::out() << "        x-wgs [m] : " << _aircraft->getPos_WGS().x() << std::endl;
        Log::out() << "        y-wgs [m] : " << _aircraft->getPos_WGS().y() << std::endl;
        Log::out() << "        z-wgs [m] : " << _aircraft->getPos_WGS().z() << std::endl;
        Log::out() << "   latitude [deg] : " << Units::rad2deg( _aircraft->getWGS().getPos_Geo().lat ) << std::endl;
        Log::out() << "  longitude [deg] : " << Units::rad2deg( _aircraft->getWGS().getPos_Geo().lon ) << std::endl;
        Log::out() << " altitude ASL [m] : " << _aircraft->getAltitude_ASL() << std::endl;
        Log::out() << " altitude AGL [m] : " << _aircraft->getAltitude_AGL() << std::endl;
        Log::out() << "       roll [deg] : " << Units::rad2deg( _aircraft->getAngles_NED().phi() ) << std::endl;
        Log::out() << "      pitch [deg] : " << Units::rad2deg( _aircraft->getAngles_NED().tht() ) << std::endl;
        Log::out() << "    heading [deg] : " << Units::rad2deg( _aircraft->getAngles_NED().psi() ) << std::endl;
        Log::out() << "      u-bas [m/s] : " << _aircraft->getVel_BAS().x() << std::endl;
        Log::out() << "      v-bas [m/s] : " << _aircraft->getVel_BAS().y() << std::endl;
        Log::out() << "      w-bas [m/s] : " << _aircraft->getVel_BAS().z() << std::endl;
        Log::out() << "    p-bas [deg/s] : " << Units::rad2deg( _aircraft->getOmg_BAS().x() ) << std::endl;
        Log::out() << "    q-bas [deg/s] : " << Units::rad2deg( _aircraft->getOmg_BAS().y() ) << std::endl;
        Log::out() << "    r-bas [deg/s] : " << Units::rad2deg( _aircraft->getOmg_BAS().z() ) << std::endl;
        Log::out() << "   airspeed [m/s] : " << _aircraft->getAirspeed() << std::endl;
        Log::out() << "        AoA [deg] : " << Units::rad2deg( _aircraft->getAngleOfAttack() ) << std::endl;
        Log::out() << "           Gx [-] : " << _aircraft->getGForce().x() << std::endl;
        Log::out() << "           Gy [-] : " << _aircraft->getGForce().y() << std::endl;
        Log::out() << "           Gz [-] : " << _aircraft->getGForce().z() << std::endl;
        Log::out() << "  total mass [kg] : " << _aircraft->getMass()->getMass() << std::endl;
        Log::out() << "       r_cm_x [m] : " << _aircraft->getMass()->getCenterOfMass().x() << std::endl;
        Log::out() << "       r_cm_y [m] : " << _aircraft->getMass()->getCenterOfMass().y() << std::endl;
        Log::out() << "       r_cm_z [m] : " << _aircraft->getMass()->getCenterOfMass().z() << std::endl;
        Log::out() << std::endl;
    }
}
