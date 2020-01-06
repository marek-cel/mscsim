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

#include <gui/Fonts.h>

////////////////////////////////////////////////////////////////////////////////

QFont Fonts::_xsmall = QFont();
QFont Fonts::_small  = QFont();
QFont Fonts::_medium = QFont();

bool Fonts::_inited = false;

////////////////////////////////////////////////////////////////////////////////

void Fonts::init()
{
    _inited = true;

#   ifdef WIN32
    _xsmall.setFamily( "Courier" );
    _xsmall.setPointSizeF( 11.0f );
    _xsmall.setStretch( QFont::Condensed );
    _xsmall.setWeight( QFont::Bold );

    _small.setFamily( "Courier" );
    _small.setPointSizeF( 12.0f );
    _small.setStretch( QFont::Condensed );
    _small.setWeight( QFont::Bold );

    _medium.setFamily( "Courier" );
    _medium.setPointSizeF( 16.0f );
    _medium.setStretch( QFont::Condensed );
    _medium.setWeight( QFont::Bold );
#   else
    _xsmall.setFamily( "courier" );
    _xsmall.setPointSizeF( 11.0f );
    _xsmall.setStretch( QFont::Condensed );
    _xsmall.setWeight( QFont::Bold );

    _small.setFamily( "courier" );
    _small.setPointSizeF( 12.0f );
    _small.setStretch( QFont::Condensed );
    _small.setWeight( QFont::Bold );

    _medium.setFamily( "courier" );
    _medium.setPointSizeF( 16.0f );
    _medium.setStretch( QFont::Condensed );
    _medium.setWeight( QFont::Bold );
#   endif
}
