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
    _aircraft ( aircraft ),

    _channelAilerons     ( 0 ),
    _channelElevator     ( 0 ),
    _channelRudder       ( 0 ),
    _channelElevatorTrim ( 0 ),
    _channelFlaps        ( 0 ),
    _channelBrakeL       ( 0 ),
    _channelBrakeR       ( 0 ),

    _ailerons      ( 0.0 ),
    _elevator      ( 0.0 ),
    _rudder        ( 0.0 ),
    _elevator_trim ( 0.0 ),
    _flaps         ( 0.0 ),
    _brake_l       ( 0.0 ),
    _brake_r       ( 0.0 )
{}

////////////////////////////////////////////////////////////////////////////////

P51_Controls::~P51_Controls() {}

////////////////////////////////////////////////////////////////////////////////

void P51_Controls::init()
{
    _channelAilerons     = getChannelByName( "ailerons"      );
    _channelElevator     = getChannelByName( "elevator"      );
    _channelRudder       = getChannelByName( "rudder"        );
    _channelElevatorTrim = getChannelByName( "elevator_trim" );
    _channelFlaps        = getChannelByName( "flaps"         );
    _channelBrakeL       = getChannelByName( "brake_l"       );
    _channelBrakeR       = getChannelByName( "brake_r"       );

    if ( 0 != _channelAilerons
      && 0 != _channelElevator
      && 0 != _channelRudder
      && 0 != _channelElevatorTrim
      && 0 != _channelFlaps
      && 0 != _channelBrakeL
      && 0 != _channelBrakeR )
    {
        _channelAilerons     ->input = &_aircraft->getDataInp()->controls.roll;
        _channelElevator     ->input = &_aircraft->getDataInp()->controls.pitch;
        _channelRudder       ->input = &_aircraft->getDataInp()->controls.yaw;
        _channelElevatorTrim ->input = &_aircraft->getDataInp()->controls.trim_pitch;
        _channelFlaps        ->input = &_aircraft->getDataInp()->controls.flaps;
        _channelBrakeL       ->input = &_aircraft->getDataInp()->controls.brake_l;
        _channelBrakeR       ->input = &_aircraft->getDataInp()->controls.brake_r;
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

    _ailerons = _channelAilerons->output;
    _elevator = _channelElevator->output;
    _rudder   = _channelRudder->output;

    _elevator_trim = _channelElevatorTrim->output;

    _flaps = _channelFlaps->output;

    _brake_l = _channelBrakeL->output;
    _brake_r = _channelBrakeR->output;
}
