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

#include <fdm_p51/p51_Controls.h>
#include <fdm_p51/p51_Aircraft.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

P51_Controls::P51_Controls( const P51_Aircraft *aircraft ) :
    Controls( aircraft ),
    m_aircraft ( aircraft ),

    m_channelAilerons     ( 0 ),
    m_channelElevator     ( 0 ),
    m_channelRudder       ( 0 ),
    m_channelElevatorTrim ( 0 ),
    m_channelFlaps        ( 0 ),
    m_channelBrakeL       ( 0 ),
    m_channelBrakeR       ( 0 ),

    m_ailerons      ( 0.0 ),
    m_elevator      ( 0.0 ),
    m_rudder        ( 0.0 ),
    m_elevator_trim ( 0.0 ),
    m_flaps         ( 0.0 ),
    m_brake_l       ( 0.0 ),
    m_brake_r       ( 0.0 )
{}

////////////////////////////////////////////////////////////////////////////////

P51_Controls::~P51_Controls() {}

////////////////////////////////////////////////////////////////////////////////

void P51_Controls::init()
{
    m_channelAilerons     = getChannelByName( "ailerons"      );
    m_channelElevator     = getChannelByName( "elevator"      );
    m_channelRudder       = getChannelByName( "rudder"        );
    m_channelElevatorTrim = getChannelByName( "elevator_trim" );
    m_channelFlaps        = getChannelByName( "flaps"         );
    m_channelBrakeL       = getChannelByName( "brake_l"       );
    m_channelBrakeR       = getChannelByName( "brake_r"       );

    if ( 0 != m_channelAilerons
      && 0 != m_channelElevator
      && 0 != m_channelRudder
      && 0 != m_channelElevatorTrim
      && 0 != m_channelFlaps
      && 0 != m_channelBrakeL
      && 0 != m_channelBrakeR )
    {
        m_channelAilerons     ->input = &m_aircraft->getDataInp()->controls.roll;
        m_channelElevator     ->input = &m_aircraft->getDataInp()->controls.pitch;
        m_channelRudder       ->input = &m_aircraft->getDataInp()->controls.yaw;
        m_channelElevatorTrim ->input = &m_aircraft->getDataInp()->controls.trim_pitch;
        m_channelFlaps        ->input = &m_aircraft->getDataInp()->controls.flaps;
        m_channelBrakeL       ->input = &m_aircraft->getDataInp()->controls.brake_l;
        m_channelBrakeR       ->input = &m_aircraft->getDataInp()->controls.brake_r;
    }
    else
    {
        Exception e;

        e.setType( Exception::UnknownException );
        e.setInfo( "Obtaining control channels failed." );

        FDM_THROW( e );
    }

    /////////////////
    Controls::init();
    /////////////////
}

////////////////////////////////////////////////////////////////////////////////

void P51_Controls::update()
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
}
