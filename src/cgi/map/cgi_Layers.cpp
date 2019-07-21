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

#include <cgi/map/cgi_Layers.h>

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/LineWidth>
#include <osg/PositionAttitudeTransform>
#include <osg/Material>

#include <cgi/cgi_Geometry.h>
#include <cgi/cgi_Mercator.h>
#include <cgi/cgi_Models.h>

#include <cgi/map/cgi_Map.h>

#include <fdm/fdm_Path.h>
#include <fdm/utils/fdm_String.h>
#include <fdm/xml/fdm_XmlDoc.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

Layers::Layers( Module *parent ) :
    Module( parent )
{
    m_oceans    = new osg::PositionAttitudeTransform();
    m_landmass  = new osg::PositionAttitudeTransform();
    m_woodland  = new osg::PositionAttitudeTransform();
    m_builtup   = new osg::PositionAttitudeTransform();
    m_railroads = new osg::PositionAttitudeTransform();
    m_roads     = new osg::PositionAttitudeTransform();
    m_airports  = new osg::PositionAttitudeTransform();
    m_water     = new osg::PositionAttitudeTransform();

    m_oceans    ->setPosition( osg::Vec3d( 0.0, 0.0, Map::zOceans    ) );
    m_landmass  ->setPosition( osg::Vec3d( 0.0, 0.0, Map::zLandmass  ) );
    m_woodland  ->setPosition( osg::Vec3d( 0.0, 0.0, Map::zWoodland  ) );
    m_builtup   ->setPosition( osg::Vec3d( 0.0, 0.0, Map::zBuiltup   ) );
    m_railroads ->setPosition( osg::Vec3d( 0.0, 0.0, Map::zRailroads ) );
    m_roads     ->setPosition( osg::Vec3d( 0.0, 0.0, Map::zRoads     ) );
    m_airports  ->setPosition( osg::Vec3d( 0.0, 0.0, Map::zAirports  ) );
    m_water     ->setPosition( osg::Vec3d( 0.0, 0.0, Map::zWater     ) );

    m_root->addChild( m_oceans    .get() );
    m_root->addChild( m_landmass  .get() );
    m_root->addChild( m_woodland  .get() );
    m_root->addChild( m_builtup   .get() );
    m_root->addChild( m_railroads .get() );
    m_root->addChild( m_roads     .get() );
    m_root->addChild( m_airports  .get() );
    m_root->addChild( m_water     .get() );

    initLayer( m_oceans    .get() , Map::colorOceans    );
    initLayer( m_landmass  .get() , Map::colorLandmass  );
    initLayer( m_woodland  .get() , Map::colorWoodland  );
    initLayer( m_builtup   .get() , Map::colorBuiltup   );
    initLayer( m_railroads .get() , Map::colorRailroads );
    initLayer( m_roads     .get() , Map::colorRoads     );
    initLayer( m_airports  .get() , Map::colorAirports  );
    initLayer( m_water     .get() , Map::colorWater     );

    createOcean();
    readLayers();
}

////////////////////////////////////////////////////////////////////////////////

Layers::~Layers() {}

////////////////////////////////////////////////////////////////////////////////

void Layers::createOcean()
{
    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    m_oceans->addChild( geode.get() );

    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
    geode->addDrawable( geometry.get() );

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();

    v->push_back( osg::Vec3( -Mercator::maxX, -Mercator::maxY, 0.0 ) );
    v->push_back( osg::Vec3(  Mercator::maxX, -Mercator::maxY, 0.0 ) );
    v->push_back( osg::Vec3(  Mercator::maxX,  Mercator::maxY, 0.0 ) );
    v->push_back( osg::Vec3( -Mercator::maxX,  Mercator::maxY, 0.0 ) );

    Geometry::createQuad( geometry.get(), v.get() );
}

////////////////////////////////////////////////////////////////////////////////

void Layers::initLayer( osg::Node* layer, osg::Vec3 color )
{
    osg::ref_ptr<osg::Material> material = new osg::Material();
    material->setColorMode( osg::Material::AMBIENT_AND_DIFFUSE );
    material->setAmbient( osg::Material::FRONT, osg::Vec4( color, 1.0f ) );
    material->setDiffuse( osg::Material::FRONT, osg::Vec4( color, 1.0f ) );
    layer->getOrCreateStateSet()->setAttribute( material.get(),
            osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON );

//    osg::ref_ptr<osg::LineWidth> lineWidth = new osg::LineWidth();
//    lineWidth->setWidth( 2.0f );
//    layer->getOrCreateStateSet()->setAttributeAndModes( lineWidth.get(),
//        osg::StateAttribute::ON );
}

////////////////////////////////////////////////////////////////////////////////

void Layers::readLayers()
{
    fdm::XmlDoc doc( fdm::Path::get( "data/map/layers/layers.xml" ) );

    if ( doc.isOpen() )
    {
        fdm::XmlNode rootNode = doc.getRootNode();

        if ( rootNode.isValid() )
        {
            if ( 0 == fdm::String::icompare( rootNode.getName(), "layers" ) )
            {
                fdm::XmlNode landmassNode  = rootNode.getFirstChildElement( "landmass"  );
                fdm::XmlNode woodlandNode  = rootNode.getFirstChildElement( "woodland"  );
                fdm::XmlNode builtupNode   = rootNode.getFirstChildElement( "builtup"   );
                fdm::XmlNode railroadsNode = rootNode.getFirstChildElement( "railroads" );
                fdm::XmlNode roadsNode     = rootNode.getFirstChildElement( "roads"     );
                fdm::XmlNode airportsNode  = rootNode.getFirstChildElement( "airports"  );
                fdm::XmlNode waterNode     = rootNode.getFirstChildElement( "water"     );

                if ( landmassNode  .isValid() ) readLayer( landmassNode  , m_landmass  .get() );
                if ( woodlandNode  .isValid() ) readLayer( woodlandNode  , m_woodland  .get() );
                if ( builtupNode   .isValid() ) readLayer( builtupNode   , m_builtup   .get() );
                if ( railroadsNode .isValid() ) readLayer( railroadsNode , m_railroads .get() );
                if ( roadsNode     .isValid() ) readLayer( roadsNode     , m_roads     .get() );
                if ( airportsNode  .isValid() ) readLayer( airportsNode  , m_airports  .get() );
                if ( waterNode     .isValid() ) readLayer( waterNode     , m_water     .get() );
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Layers::readLayer( const fdm::XmlNode &node, osg::Group *parent )
{
    fdm::XmlNode fileNode = node.getFirstChildElement( "file" );

    while ( fileNode.isValid() )
    {
        fdm::XmlNode fileTextNode = fileNode.getFirstChild();

        if ( fileTextNode.isValid() && fileTextNode.isText() )
        {
            std::string file = fileTextNode.getText();

            osg::ref_ptr<osg::Node> layerNode = Models::get( file );

            if ( layerNode.valid() )
            {
                parent->addChild( layerNode.get() );
            }
        }

        fileNode = fileNode.getNextSiblingElement( "file" );
    }
}
