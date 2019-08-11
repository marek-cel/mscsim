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
    createSymbolAerodrome();

    createOwnship();

    initAerodromes();

    setScale( 1.0 );
}

////////////////////////////////////////////////////////////////////////////////

Icons::~Icons() {}

////////////////////////////////////////////////////////////////////////////////

void Icons::update()
{
    updateOwnship();
}

////////////////////////////////////////////////////////////////////////////////

void Icons::setScale( double scale )
{
    double s = 1.5 * 1.0e6 * scale;
    osg::Vec3d sv( s, s, 1.0 );

    m_ownship.pat->setScale( sv );

    for ( Aerodromes::iterator it = m_aerodromes.begin(); it != m_aerodromes.end(); it++ )
    {
        (*it)->setScale( sv );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Icons::createIcon( osg::Group *parent, float z,
                        const std::string &textureFile )
{
    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    parent->addChild( geode.get() );

    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
    geode->addDrawable( geometry.get() );

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();  // vertices

    v->push_back( osg::Vec3( -0.5f, -0.5f, z ) );
    v->push_back( osg::Vec3(  0.5f, -0.5f, z ) );
    v->push_back( osg::Vec3(  0.5f,  0.5f, z ) );
    v->push_back( osg::Vec3( -0.5f,  0.5f, z ) );

    Geometry::createQuad( geometry.get(), v.get(), true, true );

    osg::ref_ptr<osg::StateSet> stateSet = geode->getOrCreateStateSet();
    stateSet->setMode( GL_LIGHTING, osg::StateAttribute::OFF );

    // texture
    osg::ref_ptr<osg::Texture2D> texture = Textures::get( textureFile );

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

////////////////////////////////////////////////////////////////////////////////

void Icons::createOwnship()
{
    m_ownship.pat = new osg::PositionAttitudeTransform();
    m_root->addChild( m_ownship.pat.get() );

    m_ownship.speedLeader = new osg::Group();
    m_ownship.pat->addChild( m_ownship.speedLeader.get() );

    createIcon( m_ownship.pat.get(), Map::zOwnship, "data/map/icons/air_friend.png" );
}

////////////////////////////////////////////////////////////////////////////////

void Icons::createSymbolAerodrome()
{
    const float ri = 0.2;
    const float ro = 0.25;
    const float rm = 0.5 * ( ri + ro );

    const float w_2 = 12.0 * ro / 72.0;
    const float h = 2.0 * 12.0 * ro / 72.0;

    m_symbolAerodrome = new osg::Geode();

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();
    osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();

    c->push_back( osg::Vec4( Map::colorAeroData, 1.0f ) );
    n->push_back( osg::Vec3( 0.0f, 0.0f, 1.0f ) );

    osg::ref_ptr<osg::Geometry> geom1 = new osg::Geometry();
    osg::ref_ptr<osg::Geometry> geom2 = new osg::Geometry();

    m_symbolAerodrome->addDrawable( geom1.get() );
    m_symbolAerodrome->addDrawable( geom2.get() );

    Geometry::createRing( geom1.get(), 0.2, 0.25 );

    geom1->setColorArray( c.get() );
    geom1->setColorBinding( osg::Geometry::BIND_OVERALL );

    v->push_back( osg::Vec3( -w_2,  rm     , 0.0f ) );
    v->push_back( osg::Vec3(  w_2,  rm     , 0.0f ) );
    v->push_back( osg::Vec3(  w_2,  rm + h , 0.0f ) );
    v->push_back( osg::Vec3( -w_2,  rm + h , 0.0f ) );

    v->push_back( osg::Vec3( -w_2, -rm - h , 0.0f ) );
    v->push_back( osg::Vec3(  w_2, -rm - h , 0.0f ) );
    v->push_back( osg::Vec3(  w_2, -rm     , 0.0f ) );
    v->push_back( osg::Vec3( -w_2, -rm     , 0.0f ) );

    v->push_back( osg::Vec3(  rm     , -w_2, 0.0f ) );
    v->push_back( osg::Vec3(  rm + h , -w_2, 0.0f ) );
    v->push_back( osg::Vec3(  rm + h ,  w_2, 0.0f ) );
    v->push_back( osg::Vec3(  rm     ,  w_2, 0.0f ) );

    v->push_back( osg::Vec3( -rm - h , -w_2, 0.0f ) );
    v->push_back( osg::Vec3( -rm     , -w_2, 0.0f ) );
    v->push_back( osg::Vec3( -rm     ,  w_2, 0.0f ) );
    v->push_back( osg::Vec3( -rm - h ,  w_2, 0.0f ) );

    geom2->setVertexArray( v.get() );
    geom2->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::QUADS, 0, v->size() ) );
    geom2->setNormalArray( n.get() );
    geom2->setNormalBinding( osg::Geometry::BIND_OVERALL );
    geom2->setColorArray( c.get() );
    geom2->setColorBinding( osg::Geometry::BIND_OVERALL );
}

////////////////////////////////////////////////////////////////////////////////

void Icons::initAerodromes()
{
    // TODO
    osg::ref_ptr<osg::PositionAttitudeTransform> pat = new osg::PositionAttitudeTransform();
    m_root->addChild( pat.get() );
    pat->addChild( m_symbolAerodrome.get() );
    pat->setPosition( osg::Vec3d( 0.0, 0.0, Map::zAerodromes ) );
    m_aerodromes.push_back( pat.get() );
    // TODO
}

////////////////////////////////////////////////////////////////////////////////

void Icons::updateOwnship()
{
    m_ownship.pat->setPosition( osg::Vec3( Mercator::x( Data::get()->ownship.longitude ),
                                           Mercator::y( Data::get()->ownship.latitude ),
                                           0.0f ) );

    if ( m_ownship.speedLeader->getNumChildren() > 0 )
    {
        m_ownship.speedLeader->removeChildren( 0, m_ownship.speedLeader->getNumChildren() );
    }

    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    m_ownship.speedLeader->addChild( geode.get() );

    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();  // colors
    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();

    n->push_back( osg::Vec3( 0.0f, 0.0f, 1.0f ) );
    c->push_back( osg::Vec4( 0.0f, 0.0f, 0.0f, 1.0f ) );

    const float coef = 1.0f / 50.0f; // 180 km/h
    osg::Vec3 vel_ned( coef * Data::get()->ownship.vel_east,
                       coef * Data::get()->ownship.vel_north,
                       Map::zSpeedLeader );

    if ( vel_ned.length2() > 1.0f )
    {
        vel_ned.normalize();
    }

    v->push_back( osg::Vec3( 0.0f, 0.0f, Map::zSpeedLeader ) );
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
    lineWidth->setWidth( 1.25f );

    stateSet->setAttributeAndModes( lineWidth, osg::StateAttribute::ON );
    stateSet->setMode( GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE );
}
