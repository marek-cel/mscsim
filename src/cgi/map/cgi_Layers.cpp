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

#include <cgi/map/cgi_Layers.h>

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/LineWidth>
#include <osg/PositionAttitudeTransform>
#include <osg/Material>

#include <cgi/cgi_Geometry.h>
#include <cgi/cgi_GeoTIFF.h>
#include <cgi/cgi_Mercator.h>
#include <cgi/cgi_Models.h>
#include <cgi/cgi_Textures.h>

#include <cgi/map/cgi_Map.h>

#include <fdm/utils/fdm_String.h>
#include <fdm/xml/fdm_XmlDoc.h>

#include <sim/Log.h>
#include <sim/Path.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

Layers::Layers( const Module *parent ) :
    Module( parent )
{
    _switchInWaters  = new osg::Switch();
    _switchCrops     = new osg::Switch();
    _switchGrassland = new osg::Switch();
    _switchWoodland  = new osg::Switch();
    _switchBuiltup   = new osg::Switch();
    _switchSatellite = new osg::Switch();
    _switchAirports  = new osg::Switch();
    _switchRailroads = new osg::Switch();
    _switchRoads     = new osg::Switch();
    _switchBorders   = new osg::Switch();
    _switchCoastline = new osg::Switch();

    _oceans       = new osg::PositionAttitudeTransform();
    _landmass     = new osg::PositionAttitudeTransform();
    _coastline    = new osg::PositionAttitudeTransform();
    _crops        = new osg::PositionAttitudeTransform();
    _grassland    = new osg::PositionAttitudeTransform();
    _woodland     = new osg::PositionAttitudeTransform();
    _builtup      = new osg::PositionAttitudeTransform();
    _water_course = new osg::PositionAttitudeTransform();
    _water_inland = new osg::PositionAttitudeTransform();
    _satellite    = new osg::PositionAttitudeTransform();
    _airports     = new osg::PositionAttitudeTransform();
    _railroads    = new osg::PositionAttitudeTransform();
    _roads        = new osg::PositionAttitudeTransform();
    _borders      = new osg::PositionAttitudeTransform();

    _oceans       ->setPosition( osg::Vec3d( 0.0, 0.0, Map::_zOceans      ) );
    _landmass     ->setPosition( osg::Vec3d( 0.0, 0.0, Map::_zLandmass    ) );
    _crops        ->setPosition( osg::Vec3d( 0.0, 0.0, Map::_zCrops       ) );
    _grassland    ->setPosition( osg::Vec3d( 0.0, 0.0, Map::_zGrassland   ) );
    _woodland     ->setPosition( osg::Vec3d( 0.0, 0.0, Map::_zWoodland    ) );
    _builtup      ->setPosition( osg::Vec3d( 0.0, 0.0, Map::_zBuiltup     ) );
    _water_course ->setPosition( osg::Vec3d( 0.0, 0.0, Map::_zWaterCourse ) );
    _water_inland ->setPosition( osg::Vec3d( 0.0, 0.0, Map::_zWaterInland ) );
    _satellite    ->setPosition( osg::Vec3d( 0.0, 0.0, Map::_zSatellite   ) );
    _airports     ->setPosition( osg::Vec3d( 0.0, 0.0, Map::_zAirports    ) );
    _railroads    ->setPosition( osg::Vec3d( 0.0, 0.0, Map::_zRailroads   ) );
    _roads        ->setPosition( osg::Vec3d( 0.0, 0.0, Map::_zRoads       ) );
    _borders      ->setPosition( osg::Vec3d( 0.0, 0.0, Map::_zBorders     ) );
    _coastline    ->setPosition( osg::Vec3d( 0.0, 0.0, Map::_zCoastline   ) );

    _switchCrops     ->addChild( _crops        .get() );
    _switchGrassland ->addChild( _grassland    .get() );
    _switchWoodland  ->addChild( _woodland     .get() );
    _switchBuiltup   ->addChild( _builtup      .get() );
    _switchInWaters  ->addChild( _water_course .get() );
    _switchInWaters  ->addChild( _water_inland .get() );
    _switchSatellite ->addChild( _satellite    .get() );
    _switchAirports  ->addChild( _airports     .get() );
    _switchRailroads ->addChild( _railroads    .get() );
    _switchRoads     ->addChild( _roads        .get() );
    _switchBorders   ->addChild( _borders      .get() );
    _switchCoastline ->addChild( _coastline    .get() );

    _root->addChild( _oceans    .get() );
    _root->addChild( _landmass  .get() );

    _root->addChild( _switchCrops     .get() );
    _root->addChild( _switchGrassland .get() );
    _root->addChild( _switchWoodland  .get() );
    _root->addChild( _switchBuiltup   .get() );
    _root->addChild( _switchInWaters  .get() );
    _root->addChild( _switchSatellite .get() );
    _root->addChild( _switchAirports  .get() );
    _root->addChild( _switchRailroads .get() );
    _root->addChild( _switchRoads     .get() );
    _root->addChild( _switchBorders   .get() );
    _root->addChild( _switchCoastline .get() );

    initLayer( _oceans       .get() , Map::_colorOceans      );
    initLayer( _landmass     .get() , Map::_colorLandmass    );
    initLayer( _crops        .get() , Map::_colorCrops       );
    initLayer( _grassland    .get() , Map::_colorGrassland   );
    initLayer( _woodland     .get() , Map::_colorWoodland    );
    initLayer( _builtup      .get() , Map::_colorBuiltup     );
    initLayer( _water_course .get() , Map::_colorWaterCourse );
    initLayer( _water_inland .get() , Map::_colorWaterInland );
    initLayer( _airports     .get() , Map::_colorAirports    );
    initLayer( _railroads    .get() , Map::_colorRailroads   );
    initLayer( _roads        .get() , Map::_colorRoads       );
    initLayer( _borders      .get() , Map::_colorBorders     );
    //initLayer( _coastline    .get() , Map::_colorCoastline   );
    initLayer( _coastline    .get() , Map::_colorOceans      ); // !!

    createOcean();
    readLayers();
}

////////////////////////////////////////////////////////////////////////////////

Layers::~Layers() {}

////////////////////////////////////////////////////////////////////////////////

void Layers::setVisibilityCrops( bool visible )
{
    if ( visible )
        _switchCrops->setAllChildrenOn();
    else
        _switchCrops->setAllChildrenOff();
}

////////////////////////////////////////////////////////////////////////////////

void Layers::setVisibilityGrassland( bool visible )
{
    if ( visible )
        _switchGrassland->setAllChildrenOn();
    else
        _switchGrassland->setAllChildrenOff();
}

////////////////////////////////////////////////////////////////////////////////

void Layers::setVisibilityWoodland( bool visible )
{
    if ( visible )
        _switchWoodland->setAllChildrenOn();
    else
        _switchWoodland->setAllChildrenOff();
}

////////////////////////////////////////////////////////////////////////////////

void Layers::setVisibilityBuiltup( bool visible )
{
    if ( visible )
        _switchBuiltup->setAllChildrenOn();
    else
        _switchBuiltup->setAllChildrenOff();
}

////////////////////////////////////////////////////////////////////////////////

void Layers::setVisibilityInWaters( bool visible )
{
    if ( visible )
        _switchInWaters->setAllChildrenOn();
    else
        _switchInWaters->setAllChildrenOff();
}

////////////////////////////////////////////////////////////////////////////////

void Layers::setVisibilitySatellite( bool visible )
{
    if ( visible )
    {
        _switchSatellite->setAllChildrenOn();
        _switchCoastline->setAllChildrenOn();
    }
    else
    {
        _switchSatellite->setAllChildrenOff();
        _switchCoastline->setAllChildrenOff();
    }
}

////////////////////////////////////////////////////////////////////////////////

void Layers::setVisibilityAirports( bool visible )
{
    if ( visible )
        _switchAirports->setAllChildrenOn();
    else
        _switchAirports->setAllChildrenOff();
}

////////////////////////////////////////////////////////////////////////////////

void Layers::setVisibilityRailroads( bool visible )
{
    if ( visible )
        _switchRailroads->setAllChildrenOn();
    else
        _switchRailroads->setAllChildrenOff();
}

////////////////////////////////////////////////////////////////////////////////

void Layers::setVisibilityRoads( bool visible )
{
    if ( visible )
        _switchRoads->setAllChildrenOn();
    else
        _switchRoads->setAllChildrenOff();
}

////////////////////////////////////////////////////////////////////////////////

void Layers::setVisibilityBorders( bool visible )
{
    if ( visible )
        _switchBorders->setAllChildrenOn();
    else
        _switchBorders->setAllChildrenOff();
}

////////////////////////////////////////////////////////////////////////////////

void Layers::createOcean()
{
    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    _oceans->addChild( geode.get() );

    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
    geode->addDrawable( geometry.get() );

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();

    v->push_back( osg::Vec3( -Mercator::_max_x, -Mercator::_max_y, 0.0 ) );
    v->push_back( osg::Vec3(  Mercator::_max_x, -Mercator::_max_y, 0.0 ) );
    v->push_back( osg::Vec3(  Mercator::_max_x,  Mercator::_max_y, 0.0 ) );
    v->push_back( osg::Vec3( -Mercator::_max_x,  Mercator::_max_y, 0.0 ) );

    Geometry::createQuad( geometry.get(), v.get() );
}

////////////////////////////////////////////////////////////////////////////////

void Layers::initLayer( osg::Node* layer, osg::Vec3 color, float width )
{
    osg::ref_ptr<osg::Material> material = new osg::Material();
    material->setColorMode( osg::Material::AMBIENT_AND_DIFFUSE );
    material->setAmbient( osg::Material::FRONT, osg::Vec4( color, 1.0f ) );
    material->setDiffuse( osg::Material::FRONT, osg::Vec4( color, 1.0f ) );
    layer->getOrCreateStateSet()->setAttribute( material.get(),
            osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON );

    osg::ref_ptr<osg::LineWidth> lineWidth = new osg::LineWidth();
    lineWidth->setWidth( width );
    layer->getOrCreateStateSet()->setAttributeAndModes( lineWidth.get(),
        osg::StateAttribute::ON );
}

////////////////////////////////////////////////////////////////////////////////

void Layers::readLayers()
{
    fdm::XmlDoc doc( Path::get( "map/layers/layers.xml" ).c_str() );

    if ( doc.isOpen() )
    {
        fdm::XmlNode rootNode = doc.getRootNode();

        if ( rootNode.isValid() )
        {
            if ( 0 == fdm::String::icompare( rootNode.getName(), "layers" ) )
            {
                fdm::XmlNode landmassNode    = rootNode.getFirstChildElement( "landmass"     );
                fdm::XmlNode coastlineNode   = rootNode.getFirstChildElement( "coastline"    );
                fdm::XmlNode cropsNode       = rootNode.getFirstChildElement( "crops"        );
                fdm::XmlNode grasslandNode   = rootNode.getFirstChildElement( "grassland"    );
                fdm::XmlNode woodlandNode    = rootNode.getFirstChildElement( "woodland"     );
                fdm::XmlNode builtupNode     = rootNode.getFirstChildElement( "builtup"      );
                fdm::XmlNode railroadsNode   = rootNode.getFirstChildElement( "railroads"    );
                fdm::XmlNode roadsNode       = rootNode.getFirstChildElement( "roads"        );
                fdm::XmlNode airportsNode    = rootNode.getFirstChildElement( "airports"     );
                fdm::XmlNode waterCourseNode = rootNode.getFirstChildElement( "water_course" );
                fdm::XmlNode waterInlandNode = rootNode.getFirstChildElement( "water_inland" );
                fdm::XmlNode bordersNode     = rootNode.getFirstChildElement( "borders"      );

                if ( landmassNode    .isValid() ) readLayer( landmassNode    , _landmass     .get() );
                if ( coastlineNode   .isValid() ) readLayer( coastlineNode   , _coastline    .get() );
                if ( cropsNode       .isValid() ) readLayer( cropsNode       , _crops        .get() );
                if ( grasslandNode   .isValid() ) readLayer( grasslandNode   , _grassland    .get() );
                if ( woodlandNode    .isValid() ) readLayer( woodlandNode    , _woodland     .get() );
                if ( builtupNode     .isValid() ) readLayer( builtupNode     , _builtup      .get() );
                if ( railroadsNode   .isValid() ) readLayer( railroadsNode   , _railroads    .get() );
                if ( roadsNode       .isValid() ) readLayer( roadsNode       , _roads        .get() );
                if ( airportsNode    .isValid() ) readLayer( airportsNode    , _airports     .get() );
                if ( waterCourseNode .isValid() ) readLayer( waterCourseNode , _water_course .get() );
                if ( waterInlandNode .isValid() ) readLayer( waterInlandNode , _water_inland .get() );
                if ( bordersNode     .isValid() ) readLayer( bordersNode     , _borders      .get() );

                fdm::XmlNode satelliteNode = rootNode.getFirstChildElement( "satellite" );
                if ( satelliteNode.isValid() ) readLayerSatellite( satelliteNode, _satellite.get() );
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

            osg::ref_ptr<osg::Node> layerNode = Models::get( file.c_str(), true );

            if ( layerNode.valid() )
            {
                parent->addChild( layerNode.get() );
            }
        }

        fileNode = fileNode.getNextSiblingElement( "file" );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Layers::readLayerSatellite( const fdm::XmlNode &node, osg::Group *parent )
{
    fdm::XmlNode fileNode = node.getFirstChildElement( "file" );

    while ( fileNode.isValid() )
    {
        fdm::XmlNode fileTextNode = fileNode.getFirstChild();

        if ( fileTextNode.isValid() && fileTextNode.isText() )
        {
            readSatelliteImage( parent, fileTextNode.getText().c_str() );
        }

        fileNode = fileNode.getNextSiblingElement( "file" );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Layers::readSatelliteImage( osg::Group *parent, const char *file )
{
    // images should be in World Mercator WGS-84 (EPSG:3395) projection
    char proj4_merc[] = { "+proj=merc +lon_0=0 +k=1 +x_0=0 +y_0=0 +datum=WGS84 +units=m +no_defs" };

    GeoTIFF geoTiff( Path::get( file ).c_str() );

    if ( geoTiff.isOpen() )
    {
        if ( geoTiff.compareSRS_PROJ4( proj4_merc ) )
        {
            double x_ul = geoTiff.getMapUpperLeftX();
            double y_ul = geoTiff.getMapUpperLeftY();

            double w = geoTiff.getMapW();
            double h = geoTiff.getMapH();

            osg::ref_ptr<osg::PositionAttitudeTransform> pat = new osg::PositionAttitudeTransform();
            parent->addChild( pat.get() );
            pat->setPosition( osg::Vec3d( x_ul, y_ul, 0.0 ) );

            osg::ref_ptr<osg::Geode> geode = new osg::Geode();
            pat->addChild( geode.get() );

            osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
            geode->addDrawable( geometry.get() );

            osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();

            v->push_back( osg::Vec3( 0.0 ,   h , 0.0 ) );
            v->push_back( osg::Vec3(   w ,   h , 0.0 ) );
            v->push_back( osg::Vec3(   w , 0.0 , 0.0 ) );
            v->push_back( osg::Vec3( 0.0 , 0.0 , 0.0 ) );

            Geometry::createQuad( geometry.get(), v.get(), true );

            osg::ref_ptr<osg::Texture2D> texture = Textures::get( file );

            osg::ref_ptr<osg::StateSet> stateSet = geode->getOrCreateStateSet();

            if ( texture.valid() )
            {
                stateSet->setMode( GL_BLEND, osg::StateAttribute::ON );
                stateSet->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
                stateSet->setTextureAttributeAndModes( 0, texture, osg::StateAttribute::ON );
            }

            // material
            osg::ref_ptr<osg::Material> material = new osg::Material();

            material->setColorMode( osg::Material::AMBIENT_AND_DIFFUSE );
            material->setAmbient( osg::Material::FRONT, osg::Vec4( 1.0f, 1.0f, 1.0f, 1.0f ) );
            material->setDiffuse( osg::Material::FRONT, osg::Vec4( 1.0f, 1.0f, 1.0f, 1.0f ) );

            stateSet->setAttribute( material.get() );
        }
        else
        {
            Log::e() << "File: " << file << " projection is not EPSG:3395" << std::endl;
        }
    }
    else
    {
        Log::e() << "Cannot open dataset: " << file << std::endl;
    }
}
