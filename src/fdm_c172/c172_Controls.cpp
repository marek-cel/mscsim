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

#include <fdm_c172/c172_Controls.h>
#include <fdm_c172/c172_Aircraft.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

C172_Controls::C172_Controls( const C172_Aircraft *aircraft ) :
    Controls( aircraft ),
    _aircraft ( aircraft ),

    _channelAilerons     ( FDM_NULLPTR ),
    _channelElevator     ( FDM_NULLPTR ),
    _channelRudder       ( FDM_NULLPTR ),
    _channelElevatorTrim ( FDM_NULLPTR ),
    _channelFlaps        ( FDM_NULLPTR ),
    _channelBrakeL       ( FDM_NULLPTR ),
    _channelBrakeR       ( FDM_NULLPTR ),
    _channelNoseWheel    ( FDM_NULLPTR ),

    _ailerons      ( 0.0 ),
    _elevator      ( 0.0 ),
    _rudder        ( 0.0 ),
    _elevator_trim ( 0.0 ),
    _flaps         ( 0.0 ),
    _brake_l       ( 0.0 ),
    _brake_r       ( 0.0 ),
    _nose_wheel    ( 0.0 )
{}

////////////////////////////////////////////////////////////////////////////////

C172_Controls::~C172_Controls() {}

////////////////////////////////////////////////////////////////////////////////

void C172_Controls::init()
{
    _channelAilerons     = getChannelByName( "ailerons"      );
    _channelElevator     = getChannelByName( "elevator"      );
    _channelRudder       = getChannelByName( "rudder"        );
    _channelElevatorTrim = getChannelByName( "elevator_trim" );
    _channelFlaps        = getChannelByName( "flaps"         );
    _channelBrakeL       = getChannelByName( "brake_l"       );
    _channelBrakeR       = getChannelByName( "brake_r"       );
    _channelNoseWheel    = getChannelByName( "nose_wheel"    );

    if ( FDM_NULLPTR != _channelAilerons
      && FDM_NULLPTR != _channelElevator
      && FDM_NULLPTR != _channelRudder
      && FDM_NULLPTR != _channelElevatorTrim
      && FDM_NULLPTR != _channelFlaps
      && FDM_NULLPTR != _channelBrakeL
      && FDM_NULLPTR != _channelBrakeR
      && FDM_NULLPTR != _channelNoseWheel )
    {
        _channelAilerons     ->input = &_aircraft->getDataInp()->controls.roll;
        _channelElevator     ->input = &_aircraft->getDataInp()->controls.pitch;
        _channelRudder       ->input = &_aircraft->getDataInp()->controls.yaw;
        _channelElevatorTrim ->input = &_aircraft->getDataInp()->controls.trim_pitch;
        _channelFlaps        ->input = &_aircraft->getDataInp()->controls.flaps;
        _channelBrakeL       ->input = &_aircraft->getDataInp()->controls.brake_l;
        _channelBrakeR       ->input = &_aircraft->getDataInp()->controls.brake_r;
        _channelNoseWheel    ->input = &_aircraft->getDataInp()->controls.nose_wheel;
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

void C172_Controls::update()
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

    _nose_wheel = _channelNoseWheel->output;
}
