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

#include <iostream>

#include <QFile>

#include <fdm/fdm_Path.h>
#include <fdm/utils/fdm_Units.h>

#include <Common.h>
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
    _ils ( NULLPTR ),
    _ndb ( NULLPTR ),
    _vor ( NULLPTR ),

    _course  ( 0.0 ),

    _adf_freq ( 190 ),
    _nav_freq ( 11800 ),

    _adf_visible ( false ),
    _adf_bearing ( 0.0 ),

    _ils_gs_visible ( false ),
    _ils_lc_visible ( false ),
    _ils_gs_deviation ( 0.0 ),
    _ils_lc_deviation ( 0.0 ),
    _ils_gs_norm ( 0.0 ),
    _ils_lc_norm ( 0.0 ),

    _nav_cdi ( Data::Navigation::NONE ),
    _nav_dme ( false ),
    _nav_bearing   ( 0.0 ),
    _nav_deviation ( 0.0 ),
    _nav_distance  ( 0.0 ),
    _nav_norm ( 0.0 )
{
    QFile file( fdm::Path::get( "data/navigation.xml" ).c_str() );

    if ( file.open(QFile::ReadOnly | QFile::Text) )
    {
        QDomDocument doc;

        doc.setContent( &file, false );

        QDomElement rootNode = doc.documentElement();

        if ( rootNode.tagName() == "navigation" )
        {
            QDomElement navaidNode = rootNode.firstChildElement();

            while ( !navaidNode.isNull() )
            {
                parseNavaid( navaidNode );
                navaidNode = navaidNode.nextSiblingElement();
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

    while ( _course < 0.0        ) _course += 2.0 * M_PI;
    while ( _course > 2.0 * M_PI ) _course -= 2.0 * M_PI;

    updateNavaids();

    Data::get()->navigation.course = _course;

    Data::get()->navigation.adf_visible = _adf_visible;
    Data::get()->navigation.adf_bearing = _adf_bearing;

    Data::get()->navigation.ils_visible      = _ils_gs_visible && _ils_lc_visible;
    Data::get()->navigation.ils_gs_visible   = _ils_gs_visible;
    Data::get()->navigation.ils_lc_visible   = _ils_lc_visible;
    Data::get()->navigation.ils_gs_deviation = _ils_gs_deviation;
    Data::get()->navigation.ils_lc_deviation = _ils_lc_deviation;
    Data::get()->navigation.ils_gs_norm = _ils_gs_norm;
    Data::get()->navigation.ils_lc_norm = _ils_lc_norm;

    Data::get()->navigation.nav_cdi = _nav_cdi;
    Data::get()->navigation.nav_dme = _nav_dme;
    Data::get()->navigation.nav_bearing   = _nav_bearing;
    Data::get()->navigation.nav_deviation = _nav_deviation;
    Data::get()->navigation.nav_distance  = _nav_distance;
    Data::get()->navigation.nav_norm = _nav_norm;
}

////////////////////////////////////////////////////////////////////////////////

double Navigation::getAzimuth( const fdm::Vector3 &pos_wgs )
{
    fdm::Vector3 pos_ned = _aircraft_wgs.getWGS2NED() * ( pos_wgs - _aircraft_wgs.getPos_WGS() );

    double azim = atan2(  pos_ned.y(), pos_ned.x() );

    if      ( azim < 0.0        ) azim += 2.0 * M_PI;
    else if ( azim > 2.0 * M_PI ) azim -= 2.0 * M_PI;

    return azim;
}

////////////////////////////////////////////////////////////////////////////////

double Navigation::getDistance( const fdm::Vector3 &pos_wgs )
{
    // orthodromic distance
    return fdm::WGS84::_r1 * acos( pos_wgs.getNormalized() * _aircraft_wgs.getPos_WGS().getNormalized() );
}

////////////////////////////////////////////////////////////////////////////////

void Navigation::parseNavaid( const QDomElement &node )
{
    if ( !node.isNull() )
    {
        if      ( node.tagName() == "ils" ) parseILS( node );
        else if ( node.tagName() == "ndb" ) parseNDB( node );
        else if ( node.tagName() == "vor" ) parseVOR( node );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Navigation::parseILS( const QDomElement &node )
{
    if ( !node.isNull() )
    {
        QDomElement nodeIdentifier = node.firstChildElement( "ident" );
        QDomElement nodeFrequncy   = node.firstChildElement( "freq" );
        QDomElement nodeGlideSlope = node.firstChildElement( "glide_slope" );
        QDomElement nodeHeading    = node.firstChildElement( "heading" );

        QDomElement nodePositionGS  = node.firstChildElement( "position_gs" );
        QDomElement nodePositionLOC = node.firstChildElement( "position_loc" );
        QDomElement nodePositionDME = node.firstChildElement( "position_dme" );

        if ( !nodeIdentifier.isNull()
          && !nodeFrequncy.isNull()
          && !nodeGlideSlope.isNull()
          && !nodeHeading.isNull()
          && !nodePositionLOC.isNull() )
        {
            ILS ils;

            ils.ident       = nodeIdentifier.text().toStdString();
            ils.freq        = 1000 * nodeFrequncy.text().toDouble();
            ils.glide_slope = fdm::Units::deg2rad( nodeGlideSlope.text().toDouble() );
            ils.heading     = fdm::Units::deg2rad( nodeHeading.text().toDouble() );

            ils.pos_wgs_loc = readPosition( nodePositionLOC );

            if ( !nodePositionGS  .isNull() ) ils.pos_wgs_gs  = readPosition( nodePositionGS  );
            if ( !nodePositionDME .isNull() ) ils.pos_wgs_dme = readPosition( nodePositionDME );

            ils.dme = !nodePositionDME.isNull();

            _listILS.append( ils );
        }
    }
}


////////////////////////////////////////////////////////////////////////////////

void Navigation::parseNDB( const QDomElement &node )
{
    if ( !node.isNull() )
    {
        NDB ndb;

        ndb.freq = node.firstChildElement( "freq" ).text().toInt();

        QDomElement positionNode = node.firstChildElement( "position" );
        ndb.pos_wgs = readPosition( positionNode );

        _listNDB.append( ndb );
    }
}


////////////////////////////////////////////////////////////////////////////////

void Navigation::parseVOR( const QDomElement &node )
{
    if ( !node.isNull() )
    {
        QDomElement nodeIdentifier = node.firstChildElement( "ident" );
        QDomElement nodeFrequncy   = node.firstChildElement( "freq" );

        QDomElement nodePosition = node.firstChildElement( "position" );

        if ( !nodeIdentifier.isNull()
          && !nodeFrequncy.isNull()
          && !nodePosition.isNull() )
        {
            VOR vor;

            vor.ident = nodeIdentifier.text().toStdString();
            vor.freq  = 1000 * nodeFrequncy.text().toFloat();

            vor.pos_wgs = readPosition( nodePosition );

            vor.dme = true; // TODO

            _listVOR.append( vor );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

fdm::Vector3 Navigation::readPosition( const QDomElement &node )
{
    fdm::WGS84::Geo position_geo;

    position_geo.lat = 0.0;
    position_geo.lon = 0.0;
    position_geo.alt = 0.0;

    QDomElement nodeLat = node.firstChildElement( "lat" );
    QDomElement nodeLon = node.firstChildElement( "lon" );
    QDomElement nodeAlt = node.firstChildElement( "alt" );

    if ( !nodeLat.isNull() && !nodeLon.isNull() && !nodeAlt.isNull() )
    {
        position_geo.lat = fdm::Units::deg2rad( nodeLat.text().toDouble() );
        position_geo.lon = fdm::Units::deg2rad( nodeLon.text().toDouble() );
        position_geo.alt = nodeAlt.text().toDouble();
    }

    return fdm::WGS84::geo2wgs( position_geo );
}

////////////////////////////////////////////////////////////////////////////////

void Navigation::updateNavaids()
{
    _adf_visible = false;
    _adf_bearing = 0.0;

    _ils_gs_visible = false;
    _ils_lc_visible = false;

    _ils_gs_deviation = 0.0;
    _ils_lc_deviation = 0.0;

    _nav_cdi = Data::Navigation::NONE;
    _nav_dme = false;
    _nav_bearing   = 0.0;
    _nav_deviation = 0.0;
    _nav_distance  = 0.0;

    updateNavaidsActive();

    updateADF();
    updateILS();
    updateNAV();
}

////////////////////////////////////////////////////////////////////////////////

void Navigation::updateNavaidsActive()
{
    if ( _ndb )
    {
        if ( _ndb->freq != _adf_freq || getDistance( _ndb->pos_wgs ) > _range_ndb )
            _ndb = NULLPTR;
    }

    if ( _ils )
    {
        if ( _ils->freq != _nav_freq || getDistance( _ils->pos_wgs_loc ) > _range_ils )
            _ils = NULLPTR;
    }

    if ( _vor )
    {
        if ( _vor->freq != _nav_freq || getDistance( _vor->pos_wgs ) > _range_vor )
            _vor = NULLPTR;
    }

    if ( !_ndb )
    {
        for ( ListNDB::iterator it = _listNDB.begin(); it != _listNDB.end(); it++ )
        {
            if ( _adf_freq == (*it).freq && getDistance( (*it).pos_wgs ) < _range_ndb )
            {
                _ndb = &(*it);
                break;
            }
        }
    }

    if ( !_ils )
    {
        for ( ListILS::iterator it = _listILS.begin(); it != _listILS.end(); it++ )
        {
            if ( _nav_freq == (*it).freq && getDistance( (*it).pos_wgs_loc ) < _range_ils )
            {
                _ils = &(*it);
                break;
            }
        }
    }

    if ( !_vor )
    {
        for ( ListVOR::iterator it = _listVOR.begin(); it != _listVOR.end(); it++ )
        {
            if ( _nav_freq == (*it).freq && getDistance( (*it).pos_wgs ) < _range_vor )
            {
                _vor = &(*it);
                break;
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Navigation::updateADF()
{
    if ( _ndb )
    {
        _adf_visible = true;
        _adf_bearing = getAzimuth( _ndb->pos_wgs );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Navigation::updateILS()
{
    if ( _ils )
    {
        fdm::Vector3 pos_ned_gs = _aircraft_wgs.getWGS2NED() * ( _ils->pos_wgs_gs - _aircraft_wgs.getPos_WGS() );
        fdm::Vector3 pos_rwy_gs = fdm::Matrix3x3( fdm::Angles( 0.0, 0.0, _ils->heading ) ) * pos_ned_gs;

        fdm::Vector3 pos_ned_dme = _aircraft_wgs.getWGS2NED() * ( _ils->pos_wgs_dme - _aircraft_wgs.getPos_WGS() );

        double azim = getAzimuth( _ils->pos_wgs_loc );
        double elev = atan2( -pos_rwy_gs.z(), pos_rwy_gs.x() );

        if ( fabs( azim - _ils->heading ) < fdm::Units::deg2rad( 10.0 ) )
        {
            _nav_dme = _ils->dme;
            _nav_distance = _nav_dme ? pos_ned_dme.getLength() : 0.0;

            updateNAV( azim, _course, _ils_lc_max );

            _ils_lc_visible = _nav_cdi != Data::Navigation::NONE;
            _ils_lc_deviation = _nav_deviation;
            _ils_lc_norm = _nav_norm;

            double elev_rel = elev + _ils->glide_slope;

            if ( pos_rwy_gs.x() > 0.0 )
            {
                _ils_gs_visible = true;

                _ils_gs_deviation = elev_rel;

                _ils_gs_norm = _ils_gs_deviation / _ils_gs_max;
                _ils_gs_norm = fdm::Misc::satur( -1.0, 1.0, _ils_gs_norm );
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Navigation::updateNAV()
{
    if ( _vor )
    {
        _nav_dme = true; // TODO
        _nav_distance = getDistance( _vor->pos_wgs );

        double azim = getAzimuth( _vor->pos_wgs );

        updateNAV( azim, _course, _vor_max );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Navigation::updateNAV( double azim, double course, double max )
{
    double azim_rel = azim - course;

    while ( azim_rel < -M_PI ) azim_rel += 2.0 * M_PI;
    while ( azim_rel >  M_PI ) azim_rel -= 2.0 * M_PI;

    if ( azim_rel < -M_PI_2 || M_PI_2 < azim_rel )
    {
        _nav_cdi = Data::Navigation::FROM;

        if ( azim_rel < -M_PI_2 ) azim_rel += M_PI;
        if ( azim_rel >  M_PI_2 ) azim_rel -= M_PI;

        azim_rel *= -1.0;
    }
    else
    {
        _nav_cdi = Data::Navigation::TO;
    }

    _nav_bearing = azim;

    _nav_deviation = azim_rel;

    _nav_norm = _nav_deviation / max;
    _nav_norm = fdm::Misc::satur( -1.0, 1.0, _nav_norm );
}
