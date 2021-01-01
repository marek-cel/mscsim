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

#include <gui/Airports.h>

#include <QFile>

#include <fdm/utils/fdm_Units.h>

#include <gui/gui_Defines.h>

#include <sim/Path.h>

////////////////////////////////////////////////////////////////////////////////

Airports::Airports()
{
    _default.name   = "";
    _default.lat    = 0.0;
    _default.lon    = 0.0;
    _default.alt    = 0.0;
    _default.hdg    = 0.0;
    _default.elev   = 0.0;
    _default.slope  = 0.0;
    _default.runway = false;

    QFile file( Path::get( "gui/airports.xml" ).c_str() );

    if ( file.open(QFile::ReadOnly | QFile::Text) )
    {
        QDomDocument doc;

        doc.setContent( &file, false );

        QDomElement rootNode = doc.documentElement();

        if ( rootNode.tagName() == "airports" )
        {
            QDomElement airportNode = rootNode.firstChildElement( "airport" );

            while ( !airportNode.isNull() )
            {
                parseAirport( airportNode );
                airportNode = airportNode.nextSiblingElement( "airport" );
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

Airports::~Airports() {}

////////////////////////////////////////////////////////////////////////////////

void Airports::parseAirport( QDomElement &node )
{
    QDomElement nameNode = node.firstChildElement( "name" );

    if ( !nameNode.isNull() )
    {
        Airport airport;

        airport.name = nameNode.text();

        QDomElement locationNode = node.firstChildElement( "location" );

        while ( !locationNode.isNull() )
        {
            parseLocation( locationNode, airport );
            locationNode = locationNode.nextSiblingElement( "location" );
        }

        _airports.push_back( airport );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Airports::parseLocation( QDomElement &node, Airport &airport )
{
    QDomElement nameNode = node.firstChildElement( "name" );

    QDomElement latNode = node.firstChildElement( "lat" );
    QDomElement lonNode = node.firstChildElement( "lon" );
    QDomElement altNode = node.firstChildElement( "alt" );
    QDomElement hdgNode = node.firstChildElement( "hdg" );

    if ( !nameNode.isNull()
      && !latNode.isNull()
      && !lonNode.isNull()
      && !altNode.isNull()
      && !hdgNode.isNull() )
    {
        Location location;

        location.name = nameNode.text();

        location.lat = fdm::Units::deg2rad( latNode.text().toDouble() );
        location.lon = fdm::Units::deg2rad( lonNode.text().toDouble() );
        location.alt = altNode.text().toDouble();
        location.hdg = fdm::Units::deg2rad( hdgNode.text().toDouble() );

        location.elev  = node.attributeNode( "elevation" ).value().toDouble();
        location.slope = fdm::Units::deg2rad( node.attributeNode( "slope" ).value().toDouble() );

        location.runway = node.attributeNode( "runway" ).value().toInt() != 0;

        if (  node.attributeNode( "default" ).value().toInt() != 0 )
        {
            _default = location;
        }

        airport.locations.push_back( location );
    }
}
