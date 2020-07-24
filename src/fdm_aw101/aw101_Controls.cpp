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

#include <fdm_aw101/aw101_Controls.h>
#include <fdm_aw101/aw101_Aircraft.h>

#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

AW101_Controls::AW101_Controls( const AW101_Aircraft *aircraft ) :
    Controls( aircraft ),
    _aircraft ( aircraft ),

    _channelCyclicLat  ( FDM_NULLPTR ),
    _channelCyclicLon  ( FDM_NULLPTR ),
    _channelCollective ( FDM_NULLPTR ),
    _channelTailPitch  ( FDM_NULLPTR ),
    _channelBrakeL     ( FDM_NULLPTR ),
    _channelBrakeR     ( FDM_NULLPTR ),

    _afcs ( FDM_NULLPTR ),

    _cyclic_lat ( 0.0 ),
    _cyclic_lon ( 0.0 ),
    _collective ( 0.0 ),
    _tail_pitch ( 0.0 ),
    _brake_l    ( 0.0 ),
    _brake_r    ( 0.0 )
{
    _afcs = new AW101_AFCS();
}

////////////////////////////////////////////////////////////////////////////////

AW101_Controls::~AW101_Controls()
{
    FDM_DELPTR( _afcs );
}

////////////////////////////////////////////////////////////////////////////////

void AW101_Controls::readData( XmlNode &dataNode )
{
    ///////////////////////////////
    Controls::readData( dataNode );
    ///////////////////////////////

    if ( dataNode.isValid() )
    {
        XmlNode nodeAFCS = dataNode.getFirstChildElement( "afcs" );
        _afcs->readData( nodeAFCS );
    }
    else
    {
        XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
}

////////////////////////////////////////////////////////////////////////////////

void AW101_Controls::initialize()
{
    _channelCyclicLat  = getChannelByName( "cyclic_lat" );
    _channelCyclicLon  = getChannelByName( "cyclic_lon" );
    _channelCollective = getChannelByName( "collective" );
    _channelTailPitch  = getChannelByName( "tail_pitch" );
    _channelBrakeL     = getChannelByName( "brake_l"    );
    _channelBrakeR     = getChannelByName( "brake_r"    );

    if ( FDM_NULLPTR != _channelCyclicLat
      && FDM_NULLPTR != _channelCyclicLon
      && FDM_NULLPTR != _channelCollective
      && FDM_NULLPTR != _channelTailPitch
      && FDM_NULLPTR != _channelBrakeL
      && FDM_NULLPTR != _channelBrakeR )
    {
        _channelCyclicLat  ->input = FDM_NULLPTR;
        _channelCyclicLon  ->input = FDM_NULLPTR;
        _channelCollective ->input = &_aircraft->getDataInp()->controls.collective;
        _channelTailPitch  ->input = FDM_NULLPTR;
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

    ///////////////////////
    Controls::initialize();
    ///////////////////////
}

////////////////////////////////////////////////////////////////////////////////

void AW101_Controls::update()
{
    ///////////////////
    Controls::update();
    ///////////////////

    _afcs->update( _aircraft->getTimeStep(),
                   _aircraft->getDataInp()->controls.roll  , 0.0,
                   _aircraft->getDataInp()->controls.pitch , 0.0,
                   _aircraft->getDataInp()->controls.yaw   , 0.0,
                   _aircraft->getAngles_NED(),
                   _aircraft->getOmg_BAS() );

    double cyclic_lat = _aircraft->getDataInp()->controls.roll  + _afcs->getCyclicLat();
    double cyclic_lon = _aircraft->getDataInp()->controls.pitch + _afcs->getCyclicLon();
    double tail_pitch = _aircraft->getDataInp()->controls.yaw   + _afcs->getTailPitch()
                      - 0.2 * _aircraft->getDataInp()->controls.collective;

    cyclic_lat = Misc::satur( -1.0, 1.0, cyclic_lat );
    cyclic_lon = Misc::satur( -1.0, 1.0, cyclic_lon );
    tail_pitch = Misc::satur( -1.0, 1.0, tail_pitch );

    _channelCyclicLat  ->output = _channelCyclicLat  ->table.getValue( cyclic_lat );
    _channelCyclicLon  ->output = _channelCyclicLon  ->table.getValue( cyclic_lon );
    _channelTailPitch  ->output = _channelTailPitch  ->table.getValue( tail_pitch );

    _cyclic_lat = _channelCyclicLat  ->output;
    _cyclic_lon = _channelCyclicLon  ->output;
    _collective = _channelCollective ->output;
    _tail_pitch = _channelTailPitch  ->output;

    _brake_l = _channelBrakeL->output;
    _brake_r = _channelBrakeR->output;
}
