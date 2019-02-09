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

#include <fdm_c172/c172_Aircraft.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

C172_Controls::C172_Controls( const C172_Aircraft *aircraft ) :
    Controls( aircraft ),
    m_aircraft ( aircraft ),

    m_channelAilerons     ( 0 ),
    m_channelElevator     ( 0 ),
    m_channelRudder       ( 0 ),
    m_channelElevatorTrim ( 0 ),
    m_channelFlaps        ( 0 ),
    m_channelBrakeL       ( 0 ),
    m_channelBrakeR       ( 0 ),
    m_channelNoseWheel    ( 0 ),

    m_ailerons      ( 0.0 ),
    m_elevator      ( 0.0 ),
    m_rudder        ( 0.0 ),
    m_elevator_trim ( 0.0 ),
    m_flaps         ( 0.0 ),
    m_brake_l       ( 0.0 ),
    m_brake_r       ( 0.0 ),
    m_nose_wheel    ( 0.0 )
{}

////////////////////////////////////////////////////////////////////////////////

C172_Controls::~C172_Controls() {}

////////////////////////////////////////////////////////////////////////////////

void C172_Controls::initDataRefs()
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

    if ( result == FDM_SUCCESS )
    {
        m_outputAilerons  = getDataRef( "output/controls/ailerons" );
        m_outputElevator  = getDataRef( "output/controls/elevator" );
        m_outputRudder    = getDataRef( "output/controls/rudder"   );
        m_outputFlaps     = getDataRef( "output/controls/flaps"    );
    }
    else
    {
        Exception e;

        e.setType( Exception::UnknownException );
        e.setInfo( "ERROR! Initializing data references failed." );

        FDM_THROW( e );
    }

    m_channelAilerons     = getChannelByName( "ailerons"      );
    m_channelElevator     = getChannelByName( "elevator"      );
    m_channelRudder       = getChannelByName( "rudder"        );
    m_channelElevatorTrim = getChannelByName( "elevator_trim" );
    m_channelFlaps        = getChannelByName( "flaps"         );
    m_channelBrakeL       = getChannelByName( "brake_l"       );
    m_channelBrakeR       = getChannelByName( "brake_r"       );
    m_channelNoseWheel    = getChannelByName( "nose_wheel"    );

    if ( 0 == m_channelAilerons
      || 0 == m_channelElevator
      || 0 == m_channelRudder
      || 0 == m_channelElevatorTrim
      || 0 == m_channelFlaps
      || 0 == m_channelBrakeL
      || 0 == m_channelBrakeR
      || 0 == m_channelNoseWheel )
    {
        Exception e;

        e.setType( Exception::UnknownException );
        e.setInfo( "ERROR! Obtaining control channels failed." );

        FDM_THROW( e );
    }

    if ( FDM_SUCCESS == addDataRef( "input/controls/nw_steering", DataNode::Bool ) )
    {
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

void C172_Controls::update()
{
    ///////////////////
    Controls::update();
    ///////////////////

    m_ailerons = m_channelAilerons->output;
    m_elevator = m_channelElevator->output;
    m_rudder   = m_channelRudder->output;

    m_elevator_trim = m_channelElevatorTrim->output;

    m_flaps = m_channelFlaps->output;

    m_brake_l = m_channelBrakeL->output;
    m_brake_r = m_channelBrakeR->output;

    m_nose_wheel = m_channelNoseWheel->output;
    m_nwSteering = m_drNwSteering.getDatab();

    m_outputAilerons .setDatad( m_ailerons );
    m_outputElevator .setDatad( m_elevator );
    m_outputRudder   .setDatad( m_rudder   );
    m_outputFlaps    .setDatad( m_flaps    );
}
