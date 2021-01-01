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
#ifndef CGI_CLOUDSBLOCK_H
#define CGI_CLOUDSBLOCK_H

////////////////////////////////////////////////////////////////////////////////

#include <osg/PositionAttitudeTransform>

#include <cgi/cgi_Module.h>
#include <cgi/cgi_Textures.h>

////////////////////////////////////////////////////////////////////////////////

namespace cgi
{

/** @brief Block clouds class. */
class CloudsBlock : public Module
{
public:

    /** @brief Constructor. */
    CloudsBlock( const Module *parent = NULLPTR );

    /** @brief Destructor. */
    virtual ~CloudsBlock();

    /** @brief Updates clouds. */
    void update();

private:

    std::vector< osg::ref_ptr< osg::PositionAttitudeTransform > > _patClouds;

    Textures::List _textures;   ///<

    osg::Vec3 _pos_wgs;         ///<

    short _count;               ///< number of clouds within visual range
    float _base_asl;            ///< [m] clouds base above mean sea level
    float _thickness;           ///< [m] clouds thickness

    short _framesCounter;       ///<
    bool _created;              ///<

    void create();
    void createBlock( osg::Group *parent );
    void createSprite( osg::Group *parent );

    void remove();
};

} // end of cgi namespace

////////////////////////////////////////////////////////////////////////////////

#endif // CGI_CLOUDSBLOCK_H
