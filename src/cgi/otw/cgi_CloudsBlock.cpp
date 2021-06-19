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

#include <cgi/otw/cgi_CloudsBlock.h>

#ifdef _MSC_VER
#   include <algorithm>
#endif

#include <osg/AlphaFunc>
#include <osg/Billboard>
#include <osg/BlendFunc>
#include <osg/Depth>
#include <osg/Geometry>
#include <osg/Material>

#include <Data.h>

#include <cgi/cgi_Defines.h>
#include <cgi/cgi_Geometry.h>
#include <cgi/cgi_WGS84.h>

#include <fdm/utils/fdm_Random.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

CloudsBlock::CloudsBlock( const Module *parent ) :
    Module( parent ),

    _count ( 0 ),
    _base_asl ( 0.0f ),
    _thickness ( 0.0f ),

    _framesCounter ( 0 ),
    _created ( false )
{
    osg::ref_ptr<osg::StateSet> stateSet = _root->getOrCreateStateSet();

    osg::ref_ptr<osg::AlphaFunc> alphaFunc = new osg::AlphaFunc();
    osg::ref_ptr<osg::BlendFunc> blendFunc = new osg::BlendFunc();
    alphaFunc->setFunction( osg::AlphaFunc::GEQUAL, 0.05f );
    blendFunc->setFunction( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    osg::ref_ptr<osg::Depth> depth = new osg::Depth;
    depth->setWriteMask( false );
    stateSet->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
    stateSet->setAttributeAndModes( blendFunc.get(), osg::StateAttribute::ON );
    stateSet->setAttributeAndModes( alphaFunc.get(), osg::StateAttribute::ON );
    stateSet->setAttributeAndModes( depth.get(), osg::StateAttribute::ON );
    stateSet->setMode( GL_BLEND     , osg::StateAttribute::ON  | osg::StateAttribute::OVERRIDE );
    stateSet->setMode( GL_CULL_FACE , osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE  );
    stateSet->setRenderBinDetails( CGI_DEPTH_SORTED_BIN_CLOUDS, "DepthSortedBin" );

    _textures.push_back( Textures::get( "cgi/textures/cloud_cu_1.png" ) );
    _textures.push_back( Textures::get( "cgi/textures/cloud_cu_2.png" ) );
    _textures.push_back( Textures::get( "cgi/textures/cloud_cu_3.png" ) );
    _textures.push_back( Textures::get( "cgi/textures/cloud_cu_4.png" ) );
    _textures.push_back( Textures::get( "cgi/textures/cloud_cu_5.png" ) );
    _textures.push_back( Textures::get( "cgi/textures/cloud_cu_6.png" ) );
}

////////////////////////////////////////////////////////////////////////////////

CloudsBlock::~CloudsBlock() {}

////////////////////////////////////////////////////////////////////////////////

void CloudsBlock::update()
{
    /////////////////
    Module::update();
    /////////////////

    if ( Data::get()->cgi.environment.clouds.type == Data::CGI::Environment::Clouds::Block )
    {
        if ( _framesCounter % 10 == 0 )
        {
            _framesCounter = 0;

            float lat = Data::get()->cgi.camera.latitude;
            float lon = Data::get()->cgi.camera.longitude;
            float alt = _base_asl;

            WGS84 wgs_cam( lat, lon, alt );

            float radius2 = CGI_SKYDOME_RADIUS * CGI_SKYDOME_RADIUS;

            if ( !_created
              || _count     != Data::get()->cgi.environment.clouds.data.block.count
              || _base_asl  != Data::get()->cgi.environment.clouds.data.block.base_asl
              || _thickness != Data::get()->cgi.environment.clouds.data.block.thickness
              || ( _pos_wgs - wgs_cam.getPosition() ).length2() > 0.01 * radius2 )
            {
                _count     = Data::get()->cgi.environment.clouds.data.block.count;
                _base_asl  = Data::get()->cgi.environment.clouds.data.block.base_asl;
                _thickness = Data::get()->cgi.environment.clouds.data.block.thickness;

                create();
            }

            float azim = 0.0f;
            float dist = 0.0f;

            for ( unsigned int i = 0; i < _patClouds.size(); i++ )
            {
                osg::Vec3 pos_wgs = _patClouds[ i ]->getPosition();

                if ( ( wgs_cam.getPosition() - pos_wgs ).length2() > radius2 )
                {
                    azim = fdm::Random::get( 0.0, 2.0 * M_PI );
                    dist = 0.95f * CGI_SKYDOME_RADIUS;

                    osg::Vec3 pos_ned( dist * cos( azim ), dist * sin( azim ), 0.0f );

                    pos_wgs = wgs_cam.getPosition() + wgs_cam.getAttitude() * pos_ned;

                    WGS84 wgs_new( pos_wgs );
                    wgs_new = WGS84( wgs_new.getLat(), wgs_new.getLon(), alt );

                    _patClouds[ i ]->setPosition( wgs_new.getPosition() );
                    _patClouds[ i ]->setAttitude( wgs_new.getAttitude() );
                }
            }

            _pos_wgs = wgs_cam.getPosition();
        }

        _framesCounter++;
    }
    else
    {
        remove();

        _framesCounter = 0;
        _count = 0;
        _base_asl = 0.0;
        _thickness = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////

void CloudsBlock::create()
{
    remove();

    _created = true;

    float lat = Data::get()->cgi.camera.latitude;
    float lon = Data::get()->cgi.camera.longitude;
    float alt = _base_asl;
    float ang = CGI_SKYDOME_RADIUS / 1852.0f / 60.0f;

    int cloudsNumber = _count;
    cloudsNumber = std::min( std::max( cloudsNumber, 0 ), CGI_CLOUDS_MAX_COUNT );

    for ( int i = 0; i < cloudsNumber; i++ )
    {
        osg::ref_ptr< osg::PositionAttitudeTransform > pat = new osg::PositionAttitudeTransform();
        _root->addChild( pat.get() );
        _patClouds.push_back( pat );

        createBlock( pat.get() );

        float d_lat = osg::DegreesToRadians( fdm::Random::get( -ang, ang ) );
        float d_lon = osg::DegreesToRadians( fdm::Random::get( -ang, ang ) );

        WGS84 wgs( lat + d_lat, lon + d_lon, alt );

        pat->setPosition( wgs.getPosition() );
        pat->setAttitude( wgs.getAttitude() );
    }
}

////////////////////////////////////////////////////////////////////////////////

void CloudsBlock::createBlock( osg::Group *parent )
{
    int spritesNumber = fdm::Random::get( 2, 5 );
    spritesNumber = std::min( std::max( spritesNumber, 0 ), CGI_CLOUDS_MAX_SPRITES );

    for ( int i = 0; i < spritesNumber; i++ )
    {
        osg::ref_ptr<osg::PositionAttitudeTransform> pat = new osg::PositionAttitudeTransform();
        parent->addChild( pat.get() );

        createSprite( pat.get() );

        double scale = _thickness * fdm::Random::get( 0.6f, 1.0f );

        osg::Vec3 pos( _thickness * fdm::Random::get( 0.1f, 1.0f ),
                       _thickness * fdm::Random::get( 0.1f, 1.0f ),
                       0.0 );

        pat->setScale( osg::Vec3( scale, scale, scale ) );
        pat->setPosition( pos );
    }
}

////////////////////////////////////////////////////////////////////////////////

void CloudsBlock::createSprite( osg::Group *parent )
{
    osg::ref_ptr<osg::Billboard> billboard = new osg::Billboard();
    parent->addChild( billboard.get() );

    billboard->setMode( osg::Billboard::AXIAL_ROT );
    billboard->setNormal( osg::Vec3f( 0.0f, 1.0f, 0.0f ) );

    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
    billboard->addDrawable( geometry.get(), osg::Vec3( 0.0, 0.0, 0.0 ) );

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();

    v->push_back( osg::Vec3f( -1.0f, 0.0f,  0.0f ) );
    v->push_back( osg::Vec3f(  1.0f, 0.0f,  0.0f ) );
    v->push_back( osg::Vec3f(  1.0f, 0.0f, -1.0f ) );
    v->push_back( osg::Vec3f( -1.0f, 0.0f, -1.0f ) );

    Geometry::createQuad( geometry.get(), v.get(), true );

    osg::ref_ptr<osg::StateSet> billboardStateSet = billboard->getOrCreateStateSet();

    // texture
    int i_tex = fdm::Random::get( 0, _textures.size() - 1 );
    billboardStateSet->setTextureAttributeAndModes( 0, _textures.at( i_tex ).get(), osg::StateAttribute::ON );

    // material
    osg::ref_ptr<osg::Material> material = new osg::Material();
    material->setColorMode( osg::Material::AMBIENT_AND_DIFFUSE );
    material->setAmbient( osg::Material::FRONT_AND_BACK, osg::Vec4f( 0.8f, 0.8f, 0.8f, 1.0f ) );
    material->setDiffuse( osg::Material::FRONT_AND_BACK, osg::Vec4f( 1.0f, 1.0f, 1.0f, 1.0f ) );
    billboardStateSet->setAttribute( material.get() );
}

////////////////////////////////////////////////////////////////////////////////

void CloudsBlock::remove()
{
    _created = false;

    if ( _root->getNumChildren() > 0 )
    {
        _root->removeChildren( 0, _root->getNumChildren() );
    }

    for ( unsigned int i = 0; i < _patClouds.size(); i++ )
    {
        _patClouds[ i ] = 0;
    }

    _patClouds.clear();
}
