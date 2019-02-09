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

    m_ailerons_max ( 0.0 ),
    m_elevator_max ( 0.0 ),
    m_rudder_max   ( 0.0 ),
    m_flaps_le_max ( 0.0 ),
    m_airbrake_max ( 0.0 ),

    m_ailerons      ( 0.0 ),
    m_ailerons_norm ( 0.0 ),
    m_elevator      ( 0.0 ),
    m_elevator_norm ( 0.0 ),
    m_rudder        ( 0.0 ),
    m_rudder_norm   ( 0.0 ),
    m_flaps_le      ( 0.0 ),
    m_flaps_le_norm ( 0.0 ),
    m_airbrake      ( 0.0 ),
    m_airbrake_norm ( 0.0 ),
    m_brake_l       ( 0.0 ),
    m_brake_r       ( 0.0 ),
    m_nose_wheel    ( 0.0 ),

    m_nwSteering ( false ),

    m_cat   ( CAT_I  ),
    m_gains ( Cruise ),

    m_alpha_deg ( 0.0 ),
    m_pitch_int ( 0.0 )
{}

////////////////////////////////////////////////////////////////////////////////

F16C_Controls::~F16C_Controls()
{}

////////////////////////////////////////////////////////////////////////////////

void F16C_Controls::readData( XmlNode &dataNode )
{
    ///////////////////////////////
    Controls::readData( dataNode );
    ///////////////////////////////

    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        m_flaps_le_max = 0.0;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_ailerons_max , "ailerons_max" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_elevator_max , "elevator_max" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_rudder_max   , "rudder_max"   );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_flaps_le_max , "flaps_le_max" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_airbrake_max , "airbrake_max" );


        if ( result != FDM_SUCCESS )
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

    updateAFCS();

    m_outputAilerons  .setDatad( m_ailerons );
    m_outputElevator  .setDatad( m_elevator );
    m_outputRudder    .setDatad( m_rudder   );
    m_outputFlaps     .setDatad( 0.0 );
    m_outputFlapsLE   .setDatad( m_flaps_le );
    m_outputAirbrake  .setDatad( m_airbrake );
}

////////////////////////////////////////////////////////////////////////////////

void F16C_Controls::updateAFCS()
{
    m_alpha_deg = Units::rad2deg( m_aircraft->getAngleOfAttack() );

    // TODO: Aileron Rudder Interconnect (ARI), rudder authority limiter, yaw rate limiter

    // TODO (GR1F-16CJ-1, p.1-127)
//    if ( ( Units::mps2kts( m_aircraft->getAirspeed() ) < 400.0 && ( alt_flaps_extend || air_refuel_open ) )
//      || landing_gear_dn )
//    {
//        m_gains = Takeoff;
//    }
//    else
//    {
//        m_gains = Cruise;
//    }

    updateAFCS_Flaps();
    updateAFCS_Pitch(); // before roll
    updateAFCS_Roll();
    updateAFCS_Yaw();
}

////////////////////////////////////////////////////////////////////////////////

void F16C_Controls::updateAFCS_Flaps()
{
    // NASA-TP-1538, p.34
    // d_lef = 1.38 (2s+7.25)/(s+7.25) AoA - 9.05 q/p_s + 1.45
    double flaps_le_deg = 1.38 * ( m_alpha_deg )
            - 9.05 * m_aircraft->getDynPress() / m_aircraft->getEnvir()->getPressure() + 1.45;

    m_flaps_le = Misc::satur( 0.0, m_flaps_le_max, Units::deg2rad( flaps_le_deg ) );
    m_flaps_le_norm = m_flaps_le / m_flaps_le_max;
}

////////////////////////////////////////////////////////////////////////////////

void F16C_Controls::updateAFCS_Pitch()
{
    // TODO lags
    double q_com = 0.0;

    if ( m_gains == Cruise )
    {
        double g_com = ( m_aircraft->getGear()->getOnGround() ? 1.0 : 0.5 ) * getGCommand();
        q_com = g_com - ( m_aircraft->getGForce().z() - 1.0 );
    }
    else
    {
        // TODO pitch rate (Takeoff gains)
    }

    // Marchand, p.21
    double delta_q = 0.167 * m_aircraft->getOmg_BAS()( i_q ) - q_com;

    // NASA-TP-1538, p.210
    // Marchand, p.20
    // TO 1F-16A-1, p.1-124
    double pitch_u = 3.0 * getPitchLoopGain() * Units::deg2rad( delta_q );
    m_pitch_int = m_pitch_int + m_aircraft->getTimeStep() * pitch_u;
    m_pitch_int = Misc::satur( -m_elevator_max, m_elevator_max, m_pitch_int );
    double elevator_rad = pitch_u + m_pitch_int;

    // NASA-TP-1538, p.210
    double elevator_delta_max = Units::deg2rad( 60.0 ) * m_aircraft->getTimeStep();
    if ( fabs( elevator_rad - m_elevator ) < elevator_delta_max )
    {
        m_elevator = elevator_rad;
    }
    else
    {
        m_elevator += Misc::sign( elevator_rad - m_elevator ) * elevator_delta_max;
    }

    m_elevator = Misc::satur( -m_elevator_max, m_elevator_max, m_elevator );
    m_elevator_norm = m_elevator / m_elevator_max;
}

////////////////////////////////////////////////////////////////////////////////

void F16C_Controls::updateAFCS_Roll()
{
    // TODO according to scheme
    double p_com = getRollRateCommand( Units::rad2deg( m_elevator ) );
    double delta_p = p_com - m_aircraft->getOmg_BAS()( i_p );
    double ailerons_deg = -0.12 * Units::rad2deg( delta_p );
    double ailerons_rad = Units::deg2rad( ailerons_deg );

    m_ailerons = Misc::satur( -m_ailerons_max, m_ailerons_max, ailerons_rad );
    m_ailerons_norm = m_ailerons / m_ailerons_max;
}

////////////////////////////////////////////////////////////////////////////////

void F16C_Controls::updateAFCS_Yaw()
{

}

////////////////////////////////////////////////////////////////////////////////

double F16C_Controls::getGCommand()
{
    // negative g limit
    // NASA-TP-1538, p.211 - 62(b)
    // Marchand, p.20 - F1
    double g_min = -4.0;
    if ( m_aircraft->getDynPress() < 1628.0 ) // 34 psf
    {
        g_min = -1.0;
    }
    else if ( m_aircraft->getDynPress() < 8810.0 ) // 184 psf
    {
        g_min = -1.0 - ( 3.0 / ( 8810.0 - 1628.0 ) ) * ( m_aircraft->getDynPress() - 1628.0 );
    }

    double g_max = getMaxG();

    // NASA-TP-1538, p.210
    double g_com = -9.0 * m_channelPitch->output - 2.4 * m_channelPitchTrim->output;

    return Misc::satur( g_min, g_max, g_com );
}

////////////////////////////////////////////////////////////////////////////////

double F16C_Controls::getMaxG()
{
    // AoA/G Limiter
    // NASA-TP-1538, p.34
    // Droste, p.94
    // GR1F-16CJ-1, p.1-125
    double g_max = 9.0;

    if ( m_cat == CAT_I )
    {
        if ( m_alpha_deg >= 25.2 )
        {
            g_max = 0.0;
        }
        else if ( m_alpha_deg >= 20.4 )
        {
            g_max = 7.2612 - ( m_alpha_deg - 20.4 ) * 1.322;
        }
        else if ( m_alpha_deg >= 15.0 )
        {
            g_max = 9.0 - ( m_alpha_deg - 15.0 ) * 0.322;
        }
    }
    else
    {
        // TODO
    }


    return g_max;
}

////////////////////////////////////////////////////////////////////////////////

double F16C_Controls::getPitchRateGain()
{
    // NASA-TP-1538, p.211 - 62(c)
    // Marchand, p.20 - F3
}

////////////////////////////////////////////////////////////////////////////////

double F16C_Controls::getPitchLoopGain()
{
    // NASA-TP-1538, p.211 - 62(d)
    // Marchand, p.20 - F3
    double gain = 1.0;

    if ( m_aircraft->getDynPress() > 143640.0 ) // 3,000 psf
    {
        gain = 0.083;
    }
    else if ( m_aircraft->getDynPress() > 38304.0 ) // 800 psf
    {
        gain = 0.533 - ( 0.533 - 0.083 ) / ( 143640.0 - 38304.0 ) * ( m_aircraft->getDynPress() - 38304.0 );
    }
    else if ( m_aircraft->getDynPress() > 14364.0 ) // 300 psf
    {
        gain = 1.0 - ( 1.0 - 0.533 ) / ( 38304.0 - 14364.0 ) * ( m_aircraft->getDynPress() - 14364.0 );
    }

    return gain;
}

////////////////////////////////////////////////////////////////////////////////

double F16C_Controls::getRollRateCommand( double delta_h )
{
    // NASA-TP-1538, p.214
    double p_com_pilot = -308.0 * m_channelRoll->output - 40.0 * m_channelRollTrim->output * 1.67;

    // NASA-TP-1538, p.129
    // Droste, p.99
    // GR1F-16CJ-1, p.1-125
    double delta_p_max = -0.0115 * std::min( m_aircraft->getDynPress() - 10500.0, 0.0 )
            + 4.0 * std::max( m_alpha_deg - 15.0, 0.0 )
            + 4.0 * std::max( delta_h     -  5.0, 0.0 );
            // TODO Total rudder command (from pilot and FLCS) exceeding 20deg (GR1F-16CJ-1, p.1-125)
            // TODO Combination of horizontal tail greater than 15deg trailing edge down and AoA above 22deg (GR1F-16CJ-1, p.1-125)

    double p_max = 308.0 - std::min( delta_p_max, 228.0 );

    if ( m_cat == CAT_III )
    {
        p_max *= 0.6;
    }

    return Units::deg2rad( Misc::satur( -p_max, p_max, p_com_pilot ) );
}
