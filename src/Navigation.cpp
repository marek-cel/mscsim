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

#include <fdm/fdm_Path.h>
#include <fdm/utils/fdm_Units.h>

#include <Data.h>

////////////////////////////////////////////////////////////////////////////////

const double Navigation::_range_ils = fdm::Units::nmi2m( 18.0 );
const double Navigation::_range_ndb = fdm::Units::nmi2m( 40.0 );
const double Navigation::_range_vor = fdm::Units::nmi2m( 40.0 );

const double Navigation::_ils_gs_max = fdm::Units::deg2rad( 0.7 );
const double Navigation::_ils_lc_max = fdm::Units::deg2rad( 2.5 );

const double Navigation::_vor_max = fdm::Units::deg2rad( 10.0 );

////////////////////////////////////////////////////////////////////////////////

Navigation::Navigation() :
    _course ( 0.0f ),

    _adf_visible ( false ),
    _adf_bearing ( 0.0f ),

    _ils_gs_visible ( false ),
    _ils_lc_visible ( false ),
    _ils_gs_deviation ( 0.0f ),
    _ils_lc_deviation ( 0.0f ),

    _nav_visible ( false ),
    _nav_deviation ( 0.0f ),
    _nav_distance ( 0.0f )
{
    QFile file( fdm::Path::get( "data/navigation.xml" ).c_str() );

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

                    _ils.pos_gs_wgs = readPosition( positionNode );

                    if ( !glideSlopeNode.isNull() )
                    {
                        _ils.glide_slope = fdm::Units::deg2rad( glideSlopeNode.text().toDouble() );
                    }
                }

                if ( !lcNode.isNull() )
                {
                    QDomElement positionNode = lcNode.firstChildElement( "position" );
                    QDomElement headingNode  = lcNode.firstChildElement( "heading" );

                    _ils.pos_lc_wgs = readPosition( positionNode );

                    if ( !headingNode.isNull() )
                    {
                        _ils.heading = fdm::Units::deg2rad( headingNode.text().toDouble() );
                    }
                }
            }

            // VOR
            if ( !ndbNode.isNull() )
            {
                QDomElement positionNode = ndbNode.firstChildElement( "position" );

                _ndb.pos_wgs = readPosition( positionNode );
            }

            // VOR
            if ( !vorNode.isNull() )
            {
                QDomElement positionNode = vorNode.firstChildElement( "position" );

                _vor.pos_wgs = readPosition( positionNode );
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

Navigation::~Navigation() {}

////////////////////////////////////////////////////////////////////////////////

void Navigation::update()
{
    _aircraft_wgs.setPos_WGS( fdm::Vector3( Data::get()->ownship.pos_x_wgs,
                                            Data::get()->ownship.pos_y_wgs,
                                            Data::get()->ownship.pos_z_wgs ) );

    updateADF();
    updateILS();
    updateNav();

    Data::get()->navigation.course = _course;

    Data::get()->navigation.adf_visible = _adf_visible;
    Data::get()->navigation.adf_bearing = _adf_bearing;

    Data::get()->navigation.ils_visible      = _ils_gs_visible && _ils_lc_visible;
    Data::get()->navigation.ils_gs_visible   = _ils_gs_visible;
    Data::get()->navigation.ils_lc_visible   = _ils_lc_visible;
    Data::get()->navigation.ils_gs_deviation = _ils_gs_deviation;
    Data::get()->navigation.ils_lc_deviation = _ils_lc_deviation;

    Data::get()->navigation.nav_visible   = _nav_visible;
    Data::get()->navigation.nav_deviation = _nav_deviation;
    Data::get()->navigation.nav_distance  = _nav_distance;
}

////////////////////////////////////////////////////////////////////////////////

double Navigation::getAzimuth( const fdm::Vector3 &pos_wgs )
{
    fdm::Vector3 pos_ned = _aircraft_wgs.getWGS2NED() * ( pos_wgs - _aircraft_wgs.getPos_WGS() );
    return atan2(  pos_ned.y(), pos_ned.x() );
}

////////////////////////////////////////////////////////////////////////////////
double Navigation::getDistance( const fdm::Vector3 &pos_wgs )
{
    // orthodromic distance
    return fdm::WGS84::_r1 * acos( pos_wgs.getNormalized() * _aircraft_wgs.getPos_WGS().getNormalized() );
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
    _adf_visible = false;
    _adf_bearing = 0.0;

    double distance = getDistance( _ndb.pos_wgs );

    if ( distance < _range_ndb )
    {
        _adf_visible = true;

        _adf_bearing = getAzimuth( _ndb.pos_wgs );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Navigation::updateILS()
{
    _ils_gs_visible = false;
    _ils_lc_visible = false;

    _ils_gs_deviation = 0.0;
    _ils_lc_deviation = 0.0;

    double distance_lc = getDistance( _ils.pos_lc_wgs );

    if ( distance_lc < _range_ils )
    {
        fdm::Vector3 pos_gs_ned = _aircraft_wgs.getWGS2NED() * ( _ils.pos_gs_wgs - _aircraft_wgs.getPos_WGS() );
        fdm::Vector3 pos_gs_rwy = fdm::Matrix3x3( fdm::Angles( 0.0, 0.0, _ils.heading ) ) * pos_gs_ned;

        double azim = getAzimuth( _ils.pos_lc_wgs );
        double elev = atan2( -pos_gs_rwy.z(), pos_gs_rwy.x() );

        double azim_rel = azim - _ils.heading;
        double elev_rel = elev + _ils.glide_slope;

        if ( fabs( azim_rel ) < fdm::Units::deg2rad( 10.0 ) )
        {
            _ils_lc_visible = true;
            _nav_visible = true;

            _nav_distance = pos_gs_ned.getLength();

            _ils_lc_deviation = azim_rel / _ils_lc_max;
            _ils_lc_deviation = fdm::Misc::satur( -1.0, 1.0, _ils_lc_deviation );
            _nav_deviation = _ils_lc_deviation;

            if ( pos_gs_rwy.x() > 0.0 )
            {
                _ils_gs_visible = true;

                _ils_gs_deviation = elev_rel / _ils_gs_max;
                _ils_gs_deviation = fdm::Misc::satur( -1.0, 1.0, _ils_gs_deviation );
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Navigation::updateNav()
{
    if ( !_ils_lc_visible )
    {
        _nav_visible = false;
        _nav_deviation = 0.0;

        double distance = getDistance( _vor.pos_wgs );

        if ( distance < _range_vor )
        {
            _nav_visible = true;

            _nav_distance = distance;

            double azim = getAzimuth( _vor.pos_wgs );
            double azim_rel = azim - _ils.heading;

            _nav_deviation = azim_rel / _vor_max;
            _nav_deviation = fdm::Misc::satur( -1.0, 1.0, _nav_deviation );
        }
    }
}
