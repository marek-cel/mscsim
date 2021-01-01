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

#include <cgi/cgi_Models.h>

#include <osg/Drawable>
#include <osg/Geometry>
#include <osg/LineWidth>

#include <osgDB/ReadFile>

#include <sim/Log.h>
#include <sim/Path.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

osg::Node* Models::get( const char *objectFile, bool straight )
{
    for ( unsigned int i = 0; i < instance()->_fileNames.size(); i++ )
    {
        if ( objectFile == instance()->_fileNames.at( i ) )
        {
            return instance()->_objects.at( i );
        }
    }

    std::string filePath = Path::get( objectFile );
    osg::ref_ptr<osg::Node> object = ( straight ) ? osgDB::readNodeFile( filePath ) : readNodeFile( filePath );

    if ( object.valid() )
    {
        instance()->_objects.push_back( object.get() );
        instance()->_fileNames.push_back( objectFile );

        return object.get();
    }
    else
    {
        Log::e() << "Cannot open file: " << filePath << std::endl;
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////

osg::Node* Models::readNodeFile( std::string objectFile )
{
    osg::ref_ptr<osg::Node> object = osgDB::readNodeFile( objectFile );

    return object.release();
}

////////////////////////////////////////////////////////////////////////////////

Models::Models()
{
    _fileNames.clear();
    _objects.clear();
}

////////////////////////////////////////////////////////////////////////////////

Models::~Models() {}
