/***************************************************************************//**
 *
 * @author Marek M. Cel <marekcel@marekcel.pl>
 *
 * @section LICENSE
 *
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

#include <fdm/fdm_FDM.h>

#include <cstring>

#include <fdm/fdm_Exception.h>
#include <fdm/fdm_Log.h>

#include <fdm/utils/fdm_String.h>
#include <fdm/utils/fdm_Units.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

FDM::FDM( const DataInp *dataInpPtr, DataOut *dataOutPtr, bool verbose ) :
    Base( new DataNode() ),

    _dataInpPtr ( dataInpPtr ),
    _dataOutPtr ( dataOutPtr ),

    _rootNode ( getDataRootNode() ),
    _aircraft ( FDM_NULLPTR ),
    _recorder ( new Recorder( 0.1 ) ),

    _initStep ( 0 ),

    _init_g_coef_p ( 0.001 ),
    _init_g_coef_q ( 0.001 ),
    _init_g_coef_n ( 0.010 ),

    _init_phi ( 0.0 ),
    _init_tht ( 0.0 ),
    _init_alt ( 0.0 ),

    _initialized ( false ),
    _ready ( false ),
    _verbose ( verbose )
{
    initDataTreeBasic();

    memset( &_dataInp, 0, sizeof(DataInp) );
    memset( &_dataOut, 0, sizeof(DataOut) );
}

////////////////////////////////////////////////////////////////////////////////

FDM::~FDM()
{
    FDM_DELPTR( _rootNode );
    FDM_DELPTR( _recorder );
}

////////////////////////////////////////////////////////////////////////////////

void FDM::initialize()
{
    updateAndSetDataInp();

    if ( !_initialized )
    {
        _initialized = true;
        initializeRecorder();
        _aircraft->initialize( _dataInp.initial.engineOn );
    }
    else
    {
        if ( !_ready )
        {
            if ( _dataInp.initial.altitude_agl < FDM_MIN_INIT_ALTITUDE )
                initializeOnGround();
            else
                initializeInFlight();
        }
    }

    updateAndSetDataOut();
}

////////////////////////////////////////////////////////////////////////////////

void FDM::update( double timeStep )
{
    updateAndSetDataInp();

    if ( _ready )
    {
        _recorder->step( timeStep );

        _aircraft->setFreezePosition( _dataInp.freezePosition );
        _aircraft->setFreezeAttitude( _dataInp.freezeAttitude );
        _aircraft->setFreezeVelocity( _dataInp.freezeVelocity );

        if ( _dataInp.recording.mode != DataInp::Recording::Replay || _recorder->isReplaying() )
        {
            _aircraft->update( timeStep, !_recorder->isReplaying() );
        }
    }

    updateAndSetDataOut();
}

////////////////////////////////////////////////////////////////////////////////

void FDM::printInitialConditions()
{
    Log::out().setf( std::ios_base::showpoint );
    Log::out().setf( std::ios_base::fixed );

    Log::out() << "    initial latitude [deg] : " << std::setprecision( 6 ) << Units::rad2deg( _dataInp.initial.latitude  ) << std::endl;
    Log::out() << "   initial longitude [deg] : " << std::setprecision( 6 ) << Units::rad2deg( _dataInp.initial.longitude ) << std::endl;

    Log::out().unsetf( std::ios_base::showpoint );
    Log::out().unsetf( std::ios_base::fixed );

    Log::out() << "  initial altitude AGL [m] : " << _dataInp.initial.altitude_agl << std::endl;
    Log::out() << "    initial airspeed [m/s] : " << _dataInp.initial.airspeed    << std::endl;
}

////////////////////////////////////////////////////////////////////////////////

void FDM::printState()
{
    Log::out().setf( std::ios_base::showpoint );
    Log::out().setf( std::ios_base::fixed );

    Log::out() << "        x-wgs [m] : " << std::setprecision( 3 ) << _aircraft->getPos_WGS().x() << std::endl;
    Log::out() << "        y-wgs [m] : " << std::setprecision( 3 ) << _aircraft->getPos_WGS().y() << std::endl;
    Log::out() << "        z-wgs [m] : " << std::setprecision( 3 ) << _aircraft->getPos_WGS().z() << std::endl;
    Log::out() << "   latitude [deg] : " << std::setprecision( 6 ) << Units::rad2deg( _aircraft->getWGS().getPos_Geo().lat ) << std::endl;
    Log::out() << "  longitude [deg] : " << std::setprecision( 6 ) << Units::rad2deg( _aircraft->getWGS().getPos_Geo().lon ) << std::endl;

    Log::out().unsetf( std::ios_base::showpoint );
    Log::out().unsetf( std::ios_base::fixed );

    Log::out() << " altitude ASL [m] : " << _aircraft->getAltitude_ASL() << std::endl;
    Log::out() << " altitude AGL [m] : " << _aircraft->getAltitude_AGL() << std::endl;
    Log::out() << "       roll [deg] : " << Units::rad2deg( _aircraft->getAngles_NED().phi() ) << std::endl;
    Log::out() << "      pitch [deg] : " << Units::rad2deg( _aircraft->getAngles_NED().tht() ) << std::endl;
    Log::out() << "    heading [deg] : " << Units::rad2deg( _aircraft->getAngles_NED().psi() ) << std::endl;
    Log::out() << "      u-bas [m/s] : " << _aircraft->getVel_BAS().u() << std::endl;
    Log::out() << "      v-bas [m/s] : " << _aircraft->getVel_BAS().v() << std::endl;
    Log::out() << "      w-bas [m/s] : " << _aircraft->getVel_BAS().w() << std::endl;
    Log::out() << "    p-bas [deg/s] : " << Units::rad2deg( _aircraft->getOmg_BAS().p() ) << std::endl;
    Log::out() << "    q-bas [deg/s] : " << Units::rad2deg( _aircraft->getOmg_BAS().q() ) << std::endl;
    Log::out() << "    r-bas [deg/s] : " << Units::rad2deg( _aircraft->getOmg_BAS().r() ) << std::endl;
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

////////////////////////////////////////////////////////////////////////////////

void FDM::initializeOnGround()
{
    if ( _initStep == 0 )
    {
        Log::i() << "On-ground initialization..." << std::endl;

        printInitialConditions();
    }

    if ( _initStep == 0 )
    {
        _init_phi =  0.0;
        _init_tht =  0.0;
        _init_alt = FDM_MIN_INIT_ALTITUDE + _dataInp.ground.elevation;
    }

    if ( _initStep < FDM_MAX_INIT_STEPS )
    {
        for ( UInt32 i = 0; i < 1000 && !_ready; i++  )
        {
            Vector3 n_wgs = _aircraft->getIsect()->getNormal( _dataInp.initial.latitude,
                                                              _dataInp.initial.longitude,
                                                              true );

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

            Aircraft::StateVector stateVector( _aircraft->getStateVect() );
            Aircraft::StateVector derivVector( _aircraft->getDerivVect() );

            stateVector( Aircraft::_i_x  ) = _init_pos_wgs.x();
            stateVector( Aircraft::_i_y  ) = _init_pos_wgs.y();
            stateVector( Aircraft::_i_z  ) = _init_pos_wgs.z();
            stateVector( Aircraft::_i_e0 ) = _init_att_wgs.e0();
            stateVector( Aircraft::_i_ex ) = _init_att_wgs.ex();
            stateVector( Aircraft::_i_ey ) = _init_att_wgs.ey();
            stateVector( Aircraft::_i_ez ) = _init_att_wgs.ez();
            stateVector( Aircraft::_i_u  ) = 0.0;
            stateVector( Aircraft::_i_v  ) = 0.0;
            stateVector( Aircraft::_i_w  ) = 0.0;
            stateVector( Aircraft::_i_p  ) = 0.0;
            stateVector( Aircraft::_i_q  ) = 0.0;
            stateVector( Aircraft::_i_r  ) = 0.0;

            _aircraft->setStateVector( stateVector );
            derivVector = _aircraft->getDerivVect();

            const double coef_p = 0.001;
            const double coef_q = 0.001;
            const double coef_n = 0.01;

            double dp_dt = derivVector( Aircraft::_i_p );
            double dq_dt = derivVector( Aircraft::_i_q );
            double dn_dt = ( _aircraft->getWGS2BAS() * n_wgs )
                         * Vector3( derivVector( Aircraft::_i_u ),
                                    derivVector( Aircraft::_i_v ),
                                    derivVector( Aircraft::_i_w ) );

            // attitude updating only on the ground
            if ( _aircraft->getGear()->getFor_BAS().getLength2() > 0.0 )
            {
                _init_phi += dp_dt * _init_g_coef_p;
                _init_tht += dq_dt * _init_g_coef_q;
                _init_alt += dn_dt * _init_g_coef_n;
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
                _ready = true;

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
    else
    {
        _ready = true;

        if ( _verbose )
        {
            Log::i() << "On-ground initialization failed." << std::endl;
            printState();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void FDM::initializeInFlight()
{
    if ( _initStep == 0 )
    {
        Log::i() << "In-flight initialization..." << std::endl;

        printInitialConditions();
    }

    updateInitialPositionAndAttitude();

    Aircraft::StateVector stateVector( _aircraft->getStateVect() );

    stateVector( Aircraft::_i_x  ) = _init_pos_wgs.x();
    stateVector( Aircraft::_i_y  ) = _init_pos_wgs.y();
    stateVector( Aircraft::_i_z  ) = _init_pos_wgs.z();
    stateVector( Aircraft::_i_e0 ) = _init_att_wgs.e0();
    stateVector( Aircraft::_i_ex ) = _init_att_wgs.ex();
    stateVector( Aircraft::_i_ey ) = _init_att_wgs.ey();
    stateVector( Aircraft::_i_ez ) = _init_att_wgs.ez();
    stateVector( Aircraft::_i_u  ) = _dataInp.initial.airspeed;
    stateVector( Aircraft::_i_v  ) = 0.0;
    stateVector( Aircraft::_i_w  ) = 0.0;
    stateVector( Aircraft::_i_p  ) = 0.0;
    stateVector( Aircraft::_i_q  ) = 0.0;
    stateVector( Aircraft::_i_r  ) = 0.0;

    _aircraft->setStateVector( stateVector );

    // TODO
    _ready = true;

    if ( _verbose )
    {
        Log::i() << "In-flight initialization finished in " << _initStep << " steps" << std::endl;
        printState();
    }
}

////////////////////////////////////////////////////////////////////////////////

void FDM::initializeRecorder()
{
    // state
    _recorder->addVariable( new Recorder::Variable< double >( "state_x"  , &( _aircraft->getStateVect()( Aircraft::_i_x  ) ), 3 ) );
    _recorder->addVariable( new Recorder::Variable< double >( "state_y"  , &( _aircraft->getStateVect()( Aircraft::_i_y  ) ), 3 ) );
    _recorder->addVariable( new Recorder::Variable< double >( "state_z"  , &( _aircraft->getStateVect()( Aircraft::_i_z  ) ), 3 ) );
    _recorder->addVariable( new Recorder::Variable< double >( "state_e0" , &( _aircraft->getStateVect()( Aircraft::_i_e0 ) ), 8 ) );
    _recorder->addVariable( new Recorder::Variable< double >( "state_ex" , &( _aircraft->getStateVect()( Aircraft::_i_ex ) ), 8 ) );
    _recorder->addVariable( new Recorder::Variable< double >( "state_ey" , &( _aircraft->getStateVect()( Aircraft::_i_ey ) ), 8 ) );
    _recorder->addVariable( new Recorder::Variable< double >( "state_ez" , &( _aircraft->getStateVect()( Aircraft::_i_ez ) ), 8 ) );
    _recorder->addVariable( new Recorder::Variable< double >( "state_u"  , &( _aircraft->getStateVect()( Aircraft::_i_u  ) ), 3 ) );
    _recorder->addVariable( new Recorder::Variable< double >( "state_v"  , &( _aircraft->getStateVect()( Aircraft::_i_v  ) ), 3 ) );
    _recorder->addVariable( new Recorder::Variable< double >( "state_w"  , &( _aircraft->getStateVect()( Aircraft::_i_w  ) ), 3 ) );
    _recorder->addVariable( new Recorder::Variable< double >( "state_p"  , &( _aircraft->getStateVect()( Aircraft::_i_p  ) ), 8 ) );
    _recorder->addVariable( new Recorder::Variable< double >( "state_q"  , &( _aircraft->getStateVect()( Aircraft::_i_q  ) ), 8 ) );
    _recorder->addVariable( new Recorder::Variable< double >( "state_r"  , &( _aircraft->getStateVect()( Aircraft::_i_r  ) ), 8 ) );

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
    _recorder->addVariable( new Recorder::Variable< double >( "wheel_nose"     , &( _dataInp.controls.wheel_nose   ), 3 ) );
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

        std::string throttle  = "throttle"   + num;
        std::string mixture   = "mixture"    + num;
        std::string propeller = "propeller"  + num;

        std::string fuel     = "fuel"      + num;
        std::string ignition = "ignition"  + num;
        std::string starter  = "starter"   + num;

        _recorder->addVariable( new Recorder::Variable< double >( throttle  .c_str(), &( _dataInp.engine[ i ].throttle  ), 3 ) );
        _recorder->addVariable( new Recorder::Variable< double >( mixture   .c_str(), &( _dataInp.engine[ i ].mixture   ), 3 ) );
        _recorder->addVariable( new Recorder::Variable< double >( propeller .c_str(), &( _dataInp.engine[ i ].propeller ), 3 ) );

        _recorder->addVariable( new Recorder::Variable< bool >( fuel     .c_str(), &( _dataInp.engine[ i ].fuel     ) ) );
        _recorder->addVariable( new Recorder::Variable< bool >( ignition .c_str(), &( _dataInp.engine[ i ].ignition ) ) );
        _recorder->addVariable( new Recorder::Variable< bool >( starter  .c_str(), &( _dataInp.engine[ i ].starter  ) ) );
    }

    _recorder->initialize( _dataInp.recording.mode, _dataInp.recording.file );
}

////////////////////////////////////////////////////////////////////////////////

void FDM::updateDataInp()
{
    if ( !_ready )
    {
        _dataInp.controls.collective = 0.0;
    }

    // input - controls
    _dataRefs.input.controls.roll         .setDatad( _dataInp.controls.roll         );
    _dataRefs.input.controls.pitch        .setDatad( _dataInp.controls.pitch        );
    _dataRefs.input.controls.yaw          .setDatad( _dataInp.controls.yaw          );
    _dataRefs.input.controls.trim_roll    .setDatad( _dataInp.controls.trim_roll    );
    _dataRefs.input.controls.trim_pitch   .setDatad( _dataInp.controls.trim_pitch   );
    _dataRefs.input.controls.trim_yaw     .setDatad( _dataInp.controls.trim_yaw     );
    _dataRefs.input.controls.brake_l      .setDatad( _dataInp.controls.brake_l      );
    _dataRefs.input.controls.brake_r      .setDatad( _dataInp.controls.brake_r      );
    _dataRefs.input.controls.landing_gear .setDatad( _dataInp.controls.landing_gear );
    _dataRefs.input.controls.wheel_nose   .setDatad( _dataInp.controls.wheel_nose   );
    _dataRefs.input.controls.flaps        .setDatad( _dataInp.controls.flaps        );
    _dataRefs.input.controls.airbrake     .setDatad( _dataInp.controls.airbrake     );
    _dataRefs.input.controls.spoilers     .setDatad( _dataInp.controls.spoilers     );
    _dataRefs.input.controls.collective   .setDatad( _dataInp.controls.collective   );

    _dataRefs.input.controls.lgh.setDatab( _dataInp.controls.lgh );
    _dataRefs.input.controls.nws.setDatab( _dataInp.controls.nws );
    _dataRefs.input.controls.abs.setDatab( _dataInp.controls.abs );

    // input - engines
    for ( int i = 0; i < FDM_MAX_ENGINES; i++ )
    {
        _dataRefs.input.engine[ i ].throttle  .setDatad( _dataInp.engine[ i ].throttle  );
        _dataRefs.input.engine[ i ].mixture   .setDatad( _dataInp.engine[ i ].mixture   );
        _dataRefs.input.engine[ i ].propeller .setDatad( _dataInp.engine[ i ].propeller );

        _dataRefs.input.engine[ i ].fuel     .setDatab( _dataInp.engine[ i ].fuel      );
        _dataRefs.input.engine[ i ].ignition .setDatab( _dataInp.engine[ i ].ignition  );
        _dataRefs.input.engine[ i ].starter  .setDatab( _dataInp.engine[ i ].starter   );
    }

    // input - masses
    for ( int i = 0; i < FDM_MAX_PILOTS; i++ )
    {
        _dataRefs.input.masses.pilot[ i ].setDatad( _dataInp.masses.pilot[ i ] );
    }

    for ( int i = 0; i < FDM_MAX_TANKS; i++ )
    {
        _dataRefs.input.masses.tank[ i ].setDatad( _dataInp.masses.tank[ i ] );
    }

    _dataRefs.input.masses.cabin.setDatad( _dataInp.masses.cabin );
    _dataRefs.input.masses.trunk.setDatad( _dataInp.masses.trunk );
    _dataRefs.input.masses.slung.setDatad( _dataInp.masses.slung );
}

////////////////////////////////////////////////////////////////////////////////

void FDM::updateDataOut()
{
    memset( &_dataOut, 0, sizeof(DataOut) );

    // flight data
    _dataOut.flight.latitude  = _aircraft->getWGS().getPos_Geo().lat;
    _dataOut.flight.longitude = _aircraft->getWGS().getPos_Geo().lon;

    _dataOut.flight.altitude_asl = _aircraft->getAltitude_ASL();
    _dataOut.flight.altitude_agl = _aircraft->getAltitude_AGL();

    _dataOut.flight.roll    = _aircraft->getRoll();
    _dataOut.flight.pitch   = _aircraft->getPitch();
    _dataOut.flight.heading = _aircraft->getHeading();

    _dataOut.flight.angleOfAttack = _aircraft->getAngleOfAttack();
    _dataOut.flight.sideslipAngle = _aircraft->getSideslipAngle();

    _dataOut.flight.climbAngle = _aircraft->getClimbAngle();
    _dataOut.flight.trackAngle = _aircraft->getTrackAngle();

    _dataOut.flight.slipSkidAngle = _aircraft->getSlipSkidAngle();

    _dataOut.flight.airspeed    = _aircraft->getAirspeed();
    _dataOut.flight.ias         = _aircraft->getIAS();
    _dataOut.flight.tas         = _aircraft->getTAS();
    _dataOut.flight.groundSpeed = _aircraft->getGroundSpeed();
    _dataOut.flight.machNumber  = _aircraft->getMachNumber();
    _dataOut.flight.climbRate   = _aircraft->getClimbRate();

    _dataOut.flight.rollRate  = _aircraft->getOmg_BAS().p();
    _dataOut.flight.pitchRate = _aircraft->getOmg_BAS().q();
    _dataOut.flight.yawRate   = _aircraft->getOmg_BAS().r();
    _dataOut.flight.turnRate  = _aircraft->getTurnRate();

    _dataOut.flight.pos_x_wgs = _aircraft->getPos_WGS().x();
    _dataOut.flight.pos_y_wgs = _aircraft->getPos_WGS().y();
    _dataOut.flight.pos_z_wgs = _aircraft->getPos_WGS().z();

    _dataOut.flight.att_e0_wgs = _aircraft->getAtt_WGS().e0();
    _dataOut.flight.att_ex_wgs = _aircraft->getAtt_WGS().ex();
    _dataOut.flight.att_ey_wgs = _aircraft->getAtt_WGS().ey();
    _dataOut.flight.att_ez_wgs = _aircraft->getAtt_WGS().ez();

    _dataOut.flight.vel_u_bas = _aircraft->getVel_BAS().u();
    _dataOut.flight.vel_v_bas = _aircraft->getVel_BAS().v();
    _dataOut.flight.vel_w_bas = _aircraft->getVel_BAS().w();

    _dataOut.flight.omg_p_bas = _aircraft->getOmg_BAS().p();
    _dataOut.flight.omg_q_bas = _aircraft->getOmg_BAS().q();
    _dataOut.flight.omg_r_bas = _aircraft->getOmg_BAS().r();

    _dataOut.flight.phi_wgs = _aircraft->getAngles_WGS().phi();
    _dataOut.flight.tht_wgs = _aircraft->getAngles_WGS().tht();
    _dataOut.flight.psi_wgs = _aircraft->getAngles_WGS().psi();

    _dataOut.flight.airspeed_u_bas = _aircraft->getVel_air_BAS().u();
    _dataOut.flight.airspeed_v_bas = _aircraft->getVel_air_BAS().v();
    _dataOut.flight.airspeed_w_bas = _aircraft->getVel_air_BAS().w();

    _dataOut.flight.vel_north = _aircraft->getVel_NED().x();
    _dataOut.flight.vel_east  = _aircraft->getVel_NED().y();

    _dataOut.flight.acc_x_bas = _aircraft->getAcc_BAS().x();
    _dataOut.flight.acc_y_bas = _aircraft->getAcc_BAS().y();
    _dataOut.flight.acc_z_bas = _aircraft->getAcc_BAS().z();

    _dataOut.flight.g_force_x = _aircraft->getGForce().x();
    _dataOut.flight.g_force_y = _aircraft->getGForce().y();
    _dataOut.flight.g_force_z = _aircraft->getGForce().z();

    _dataOut.flight.g_pilot_x = _aircraft->getGPilot().x();
    _dataOut.flight.g_pilot_y = _aircraft->getGPilot().y();
    _dataOut.flight.g_pilot_z = _aircraft->getGPilot().z();

    _dataOut.flight.onGround = _aircraft->getGear()->getOnGround();
    _dataOut.flight.stall    = _aircraft->getAero()->getStall();

    // environment data
    _dataOut.environment.air_pressure    = _aircraft->getEnvir()->getPressure();
    _dataOut.environment.air_density     = _aircraft->getEnvir()->getDensity();
    _dataOut.environment.air_temperature = _aircraft->getEnvir()->getTemperature();

    // crash
    _dataOut.crash = _aircraft->getCrash();
}

////////////////////////////////////////////////////////////////////////////////

void FDM::updateAndSetDataInp()
{
    ///////////////////////////////////////////
    _dataInp = (*_dataInpPtr); updateDataInp();
    ///////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////

void FDM::updateAndSetDataOut()
{
    ///////////////////////////////////////////
    updateDataOut(); (*_dataOutPtr) = _dataOut;
    ///////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////

void FDM::updateInitialPositionAndAttitude()
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

    pos_geo = WGS84::getGeoOffset( pos_geo, _dataInp.initial.heading,
                                   _dataInp.initial.offset_x,
                                   _dataInp.initial.offset_y );

    Quaternion ned2bas( Angles( 0.0, 0.0, _dataInp.initial.heading ) );

    WGS84 wgs( pos_geo );

    _init_pos_wgs = wgs.getPos_WGS();
    _init_att_wgs = wgs.getWGS2BAS( ned2bas );
}

////////////////////////////////////////////////////////////////////////////////

void FDM::initDataTreeBasic()
{
    int result = FDM_SUCCESS;

    // input - controls
    if ( result == FDM_SUCCESS ) result = addDataRef( "input.controls.roll"         , DataNode::Double );
    if ( result == FDM_SUCCESS ) result = addDataRef( "input.controls.pitch"        , DataNode::Double );
    if ( result == FDM_SUCCESS ) result = addDataRef( "input.controls.yaw"          , DataNode::Double );
    if ( result == FDM_SUCCESS ) result = addDataRef( "input.controls.trim_roll"    , DataNode::Double );
    if ( result == FDM_SUCCESS ) result = addDataRef( "input.controls.trim_pitch"   , DataNode::Double );
    if ( result == FDM_SUCCESS ) result = addDataRef( "input.controls.trim_yaw"     , DataNode::Double );
    if ( result == FDM_SUCCESS ) result = addDataRef( "input.controls.brake_left"   , DataNode::Double );
    if ( result == FDM_SUCCESS ) result = addDataRef( "input.controls.brake_right"  , DataNode::Double );
    if ( result == FDM_SUCCESS ) result = addDataRef( "input.controls.landing_gear" , DataNode::Double );
    if ( result == FDM_SUCCESS ) result = addDataRef( "input.controls.wheel_nose"   , DataNode::Double );
    if ( result == FDM_SUCCESS ) result = addDataRef( "input.controls.flaps"        , DataNode::Double );
    if ( result == FDM_SUCCESS ) result = addDataRef( "input.controls.airbrake"     , DataNode::Double );
    if ( result == FDM_SUCCESS ) result = addDataRef( "input.controls.spoilers"     , DataNode::Double );
    if ( result == FDM_SUCCESS ) result = addDataRef( "input.controls.collective"   , DataNode::Double );

    if ( result == FDM_SUCCESS ) result = addDataRef( "input.controls.lgh" , DataNode::Bool );
    if ( result == FDM_SUCCESS ) result = addDataRef( "input.controls.nws" , DataNode::Bool );
    if ( result == FDM_SUCCESS ) result = addDataRef( "input.controls.abs" , DataNode::Bool );

    if ( result == FDM_SUCCESS )
    {
        _dataRefs.input.controls.roll         = getDataRef( "input.controls.roll"         );
        _dataRefs.input.controls.pitch        = getDataRef( "input.controls.pitch"        );
        _dataRefs.input.controls.yaw          = getDataRef( "input.controls.yaw"          );
        _dataRefs.input.controls.trim_roll    = getDataRef( "input.controls.trim_roll"    );
        _dataRefs.input.controls.trim_pitch   = getDataRef( "input.controls.trim_pitch"   );
        _dataRefs.input.controls.trim_yaw     = getDataRef( "input.controls.trim_yaw"     );
        _dataRefs.input.controls.brake_l      = getDataRef( "input.controls.brake_left"   );
        _dataRefs.input.controls.brake_r      = getDataRef( "input.controls.brake_right"  );
        _dataRefs.input.controls.landing_gear = getDataRef( "input.controls.landing_gear" );
        _dataRefs.input.controls.wheel_nose   = getDataRef( "input.controls.wheel_nose"   );
        _dataRefs.input.controls.flaps        = getDataRef( "input.controls.flaps"        );
        _dataRefs.input.controls.airbrake     = getDataRef( "input.controls.airbrake"     );
        _dataRefs.input.controls.spoilers     = getDataRef( "input.controls.spoilers"     );
        _dataRefs.input.controls.collective   = getDataRef( "input.controls.collective"   );

        _dataRefs.input.controls.lgh = getDataRef( "input.controls.lgh" );
        _dataRefs.input.controls.nws = getDataRef( "input.controls.nws" );
        _dataRefs.input.controls.abs = getDataRef( "input.controls.abs" );
    }

    // input - engines
    for ( int i = 0; i < FDM_MAX_ENGINES; i++ )
    {
        std::string number = String::toString( i + 1 );

        std::string idstr_throttle  = "input.engine_" + number + ".throttle";
        std::string idstr_mixture   = "input.engine_" + number + ".mixture";
        std::string idstr_propeller = "input.engine_" + number + ".propeller";
        std::string idstr_fuel      = "input.engine_" + number + ".fuel";
        std::string idstr_ignition  = "input.engine_" + number + ".ignition";
        std::string idstr_starter   = "input.engine_" + number + ".starter";

        if ( result == FDM_SUCCESS ) result = addDataRef( idstr_throttle  , DataNode::Double );
        if ( result == FDM_SUCCESS ) result = addDataRef( idstr_mixture   , DataNode::Double );
        if ( result == FDM_SUCCESS ) result = addDataRef( idstr_propeller , DataNode::Double );

        if ( result == FDM_SUCCESS ) result = addDataRef( idstr_fuel      , DataNode::Bool );
        if ( result == FDM_SUCCESS ) result = addDataRef( idstr_ignition  , DataNode::Bool );
        if ( result == FDM_SUCCESS ) result = addDataRef( idstr_starter   , DataNode::Bool );

        if ( result == FDM_SUCCESS )
        {
            _dataRefs.input.engine[ i ].throttle  = getDataRef( idstr_throttle  );
            _dataRefs.input.engine[ i ].mixture   = getDataRef( idstr_mixture   );
            _dataRefs.input.engine[ i ].propeller = getDataRef( idstr_propeller );
            _dataRefs.input.engine[ i ].fuel      = getDataRef( idstr_fuel      );
            _dataRefs.input.engine[ i ].ignition  = getDataRef( idstr_ignition  );
            _dataRefs.input.engine[ i ].starter   = getDataRef( idstr_starter   );
        }
    }

    // input - masses
    for ( int i = 0; i < FDM_MAX_PILOTS; i++ )
    {
        std::string idstr = "input.masses.pilot_" + String::toString( i + 1 );

        if ( result == FDM_SUCCESS ) result = addDataRef( idstr, DataNode::Double );
        if ( result == FDM_SUCCESS ) _dataRefs.input.masses.pilot[ i ]  = getDataRef( idstr );
    }

    for ( int i = 0; i < FDM_MAX_TANKS; i++ )
    {
        std::string idstr = "input.masses.tank_" + String::toString( i + 1 );

        if ( result == FDM_SUCCESS ) result = addDataRef( idstr, DataNode::Double );
        if ( result == FDM_SUCCESS ) _dataRefs.input.masses.tank[ i ]  = getDataRef( idstr );
    }

    if ( result == FDM_SUCCESS ) result = addDataRef( "input.masses.cabin" , DataNode::Double );
    if ( result == FDM_SUCCESS ) result = addDataRef( "input.masses.trunk" , DataNode::Double );
    if ( result == FDM_SUCCESS ) result = addDataRef( "input.masses.slung" , DataNode::Double );

    if ( result == FDM_SUCCESS )
    {
        _dataRefs.input.masses.cabin  = getDataRef( "input.masses.cabin" );
        _dataRefs.input.masses.trunk  = getDataRef( "input.masses.trunk" );
        _dataRefs.input.masses.slung  = getDataRef( "input.masses.slung" );
    }

    if ( result != FDM_SUCCESS )
    {
        Exception e;

        e.setType( Exception::DataRefInitError );
        e.setInfo( "Cannot initialize data tree." );

        FDM_THROW( e );
    }
}

