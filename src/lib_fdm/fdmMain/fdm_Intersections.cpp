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

#include <fdmMain/fdm_Intersections.h>

#ifdef SIM_INTERSECTIONS
#   include <cgi/cgi_Intersections.h>
#   include <cgi/cgi_WGS84.h>
#endif

#include <fdmUtils/fdm_WGS84.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Intersections::Intersections() {}

////////////////////////////////////////////////////////////////////////////////

Intersections::~Intersections() {}

////////////////////////////////////////////////////////////////////////////////

void Intersections::update( double lat, double lon )
{
#   ifdef SIM_INTERSECTIONS
    cgi::WGS84 wgs_b( lat, lon, 10000.0 );
    cgi::WGS84 wgs_e( lat, lon, -1000.0 );

    osg::Vec3d b = wgs_b.getPosition();
    osg::Vec3d e = wgs_e.getPosition();
    osg::Vec3d r;
    osg::Vec3d n;

    if ( cgi::Intersections::instance()->findFirst( b, e, r, n ) )
    {
        m_ground_wgs = Vector3( r.x(), r.y(), r.z() );
        m_normal_wgs = Vector3( n.x(), n.y(), n.z() );
    }
#   endif
}

////////////////////////////////////////////////////////////////////////////////

double Intersections::getElevation( double lat, double lon ) const
{
    WGS84::Geo b_geo;
    WGS84::Geo e_geo;

    b_geo.lat = lat;
    b_geo.lon = lon;
    b_geo.alt = 10000.0;

    e_geo.lat = lat;
    e_geo.lon = lon;
    e_geo.alt = -1000.0;

    Vector3 b_wgs = WGS84::geo2wgs( b_geo );
    Vector3 e_wgs = WGS84::geo2wgs( e_geo );
    Vector3 r_wgs;
    Vector3 n_wgs;

    if ( FDM_SUCCESS == getIntersection( b_wgs, e_wgs, r_wgs, n_wgs ) )
    {
        return WGS84::wgs2geo( r_wgs ).alt;
    }

    return 0.0;
}

////////////////////////////////////////////////////////////////////////////////

int Intersections::getIntersection( const Vector3 &b, const Vector3 &e,
                                    Vector3 &r, Vector3 &n ) const
{
    double num = m_normal_wgs * ( m_ground_wgs - b );
    double den = m_normal_wgs * ( e - b );

    double u = 0.0;

    if ( fabs( den ) > 10e-15 ) u = num / den;

    if ( 0.0 < u && u < 1.0 )
    {
        r = b + u * ( e - b );
        n = m_normal_wgs;

        return FDM_SUCCESS;
    }

    return FDM_FAILURE;
}

////////////////////////////////////////////////////////////////////////////////

bool Intersections::isIntersection( const Vector3 &b, const Vector3 &e ) const
{
    double num = m_normal_wgs * ( m_ground_wgs - b );
    double den = m_normal_wgs * ( e - b );

    double u = 0.0;

    if ( fabs( den ) > 10e-15 ) u = num / den;

    if ( 0.0 < u && u < 1.0 )
    {
        return true;
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////

Vector3 Intersections::getNormal( double lat, double lon ) const
{
    WGS84::Geo b_geo;
    WGS84::Geo e_geo;

    b_geo.lat = lat;
    b_geo.lon = lon;
    b_geo.alt = 10000.0;

    e_geo.lat = lat;
    e_geo.lon = lon;
    e_geo.alt = -1000.0;

    Vector3 b_wgs = WGS84::geo2wgs( b_geo );
    Vector3 e_wgs = WGS84::geo2wgs( e_geo );
    Vector3 r_wgs;
    Vector3 n_wgs;

    if ( FDM_SUCCESS == getIntersection( b_wgs, e_wgs, r_wgs, n_wgs ) )
    {
        return n_wgs;
    }
    else
    {
        return WGS84( b_wgs ).getNorm_WGS();
    }
}
