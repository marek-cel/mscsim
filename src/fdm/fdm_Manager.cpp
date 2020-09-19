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

#include <fdm/fdm_Manager.h>

#include <cstring>

#include <fdm/fdm_Log.h>

#include <fdm/utils/fdm_String.h>
#include <fdm/utils/fdm_Time.h>
#include <fdm/utils/fdm_Units.h>

#include <fdm_aw101/aw101_FDM.h>
#include <fdm_c130/c130_FDM.h>
#include <fdm_c172/c172_FDM.h>
#include <fdm_f16/f16_FDM.h>
#include <fdm_f35a/f35a_FDM.h>
#include <fdm_p51/p51_FDM.h>
#include <fdm_pw5/pw5_FDM.h>
#include <fdm_r44/r44_FDM.h>
#include <fdm_uh60/uh60_FDM.h>

#ifdef FDM_TEST
#   include <fdm_xf/xf_FDM.h>
#   include <fdm_xh/xh_FDM.h>
#endif

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Manager::Manager( const DataInp *dataInpPtr, DataOut *dataOutPtr ) :
    _dataInpPtr ( dataInpPtr ),
    _dataOutPtr ( dataOutPtr ),

    _fdm ( FDM_NULLPTR ),

    _stateInp ( DataInp::Idle ),
    _stateOut ( DataOut::Idle ),

    _timeStep ( 0.0 ),
    _realTime ( 0.0 ),

    _compTimeMax  ( 0.0 ),
    _compTimeSum  ( 0.0 ),
    _compTimeSum2 ( 0.0 ),

    _timeStepRaw  ( 0.0 ),
    _timeStepMin  ( DBL_MAX ),
    _timeStepMax  ( 0.0 ),
    _timeStepSum  ( 0.0 ),
    _timeStepSum2 ( 0.0 ),

    _timeSteps ( 0 ),

    _stepsLT_def ( 0 ),
    _stepsGT_def ( 0 ),

    _verbose ( true )
{}

////////////////////////////////////////////////////////////////////////////////

Manager::~Manager()
{
    FDM_DELPTR( _fdm );
}

////////////////////////////////////////////////////////////////////////////////

void Manager::step( double timeStep )
{
    _timeStep = _timeStepRaw = timeStep;

    if ( _timeStep < FDM_TIME_STEP_MIN ) _timeStep = FDM_TIME_STEP_MIN;
    if ( _timeStep > FDM_TIME_STEP_MAX ) _timeStep = FDM_TIME_STEP_MAX;

    updateStateInp();

    if ( _stateOut == DataOut::Idle )
    {
        _aircraftType = _dataInpPtr->aircraftType;
    }

    switch ( _stateInp )
    {
        case DataInp::Idle:  updateStateIdle();  break;
        case DataInp::Init:  updateStateInit();  break;
        case DataInp::Work:  updateStateWork();  break;
        case DataInp::Pause: updateStatePause(); break;
        default:
        case DataInp::Stop:  updateStateStop();  break;
    }

    _dataOutPtr->stateOut = _stateOut;
}

////////////////////////////////////////////////////////////////////////////////

FDM* Manager::createFDM( AircraftType aircraftType )
{
    FDM *fdm = 0;

    switch ( aircraftType )
    {
        case DataInp::AW101: fdm = new AW101_FDM ( _dataInpPtr, _dataOutPtr, _verbose ); break;
        case DataInp::C130:  fdm = new C130_FDM  ( _dataInpPtr, _dataOutPtr, _verbose ); break;
        case DataInp::C172:  fdm = new C172_FDM  ( _dataInpPtr, _dataOutPtr, _verbose ); break;
        case DataInp::F16:   fdm = new F16_FDM   ( _dataInpPtr, _dataOutPtr, _verbose ); break;
        case DataInp::F35A:  fdm = new F35A_FDM  ( _dataInpPtr, _dataOutPtr, _verbose ); break;
        case DataInp::P51:   fdm = new P51_FDM   ( _dataInpPtr, _dataOutPtr, _verbose ); break;
        case DataInp::PW5:   fdm = new PW5_FDM   ( _dataInpPtr, _dataOutPtr, _verbose ); break;
        case DataInp::R44:   fdm = new R44_FDM   ( _dataInpPtr, _dataOutPtr, _verbose ); break;
        case DataInp::UH60:  fdm = new UH60_FDM  ( _dataInpPtr, _dataOutPtr, _verbose ); break;
#       ifdef FDM_TEST
        case DataInp::XF:    fdm = new XF_FDM    ( _dataInpPtr, _dataOutPtr, _verbose ); break;
        case DataInp::XH:    fdm = new XH_FDM    ( _dataInpPtr, _dataOutPtr, _verbose ); break;
#       endif
    }

    return fdm;
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updateStateInp()
{
    switch ( _dataInpPtr->stateInp )
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

    case DataInp::Pause:
        if ( _stateOut == DataOut::Ready
          || _stateOut == DataOut::Working
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
    _realTime = 0.0;

    _compTimeMax  = 0.0;
    _compTimeSum  = 0.0;
    _compTimeSum2 = 0.0;

    _timeStepMin  = DBL_MAX;
    _timeStepMax  = 0.0;
    _timeStepSum  = 0.0;
    _timeStepSum2 = 0.0;

    _timeSteps = 0;

    _stepsLT_def = 0;
    _stepsGT_def = 0;

    WGS84::Geo pos_geo;

    pos_geo.lat = _dataInpPtr->initial.latitude;
    pos_geo.lon = _dataInpPtr->initial.longitude;
    pos_geo.alt = _dataInpPtr->ground.elevation;

    if ( _dataInpPtr->initial.altitude_agl < FDM_MIN_INIT_ALTITUDE )
        pos_geo.alt += 1.0;
    else
        pos_geo.alt += _dataInpPtr->initial.altitude_agl;

    pos_geo = WGS84::getGeoOffset( pos_geo, _dataInpPtr->initial.heading,
                                   _dataInpPtr->initial.offset_x,
                                   _dataInpPtr->initial.offset_y );

    Quaternion ned2bas( Angles( 0.0, 0.0, _dataInpPtr->initial.heading ) );

    WGS84 wgs( pos_geo );

    Vector3    pos_wgs = wgs.getPos_WGS();
    Quaternion att_wgs = wgs.getWGS2BAS( ned2bas );

    Angles angles_wgs = att_wgs.getAngles();

    double altitude_asl = _dataInpPtr->initial.altitude_agl + _dataInpPtr->ground.elevation;
    double altitude_agl = _dataInpPtr->initial.altitude_agl;
    bool onGround = false;

    if ( _dataInpPtr->initial.altitude_agl < FDM_MIN_INIT_ALTITUDE )
    {
        altitude_asl = WGS84( pos_wgs ).getPos_Geo().alt;
        altitude_agl = altitude_asl - _dataInpPtr->ground.elevation;
        onGround = true;
    }

    // flight data
    _dataOutPtr->flight.latitude  = _dataInpPtr->initial.latitude;
    _dataOutPtr->flight.longitude = _dataInpPtr->initial.longitude;

    _dataOutPtr->flight.altitude_asl = altitude_asl;
    _dataOutPtr->flight.altitude_agl = altitude_agl;

    _dataOutPtr->flight.roll    = 0.0;
    _dataOutPtr->flight.pitch   = 0.0;
    _dataOutPtr->flight.heading = _dataInpPtr->initial.heading;

    _dataOutPtr->flight.angleOfAttack = 0.0;
    _dataOutPtr->flight.sideslipAngle = 0.0;

    _dataOutPtr->flight.climbAngle = 0.0;
    _dataOutPtr->flight.trackAngle = _dataInpPtr->initial.heading;

    _dataOutPtr->flight.slipSkidAngle = 0.0;

    _dataOutPtr->flight.airspeed   = _dataInpPtr->initial.airspeed;
    _dataOutPtr->flight.machNumber = _dataInpPtr->initial.airspeed / 340.29;
    _dataOutPtr->flight.climbRate  = 0.0;

    _dataOutPtr->flight.rollRate  = 0.0;
    _dataOutPtr->flight.pitchRate = 0.0;
    _dataOutPtr->flight.yawRate   = 0.0;
    _dataOutPtr->flight.turnRate  = 0.0;

    _dataOutPtr->flight.pos_x_wgs = pos_wgs.x();
    _dataOutPtr->flight.pos_y_wgs = pos_wgs.y();
    _dataOutPtr->flight.pos_z_wgs = pos_wgs.z();

    _dataOutPtr->flight.att_e0_wgs = att_wgs.e0();
    _dataOutPtr->flight.att_ex_wgs = att_wgs.ex();
    _dataOutPtr->flight.att_ey_wgs = att_wgs.ey();
    _dataOutPtr->flight.att_ez_wgs = att_wgs.ez();

    _dataOutPtr->flight.vel_u_bas = _dataInpPtr->initial.airspeed;
    _dataOutPtr->flight.vel_v_bas = 0.0;
    _dataOutPtr->flight.vel_w_bas = 0.0;

    _dataOutPtr->flight.omg_p_bas = 0.0;
    _dataOutPtr->flight.omg_q_bas = 0.0;
    _dataOutPtr->flight.omg_r_bas = 0.0;

    _dataOutPtr->flight.phi_wgs = angles_wgs.phi();
    _dataOutPtr->flight.tht_wgs = angles_wgs.tht();
    _dataOutPtr->flight.psi_wgs = angles_wgs.psi();

    _dataOutPtr->flight.airspeed_u_bas = _dataInpPtr->initial.airspeed;
    _dataOutPtr->flight.airspeed_v_bas = 0.0;
    _dataOutPtr->flight.airspeed_w_bas = 0.0;

    _dataOutPtr->flight.vel_north = 0.0;
    _dataOutPtr->flight.vel_east  = 0.0;

    _dataOutPtr->flight.acc_x_bas = 0.0;
    _dataOutPtr->flight.acc_y_bas = 0.0;
    _dataOutPtr->flight.acc_z_bas = 0.0;

    _dataOutPtr->flight.g_force_x = 0.0;
    _dataOutPtr->flight.g_force_y = 0.0;
    _dataOutPtr->flight.g_force_z = 1.0;

    _dataOutPtr->flight.g_pilot_x = 0.0;
    _dataOutPtr->flight.g_pilot_y = 0.0;
    _dataOutPtr->flight.g_pilot_z = 1.0;

    _dataOutPtr->flight.onGround = onGround;
    _dataOutPtr->flight.stall = false;

    // controls
    _dataOutPtr->controls.ailerons  = 0.0;
    _dataOutPtr->controls.elevator  = 0.0;
    _dataOutPtr->controls.elevons   = 0.0;
    _dataOutPtr->controls.rudder    = 0.0;
    _dataOutPtr->controls.flaps     = 0.0;
    _dataOutPtr->controls.flaperons = 0.0;
    _dataOutPtr->controls.lef       = 0.0;
    _dataOutPtr->controls.airbrake  = 0.0;

    // propulsion
    for ( int i = 0; i < FDM_MAX_ENGINES; i++ )
    {
        _dataOutPtr->engine[ i ].state = false;
        _dataOutPtr->engine[ i ].afterburner = false;
        _dataOutPtr->engine[ i ].rpm  = 0.0;
        _dataOutPtr->engine[ i ].prop = 0.0;
        _dataOutPtr->engine[ i ].ng   = 0.0;
        _dataOutPtr->engine[ i ].n1   = 0.0;
        _dataOutPtr->engine[ i ].n2   = 0.0;
        _dataOutPtr->engine[ i ].trq  = 0.0;
        _dataOutPtr->engine[ i ].epr  = 0.0;
        _dataOutPtr->engine[ i ].map  = 0.0;
        _dataOutPtr->engine[ i ].egt  = 0.0;
        _dataOutPtr->engine[ i ].itt  = 0.0;
        _dataOutPtr->engine[ i ].tit  = 0.0;
        _dataOutPtr->engine[ i ].fuelFlow = 0.0;
    }

    // rotor
    _dataOutPtr->rotor.mainRotor_azimuth     = 0.0;
    _dataOutPtr->rotor.mainRotor_coningAngle = 0.0;
    _dataOutPtr->rotor.mainRotor_diskRoll    = 0.0;
    _dataOutPtr->rotor.mainRotor_diskPitch   = 0.0;
    _dataOutPtr->rotor.mainRotor_collective  = 0.0;
    _dataOutPtr->rotor.mainRotor_cyclicLon   = 0.0;
    _dataOutPtr->rotor.mainRotor_cyclicLat   = 0.0;
    _dataOutPtr->rotor.tailRotor_azimuth     = 0.0;

    // crash
    _dataOutPtr->crash = DataOut::NoCrash;

    _stateOut = DataOut::Idle;
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updateStateInit()
{
    if ( _stateOut != DataOut::Ready )
    {
        try
        {
            if ( _fdm == FDM_NULLPTR )
            {
                _stateOut = DataOut::Initializing;

                _fdm = createFDM( _aircraftType );

                if ( _fdm != FDM_NULLPTR )
                {
                    _fdm->initialize();

                    if ( _dataInpPtr->recording.mode == DataInp::Recording::Replay )
                    {
                        _fdm->update( FDM_TIME_STEP );
                        _stateOut = DataOut::Ready;

                        if ( _verbose )
                        {
                            Log::i() << "Flight replaying started." << std::endl;
                            _fdm->printState();
                        }
                    }
                }
            }

            if ( _fdm != FDM_NULLPTR )
            {
                _fdm->initialize();

                if ( _fdm->isReady() )
                {
                    _stateOut = DataOut::Ready;
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
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updateStateWork()
{
    if ( _fdm != 0 )
    {
        try
        {
            double compTime_0 = Time::get();

            _fdm->update( _timeStep );

            _realTime += _timeStep;
            _timeSteps++;

            if ( DataOut::NoCrash == _fdm->getCrash() || _fdm->isReplaying() )
            {
                if ( _dataInpPtr->recording.mode == DataInp::Recording::Replay && !_fdm->isReplaying() )
                    _stateOut = DataOut::Stopped;
                else
                    _stateOut = DataOut::Working;
            }
            else
            {
                _stateOut = DataOut::Stopped;

                if ( _verbose )
                {
                    switch ( _fdm->getCrash() )
                    {
                    case fdm::DataOut::Collision:
                        Log::i() << "Crash: Collision with terrain or obstacle." << std::endl;
                        break;

                    case fdm::DataOut::Overspeed:
                        Log::i() << "Crash: Airspeed too high." << std::endl;
                        break;

                    case fdm::DataOut::Overstress:
                        Log::i() << "Crash: Load factor too high." << std::endl;
                        break;

                    default:
                        Log::i() << "Crash: Unknown crash cause." << std::endl;
                        break;
                    }

                    printFlightEndInfo();
                }
            }

            updateTimeStepStats( compTime_0 );
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
    if ( _verbose && _timeSteps > 0 && _stateOut != DataOut::Stopped )
    {
        printFlightEndInfo();
    }

    FDM_DELPTR( _fdm );

    _stateOut = DataOut::Stopped;
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updateTimeStepStats( double compTime_0 )
{
    _timeStepMin = Misc::min( _timeStepMin, _timeStepRaw );
    _timeStepMax = Misc::max( _timeStepMax, _timeStepRaw );

    double compTime = Time::get() - compTime_0;
    _compTimeMax = Misc::max( _compTimeMax, compTime );

    _compTimeSum  += compTime;
    _compTimeSum2 += Misc::pow2( compTime );

    _timeStepSum  += _timeStepRaw;
    _timeStepSum2 += Misc::pow2( _timeStepRaw );

    if ( _timeStepRaw < FDM_TIME_STEP ) _stepsLT_def++;
    if ( _timeStepRaw > FDM_TIME_STEP ) _stepsGT_def++;
}

////////////////////////////////////////////////////////////////////////////////

void Manager::printFlightEndInfo()
{
    Log::i() << "Flight finished." << std::endl;

    if ( _fdm ) _fdm->printState();
    printTimeStepStats();
}

////////////////////////////////////////////////////////////////////////////////

void Manager::printTimeStepStats()
{
    double meanStep = _realTime / (double)_timeSteps;
    double meanFreq = 1.0 / meanStep;
    double meanComp = _compTimeSum / (double)_timeSteps;

    double sdStep = Misc::stDev( _timeStepSum, _timeStepSum2, _timeSteps );
    double sdComp = Misc::stDev( _compTimeSum, _compTimeSum2, _timeSteps );

    Log::out().setf( std::ios_base::showpoint );
    Log::out().setf( std::ios_base::fixed );

    Log::out() << "  Total simulation time [s] : " << std::setprecision( 3 ) << _realTime << std::endl;
    Log::out() << "        Mean frequency [Hz] : " << std::setprecision( 3 ) << meanFreq  << std::endl;
    Log::out() << "         Mean time step [s] : " << std::setprecision( 6 ) << meanStep  << std::endl;
    Log::out() << "           Time step SD [s] : " << std::setprecision( 6 ) << sdStep    << std::endl;
    Log::out() << " Mean computations time [s] : " << std::setprecision( 6 ) << meanComp  << std::endl;
    Log::out() << "   Computations time SD [s] : " << std::setprecision( 6 ) << sdComp    << std::endl;
    Log::out() << "          Min time step [s] : " << std::setprecision( 6 ) << _timeStepMin << std::endl;
    Log::out() << "          Max time step [s] : " << std::setprecision( 6 ) << _timeStepMax << std::endl;
    Log::out() << "  Max computations time [s] : " << std::setprecision( 6 ) << _compTimeMax << std::endl;
    //Log::out() << "   mean_ts + 3*sigma_ts [s] : " << std::setprecision( 6 ) << ( meanStep + 3.0 * sdStep ) << " (96.65% is less than this value)"       << std::endl;
    //Log::out() << "   mean_ct + 3*sigma_ct [s] : " << std::setprecision( 6 ) << ( meanComp + 3.0 * sdComp ) << " (96.65% is less than this value)"       << std::endl;
    //Log::out() << "   mean_ts + 6*sigma_ts [s] : " << std::setprecision( 6 ) << ( meanStep + 6.0 * sdStep ) << " (99.99983% is less than this value)"   << std::endl;
    //Log::out() << "   mean_ct + 6*sigma_ct [s] : " << std::setprecision( 6 ) << ( meanComp + 6.0 * sdComp ) << " (99.99983% is less than this value)"   << std::endl;
    //Log::out() << "   mean_ts + 7*sigma_ts [s] : " << std::setprecision( 6 ) << ( meanStep + 7.0 * sdStep ) << " (99.99999905% is less than this value)" << std::endl;
    //Log::out() << "   mean_ct + 7*sigma_ct [s] : " << std::setprecision( 6 ) << ( meanComp + 7.0 * sdComp ) << " (99.99999905% is less than this value)" << std::endl;
    Log::out() << "            Number of steps : " << _timeSteps << std::endl;
    Log::out() << "     Steps less than " << std::setprecision( 3 ) << FDM_TIME_STEP << "s : " << _stepsLT_def << std::endl;
    Log::out() << "  Steps greater than " << std::setprecision( 3 ) << FDM_TIME_STEP << "s : " << _stepsGT_def << std::endl;

    Log::out().unsetf( std::ios_base::showpoint );
    Log::out().unsetf( std::ios_base::fixed );
}
