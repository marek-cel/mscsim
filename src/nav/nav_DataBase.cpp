/****************************************************************************//*
 * Copyright (C) 2021 Marek M. Cel
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

#include <nav/nav_DataBase.h>

#include <cstdio>
#include <fstream>

#include <QDomElement>
#include <QFile>

#include <fdm/utils/fdm_String.h>
#include <fdm/utils/fdm_Units.h>
#include <fdm/utils/fdm_WGS84.h>

#include <nav/nav_Frequency.h>

#include <sim/Path.h>

////////////////////////////////////////////////////////////////////////////////

using namespace nav;

////////////////////////////////////////////////////////////////////////////////

double DataBase::getTrueBearing( const char *str_bear, const char *str_magvar )
{
    double bear = fdm::Units::deg2rad( fdm::String::toDouble( str_bear ) );

    bool true_bear = toupper( str_bear[ strlen( str_bear ) - 1 ] ) == 'T';

    std::vector< std::string > temp = fdm::String::split( str_magvar, " " );

    if ( temp.size() > 0 && !true_bear )
    {
        double coef = toupper( temp.at( 0 ).c_str()[ 0 ] ) == 'E' ? 1.0 : -1.0;

        if ( temp.at( 0 ).length() >= 4 )
        {
            std::string str_deg;

            str_deg += temp.at( 0 ).c_str()[ 1 ];
            str_deg += temp.at( 0 ).c_str()[ 2 ];
            str_deg += temp.at( 0 ).c_str()[ 3 ];

            double magvar = fdm::String::toDouble( str_deg );

            if ( temp.at( 0 ).length() == 7 )
            {
                std::string str_dec;

                str_dec += temp.at( 0 ).c_str()[ 4 ];
                str_dec += temp.at( 0 ).c_str()[ 5 ];
                str_dec += temp.at( 0 ).c_str()[ 6 ];

                magvar += fdm::String::toDouble( str_dec ) / 1000.0;
            }

            bear += coef * fdm::Units::deg2rad( magvar );
        }
    }

    return bear;
}

////////////////////////////////////////////////////////////////////////////////

DataBase::DataBase()
{
    QFile file( Path::get( "nav/nav_db.xml" ).c_str() );

    if ( file.open(QFile::ReadOnly | QFile::Text) )
    {
        QDomDocument doc;

        doc.setContent( &file, false );

        QDomElement rootNode = doc.documentElement();

        if ( rootNode.tagName() == "nav_db" )
        {
            QDomElement nodeAPT = rootNode.firstChildElement( "apt" );
            QDomElement nodeILS = rootNode.firstChildElement( "ils" );
            QDomElement nodeNAV = rootNode.firstChildElement( "nav" );
            QDomElement nodeRWY = rootNode.firstChildElement( "rwy" );

            if ( !nodeAPT.isNull() )
            {
                QDomElement fileNode = nodeAPT.firstChildElement( "file" );

                while ( !fileNode.isNull() )
                {
                    parseAPT( fileNode.text().toStdString().c_str() );
                    fileNode = fileNode.nextSiblingElement( "file" );
                }
            }

            if ( !nodeILS.isNull() )
            {
                QDomElement fileNode = nodeILS.firstChildElement( "file" );

                while ( !fileNode.isNull() )
                {
                    parseILS( fileNode.text().toStdString().c_str() );
                    fileNode = fileNode.nextSiblingElement( "file" );
                }
            }

            if ( !nodeNAV.isNull() )
            {
                QDomElement fileNode = nodeNAV.firstChildElement( "file" );

                while ( !fileNode.isNull() )
                {
                    parseNAV( fileNode.text().toStdString().c_str() );
                    fileNode = fileNode.nextSiblingElement( "file" );
                }
            }

            if ( !nodeRWY.isNull() )
            {
                QDomElement fileNode = nodeRWY.firstChildElement( "file" );

                while ( !fileNode.isNull() )
                {
                    parseRWY( fileNode.text().toStdString().c_str() );
                    fileNode = fileNode.nextSiblingElement( "file" );
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

DataBase::~DataBase() {}

////////////////////////////////////////////////////////////////////////////////

void DataBase::parseAPT( const char *path )
{
    std::fstream file( Path::get( path ).c_str(), std::ios_base::in );

    if ( file.is_open() )
    {
        std::string line;

        int i_line = 0;

        while ( !file.eof() )
        {
            getline( file, line );

            if ( i_line > 0 )
            {
                std::vector< std::string > cols = fdm::String::split( line, "\t" );

                line.clear();

                if ( cols.size() == 23 )
                {
                    APT apt;

                    apt.ident = cols.at( 0 );
                    apt.name  = cols.at( 1 );
                    apt.icao  = cols.at( 3 );

                    apt.lat = fdm::Units::deg2rad( fdm::String::toDouble( cols.at(  8 ) ) );
                    apt.lon = fdm::Units::deg2rad( fdm::String::toDouble( cols.at( 10 ) ) );

                    _list_apt.push_back( apt );
                }
            }

            i_line++;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void DataBase::parseILS( const char *path )
{
    std::fstream file( Path::get( path ).c_str(), std::ios_base::in );

    if ( file.is_open() )
    {
        std::string line;

        int i_line = 0;

        while ( !file.eof() )
        {
            getline( file, line );

            if ( i_line > 0 )
            {
                std::vector< std::string > cols = fdm::String::split( line, "\t" );

                line.clear();

                if ( cols.size() == 29 )
                {
                    if ( cols.at( 2 ).length() > 0 )
                    {
                        ILS ils;

                        switch( toupper( cols.at( 2 ).c_str()[ 0 ] ) )
                        {
                            default:  ils.type = ILS::NONE; break;
                            case 'D': ils.type = ILS::DME;  break;
                            case 'Z': ils.type = ILS::LOC;  break;
                            case 'G': ils.type = ILS::GS;   break;
                            case 'B': ils.type = ILS::BCM;  break;
                            case 'I': ils.type = ILS::IM;   break;
                            case 'M': ils.type = ILS::MM;   break;
                            case 'O': ils.type = ILS::OM;   break;
                        }

                        if ( ils.type != ILS::NONE )
                        {
                            ils.lat = fdm::Units::deg2rad( fdm::String::toDouble( cols.at( 15 ) ) );
                            ils.lon = fdm::Units::deg2rad( fdm::String::toDouble( cols.at( 17 ) ) );

                            ils.elev = fdm::Units::ft2m( fdm::String::toDouble( cols.at( 10 ) ) );

                            ils.true_bearing = ils.type == ILS::LOC ? getTrueBearing( cols.at( 24 ).c_str(), cols.at( 22 ).c_str() ) : 0.0;
                            ils.glide_slope  = ils.type == ILS::GS  ? fdm::Units::deg2rad( fdm::String::toDouble( cols.at( 7 ) ) ) : 0.0;

                            ils.freq = fdm::String::toDouble( cols.at( 5 ) );

                            if ( ils.type == ILS::DME && cols.at( 6 ).length() > 0 )
                            {
                                ils.freq = Frequency::getFreqDME( cols.at( 6 ).c_str() );
                            }

                            fdm::Geo position_geo;

                            position_geo.lat = ils.lat;
                            position_geo.lon = ils.lon;
                            position_geo.alt = ils.elev;

                            ils.pos_wgs = fdm::WGS84::geo2wgs( position_geo );

                            _list_ils.push_back( ils );
                        }
                    }
                }
            }

            i_line++;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void DataBase::parseNAV( const char *path )
{
    std::fstream file( Path::get( path ).c_str(), std::ios_base::in );

    if ( file.is_open() )
    {
        std::string line;

        int i_line = 0;

        while ( !file.eof() )
        {
            getline( file, line );

            if ( i_line > 0 )
            {
                std::vector< std::string > cols = fdm::String::split( line, "\t" );

                line.clear();

                if ( cols.size() == 32 )
                {
                    NAV nav;

                    switch( fdm::String::toInt( cols.at( 1 ) ) )
                    {
                        default:           nav.type = NAV::NONE;    break;
                        case NAV::VOR:     nav.type = NAV::VOR;     break;
                        case NAV::VORTAC:  nav.type = NAV::VORTAC;  break;
                        case NAV::TACAN:   nav.type = NAV::TACAN;   break;
                        case NAV::VOR_DME: nav.type = NAV::VOR_DME; break;
                        case NAV::NDB:     nav.type = NAV::NDB;     break;
                        case NAV::NDB_DME: nav.type = NAV::NDB_DME; break;
                        case NAV::LOCATOR: nav.type = NAV::LOCATOR; break;
                        case NAV::DME:     nav.type = NAV::DME;     break;
                    }

                    if ( nav.type != NAV::NONE )
                    {
                        nav.lat = fdm::Units::deg2rad( fdm::String::toDouble( cols.at( 18 ) ) );
                        nav.lon = fdm::Units::deg2rad( fdm::String::toDouble( cols.at( 20 ) ) );

                        nav.elev = fdm::Units::ft2m( fdm::String::toDouble( cols.at( 23 ) ) );

                        nav.freq = fdm::String::toDouble( cols.at( 8 ) );

                        fdm::Geo position_geo;

                        position_geo.lat = nav.lat;
                        position_geo.lon = nav.lon;
                        position_geo.alt = nav.elev;

                        nav.pos_wgs = fdm::WGS84::geo2wgs( position_geo );

                        _list_nav.push_back( nav );
                    }
                }
            }

            i_line++;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void DataBase::parseRWY( const char *path )
{
    std::fstream file( Path::get( path ).c_str(), std::ios_base::in );

    if ( file.is_open() )
    {
        std::string line;

        int i_line = 0;

        while ( !file.eof() )
        {
            getline( file, line );

            if ( i_line > 0 )
            {
                std::vector< std::string > cols = fdm::String::split( line, "\t" );

                line.clear();

                if ( cols.size() == 51 )
                {
                    RWY rwy;

                    rwy.he_lat = fdm::Units::deg2rad( fdm::String::toDouble( cols.at( 10 ) ) );
                    rwy.he_lon = fdm::Units::deg2rad( fdm::String::toDouble( cols.at( 12 ) ) );
                    rwy.he_elev = fdm::Units::ft2m( fdm::String::toDouble( cols.at( 13 ) ) );

                    rwy.le_lat = fdm::Units::deg2rad( fdm::String::toDouble( cols.at( 27 ) ) );
                    rwy.le_lon = fdm::Units::deg2rad( fdm::String::toDouble( cols.at( 29 ) ) );
                    rwy.le_elev = fdm::Units::ft2m( fdm::String::toDouble( cols.at( 30 ) ) );

                    rwy.true_hdg = fdm::Units::deg2rad( fdm::String::toDouble( cols.at( 44 ) ) );

                    rwy.length = fdm::Units::ft2m( fdm::String::toDouble( cols.at( 5 ) ) );
                    rwy.width  = fdm::Units::ft2m( fdm::String::toDouble( cols.at( 6 ) ) );

                    _list_rwy.push_back( rwy );
                }
            }

            i_line++;
        }
    }
}
