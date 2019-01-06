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

#include <fdmUtils/fdm_WGS84.h>

#include <math.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

const double WGS84::m_a = 6378137.0;
const double WGS84::m_f = 1.0 / 298.257223563;

const double WGS84::m_b   = 6356752.3142;
const double WGS84::m_r1  = ( 2.0 * m_a + m_b ) / 3.0;
const double WGS84::m_a2  = WGS84::m_a * WGS84::m_a;
const double WGS84::m_b2  = WGS84::m_b * WGS84::m_b;
const double WGS84::m_e2  = 6.6943799901400e-3;
const double WGS84::m_e   = 8.1819190842622e-2;
const double WGS84::m_ep2 = 6.7394967422800e-3;
const double WGS84::m_ep  = 8.2094437949696e-2;

const double WGS84::m_g       = 9.80665;
const double WGS84::m_gm      = 3986004.418e8;
const double WGS84::m_omega   = 7.2921151467e-5;
const double WGS84::m_gamma_e = 9.7803253359;
const double WGS84::m_gamma_p = 9.8321849378;
const double WGS84::m_k       = 0.00193185265241;
const double WGS84::m_m       = 0.00344978650684;

//0.0,  1.0,  0.0
//1.0,  0.0,  0.0
//0.0,  0.0, -1.0

const Matrix3x3 WGS84::m_T_enu2ned( 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, -1.0 );
const Matrix3x3 WGS84::m_T_ned2enu( 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, -1.0 );

const Quaternion WGS84::m_enu2ned( WGS84::m_T_enu2ned.getQuaternion() );
const Quaternion WGS84::m_ned2enu( WGS84::m_T_ned2enu.getQuaternion() );

////////////////////////////////////////////////////////////////////////////////

Vector3 WGS84::getOmega_WGS()
{
    return Vector3( 0.0, 0.0, m_omega );
}

////////////////////////////////////////////////////////////////////////////////

void WGS84::geo2wgs( double lat, double lon, double alt,
                     double &x, double &y, double &z )
{
    double sinLat = sin( lat );
    double cosLat = cos( lat );
    double sinLon = sin( lon );
    double cosLon = cos( lon );

    double n = m_a / sqrt( 1.0 - m_e2 * sinLat*sinLat );

    x = ( n + alt ) * cosLat * cosLon;
    y = ( n + alt ) * cosLat * sinLon;
    z = ( n * ( m_b2 / m_a2 ) + alt ) * sinLat;
}

////////////////////////////////////////////////////////////////////////////////

Vector3 WGS84::geo2wgs( double lat, double lon, double alt )
{
    Vector3 pos_wgs;

    geo2wgs( lat, lon, alt, pos_wgs.x(), pos_wgs.y(), pos_wgs.z() );

    return pos_wgs;
}

////////////////////////////////////////////////////////////////////////////////

Vector3 WGS84::geo2wgs( const Geo &geo )
{
    return geo2wgs( geo.lat, geo.lon, geo.alt );
}

////////////////////////////////////////////////////////////////////////////////

void WGS84::geo2wgs( const Geo &geo, Vector3 &pos_wgs )
{
    pos_wgs = geo2wgs( geo );
}

////////////////////////////////////////////////////////////////////////////////

void WGS84::wgs2geo( double x, double y, double z,
                     double &lat, double &lon, double &alt )
{
#   ifdef WGS84_SIMPLE_CONVERSION
    // This method provides 1cm accuracy for height less than 1000km
    double p   = sqrt( x*x + y*y );
    double tht = atan2( z*m_a, p*m_b );
    double ed2 = ( m_a2 - m_b2 ) / m_b2;

    double sinTht = sin( tht );
    double cosTht = cos( tht );

    lat = atan( (z + ed2*m_b*sinTht*sinTht*sinTht) / ( p - m_e2*m_a*cosTht*cosTht*cosTht ) );
    lon = atan2( y, x );

    double sinLat = sin( lat );
    double n = m_a / sqrt( 1.0 - m_e2*sinLat*sinLat );

    alt = p / cos( lat ) - n;
#   else
    double z2 = z*z;
    double r  = sqrt( x*x + y*y );
    double r2 = r*r;
    double e2 = m_a2 - m_b2;
    double f  = 54.0 * m_b2 * z2;
    double g  = r2 + ( 1.0 - m_e2 )*z2 - m_e2*e2;
    double c  = m_e2*m_e2 * f * r2 / ( g*g*g );
    double s  = pow( 1.0 + c + sqrt( c*c + 2.0*c ), 1.0/3.0 );
    double p0 = s + 1.0/s + 1.0;
    double p  = f / ( 3.0 * p0*p0 * g*g );
    double q  = sqrt( 1.0 + 2.0*( m_e2*m_e2 )*p );
    double r0 = -( p * m_e2 * r )/( 1.0 + q ) + sqrt( 0.5*m_a2*( 1.0 + 1.0/q ) - p*( 1.0 - m_e2 )*z2/( q + q*q ) - 0.5*p*r2 );
    double uv = r - m_e2*r0;
    double u  = sqrt( uv*uv + z2 );
    double v  = sqrt( uv*uv + ( 1.0 - m_e2 )*z2 );
    double z0 = m_b2 * z / ( m_a * v );

    alt = u * ( 1.0 - m_b2 / ( m_a * v ) );
    lat = atan( ( z + m_ep2*z0 )/r );
    lon = atan2( y, x );
#   endif
}

////////////////////////////////////////////////////////////////////////////////

WGS84::Geo WGS84::wgs2geo( double x, double y, double z )
{
    Geo pos_geo;

    wgs2geo( x, y, z, pos_geo.lat, pos_geo.lon, pos_geo.alt );

    return pos_geo;
}

////////////////////////////////////////////////////////////////////////////////

WGS84::Geo WGS84::wgs2geo( const Vector3 &pos_wgs )
{
    return wgs2geo( pos_wgs.x(), pos_wgs.y(), pos_wgs.z() );
}

////////////////////////////////////////////////////////////////////////////////

void WGS84::wgs2geo( const Vector3 &pos_wgs, Geo &pos_geo )
{
    pos_geo = wgs2geo( pos_wgs );
}

////////////////////////////////////////////////////////////////////////////////

WGS84::WGS84()
{
    m_pos_geo.lat = 0.0;
    m_pos_geo.lon = 0.0;
    m_pos_geo.alt = 0.0;

    geo2wgs( m_pos_geo, m_pos_wgs );
    update();
}

////////////////////////////////////////////////////////////////////////////////

WGS84::WGS84( const WGS84 &wgs )
{
    m_pos_wgs = wgs.m_pos_wgs;

    m_pos_geo.lat = wgs.m_pos_geo.lat;
    m_pos_geo.lon = wgs.m_pos_geo.lon;
    m_pos_geo.alt = wgs.m_pos_geo.alt;

    m_norm_wgs = wgs.m_norm_wgs;
    m_grav_wgs = wgs.m_grav_wgs;

    m_T_ned2wgs = wgs.m_T_ned2wgs;
    m_T_wgs2ned = wgs.m_T_wgs2ned;
}

////////////////////////////////////////////////////////////////////////////////

WGS84::WGS84( const Geo &pos_geo )
{
    setPos_Geo( pos_geo );
}

////////////////////////////////////////////////////////////////////////////////

WGS84::WGS84( const Vector3 &pos_wgs )
{
    setPos_WGS( pos_wgs );
}

////////////////////////////////////////////////////////////////////////////////

Angles WGS84::getAngles_NED( const Angles &angles_wgs ) const
{
    return getNED2BAS( Quaternion( angles_wgs ) ).getAngles();
}

////////////////////////////////////////////////////////////////////////////////

Angles WGS84::getAngles_WGS( const Angles &angles_ned ) const
{
    return getWGS2BAS( Quaternion( angles_ned ) ).getAngles();
}

////////////////////////////////////////////////////////////////////////////////

Quaternion WGS84::getNED2BAS( const Quaternion &wgs2bas ) const
{
    return m_ned2wgs * wgs2bas;
}

////////////////////////////////////////////////////////////////////////////////

Quaternion WGS84::getWGS2BAS( const Quaternion &ned2bas ) const
{
    return m_wgs2ned * ned2bas;
}

////////////////////////////////////////////////////////////////////////////////

void WGS84::setPos_Geo( const Geo &pos_geo )
{
    m_pos_geo.lat = pos_geo.lat;
    m_pos_geo.lon = pos_geo.lon;
    m_pos_geo.alt = pos_geo.alt;

    geo2wgs( m_pos_geo, m_pos_wgs );
    update();
}

////////////////////////////////////////////////////////////////////////////////

void WGS84::setPos_WGS( const Vector3 &pos_wgs )
{
    m_pos_wgs = pos_wgs;

    wgs2geo( m_pos_wgs, m_pos_geo );
    update();
}

////////////////////////////////////////////////////////////////////////////////

void WGS84::update()
{
    double cosLat = cos( m_pos_geo.lat );
    double cosLon = cos( m_pos_geo.lon );
    double sinLat = sin( m_pos_geo.lat );
    double sinLon = sin( m_pos_geo.lon );

    double sinLat2 = sinLat*sinLat;

    // normal to ellipsoid
    m_norm_wgs( 0 ) = cosLat * cos( m_pos_geo.lon );
    m_norm_wgs( 1 ) = cosLat * sin( m_pos_geo.lon );
    m_norm_wgs( 2 ) = sin( m_pos_geo.lat );

    // gravity formula (NIMA TR-8350.2 - Department of Defence World Geodetic System 1984, p. 4-2)
    double gamma_0 = m_gamma_e * ( 1.0 + m_k * sinLat2 ) / sqrt( 1.0 - m_e2 * sinLat2 );
    double gamma_h = gamma_0 * ( 1.0 - (2.0/m_a)*(1.0 + m_f + m_m - 2.0*m_f*sinLat2)*m_pos_geo.alt + (3.0/m_a2)*m_pos_geo.alt*m_pos_geo.alt );

    m_grav_wgs = -gamma_h * m_norm_wgs;

    // NED to WGS
    m_T_ned2wgs(0,0) = -cosLon*sinLat;
    m_T_ned2wgs(0,1) = -sinLon;
    m_T_ned2wgs(0,2) = -cosLon*cosLat;

    m_T_ned2wgs(1,0) = -sinLon*sinLat;
    m_T_ned2wgs(1,1) =  cosLon;
    m_T_ned2wgs(1,2) = -sinLon*cosLat;

    m_T_ned2wgs(2,0) =  cosLat;
    m_T_ned2wgs(2,1) =  0.0;
    m_T_ned2wgs(2,2) = -sinLat;

    m_ned2wgs = m_T_ned2wgs.getQuaternion();
    m_enu2wgs = m_enu2ned * m_ned2wgs;

    // WGS to NED
    m_T_wgs2ned(0,0) = -cosLon * sinLat;
    m_T_wgs2ned(0,1) = -sinLon * sinLat;
    m_T_wgs2ned(0,2) =  cosLat;

    m_T_wgs2ned(1,0) = -sinLon;
    m_T_wgs2ned(1,1) =  cosLon;
    m_T_wgs2ned(1,2) =  0.0;

    m_T_wgs2ned(2,0) = -cosLon * cosLat;
    m_T_wgs2ned(2,1) = -sinLon * cosLat;
    m_T_wgs2ned(2,2) = -sinLat;

    m_wgs2ned = m_T_wgs2ned.getQuaternion();
    m_wgs2enu = m_wgs2ned * m_ned2enu;
}
