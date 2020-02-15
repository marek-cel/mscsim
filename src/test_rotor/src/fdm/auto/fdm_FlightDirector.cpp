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

#include <fdm/auto/fdm_FlightDirector.h>

#include <fdm/utils/fdm_Misc.h>
#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

FlightDirector::FlightDirector() :
    _cmd_roll  ( 0.0 ),
    _cmd_pitch ( 0.0 ),

    _altitude  ( 0.0 ),
    _airspeed  ( 0.0 ),
    _heading   ( 0.0 ),
    _course    ( 0.0 ),
    _climbRate ( 0.0 ),
    _pitch     ( 0.0 ),

    _engaged ( false )
{}

////////////////////////////////////////////////////////////////////////////////

FlightDirector::~FlightDirector() {}

////////////////////////////////////////////////////////////////////////////////

void FlightDirector::setAltitude( double altitude )
{
    _altitude = fdm::Misc::max( 0.0, altitude );
}

////////////////////////////////////////////////////////////////////////////////

void FlightDirector::setAirspeed( double airspeed )
{
    _airspeed = fdm::Misc::max( 0.0, airspeed );
}

////////////////////////////////////////////////////////////////////////////////

void FlightDirector::setClimbRate( double climbRate )
{
    _climbRate = climbRate;
}

////////////////////////////////////////////////////////////////////////////////

void FlightDirector::setHeading( double heading )
{
    _heading = heading;
}

////////////////////////////////////////////////////////////////////////////////

void FlightDirector::setCourse( double course )
{
    _course = course;
}

////////////////////////////////////////////////////////////////////////////////

void FlightDirector::setPitch( double pitch )
{
    _pitch = Misc::satur( -M_PI_2, M_PI_2, pitch );
    _cmd_pitch = _pitch;
}
