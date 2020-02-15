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

#include <cgi/cgi_Vector.h>

#include <osg/Billboard>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/LineWidth>
#include <osg/PositionAttitudeTransform>

#include <osgText/Text>

#include <Data.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

Vector::Vector()
{
    _root = new osg::Group();
}

////////////////////////////////////////////////////////////////////////////////

Vector::~Vector() {}

////////////////////////////////////////////////////////////////////////////////

void Vector::update()
{
    if ( _root->getNumChildren() > 0 )
    {
        _root->removeChildren( 0, _root->getNumChildren() );
    }

    drawLine( 0, osg::Vec3( 1.0f, 0.0f, 0.0f ) );
    drawLine( 1, osg::Vec3( 0.0f, 1.0f, 0.0f ) );
    drawLine( 2, osg::Vec3( 0.0f, 0.0f, 1.0f ) );
}

////////////////////////////////////////////////////////////////////////////////

void Vector::drawLine( int index, const osg::Vec3 &color )
{
    if ( Data::get()->other.vect[ index ] )
    {
        osg::Vec3 b( 0.0f, 0.0f, 0.0f );
        osg::Vec3 e( Data::get()->other.vect_x_enu[ index ],
                     Data::get()->other.vect_y_enu[ index ],
                     Data::get()->other.vect_z_enu[ index ] );

        drawLine( b, e, color, Data::get()->other.vect_label[ index ] );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Vector::drawLine( const osg::Vec3 &b,
                       const osg::Vec3 &e,
                       const osg::Vec3 &color,
                       const std::string &label )
{
    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    _root->addChild( geode.get() );

    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();  // normals
    osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();  // colors

    v->push_back( b );
    v->push_back( e );

    n->push_back( osg::Vec3( 0.0f, 0.0f, 1.0f ) );
    c->push_back( osg::Vec4( color, 1.0f ) );

    geometry->setVertexArray( v.get() );
    geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINE_STRIP, 0, v->size() ) );
    geometry->setNormalArray( n.get() );
    geometry->setNormalBinding( osg::Geometry::BIND_OVERALL );
    geometry->setColorArray( c.get() );
    geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

    geode->addDrawable( geometry.get() );

    osg::ref_ptr<osg::LineWidth> lineWidth = new osg::LineWidth();
    lineWidth->setWidth( 2.0f );

    geode->getOrCreateStateSet()->setAttributeAndModes( lineWidth, osg::StateAttribute::ON );

    //
    osg::ref_ptr<osg::PositionAttitudeTransform> pat = new osg::PositionAttitudeTransform();
    _root->addChild( pat.get() );

    osg::Vec3 perpendicular = ( e - b ) ^ osg::Vec3( 0.0, 0.0, 1.0 );
    perpendicular.normalize();

    pat->setPosition( b + perpendicular + osg::Vec3( 0.0, 0.0, 1.0 ) );

    osg::ref_ptr<osg::Billboard> billboard = new osg::Billboard();
    pat->addChild( billboard.get() );

    billboard->setMode( osg::Billboard::POINT_ROT_EYE );
    billboard->setNormal( osg::Vec3f( 0.0f, 0.0f, 1.0f ) );

    osg::ref_ptr<osgText::Text> text = new osgText::Text();
    billboard->addDrawable( text );

    text->setColor( osg::Vec4( color, 1.0f ) );
    text->setCharacterSize( 0.5f );
    text->setAxisAlignment( osgText::TextBase::XY_PLANE );
    text->setPosition( osg::Vec3( 0.0f, 0.0f, 0.0f ) );
    text->setLayout( osgText::Text::LEFT_TO_RIGHT );
    text->setAlignment( osgText::Text::CENTER_CENTER );
    text->setText( label.c_str() );
}
