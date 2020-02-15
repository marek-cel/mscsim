/****************************************************************************//*
 * Copyright (C) 2020 Marek M. Cel
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

#include <cgi/cgi_Terrain.h>

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Texture2D>

#include <osgDB/ReadFile>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

Terrain::Terrain()
{
    _root = new osg::Group();

    createTerrain();
}

////////////////////////////////////////////////////////////////////////////////

Terrain::~Terrain() {}

////////////////////////////////////////////////////////////////////////////////

void Terrain::update() {}

////////////////////////////////////////////////////////////////////////////////

void Terrain::createTerrain()
{
    const double w = 300.0;
    const double z = -10.0;

    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    _root->addChild( geode.get() );

    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();
    geode->addDrawable( geom.get() );

    osg::ref_ptr<osg::StateSet> stateSet = geode->getOrCreateStateSet();

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec2Array> t = new osg::Vec2Array();
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();

    v->push_back( osg::Vec3(  0.5 * w, -0.5 * w, z ) );
    v->push_back( osg::Vec3(  0.5 * w,  0.5 * w, z ) );
    v->push_back( osg::Vec3( -0.5 * w,  0.5 * w, z ) );
    v->push_back( osg::Vec3( -0.5 * w, -0.5 * w, z ) );

    n->push_back( osg::Vec3( 0.0, 0.0, 1.0 ) );

    t->push_back( osg::Vec2( 0, 0 ) );
    t->push_back( osg::Vec2( 1, 0 ) );
    t->push_back( osg::Vec2( 1, 1 ) );
    t->push_back( osg::Vec2( 0, 1 ) );

    c->push_back( osg::Vec4( 1.0, 1.0, 1.0, 1.0 ) );

    geom->setVertexArray( v );
    geom->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::TRIANGLE_FAN, 0, v->size() ) );

    geom->setNormalArray( n );
    geom->setNormalBinding( osg::Geometry::BIND_OVERALL );

    geom->setColorArray( c.get() );
    geom->setColorBinding( osg::Geometry::BIND_OVERALL );

    geom->setTexCoordArray( 0, t.get() );

    osg::ref_ptr<osg::Image> image = osgDB::readImageFile( "../data/terrain.png" );

    if ( image.valid() )
    {
        osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D();
        texture->setImage( image.get() );

        texture->setWrap( osg::Texture2D::WRAP_S, osg::Texture::MIRROR );
        texture->setWrap( osg::Texture2D::WRAP_T, osg::Texture::MIRROR );

        texture->setNumMipmapLevels( 4 );
        texture->setMaxAnisotropy( 8.0 );

        texture->setFilter( osg::Texture::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_NEAREST );
        texture->setFilter( osg::Texture::MAG_FILTER, osg::Texture::LINEAR );

        texture->setUnRefImageDataAfterApply( false );

        stateSet->setTextureAttributeAndModes( 0, texture, osg::StateAttribute::ON );
    }
}
