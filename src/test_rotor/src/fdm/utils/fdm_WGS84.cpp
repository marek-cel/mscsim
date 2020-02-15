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

#include <fdm/utils/fdm_WGS84.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

const double WGS84::_a = 6378137.0;
const double WGS84::_f = 1.0 / 298.257223563;

const double WGS84::_b   = 6356752.3142;
const double WGS84::_r1  = ( 2.0 * WGS84::_a + WGS84::_b ) / 3.0;
const double WGS84::_a2  = WGS84::_a * WGS84::_a;
const double WGS84::_b2  = WGS84::_b * WGS84::_b;
const double WGS84::_e2  = 6.6943799901400e-3;
const double WGS84::_e   = 8.1819190842622e-2;
const double WGS84::_ep2 = 6.7394967422800e-3;
const double WGS84::_ep  = 8.2094437949696e-2;

const double WGS84::_g       = 9.80665;
const double WGS84::_gm      = 3986004.418e8;
const double WGS84::_omega   = 7.2921151467e-5;
const double WGS84::_gamma_e = 9.7803253359;
const double WGS84::_gamma_p = 9.8321849378;
const double WGS84::_k       = 0.00193185265241;
const double WGS84::_m       = 0.00344978650684;

//0.0,  1.0,  0.0
//1.0,  0.0,  0.0
//0.0,  0.0, -1.0

const Matrix3x3 WGS84::_enu2ned( 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, -1.0 );
const Matrix3x3 WGS84::_ned2enu( 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, -1.0 );

////////////////////////////////////////////////////////////////////////////////

WGS84::Geo WGS84::getGeoOffset( const Geo &pos_geo, double heading,
                                double offset_x, double offset_y )
{
    Matrix3x3 ned2bas( Angles( 0.0, 0.0, heading ) );
    Matrix3x3 bas2ned = ned2bas.getTransposed();

    WGS84 wgs( pos_geo );

    Vector3 r_bas( offset_x, offset_y, 0.0 );
    Vector3 r_ned = bas2ned * r_bas;

    Vector3 pos_wgs = wgs.getPos_WGS() + wgs.getNED2WGS() * r_ned;

    return WGS84::wgs2geo( pos_wgs );
}

////////////////////////////////////////////////////////////////////////////////

Vector3 WGS84::getOmega_WGS()
{
    return Vector3( 0.0, 0.0, _omega );
}

////////////////////////////////////////////////////////////////////////////////

void WGS84::geo2wgs( double lat, double lon, double alt,
                     double &x, double &y, double &z )
{
    double sinLat = sin( lat );
    double cosLat = cos( lat );
    double sinLon = sin( lon );
    double cosLon = cos( lon );

    double n = _a / sqrt( 1.0 - _e2 * sinLat*sinLat );

    x = ( n + alt ) * cosLat * cosLon;
    y = ( n + alt ) * cosLat * sinLon;
    z = ( n * ( _b2 / _a2 ) + alt ) * sinLat;
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
    double tht = atan2( z*_a, p*_b );
    double ed2 = ( _a2 - _b2 ) / _b2;

    double sinTht = sin( tht );
    double cosTht = cos( tht );

    lat = atan( (z + ed2*_b*sinTht*sinTht*sinTht) / ( p - _e2*_a*cosTht*cosTht*cosTht ) );
    lon = atan2( y, x );

    double sinLat = sin( lat );
    double n = _a / sqrt( 1.0 - _e2*sinLat*sinLat );

    alt = p / cos( lat ) - n;
#   else
    double z2 = z*z;
    double r  = sqrt( x*x + y*y );
    double r2 = r*r;
    double e2 = _a2 - _b2;
    double f  = 54.0 * _b2 * z2;
    double g  = r2 + ( 1.0 - _e2 )*z2 - _e2*e2;
    double c  = _e2*_e2 * f * r2 / ( g*g*g );
    double s  = pow( 1.0 + c + sqrt( c*c + 2.0*c ), 1.0/3.0 );
    double p0 = s + 1.0/s + 1.0;
    double p  = f / ( 3.0 * p0*p0 * g*g );
    double q  = sqrt( 1.0 + 2.0*( _e2*_e2 )*p );
    double r0 = -( p * _e2 * r )/( 1.0 + q ) + sqrt( 0.5*_a2*( 1.0 + 1.0/q ) - p*( 1.0 - _e2 )*z2/( q + q*q ) - 0.5*p*r2 );
    double uv = r - _e2*r0;
    double u  = sqrt( uv*uv + z2 );
    double v  = sqrt( uv*uv + ( 1.0 - _e2 )*z2 );
    double z0 = _b2 * z / ( _a * v );

    alt = u * ( 1.0 - _b2 / ( _a * v ) );
    lat = atan( ( z + _ep2*z0 )/r );
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
    _pos_geo.lat = 0.0;
    _pos_geo.lon = 0.0;
    _pos_geo.alt = 0.0;

    geo2wgs( _pos_geo, _pos_wgs );
    update();
}

////////////////////////////////////////////////////////////////////////////////

WGS84::WGS84( const WGS84 &wgs )
{
    _pos_wgs = wgs._pos_wgs;

    _pos_geo.lat = wgs._pos_geo.lat;
    _pos_geo.lon = wgs._pos_geo.lon;
    _pos_geo.alt = wgs._pos_geo.alt;

    _norm_wgs = wgs._norm_wgs;
    _grav_wgs = wgs._grav_wgs;

    _enu2wgs = wgs._enu2wgs;
    _ned2wgs = wgs._ned2wgs;
    _wgs2enu = wgs._wgs2enu;
    _wgs2ned = wgs._wgs2ned;
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

Quaternion WGS84::getNED2BAS(const Quaternion &att_wgs ) const
{
    return _ned2wgs.getQuaternion() * att_wgs;
}

////////////////////////////////////////////////////////////////////////////////

Quaternion WGS84::getWGS2BAS(const Quaternion &att_ned ) const
{
    return _wgs2ned.getQuaternion() * att_ned;
}

////////////////////////////////////////////////////////////////////////////////

void WGS84::setPos_Geo( const Geo &pos_geo )
{
    _pos_geo.lat = pos_geo.lat;
    _pos_geo.lon = pos_geo.lon;
    _pos_geo.alt = pos_geo.alt;

    geo2wgs( _pos_geo, _pos_wgs );
    update();
}

////////////////////////////////////////////////////////////////////////////////

void WGS84::setPos_WGS( const Vector3 &pos_wgs )
{
    _pos_wgs = pos_wgs;

    wgs2geo( _pos_wgs, _pos_geo );
    update();
}

////////////////////////////////////////////////////////////////////////////////

void WGS84::update()
{
    double cosLat = cos( _pos_geo.lat );
    double cosLon = cos( _pos_geo.lon );
    double sinLat = sin( _pos_geo.lat );
    double sinLon = sin( _pos_geo.lon );

    double sinLat2 = sinLat*sinLat;

    // normal to ellipsoid
    _norm_wgs( 0 ) = cosLat * cos( _pos_geo.lon );
    _norm_wgs( 1 ) = cosLat * sin( _pos_geo.lon );
    _norm_wgs( 2 ) = sin( _pos_geo.lat );

    // gravity formula (NIMA TR-8350.2 - Department of Defence World Geodetic System 1984, p. 4-2)
    double gamma_0 = _gamma_e * ( 1.0 + _k * sinLat2 ) / sqrt( 1.0 - _e2 * sinLat2 );
    double gamma_h = gamma_0 * ( 1.0 - (2.0/_a)*(1.0 + _f + _m - 2.0*_f*sinLat2)*_pos_geo.alt + (3.0/_a2)*_pos_geo.alt*_pos_geo.alt );

    _grav_wgs = -gamma_h * _norm_wgs;

    // NED to WGS
    _ned2wgs(0,0) = -cosLon*sinLat;
    _ned2wgs(0,1) = -sinLon;
    _ned2wgs(0,2) = -cosLon*cosLat;

    _ned2wgs(1,0) = -sinLon*sinLat;
    _ned2wgs(1,1) =  cosLon;
    _ned2wgs(1,2) = -sinLon*cosLat;

    _ned2wgs(2,0) =  cosLat;
    _ned2wgs(2,1) =  0.0;
    _ned2wgs(2,2) = -sinLat;

    _enu2wgs = _enu2ned * _ned2wgs;

    // WGS to NED
    _wgs2ned(0,0) = -cosLon * sinLat;
    _wgs2ned(0,1) = -sinLon * sinLat;
    _wgs2ned(0,2) =  cosLat;

    _wgs2ned(1,0) = -sinLon;
    _wgs2ned(1,1) =  cosLon;
    _wgs2ned(1,2) =  0.0;

    _wgs2ned(2,0) = -cosLon * cosLat;
    _wgs2ned(2,1) = -sinLon * cosLat;
    _wgs2ned(2,2) = -sinLat;

    _wgs2enu = _wgs2ned * _ned2enu;
}
