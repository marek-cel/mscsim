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
#ifndef CGI_LANDMARK_H
#define CGI_LANDMARK_H

////////////////////////////////////////////////////////////////////////////////

#include <osg/PositionAttitudeTransform>

#include <cgi/cgi_Module.h>

////////////////////////////////////////////////////////////////////////////////

namespace cgi
{

/**
 * @brief Landmark class.
 */
class Landmark : public Module
{
public:

    /** @brief Constructor. */
    Landmark( const char *file, double lat, double lon, double alt,
              double hdg, bool reflection,
              const Module *parent = NULLPTR );

    /** @brief Destructor. */
    virtual ~Landmark();

    /** @brief Updates landmark. */
    void update();

private:

    osg::ref_ptr<osg::PositionAttitudeTransform> _pat;
};

} // end of cgi namespace

////////////////////////////////////////////////////////////////////////////////

#endif // CGI_LANDMARK_H
