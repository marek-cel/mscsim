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

#include <cgi/otw/cgi_CloudsBlock.h>

#include <osg/AlphaFunc>
#include <osg/Billboard>
#include <osg/BlendFunc>
#include <osg/Geometry>
#include <osg/Material>

#include <Data.h>

#include <fdm/utils/fdm_Random.h>

#include <cgi/cgi_Defines.h>
#include <cgi/cgi_Geometry.h>
#include <cgi/cgi_WGS84.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

CloudsBlock::CloudsBlock( Module *parent ) :
    Module( parent ),
    m_framesCounter ( 0 ),
    m_created ( false ),
    m_count ( 0 ),
    m_base_asl ( 0.0 ),
    m_thickness ( 0.0 )
{
    m_textures.push_back( Textures::get( "data/cgi/textures/cloud_cu_1.png" ) );
    m_textures.push_back( Textures::get( "data/cgi/textures/cloud_cu_2.png" ) );
    m_textures.push_back( Textures::get( "data/cgi/textures/cloud_cu_3.png" ) );
    m_textures.push_back( Textures::get( "data/cgi/textures/cloud_cu_4.png" ) );
    m_textures.push_back( Textures::get( "data/cgi/textures/cloud_cu_5.png" ) );
    m_textures.push_back( Textures::get( "data/cgi/textures/cloud_cu_6.png" ) );
}

////////////////////////////////////////////////////////////////////////////////

CloudsBlock::~CloudsBlock() {}

////////////////////////////////////////////////////////////////////////////////

void CloudsBlock::update()
{
    /////////////////
    Module::update();
    /////////////////

    if ( Data::get()->environment.clouds.type == Data::Environment::Clouds::Block )
    {
        if ( m_framesCounter % 10 == 0 )
        {
            m_framesCounter = 0;

            float lat = Data::get()->camera.latitude;
            float lon = Data::get()->camera.longitude;
            float alt = m_base_asl;

            WGS84 wgs_cam( lat, lon, alt );

            float radius2 = CGI_SKYDOME_RADIUS * CGI_SKYDOME_RADIUS;

            if ( ( m_pos_wgs - wgs_cam.getPosition() ).length2() > 0.01 * radius2
              || !m_created || m_count != Data::get()->environment.clouds.data.block.count
              || m_base_asl != Data::get()->environment.clouds.data.block.base_asl
              || m_thickness != Data::get()->environment.clouds.data.block.thickness )
            {
                m_count = Data::get()->environment.clouds.data.block.count;
                m_base_asl = Data::get()->environment.clouds.data.block.base_asl;
                m_thickness = Data::get()->environment.clouds.data.block.thickness;

                create();
            }
            else
            {
                float azim = 0.0f;
                float dist = 0.0f;

                for ( unsigned int i = 0; i < m_patClouds.size(); i++ )
                {
                    osg::Vec3 pos_wgs = m_patClouds[ i ]->getPosition();

                    if ( ( wgs_cam.getPosition() - pos_wgs ).length2() > radius2 )
                    {
                        azim = fdm::Random::get( 0.0f, 2.0f * M_PI );
                        dist = 0.95f * CGI_SKYDOME_RADIUS;

                        osg::Vec3 pos_ned( dist * cos( azim ), dist * sin( azim ), 0.0f );

                        pos_wgs = wgs_cam.getPosition() + wgs_cam.getAttitude() * pos_ned;

                        WGS84 wgs_new( pos_wgs );
                        wgs_new = WGS84( wgs_new.getLat(), wgs_new.getLon(), alt );

                        m_patClouds[ i ]->setPosition( wgs_new.getPosition() );
                        m_patClouds[ i ]->setAttitude( wgs_new.getAttitude() );
                    }
                }
            }

            m_pos_wgs = wgs_cam.getPosition();
        }

        m_framesCounter++;
    }
    else
    {
        remove();

        m_framesCounter = 0;
        m_count = 0;
        m_base_asl = 0.0;
        m_thickness = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////

void CloudsBlock::create()
{
    remove();

    m_created = true;

    float lat = Data::get()->camera.latitude;
    float lon = Data::get()->camera.longitude;
    float alt = m_base_asl;
    float ang = CGI_SKYDOME_RADIUS / 1852.0f / 60.0f;

    int cloudsNumber = m_count;
    cloudsNumber = std::min( std::max( cloudsNumber, 0 ), CGI_CLOUDS_MAX_COUNT );

    for ( int i = 0; i < cloudsNumber; i++ )
    {
        osg::ref_ptr< osg::PositionAttitudeTransform > pat = new osg::PositionAttitudeTransform();
        m_root->addChild( pat.get() );
        m_patClouds.push_back( pat );

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

        double scale = m_thickness * fdm::Random::get( 0.6f, 1.0f );

        osg::Vec3 pos( m_thickness * fdm::Random::get( 0.1f, 1.0f ),
                       m_thickness * fdm::Random::get( 0.1f, 1.0f ),
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
    int i_tex = fdm::Random::get( 0, m_textures.size() - 1 );
    billboardStateSet->setTextureAttributeAndModes( 0, m_textures.at( i_tex ).get(), osg::StateAttribute::ON );

    // material
    osg::ref_ptr<osg::Material> material = new osg::Material();
    material->setColorMode( osg::Material::AMBIENT_AND_DIFFUSE );
    material->setAmbient( osg::Material::FRONT, osg::Vec4f( 0.8f, 0.8f, 0.8f, 1.0f ) );
    material->setDiffuse( osg::Material::FRONT, osg::Vec4f( 1.0f, 1.0f, 1.0f, 1.0f ) );
    billboardStateSet->setAttribute( material.get() );

    // alpha blending
    osg::ref_ptr<osg::AlphaFunc> alphaFunc = new osg::AlphaFunc();
    osg::ref_ptr<osg::BlendFunc> blendFunc = new osg::BlendFunc();
    alphaFunc->setFunction( osg::AlphaFunc::GEQUAL, 0.05 );
    billboardStateSet->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
    billboardStateSet->setAttributeAndModes( blendFunc.get(), osg::StateAttribute::ON );
    billboardStateSet->setAttributeAndModes( alphaFunc.get(), osg::StateAttribute::ON );
    billboardStateSet->setMode( GL_BLEND, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE );
    billboardStateSet->setRenderBinDetails( CGI_DEPTH_SORTED_BIN_CLOUDS, "DepthSortedBin" );
}

////////////////////////////////////////////////////////////////////////////////

void CloudsBlock::remove()
{
    m_created = false;

    if ( m_root->getNumChildren() > 0 )
    {
        m_root->removeChildren( 0, m_root->getNumChildren() );
    }

    for ( unsigned int i = 0; i < m_patClouds.size(); i++ )
    {
        m_patClouds[ i ] = 0;
    }

    m_patClouds.clear();
}
