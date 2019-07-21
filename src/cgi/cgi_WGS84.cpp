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

#include <cgi/cgi_WGS84.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

const osg::EllipsoidModel WGS84::em = osg::EllipsoidModel();

const osg::Quat WGS84::m_enu2ned = osg::Matrixd( 0.0,  1.0,  0.0,  0.0,
                                                 1.0,  0.0,  0.0,  0.0,
                                                 0.0,  0.0, -1.0,  0.0,
                                                 0.0,  0.0,  0.0,  1.0 ).getRotate();

////////////////////////////////////////////////////////////////////////////////

double WGS84::getRadiusEquatorial()
{
    return osg::WGS_84_RADIUS_EQUATOR;
}

////////////////////////////////////////////////////////////////////////////////

WGS84::WGS84()
{
    set( 0.0, 0.0, 0.0 );
}

////////////////////////////////////////////////////////////////////////////////

WGS84::WGS84( double lat, double lon, double alt )
{
    set( lat, lon, alt );
}

////////////////////////////////////////////////////////////////////////////////

WGS84::WGS84( const osg::Vec3d &position )
{
    set( position );
}

////////////////////////////////////////////////////////////////////////////////

WGS84::~WGS84() {}

////////////////////////////////////////////////////////////////////////////////

void WGS84::set( double lat, double lon, double alt )
{
    m_lat = lat;
    m_lon = lon;
    m_alt = alt;

    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    osg::Matrixd localToWorld;

    em.convertLatLongHeightToXYZ( lat, lon, alt, x, y, z );
    em.computeLocalToWorldTransformFromXYZ( x, y, z, localToWorld );

    osg::Quat wgs2enu = localToWorld.getRotate();

    m_position = osg::Vec3( x, y, z );
    m_attitude = m_enu2ned * wgs2enu;
}

////////////////////////////////////////////////////////////////////////////////

void WGS84::set( const osg::Vec3d &position )
{
    double lat = 0.0;
    double lon = 0.0;
    double alt = 0.0;

    osg::Matrixd localToWorld;

    em.convertXYZToLatLongHeight( position.x(), position.y(), position.z(),
                                  lat, lon, alt );
    em.computeLocalToWorldTransformFromXYZ( position.x(), position.y(), position.z(),
                                            localToWorld );

    m_lat = lat;
    m_lon = lon;
    m_alt = alt;

    osg::Quat wgs2enu = localToWorld.getRotate();

    m_position = position;
    m_attitude = m_enu2ned * wgs2enu;
}
