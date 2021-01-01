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

#include <fdm_c172/c172_Controls.h>
#include <fdm_c172/c172_Aircraft.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

C172_Controls::C172_Controls( const C172_Aircraft *aircraft, Input *input ) :
    Controls( aircraft, input ),
    _aircraft ( aircraft ),

    _channelAilerons     ( FDM_NULLPTR ),
    _channelElevator     ( FDM_NULLPTR ),
    _channelRudder       ( FDM_NULLPTR ),
    _channelElevatorTrim ( FDM_NULLPTR ),
    _channelFlaps        ( FDM_NULLPTR ),
    _channelBrakeLeft    ( FDM_NULLPTR ),
    _channelBrakeRight   ( FDM_NULLPTR ),
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

void C172_Controls::initialize()
{
    _channelAilerons     = _channels.getItemByKey( "ailerons"      );
    _channelElevator     = _channels.getItemByKey( "elevator"      );
    _channelRudder       = _channels.getItemByKey( "rudder"        );
    _channelElevatorTrim = _channels.getItemByKey( "elevator_trim" );
    _channelFlaps        = _channels.getItemByKey( "flaps"         );
    _channelBrakeLeft    = _channels.getItemByKey( "brake_left"    );
    _channelBrakeRight   = _channels.getItemByKey( "brake_right"   );
    _channelNoseWheel    = _channels.getItemByKey( "nose_wheel"    );

    if ( FDM_NULLPTR == _channelAilerons
      || FDM_NULLPTR == _channelElevator
      || FDM_NULLPTR == _channelRudder
      || FDM_NULLPTR == _channelElevatorTrim
      || FDM_NULLPTR == _channelFlaps
      || FDM_NULLPTR == _channelBrakeLeft
      || FDM_NULLPTR == _channelBrakeRight
      || FDM_NULLPTR == _channelNoseWheel )
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

void C172_Controls::update()
{
    ///////////////////
    Controls::update();
    ///////////////////

    _ailerons = _channelAilerons ->output;
    _elevator = _channelElevator ->output;
    _rudder   = _channelRudder   ->output;

    _elevator_trim = _channelElevatorTrim->output;

    _flaps = _channelFlaps->output;

    _brake_l = _channelBrakeLeft  ->output;
    _brake_r = _channelBrakeRight ->output;

    _nose_wheel = _channelNoseWheel->output;
}
