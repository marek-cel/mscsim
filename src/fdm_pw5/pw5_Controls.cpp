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

#include <fdm_pw5/pw5_Controls.h>
#include <fdm_pw5/pw5_Aircraft.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

PW5_Controls::PW5_Controls( const PW5_Aircraft *aircraft, Input *input ) :
    Controls( aircraft, input ),
    _aircraft ( aircraft ),

    _channelAilerons     ( FDM_NULLPTR ),
    _channelElevator     ( FDM_NULLPTR ),
    _channelRudder       ( FDM_NULLPTR ),
    _channelElevatorTrim ( FDM_NULLPTR ),
    _channelAirbrake     ( FDM_NULLPTR ),
    _channelWheelBrake   ( FDM_NULLPTR ),

    _ailerons      ( 0.0 ),
    _elevator      ( 0.0 ),
    _rudder        ( 0.0 ),
    _elevator_trim ( 0.0 ),
    _airbrake      ( 0.0 ),
    _wheelBrake    ( 0.0 )
{}

////////////////////////////////////////////////////////////////////////////////

PW5_Controls::~PW5_Controls() {}

////////////////////////////////////////////////////////////////////////////////

void PW5_Controls::initialize()
{
    _channelAilerons     = _channels.getItemByKey( "ailerons"      );
    _channelElevator     = _channels.getItemByKey( "elevator"      );
    _channelRudder       = _channels.getItemByKey( "rudder"        );
    _channelElevatorTrim = _channels.getItemByKey( "elevator_trim" );
    _channelAirbrake     = _channels.getItemByKey( "airbrake"      );
    _channelWheelBrake   = _channels.getItemByKey( "wheel_brake"   );

    if ( FDM_NULLPTR == _channelAilerons
      || FDM_NULLPTR == _channelElevator
      || FDM_NULLPTR == _channelRudder
      || FDM_NULLPTR == _channelElevatorTrim
      || FDM_NULLPTR == _channelAirbrake
      || FDM_NULLPTR == _channelWheelBrake )
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

void PW5_Controls::update()
{
    ///////////////////
    Controls::update();
    ///////////////////

    _ailerons = _channelAilerons->output;
    _elevator = _channelElevator->output;
    _rudder   = _channelRudder->output;

    _elevator_trim = _channelElevatorTrim->output;

    _airbrake = _channelAirbrake->output;

    _wheelBrake = _channelWheelBrake->output;
}
