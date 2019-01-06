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
#ifndef CGI_WGS84_H
#define CGI_WGS84_H

////////////////////////////////////////////////////////////////////////////////

#include <osg/Quat>
#include <osg/Vec3>

////////////////////////////////////////////////////////////////////////////////

namespace cgi
{

/**
 * @brief WGS84 class.
 */
class WGS84
{
public:

    static const osg::Quat m_enu2ned;

    static double getRadiusEquatorial();

    /** Constructor. */
    WGS84();

    /** Constructor. */
    WGS84( double lat, double lon, double alt );

    /** Constructor. */
    WGS84( const osg::Vec3d &position );

    /** Destructor. */
    virtual ~WGS84();

    inline double getLat() const { return m_lat; }
    inline double getLon() const { return m_lon; }
    inline double getAlt() const { return m_alt; }

    inline osg::Quat  getAttitude() const { return m_attitude; }
    inline osg::Vec3d getPosition() const { return m_position; }

    void set( double lat, double lon, double alt );

    void set( const osg::Vec3d &position );

    inline void setLat( double lat ) { set( lat, m_lon, m_alt ); }
    inline void setLon( double lon ) { set( m_lat, lon, m_alt ); }
    inline void setAlt( double alt ) { set( m_lat, m_lon, alt ); }

private:

    double m_lat;
    double m_lon;
    double m_alt;

    osg::Quat  m_attitude;
    osg::Vec3d m_position;
};

} // end of cgi namespace

////////////////////////////////////////////////////////////////////////////////

#endif // CGI_WGS84_H
