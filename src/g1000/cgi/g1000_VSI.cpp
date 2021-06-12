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

#include <g1000/cgi/g1000_VSI.h>

#include <sstream>

#include <osg/Geode>
#include <osg/Geometry>

#include <g1000/g1000_Defines.h>

#include <g1000/cgi/g1000_Colors.h>
#include <g1000/cgi/g1000_Fonts.h>

#include <g1000/utils/g1000_Misc.h>
#include <g1000/utils/g1000_Units.h>

////////////////////////////////////////////////////////////////////////////////

using namespace g1000;

////////////////////////////////////////////////////////////////////////////////

const double VSI::_x_offset = -10.0;
const double VSI::_y_offset =  19.0;

const osg::Vec3 VSI::_colorBack = osg::Vec3( 0.38, 0.38, 0.38 );

const double VSI::_z_back  = -80.0;
const double VSI::_z_box   = -60.0;
const double VSI::_z_bug   = -59.0;
const double VSI::_z_frame = -60.0;
const double VSI::_z_ptr   = -58.0;
const double VSI::_z_scale = -70.0;

const double VSI::_vs2pt = 0.014;

////////////////////////////////////////////////////////////////////////////////

VSI::VSI( IFD *ifd ) :
    Module( ifd )
{
    _pat = new osg::PositionAttitudeTransform();
    _root->addChild( _pat.get() );
    _pat->setPosition( osg::Vec3( _x_offset, _y_offset, 0.0 ) );

    _patPointer = new osg::PositionAttitudeTransform();
    _pat->addChild( _patPointer.get() );

    _patBug = new osg::PositionAttitudeTransform();
    _pat->addChild( _patBug.get() );

    createBack();
    createBoxSelect();
    createBug();
    createFrame();
    createPointer();
    createScale();
}

////////////////////////////////////////////////////////////////////////////////

VSI::~VSI() {}

////////////////////////////////////////////////////////////////////////////////

void VSI::update()
{
    double vs_fpm = Units::mps2fpm( _ifd->gdc()->getClimbRate() );
    double select_fpm = Units::mps2fpm( _ifd->input().sel_climbRate );

    std::stringstream vs_str;
    int vs_int = 50 * floor( fabs( vs_fpm / 50.0 ) + 0.5 );

    //if ( vs_fpm < -2000.0 ) vs_int = -vs_int;

    if ( vs_int != 0 )
        vs_str << vs_int;

    _textVS->setText( vs_str.str() );

    //if ( climbRate_fpm < -2000.0 ) climbRate_fpm = -2000.0;
    //if ( climbRate_fpm >  2000.0 ) climbRate_fpm =  2000.0;
    double dy_ptr = Misc::satur( -29.0, 29.0, _vs2pt * vs_fpm );
    double dy_sel = Misc::satur( -29.0, 29.0, _vs2pt * select_fpm );

    _patPointer->setPosition( osg::Vec3( 0.0, dy_ptr, 0.0 ) );
    _patBug->setPosition( osg::Vec3( 0.0, dy_sel, 0.0 ) );

    std::stringstream sel_str;
    sel_str << (int)select_fpm;

    _textSelect->setText( sel_str.str() );

}

////////////////////////////////////////////////////////////////////////////////

void VSI::createBack()
{
    const double x = 67.0;
    const double h = 31.0;
    const double w = 9.0;
    const double d = 4.5;

    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    _pat->addChild( geode.get() );

    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();

    c->push_back( osg::Vec4( _colorBack, 0.5 ) );

    v->push_back( osg::Vec3( x     ,  -h , _z_back ) );
    v->push_back( osg::Vec3( x + w ,  -h , _z_back ) );
    v->push_back( osg::Vec3( x + w ,  -d , _z_back ) );
    v->push_back( osg::Vec3( x     , 0.0 , _z_back ) );

    v->push_back( osg::Vec3( x     , 0.0 , _z_back ) );
    v->push_back( osg::Vec3( x + w ,   d , _z_back ) );
    v->push_back( osg::Vec3( x + w ,   h , _z_back ) );
    v->push_back( osg::Vec3( x     ,   h , _z_back ) );

    geometry->setVertexArray( v.get() );
    geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::QUADS, 0, v->size() ) );

    geometry->setColorArray( c.get() );
    geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

    geode->addDrawable( geometry.get() );
}

////////////////////////////////////////////////////////////////////////////////

void VSI::createBoxSelect()
{
    const double x = 67.1;
    const double y = 31.0;
    const double h = 6.0;
    const double w = 12.0;

    const double delta_x_txt = 1.0;
    const double delta_y_txt = 1.7;

    osg::ref_ptr<osg::Geode> geode_b = new osg::Geode();
    osg::ref_ptr<osg::Geode> geode_f = new osg::Geode();
    osg::ref_ptr<osg::Geode> geode_t = new osg::Geode();

    _pat->addChild( geode_b.get() );
    _pat->addChild( geode_f.get() );
    _pat->addChild( geode_t.get() );

    osg::ref_ptr<osg::Geometry> geom_b = new osg::Geometry();
    osg::ref_ptr<osg::Geometry> geom_f = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> vb = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec3Array> vf = new osg::Vec3Array();

    osg::ref_ptr<osg::Vec4Array> cb = new osg::Vec4Array();
    osg::ref_ptr<osg::Vec4Array> cf = new osg::Vec4Array();

    cb->push_back( osg::Vec4( Colors::_black, 1.0 ) );
    cf->push_back( osg::Vec4( Colors::_white, 1.0 ) );

    vf->push_back( osg::Vec3( x     , y     , _z_box ) );
    vf->push_back( osg::Vec3( x + w , y     , _z_box ) );
    vf->push_back( osg::Vec3( x + w , y + h , _z_box ) );
    vf->push_back( osg::Vec3( x     , y + h , _z_box ) );

    vb->push_back( osg::Vec3( x     , y     , _z_box + 1.0 ) );
    vb->push_back( osg::Vec3( x + w , y     , _z_box + 1.0 ) );
    vb->push_back( osg::Vec3( x + w , y + h , _z_box + 1.0 ) );
    vb->push_back( osg::Vec3( x     , y + h , _z_box + 1.0 ) );

    geom_b->setVertexArray( vb.get() );
    geom_f->setVertexArray( vf.get() );

    geom_b->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::QUADS, 0, vb->size() ) );
    geom_f->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINE_LOOP, 0, vf->size() ) );

    geom_b->setColorArray( cb.get() );
    geom_f->setColorArray( cf.get() );

    geom_b->setColorBinding( osg::Geometry::BIND_OVERALL );
    geom_f->setColorBinding( osg::Geometry::BIND_OVERALL );

    geode_b->addDrawable( geom_b.get() );
    geode_f->addDrawable( geom_f.get() );

    _textSelect = new osgText::Text();
    _textSelect->setFont( Fonts::get( "fonts/g1000.ttf" ) );
    _textSelect->setColor( osg::Vec4( Colors::_cyan, 1.0 ) );
    _textSelect->setCharacterSize( 4.0 );
    _textSelect->setAxisAlignment( osgText::TextBase::XY_PLANE );
    _textSelect->setPosition( osg::Vec3( x + w - delta_x_txt, y + delta_y_txt, _z_box + 2.0 ) );
    _textSelect->setLayout( osgText::Text::LEFT_TO_RIGHT );
    _textSelect->setAlignment( osgText::Text::RIGHT_BASE_LINE );
    _textSelect->setText( "0000" );
    geode_t->addDrawable( _textSelect );
}

////////////////////////////////////////////////////////////////////////////////

void VSI::createBug()
{
    const double dx = 67.0;

    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    _patBug->addChild( geode.get() );

    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();

    v->push_back( osg::Vec3( dx       ,  0.0, _z_bug ) );
    v->push_back( osg::Vec3( dx       ,  2.0, _z_bug ) );
    v->push_back( osg::Vec3( dx + 2.0 ,  1.0, _z_bug ) );
    v->push_back( osg::Vec3( dx       ,  2.0, _z_bug ) );
    v->push_back( osg::Vec3( dx + 2.0 ,  1.0, _z_bug ) );
    v->push_back( osg::Vec3( dx + 2.0 ,  2.0, _z_bug ) );

    v->push_back( osg::Vec3( dx       ,  0.0, _z_bug ) );
    v->push_back( osg::Vec3( dx       , -2.0, _z_bug ) );
    v->push_back( osg::Vec3( dx + 2.0 , -1.0, _z_bug ) );
    v->push_back( osg::Vec3( dx       , -2.0, _z_bug ) );
    v->push_back( osg::Vec3( dx + 2.0 , -1.0, _z_bug ) );
    v->push_back( osg::Vec3( dx + 2.0 , -2.0, _z_bug ) );

    c->push_back( osg::Vec4( Colors::_cyan, 1.0 ) );

    geom->setVertexArray( v.get() );
    geom->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::TRIANGLES, 0, v->size() ) );

    geom->setColorArray( c.get() );
    geom->setColorBinding( osg::Geometry::BIND_OVERALL );

    geode->addDrawable( geom.get() );
}

////////////////////////////////////////////////////////////////////////////////

void VSI::createFrame()
{
    const double x = 67.0;
    const double h = 31.0;
    const double w = 9.0;
    const double d = 4.5;

    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    _pat->addChild( geode.get() );

    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();

    c->push_back( osg::Vec4( Colors::_white, 1.0 ) );

    v->push_back( osg::Vec3( x     ,  -h , _z_frame ) );
    v->push_back( osg::Vec3( x + w ,  -h , _z_frame ) );
    v->push_back( osg::Vec3( x + w ,  -d , _z_frame ) );
    v->push_back( osg::Vec3( x     , 0.0 , _z_frame ) );

    v->push_back( osg::Vec3( x     , 0.0 , _z_frame ) );
    v->push_back( osg::Vec3( x + w ,   d , _z_frame ) );
    v->push_back( osg::Vec3( x + w ,   h , _z_frame ) );
    v->push_back( osg::Vec3( x     ,   h , _z_frame ) );

    geometry->setVertexArray( v.get() );
    geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINE_LOOP, 0, v->size() ) );

    geometry->setColorArray( c.get() );
    geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

    geode->addDrawable( geometry.get() );
}

////////////////////////////////////////////////////////////////////////////////

void VSI::createPointer()
{
    const double dx = 67.0;

    const double zf = _z_ptr;
    const double zb = _z_ptr + 1.0;;

    osg::ref_ptr<osg::Geode> geode_f = new osg::Geode();
    osg::ref_ptr<osg::Geode> geode_b = new osg::Geode();

    _patPointer->addChild( geode_f.get() );
    _patPointer->addChild( geode_b.get() );

    osg::ref_ptr<osg::Geometry> geom_f = new osg::Geometry();
    osg::ref_ptr<osg::Geometry> geom_b = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> vf = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec4Array> cf = new osg::Vec4Array();

    osg::ref_ptr<osg::Vec3Array> vb = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec4Array> cb = new osg::Vec4Array();

    vf->push_back( osg::Vec3( dx        ,  0.0, zf ) );
    vf->push_back( osg::Vec3( dx +  4.0 ,  2.0, zf ) );
    vf->push_back( osg::Vec3( dx +  4.0 , -2.0, zf ) );

    vf->push_back( osg::Vec3( dx +  4.0 ,  2.0, zf ) );
    vf->push_back( osg::Vec3( dx +  4.0 , -2.0, zf ) );
    vf->push_back( osg::Vec3( dx + 13.0 , -2.0, zf ) );

    vf->push_back( osg::Vec3( dx +  4.0 ,  2.0, zf ) );
    vf->push_back( osg::Vec3( dx + 13.0 ,  2.0, zf ) );
    vf->push_back( osg::Vec3( dx + 13.0 , -2.0, zf ) );;

    cf->push_back( osg::Vec4( Colors::_black, 1.0 ) );

    vb->push_back( osg::Vec3( dx        ,  0.0, zb ) );
    vb->push_back( osg::Vec3( dx +  4.0 ,  2.0, zb ) );
    vb->push_back( osg::Vec3( dx +  4.0 ,  2.0, zb ) );
    vb->push_back( osg::Vec3( dx + 13.0 ,  2.0, zb ) );
    vb->push_back( osg::Vec3( dx +  4.0 ,  2.0, zb ) );
    vb->push_back( osg::Vec3( dx + 13.0 ,  2.0, zb ) );

    vb->push_back( osg::Vec3( dx        ,  0.0, zb ) );
    vb->push_back( osg::Vec3( dx +  4.0 , -2.0, zb ) );
    vb->push_back( osg::Vec3( dx +  4.0 , -2.0, zb ) );
    vb->push_back( osg::Vec3( dx + 13.0 , -2.0, zb ) );
    vb->push_back( osg::Vec3( dx +  4.0 , -2.0, zb ) );
    vb->push_back( osg::Vec3( dx + 13.0 , -2.0, zb ) );

    vb->push_back( osg::Vec3( dx + 13.0 ,  2.0, zb ) );
    vb->push_back( osg::Vec3( dx + 13.0 , -2.0, zb ) );

    cb->push_back( osg::Vec4( Colors::_white, 1.0 ) );

    geom_f->setVertexArray( vf.get() );
    geom_f->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::TRIANGLES, 0, vf->size() ) );

    geom_f->setColorArray( cf.get() );
    geom_f->setColorBinding( osg::Geometry::BIND_OVERALL );

    geom_b->setVertexArray( vb.get() );
    geom_b->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, vb->size() ) );

    geom_b->setColorArray( cb.get() );
    geom_b->setColorBinding( osg::Geometry::BIND_OVERALL );

    geode_f->addDrawable( geom_f.get() );
    geode_b->addDrawable( geom_b.get() );

    osg::ref_ptr<osg::Geode> geode_t = new osg::Geode();

    _patPointer->addChild( geode_t.get() );

    _textVS = new osgText::Text();
    _textVS->setFont( Fonts::get( "fonts/g1000.ttf" ) );
    _textVS->setColor( osg::Vec4( Colors::_white, 1.0 ) );
    _textVS->setCharacterSize( 4.0 );
    _textVS->setAxisAlignment( osgText::TextBase::XY_PLANE );
    _textVS->setPosition( osg::Vec3( 79.8, 0.0, _z_box + 4.0 ) );
    _textVS->setLayout( osgText::Text::LEFT_TO_RIGHT );
    _textVS->setAlignment( osgText::Text::RIGHT_CENTER );
    _textVS->setText( "0000" );
    geode_t->addDrawable( _textVS );
}

////////////////////////////////////////////////////////////////////////////////

void VSI::createScale()
{
    const double x = 67.0;
    const double w1 = 3.0;
    const double w2 = 2.0;

    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    _pat->addChild( geode.get() );

    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();

    c->push_back( osg::Vec4( Colors::_white, 1.0 ) );

    const double step = 500.0;
    const double min = -2000.0;
    const double max =  2000.0;
    const int steps = ( max - min ) / step + 1;

    for ( int i = 0; i < steps; i++ )
    {
        double vs = min + i * step;

        if ( fabs( vs ) > 1.0e-9 )
        {
            double x1 = x + w2;
            double y = _vs2pt * vs;

            if ( i % 2 == 0 )
            {
                x1 = x + w1;

                std::stringstream alt_str;
                alt_str << floor( fabs( vs / 1000 ) );

                osg::ref_ptr<osgText::Text> text = new osgText::Text();
                text->setFont( Fonts::get( "fonts/g1000.ttf" ) );
                text->setColor( osg::Vec4( Colors::_white, 1.0 ) );
                text->setCharacterSize( 4.0 );
                text->setAxisAlignment( osgText::TextBase::XY_PLANE );
                text->setPosition( osg::Vec3( x1 + 1.0, y, _z_scale ) );
                text->setLayout( osgText::Text::LEFT_TO_RIGHT );
                text->setAlignment( osgText::Text::LEFT_CENTER );
                text->setText( alt_str.str() );
                geode->addDrawable( text );
            }

            v->push_back( osg::Vec3( x  , y, _z_scale ) );
            v->push_back( osg::Vec3( x1 , y, _z_scale ) );
        }
    }

    geometry->setVertexArray( v.get() );
    geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, v->size() ) );

    geometry->setColorArray( c.get() );
    geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

    geode->addDrawable( geometry.get() );
}
