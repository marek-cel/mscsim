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

#include <g1000/cgi/g1000_AFCS.h>

#include <g1000/cgi/g1000_Colors.h>
#include <g1000/cgi/g1000_Fonts.h>

////////////////////////////////////////////////////////////////////////////////

using namespace g1000;

////////////////////////////////////////////////////////////////////////////////

const double AFCS::_z_text = -40.0;

////////////////////////////////////////////////////////////////////////////////

AFCS::AFCS( IFD *ifd ) :
    Module( ifd )
{
    create();
}

////////////////////////////////////////////////////////////////////////////////

AFCS::~AFCS() {}

////////////////////////////////////////////////////////////////////////////////

void AFCS::update()
{

}

////////////////////////////////////////////////////////////////////////////////

void AFCS::create()
{
    const double h = 11.0;

    const double dx_freq = 51.0;

    const double x_ap      = -G1000_GDU_WIDTH_2 + dx_freq + 26.0;
    const double x_lat     = -G1000_GDU_WIDTH_2 + dx_freq + 24.0;
    const double x_lat_arm = -G1000_GDU_WIDTH_2 + dx_freq +  3.0;
    const double x_ver     =  G1000_GDU_WIDTH_2 - dx_freq - 54.0;
    const double x_ver_ref =  G1000_GDU_WIDTH_2 - dx_freq - 37.0;
    const double x_ver_arm =  G1000_GDU_WIDTH_2 - dx_freq - 14.0;

    const double y = G1000_GDU_HEIGHT_2 - 0.9 * h;

    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    _root->addChild( geode.get() );

    _textStateAP = new osgText::Text();
    _textStateAP->setFont( Fonts::get( "fonts/g1000.ttf" ) );
    _textStateAP->setColor( osg::Vec4( Colors::_lime, 1.0 ) );
    _textStateAP->setCharacterSize( 5.0 );
    _textStateAP->setAxisAlignment( osgText::TextBase::XY_PLANE );
    _textStateAP->setPosition( osg::Vec3( x_ap, y, _z_text ) );
    _textStateAP->setLayout( osgText::Text::LEFT_TO_RIGHT );
    _textStateAP->setAlignment( osgText::Text::LEFT_BOTTOM );
    _textStateAP->setText( "AP" );
    geode->addDrawable( _textStateAP );

    _textModeLat = new osgText::Text();
    _textModeLat->setFont( Fonts::get( "fonts/g1000.ttf" ) );
    _textModeLat->setColor( osg::Vec4( Colors::_lime, 1.0 ) );
    _textModeLat->setCharacterSize( 5.0 );
    _textModeLat->setAxisAlignment( osgText::TextBase::XY_PLANE );
    _textModeLat->setPosition( osg::Vec3( x_lat, y, _z_text ) );
    _textModeLat->setLayout( osgText::Text::LEFT_TO_RIGHT );
    _textModeLat->setAlignment( osgText::Text::RIGHT_BOTTOM );
    _textModeLat->setText( "HDG" );
    geode->addDrawable( _textModeLat );

    _textModeLatArm = new osgText::Text();
    _textModeLatArm->setFont( Fonts::get( "fonts/g1000.ttf" ) );
    _textModeLatArm->setColor( osg::Vec4( Colors::_white, 1.0 ) );
    _textModeLatArm->setCharacterSize( 4.0 );
    _textModeLatArm->setAxisAlignment( osgText::TextBase::XY_PLANE );
    _textModeLatArm->setPosition( osg::Vec3( x_lat_arm, y, _z_text ) );
    _textModeLatArm->setLayout( osgText::Text::LEFT_TO_RIGHT );
    _textModeLatArm->setAlignment( osgText::Text::LEFT_BOTTOM );
    _textModeLatArm->setText( "GPS" );
    geode->addDrawable( _textModeLatArm );

    _textModeVer = new osgText::Text();
    _textModeVer->setFont( Fonts::get( "fonts/g1000.ttf" ) );
    _textModeVer->setColor( osg::Vec4( Colors::_lime, 1.0 ) );
    _textModeVer->setCharacterSize( 5.0 );
    _textModeVer->setAxisAlignment( osgText::TextBase::XY_PLANE );
    _textModeVer->setPosition( osg::Vec3( x_ver, y, _z_text ) );
    _textModeVer->setLayout( osgText::Text::LEFT_TO_RIGHT );
    _textModeVer->setAlignment( osgText::Text::LEFT_BOTTOM );
    _textModeVer->setText( "ALTS" );
    geode->addDrawable( _textModeVer );

    _textModeVerRef = new osgText::Text();
    _textModeVerRef->setFont( Fonts::get( "fonts/g1000.ttf" ) );
    _textModeVerRef->setColor( osg::Vec4( Colors::_lime, 1.0 ) );
    _textModeVerRef->setCharacterSize( 5.0 );
    _textModeVerRef->setAxisAlignment( osgText::TextBase::XY_PLANE );
    _textModeVerRef->setPosition( osg::Vec3( x_ver_ref, y, _z_text ) );
    _textModeVerRef->setLayout( osgText::Text::LEFT_TO_RIGHT );
    _textModeVerRef->setAlignment( osgText::Text::LEFT_BOTTOM );
    _textModeVerRef->setText( "1500FT" );
    geode->addDrawable( _textModeVerRef );

    _textModeVerArm = new osgText::Text();
    _textModeVerArm->setFont( Fonts::get( "fonts/g1000.ttf" ) );
    _textModeVerArm->setColor( osg::Vec4( Colors::_white, 1.0 ) );
    _textModeVerArm->setCharacterSize( 4.0 );
    _textModeVerArm->setAxisAlignment( osgText::TextBase::XY_PLANE );
    _textModeVerArm->setPosition( osg::Vec3( x_ver_arm, y, _z_text ) );
    _textModeVerArm->setLayout( osgText::Text::LEFT_TO_RIGHT );
    _textModeVerArm->setAlignment( osgText::Text::LEFT_BOTTOM );
    _textModeVerArm->setText( "ALTS" );
    geode->addDrawable( _textModeVerArm );
}
