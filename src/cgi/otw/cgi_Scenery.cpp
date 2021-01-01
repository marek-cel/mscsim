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

#include <cgi/otw/cgi_Scenery.h>

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Material>
#include <osg/TexEnv>

#include <Data.h>

#include <fdm/utils/fdm_String.h>
#include <fdm/xml/fdm_XmlDoc.h>

#include <cgi/cgi_Geometry.h>
#include <cgi/cgi_Intersections.h>
#include <cgi/cgi_Textures.h>
#include <cgi/cgi_WGS84.h>

#include <cgi/otw/cgi_Airport.h>
#include <cgi/otw/cgi_Entities.h>
#include <cgi/otw/cgi_Landmark.h>
#include <cgi/otw/cgi_Terrain.h>

#include <sim/Path.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

Scenery::Scenery( const Module *parent ) :
    Module( parent )
{
    _root->setName( "Scenery" );

    _patMasterPos = new osg::PositionAttitudeTransform();
    _patMasterAtt = new osg::PositionAttitudeTransform();

    _patMasterPos->addChild( _patMasterAtt.get() );

    _root->addChild( _patMasterPos.get() );

    _on = new osgSim::OverlayNode( osgSim::OverlayNode::OBJECT_DEPENDENT_WITH_ORTHOGRAPHIC_OVERLAY );
    _on->setContinuousUpdate( true );
    _on->setOverlayTextureSizeHint( 1024 );
    _on->getOrCreateStateSet()->setTextureAttribute( 1, new osg::TexEnv( osg::TexEnv::DECAL ) );
    _on->getOrCreateStateSet()->setMode( GL_BLEND, osg::StateAttribute::ON );
    _on->getOrCreateStateSet()->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
    _on->getOrCreateStateSet()->setRenderBinDetails( CGI_DEPTH_SORTED_BIN_WORLD, "RenderBin" );
    _patMasterAtt->addChild( _on.get() );

    //_groupScenery = new osg::Group();
    //_on->addChild( _groupScenery.get() );

    _patSceneryPos = new osg::PositionAttitudeTransform();
    _patSceneryAtt = new osg::PositionAttitudeTransform();

    _patSceneryAtt->addChild( _patSceneryPos.get() );

    _on->addChild( _patSceneryAtt.get() );

    _patShadow = new osg::PositionAttitudeTransform();
    _on->setOverlaySubgraph( _patShadow.get() );

    createShadow();

    fdm::XmlDoc doc( Path::get( "cgi/scenery/scenery.xml" ).c_str() );

    if ( doc.isOpen() )
    {
        fdm::XmlNode rootNode = doc.getRootNode();

        if ( rootNode.isValid() )
        {
            if ( 0 == fdm::String::icompare( rootNode.getName(), "scenery" ) )
            {
                readAirports  ( rootNode );
                readTerrain   ( rootNode );
                readLandmarks ( rootNode ); // landmarks after terrain!
            }
        }
    }

    addChild( new Entities( this ) ); // entities after terrain!

    Intersections::instance()->setScenery( _root.get() );
}

////////////////////////////////////////////////////////////////////////////////

Scenery::~Scenery() {}

////////////////////////////////////////////////////////////////////////////////

void Scenery::addChild( Module *child )
{
    if ( child )
    {
        _children.push_back( child );
        _patSceneryPos->addChild( child->getNode() );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Scenery::update()
{
    /////////////////
    Module::update();
    /////////////////

    osg::Vec3d v( Data::get()->ownship.pos_x_wgs,
                  Data::get()->ownship.pos_y_wgs,
                  Data::get()->ownship.pos_z_wgs );

    WGS84 wgs( v );

    _patSceneryAtt->setAttitude(  wgs.getAttitude().inverse() );
    _patSceneryPos->setPosition( -wgs.getPosition() );

    _patMasterAtt->setAttitude(  wgs.getAttitude() );
    _patMasterPos->setPosition(  wgs.getPosition() );

    osg::Quat att( Data::get()->ownship.roll    , osg::X_AXIS,
                   Data::get()->ownship.pitch   , osg::Y_AXIS,
                   Data::get()->ownship.heading , osg::Z_AXIS );

    _patShadow->setAttitude( att );
}

////////////////////////////////////////////////////////////////////////////////

void Scenery::setShadow( const char *shadowFile )
{
    osg::ref_ptr<osg::Texture2D> texture = Textures::get( shadowFile );

    if ( !texture.valid() )
    {
        texture = Textures::get( "cgi/textures/shadow.png" );
    }

    _geodeShadow->getOrCreateStateSet()->setTextureAttributeAndModes( 0, texture.get(), osg::StateAttribute::ON );
}


////////////////////////////////////////////////////////////////////////////////

void Scenery::createShadow()
{
    const double w_2 = 50.0;
    const double l_2 = 50.0;

    _geodeShadow = new osg::Geode();
    _patShadow->addChild( _geodeShadow.get() );

    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();
    _geodeShadow->addDrawable( geom.get() );

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();

    v->push_back( osg::Vec3d( -w_2, -l_2, 0.0 ) );
    v->push_back( osg::Vec3d( -w_2,  l_2, 0.0 ) );
    v->push_back( osg::Vec3d(  w_2,  l_2, 0.0 ) );
    v->push_back( osg::Vec3d(  w_2, -l_2, 0.0 ) );

    Geometry::createQuad( geom.get(), v.get(), true );

    // material
    osg::ref_ptr<osg::Material> material = new osg::Material();
    material->setColorMode( osg::Material::AMBIENT_AND_DIFFUSE );
    material->setAmbient( osg::Material::FRONT, osg::Vec4f( 0.8f, 0.8f, 0.8f, 1.0f ) );
    material->setDiffuse( osg::Material::FRONT, osg::Vec4f( 0.5f, 0.5f, 0.5f, 1.0f ) );

    _geodeShadow->getOrCreateStateSet()->setAttribute( material.get() );
}

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

                    addChild( new Airport( file.c_str(), lat, lon, alt, this ) );
                }
            }

            airportNode = airportNode.getNextSiblingElement( "airport" );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Scenery::readLandmarks( const fdm::XmlNode &node )
{
    fdm::XmlNode landmarksNode = node.getFirstChildElement( "landmarks" );

    if ( landmarksNode.isValid() )
    {
        fdm::XmlNode landmarkNode = landmarksNode.getFirstChildElement( "landmark" );

        while ( landmarkNode.isValid() )
        {
            fdm::XmlNode fileNode = landmarkNode.getFirstChildElement( "file" );

            fdm::XmlNode latNode = landmarkNode.getFirstChildElement( "lat" );
            fdm::XmlNode lonNode = landmarkNode.getFirstChildElement( "lon" );
            fdm::XmlNode altNode = landmarkNode.getFirstChildElement( "alt" );
            fdm::XmlNode hdgNode = landmarkNode.getFirstChildElement( "hdg" );

            if ( fileNode.isValid()
              && latNode.isValid() && lonNode.isValid()
              && altNode.isValid() && hdgNode.isValid() )
            {
                fdm::XmlNode fileTextNode = fileNode.getFirstChild();

                fdm::XmlNode latTextNode = latNode.getFirstChild();
                fdm::XmlNode lonTextNode = lonNode.getFirstChild();
                fdm::XmlNode altTextNode = altNode.getFirstChild();
                fdm::XmlNode hdgTextNode = hdgNode.getFirstChild();

                if ( fileTextNode.isValid() && fileTextNode.isText()
                  && latTextNode.isValid() && latTextNode.isText()
                  && lonTextNode.isValid() && lonTextNode.isText()
                  && altTextNode.isValid() && altTextNode.isText()
                  && hdgTextNode.isValid() && hdgTextNode.isText() )
                {
                    std::string file = fileTextNode.getText();

                    double lat = osg::DegreesToRadians( fdm::String::toDouble( latTextNode.getText() ) );
                    double lon = osg::DegreesToRadians( fdm::String::toDouble( lonTextNode.getText() ) );
                    double alt = fdm::String::toDouble( altTextNode.getText() );
                    double hdg = osg::DegreesToRadians( fdm::String::toDouble( hdgTextNode.getText() ) );

                    bool reflection = fdm::String::toBool( landmarkNode.getAttribute( "reflection" ), false );

                    addChild( new Landmark( file.c_str(), lat, lon, alt, hdg, reflection, this ) );
                }
            }

            landmarkNode = landmarkNode.getNextSiblingElement( "landmark" );
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

            addChild( new Terrain( file.c_str(), this ) );
        }
    }
}
