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

#include <fdm/utils/fdm_String.h>

#include <fdm_c130/c130_Aircraft.h>
#include <fdm_c172/c172_Aircraft.h>
#include <fdm_f16/f16_Aircraft.h>
#include <fdm_uh60/uh60_Aircraft.h>

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

    m_timeSteps ( 0 ),

    m_verbose ( true )
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

        for ( UInt32 i = 0; i < 1000 && m_initStep < FDM_MAX_INIT_STEPS && m_stateOut != DataOut::Ready; i++  )
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
            if ( m_aircraft->getGear()->getFor_BAS().getLength2() > 0.0 )
            {
                m_init_phi += dp_dt * coef;
                m_init_tht += dq_dt * coef;
                m_init_alt += dn_dt * coef;
            }
            else
            {
                m_init_alt += dn_dt * coef;
            }

            if ( m_init_alt > 0.0
              && fabs( dp_dt ) < 1.0e-3
              && fabs( dq_dt ) < 1.0e-3
              && fabs( dn_dt ) < 1.0e-3 )
            {
                m_stateOut = DataOut::Ready;

                if ( m_verbose )
                {
                    std::cout << "On-ground initialization finished in " << m_initStep << " steps" << std::endl;
                }
            }
            else
            {
                m_initStep++;
            }
        }
    }
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
    m_dataOut.flight.machNumber = m_dataInp.initial.airspeed / 340.29;
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

    // controls
    m_dataOut.controls.ailerons  = 0.0;
    m_dataOut.controls.elevator  = 0.0;
    m_dataOut.controls.elevons   = 0.0;
    m_dataOut.controls.rudder    = 0.0;
    m_dataOut.controls.flaps     = 0.0;
    m_dataOut.controls.flaperons = 0.0;
    m_dataOut.controls.lef       = 0.0;
    m_dataOut.controls.airbrake  = 0.0;

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
        m_dataOut.engine[ i ].tit   = 0.0;
        m_dataOut.engine[ i ].ff    = 0.0;
    }

    // rotor
    m_dataOut.rotor.mainRotor_azimuth     = 0.0;
    m_dataOut.rotor.mainRotor_coningAngle = 0.0;
    m_dataOut.rotor.mainRotor_diskRoll    = 0.0;
    m_dataOut.rotor.mainRotor_diskPitch   = 0.0;
    m_dataOut.rotor.mainRotor_collective  = 0.0;
    m_dataOut.rotor.mainRotor_cyclicLon   = 0.0;
    m_dataOut.rotor.mainRotor_cyclicLat   = 0.0;
    m_dataOut.rotor.tailRotor_azimuth     = 0.0;

    // crash
    m_dataOut.crash = DataOut::NoCrash;

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
                    m_aircraft = new C130_Aircraft( &m_dataInp, &m_dataOut );
                    break;

                case DataInp::C172:
                    m_aircraft = new C172_Aircraft( &m_dataInp, &m_dataOut );
                    break;

                case DataInp::F16:
                    m_aircraft = new F16_Aircraft( &m_dataInp, &m_dataOut );
                    break;

                case DataInp::UH60:
                    m_aircraft = new UH60_Aircraft( &m_dataInp, &m_dataOut );
                    break;
                }

                if ( m_aircraft != 0 )
                {
                    m_aircraft->init( m_dataInp.initial.engineOn );
                }
            }

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

                m_aircraft->dataOutput();
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
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updatePhaseWork()
{
    if ( m_aircraft != 0 )
    {
        try
        {
            m_realTime += m_timeStep;
            m_timeSteps++;

            m_aircraft->step( m_timeStep );
            m_aircraft->dataOutput();

            if ( DataOut::NoCrash == m_aircraft->getCrash() )
            {
                m_stateOut = DataOut::Working;
            }
            else
            {
                m_stateOut = DataOut::Stopped;

                if ( m_verbose )
                {
                    switch ( m_aircraft->getCrash() )
                    {
                    case fdm::DataOut::Collision:
                        std::cout << "CRASH: Collision with terrain or obstacle." << std::endl;
                        break;

                    case fdm::DataOut::Overspeed:
                        std::cout << "CRASH: Airspeed too high. Airspeed= " << m_aircraft->getAirspeed() << " [m/s]" << std::endl;
                        break;

                    case fdm::DataOut::Overstressed:
                        std::cout << "CRASH: Load factor too high. Gz= " << m_aircraft->getGForce().z() << std::endl;
                        break;

                    default:
                        std::cout << "CRASH: Unknown crash cause." << std::endl;
                        break;
                    }
                }
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
    if ( m_verbose )
    {
        if ( m_timeSteps > 0 )
        {
            double meanStep = m_realTime / (double)m_timeSteps;
            double meanFreq = 1.0 / meanStep;
            std::cout << "Mean time step: " << meanStep << " s"  << std::endl;
            std::cout << "Mean frequency: " << meanFreq << " Hz" << std::endl;
        }
    }

    m_realTime = 0.0;
    m_timeSteps = 0;

    if ( m_aircraft ) delete m_aircraft;
    m_aircraft = 0;

    m_stateOut = DataOut::Stopped;
}
