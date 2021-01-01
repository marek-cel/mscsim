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

#include <g1000/cgi/g1000_COM.h>

#include <g1000/cgi/g1000_Colors.h>
#include <g1000/cgi/g1000_Fonts.h>

////////////////////////////////////////////////////////////////////////////////

using namespace g1000;

////////////////////////////////////////////////////////////////////////////////

const double COM::_z_text = -40.0;

////////////////////////////////////////////////////////////////////////////////

COM::COM( IFD *ifd ) :
    Module( ifd )
{
    create();
}

////////////////////////////////////////////////////////////////////////////////

COM::~COM() {}

////////////////////////////////////////////////////////////////////////////////

void COM::update()
{

}

////////////////////////////////////////////////////////////////////////////////

void COM::create()
{
    const double h = 11.0;

    const double x_label   = G1000_GDU_WIDTH_2 - 10.0;
    const double x_active  = G1000_GDU_WIDTH_2 - 50.0;
    const double x_standby = x_active + 23.0;

    const double y_upper = G1000_GDU_HEIGHT_2 - 0.37 * h;
    const double y_lower = G1000_GDU_HEIGHT_2 - 0.87 * h;

    const double y_arrow_upper = G1000_GDU_HEIGHT_2 - 0.25 * h;
    const double y_arrow_lower = G1000_GDU_HEIGHT_2 - 0.75 * h;

    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    _root->addChild( geode.get() );

    osg::ref_ptr<osgText::Text> textLabel1 = new osgText::Text();
    textLabel1->setFont( Fonts::get( "fonts/g1000.ttf" ) );
    textLabel1->setColor( osg::Vec4( Colors::_white, 1.0 ) );
    textLabel1->setCharacterSize( 3.0 );
    textLabel1->setAxisAlignment( osgText::TextBase::XY_PLANE );
    textLabel1->setPosition( osg::Vec3( x_label, y_upper, _z_text ) );
    textLabel1->setLayout( osgText::Text::LEFT_TO_RIGHT );
    textLabel1->setAlignment( osgText::Text::LEFT_BASE_LINE );
    textLabel1->setText( "COM1" );
    geode->addDrawable( textLabel1 );

    osg::ref_ptr<osgText::Text> textLabel2 = new osgText::Text();
    textLabel2->setFont( Fonts::get( "fonts/g1000.ttf" ) );
    textLabel2->setColor( osg::Vec4( Colors::_white, 1.0 ) );
    textLabel2->setCharacterSize( 3.0 );
    textLabel2->setAxisAlignment( osgText::TextBase::XY_PLANE );
    textLabel2->setPosition( osg::Vec3( x_label, y_lower, _z_text ) );
    textLabel2->setLayout( osgText::Text::LEFT_TO_RIGHT );
    textLabel2->setAlignment( osgText::Text::LEFT_BASE_LINE );
    textLabel2->setText( "COM2" );
    geode->addDrawable( textLabel2 );

    _textComActive1 = new osgText::Text();
    _textComActive1->setFont( Fonts::get( "fonts/g1000.ttf" ) );
    _textComActive1->setColor( osg::Vec4( Colors::_white, 1.0 ) );
    _textComActive1->setCharacterSize( 4.0 );
    _textComActive1->setAxisAlignment( osgText::TextBase::XY_PLANE );
    _textComActive1->setPosition( osg::Vec3( x_active, y_upper, _z_text ) );
    _textComActive1->setLayout( osgText::Text::LEFT_TO_RIGHT );
    _textComActive1->setAlignment( osgText::Text::LEFT_BASE_LINE );
    _textComActive1->setText( "118.000" );
    geode->addDrawable( _textComActive1 );

    _textComActive2 = new osgText::Text();
    _textComActive2->setFont( Fonts::get( "fonts/g1000.ttf" ) );
    _textComActive2->setColor( osg::Vec4( Colors::_white, 1.0 ) );
    _textComActive2->setCharacterSize( 4.0 );
    _textComActive2->setAxisAlignment( osgText::TextBase::XY_PLANE );
    _textComActive2->setPosition( osg::Vec3( x_active, y_lower, _z_text ) );
    _textComActive2->setLayout( osgText::Text::LEFT_TO_RIGHT );
    _textComActive2->setAlignment( osgText::Text::LEFT_BASE_LINE );
    _textComActive2->setText( "118.000" );
    geode->addDrawable( _textComActive2 );

    _textComStandby1 = new osgText::Text();
    _textComStandby1->setFont( Fonts::get( "fonts/g1000.ttf" ) );
    _textComStandby1->setColor( osg::Vec4( Colors::_white, 1.0 ) );
    _textComStandby1->setCharacterSize( 4.0 );
    _textComStandby1->setAxisAlignment( osgText::TextBase::XY_PLANE );
    _textComStandby1->setPosition( osg::Vec3( x_standby, y_upper, _z_text ) );
    _textComStandby1->setLayout( osgText::Text::LEFT_TO_RIGHT );
    _textComStandby1->setAlignment( osgText::Text::LEFT_BASE_LINE );
    _textComStandby1->setText( "136.975" );
    geode->addDrawable( _textComStandby1 );

    _textComStandby2 = new osgText::Text();
    _textComStandby2->setFont( Fonts::get( "fonts/g1000.ttf" ) );
    _textComStandby2->setColor( osg::Vec4( Colors::_white, 1.0 ) );
    _textComStandby2->setCharacterSize( 4.0 );
    _textComStandby2->setAxisAlignment( osgText::TextBase::XY_PLANE );
    _textComStandby2->setPosition( osg::Vec3( x_standby, y_lower, _z_text ) );
    _textComStandby2->setLayout( osgText::Text::LEFT_TO_RIGHT );
    _textComStandby2->setAlignment( osgText::Text::LEFT_BASE_LINE );
    _textComStandby2->setText( "136.975" );
    geode->addDrawable( _textComStandby2 );

    createArrow( _root.get(), h, y_arrow_upper, x_active );
    createArrow( _root.get(), h, y_arrow_lower, x_active );
}

////////////////////////////////////////////////////////////////////////////////

void COM::createArrow( osg::Group *parent, double h, double y, double x_active )
{
    const double w = 4.0;
    const double d = 1.0;

    double x_l = x_active + 18.0;
    double x_r = x_l + w;

    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    parent->addChild( geode.get() );

    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();

    v->push_back( osg::Vec3( x_l     , y     , _z_text ) );
    v->push_back( osg::Vec3( x_r     , y     , _z_text ) );
    v->push_back( osg::Vec3( x_l     , y     , _z_text ) );
    v->push_back( osg::Vec3( x_l + d , y + d , _z_text ) );
    v->push_back( osg::Vec3( x_l     , y     , _z_text ) );
    v->push_back( osg::Vec3( x_l + d , y - d , _z_text ) );
    v->push_back( osg::Vec3( x_r     , y     , _z_text ) );
    v->push_back( osg::Vec3( x_r - d , y + d , _z_text ) );
    v->push_back( osg::Vec3( x_r     , y     , _z_text ) );
    v->push_back( osg::Vec3( x_r - d , y - d , _z_text ) );

    c->push_back( osg::Vec4( Colors::_cyan, 1.0 ) );

    geometry->setVertexArray( v.get() );
    geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, v->size() ) );

    geometry->setColorArray( c.get() );
    geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

    geode->addDrawable( geometry.get() );
}
