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

#include <g1000/cgi/g1000_HSI.h>

#include <iomanip>
#include <sstream>

#include <osg/Geode>
#include <osg/Geometry>

#include <g1000/g1000_Defines.h>

#include <g1000/cgi/g1000_Colors.h>
#include <g1000/cgi/g1000_Fonts.h>

////////////////////////////////////////////////////////////////////////////////

using namespace g1000;

////////////////////////////////////////////////////////////////////////////////

const double HSI::_x_offset = -10.0;
const double HSI::_y_offset = -40.0;

const osg::Vec3 HSI::_colorFace = osg::Vec3( 0.38, 0.38, 0.38 );

const double HSI::_z_aircraft_symbol = -60.0;
const double HSI::_z_box             = -70.0;
const double HSI::_z_bug             = -70.0;
const double HSI::_z_compass_face    = -80.0;
const double HSI::_z_course_pointer  = -70.0;
const double HSI::_z_heading_bug     = -70.0;

////////////////////////////////////////////////////////////////////////////////

HSI::HSI( IFD *ifd ) :
    Module( ifd )
{
    _pat = new osg::PositionAttitudeTransform();
    _root->addChild( _pat.get() );
    _pat->setPosition( osg::Vec3( _x_offset, _y_offset, 0.0 ) );

    _patHDG = new osg::PositionAttitudeTransform();
    _pat->addChild( _patHDG.get() );

    _patBug = new osg::PositionAttitudeTransform();
    _pat->addChild( _patBug.get() );

    _patCRS = new osg::PositionAttitudeTransform();
    _pat->addChild( _patCRS.get() );

    createAircraftSymbol();
    createBoxCRS();
    createBoxHDG();
    createCompass();
    createCoursePointer();
    createHeadingBox();
    createHeadingBug();
}

////////////////////////////////////////////////////////////////////////////////

HSI::~HSI() {}

////////////////////////////////////////////////////////////////////////////////

void HSI::update()
{
    double ahrs_heading = _ifd->grs()->getHeading();

    double sel_heading = _ifd->input().sel_heading;
    double sel_course  = _ifd->input().sel_course;

    _patHDG->setAttitude( osg::Quat( ahrs_heading, osg::Z_AXIS ) );
    _patBug->setAttitude( osg::Quat( ahrs_heading - sel_heading , osg::Z_AXIS ) );
    _patCRS->setAttitude( osg::Quat( ahrs_heading - sel_course  , osg::Z_AXIS ) );

    std::stringstream heading_str;
    double heading_deg = floor( osg::RadiansToDegrees( ahrs_heading ) + 0.5 );
    while ( heading_deg <   0.0 ) heading_deg += 360.0;
    while ( heading_deg > 360.0 ) heading_deg -= 360.0;

    heading_str << std::setfill( '0' ) << std::setw( 3 ) << (int)( floor( heading_deg + 0.5 ) ) << u8"\302\260";
    _headingText->setText( osgText::String( heading_str.str(), osgText::String::ENCODING_UTF8 ) );

    std::stringstream crs_str;
    double crs_deg = osg::RadiansToDegrees( sel_course );
    while ( crs_deg <   0.0 ) crs_deg += 360.0;
    while ( crs_deg > 360.0 ) crs_deg -= 360.0;

    crs_str << std::setfill( '0' ) << std::setw( 3 ) << (int)( floor( crs_deg + 0.5 ) ) << u8"\302\260";
    _textCRS->setText( osgText::String( crs_str.str(), osgText::String::ENCODING_UTF8 ) );

    std::stringstream hdg_str;
    double hdg_deg = osg::RadiansToDegrees( sel_heading );
    while ( hdg_deg <   0.0 ) hdg_deg += 360.0;
    while ( hdg_deg > 360.0 ) hdg_deg -= 360.0;

    hdg_str << std::setfill( '0' ) << std::setw( 3 ) << (int)( floor( hdg_deg + 0.5 ) ) << u8"\302\260";
    _textHDG->setText( osgText::String( hdg_str.str(), osgText::String::ENCODING_UTF8 ) );
}

////////////////////////////////////////////////////////////////////////////////

void HSI::createAircraftSymbol()
{
    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    _pat->addChild( geode.get() );

    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();

    // fuselage
    v->push_back( osg::Vec3(  0.0 ,  4.1, _z_box ) );
    v->push_back( osg::Vec3( -0.6 ,  3.3, _z_box ) );
    v->push_back( osg::Vec3(  0.6 ,  3.3, _z_box ) );

    v->push_back( osg::Vec3( -0.6 ,  3.3, _z_box ) );
    v->push_back( osg::Vec3(  0.6 ,  3.3, _z_box ) );
    v->push_back( osg::Vec3(  0.6 , -3.3, _z_box ) );

    v->push_back( osg::Vec3( -0.6 , -3.3, _z_box ) );
    v->push_back( osg::Vec3(  0.6 , -3.3, _z_box ) );
    v->push_back( osg::Vec3( -0.6 ,  3.3, _z_box ) );

    // wing
    v->push_back( osg::Vec3( -0.6 ,  1.4, _z_box ) );
    v->push_back( osg::Vec3( -0.6 , -0.3, _z_box ) );
    v->push_back( osg::Vec3( -3.5 ,  0.3, _z_box ) );

    v->push_back( osg::Vec3( -3.5 , -0.3, _z_box ) );
    v->push_back( osg::Vec3( -3.5 ,  0.3, _z_box ) );
    v->push_back( osg::Vec3( -0.6 , -0.3, _z_box ) );

    v->push_back( osg::Vec3(  0.6 ,  1.4, _z_box ) );
    v->push_back( osg::Vec3(  0.6 , -0.3, _z_box ) );
    v->push_back( osg::Vec3(  3.5 ,  0.3, _z_box ) );

    v->push_back( osg::Vec3(  3.5 , -0.3, _z_box ) );
    v->push_back( osg::Vec3(  3.5 ,  0.3, _z_box ) );
    v->push_back( osg::Vec3(  0.6 , -0.3, _z_box ) );

    // stab
    v->push_back( osg::Vec3( -0.6 , -2.1, _z_box ) );
    v->push_back( osg::Vec3( -0.6 , -3.3, _z_box ) );
    v->push_back( osg::Vec3( -1.6 , -2.8, _z_box ) );

    v->push_back( osg::Vec3( -1.6 , -2.8, _z_box ) );
    v->push_back( osg::Vec3( -1.6 , -3.3, _z_box ) );
    v->push_back( osg::Vec3( -0.6 , -3.3, _z_box ) );

    v->push_back( osg::Vec3(  0.6 , -2.1, _z_box ) );
    v->push_back( osg::Vec3(  0.6 , -3.3, _z_box ) );
    v->push_back( osg::Vec3(  1.6 , -2.8, _z_box ) );

    v->push_back( osg::Vec3(  1.6 , -2.8, _z_box ) );
    v->push_back( osg::Vec3(  1.6 , -3.3, _z_box ) );
    v->push_back( osg::Vec3(  0.6 , -3.3, _z_box ) );

    c->push_back( osg::Vec4( Colors::_white, 1.0 ) );

    geom->setVertexArray( v.get() );
    geom->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::TRIANGLES, 0, v->size() ) );

    geom->setColorArray( c.get() );
    geom->setColorBinding( osg::Geometry::BIND_OVERALL );

    geode->addDrawable( geom.get() );
}

////////////////////////////////////////////////////////////////////////////////

void HSI::createBoxCRS()
{
    double x = 20.0;
    double y = 26.0;
    double h = 6.0;
    double w = 17.0;

    const double delta_x_txt = 0.5;
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

    vb->push_back( osg::Vec3( x     , y     , _z_box + 1.0 ) );
    vb->push_back( osg::Vec3( x + w , y     , _z_box + 1.0 ) );
    vb->push_back( osg::Vec3( x + w , y + h , _z_box + 1.0 ) );
    vb->push_back( osg::Vec3( x     , y + h , _z_box + 1.0 ) );

    vf->push_back( osg::Vec3( x     , y     , _z_box ) );
    vf->push_back( osg::Vec3( x + w , y     , _z_box ) );
    vf->push_back( osg::Vec3( x + w , y + h , _z_box ) );
    vf->push_back( osg::Vec3( x     , y + h , _z_box ) );

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
    textLabel->setCharacterSize( 2.9 );
    textLabel->setAxisAlignment( osgText::TextBase::XY_PLANE );
    textLabel->setPosition( osg::Vec3( x + delta_x_txt, y + delta_y_txt, _z_box + 2.0 ) );
    textLabel->setLayout( osgText::Text::LEFT_TO_RIGHT );
    textLabel->setAlignment( osgText::Text::LEFT_BASE_LINE );
    textLabel->setText( "CRS" );
    geode_t->addDrawable( textLabel );

    _textCRS = new osgText::Text();
    _textCRS->setFont( Fonts::get( "fonts/g1000.ttf" ) );
    _textCRS->setColor( osg::Vec4( Colors::_magenta, 1.0 ) );
    _textCRS->setCharacterSize( 4.0 );
    _textCRS->setAxisAlignment( osgText::TextBase::XY_PLANE );
    _textCRS->setPosition( osg::Vec3( x + w - delta_x_txt, y + delta_y_txt, _z_box + 2.0 ) );
    _textCRS->setLayout( osgText::Text::LEFT_TO_RIGHT );
    _textCRS->setAlignment( osgText::Text::RIGHT_BASE_LINE );
    _textCRS->setText( "000" );
    geode_t->addDrawable( _textCRS );
}

////////////////////////////////////////////////////////////////////////////////

void HSI::createBoxHDG()
{
    double x = -36.0;
    double y = 26.0;
    double h = 6.0;
    double w = 17.0;

    const double delta_x_txt = 0.5;
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

    vb->push_back( osg::Vec3( x     , y     , _z_box + 1.0 ) );
    vb->push_back( osg::Vec3( x + w , y     , _z_box + 1.0 ) );
    vb->push_back( osg::Vec3( x + w , y + h , _z_box + 1.0 ) );
    vb->push_back( osg::Vec3( x     , y + h , _z_box + 1.0 ) );

    vf->push_back( osg::Vec3( x     , y     , _z_box ) );
    vf->push_back( osg::Vec3( x + w , y     , _z_box ) );
    vf->push_back( osg::Vec3( x + w , y + h , _z_box ) );
    vf->push_back( osg::Vec3( x     , y + h , _z_box ) );

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
    textLabel->setCharacterSize( 2.9 );
    textLabel->setAxisAlignment( osgText::TextBase::XY_PLANE );
    textLabel->setPosition( osg::Vec3( x + delta_x_txt, y + delta_y_txt, _z_box + 2.0 ) );
    textLabel->setLayout( osgText::Text::LEFT_TO_RIGHT );
    textLabel->setAlignment( osgText::Text::LEFT_BASE_LINE );
    textLabel->setText( "HDG" );
    geode_t->addDrawable( textLabel );

    _textHDG = new osgText::Text();
    _textHDG->setFont( Fonts::get( "fonts/g1000.ttf" ) );
    _textHDG->setColor( osg::Vec4( Colors::_cyan, 1.0 ) );
    _textHDG->setCharacterSize( 4.0 );
    _textHDG->setAxisAlignment( osgText::TextBase::XY_PLANE );
    _textHDG->setPosition( osg::Vec3( x + w - delta_x_txt, y + delta_y_txt, _z_box + 2.0 ) );
    _textHDG->setLayout( osgText::Text::LEFT_TO_RIGHT );
    _textHDG->setAlignment( osgText::Text::RIGHT_BASE_LINE );
    _textHDG->setText( "000" );
    geode_t->addDrawable( _textHDG );
}

////////////////////////////////////////////////////////////////////////////////

void HSI::createCompass()
{
    const double r0 = 28.0;
    const double r1 = 27.0;
    const double r2 = 26.0;

    const double r3 = 29.0;
    const double r4 = 32.0;

    osg::ref_ptr<osg::Geode> geode_f = new osg::Geode();
    osg::ref_ptr<osg::Geode> geode_l = new osg::Geode();
    osg::ref_ptr<osg::Geode> geode_s = new osg::Geode();
    osg::ref_ptr<osg::Geode> geode_p = new osg::Geode();

    _patHDG->addChild( geode_f.get() );
    _patHDG->addChild( geode_l.get() );
    _pat->addChild( geode_s.get() );
    _pat->addChild( geode_p.get() );

    osg::ref_ptr<osg::Vec4Array> cf = new osg::Vec4Array();
    osg::ref_ptr<osg::Vec4Array> cw = new osg::Vec4Array();

    cf->push_back( osg::Vec4( _colorFace, 0.5 ) );
    cw->push_back( osg::Vec4( Colors::_white, 1.0 ) );

    osg::ref_ptr<osg::Geometry> geom_f = new osg::Geometry();
    osg::ref_ptr<osg::Geometry> geom_l = new osg::Geometry();
    osg::ref_ptr<osg::Geometry> geom_s = new osg::Geometry();
    osg::ref_ptr<osg::Geometry> geom_p = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> vf = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec3Array> vl = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec3Array> vs = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec3Array> vp = new osg::Vec3Array();

    vf->push_back( osg::Vec3( 0.0, 0.0, _z_compass_face ) );

    const double step = 5.0;
    const int steps = 360.0 / step;

    for ( int i = 0; i <= steps; i++ )
    {
        double heading = i * step;
        double a = osg::DegreesToRadians( heading );

        double sinA = sin( a );
        double cosA = cos( a );

        double x0 = r0 * sinA;
        double y0 = r0 * cosA;

        vf->push_back( osg::Vec3( x0, y0, _z_compass_face ) );

        if ( i < steps )
        {
            double x1 = ( i % 2 == 0 ? r2: r1 ) * sinA;
            double y1 = ( i % 2 == 0 ? r2: r1 ) * cosA;

            vl->push_back( osg::Vec3( x0, y0, _z_compass_face + 1.0 ) );
            vl->push_back( osg::Vec3( x1, y1, _z_compass_face + 1.0 ) );

            if ( i % 9 == 0 && i != 0 && i != 36 )
            {
                double x3 = r3 * sinA;
                double y3 = r3 * cosA;

                double x4 = r4 * sinA;
                double y4 = r4 * cosA;

                vs->push_back( osg::Vec3( x3, y3, _z_compass_face + 1.0 ) );
                vs->push_back( osg::Vec3( x4, y4, _z_compass_face + 1.0 ) );
            }

            if ( i % 6 == 0 )
            {
                std::stringstream heading_str;

                double charSize = 4.0;

                switch ( i )
                {
                    case 0:  heading_str << "N"; charSize = 5.0; break;
                    case 18: heading_str << "E"; charSize = 5.0; break;
                    case 36: heading_str << "S"; charSize = 5.0; break;
                    case 54: heading_str << "W"; charSize = 5.0; break;
                    default: heading_str << (int)( heading / 10.0 ); break;
                }

                osg::ref_ptr<osg::PositionAttitudeTransform> pat = new osg::PositionAttitudeTransform();
                _patHDG->addChild( pat.get() );

                osg::ref_ptr<osg::Geode> geode_t = new osg::Geode();
                pat->addChild( geode_t.get() );

                pat->setAttitude( osg::Quat( -a, osg::Z_AXIS ) );

                osg::ref_ptr<osgText::Text> text = new osgText::Text();
                text->setFont( Fonts::get( "fonts/g1000.ttf" ) );
                text->setColor( osg::Vec4( Colors::_white, 1.0 ) );
                text->setCharacterSize( charSize );
                text->setAxisAlignment( osgText::TextBase::XY_PLANE );
                text->setPosition( osg::Vec3( 0.0, r2 - 1, _z_compass_face + 1.0 ) );
                text->setLayout( osgText::Text::LEFT_TO_RIGHT );
                text->setAlignment( osgText::Text::CENTER_TOP );
                text->setText( heading_str.str() );
                geode_t->addDrawable( text );
            }
        }
    }

    // pointer
    const double yp = 26.5;
    const double hp = 3.5;
    const double wp = 1.8;

    vp->push_back( osg::Vec3(  0.0, yp      , _z_compass_face + 1.0 ) );
    vp->push_back( osg::Vec3(  -wp, yp + hp , _z_compass_face + 1.0 ) );
    vp->push_back( osg::Vec3(   wp, yp + hp , _z_compass_face + 1.0 ) );

    geom_f->setVertexArray( vf.get() );
    geom_l->setVertexArray( vl.get() );
    geom_s->setVertexArray( vs.get() );
    geom_p->setVertexArray( vp.get() );

    geom_f->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::TRIANGLE_FAN, 0, vf->size() ) );
    geom_l->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, vl->size() ) );
    geom_s->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, vs->size() ) );
    geom_p->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::TRIANGLES, 0, vp->size() ) );

    geom_f->setColorArray( cf.get() );
    geom_l->setColorArray( cw.get() );
    geom_s->setColorArray( cw.get() );
    geom_p->setColorArray( cw.get() );

    geom_f->setColorBinding( osg::Geometry::BIND_OVERALL );
    geom_l->setColorBinding( osg::Geometry::BIND_OVERALL );
    geom_s->setColorBinding( osg::Geometry::BIND_OVERALL );
    geom_p->setColorBinding( osg::Geometry::BIND_OVERALL );

    geode_f->addDrawable( geom_f.get() );
    geode_l->addDrawable( geom_l.get() );
    geode_s->addDrawable( geom_s.get() );
    geode_p->addDrawable( geom_p.get() );
}

////////////////////////////////////////////////////////////////////////////////

void HSI::createCoursePointer()
{
    const double y = 26.0;
    const double w = 0.4;
    const double h = 14.0;

    const double wa = 3.0;
    const double ha = 3.0;

    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    _patCRS->addChild( geode.get() );

    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();

    c->push_back( osg::Vec4( Colors::_magenta, 1.0 ) );

    v->push_back( osg::Vec3( -w, -y     , _z_course_pointer ) );
    v->push_back( osg::Vec3(  w, -y     , _z_course_pointer ) );
    v->push_back( osg::Vec3(  w, -y + h , _z_course_pointer ) );
    v->push_back( osg::Vec3( -w, -y + h , _z_course_pointer ) );

    v->push_back( osg::Vec3( -w,  y - h , _z_course_pointer ) );
    v->push_back( osg::Vec3(  w,  y - h , _z_course_pointer ) );
    v->push_back( osg::Vec3(  w,  y     , _z_course_pointer ) );
    v->push_back( osg::Vec3( -w,  y     , _z_course_pointer ) );

    v->push_back( osg::Vec3( -wa ,  y - ha , _z_course_pointer ) );
    v->push_back( osg::Vec3(  wa ,  y - ha , _z_course_pointer ) );
    v->push_back( osg::Vec3(  w  ,  y      , _z_course_pointer ) );
    v->push_back( osg::Vec3( -w  ,  y      , _z_course_pointer ) );

    geometry->setVertexArray( v.get() );
    geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::QUADS, 0, v->size() ) );

    geometry->setColorArray( c.get() );
    geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

    geode->addDrawable( geometry.get() );
}

////////////////////////////////////////////////////////////////////////////////

void HSI::createHeadingBox()
{
    const double y = 30.0;
    const double h = 6.5;
    const double w = 7.0;

    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    _pat->addChild( geode.get() );

    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();

    c->push_back( osg::Vec4( Colors::_black, 1.0 ) );

    v->push_back( osg::Vec3( -w, y     , _z_box ) );
    v->push_back( osg::Vec3(  w, y     , _z_box ) );
    v->push_back( osg::Vec3(  w, y + h , _z_box ) );
    v->push_back( osg::Vec3( -w, y + h , _z_box ) );

    geometry->setVertexArray( v.get() );
    geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::QUADS, 0, v->size() ) );

    geometry->setColorArray( c.get() );
    geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

    geode->addDrawable( geometry.get() );

    _headingText = new osgText::Text();
    _headingText->setFont( Fonts::get( "fonts/g1000.ttf" ) );
    _headingText->setColor( osg::Vec4( Colors::_white, 1.0 ) );
    _headingText->setCharacterSize( 6.0 );
    _headingText->setAxisAlignment( osgText::TextBase::XY_PLANE );
    _headingText->setPosition( osg::Vec3( w + 0.2, y + 1.0, _z_box + 1.0 ) );
    _headingText->setLayout( osgText::Text::LEFT_TO_RIGHT );
    _headingText->setAlignment( osgText::Text::RIGHT_BOTTOM );
    _headingText->setText( osgText::String( "000°", osgText::String::ENCODING_UTF8 ) );
    geode->addDrawable( _headingText );
}

////////////////////////////////////////////////////////////////////////////////

void HSI::createHeadingBug()
{
    const double dy = 26.5;

    const double v13 = 1.0 + ( 1.0 / 3.0 );

    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    _patBug->addChild( geode.get() );

    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();

    v->push_back( osg::Vec3(  0.0, dy       , _z_bug ) );
    v->push_back( osg::Vec3(  0.0, dy - 0.5 , _z_bug ) );
    v->push_back( osg::Vec3(  3.0, dy - 0.5 , _z_bug ) );

    v->push_back( osg::Vec3(  0.0, dy       , _z_bug ) );
    v->push_back( osg::Vec3(  3.0, dy - 0.5 , _z_bug ) );
    v->push_back( osg::Vec3(  3.0, dy + 2.0 , _z_bug ) );

    v->push_back( osg::Vec3(  0.0, dy       , _z_bug ) );
    v->push_back( osg::Vec3(  3.0, dy + 2.0 , _z_bug ) );
    v->push_back( osg::Vec3(  v13, dy + 2.0 , _z_bug ) );

    v->push_back( osg::Vec3(  0.0, dy       , _z_bug ) );
    v->push_back( osg::Vec3(  0.0, dy - 0.5 , _z_bug ) );
    v->push_back( osg::Vec3( -3.0, dy - 0.5 , _z_bug ) );

    v->push_back( osg::Vec3(  0.0, dy       , _z_bug ) );
    v->push_back( osg::Vec3( -3.0, dy - 0.5 , _z_bug ) );
    v->push_back( osg::Vec3( -3.0, dy + 2.0 , _z_bug ) );

    v->push_back( osg::Vec3(  0.0, dy       , _z_bug ) );
    v->push_back( osg::Vec3( -3.0, dy + 2.0 , _z_bug ) );
    v->push_back( osg::Vec3( -v13, dy + 2.0 , _z_bug ) );

    c->push_back( osg::Vec4( Colors::_cyan, 1.0 ) );

    geom->setVertexArray( v.get() );
    geom->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::TRIANGLES, 0, v->size() ) );

    geom->setColorArray( c.get() );
    geom->setColorBinding( osg::Geometry::BIND_OVERALL );

    geode->addDrawable( geom.get() );
}
