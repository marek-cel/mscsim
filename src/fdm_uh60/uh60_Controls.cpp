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

#include <fdm_uh60/uh60_Controls.h>
#include <fdm_uh60/uh60_Aircraft.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

UH60_Controls::UH60_Controls( const UH60_Aircraft *aircraft ) :
    Controls( aircraft ),
    m_aircraft ( aircraft ),

    m_channelCyclicLat  ( 0 ),
    m_channelCyclicLon  ( 0 ),
    m_channelCollective ( 0 ),
    m_channelTailPitch  ( 0 ),
    m_channelElevator   ( 0 ),
    m_channelBrakeL     ( 0 ),
    m_channelBrakeR     ( 0 ),

    m_afcs ( 0 ),

    m_cyclic_lat ( 0.0 ),
    m_cyclic_lon ( 0.0 ),
    m_collective ( 0.0 ),
    m_tail_pitch ( 0.0 ),
    m_elevator   ( 0.0 ),
    m_brake_l    ( 0.0 ),
    m_brake_r    ( 0.0 )
{
    m_afcs = new UH60_AFCS();
}

////////////////////////////////////////////////////////////////////////////////

UH60_Controls::~UH60_Controls()
{
    if ( m_afcs ) delete m_afcs;
    m_afcs = 0;
}

////////////////////////////////////////////////////////////////////////////////

void UH60_Controls::init()
{
    m_channelCyclicLat  = getChannelByName( "cyclic_lat" );
    m_channelCyclicLon  = getChannelByName( "cyclic_lon" );
    m_channelCollective = getChannelByName( "collective" );
    m_channelTailPitch  = getChannelByName( "tail_pitch" );
    m_channelElevator   = getChannelByName( "elevator"   );
    m_channelBrakeL     = getChannelByName( "brake_l"    );
    m_channelBrakeR     = getChannelByName( "brake_r"    );

    if ( 0 != m_channelCyclicLat
      && 0 != m_channelCyclicLon
      && 0 != m_channelCollective
      && 0 != m_channelTailPitch
      && 0 != m_channelElevator
      && 0 != m_channelBrakeL
      && 0 != m_channelBrakeR )
    {
        m_channelCyclicLat  ->input = &m_aircraft->getDataInp()->controls.roll;
        m_channelCyclicLon  ->input = &m_aircraft->getDataInp()->controls.pitch;
        m_channelCollective ->input = &m_aircraft->getDataInp()->controls.collective;
        m_channelTailPitch  ->input = &m_aircraft->getDataInp()->controls.yaw;
        m_channelElevator   ->input = &m_aircraft->getDataInp()->controls.pitch;
        m_channelBrakeL     ->input = &m_aircraft->getDataInp()->controls.brake_l;
        m_channelBrakeR     ->input = &m_aircraft->getDataInp()->controls.brake_r;
    }
    else
    {
        Exception e;

        e.setType( Exception::UnknownException );
        e.setInfo( "ERROR! Obtaining control channels failed." );

        FDM_THROW( e );
    }

    /////////////////
    Controls::init();
    /////////////////
}

////////////////////////////////////////////////////////////////////////////////

void UH60_Controls::update()
{
    ///////////////////
    Controls::update();
    ///////////////////

    m_cyclic_lat = m_channelCyclicLat->output - 0.2 * m_channelCollective->output;
    m_cyclic_lon = m_channelCyclicLon->output;
    m_collective = m_channelCollective->output;
    m_tail_pitch = m_channelTailPitch->output;
    m_elevator   = 0.0;//m_channelElevator->output; // TODO

    m_brake_l = m_channelBrakeL->output;
    m_brake_r = m_channelBrakeR->output;
}
