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

#include <fdm_f16c/f16c_Aircraft.h>

#include <fdmUtils/fdm_Units.h>
#include <fdmXml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

F16C_Controls::F16C_Controls( const F16C_Aircraft *aircraft ) :
    Controls( aircraft ),
    m_aircraft ( aircraft ),

    m_channelRoll      ( 0 ),
    m_channelPitch     ( 0 ),
    m_channelYaw       ( 0 ),
    m_channelRollTrim  ( 0 ),
    m_channelPitchTrim ( 0 ),
    m_channelYawTrim   ( 0 ),
    m_channelAirbrake  ( 0 ),
    m_channelBrakeL    ( 0 ),
    m_channelBrakeR    ( 0 ),
    m_channelNoseWheel ( 0 ),

    m_flcs ( 0 ),

    m_airbrake_max ( 0.0 ),

    m_airbrake      ( 0.0 ),
    m_airbrake_norm ( 0.0 ),
    m_brake_l       ( 0.0 ),
    m_brake_r       ( 0.0 ),
    m_nose_wheel    ( 0.0 ),

    m_nwSteering ( false ),

    m_angleOfAttack ( 0.0 ),
    m_gz            ( 0.0 ),
    m_rollRate      ( 0.0 ),
    m_pitchRate     ( 0.0 ),
    m_stickLat      ( 0.0 ),
    m_stickLon      ( 0.0 ),
    m_trimLat       ( 0.0 ),
    m_trimLon       ( 0.0 ),
    m_staticPress   ( 0.0 ),
    m_dynPress      ( 0.0 )
{
    m_flcs = new F16C_FLCS();
}

////////////////////////////////////////////////////////////////////////////////

F16C_Controls::~F16C_Controls()
{
    if ( m_flcs ) delete m_flcs;
    m_flcs = 0;
}

////////////////////////////////////////////////////////////////////////////////

void F16C_Controls::readData( XmlNode &dataNode )
{
    ///////////////////////////////
    Controls::readData( dataNode );
    ///////////////////////////////

    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        double ailerons_max = 0.0;
        double elevator_max = 0.0;
        double rudder_max   = 0.0;
        double flaps_le_max = 0.0;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, ailerons_max , "ailerons_max" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, elevator_max , "elevator_max" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, rudder_max   , "rudder_max"   );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, flaps_le_max , "flaps_le_max" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_airbrake_max, "airbrake_max" );

        if ( result == FDM_SUCCESS )
        {
            m_flcs->setAilerons_max( ailerons_max );
            m_flcs->setElevator_max( elevator_max );
            m_flcs->setRudder_max( rudder_max );
            m_flcs->setFlaps_le_max( flaps_le_max );
        }
        else
        {
            Exception e;

            e.setType( Exception::FileReadingError );
            e.setInfo( "ERROR! Reading XML file failed. " + XmlUtils::getErrorInfo( dataNode ) );

            FDM_THROW( e );
        }
    }
    else
    {
        Exception e;

        e.setType( Exception::FileReadingError );
        e.setInfo( "ERROR! Reading XML file failed. " + XmlUtils::getErrorInfo( dataNode ) );

        FDM_THROW( e );
    }
}

////////////////////////////////////////////////////////////////////////////////

void F16C_Controls::initDataRefs()
{
    /////////////////////////
    Controls::initDataRefs();
    /////////////////////////

    int result = FDM_SUCCESS;

    // inputs
    if ( result == FDM_SUCCESS ) result = addDataRef( "output/controls/ailerons" , DataNode::Double );
    if ( result == FDM_SUCCESS ) result = addDataRef( "output/controls/elevator" , DataNode::Double );
    if ( result == FDM_SUCCESS ) result = addDataRef( "output/controls/rudder"   , DataNode::Double );
    if ( result == FDM_SUCCESS ) result = addDataRef( "output/controls/flaps"    , DataNode::Double );
    if ( result == FDM_SUCCESS ) result = addDataRef( "output/controls/flaps_le" , DataNode::Double );
    if ( result == FDM_SUCCESS ) result = addDataRef( "output/controls/airbrake" , DataNode::Double );

    if ( result == FDM_SUCCESS )
    {
        m_outputAilerons  = getDataRef( "output/controls/ailerons" );
        m_outputElevator  = getDataRef( "output/controls/elevator" );
        m_outputRudder    = getDataRef( "output/controls/rudder"   );
        m_outputFlaps     = getDataRef( "output/controls/flaps"    );
        m_outputFlapsLE   = getDataRef( "output/controls/flaps_le" );
        m_outputAirbrake  = getDataRef( "output/controls/airbrake" );
    }
    else
    {
        Exception e;

        e.setType( Exception::UnknownException );
        e.setInfo( "ERROR! Initializing data references failed." );

        FDM_THROW( e );
    }

    m_channelRoll      = getChannelByName( "roll"       );
    m_channelPitch     = getChannelByName( "pitch"      );
    m_channelYaw       = getChannelByName( "yaw"        );
    m_channelRollTrim  = getChannelByName( "roll_trim"  );
    m_channelPitchTrim = getChannelByName( "pitch_trim" );
    m_channelYawTrim   = getChannelByName( "yaw_trim"   );
    m_channelAirbrake  = getChannelByName( "airbrake"   );
    m_channelBrakeL    = getChannelByName( "brake_l"    );
    m_channelBrakeR    = getChannelByName( "brake_r"    );
    m_channelNoseWheel = getChannelByName( "nose_wheel" );

    if ( 0 == m_channelRoll
      || 0 == m_channelPitch
      || 0 == m_channelYaw
      || 0 == m_channelRollTrim
      || 0 == m_channelPitchTrim
      || 0 == m_channelYawTrim
      || 0 == m_channelAirbrake
      || 0 == m_channelBrakeL
      || 0 == m_channelBrakeR
      || 0 == m_channelNoseWheel )
    {
        Exception e;

        e.setType( Exception::UnknownException );
        e.setInfo( "ERROR! Obtaining control channels failed." );

        FDM_THROW( e );
    }
}

////////////////////////////////////////////////////////////////////////////////

void F16C_Controls::update()
{
    ///////////////////
    Controls::update();
    ///////////////////

    m_airbrake_norm = m_channelAirbrake->output;
    m_airbrake = m_airbrake_norm * m_airbrake_max;

    m_brake_l = m_channelBrakeL->output;
    m_brake_r = m_channelBrakeR->output;

    m_nose_wheel = m_channelNoseWheel->output;
    m_nwSteering = m_drNwSteering.getDatab();

    int steps = 1;
    double timeStep = m_aircraft->getTimeStep() / (double)steps;
    for ( int i = 0; i < steps; i++ )
    {
        double coef = ( (double)i + 1.0 ) / (double)steps;

        double angleOfAttack = m_angleOfAttack + coef * ( m_aircraft->getAngleOfAttack()        - m_angleOfAttack  );
        double g_z           = m_gz            + coef * ( m_aircraft->getGForce().z()           - m_gz             );
        double rollRate      = m_rollRate      + coef * ( m_aircraft->getOmg_BAS()( i_p )       - m_rollRate       );
        double pitchRate     = m_pitchRate     + coef * ( m_aircraft->getOmg_BAS()( i_q )       - m_pitchRate      );
        double stickLat      = m_stickLat      + coef * ( m_channelRoll->output                 - m_stickLat       );
        double stickLon      = m_stickLon      + coef * ( m_channelPitch->output                - m_stickLon       );
        double trimLat       = m_trimLat       + coef * ( m_channelRollTrim->output             - m_trimLat        );
        double trimLon       = m_trimLon       + coef * ( m_channelPitchTrim->output            - m_trimLon        );
        double staticPress   = m_staticPress   + coef * ( m_aircraft->getEnvir()->getPressure() - m_staticPress    );
        double dynPress      = m_dynPress      + coef * ( m_aircraft->getDynPress()             - m_dynPress       );

        m_flcs->update( timeStep, angleOfAttack, g_z,
                        rollRate, pitchRate,
                        stickLat, trimLat,
                        stickLon, trimLon,
                        staticPress, dynPress );
    }

    m_angleOfAttack = m_aircraft->getAngleOfAttack();
    m_gz            = m_aircraft->getGForce().z();
    m_rollRate      = m_aircraft->getOmg_BAS()( i_p );
    m_pitchRate     = m_aircraft->getOmg_BAS()( i_q );
    m_stickLat      = m_channelRoll->output;
    m_stickLon      = m_channelPitch->output;
    m_trimLat       = m_channelRollTrim->output;
    m_trimLon       = m_channelPitchTrim->output;
    m_staticPress   = m_aircraft->getEnvir()->getPressure();
    m_dynPress      = m_aircraft->getDynPress();

    m_outputAilerons  .setDatad( m_flcs->getAilerons() );
    m_outputElevator  .setDatad( m_flcs->getElevator() );
    m_outputRudder    .setDatad( m_flcs->getRudder() );
    m_outputFlaps     .setDatad( 0.0 );
    m_outputFlapsLE   .setDatad( m_flcs->getFlapsLE() );
    m_outputAirbrake  .setDatad( m_airbrake );
}
