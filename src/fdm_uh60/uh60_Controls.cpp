/****************************************************************************//*
 * Copyright (C) 2021 Marek M. Cel
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

UH60_Controls::UH60_Controls( const UH60_Aircraft *aircraft, Input *input ) :
    Controls( aircraft, input ),
    _aircraft ( aircraft ),

    _channelCyclicLat  ( FDM_NULLPTR ),
    _channelCyclicLon  ( FDM_NULLPTR ),
    _channelCollective ( FDM_NULLPTR ),
    _channelTailPitch  ( FDM_NULLPTR ),
    _channelElevator   ( FDM_NULLPTR ),
    _channelBrakeLeft  ( FDM_NULLPTR ),
    _channelBrakeRight ( FDM_NULLPTR ),

    _afcs ( FDM_NULLPTR ),

    _cyclic_lat ( 0.0 ),
    _cyclic_lon ( 0.0 ),
    _collective ( 0.0 ),
    _tail_pitch ( 0.0 ),
    _elevator   ( 0.0 ),
    _brake_l    ( 0.0 ),
    _brake_r    ( 0.0 )
{
    _afcs = new UH60_AFCS();
}

////////////////////////////////////////////////////////////////////////////////

UH60_Controls::~UH60_Controls()
{
    FDM_DELPTR( _afcs );
}

////////////////////////////////////////////////////////////////////////////////

void UH60_Controls::initialize()
{
    _channelCyclicLat  = _channels.getItemByKey( "cyclic_lat"  );
    _channelCyclicLon  = _channels.getItemByKey( "cyclic_lon"  );
    _channelCollective = _channels.getItemByKey( "collective"  );
    _channelTailPitch  = _channels.getItemByKey( "tail_pitch"  );
    _channelElevator   = _channels.getItemByKey( "elevator"    );
    _channelBrakeLeft  = _channels.getItemByKey( "brake_left"  );
    _channelBrakeRight = _channels.getItemByKey( "brake_right" );

    if ( FDM_NULLPTR == _channelCyclicLat
      || FDM_NULLPTR == _channelCyclicLon
      || FDM_NULLPTR == _channelCollective
      || FDM_NULLPTR == _channelTailPitch
      || FDM_NULLPTR == _channelElevator
      || FDM_NULLPTR == _channelBrakeLeft
      || FDM_NULLPTR == _channelBrakeRight )
    {
        Exception e;

        e.setType( Exception::UnknownException );
        e.setInfo( "Obtaining control channels failed." );

        FDM_THROW( e );
    }

    ///////////////////////
    Controls::initialize();
    ///////////////////////
}

////////////////////////////////////////////////////////////////////////////////

void UH60_Controls::update()
{
    ///////////////////
    Controls::update();
    ///////////////////

    _cyclic_lat = _channelCyclicLat->output - 0.2 * _channelCollective->output;
    _cyclic_lon = _channelCyclicLon->output;
    _collective = _channelCollective->output;
    _tail_pitch = _channelTailPitch->output;
    _elevator   = 0.0;//_channelElevator->output; // TODO

    _brake_l = _channelBrakeLeft  ->output;
    _brake_r = _channelBrakeRight ->output;
}
