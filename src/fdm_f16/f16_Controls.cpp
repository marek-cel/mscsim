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

#include <fdm_f16/f16_Aircraft.h>

#include <fdmUtils/fdm_Units.h>
#include <fdmXml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

F16_Controls::F16_Controls( const F16_Aircraft *aircraft ) :
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

    m_lgHandle   ( false ),
    m_nwSteering ( false ),

    m_angleOfAttack ( 0.0 ),
    m_g_y ( 0.0 ),
    m_g_z ( 0.0 ),
    m_rollRate ( 0.0 ),
    m_pitchRate ( 0.0 ),
    m_yawRate ( 0.0 ),
    m_ctrlLat ( 0.0 ),
    m_trimLat ( 0.0 ),
    m_ctrlLon ( 0.0 ),
    m_trimLon ( 0.0 ),
    m_ctrlYaw ( 0.0 ),
    m_trimYaw ( 0.0 ),
    m_statPress ( 0.0 ),
    m_dynPress ( 0.0 )
{
    m_flcs = new F16_FLCS();
}

////////////////////////////////////////////////////////////////////////////////

F16_Controls::~F16_Controls()
{
    if ( m_flcs ) delete m_flcs;
    m_flcs = 0;
}

////////////////////////////////////////////////////////////////////////////////

void F16_Controls::readData( XmlNode &dataNode )
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

void F16_Controls::init()
{
    /////////////////
    Controls::init();
    /////////////////

    int result = FDM_SUCCESS;

    // inputs
    if ( result == FDM_SUCCESS ) result = addDataRef( "output/controls/ailerons"  , DataNode::Double );
    if ( result == FDM_SUCCESS ) result = addDataRef( "output/controls/elevator"  , DataNode::Double );
    if ( result == FDM_SUCCESS ) result = addDataRef( "output/controls/elevons"   , DataNode::Double );
    if ( result == FDM_SUCCESS ) result = addDataRef( "output/controls/rudder"    , DataNode::Double );
    if ( result == FDM_SUCCESS ) result = addDataRef( "output/controls/flaps"     , DataNode::Double );
    if ( result == FDM_SUCCESS ) result = addDataRef( "output/controls/flaperons" , DataNode::Double );
    if ( result == FDM_SUCCESS ) result = addDataRef( "output/controls/lef"       , DataNode::Double );
    if ( result == FDM_SUCCESS ) result = addDataRef( "output/controls/airbrake"  , DataNode::Double );

    if ( result == FDM_SUCCESS )
    {
        m_outElevator  = getDataRef( "output/controls/elevator"  );
        m_outElevons   = getDataRef( "output/controls/elevons"   );
        m_outRudder    = getDataRef( "output/controls/rudder"    );
        m_outFlaps     = getDataRef( "output/controls/flaps"     );
        m_outFlaperons = getDataRef( "output/controls/flaperons" );
        m_outLEF       = getDataRef( "output/controls/lef"       );
        m_outAirbrake  = getDataRef( "output/controls/airbrake"  );
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

    if ( FDM_SUCCESS == addDataRef( "input/controls/lg_handle"   , DataNode::Bool )
      && FDM_SUCCESS == addDataRef( "input/controls/nw_steering" , DataNode::Bool ) )
    {
        m_drLgHandle   = getDataRef( "input/controls/lg_handle" );
        m_drNwSteering = getDataRef( "input/controls/nw_steering" );
    }
    else
    {
        Exception e;

        e.setType( Exception::UnknownException );
        e.setInfo( "ERROR! Creating data references failed." );

        FDM_THROW( e );
    }
}

////////////////////////////////////////////////////////////////////////////////

void F16_Controls::update()
{
    ///////////////////
    Controls::update();
    ///////////////////

    m_airbrake_norm = m_channelAirbrake->output;
    m_airbrake = m_airbrake_norm * m_airbrake_max;

    m_brake_l = m_channelBrakeL->output;
    m_brake_r = m_channelBrakeR->output;

    m_nose_wheel = m_channelNoseWheel->output;

    m_lgHandle   = m_drLgHandle   .getDatab();
    m_nwSteering = m_drNwSteering .getDatab();

    // 1000 Hz
    const unsigned int steps = ceil( m_aircraft->getTimeStep() / 0.001 );

    const double timeStep = m_aircraft->getTimeStep() / ( (double)steps );
    const double delta_angleOfAttack = m_aircraft->getAngleOfAttack()  - m_angleOfAttack;
    const double delta_g_y = m_aircraft->getGForce().y() - m_g_y;
    const double delta_g_z = m_aircraft->getGForce().z() - m_g_z;
    const double delta_rollRate  = m_aircraft->getOmg_BAS()( i_p ) - m_rollRate;
    const double delta_pitchRate = m_aircraft->getOmg_BAS()( i_q ) - m_pitchRate;
    const double delta_yawRate   = m_aircraft->getOmg_BAS()( i_r ) - m_yawRate;
    const double delta_ctrlLat = m_channelRoll->output      - m_ctrlLat;
    const double delta_trimLat = m_channelRollTrim->output  - m_trimLat;
    const double delta_ctrlLon = m_channelPitch->output     - m_ctrlLon;
    const double delta_trimLon = m_channelPitchTrim->output - m_trimLon;
    const double delta_ctrlYaw = m_channelYaw->output       - m_ctrlYaw;
    const double delta_trimYaw = m_channelYawTrim->output   - m_trimYaw;
    const double delta_statPress = m_aircraft->getEnvir()->getPressure() - m_statPress;
    const double delta_dynPress  = m_aircraft->getDynPress()             - m_dynPress;

    for ( unsigned int i = 0; i < steps; i++ )
    {
        const double coef = ( (double)( i + 1 ) ) / ( (double)steps );

        double angleOfAttack = m_angleOfAttack + coef * delta_angleOfAttack;
        double g_y = m_g_y + coef * delta_g_y;
        double g_z = m_g_z + coef * delta_g_z;
        double rollRate  = m_rollRate  + coef * delta_rollRate;
        double pitchRate = m_pitchRate + coef * delta_pitchRate;
        double yawRate   = m_yawRate   + coef * delta_yawRate;
        double ctrlLat   = m_ctrlLat   + coef * delta_ctrlLat;
        double trimLat   = m_trimLat   + coef * delta_trimLat;
        double ctrlLon   = m_ctrlLon   + coef * delta_ctrlLon;
        double trimLon   = m_trimLon   + coef * delta_trimLon;
        double ctrlYaw   = m_ctrlYaw   + coef * delta_ctrlYaw;
        double trimYaw   = m_trimYaw   + coef * delta_trimYaw;
        double statPress = m_statPress + coef * delta_statPress;
        double dynPress  = m_dynPress  + coef * delta_dynPress;

        m_flcs->update( timeStep, angleOfAttack,
                        g_y, g_z,
                        rollRate, pitchRate, yawRate,
                        ctrlLat, trimLat,
                        ctrlLon, trimLon,
                        ctrlYaw, trimYaw,
                        statPress, dynPress,
                        false, false, m_lgHandle, m_aircraft->getGear()->getOnGround() );

//        m_flcs->update( m_aircraft->getTimeStep(),
//                        m_aircraft->getAngleOfAttack(),
//                        m_aircraft->getGForce().y(), m_aircraft->getGForce().z(),
//                        m_aircraft->getOmg_BAS()( i_p ), m_aircraft->getOmg_BAS()( i_q ), m_aircraft->getOmg_BAS()( i_r ),
//                        m_channelRoll->output  , m_channelRollTrim->output,
//                        m_channelPitch->output , m_channelPitchTrim->output,
//                        m_channelYaw->output   , m_channelYawTrim->output,
//                        m_aircraft->getEnvir()->getPressure(), m_aircraft->getDynPress(),
//                        false, false, m_aircraft->getGear()->isDown() );
    }

    m_angleOfAttack = m_aircraft->getAngleOfAttack();
    m_g_y = m_aircraft->getGForce().y();
    m_g_z = m_aircraft->getGForce().z();
    m_rollRate  = m_aircraft->getOmg_BAS()( i_p );
    m_pitchRate = m_aircraft->getOmg_BAS()( i_q );
    m_yawRate   = m_aircraft->getOmg_BAS()( i_r );
    m_ctrlLat = m_channelRoll->output;
    m_trimLat = m_channelRollTrim->output;
    m_ctrlLon = m_channelPitch->output;
    m_trimLon = m_channelPitchTrim->output;
    m_ctrlYaw = m_channelYaw->output;
    m_trimYaw = m_channelYawTrim->output;
    m_statPress = m_aircraft->getEnvir()->getPressure();
    m_dynPress  = m_aircraft->getDynPress();

    m_outElevator  .setDatad( m_flcs->getElevator() );
    m_outElevons   .setDatad( m_flcs->getElevons() );
    m_outRudder    .setDatad( m_flcs->getRudder() );
    m_outFlaps     .setDatad( m_flcs->getFlapsTE() );
    m_outFlaperons .setDatad( m_flcs->getAilerons() ); // sic!
    m_outLEF       .setDatad( m_flcs->getFlapsLE() );
    m_outAirbrake  .setDatad( m_airbrake );
}
