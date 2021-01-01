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

#include <cgi/cgi_ManipulatorWorld.h>

#include <cgi/cgi_Defines.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

ManipulatorWorld::ManipulatorWorld() :
    inherited()
{
    setWheelZoomFactor( -getWheelZoomFactor() );
}

////////////////////////////////////////////////////////////////////////////////

bool ManipulatorWorld::handleFrame( const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us )
{
    ///////////////////////////////////////////////
    bool result = inherited::handleFrame( ea, us );
    ///////////////////////////////////////////////

    boundDistance();

    return result;
}

////////////////////////////////////////////////////////////////////////////////

bool ManipulatorWorld::performMovementRightMouseButton( const double /*eventTimeDelta*/, const double /*dx*/, const double /*dy*/ )
{
    return true;
}

////////////////////////////////////////////////////////////////////////////////

void ManipulatorWorld::boundDistance()
{
#   ifndef SIM_TEST_WORLD
    static const double distance_max = 0.8 * CGI_SKYDOME_RADIUS;

    if ( getDistance() > distance_max )
    {
        setDistance( distance_max );
    }
#   endif
}
