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

#include <g1000/cgi/g1000_Fonts.h>

#include <osgDB/ReadFile>

#include <g1000/g1000_Log.h>
#include <g1000/g1000_Path.h>

////////////////////////////////////////////////////////////////////////////////

using namespace g1000;

////////////////////////////////////////////////////////////////////////////////

osgText::Font* Fonts::get( const char *fontFile )
{
    for ( unsigned int i = 0; i < instance()->_fileNames.size(); i++ )
    {
        if ( fontFile == instance()->_fileNames.at( i ) )
        {
            return instance()->_fonts.at( i );
        }
    }

    osg::ref_ptr<osgText::Font> font = osgText::readFontFile( Path::get( fontFile ) );

    if ( font.valid() )
    {
        instance()->_fonts.push_back( font.get() );
        instance()->_fileNames.push_back( fontFile );

        return font.get();
    }
    else
    {
        Log::w() << "Cannot open font file: " << fontFile << std::endl;
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////

void Fonts::reset()
{
    instance()->_fileNames.clear();
    instance()->_fonts.clear();
}

////////////////////////////////////////////////////////////////////////////////

Fonts::Fonts()
{
    _fileNames.clear();
    _fonts.clear();
}

////////////////////////////////////////////////////////////////////////////////

Fonts::~Fonts()
{
    _fileNames.clear();
    _fonts.clear();
}
