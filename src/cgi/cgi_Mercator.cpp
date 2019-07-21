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

#include <cgi/cgi_Mercator.h>
#include <cgi/cgi_WGS84.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

const double Mercator::maxX = Mercator::getX( osg::DegreesToRadians( 180.0 ) );
const double Mercator::maxY = Mercator::getY( osg::DegreesToRadians(  85.0 ) );

////////////////////////////////////////////////////////////////////////////////

double Mercator::getLat( double y )
{
    return 2.0 * atan( exp( y / WGS84::getRadiusEquatorial() ) ) - M_PI_2;
}

////////////////////////////////////////////////////////////////////////////////

double Mercator::getLon( double x )
{
    return x / WGS84::getRadiusEquatorial();
}

////////////////////////////////////////////////////////////////////////////////

double Mercator::getX( double lon )
{
    return WGS84::getRadiusEquatorial() * lon;
}

////////////////////////////////////////////////////////////////////////////////

double Mercator::getY( double lat )
{
    return WGS84::getRadiusEquatorial() * log( tan( M_PI_4 + 0.5 * lat ) );
}

////////////////////////////////////////////////////////////////////////////////

Mercator::Mercator() {}

////////////////////////////////////////////////////////////////////////////////

Mercator::~Mercator() {}
