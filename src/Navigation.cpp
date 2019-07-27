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

#include <Navigation.h>

#include <QFile>

#include <fdm/utils/fdm_Units.h>

#include <Data.h>

////////////////////////////////////////////////////////////////////////////////

const double Navigation::m_range_ils = fdm::Units::nmi2m( 18.0 );
const double Navigation::m_range_ndb = fdm::Units::nmi2m( 40.0 );
const double Navigation::m_range_vor = fdm::Units::nmi2m( 40.0 );

const double Navigation::m_ils_gs_max = fdm::Units::deg2rad( 0.7 );
const double Navigation::m_ils_lc_max = fdm::Units::deg2rad( 2.5 );

const double Navigation::m_vor_max = fdm::Units::deg2rad( 10.0 );

////////////////////////////////////////////////////////////////////////////////

Navigation::Navigation() :
    m_adf_visible ( false ),
    m_adf_bearing ( 0.0f ),

    m_ils_gs_visible ( false ),
    m_ils_lc_visible ( false ),
    m_ils_gs_deviation ( 0.0f ),
    m_ils_lc_deviation ( 0.0f ),

    m_nav_visible ( false ),
    m_nav_deviation ( 0.0f ),
    m_nav_distance ( 0.0f )
{
    QFile file( "data/navigation.xml" );

    if ( file.open(QFile::ReadOnly | QFile::Text) )
    {
        QDomDocument doc;

        doc.setContent( &file, false );

        QDomElement rootNode = doc.documentElement();

        if ( rootNode.tagName() == "navigation" )
        {
            QDomElement ilsNode = rootNode.firstChildElement( "ils" );
            QDomElement ndbNode = rootNode.firstChildElement( "ndb" );
            QDomElement vorNode = rootNode.firstChildElement( "vor" );

            // ILS
            if ( !ilsNode.isNull() )
            {
                QDomElement gsNode = ilsNode.firstChildElement( "gs" );
                QDomElement lcNode = ilsNode.firstChildElement( "lc" );

                if ( !gsNode.isNull() )
                {
                    QDomElement positionNode   = gsNode.firstChildElement( "position" );
                    QDomElement glideSlopeNode = gsNode.firstChildElement( "glide_slope" );

                    m_ils.pos_gs_wgs = readPosition( positionNode );

                    if ( !glideSlopeNode.isNull() )
                    {
                        m_ils.glide_slope = fdm::Units::deg2rad( glideSlopeNode.text().toDouble() );
                    }
                }

                if ( !lcNode.isNull() )
                {
                    QDomElement positionNode = lcNode.firstChildElement( "position" );
                    QDomElement headingNode  = lcNode.firstChildElement( "heading" );

                    m_ils.pos_lc_wgs = readPosition( positionNode );

                    if ( !headingNode.isNull() )
                    {
                        m_ils.heading = fdm::Units::deg2rad( headingNode.text().toDouble() );
                    }
                }
            }

            // VOR
            if ( !ndbNode.isNull() )
            {
                QDomElement positionNode = ndbNode.firstChildElement( "position" );

                m_ndb.pos_wgs = readPosition( positionNode );
            }

            // VOR
            if ( !vorNode.isNull() )
            {
                QDomElement positionNode = vorNode.firstChildElement( "position" );

                m_vor.pos_wgs = readPosition( positionNode );
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

Navigation::~Navigation() {}

////////////////////////////////////////////////////////////////////////////////

void Navigation::update()
{
    m_aircraft_wgs.setPos_WGS( fdm::Vector3( Data::get()->ownship.pos_x_wgs,
                                             Data::get()->ownship.pos_y_wgs,
                                             Data::get()->ownship.pos_z_wgs ) );

    updateADF();
    updateILS();
    updateNav();

    Data::get()->navigation.adf_visible = m_adf_visible;
    Data::get()->navigation.adf_bearing = m_adf_bearing;

    Data::get()->navigation.ils_visible      = m_ils_gs_visible && m_ils_lc_visible;
    Data::get()->navigation.ils_gs_visible   = m_ils_gs_visible;
    Data::get()->navigation.ils_lc_visible   = m_ils_lc_visible;
    Data::get()->navigation.ils_gs_deviation = m_ils_gs_deviation;
    Data::get()->navigation.ils_lc_deviation = m_ils_lc_deviation;

    Data::get()->navigation.nav_visible   = m_nav_visible;
    Data::get()->navigation.nav_deviation = m_nav_deviation;
    Data::get()->navigation.nav_distance  = m_nav_distance;
}

////////////////////////////////////////////////////////////////////////////////

double Navigation::getAzimuth( const fdm::Vector3 &pos_wgs )
{
    fdm::Vector3 pos_ned = m_aircraft_wgs.getWGS2NED() * ( pos_wgs - m_aircraft_wgs.getPos_WGS() );
    return atan2(  pos_ned.y(), pos_ned.x() );
}

////////////////////////////////////////////////////////////////////////////////
double Navigation::getDistance( const fdm::Vector3 &pos_wgs )
{
    // orthodromic distance
    return fdm::WGS84::m_r1 * acos( pos_wgs.getNormalized() * m_aircraft_wgs.getPos_WGS().getNormalized() );
}

////////////////////////////////////////////////////////////////////////////////

fdm::Vector3 Navigation::readPosition( const QDomElement &node )
{
    fdm::WGS84::Geo position_geo;

    position_geo.lat = 0.0;
    position_geo.lon = 0.0;
    position_geo.alt = 0.0;

    if ( node.tagName() == "position" )
    {
        QDomElement nodeLat = node.firstChildElement( "lat" );
        QDomElement nodeLon = node.firstChildElement( "lon" );
        QDomElement nodeAlt = node.firstChildElement( "alt" );

        if ( !nodeLat.isNull() && !nodeLon.isNull() && !nodeAlt.isNull() )
        {
            position_geo.lat = fdm::Units::deg2rad( nodeLat.text().toDouble() );
            position_geo.lon = fdm::Units::deg2rad( nodeLon.text().toDouble() );
            position_geo.alt = nodeAlt.text().toDouble();
        }
    }

    return fdm::WGS84::geo2wgs( position_geo );
}

////////////////////////////////////////////////////////////////////////////////

void Navigation::updateADF()
{
    m_adf_visible = false;
    m_adf_bearing = 0.0;

    double distance = getDistance( m_ndb.pos_wgs );

    if ( distance < m_range_ndb )
    {
        m_adf_visible = true;

        m_adf_bearing = getAzimuth( m_ndb.pos_wgs );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Navigation::updateILS()
{
    m_ils_gs_visible = false;
    m_ils_lc_visible = false;

    m_ils_gs_deviation = 0.0;
    m_ils_lc_deviation = 0.0;

    double distance_lc = getDistance( m_ils.pos_lc_wgs );

    if ( distance_lc < m_range_ils )
    {
        fdm::Vector3 pos_gs_ned = m_aircraft_wgs.getWGS2NED() * ( m_ils.pos_gs_wgs - m_aircraft_wgs.getPos_WGS() );
        fdm::Vector3 pos_gs_rwy = fdm::Matrix3x3( fdm::Angles( 0.0, 0.0, m_ils.heading ) ) * pos_gs_ned;

        double azim = getAzimuth( m_ils.pos_lc_wgs );
        double elev = atan2( -pos_gs_rwy.z(), pos_gs_rwy.x() );

        double azim_rel = azim - m_ils.heading;
        double elev_rel = elev + m_ils.glide_slope;

        if ( fabs( azim_rel ) < fdm::Units::deg2rad( 10.0 ) )
        {
            m_ils_lc_visible = true;
            m_nav_visible = true;

            m_nav_distance = pos_gs_ned.getLength();

            m_ils_lc_deviation = azim_rel / m_ils_lc_max;
            m_ils_lc_deviation = fdm::Misc::satur( -1.0, 1.0, m_ils_lc_deviation );
            m_nav_deviation = m_ils_lc_deviation;

            if ( pos_gs_rwy.x() > 0.0 )
            {
                m_ils_gs_visible = true;

                m_ils_gs_deviation = elev_rel / m_ils_gs_max;
                m_ils_gs_deviation = fdm::Misc::satur( -1.0, 1.0, m_ils_gs_deviation );
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Navigation::updateNav()
{
    if ( !m_ils_lc_visible )
    {
        m_nav_visible = false;
        m_nav_deviation = 0.0;

        double distance = getDistance( m_vor.pos_wgs );

        if ( distance < m_range_vor )
        {
            m_nav_visible = true;

            m_nav_distance = distance;

            double azim = getAzimuth( m_vor.pos_wgs );
            double azim_rel = azim - m_ils.heading;

            m_nav_deviation = azim_rel / m_vor_max;
            m_nav_deviation = fdm::Misc::satur( -1.0, 1.0, m_nav_deviation );
        }
    }
}
