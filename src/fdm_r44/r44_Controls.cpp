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

#include <fdm_r44/r44_Controls.h>
#include <fdm_r44/r44_Aircraft.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

R44_Controls::R44_Controls( const R44_Aircraft *aircraft, Input *input ) :
    Controls( aircraft, input ),
    _aircraft ( aircraft ),

    _channelCyclicLat  ( FDM_NULLPTR ),
    _channelCyclicLon  ( FDM_NULLPTR ),
    _channelCollective ( FDM_NULLPTR ),
    _channelTailPitch  ( FDM_NULLPTR ),

    _cyclic_lat ( 0.0 ),
    _cyclic_lon ( 0.0 ),
    _collective ( 0.0 ),
    _tail_pitch ( 0.0 )
{}

////////////////////////////////////////////////////////////////////////////////

R44_Controls::~R44_Controls() {}

////////////////////////////////////////////////////////////////////////////////

void R44_Controls::initialize()
{
    _channelCyclicLat  = _channels.getItemByKey( "cyclic_lat" );
    _channelCyclicLon  = _channels.getItemByKey( "cyclic_lon" );
    _channelCollective = _channels.getItemByKey( "collective" );
    _channelTailPitch  = _channels.getItemByKey( "tail_pitch" );

    if ( FDM_NULLPTR == _channelCyclicLat
      || FDM_NULLPTR == _channelCyclicLon
      || FDM_NULLPTR == _channelCollective
      || FDM_NULLPTR == _channelTailPitch )
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

void R44_Controls::update()
{
    ///////////////////
    Controls::update();
    ///////////////////

    _cyclic_lat = _channelCyclicLat->output;
    _cyclic_lon = _channelCyclicLon->output;
    _collective = _channelCollective->output;
    _tail_pitch = _channelTailPitch->output;
}
