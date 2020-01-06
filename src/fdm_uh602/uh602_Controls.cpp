/****************************************************************************//*
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

#include <fdm_uh602/uh602_Controls.h>
#include <fdm_uh602/uh602_Aircraft.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

UH602_Controls::UH602_Controls( const UH602_Aircraft *aircraft ) :
    Controls( aircraft ),
    _aircraft ( aircraft ),

    _channelCyclicLat  ( 0 ),
    _channelCyclicLon  ( 0 ),
    _channelCollective ( 0 ),
    _channelTailPitch  ( 0 ),
    _channelElevator   ( 0 ),
    _channelBrakeL     ( 0 ),
    _channelBrakeR     ( 0 ),

    _afcs ( 0 ),

    _cyclic_lat ( 0.0 ),
    _cyclic_lon ( 0.0 ),
    _collective ( 0.0 ),
    _tail_pitch ( 0.0 ),
    _elevator   ( 0.0 ),
    _brake_l    ( 0.0 ),
    _brake_r    ( 0.0 )
{
    _afcs = new UH602_AFCS();
}

////////////////////////////////////////////////////////////////////////////////

UH602_Controls::~UH602_Controls()
{
    FDM_DELPTR( _afcs );
}

////////////////////////////////////////////////////////////////////////////////

void UH602_Controls::init()
{
    _channelCyclicLat  = getChannelByName( "cyclic_lat" );
    _channelCyclicLon  = getChannelByName( "cyclic_lon" );
    _channelCollective = getChannelByName( "collective" );
    _channelTailPitch  = getChannelByName( "tail_pitch" );
    _channelElevator   = getChannelByName( "elevator"   );
    _channelBrakeL     = getChannelByName( "brake_l"    );
    _channelBrakeR     = getChannelByName( "brake_r"    );

    if ( 0 != _channelCyclicLat
      && 0 != _channelCyclicLon
      && 0 != _channelCollective
      && 0 != _channelTailPitch
      && 0 != _channelElevator
      && 0 != _channelBrakeL
      && 0 != _channelBrakeR )
    {
        _channelCyclicLat  ->input = &_aircraft->getDataInp()->controls.roll;
        _channelCyclicLon  ->input = &_aircraft->getDataInp()->controls.pitch;
        _channelCollective ->input = &_aircraft->getDataInp()->controls.collective;
        _channelTailPitch  ->input = &_aircraft->getDataInp()->controls.yaw;
        _channelElevator   ->input = &_aircraft->getDataInp()->controls.pitch;
        _channelBrakeL     ->input = &_aircraft->getDataInp()->controls.brake_l;
        _channelBrakeR     ->input = &_aircraft->getDataInp()->controls.brake_r;
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

void UH602_Controls::update()
{
    ///////////////////
    Controls::update();
    ///////////////////

    _cyclic_lat = _channelCyclicLat->output - 0.2 * _channelCollective->output;
    _cyclic_lon = _channelCyclicLon->output;
    _collective = _channelCollective->output;
    _tail_pitch = _channelTailPitch->output;
    _elevator   = 0.0;//m_channelElevator->output; // TODO

    _brake_l = _channelBrakeL->output;
    _brake_r = _channelBrakeR->output;
}
