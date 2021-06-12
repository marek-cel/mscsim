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

#include <g1000/cgi/g1000_ALT.h>

#include <iomanip>
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

const double ALT::_x_offset = -10.0;
const double ALT::_y_offset =  19.0;

const osg::Vec3 ALT::_colorBack = osg::Vec3( 0.38, 0.38, 0.38 );

const double ALT::_z_back  = -80.0;
const double ALT::_z_box   = -60.0;
const double ALT::_z_bug   = -69.0;
const double ALT::_z_frame = -60.0;
const double ALT::_z_scale = -70.0;

const double ALT::_alt2pt = 0.11;

const int ALT::_depth_sorted_bin_scale = 90;
const int ALT::_depth_sorted_bin_disk = 100;

////////////////////////////////////////////////////////////////////////////////

ALT::ALT( IFD *ifd ) :
    Module( ifd )
{
    _pat = new osg::PositionAttitudeTransform();
    _root->addChild( _pat.get() );
    _pat->setPosition( osg::Vec3( _x_offset, _y_offset, 0.0 ) );

    _patDisk = new osg::PositionAttitudeTransform();
    _pat->addChild( _patDisk.get() );

    _patScale = new osg::PositionAttitudeTransform();
    _pat->addChild( _patScale.get() );

    _patBug = new osg::PositionAttitudeTransform();
    _pat->addChild( _patBug.get() );

    createAltitudeBug();
    createBack();
    createBox();
    createBoxSelect();
    createBoxPress();
    createDisk();
    createDiskMask();
    createFrame();
    createScale();
    createScaleMask();
}

////////////////////////////////////////////////////////////////////////////////

ALT::~ALT() {}

////////////////////////////////////////////////////////////////////////////////

void ALT::update()
{
    double altitude_ft = Units::m2ft( _ifd->gdc()->getAltitude() );
    double pressure_in = Units::pa2inhg( _ifd->input().ref_pressure );
    double sel_altitude_ft = Units::m2ft( _ifd->input().sel_altitude );

    double dy_bug = Misc::satur( -33.0, 33.0, -_alt2pt * ( altitude_ft - sel_altitude_ft ) );

    _patScale->setPosition( osg::Vec3( 0.0, -_alt2pt * altitude_ft, 0.0 ) );
    _patBug->setPosition( osg::Vec3( 0.0, dy_bug, 0.0 ) );

    int alt_100 = floor( altitude_ft / 100.0 );

    std::stringstream alt_str;
    if ( fabs( altitude_ft ) < 100.0 )
    {
        if ( altitude_ft < 0.0 ) alt_str << "-";
    }
    else
    {
        alt_str << alt_100;
    }

    _textAlt_100->setText( alt_str.str() );

    double a = 0.5 * osg::DegreesToRadians( 3.6 ) * ( altitude_ft - 100.0 * alt_100 );
    _patDisk->setAttitude( osg::Quat( a, osg::X_AXIS ) );

    std::stringstream press_str;

    press_str.setf( std::ios_base::showpoint );
    press_str.setf( std::ios_base::fixed );

    press_str << std::setprecision( 2 ) << pressure_in << " IN";
    _textPress->setText( press_str.str() );

    std::stringstream sel_alt_str;

    sel_alt_str.setf( std::ios_base::showpoint );
    sel_alt_str.setf( std::ios_base::fixed );

    sel_alt_str << std::setprecision( 2 ) << sel_altitude_ft;
    _textSelect->setText( sel_alt_str.str() );
}

////////////////////////////////////////////////////////////////////////////////

void ALT::createAltitudeBug()
{
    const double dx = 47.5;

    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    _patBug->addChild( geode.get() );

    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();

    v->push_back( osg::Vec3( dx - 0.5 ,  0.0, _z_bug ) );
    v->push_back( osg::Vec3( dx       ,  0.0, _z_bug ) );
    v->push_back( osg::Vec3( dx - 0.5 ,  3.0, _z_bug ) );
    v->push_back( osg::Vec3( dx       ,  0.0, _z_bug ) );
    v->push_back( osg::Vec3( dx - 0.5 ,  3.0, _z_bug ) );
    v->push_back( osg::Vec3( dx + 2.0 ,  2.0, _z_bug ) );
    v->push_back( osg::Vec3( dx - 0.5 ,  3.0, _z_bug ) );
    v->push_back( osg::Vec3( dx + 2.0 ,  2.0, _z_bug ) );
    v->push_back( osg::Vec3( dx + 2.0 ,  3.0, _z_bug ) );

    v->push_back( osg::Vec3( dx - 0.5 ,  0.0, _z_bug ) );
    v->push_back( osg::Vec3( dx       ,  0.0, _z_bug ) );
    v->push_back( osg::Vec3( dx - 0.5 , -3.0, _z_bug ) );
    v->push_back( osg::Vec3( dx       ,  0.0, _z_bug ) );
    v->push_back( osg::Vec3( dx - 0.5 , -3.0, _z_bug ) );
    v->push_back( osg::Vec3( dx + 2.0 , -2.0, _z_bug ) );
    v->push_back( osg::Vec3( dx - 0.5 , -3.0, _z_bug ) );
    v->push_back( osg::Vec3( dx + 2.0 , -2.0, _z_bug ) );
    v->push_back( osg::Vec3( dx + 2.0 , -3.0, _z_bug ) );

    c->push_back( osg::Vec4( Colors::_cyan, 1.0 ) );

    geom->setVertexArray( v.get() );
    geom->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::TRIANGLES, 0, v->size() ) );

    geom->setColorArray( c.get() );
    geom->setColorBinding( osg::Geometry::BIND_OVERALL );

    geode->addDrawable( geom.get() );

    osg::ref_ptr<osg::StateSet> stateSet = geode->getOrCreateStateSet();
    stateSet->setRenderBinDetails( _depth_sorted_bin_scale, "RenderBin" );
}

////////////////////////////////////////////////////////////////////////////////

void ALT::createBack()
{
    const double x = 47.0;
    const double h = 33.0;
    const double w = 20.0;

    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    _pat->addChild( geode.get() );

    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();

    c->push_back( osg::Vec4( _colorBack, 0.5 ) );

    v->push_back( osg::Vec3( x     , -h , _z_back ) );
    v->push_back( osg::Vec3( x + w , -h , _z_back ) );
    v->push_back( osg::Vec3( x + w ,  h , _z_back ) );
    v->push_back( osg::Vec3( x     ,  h , _z_back ) );

    geometry->setVertexArray( v.get() );
    geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::QUADS, 0, v->size() ) );

    geometry->setColorArray( c.get() );
    geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

    geode->addDrawable( geometry.get() );
}

////////////////////////////////////////////////////////////////////////////////

void ALT::createBox()
{
    const double dx = 47.5;

    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    _pat->addChild( geode.get() );

    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();

    v->push_back( osg::Vec3( dx        ,  0.0, _z_box ) );
    v->push_back( osg::Vec3( dx +  2.0 ,  2.0, _z_box ) );
    v->push_back( osg::Vec3( dx +  2.0 , -2.0, _z_box ) );

    v->push_back( osg::Vec3( dx +  2.0 ,  4.0, _z_box ) );
    v->push_back( osg::Vec3( dx +  2.0 , -4.0, _z_box ) );
    v->push_back( osg::Vec3( dx + 12.5 , -4.0, _z_box ) );

    v->push_back( osg::Vec3( dx +  2.0 ,  4.0, _z_box ) );
    v->push_back( osg::Vec3( dx + 12.5 ,  4.0, _z_box ) );
    v->push_back( osg::Vec3( dx + 12.5 , -4.0, _z_box ) );

    v->push_back( osg::Vec3( dx + 12.5 ,  6.5, _z_box ) );
    v->push_back( osg::Vec3( dx + 12.5 , -6.5, _z_box ) );
    v->push_back( osg::Vec3( dx + 18.5 , -6.5, _z_box ) );

    v->push_back( osg::Vec3( dx + 12.5 ,  6.5, _z_box ) );
    v->push_back( osg::Vec3( dx + 18.5 ,  6.5, _z_box ) );
    v->push_back( osg::Vec3( dx + 18.5 , -6.5, _z_box ) );

    c->push_back( osg::Vec4( Colors::_black, 1.0 ) );

    geom->setVertexArray( v.get() );
    geom->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::TRIANGLES, 0, v->size() ) );

    geom->setColorArray( c.get() );
    geom->setColorBinding( osg::Geometry::BIND_OVERALL );

    geode->addDrawable( geom.get() );

    // text
    osg::ref_ptr<osg::Geode> geode_t = new osg::Geode();
    _pat->addChild( geode_t.get() );

    _textAlt_100 = new osgText::Text();
    _textAlt_100->setFont( Fonts::get( "fonts/g1000.ttf" ) );
    _textAlt_100->setColor( osg::Vec4( Colors::_white, 1.0 ) );
    _textAlt_100->setCharacterSize( 5.0 );
    _textAlt_100->setAxisAlignment( osgText::TextBase::XY_PLANE );
    _textAlt_100->setPosition( osg::Vec3( 59.5, 0.0, _z_box + 1.0 ) );
    _textAlt_100->setLayout( osgText::Text::LEFT_TO_RIGHT );
    _textAlt_100->setAlignment( osgText::Text::RIGHT_CENTER );
    _textAlt_100->setText( "000" );
    geode_t->addDrawable( _textAlt_100 );
}

////////////////////////////////////////////////////////////////////////////////

void ALT::createBoxSelect()
{
    const double x = 47.0;
    const double y = 33.0;
    const double h = 6.0;
    const double w = 20.0;

    const double delta_x_txt = 4.0;
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
    _textSelect->setText( "00000" );
    geode_t->addDrawable( _textSelect );
}

////////////////////////////////////////////////////////////////////////////////

void ALT::createBoxPress()
{
    const double x = 47.0;
    const double y = -33.0;
    const double h = 6.0;
    const double w = 20.0;

    const double delta_y_txt = 1.5;

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

    vf->push_back( osg::Vec3( x     , y - h , _z_box ) );
    vf->push_back( osg::Vec3( x + w , y - h , _z_box ) );
    vf->push_back( osg::Vec3( x + w , y     , _z_box ) );
    vf->push_back( osg::Vec3( x     , y     , _z_box ) );

    vb->push_back( osg::Vec3( x     , y - h , _z_box + 1.0 ) );
    vb->push_back( osg::Vec3( x + w , y - h , _z_box + 1.0 ) );
    vb->push_back( osg::Vec3( x + w , y     , _z_box + 1.0 ) );
    vb->push_back( osg::Vec3( x     , y     , _z_box + 1.0 ) );

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

    _textPress = new osgText::Text();
    _textPress->setFont( Fonts::get( "fonts/g1000.ttf" ) );
    _textPress->setColor( osg::Vec4( Colors::_cyan, 1.0 ) );
    _textPress->setCharacterSize( 3.5 );
    _textPress->setAxisAlignment( osgText::TextBase::XY_PLANE );
    _textPress->setPosition( osg::Vec3( x + 0.5 * w, y - h + delta_y_txt, _z_box + 2.0 ) );
    _textPress->setLayout( osgText::Text::LEFT_TO_RIGHT );
    _textPress->setAlignment( osgText::Text::CENTER_BASE_LINE );
    _textPress->setText( "29.91 IN" );
    geode_t->addDrawable( _textPress );
}

////////////////////////////////////////////////////////////////////////////////

void ALT::createDisk()
{
    const double dx = 60.0;

    _patDisk->setPosition( osg::Vec3( dx, 0.0, _z_box + 1.0 ) );

    osg::ref_ptr<osg::StateSet> stateSet = _patDisk->getOrCreateStateSet();
    stateSet->setRenderBinDetails( _depth_sorted_bin_disk, "RenderBin" );

    for ( int i = 0; i <= 9; i++ )
    {
        osg::ref_ptr<osg::PositionAttitudeTransform> pat = new osg::PositionAttitudeTransform();
        _patDisk->addChild( pat.get() );

        osg::ref_ptr<osg::Geode> geode = new osg::Geode();
        pat->addChild( geode.get() );

        double a = osg::DegreesToRadians( 36.0 ) * i;

        pat->setAttitude( osg::Quat( -a, osg::X_AXIS ) );

        int d = 20*i;

        if ( d >= 100 ) d -= 100;

        std::stringstream digit;
        if ( i == 0 || i == 5 )
            digit << "00";
        else
            digit << d;

        osg::ref_ptr<osgText::Text> text = new osgText::Text();
        text->setFont( Fonts::get( "fonts/g1000.ttf" ) );
        text->setColor( osg::Vec4( Colors::_white, 1.0 ) );
        text->setCharacterSize( 5.0 );
        text->setAxisAlignment( osgText::TextBase::XY_PLANE );
        text->setPosition( osg::Vec3( 0.0, 0.0, 9.0 ) );
        text->setLayout( osgText::Text::LEFT_TO_RIGHT );
        text->setAlignment( osgText::Text::LEFT_CENTER );
        text->setText( digit.str() );
        geode->addDrawable( text );
    }

    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    _pat->addChild( geode.get() );

    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();

    c->push_back( osg::Vec4( Colors::_black, 1.0 ) );

    v->push_back( osg::Vec3( dx       , -6.5, _z_box + 1.0 ) );
    v->push_back( osg::Vec3( dx + 6.0 , -6.5, _z_box + 1.0 ) );
    v->push_back( osg::Vec3( dx + 6.0 ,  6.5, _z_box + 1.0 ) );
    v->push_back( osg::Vec3( dx       ,  6.5, _z_box + 1.0 ) );

    geometry->setVertexArray( v.get() );
    geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::QUADS, 0, v->size() ) );

    geometry->setColorArray( c.get() );
    geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

    geode->addDrawable( geometry.get() );
}

////////////////////////////////////////////////////////////////////////////////

void ALT::createDiskMask()
{
    const double dy = 6.5;
    const double x = 60.0;
    const double w = 6.0;
    const double h = 10.0;

    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    _pat->addChild( geode.get() );

    osg::ref_ptr<osg::StateSet> stateSet = geode->getOrCreateStateSet();
    stateSet->setRenderBinDetails( _depth_sorted_bin_disk - 1, "RenderBin" );

    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();

    c->push_back( osg::Vec4( 0.0, 0.0, 0.0, 0.0 ) );

    v->push_back( osg::Vec3( x     ,  dy     , _z_box + 10.0 ) );
    v->push_back( osg::Vec3( x + w ,  dy     , _z_box + 10.0 ) );
    v->push_back( osg::Vec3( x + w ,  dy + h , _z_box + 10.0 ) );
    v->push_back( osg::Vec3( x     ,  dy + h , _z_box + 10.0 ) );

    v->push_back( osg::Vec3( x     , -dy - h , _z_box + 10.0 ) );
    v->push_back( osg::Vec3( x + w , -dy - h , _z_box + 10.0 ) );
    v->push_back( osg::Vec3( x + w , -dy     , _z_box + 10.0 ) );
    v->push_back( osg::Vec3( x     , -dy     , _z_box + 10.0 ) );

    geometry->setVertexArray( v.get() );
    geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::QUADS, 0, v->size() ) );

    geometry->setColorArray( c.get() );
    geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

    geode->addDrawable( geometry.get() );
}

////////////////////////////////////////////////////////////////////////////////

void ALT::createFrame()
{
    const double x = 47.0;
    const double h = 33.0;
    const double w = 20.0;

    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    _pat->addChild( geode.get() );

    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();

    c->push_back( osg::Vec4( Colors::_white, 1.0 ) );

    v->push_back( osg::Vec3( x     , -h , _z_frame ) );
    v->push_back( osg::Vec3( x + w , -h , _z_frame ) );
    v->push_back( osg::Vec3( x + w ,  h , _z_frame ) );
    v->push_back( osg::Vec3( x     ,  h , _z_frame ) );

    geometry->setVertexArray( v.get() );
    geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINE_LOOP, 0, v->size() ) );

    geometry->setColorArray( c.get() );
    geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

    geode->addDrawable( geometry.get() );
}

////////////////////////////////////////////////////////////////////////////////

void ALT::createScale()
{
    const double x = 47.0;
    const double w1 = 4.0;
    const double w2 = 2.0;

    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    _patScale->addChild( geode.get() );

    osg::ref_ptr<osg::StateSet> stateSet = geode->getOrCreateStateSet();
    stateSet->setRenderBinDetails( _depth_sorted_bin_scale, "RenderBin" );

    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();

    c->push_back( osg::Vec4( Colors::_white, 1.0 ) );

    const double step = 20.0;
    const double min = -1000.0;
    const double max = 30000.0;
    const int steps = ( max - min ) / step + 1;

    for ( int i = 0; i < steps; i++ )
    {
        double alt = min + i * step;

        double x1 = x + w2;
        double y = _alt2pt * alt;

        if ( i % 5 == 0 )
        {
            x1 = x + w1;

            std::stringstream alt_str;
            alt_str << alt;

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

    geometry->setVertexArray( v.get() );
    geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, v->size() ) );

    geometry->setColorArray( c.get() );
    geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

    geode->addDrawable( geometry.get() );
}

////////////////////////////////////////////////////////////////////////////////

void ALT::createScaleMask()
{
    const double x  = 47.0;
    const double dy = 33.0;
    const double h1 = 23.0;
    const double h2 = 61.0;
    const double w  = 20.0;

    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    _pat->addChild( geode.get() );

    osg::ref_ptr<osg::StateSet> stateSet = geode->getOrCreateStateSet();
    stateSet->setRenderBinDetails( _depth_sorted_bin_scale - 1, "RenderBin" );

    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();

    c->push_back( osg::Vec4( 0.0, 0.0, 0.0, 0.0 ) );

    v->push_back( osg::Vec3( x     , dy      , _z_scale + 1.0 ) );
    v->push_back( osg::Vec3( x + w , dy      , _z_scale + 1.0 ) );
    v->push_back( osg::Vec3( x + w , dy + h1 , _z_scale + 1.0 ) );
    v->push_back( osg::Vec3( x     , dy + h1 , _z_scale + 1.0 ) );

    v->push_back( osg::Vec3( x     , -dy - h2 , _z_scale + 1.0 ) );
    v->push_back( osg::Vec3( x + w , -dy - h2 , _z_scale + 1.0 ) );
    v->push_back( osg::Vec3( x + w , -dy      , _z_scale + 1.0 ) );
    v->push_back( osg::Vec3( x     , -dy      , _z_scale + 1.0 ) );

    geometry->setVertexArray( v.get() );
    geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::QUADS, 0, v->size() ) );

    geometry->setColorArray( c.get() );
    geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

    geode->addDrawable( geometry.get() );
}

