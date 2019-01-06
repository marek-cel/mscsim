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

#include <fdmUtils/fdm_String.h>

#include <fdm_c130/c130_Aircraft.h>
#include <fdm_c172/c172_Aircraft.h>
#include <fdm_f16c/f16c_Aircraft.h>
#include <fdm_uh60/uh60_Aircraft.h>

////////////////////////////////////////////////////////////////////////////////

#define FDM_MIN_INIT_ALTITUDE 30.0
#define FDM_MAX_INIT_STEPS 20000

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Manager::Manager() :
    m_aircraft ( 0 ),

    m_phaseInp ( DataInp::Idle ),
    m_stateOut ( DataOut::Idle ),

    m_initStep ( 0 ),

    m_init_phi ( 0.0 ),
    m_init_tht ( 0.0 ),
    m_init_alt ( 0.0 ),

    m_timeStep ( 0.0 ),
    m_realTime ( 0.0 ),

    m_verbose ( false )
{
    memset( &m_dataInp, 0, sizeof(DataInp) );
    memset( &m_dataOut, 0, sizeof(DataOut) );
}

////////////////////////////////////////////////////////////////////////////////

Manager::~Manager()
{
    if ( m_aircraft ) delete m_aircraft;
    m_aircraft = 0;
}

////////////////////////////////////////////////////////////////////////////////

void Manager::step( double timeStep, const DataInp &dataInp, DataOut &dataOut )
{
    m_timeStep = timeStep;

    if ( m_timeStep < FDM_TIME_STEP_MIN ) m_timeStep = FDM_TIME_STEP_MIN;
    if ( m_timeStep > FDM_TIME_STEP_MAX ) m_timeStep = FDM_TIME_STEP_MAX;

    m_dataInp = dataInp;

    updateInternalPhaseInp();

    if ( m_stateOut == DataOut::Idle )
    {
        m_aircraftType = m_dataInp.aircraftType;
    }

    switch ( m_phaseInp )
    {
        case DataInp::Idle:  updatePhaseIdle();  break;
        case DataInp::Init:  updatePhaseInit();  break;
        case DataInp::Work:  updatePhaseWork();  break;
        case DataInp::Pause: updatePhasePause(); break;
        case DataInp::Stop:  updatePhaseStop();  break;
    }

    m_dataOut.stateOut = m_stateOut;

    dataOut = m_dataOut;
}

////////////////////////////////////////////////////////////////////////////////

void Manager::dataRefsInit()
{
    // controls
    m_dr.ctrlRoll    = m_aircraft->getDataRef( "input/controls/roll"         );
    m_dr.ctrlPitch   = m_aircraft->getDataRef( "input/controls/pitch"        );
    m_dr.ctrlYaw     = m_aircraft->getDataRef( "input/controls/yaw"          );
    m_dr.trimRoll    = m_aircraft->getDataRef( "input/controls/roll_trim"    );
    m_dr.trimPitch   = m_aircraft->getDataRef( "input/controls/pitch_trim"   );
    m_dr.trimYaw     = m_aircraft->getDataRef( "input/controls/yaw_trim"     );
    m_dr.brakeLeft   = m_aircraft->getDataRef( "input/controls/brake_l"      );
    m_dr.brakeRight  = m_aircraft->getDataRef( "input/controls/brake_r"      );
    m_dr.landingGear = m_aircraft->getDataRef( "input/controls/landing_gear" );
    m_dr.noseWheel   = m_aircraft->getDataRef( "input/controls/nose_wheel"   );
    m_dr.nwSteering  = m_aircraft->getDataRef( "input/controls/nw_steering"  );
    m_dr.flaps       = m_aircraft->getDataRef( "input/controls/flaps"        );
    m_dr.airbrake    = m_aircraft->getDataRef( "input/controls/airbrake"     );
    m_dr.spoilers    = m_aircraft->getDataRef( "input/controls/spoilers"     );
    m_dr.collective  = m_aircraft->getDataRef( "input/controls/collective"   );

    // propulsion
    for ( int i = 0; i < FDM_MAX_ENGINES; i++ )
    {
        std::string number = String::toString( i + 1 );

        m_dr.throttle  [ i ] = m_aircraft->getDataRef( "input/propulsion/throttle_"  + number );
        m_dr.mixture   [ i ] = m_aircraft->getDataRef( "input/propulsion/mixture_"   + number );
        m_dr.propeller [ i ] = m_aircraft->getDataRef( "input/propulsion/propeller_" + number );
        m_dr.fuel      [ i ] = m_aircraft->getDataRef( "input/propulsion/fuel_"      + number );
        m_dr.ignition  [ i ] = m_aircraft->getDataRef( "input/propulsion/ignition_"  + number );
        m_dr.starter   [ i ] = m_aircraft->getDataRef( "input/propulsion/starter_"   + number );

        m_dr.engineOn  [ i ] = m_aircraft->getDataRef( "output/propulsion/state_" + number );
        m_dr.engineMAP [ i ] = m_aircraft->getDataRef( "output/propulsion/map_"   + number );
        m_dr.engineRPM [ i ] = m_aircraft->getDataRef( "output/propulsion/rpm_"   + number );
        m_dr.engineFF  [ i ] = m_aircraft->getDataRef( "output/propulsion/ff_"    + number );
    }

    if ( !m_dr.throttle[ 0 ].isValid() )
    {
        m_dr.throttle[ 0 ] = m_aircraft->getDataRef( "input/propulsion/throttle" );
    }

    if ( !m_dr.mixture[ 0 ].isValid() )
    {
       m_dr.mixture[ 0 ] = m_aircraft->getDataRef( "input/propulsion/mixture" );
    }

    if ( !m_dr.propeller[ 0 ].isValid() )
    {
        m_dr.propeller[ 0 ] = m_aircraft->getDataRef( "input/propulsion/propeller" );
    }

    if ( !m_dr.fuel[ 0 ].isValid() )
    {
        m_dr.fuel[ 0 ] = m_aircraft->getDataRef( "input/propulsion/fuel" );
    }

    if ( !m_dr.ignition[ 0 ].isValid() )
    {
        m_dr.ignition[ 0 ] = m_aircraft->getDataRef( "input/propulsion/ignition" );
    }

    if ( !m_dr.starter[ 0 ].isValid() )
    {
        m_dr.starter[ 0 ] = m_aircraft->getDataRef( "input/propulsion/starter" );
    }

    if ( !m_dr.engineOn[ 0 ].isValid() )
    {
        m_dr.engineOn[ 0 ] = m_aircraft->getDataRef( "output/propulsion/state" );
    }

    if ( !m_dr.engineMAP[ 0 ].isValid() )
    {
        m_dr.engineMAP[ 0 ] = m_aircraft->getDataRef( "output/propulsion/map" );
    }

    if ( !m_dr.engineRPM[ 0 ].isValid() )
    {
        m_dr.engineRPM[ 0 ] = m_aircraft->getDataRef( "output/propulsion/rpm" );
    }

    if ( !m_dr.engineFF[ 0 ].isValid() )
    {
        m_dr.engineFF[ 0 ] = m_aircraft->getDataRef( "output/propulsion/ff" );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Manager::dataRefsReset()
{
    m_dr.ctrlRoll    .reset();
    m_dr.ctrlPitch   .reset();
    m_dr.ctrlYaw     .reset();
    m_dr.trimRoll    .reset();
    m_dr.trimPitch   .reset();
    m_dr.trimYaw     .reset();
    m_dr.brakeLeft   .reset();
    m_dr.brakeRight  .reset();
    m_dr.landingGear .reset();
    m_dr.noseWheel   .reset();
    m_dr.nwSteering  .reset();
    m_dr.flaps       .reset();
    m_dr.airbrake    .reset();
    m_dr.spoilers    .reset();
    m_dr.collective  .reset();

    // propulsion
    for ( int i = 0; i < FDM_MAX_ENGINES; i++ )
    {
        m_dr.throttle  [ i ].reset();
        m_dr.mixture   [ i ].reset();
        m_dr.propeller [ i ].reset();
        m_dr.fuel      [ i ].reset();
        m_dr.ignition  [ i ].reset();
        m_dr.starter   [ i ].reset();

        m_dr.engineOn  [ i ].reset();
        m_dr.engineMAP [ i ].reset();
        m_dr.engineRPM [ i ].reset();
        m_dr.engineFF  [ i ].reset();
    }
}

////////////////////////////////////////////////////////////////////////////////

void Manager::initAircraft()
{
    if ( m_aircraft != 0 )
    {
        if ( m_dataInp.initial.altitude_agl < FDM_MIN_INIT_ALTITUDE )
        {
            initEquilibriumOnGround();
        }
        else
        {
            initEquilibriumInFlight();
        }

        updateDataOutput();
    }
}

////////////////////////////////////////////////////////////////////////////////

void Manager::initEquilibriumInFlight()
{
    if ( m_stateOut != DataOut::Ready )
    {
        updateInitialPositionAndAttitude();

        StateVector stateVector;

        stateVector( is_x  ) = m_init_pos_wgs.x();
        stateVector( is_y  ) = m_init_pos_wgs.y();
        stateVector( is_z  ) = m_init_pos_wgs.z();
        stateVector( is_e0 ) = m_init_att_wgs.e0();
        stateVector( is_ex ) = m_init_att_wgs.ex();
        stateVector( is_ey ) = m_init_att_wgs.ey();
        stateVector( is_ez ) = m_init_att_wgs.ez();
        stateVector( is_u  ) = m_dataInp.initial.airspeed;
        stateVector( is_v  ) = 0.0;
        stateVector( is_w  ) = 0.0;
        stateVector( is_p  ) = 0.0;
        stateVector( is_q  ) = 0.0;
        stateVector( is_r  ) = 0.0;

        // TODO
        m_stateOut = DataOut::Ready;

        m_aircraft->setStateVector( stateVector );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Manager::initEquilibriumOnGround()
{
    if ( m_stateOut != DataOut::Ready )
    {
        if ( m_initStep == 0 )
        {
            m_init_phi =  0.0;
            m_init_tht =  0.0;
            m_init_alt = FDM_MIN_INIT_ALTITUDE + m_dataInp.ground.elevation;
        }

        if ( m_initStep < FDM_MAX_INIT_STEPS )
        {
            Vector3 n_wgs = m_aircraft->getIsect()->getNormal( m_dataInp.initial.latitude,
                                                               m_dataInp.initial.longitude );

            WGS84::Geo pos_geo;

            pos_geo.lat = m_dataInp.initial.latitude;
            pos_geo.lon = m_dataInp.initial.longitude;
            pos_geo.alt = m_init_alt;

            Quaternion ned2bas( Angles( m_init_phi,
                                        m_init_tht,
                                        m_dataInp.initial.heading ) );

            WGS84 wgs( pos_geo );

            m_init_pos_wgs = wgs.getPos_WGS();
            m_init_att_wgs = wgs.getWGS2BAS( ned2bas );

            StateVector stateVector;
            StateVector derivVector;

            stateVector( is_x  ) = m_init_pos_wgs.x();
            stateVector( is_y  ) = m_init_pos_wgs.y();
            stateVector( is_z  ) = m_init_pos_wgs.z();
            stateVector( is_e0 ) = m_init_att_wgs.e0();
            stateVector( is_ex ) = m_init_att_wgs.ex();
            stateVector( is_ey ) = m_init_att_wgs.ey();
            stateVector( is_ez ) = m_init_att_wgs.ez();
            stateVector( is_u  ) = 0.0;
            stateVector( is_v  ) = 0.0;
            stateVector( is_w  ) = 0.0;
            stateVector( is_p  ) = 0.0;
            stateVector( is_q  ) = 0.0;
            stateVector( is_r  ) = 0.0;

            m_aircraft->setStateVector( stateVector );
            derivVector = m_aircraft->getDerivVect();

            const double coef = 0.01;

            double dp_dt = derivVector( is_p );
            double dq_dt = derivVector( is_q );
            double dn_dt = ( m_aircraft->getWGS2BAS() * n_wgs )
                         * Vector3( derivVector( is_u ),
                                    derivVector( is_v ),
                                    derivVector( is_w ) );

            // attitude updating only on the ground
            if ( m_aircraft->getGear()->getFor_BAS().getLength2() > 0 )
            {
                m_init_phi += dp_dt * coef;
                m_init_tht += dq_dt * coef;
                m_init_alt += dn_dt * coef;
            }
            else
            {
                m_init_alt += dn_dt * coef;
            }

            double delta = dp_dt * dp_dt
                         + dq_dt * dq_dt
                         + dn_dt * dn_dt;

            if ( delta < 1.0e-9 && m_init_alt > 0.0 )
            {
                m_stateOut = DataOut::Ready;
            }
            else
            {
                m_initStep++;
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updateDataInput()
{
    updateDataInput_Common();

    switch ( m_aircraftType )
    {
        case DataInp::C130: updateDataInput_C130(); break;
        case DataInp::C172: updateDataInput_C172(); break;
        case DataInp::F16C: updateDataInput_F16C(); break;
        case DataInp::UH60: updateDataInput_UH60(); break;
    }
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updateDataInput_Common()
{
    // controls
    if ( m_dr.ctrlRoll    .isValid() ) m_dr.ctrlRoll    .setDatad( m_dataInp.controls.ctrl_roll    );
    if ( m_dr.ctrlPitch   .isValid() ) m_dr.ctrlPitch   .setDatad( m_dataInp.controls.ctrl_pitch   );
    if ( m_dr.ctrlYaw     .isValid() ) m_dr.ctrlYaw     .setDatad( m_dataInp.controls.ctrl_yaw     );
    if ( m_dr.trimRoll    .isValid() ) m_dr.trimRoll    .setDatad( m_dataInp.controls.trim_roll    );
    if ( m_dr.trimPitch   .isValid() ) m_dr.trimPitch   .setDatad( m_dataInp.controls.trim_pitch   );
    if ( m_dr.trimYaw     .isValid() ) m_dr.trimYaw     .setDatad( m_dataInp.controls.trim_yaw     );
    if ( m_dr.brakeLeft   .isValid() ) m_dr.brakeLeft   .setDatad( m_dataInp.controls.brake_l      );
    if ( m_dr.brakeRight  .isValid() ) m_dr.brakeRight  .setDatad( m_dataInp.controls.brake_r      );
    if ( m_dr.landingGear .isValid() ) m_dr.landingGear .setDatad( m_dataInp.controls.landing_gear );
    if ( m_dr.noseWheel   .isValid() ) m_dr.noseWheel   .setDatad( m_dataInp.controls.nose_wheel   );
    if ( m_dr.nwSteering  .isValid() ) m_dr.nwSteering  .setDatab( m_dataInp.controls.nw_steering  );
    if ( m_dr.flaps       .isValid() ) m_dr.flaps       .setDatad( m_dataInp.controls.flaps        );
    if ( m_dr.airbrake    .isValid() ) m_dr.airbrake    .setDatad( m_dataInp.controls.airbrake     );
    if ( m_dr.spoilers    .isValid() ) m_dr.spoilers    .setDatad( m_dataInp.controls.spoilers     );
    if ( m_dr.collective  .isValid() ) m_dr.collective  .setDatad( m_dataInp.controls.collective   );

    // propulsion
    for ( int i = 0; i < FDM_MAX_ENGINES; i++ )
    {
        if ( m_dr.throttle  [ i ].isValid() ) m_dr.throttle  [ i ].setDatad( m_dataInp.engine[ i ].throttle  );
        if ( m_dr.mixture   [ i ].isValid() ) m_dr.mixture   [ i ].setDatad( m_dataInp.engine[ i ].mixture   );
        if ( m_dr.propeller [ i ].isValid() ) m_dr.propeller [ i ].setDatad( m_dataInp.engine[ i ].propeller );
        if ( m_dr.fuel      [ i ].isValid() ) m_dr.fuel      [ i ].setDatab( m_dataInp.engine[ i ].fuel      );
        if ( m_dr.ignition  [ i ].isValid() ) m_dr.ignition  [ i ].setDatab( m_dataInp.engine[ i ].ignition  );
        if ( m_dr.starter   [ i ].isValid() ) m_dr.starter   [ i ].setDatab( m_dataInp.engine[ i ].starter   );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updateDataInput_C130()
{
    // TODO
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updateDataInput_C172()
{
    // TODO
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updateDataInput_F16C()
{
    // TODO
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updateDataInput_UH60()
{
    // TODO
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updateDataOutput()
{
    updateDataOutput_Common();

    switch ( m_aircraftType )
    {
        case DataInp::C130: updateDataOutput_C130(); break;
        case DataInp::C172: updateDataOutput_C172(); break;
        case DataInp::F16C: updateDataOutput_F16C(); break;
        case DataInp::UH60: updateDataOutput_UH60(); break;
    }
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updateDataOutput_Common()
{
    m_dataOut.flight.latitude  = m_aircraft->getWGS().getPos_Geo().lat;
    m_dataOut.flight.longitude = m_aircraft->getWGS().getPos_Geo().lon;

    m_dataOut.flight.altitude_asl = m_aircraft->getAltitude_ASL();
    m_dataOut.flight.altitude_agl = m_aircraft->getAltitude_AGL();

    m_dataOut.flight.roll    = m_aircraft->getRoll();
    m_dataOut.flight.pitch   = m_aircraft->getPitch();
    m_dataOut.flight.heading = m_aircraft->getHeading();

    m_dataOut.flight.angleOfAttack = m_aircraft->getAngleOfAttack();
    m_dataOut.flight.sideslipAngle = m_aircraft->getSideslipAngle();

    m_dataOut.flight.course    = m_aircraft->getCourse();
    m_dataOut.flight.pathAngle = m_aircraft->getPathAngle();

    m_dataOut.flight.slipSkidAngle = m_aircraft->getSlipSkidAngle();

    m_dataOut.flight.airspeed   = m_aircraft->getAirspeed();
    m_dataOut.flight.machNumber = m_aircraft->getMachNumber();
    m_dataOut.flight.climbRate  = m_aircraft->getClimbRate();

    m_dataOut.flight.rollRate  = m_aircraft->getOmg_BAS().x();
    m_dataOut.flight.pitchRate = m_aircraft->getOmg_BAS().y();
    m_dataOut.flight.yawRate   = m_aircraft->getOmg_BAS().z();
    m_dataOut.flight.turnRate  = m_aircraft->getTurnRate();

    m_dataOut.flight.pos_x_wgs = m_aircraft->getPos_WGS().x();
    m_dataOut.flight.pos_y_wgs = m_aircraft->getPos_WGS().y();
    m_dataOut.flight.pos_z_wgs = m_aircraft->getPos_WGS().z();

    m_dataOut.flight.att_e0_wgs = m_aircraft->getAtt_WGS().e0();
    m_dataOut.flight.att_ex_wgs = m_aircraft->getAtt_WGS().ex();
    m_dataOut.flight.att_ey_wgs = m_aircraft->getAtt_WGS().ey();
    m_dataOut.flight.att_ez_wgs = m_aircraft->getAtt_WGS().ez();

    m_dataOut.flight.vel_u_bas = m_aircraft->getVel_BAS().x();
    m_dataOut.flight.vel_v_bas = m_aircraft->getVel_BAS().y();
    m_dataOut.flight.vel_w_bas = m_aircraft->getVel_BAS().z();

    m_dataOut.flight.omg_p_bas = m_aircraft->getOmg_BAS().x();
    m_dataOut.flight.omg_q_bas = m_aircraft->getOmg_BAS().y();
    m_dataOut.flight.omg_r_bas = m_aircraft->getOmg_BAS().z();

    m_dataOut.flight.phi_wgs = m_aircraft->getAngles_WGS().phi();
    m_dataOut.flight.tht_wgs = m_aircraft->getAngles_WGS().tht();
    m_dataOut.flight.psi_wgs = m_aircraft->getAngles_WGS().psi();

    m_dataOut.flight.tas_u_bas = m_aircraft->getVel_air_BAS().x();
    m_dataOut.flight.tas_v_bas = m_aircraft->getVel_air_BAS().y();
    m_dataOut.flight.tas_w_bas = m_aircraft->getVel_air_BAS().z();

    m_dataOut.flight.acc_x_bas = m_aircraft->getAcc_BAS().x();
    m_dataOut.flight.acc_y_bas = m_aircraft->getAcc_BAS().y();
    m_dataOut.flight.acc_z_bas = m_aircraft->getAcc_BAS().z();

    m_dataOut.flight.g_force_x = m_aircraft->getGForce().x();
    m_dataOut.flight.g_force_y = m_aircraft->getGForce().y();
    m_dataOut.flight.g_force_z = m_aircraft->getGForce().z();

    m_dataOut.flight.g_pilot_x = m_aircraft->getGPilot().x();
    m_dataOut.flight.g_pilot_y = m_aircraft->getGPilot().y();
    m_dataOut.flight.g_pilot_z = m_aircraft->getGPilot().z();

    m_dataOut.flight.onGround = m_aircraft->getGear()->getOnGround();
    m_dataOut.flight.stall    = m_aircraft->getAero()->getStall();

    // propulsion
    for ( int i = 0; i < FDM_MAX_ENGINES; i++ )
    {
        m_dataOut.engine[ i ].state = m_dr.engineOn[ i ].getDatab();
        m_dataOut.engine[ i ].rpm   = (float)m_dr.engineRPM[ i ].getValue();
        m_dataOut.engine[ i ].prop  = 0.0f;
        m_dataOut.engine[ i ].ng    = 0.0f;
        m_dataOut.engine[ i ].n1    = 0.0f;
        m_dataOut.engine[ i ].n2    = 0.0f;
        m_dataOut.engine[ i ].trq   = 0.0f;
        m_dataOut.engine[ i ].epr   = 0.0f;
        m_dataOut.engine[ i ].map   = (float)m_dr.engineMAP[ i ].getValue();
        m_dataOut.engine[ i ].egt   = 0.0f;
        m_dataOut.engine[ i ].itt   = 0.0f;
        m_dataOut.engine[ i ].ff    = (float)m_dr.engineFF[ i ].getValue();
    }

    // crash
    m_dataOut.crash = m_aircraft->getCrash();
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updateDataOutput_C130()
{
    // TODO
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updateDataOutput_C172()
{
    // TODO
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updateDataOutput_F16C()
{
    // TODO
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updateDataOutput_UH60()
{
    // TODO
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updateInitialPositionAndAttitude()
{
    double altitude_asl = m_dataInp.initial.altitude_agl + m_dataInp.ground.elevation;

    if ( m_dataInp.initial.altitude_agl < FDM_MIN_INIT_ALTITUDE )
    {
        altitude_asl = 1.0 + m_dataInp.ground.elevation;
    }

    WGS84::Geo pos_geo;

    pos_geo.lat = m_dataInp.initial.latitude;
    pos_geo.lon = m_dataInp.initial.longitude;
    pos_geo.alt = altitude_asl;

    Quaternion ned2bas( Angles( 0.0, 0.0, m_dataInp.initial.heading ) );

    WGS84 wgs( pos_geo );

    m_init_pos_wgs = wgs.getPos_WGS();
    m_init_att_wgs = wgs.getWGS2BAS( ned2bas );
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updateInternalPhaseInp()
{
    switch ( m_dataInp.phaseInp )
    {
    case DataInp::Idle:
        if ( m_stateOut == DataOut::Idle
          || m_stateOut == DataOut::Stopped )
        {
            m_phaseInp = DataInp::Idle;
        }
        else
        {
            m_phaseInp = DataInp::Stop;
        }
        break;

    case DataInp::Init:
        if ( m_stateOut == DataOut::Idle
          || m_stateOut == DataOut::Initializing
          || m_stateOut == DataOut::Ready )
        {
            m_phaseInp = DataInp::Init;
        }
        else
        {
            m_phaseInp = DataInp::Stop;
        }
        break;

    case DataInp::Work:
        if ( m_stateOut == DataOut::Ready
          || m_stateOut == DataOut::Working
          || m_stateOut == DataOut::Paused )
        {
            m_phaseInp = DataInp::Work;
        }
        else if ( m_stateOut == DataOut::Idle )
        {
            m_phaseInp = DataInp::Init;
        }
        else if ( m_stateOut == DataOut::Stopped )
        {
            m_phaseInp = DataInp::Stop;
        }
        else
        {
            m_phaseInp = DataInp::Idle;
        }
        break;

    case DataInp::Pause:
        if ( m_stateOut == DataOut::Ready
          || m_stateOut == DataOut::Working
          || m_stateOut == DataOut::Paused )
        {
            m_phaseInp = DataInp::Pause;
        }
        else if ( m_stateOut == DataOut::Idle )
        {
            m_phaseInp = DataInp::Init;
        }
        else
        {
            m_phaseInp = DataInp::Idle;
        }
        break;

    default:
        m_phaseInp = DataInp::Stop;
    }
}
////////////////////////////////////////////////////////////////////////////////

void Manager::updatePhaseIdle()
{
    Atmosphere atmosphere;

    updateInitialPositionAndAttitude();

    Angles angles_wgs = m_init_att_wgs.getAngles();

    double altitude_asl = m_dataInp.initial.altitude_agl + m_dataInp.ground.elevation;
    double altitude_agl = m_dataInp.initial.altitude_agl;
    bool onGround = false;

    if ( m_dataInp.initial.altitude_agl < FDM_MIN_INIT_ALTITUDE )
    {
        altitude_asl = WGS84( m_init_pos_wgs ).getPos_Geo().alt;
        altitude_agl = altitude_asl - m_dataInp.ground.elevation;
        onGround = true;
    }

    atmosphere.update( altitude_asl );

    // flight data
    m_dataOut.flight.latitude  = m_dataInp.initial.latitude;
    m_dataOut.flight.longitude = m_dataInp.initial.longitude;

    m_dataOut.flight.altitude_asl = altitude_asl;
    m_dataOut.flight.altitude_agl = altitude_agl;

    m_dataOut.flight.roll    = 0.0;
    m_dataOut.flight.pitch   = 0.0;
    m_dataOut.flight.heading = m_dataInp.initial.heading;

    m_dataOut.flight.angleOfAttack = 0.0;
    m_dataOut.flight.sideslipAngle = 0.0;

    m_dataOut.flight.course    = m_dataInp.initial.heading;
    m_dataOut.flight.pathAngle = 0.0;

    m_dataOut.flight.slipSkidAngle = 0.0;

    m_dataOut.flight.airspeed   = m_dataInp.initial.airspeed;
    m_dataOut.flight.machNumber = m_dataInp.initial.airspeed / atmosphere.getSpeedOfSound();
    m_dataOut.flight.climbRate  = 0.0;

    m_dataOut.flight.rollRate  = 0.0;
    m_dataOut.flight.pitchRate = 0.0;
    m_dataOut.flight.yawRate   = 0.0;
    m_dataOut.flight.turnRate  = 0.0;

    m_dataOut.flight.pos_x_wgs = m_init_pos_wgs.x();
    m_dataOut.flight.pos_y_wgs = m_init_pos_wgs.y();
    m_dataOut.flight.pos_z_wgs = m_init_pos_wgs.z();

    m_dataOut.flight.att_e0_wgs = m_init_att_wgs.e0();
    m_dataOut.flight.att_ex_wgs = m_init_att_wgs.ex();
    m_dataOut.flight.att_ey_wgs = m_init_att_wgs.ey();
    m_dataOut.flight.att_ez_wgs = m_init_att_wgs.ez();

    m_dataOut.flight.vel_u_bas = m_dataInp.initial.airspeed;
    m_dataOut.flight.vel_v_bas = 0.0;
    m_dataOut.flight.vel_w_bas = 0.0;

    m_dataOut.flight.omg_p_bas = 0.0;
    m_dataOut.flight.omg_q_bas = 0.0;
    m_dataOut.flight.omg_r_bas = 0.0;

    m_dataOut.flight.phi_wgs = angles_wgs.phi();
    m_dataOut.flight.tht_wgs = angles_wgs.tht();
    m_dataOut.flight.psi_wgs = angles_wgs.psi();

    m_dataOut.flight.tas_u_bas = m_dataInp.initial.airspeed;
    m_dataOut.flight.tas_v_bas = 0.0;
    m_dataOut.flight.tas_w_bas = 0.0;

    m_dataOut.flight.acc_x_bas = 0.0;
    m_dataOut.flight.acc_y_bas = 0.0;
    m_dataOut.flight.acc_z_bas = 0.0;

    m_dataOut.flight.g_force_x = 0.0;
    m_dataOut.flight.g_force_y = 0.0;
    m_dataOut.flight.g_force_z = 1.0;

    m_dataOut.flight.g_pilot_x = 0.0;
    m_dataOut.flight.g_pilot_y = 0.0;
    m_dataOut.flight.g_pilot_z = 1.0;

    m_dataOut.flight.onGround = onGround;
    m_dataOut.flight.stall = false;

    // propulsion
    for ( int i = 0; i < FDM_MAX_ENGINES; i++ )
    {
        m_dataOut.engine[ i ].state = false;
        m_dataOut.engine[ i ].rpm   = 0.0;
        m_dataOut.engine[ i ].prop  = 0.0;
        m_dataOut.engine[ i ].ng    = 0.0;
        m_dataOut.engine[ i ].n1    = 0.0;
        m_dataOut.engine[ i ].n2    = 0.0;
        m_dataOut.engine[ i ].trq   = 0.0;
        m_dataOut.engine[ i ].epr   = 0.0;
        m_dataOut.engine[ i ].map   = 0.0;
        m_dataOut.engine[ i ].egt   = 0.0;
        m_dataOut.engine[ i ].itt   = 0.0;
        m_dataOut.engine[ i ].ff    = 0.0;
    }

    // crash
    m_dataOut.crash = Aircraft::NoCrash;

    m_stateOut = DataOut::Idle;
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updatePhaseInit()
{
    if ( m_stateOut != DataOut::Ready )
    {
        try
        {
            if ( m_aircraft == 0 )
            {
                m_stateOut = DataOut::Initializing;
                m_initStep = 0;

                switch ( m_aircraftType )
                {
                case DataInp::C130:
                    m_aircraft = new C130_Aircraft();
                    break;

                case DataInp::C172:
                    m_aircraft = new C172_Aircraft();
                    break;

                case DataInp::F16C:
                    m_aircraft = new F16C_Aircraft();
                    break;

                case DataInp::UH60:
                    m_aircraft = new UH60_Aircraft();
                    break;
                }

                if ( m_aircraft != 0 )
                {
                    dataRefsInit();

                    m_aircraft->getProp()->initialize( m_dataInp.initial.engineOn );
                }
            }

            initAircraft();
        }
        catch ( Exception &e )
        {
            std::cerr << e.getInfo() << std::endl;

            while ( e.hasCause() )
            {
                e = e.getCause();
                std::cerr << e.getInfo() << std::endl;
            }

            m_stateOut = DataOut::Stopped;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updatePhaseWork()
{
    if ( m_aircraft != 0 )
    {
        try
        {
            updateDataInput();
            m_aircraft->step( m_timeStep );
            updateDataOutput();

            if ( Aircraft::NoCrash == m_aircraft->getCrash() )
            {
                m_stateOut = DataOut::Working;
            }
            else
            {
                m_stateOut = DataOut::Stopped;
            }
        }
        catch ( Exception &e )
        {
            std::cerr << e.getInfo() << std::endl;

            while ( e.hasCause() )
            {
                e = e.getCause();
                std::cerr << e.getInfo() << std::endl;
            }

            m_stateOut = DataOut::Stopped;
        }
    }
    else
    {
        m_stateOut = DataOut::Stopped;
    }
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updatePhasePause()
{
    m_stateOut = DataOut::Paused;
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updatePhaseStop()
{
    if ( m_aircraft ) delete m_aircraft;
    m_aircraft = 0;

    dataRefsReset();

    m_stateOut = DataOut::Stopped;
}
