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

#include <iostream>
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
    _aircraft ( 0 ),

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
    if ( _aircraft ) delete _aircraft;
    _aircraft = 0;
}

////////////////////////////////////////////////////////////////////////////////

void Manager::step( double timeStep, const DataInp &dataInp, DataOut &dataOut )
{
    _timeStep = timeStep;

    if ( _timeStep < FDM_TIME_STEP_MIN ) _timeStep = FDM_TIME_STEP_MIN;
    if ( _timeStep > FDM_TIME_STEP_MAX ) _timeStep = FDM_TIME_STEP_MAX;

    _dataInp = dataInp;

    updateInternalStateInp();

    if ( _stateOut == DataOut::Idle )
    {
        _aircraftType = _dataInp.aircraftType;
    }

    if ( _aircraft )
    {
        _aircraft->setFreeze( dataInp.freeze );
    }

    switch ( _stateInp )
    {
        case DataInp::Idle:  updateStateIdle();  break;
        case DataInp::Init:  updateStateInit();  break;
        case DataInp::Work:  updateStateWork();  break;
        case DataInp::Pause: updateStatePause(); break;
        case DataInp::Stop:  updateStateStop();  break;
    }

    _dataOut.stateOut = _stateOut;

    dataOut = _dataOut;
}

////////////////////////////////////////////////////////////////////////////////

void Manager::initEquilibriumInFlight()
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

void Manager::initEquilibriumOnGround()
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

void Manager::updateInternalStateInp()
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

    _dataOut.flight.course    = _dataInp.initial.heading;
    _dataOut.flight.pathAngle = 0.0;

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

                switch ( _aircraftType )
                {
                case DataInp::C130:
                    _aircraft = new C130_Aircraft( &_dataInp, &_dataOut );
                    break;

                case DataInp::C172:
                    _aircraft = new C172_Aircraft( &_dataInp, &_dataOut );
                    break;

                case DataInp::F16:
                    _aircraft = new F16_Aircraft( &_dataInp, &_dataOut );
                    break;

                case DataInp::P51:
                    _aircraft = new P51_Aircraft( &_dataInp, &_dataOut );
                    break;

                case DataInp::UH60:
                    _aircraft = new UH60_Aircraft( &_dataInp, &_dataOut );
                    break;
                }

                if ( _aircraft != 0 )
                {
                    _aircraft->init( _dataInp.initial.engineOn );
                }
            }

            if ( _aircraft != 0 )
            {
                if ( _dataInp.initial.altitude_agl < FDM_MIN_INIT_ALTITUDE )
                {
                    initEquilibriumOnGround();
                }
                else
                {
                    initEquilibriumInFlight();
                }

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
            _realTime += _timeStep;
            _timeSteps++;

            _aircraft->step( _timeStep );
            _aircraft->updateOutputData();

            if ( DataOut::NoCrash == _aircraft->getCrash() )
            {
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
                        Log::i() << "CRASH: Collision with terrain or obstacle." << std::endl;
                        break;

                    case fdm::DataOut::Overspeed:
                        Log::i() << "CRASH: Airspeed too high. Airspeed= " << _aircraft->getAirspeed() << " [m/s]" << std::endl;
                        break;

                    case fdm::DataOut::Overstressed:
                        Log::i() << "CRASH: Load factor too high. Gz= " << _aircraft->getGForce().z() << std::endl;
                        break;

                    default:
                        Log::i() << "CRASH: Unknown crash cause." << std::endl;
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

void Manager::updateStatePause()
{
    _stateOut = DataOut::Paused;
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updateStateStop()
{
    if ( _verbose )
    {
        if ( _timeSteps > 0 )
        {
            double meanStep = _realTime / (double)_timeSteps;
            double meanFreq = 1.0 / meanStep;
            Log::i() << "Mean time step: " << meanStep << " s"  << std::endl;
            Log::i() << "Mean frequency: " << meanFreq << " Hz" << std::endl;
        }
    }

    _realTime = 0.0;
    _timeSteps = 0;

    if ( _aircraft ) delete _aircraft;
    _aircraft = 0;

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
