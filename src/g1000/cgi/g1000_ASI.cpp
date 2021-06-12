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

#include <g1000/cgi/g1000_ASI.h>

#ifdef _MSC_VER
#   include <algorithm>
#endif

#include <iomanip>
#include <sstream>

#include <osg/Geode>
#include <osg/Geometry>

#include <g1000/g1000_Defines.h>
#include <g1000/g1000_Log.h>

#include <g1000/cgi/g1000_Colors.h>
#include <g1000/cgi/g1000_Fonts.h>

#include <g1000/utils/g1000_Misc.h>
#include <g1000/utils/g1000_Units.h>

#include <g1000/xml/g1000_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace g1000;

////////////////////////////////////////////////////////////////////////////////

const double ASI::_x_offset = -10.0;
const double ASI::_y_offset =  19.0;

const osg::Vec3 ASI::_colorBack = osg::Vec3( 0.38, 0.38, 0.38 );

const double ASI::_z_back  = -80.0;
const double ASI::_z_box   = -60.0;
const double ASI::_z_bug   = -69.0;
const double ASI::_z_frame = -60.0;
const double ASI::_z_scale = -70.0;

const double ASI::_ias2pt = 1.1;

const double ASI::_ias_min   = 20.0;
const double ASI::_ias_red   = 40.0;
const double ASI::_ias_stall = 48.0;
const double ASI::_ias_white = 85.0;
const double ASI::_ias_green = 130.0;
const double ASI::_ias_vne   = 164.0;

const int ASI::_depth_sorted_bin_scale = 90;
const int ASI::_depth_sorted_bin_disk = 100;

////////////////////////////////////////////////////////////////////////////////

ASI::ASI( IFD *ifd, XmlNode &node ) :
    Module( ifd )
{
    _pat = new osg::PositionAttitudeTransform();
    _root->addChild( _pat.get() );
    _pat->setPosition( osg::Vec3( _x_offset, _y_offset, 0.0 ) );

    _switchIAS = new osg::Switch();
    _pat->addChild( _switchIAS.get() );

    _groupIAS = new osg::Group();
    _switchIAS->addChild( _groupIAS.get() );

    _patDisk = new osg::PositionAttitudeTransform();
    _groupIAS->addChild( _patDisk.get() );

    _patScale = new osg::PositionAttitudeTransform();
    _pat->addChild( _patScale.get() );

    _patBug = new osg::PositionAttitudeTransform();
    _pat->addChild( _patBug.get() );

    _groupRed = new osg::Group();
    _pat->addChild( _groupRed.get() );

    _materialBlack = new osg::Material();
    _materialBlack->setColorMode( osg::Material::AMBIENT_AND_DIFFUSE );
    _materialBlack->setAmbient( osg::Material::FRONT_AND_BACK, osg::Vec4( Colors::_black, 1.0 ) );
    _materialBlack->setDiffuse( osg::Material::FRONT_AND_BACK, osg::Vec4( Colors::_black, 1.0 ) );

    _materialRed = new osg::Material();
    _materialRed->setColorMode( osg::Material::AMBIENT_AND_DIFFUSE );
    _materialRed->setAmbient( osg::Material::FRONT_AND_BACK, osg::Vec4( Colors::_red, 1.0 ) );
    _materialRed->setDiffuse( osg::Material::FRONT_AND_BACK, osg::Vec4( Colors::_red, 1.0 ) );

    if ( node.isValid() )
    {
        // TODO
    }
    else
    {
        Log::e() << ( "Reading XML file failed. " + XmlUtils::getErrorInfo( node ) ) << std::endl;
    }

    createAirspeedBug();
    createBack();
    createBarRed();
    createBarGreen();
    createBarWhite();
    createBarYellow();
    createBarVne();
    createBox();
    createBoxSelect();
    createBoxTAS();
    createDisk();
    createDiskMask();
    createFrame();
    createScale();
    createScaleMask();
}

////////////////////////////////////////////////////////////////////////////////

ASI::~ASI() {}

////////////////////////////////////////////////////////////////////////////////

void ASI::update()
{
    double ias_kts = Units::mps2kts( _ifd->gdc()->getIAS() );
    double tas_kts = Units::mps2kts( _ifd->gdc()->getTAS() );
    double sel_kts = Units::mps2kts( _ifd->input().sel_airspeed );
    double ias_min = std::max( _ias_min, ias_kts );
    double sel_min = std::max( _ias_min, sel_kts );

    double dy_bug = Misc::satur( -33.0, 33.0, -_ias2pt * ( ias_min - sel_min ) );

    _patScale->setPosition( osg::Vec3( 0.0, -_ias2pt * ias_min, 0.0 ) );
    _patBug->setPosition( osg::Vec3( 0.0, dy_bug, 0.0 ) );

    std::stringstream ias_str;
    std::stringstream tas_str;

    int ias_10 = floor( ias_kts / 10.0 );

    if ( ias_kts > _ias_min )
    {
        _switchIAS->setValue( 0, true );
        _switchIAS->setValue( 1, false );

        int tas = floor( tas_kts + 0.5 );

        ias_str << ias_10;
        tas_str << tas << "KT";

        double a = osg::DegreesToRadians( 36.0 ) * ( ias_kts - 10.0 * ias_10 );
        _patDisk->setAttitude( osg::Quat( a, osg::X_AXIS ) );
    }
    else
    {
        _switchIAS->setValue( 0, false );
        _switchIAS->setValue( 1, true );
    }

    _textIAS_10->setText( ias_str.str() );
    _textTAS->setText( tas_str.str() );

    if ( _groupRed.valid() )
    {
        if ( ias_kts > _ias_vne )
        {
            _groupRed->getOrCreateStateSet()->setAttribute( _materialRed.get(),
                osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON );
        }
        else
        {
            _groupRed->getOrCreateStateSet()->setAttribute( _materialBlack.get(),
                osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON );
        }
    }

    std::stringstream sel_ias_str;

    sel_ias_str.setf( std::ios_base::showpoint );
    sel_ias_str.setf( std::ios_base::fixed );

    sel_ias_str << std::setprecision( 0 ) << sel_kts;
    _textSelect->setText( sel_ias_str.str() );
}

////////////////////////////////////////////////////////////////////////////////

void ASI::createAirspeedBug()
{
    const double dx = -44.5;

    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    _patBug->addChild( geode.get() );

    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();

    v->push_back( osg::Vec3( dx + 0.5 ,  0.0, _z_bug ) );
    v->push_back( osg::Vec3( dx       ,  0.0, _z_bug ) );
    v->push_back( osg::Vec3( dx + 0.5 ,  3.0, _z_bug ) );
    v->push_back( osg::Vec3( dx       ,  0.0, _z_bug ) );
    v->push_back( osg::Vec3( dx + 0.5 ,  3.0, _z_bug ) );
    v->push_back( osg::Vec3( dx - 2.0 ,  2.0, _z_bug ) );
    v->push_back( osg::Vec3( dx + 0.5 ,  3.0, _z_bug ) );
    v->push_back( osg::Vec3( dx - 2.0 ,  2.0, _z_bug ) );
    v->push_back( osg::Vec3( dx - 2.0 ,  3.0, _z_bug ) );

    v->push_back( osg::Vec3( dx + 0.5 ,  0.0, _z_bug ) );
    v->push_back( osg::Vec3( dx       ,  0.0, _z_bug ) );
    v->push_back( osg::Vec3( dx + 0.5 , -3.0, _z_bug ) );
    v->push_back( osg::Vec3( dx       ,  0.0, _z_bug ) );
    v->push_back( osg::Vec3( dx + 0.5 , -3.0, _z_bug ) );
    v->push_back( osg::Vec3( dx - 2.0 , -2.0, _z_bug ) );
    v->push_back( osg::Vec3( dx + 0.5 , -3.0, _z_bug ) );
    v->push_back( osg::Vec3( dx - 2.0 , -2.0, _z_bug ) );
    v->push_back( osg::Vec3( dx - 2.0 , -3.0, _z_bug ) );

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

void ASI::createBack()
{
    const double x = -60.0;
    const double h = 33.0;
    const double w = 16.0;

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

void ASI::createBar( double x, double z, double min, double max, osg::Vec3 color )
{
    const double y_min = _ias2pt * min;
    const double y_max = _ias2pt * max;
    const double w = 1.2;

    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    _patScale->addChild( geode.get() );

    osg::ref_ptr<osg::StateSet> stateSet = geode->getOrCreateStateSet();
    stateSet->setRenderBinDetails( _depth_sorted_bin_scale, "RenderBin" );

    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();

    c->push_back( osg::Vec4( color, 1.0 ) );

    v->push_back( osg::Vec3( x     , y_min, z ) );
    v->push_back( osg::Vec3( x + w , y_min, z ) );
    v->push_back( osg::Vec3( x + w , y_max, z ) );
    v->push_back( osg::Vec3( x     , y_max, z ) );

    geometry->setVertexArray( v.get() );
    geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::QUADS, 0, v->size() ) );

    geometry->setColorArray( c.get() );
    geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

    geode->addDrawable( geometry.get() );
}

////////////////////////////////////////////////////////////////////////////////

void ASI::createBarRed()
{
    createBar( -45.2, _z_back + 2, _ias_min, _ias_red, Colors::_red );
}

////////////////////////////////////////////////////////////////////////////////

void ASI::createBarGreen()
{
    createBar( -45.2, _z_back + 2, _ias_stall, _ias_green, Colors::_green );
}

////////////////////////////////////////////////////////////////////////////////

void ASI::createBarWhite()
{
    createBar( -45.8, _z_back + 1, _ias_red, _ias_white, Colors::_white );
}

////////////////////////////////////////////////////////////////////////////////

void ASI::createBarYellow()
{
    createBar( -45.2, _z_back + 2, _ias_green, _ias_vne, Colors::_yellow );
}

////////////////////////////////////////////////////////////////////////////////

void ASI::createBarVne()
{
    createBar( -45.2, _z_back + 2, _ias_vne, _ias_vne + 600.0, Colors::_red );
}

////////////////////////////////////////////////////////////////////////////////

void ASI::createBox()
{
    const double dx = -44.5;

    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    _groupRed->addChild( geode.get() );

    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();

    v->push_back( osg::Vec3( dx        ,  0.0, _z_box ) );
    v->push_back( osg::Vec3( dx -  2.0 ,  2.0, _z_box ) );
    v->push_back( osg::Vec3( dx -  2.0 , -2.0, _z_box ) );

    v->push_back( osg::Vec3( dx -  2.0 ,  6.5, _z_box ) );
    v->push_back( osg::Vec3( dx -  6.0 ,  6.5, _z_box ) );
    v->push_back( osg::Vec3( dx -  2.0 , -6.5, _z_box ) );

    v->push_back( osg::Vec3( dx -  6.0 ,  6.5, _z_box ) );
    v->push_back( osg::Vec3( dx -  6.0 , -6.5, _z_box ) );
    v->push_back( osg::Vec3( dx -  2.0 , -6.5, _z_box ) );

    v->push_back( osg::Vec3( dx - 15.0 ,  4.0, _z_box ) );
    v->push_back( osg::Vec3( dx -  6.0 ,  4.0, _z_box ) );
    v->push_back( osg::Vec3( dx -  6.0 , -4.0, _z_box ) );

    v->push_back( osg::Vec3( dx - 15.0 ,  4.0, _z_box ) );
    v->push_back( osg::Vec3( dx - 15.0 , -4.0, _z_box ) );
    v->push_back( osg::Vec3( dx -  6.0 , -4.0, _z_box ) );

    c->push_back( osg::Vec4( Colors::_black, 1.0 ) );

    geom->setVertexArray( v.get() );
    geom->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::TRIANGLES, 0, v->size() ) );

    geode->addDrawable( geom.get() );

    osg::ref_ptr<osg::Geode> geode_t1 = new osg::Geode();
    osg::ref_ptr<osg::Geode> geode_t2 = new osg::Geode();
    _groupIAS->addChild( geode_t1.get() );
    _switchIAS->addChild( geode_t2.get() );

    _textIAS_10 = new osgText::Text();
    _textIAS_10->setFont( Fonts::get( "fonts/g1000.ttf" ) );
    _textIAS_10->setColor( osg::Vec4( Colors::_white, 1.0 ) );
    _textIAS_10->setCharacterSize( 5.0 );
    _textIAS_10->setAxisAlignment( osgText::TextBase::XY_PLANE );
    _textIAS_10->setPosition( osg::Vec3( -50.5, 0.0, _z_box + 1.0 ) );
    _textIAS_10->setLayout( osgText::Text::LEFT_TO_RIGHT );
    _textIAS_10->setAlignment( osgText::Text::RIGHT_CENTER );
    _textIAS_10->setText( "00" );
    geode_t1->addDrawable( _textIAS_10 );

    osg::ref_ptr<osgText::Text> textIAS = new osgText::Text();
    textIAS->setFont( Fonts::get( "fonts/g1000.ttf" ) );
    textIAS->setColor( osg::Vec4( Colors::_white, 1.0 ) );
    textIAS->setCharacterSize( 5.0 );
    textIAS->setAxisAlignment( osgText::TextBase::XY_PLANE );
    textIAS->setPosition( osg::Vec3( -48.5, 0.0, _z_box + 1.0 ) );
    textIAS->setLayout( osgText::Text::LEFT_TO_RIGHT );
    textIAS->setAlignment( osgText::Text::RIGHT_CENTER );
    textIAS->setText( "---" );
    geode_t2->addDrawable( textIAS );
}

////////////////////////////////////////////////////////////////////////////////

void ASI::createBoxSelect()
{
    const double x = -60.0;
    const double y =  33.0;
    const double h = 6.0;
    const double w = 16.0;

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
    _textSelect->setText( "000" );
    geode_t->addDrawable( _textSelect );
}

////////////////////////////////////////////////////////////////////////////////

void ASI::createBoxTAS()
{
    const double x = -60.0;
    const double y = -33.0;
    const double h = 6.0;
    const double w = 16.0;

    const double delta_x_txt = 0.2;
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

    osg::ref_ptr<osgText::Text> textLabel = new osgText::Text();
    textLabel->setFont( Fonts::get( "fonts/g1000.ttf" ) );
    textLabel->setColor( osg::Vec4( Colors::_white, 1.0 ) );
    textLabel->setCharacterSize( 2.7 );
    textLabel->setAxisAlignment( osgText::TextBase::XY_PLANE );
    textLabel->setPosition( osg::Vec3( x + delta_x_txt, y - h + delta_y_txt, _z_box + 2.0 ) );
    textLabel->setLayout( osgText::Text::LEFT_TO_RIGHT );
    textLabel->setAlignment( osgText::Text::LEFT_BASE_LINE );
    textLabel->setText( "TAS" );
    geode_t->addDrawable( textLabel );

    _textTAS = new osgText::Text();
    _textTAS->setFont( Fonts::get( "fonts/g1000.ttf" ) );
    _textTAS->setColor( osg::Vec4( Colors::_white, 1.0 ) );
    _textTAS->setCharacterSize( 3.5 );
    _textTAS->setAxisAlignment( osgText::TextBase::XY_PLANE );
    _textTAS->setPosition( osg::Vec3( x + w - delta_x_txt, y - h + delta_y_txt, _z_box + 2.0 ) );
    _textTAS->setLayout( osgText::Text::LEFT_TO_RIGHT );
    _textTAS->setAlignment( osgText::Text::RIGHT_BASE_LINE );
    _textTAS->setText( "000" );
    geode_t->addDrawable( _textTAS );
}

////////////////////////////////////////////////////////////////////////////////

void ASI::createDisk()
{
    const double dx = -50.5;

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

        std::stringstream digit;
        digit << i;

        osg::ref_ptr<osgText::Text> text = new osgText::Text();
        text->setFont( Fonts::get( "fonts/g1000.ttf" ) );
        text->setColor( osg::Vec4( Colors::_white, 1.0 ) );
        text->setCharacterSize( 5.0 );
        text->setAxisAlignment( osgText::TextBase::XY_PLANE );
        text->setPosition( osg::Vec3( 0.5, 0.0, 9.0 ) );
        text->setLayout( osgText::Text::LEFT_TO_RIGHT );
        text->setAlignment( osgText::Text::LEFT_CENTER );
        text->setText( digit.str() );
        geode->addDrawable( text );
    }

    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    _groupRed->addChild( geode.get() );

    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();

    v->push_back( osg::Vec3( dx       , -6.5, _z_box + 1.0 ) );
    v->push_back( osg::Vec3( dx + 4.0 , -6.5, _z_box + 1.0 ) );
    v->push_back( osg::Vec3( dx + 4.0 ,  6.5, _z_box + 1.0 ) );
    v->push_back( osg::Vec3( dx       ,  6.5, _z_box + 1.0 ) );

    geometry->setVertexArray( v.get() );
    geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::QUADS, 0, v->size() ) );

    geode->addDrawable( geometry.get() );
}

////////////////////////////////////////////////////////////////////////////////

void ASI::createDiskMask()
{
    const double dy = 6.5;
    const double x = -50.5;
    const double w = 4.0;
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

void ASI::createFrame()
{
    const double x = -60.0;
    const double h = 33.0;
    const double w = 16.0;

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

void ASI::createScale()
{
    const double x = -44.0;
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

    const double step = 5.0;
    const double min = _ias_min;
    const double max = 800.0;
    const int steps = ( max - min ) / step + 1;

    for ( int i = 0; i < steps; i++ )
    {
        double ias = min + i * step;

        double x0 = x - w2;
        double y = _ias2pt * ias;

        if ( i % 2 == 0 )
        {
            x0 = x - w1;

            std::stringstream ias_str;
            ias_str << ias;

            osg::ref_ptr<osgText::Text> text = new osgText::Text();
            text->setFont( Fonts::get( "fonts/g1000.ttf" ) );
            text->setColor( osg::Vec4( Colors::_white, 1.0 ) );
            text->setCharacterSize( 4.0 );
            text->setAxisAlignment( osgText::TextBase::XY_PLANE );
            text->setPosition( osg::Vec3( x0 - 1.0, y, _z_scale ) );
            text->setLayout( osgText::Text::LEFT_TO_RIGHT );
            text->setAlignment( osgText::Text::RIGHT_CENTER );
            text->setText( ias_str.str() );
            geode->addDrawable( text );
        }

        v->push_back( osg::Vec3( x0 , y, _z_scale ) );
        v->push_back( osg::Vec3( x  , y, _z_scale ) );
    }

    geometry->setVertexArray( v.get() );
    geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, v->size() ) );

    geometry->setColorArray( c.get() );
    geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

    geode->addDrawable( geometry.get() );
}

////////////////////////////////////////////////////////////////////////////////

void ASI::createScaleMask()
{
    const double x  = -60.0;
    const double dy = 33.0;
    const double h1 = 23.0;
    const double h2 = 61.0;
    const double w  = 16.0;

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
