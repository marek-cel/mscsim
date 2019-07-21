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

#include <cgi/map/cgi_Icons.h>

#include <osg/LineWidth>
#include <osg/Material>

#include <Data.h>

#include <cgi/cgi_Geometry.h>
#include <cgi/cgi_Mercator.h>
#include <cgi/cgi_Textures.h>

#include <cgi/map/cgi_Map.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

Icons::Icons( Module *parent ) :
    Module( parent )
{
    m_pat = new osg::PositionAttitudeTransform();
    m_root->addChild( m_pat.get() );

    m_patFill = new osg::PositionAttitudeTransform();
    m_patFill->setPosition( osg::Vec3( 0.0f, 0.0f, Map::zIconsFill ) );
    m_pat->addChild( m_patFill.get() );

    m_patSpeedLeader = new osg::PositionAttitudeTransform();
    m_patSpeedLeader->setPosition( osg::Vec3( 0.0f, 0.0f, Map::zSpeedLeader ) );
    m_pat->addChild( m_patSpeedLeader.get() );

    createIcon();
    setScale( 1.0 );
}

////////////////////////////////////////////////////////////////////////////////

Icons::~Icons() {}

////////////////////////////////////////////////////////////////////////////////

void Icons::update()
{
    m_pat->setPosition( osg::Vec3( Mercator::getX( Data::get()->ownship.longitude ),
                                   Mercator::getY( Data::get()->ownship.latitude ),
                                   0.0f ) );

    if ( m_patSpeedLeader->getNumChildren() > 0 )
    {
        m_patSpeedLeader->removeChildren( 0, m_patSpeedLeader->getNumChildren() );
    }

    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    m_patSpeedLeader->addChild( geode.get() );

    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();  // colors
    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();

    n->push_back( osg::Vec3( 0.0f, 0.0f, 1.0f ) );
    c->push_back( osg::Vec4( 0.0f, 0.0f, 0.0f, 1.0f ) );

    const float coef = 1.0f / 50.0f; // 180 km/h
    osg::Vec3 vel_ned( coef * Data::get()->ownship.vel_east,
                       coef * Data::get()->ownship.vel_north,
                       0.0f );

    if ( vel_ned.length2() > 1.0f )
    {
        vel_ned.normalize();
    }

    v->push_back( osg::Vec3( 0.0f, 0.0f, 0.0f ) );
    v->push_back( vel_ned );

    geometry->setVertexArray( v.get() );
    geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, v->size() ) );
    geometry->setNormalArray( n.get() );
    geometry->setNormalBinding( osg::Geometry::BIND_OVERALL );
    geometry->setColorArray( c.get() );
    geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

    geode->addDrawable( geometry.get() );

    ////////////////////

    osg::ref_ptr<osg::StateSet> stateSet = geode->getOrCreateStateSet();

    osg::ref_ptr<osg::LineWidth> lineWidth = new osg::LineWidth();
    lineWidth->setWidth( 1.0f );

    stateSet->setAttributeAndModes( lineWidth, osg::StateAttribute::ON );
    stateSet->setMode( GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE );
}

////////////////////////////////////////////////////////////////////////////////

void Icons::setScale( double scale )
{
    double s = 1.5 * 1.0e6 * scale;
    m_patFill->setScale( osg::Vec3( s, s, s ) );
    m_patSpeedLeader->setScale( osg::Vec3( s, s, s ) );
}

////////////////////////////////////////////////////////////////////////////////

void Icons::createIcon()
{
    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    m_patFill->addChild( geode.get() );

    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
    geode->addDrawable( geometry.get() );

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();  // vertices

    v->push_back( osg::Vec3( -0.5f, -0.5f, 0.0f ) );
    v->push_back( osg::Vec3(  0.5f, -0.5f, 0.0f ) );
    v->push_back( osg::Vec3(  0.5f,  0.5f, 0.0f ) );
    v->push_back( osg::Vec3( -0.5f,  0.5f, 0.0f ) );

    Geometry::createQuad( geometry.get(), v.get(), true, true );

    osg::ref_ptr<osg::StateSet> stateSet = geode->getOrCreateStateSet();
    stateSet->setMode( GL_LIGHTING, osg::StateAttribute::OFF );

    // texture
    osg::ref_ptr<osg::Texture2D> texture = Textures::get( "data/map/icons/air_friend.png" );

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
