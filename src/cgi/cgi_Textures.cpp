/****************************************************************************//*
 * Copyright (C) 2019 Marek M. Cel
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

#include <cgi/cgi_Textures.h>

#include <osgDB/ReadFile>

#include <fdm/fdm_Path.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

osg::Texture2D* Textures::get( const std::string &textureFile, float maxAnisotropy,
                               osg::Texture::WrapMode mode )
{
    for ( unsigned int i = 0; i < instance()->m_fileNames.size(); i++ )
    {
        if ( textureFile == instance()->m_fileNames.at( i ) )
        {
            instance()->m_textures.at( i )->setMaxAnisotropy( maxAnisotropy );
            return instance()->m_textures.at( i );
        }
    }

    std::string filePath = fdm::Path::get( textureFile );
    osg::ref_ptr<osg::Image> image = osgDB::readImageFile( filePath );

    if ( image.valid() )
    {
        osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D();
        texture->setImage( image.get() );

        texture->setWrap( osg::Texture2D::WRAP_S, mode );
        texture->setWrap( osg::Texture2D::WRAP_T, mode );

        texture->setNumMipmapLevels( 4 );
        texture->setMaxAnisotropy( maxAnisotropy );

        texture->setFilter( osg::Texture::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_NEAREST );
        texture->setFilter( osg::Texture::MAG_FILTER, osg::Texture::LINEAR );

        texture->setUnRefImageDataAfterApply( false );

        instance()->m_textures.push_back( texture.get() );
        instance()->m_fileNames.push_back( textureFile );

        return texture.get();
    }
    else
    {
        osg::notify(osg::ALWAYS) << "ERROR! Cannot open file: " << filePath << std::endl;
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////

Textures::Textures()
{
    m_fileNames.clear();
    m_textures.clear();
}

////////////////////////////////////////////////////////////////////////////////

Textures::~Textures() {}
