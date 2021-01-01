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

#include <cgi/cgi_ManipulatorOrbit.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

ManipulatorOrbit::ManipulatorOrbit() :
    inherited(),
    _distance_min ( 5.0 ),
    _distance_max ( 5000.0 )
{
    setTrackerMode( NODE_CENTER );
    setWheelZoomFactor( -getWheelZoomFactor() );
}

////////////////////////////////////////////////////////////////////////////////

bool ManipulatorOrbit::handleFrame( const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us )
{
    ///////////////////////////////////////////////
    bool result = inherited::handleFrame( ea, us );
    ///////////////////////////////////////////////

    boundDistance();

    return result;
}

////////////////////////////////////////////////////////////////////////////////

void ManipulatorOrbit::setDistanceMin( double distance_min )
{
    if ( distance_min > 0.0 && distance_min < _distance_max )
    {
        _distance_min = distance_min;
    }

    boundDistance();
}

////////////////////////////////////////////////////////////////////////////////

void ManipulatorOrbit::setDistanceMax( double distance_max )
{
    if ( distance_max > 0.0 && distance_max > _distance_min )
    {
        _distance_max = distance_max;
    }

    boundDistance();
}

////////////////////////////////////////////////////////////////////////////////

bool ManipulatorOrbit::performMovementRightMouseButton( const double /*eventTimeDelta*/, const double /*dx*/, const double /*dy*/ )
{
    return true;
}

////////////////////////////////////////////////////////////////////////////////

void ManipulatorOrbit::boundDistance()
{
    if ( getDistance() > _distance_max )
    {
        setDistance( _distance_max );
    }
    else if ( getDistance() < _distance_min )
    {
        setDistance( _distance_min );
    }
}
