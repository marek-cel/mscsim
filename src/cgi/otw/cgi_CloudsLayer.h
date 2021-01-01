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
#ifndef CGI_CLOUDSLAYER_H
#define CGI_CLOUDSLAYER_H

////////////////////////////////////////////////////////////////////////////////

#include <Data.h>

#include <cgi/cgi_Module.h>
#include <cgi/cgi_Textures.h>

////////////////////////////////////////////////////////////////////////////////

namespace cgi
{

/** @brief Layer clouds class. */
class CloudsLayer : public Module
{
public:

    typedef Data::CGI::Environment::Clouds::Data::Layer::Cover Cover;

    /** @brief Constructor. */
    CloudsLayer( const Module *parent = NULLPTR );

    /** @brief Destructor. */
    virtual ~CloudsLayer();

    /** @brief Updates clouds. */
    void update();

private:

    Textures::List _textures;   ///<

    Cover _cover;               ///< cover
    float _base_asl;            ///< [m] clouds base above mean sea level

    short _framesCounter;       ///<
    bool _created;              ///<

    void create();
    void createLayer( osg::Group *parent, double lat, double lon, double alt );

    void remove();
};

} // end of cgi namespace

////////////////////////////////////////////////////////////////////////////////

#endif // CGI_CLOUDSLAYER_H
