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

#include <g1000/cgi/g1000_WYPT.h>

#include <g1000/cgi/g1000_Colors.h>
#include <g1000/cgi/g1000_Fonts.h>

////////////////////////////////////////////////////////////////////////////////

using namespace g1000;

////////////////////////////////////////////////////////////////////////////////

const double WYPT::_z_text = -40.0;

////////////////////////////////////////////////////////////////////////////////

WYPT::WYPT( IFD *ifd ) :
    Module( ifd )
{
    create();
}

////////////////////////////////////////////////////////////////////////////////

WYPT::~WYPT() {}

////////////////////////////////////////////////////////////////////////////////

void WYPT::update()
{

}

////////////////////////////////////////////////////////////////////////////////

void WYPT::create()
{
    const double h = 11.0;

    const double dx_freq = 51.0;

    const double x_ap = -G1000_GDU_WIDTH_2 + dx_freq + 25.0;

    const double y = G1000_GDU_HEIGHT_2 - 0.4 * h;

    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    _root->addChild( geode.get() );

    _textWYPT = new osgText::Text();
    _textWYPT->setFont( Fonts::get( "fonts/g1000.ttf" ) );
    _textWYPT->setColor( osg::Vec4( Colors::_magenta, 1.0 ) );
    _textWYPT->setCharacterSize( 5.0 );
    _textWYPT->setAxisAlignment( osgText::TextBase::XY_PLANE );
    _textWYPT->setPosition( osg::Vec3( x_ap, y, _z_text ) );
    _textWYPT->setLayout( osgText::Text::LEFT_TO_RIGHT );
    _textWYPT->setAlignment( osgText::Text::LEFT_BOTTOM );
    _textWYPT->setText( "D" );
    geode->addDrawable( _textWYPT );
}
