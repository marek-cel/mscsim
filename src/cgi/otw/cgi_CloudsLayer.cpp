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

#include <cgi/otw/cgi_CloudsLayer.h>

#include <osg/AlphaFunc>
#include <osg/BlendFunc>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Material>
#include <osg/PositionAttitudeTransform>

#include <cgi/cgi_Defines.h>
#include <cgi/cgi_WGS84.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

CloudsLayer::CloudsLayer( const Module *parent ) :
    Module( parent ),

    _cover ( Data::CGI::Environment::Clouds::Data::Layer::SKC ),
    _base_asl ( 0.0f ),

    _framesCounter ( 0 ),
    _created ( false )
{
    _textures.push_back( Textures::get( "cgi/textures/cloud_st_few.png" ) );
    _textures.push_back( Textures::get( "cgi/textures/cloud_st_sct.png" ) );
    _textures.push_back( Textures::get( "cgi/textures/cloud_st_bkn.png" ) );
    _textures.push_back( Textures::get( "cgi/textures/cloud_st_ovc.png" ) );
}

////////////////////////////////////////////////////////////////////////////////

CloudsLayer::~CloudsLayer() {}

////////////////////////////////////////////////////////////////////////////////

void CloudsLayer::update()
{
    /////////////////
    Module::update();
    /////////////////

    if ( Data::get()->cgi.environment.clouds.type == Data::CGI::Environment::Clouds::Layer )
    {
        if ( _framesCounter % 10 == 0 )
        {
            _framesCounter = 0;

            if ( !_created
              || _cover     != Data::get()->cgi.environment.clouds.data.layer.cover
              || _base_asl  != Data::get()->cgi.environment.clouds.data.layer.base_asl )
            {
                _cover     = Data::get()->cgi.environment.clouds.data.layer.cover;
                _base_asl  = Data::get()->cgi.environment.clouds.data.layer.base_asl;

                create();
            }
        }

        _framesCounter++;
    }
    else
    {
        remove();

        _framesCounter = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////

void CloudsLayer::create()
{
    remove();

    _created = true;

    osg::ref_ptr<osg::PositionAttitudeTransform> pat = new osg::PositionAttitudeTransform();
    _root->addChild( pat.get() );

    WGS84 wgs( Data::get()->cgi.camera.latitude,
               Data::get()->cgi.camera.longitude,
               0.0 );

    pat->setAttitude( wgs.getAttitude() );
    pat->setPosition( wgs.getPosition() );

    createLayer( pat.get(),
                 Data::get()->cgi.camera.latitude,
                 Data::get()->cgi.camera.longitude,
                 _base_asl );
}

////////////////////////////////////////////////////////////////////////////////

void CloudsLayer::createLayer( osg::Group *parent, double lat , double lon, double alt  )
{
    osg::Vec3 ctr_wgs = WGS84::geo2wgs( lat, lon, 0.0 );

    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    parent->addChild( geode.get() );

    osg::ref_ptr<osg::StateSet> geodeStateSet = geode->getOrCreateStateSet();

    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();
    geode->addDrawable( geom.get() );

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec2Array> t = new osg::Vec2Array();
    osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();

    short k = 20;

    float x = -CGI_SKYDOME_RADIUS;
    float y = -CGI_SKYDOME_RADIUS;

    float dx = 2.0f * CGI_SKYDOME_RADIUS / (float)k;
    float dy = 2.0f * CGI_SKYDOME_RADIUS / (float)k;

    osg::Vec3 v1_ned;
    osg::Vec3 v2_ned;
    osg::Vec3 v1_wgs;
    osg::Vec3 v2_wgs;

    double lat1 = 0.0;
    double lat2 = 0.0;
    double lon1 = 0.0;
    double lon2 = 0.0;
    double dummy = 0.0;

    for ( short ix = 0; ix < k + 1; ix++ )
    {
        y = ( ix % 2 == 0 ) ? -CGI_SKYDOME_RADIUS : CGI_SKYDOME_RADIUS;

        for ( short iy = 0; iy < k + 1; iy++ )
        {
            v1_ned.set( x      , y, 0.0f );
            v2_ned.set( x + dx , y, 0.0f );

            v1_wgs = WGS84::r_ned2wgs( ctr_wgs, v1_ned );
            v2_wgs = WGS84::r_ned2wgs( ctr_wgs, v2_ned );

            WGS84::wgs2geo( v1_wgs, lat1, lon1, dummy );
            WGS84::wgs2geo( v2_wgs, lat2, lon2, dummy );

            v1_wgs = WGS84::geo2wgs( lat1, lon1, alt );
            v2_wgs = WGS84::geo2wgs( lat2, lon2, alt );

            v1_ned = WGS84::r_wgs2ned( ctr_wgs, v1_wgs );
            v2_ned = WGS84::r_wgs2ned( ctr_wgs, v2_wgs );

            osg::Vec3f n1 = WGS84::wgs2ned( ctr_wgs, v1_wgs );
            osg::Vec3f n2 = WGS84::wgs2ned( ctr_wgs, v2_wgs );

            n1.normalize();
            n2.normalize();

            v->push_back( v1_ned );
            v->push_back( v2_ned );

            n->push_back( n1 );
            n->push_back( n2 );

            if ( iy % 2 == 0 )
            {
                t->push_back( osg::Vec2( 0, 0 ) );
                t->push_back( osg::Vec2( 0, 1 ) );
            }
            else
            {
                t->push_back( osg::Vec2( 1, 0 ) );
                t->push_back( osg::Vec2( 1, 1 ) );
            }

            y += ( ix % 2 == 0 ) ? dy : -dy;
        }

        x += dx;
    }

    geom->setNormalArray( n.get() );
    geom->setNormalBinding( osg::Geometry::BIND_PER_VERTEX );
    geom->setVertexArray( v.get() );
    geom->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::TRIANGLE_STRIP, 0, v->size() ) );
    geom->setTexCoordArray( 0, t.get() );

    // texture
    osg::ref_ptr<osg::Texture2D> texture;
    switch ( _cover )
    {
    case Data::CGI::Environment::Clouds::Data::Layer::FEW:
        texture = _textures.at( 0 ).get();
        break;

    case Data::CGI::Environment::Clouds::Data::Layer::SCT:
        texture = _textures.at( 1 ).get();
        break;

    case Data::CGI::Environment::Clouds::Data::Layer::BKN:
        texture = _textures.at( 2 ).get();
        break;

    case Data::CGI::Environment::Clouds::Data::Layer::OVC:
    default:
        texture = _textures.at( 3 ).get();
        break;
    }
    geodeStateSet->setTextureAttributeAndModes( 0, texture.get(), osg::StateAttribute::ON );

    // material
    osg::ref_ptr<osg::Material> material = new osg::Material();
    material->setColorMode( osg::Material::AMBIENT_AND_DIFFUSE );
    material->setAmbient( osg::Material::FRONT, osg::Vec4f( 0.8f, 0.8f, 0.8f, 1.0f ) );
    material->setDiffuse( osg::Material::FRONT, osg::Vec4f( 1.0f, 1.0f, 1.0f, 1.0f ) );

    geodeStateSet->setAttribute( material.get() );

    // alpha blending
    osg::ref_ptr<osg::AlphaFunc> alphaFunc = new osg::AlphaFunc();
    osg::ref_ptr<osg::BlendFunc> blendFunc = new osg::BlendFunc();
    alphaFunc->setFunction( osg::AlphaFunc::GEQUAL, 0.01f );

    geodeStateSet->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
    geodeStateSet->setAttributeAndModes( blendFunc.get(), osg::StateAttribute::ON );
    geodeStateSet->setAttributeAndModes( alphaFunc.get(), osg::StateAttribute::ON );
    geodeStateSet->setMode( GL_BLEND, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE );
    geodeStateSet->setRenderBinDetails( CGI_DEPTH_SORTED_BIN_CLOUDS, "DepthSortedBin" );
}

////////////////////////////////////////////////////////////////////////////////

void CloudsLayer::remove()
{
    _created = false;

    if ( _root->getNumChildren() > 0 )
    {
        _root->removeChildren( 0, _root->getNumChildren() );
    }
}
