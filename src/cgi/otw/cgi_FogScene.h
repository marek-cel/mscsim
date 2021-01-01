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
#ifndef CGI_FOGSCENE_H
#define CGI_FOGSCENE_H

////////////////////////////////////////////////////////////////////////////////

#include <osg/Fog>

#include <cgi/cgi_Module.h>

////////////////////////////////////////////////////////////////////////////////

namespace cgi
{

/**
 * @brief Fog scene.
 */
class FogScene : public Module
{
public:

    /**
     * @brief Gets fog color.
     * @param sun_elev [deg] sun elevation
     * @param visibility [m] visibility
     * @return fog color
     */
    static osg::Vec4 getFogColor( float sun_elev , float visibility );

    /**
     * @brief Gets fog number.
     * @param sun_elev [deg] sun elevation
     * @return
     */
    static int getFogNumber( float sun_elev );

    /** @brief Constructor. */
    FogScene( const Module *parent = NULLPTR );

    /** @brief Destructor. */
    virtual ~FogScene();

    /** @brief Updates fog scene. */
    void update();

private:

    osg::ref_ptr<osg::Fog> _fog;
};

} // end of cgi namespace

////////////////////////////////////////////////////////////////////////////////

#endif // CGI_FOGSCENE_H
