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
 * @brief DoD World Geodetic System 1984 (WGS) class.
 *
 * <p>This class is used to store and calculate location expressed in DoD World
 * Geodetic System 1984 (WGS). It also provides functions to calculate
 * rotation matricies between WGS and local North-East-Down (NED) as well as
 * between WGS and local East-North-Up (ENU) axis systems and function
 * calculating local normal to ellipsoid vector and gravity acceleration.</p>
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

    static const double m_a;            ///< [m] equatorial radius
    static const double m_f;            ///< [-] ellipsoid flattening

    static const double m_b;            ///< [m] polar radius
    static const double m_r1;           ///< [m] mean radius
    static const double m_a2;           ///< [m^2] equatorial radius squared
    static const double m_b2;           ///< [m^2] polar radius squared
    static const double m_e2;           ///< [-] ellipsoid first eccentricity squared
    static const double m_e;            ///< [-] ellipsoid first eccentricity
    static const double m_ep2;          ///< [-] ellipsoid second eccentricity squared
    static const double m_ep;           ///< [-] ellipsoid second eccentricity

    static const double m_g;            ///< [m/s^2] standard gravitional acceleration
    static const double m_gm;           ///< [m^3/s^2] Earth’s gravitational constant (mass of Earth’s atmosphere included)
    static const double m_omega;        ///< [rad/s] angular velocity of the Earth ( 360deg / 23:56:04.091 )
    static const double m_gamma_e;      ///< [m/s^2] theoretical (normal) gravity at the equator (on the ellipsoid)
    static const double m_gamma_p;      ///< [m/s^2] theoretical (normal) gravity at the pole (on the ellipsoid)
    static const double m_k;            ///< [-] theoretical (normal) gravity formula constant
    static const double m_m;            ///< [-] m = omega^2 * a^2 * b / GM

    static const Matrix3x3 m_enu2ned;   ///< matrix of rotation from ENU to NED
    static const Matrix3x3 m_ned2enu;   ///< matrix of rotation from NED to ENU

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

    inline Geo     getPos_Geo() const { return m_pos_geo; }
    inline Vector3 getPos_WGS() const { return m_pos_wgs; }

    inline Vector3 getNorm_WGS() const { return m_norm_wgs; }
    inline Vector3 getGrav_WGS() const { return m_grav_wgs; }

    inline Matrix3x3 getENU2NED() const { return m_enu2ned; }
    inline Matrix3x3 getNED2ENU() const { return m_ned2enu; }
    inline Matrix3x3 getENU2WGS() const { return m_enu2wgs; }
    inline Matrix3x3 getNED2WGS() const { return m_ned2wgs; }
    inline Matrix3x3 getWGS2ENU() const { return m_wgs2enu; }
    inline Matrix3x3 getWGS2NED() const { return m_wgs2ned; }

    /** */
    void setPos_Geo( const Geo &pos_geo );

    /** */
    void setPos_WGS( const Vector3 &pos_wgs );

private:

    Geo m_pos_geo;          ///< geodetic coordinates

    Vector3 m_pos_wgs;      ///< [m] coordinates vector expressed in WGS

    Vector3 m_norm_wgs;     ///< [-] normal to ellipsoid vector expressed in WGS
    Vector3 m_grav_wgs;     ///< [m/s^2] gravity acceleration vector expressed in WGS

    Matrix3x3 m_enu2wgs;    ///< rotation matrix from ENU to WGS
    Matrix3x3 m_ned2wgs;    ///< rotation matrix from NED to WGS
    Matrix3x3 m_wgs2enu;    ///< rotation matrix from WGS to ENU
    Matrix3x3 m_wgs2ned;    ///< rotation matrix from WGS to NED

    /**
     * This function updates location (normal to ellipsoid vector, gravity
     * acceleration vector and rotation matricies) due to current WGS coordinates.
     */
    void update();
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_WGS84_H
