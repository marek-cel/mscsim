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
#ifndef FDM_WGS84_H
#define FDM_WGS84_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/utils/fdm_Angles.h>
#include <fdm/utils/fdm_Matrix3x3.h>
#include <fdm/utils/fdm_Quaternion.h>
#include <fdm/utils/fdm_Vector3.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief US Department of Defense World Geodetic System 1984 (WGS) class.
 *
 * This class is used to store and calculate location expressed in World
 * Geodetic System 1984 (WGS). It also provides functions to calculate
 * rotation matricies between WGS and local North-East-Down (NED) as well as
 * between WGS and local East-North-Up (ENU) axis systems and function
 * calculating local normal to ellipsoid vector and gravity acceleration.
 *
 * @see Department of Defence World Geodetic System 1984, NIMA, TR-8350.2, 2000
 * @see Bowring B.: Transformation from spatial to geocentric coordinates, 1976
 * @see Zhu J.: Conversion of Earth-centered Earth-fixed coordinates to geodetic coordinates, 1994
 */
class FDMEXPORT WGS84
{
public:

    /** Geodetic coordinates. */
    struct Geo
    {
        double lat;                     ///< [rad] geodetic latitude (positive north)
        double lon;                     ///< [rad] geodetic longitude (positive east)
        double alt;                     ///< [m] altitude above mean sea level
    };

    static const double _a;             ///< [m] equatorial radius
    static const double _f;             ///< [-] ellipsoid flattening

    static const double _b;             ///< [m] polar radius
    static const double _r1;            ///< [m] mean radius
    static const double _a2;            ///< [m^2] equatorial radius squared
    static const double _b2;            ///< [m^2] polar radius squared
    static const double _e2;            ///< [-] ellipsoid first eccentricity squared
    static const double _e;             ///< [-] ellipsoid first eccentricity
    static const double _ep2;           ///< [-] ellipsoid second eccentricity squared
    static const double _ep;            ///< [-] ellipsoid second eccentricity

    static const double _g;             ///< [m/s^2] standard gravitional acceleration
    static const double _gm;            ///< [m^3/s^2] Earth’s gravitational constant (mass of Earth’s atmosphere included)
    static const double _omega;         ///< [rad/s] angular velocity of the Earth ( 360deg / 23:56:04.091 )
    static const double _gamma_e;       ///< [m/s^2] theoretical (normal) gravity at the equator (on the ellipsoid)
    static const double _gamma_p;       ///< [m/s^2] theoretical (normal) gravity at the pole (on the ellipsoid)
    static const double _k;             ///< [-] theoretical (normal) gravity formula constant
    static const double _m;             ///< [-] m = omega^2 * a^2 * b / GM

    static const Matrix3x3 _enu2ned;    ///< matrix of rotation from ENU to NED
    static const Matrix3x3 _ned2enu;    ///< matrix of rotation from NED to ENU

    /**
     * Calculates coordinates moved by the given offset.
     * @param pos_geo geodetic coordinates
     * @param heading [rad] heading
     * @param offset_x [m] longitudinal offset
     * @param offset_y [m] lateral offset
     * @return resulting geodetic coordinates
     */
    static Geo getGeoOffset( const Geo &pos_geo, double heading,
                             double offset_x, double offset_y );

    /**
     * Returns Earth angular velocity vector expressed in WGS axis system.
     * @return [rad/s] Earth angular velocity vector expressed in WGS
     */
    static Vector3 getOmega_WGS();

    /**
     * Converts geodetic coordinates into WGS coordinates.
     * @param lat geodetic latitude [rad]
     * @param lon geodetic longitude [rad]
     * @param alt altitude above mean sea level [m]
     * @param x resulting WGS x-coordinate [m]
     * @param y resulting WGS y-coordinate [m]
     * @param z resulting WGS z-coordinate [m]
     */
    static void geo2wgs( double lat, double lon, double alt,
                         double &x, double &y, double &z );

    /**
     * Converts geodetic coordinates into WGS coordinates.
     * @param lat geodetic latitude [rad]
     * @param lon geodetic longitude [rad]
     * @param alt altitude above mean sea level [m]
     * @return resulting WGS coordinates vector [m]
     */
    static Vector3 geo2wgs( double lat, double lon, double alt );

    /**
     * Converts geodetic coordinates into WGS coordinates.
     * @param pos_geo geodetic coordinates
     * @return resulting WGS coordinates vector [m]
     */
    static Vector3 geo2wgs( const Geo &pos_geo );

    /**
     * Converts geodetic coordinates into WGS coordinates.
     * @param pos_geo geodetic coordinates
     * @param pos_wgs resulting WGS coordinates vector [m]
     */
    static void geo2wgs( const Geo &pos_geo, Vector3 &pos_wgs );

    /**
     * Converts WGS coordinates into geodetic coordinates.
     * @param x WGS x-coordinate [m]
     * @param y WGS y-coordinate [m]
     * @param z WGS z-coordinate [m]
     * @param lat resulting geodetic latitude [rad]
     * @param lon resulting geodetic longitude [rad]
     * @param alt resulting altitude above mean sea level [m]
     */
    static void wgs2geo( double x, double y, double z,
                         double &lat, double &lon, double &alt );

    /**
     * Converts WGS coordinates into geodetic coordinates.
     * @param x WGS x-coordinate [m]
     * @param y WGS y-coordinate [m]
     * @param z WGS z-coordinate [m]
     * @return resulting geodetic coordinates
     */
    static Geo wgs2geo( double x, double y, double z );

    /**
     * Converts WGS coordinates into geodetic coordinates.
     * @param pos_wgs WGS coordinates vector [m]
     * @return resulting geodetic coordinates
     */
    static Geo wgs2geo( const Vector3 &pos_wgs );

    /**
     * Converts WGS coordinates into geodetic coordinates.
     * @param pos_wgs WGS coordinates vector [m]
     * @param pos_geo resulting geodetic coordinates
     */
    static void wgs2geo( const Vector3 &pos_wgs, Geo &pos_geo );

    /** Constructor. */
    WGS84();

    /** Copy constructor. */
    WGS84( const WGS84 &wgs );

    /** Constructor. */
    WGS84( const Geo &pos_geo );

    /** Constructor. */
    WGS84( const Vector3 &pos_wgs );

    Angles getAngles_NED( const Angles &angles_wgs ) const;
    Angles getAngles_WGS( const Angles &angles_ned ) const;

    Quaternion getNED2BAS( const Quaternion &att_wgs ) const;
    Quaternion getWGS2BAS( const Quaternion &att_ned ) const;

    inline Geo     getPos_Geo() const { return _pos_geo; }
    inline Vector3 getPos_WGS() const { return _pos_wgs; }

    inline Vector3 getNorm_WGS() const { return _norm_wgs; }
    inline Vector3 getGrav_WGS() const { return _grav_wgs; }

    inline Matrix3x3 getENU2NED() const { return _enu2ned; }
    inline Matrix3x3 getNED2ENU() const { return _ned2enu; }
    inline Matrix3x3 getENU2WGS() const { return _enu2wgs; }
    inline Matrix3x3 getNED2WGS() const { return _ned2wgs; }
    inline Matrix3x3 getWGS2ENU() const { return _wgs2enu; }
    inline Matrix3x3 getWGS2NED() const { return _wgs2ned; }

    /** */
    void setPos_Geo( const Geo &pos_geo );

    /** */
    void setPos_WGS( const Vector3 &pos_wgs );

private:

    Geo _pos_geo;           ///< geodetic coordinates

    Vector3 _pos_wgs;       ///< [m] coordinates vector expressed in WGS

    Vector3 _norm_wgs;      ///< [-] normal to ellipsoid vector expressed in WGS
    Vector3 _grav_wgs;      ///< [m/s^2] gravity acceleration vector expressed in WGS

    Matrix3x3 _enu2wgs;     ///< rotation matrix from ENU to WGS
    Matrix3x3 _ned2wgs;     ///< rotation matrix from NED to WGS
    Matrix3x3 _wgs2enu;     ///< rotation matrix from WGS to ENU
    Matrix3x3 _wgs2ned;     ///< rotation matrix from WGS to NED

    /**
     * This function updates location (normal to ellipsoid vector, gravity
     * acceleration vector and rotation matricies) due to current WGS coordinates.
     */
    void update();
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_WGS84_H
