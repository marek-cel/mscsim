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

#include <cgi/otw/cgi_Scenery.h>

#include <fdm/fdm_Path.h>
#include <fdm/utils/fdm_String.h>
#include <fdm/xml/fdm_XmlDoc.h>

#include <cgi/cgi_Intersections.h>

#include <cgi/otw/cgi_Airport.h>
#include <cgi/otw/cgi_Terrain.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

Scenery::Scenery( Module *parent ) :
    Module( parent )
{
    m_root->setName( "Scenery" );

    fdm::XmlDoc doc( fdm::Path::get( "data/cgi/scenery/scenery.xml" ) );

    if ( doc.isOpen() )
    {
        fdm::XmlNode rootNode = doc.getRootNode();

        if ( rootNode.isValid() )
        {
            if ( 0 == fdm::String::icompare( rootNode.getName(), "scenery" ) )
            {
                readTerrain( rootNode );
                readAirports( rootNode );
            }
        }
    }

    Intersections::instance()->setScenery( m_root.get() );
}

////////////////////////////////////////////////////////////////////////////////

Scenery::~Scenery() {}

////////////////////////////////////////////////////////////////////////////////

void Scenery::readAirports( const fdm::XmlNode &node )
{
    fdm::XmlNode airportsNode = node.getFirstChildElement( "airports" );

    if ( airportsNode.isValid() )
    {
        fdm::XmlNode airportNode = airportsNode.getFirstChildElement( "airport" );

        while ( airportNode.isValid() )
        {
            fdm::XmlNode nameNode = airportNode.getFirstChildElement( "name" );
            fdm::XmlNode icaoNode = airportNode.getFirstChildElement( "icao" );
            fdm::XmlNode fileNode = airportNode.getFirstChildElement( "file" );

            fdm::XmlNode latNode = airportNode.getFirstChildElement( "lat" );
            fdm::XmlNode lonNode = airportNode.getFirstChildElement( "lon" );
            fdm::XmlNode altNode = airportNode.getFirstChildElement( "alt" );

            if ( nameNode.isValid() && icaoNode.isValid() && fileNode.isValid()
              && latNode.isValid() && lonNode.isValid() && altNode.isValid() )
            {
                fdm::XmlNode nameTextNode = nameNode.getFirstChild();
                fdm::XmlNode icaoTextNode = icaoNode.getFirstChild();
                fdm::XmlNode fileTextNode = fileNode.getFirstChild();

                fdm::XmlNode latTextNode = latNode.getFirstChild();
                fdm::XmlNode lonTextNode = lonNode.getFirstChild();
                fdm::XmlNode altTextNode = altNode.getFirstChild();

                if ( nameTextNode.isValid() && nameTextNode.isText()
                  && icaoTextNode.isValid() && icaoTextNode.isText()
                  && fileTextNode.isValid() && fileTextNode.isText()
                  && latTextNode.isValid() && latTextNode.isText()
                  && lonTextNode.isValid() && lonTextNode.isText()
                  && altTextNode.isValid() && altTextNode.isText() )
                {
                    //std::string name = nameTextNode.getText();
                    //std::string icao = icaoTextNode.getText();
                    std::string file = fileTextNode.getText();

                    double lat = osg::DegreesToRadians( fdm::String::toDouble( latTextNode.getText() ) );
                    double lon = osg::DegreesToRadians( fdm::String::toDouble( lonTextNode.getText() ) );
                    double alt = fdm::String::toDouble( altTextNode.getText() );

                    addChild( new Airport( file, lat, lon, alt, this ) );
                }
            }

            airportNode = airportNode.getNextSiblingElement( "airport" );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Scenery::readTerrain( const fdm::XmlNode &node )
{
    fdm::XmlNode terrainNode = node.getFirstChildElement( "terrain" );

    if ( terrainNode.isValid() )
    {
        fdm::XmlNode terrainTextNode = terrainNode.getFirstChild();

        if ( terrainTextNode.isValid() && terrainTextNode.isText() )
        {
            std::string file = terrainTextNode.getText();

            addChild( new Terrain( file, this ) );
        }
    }
}
