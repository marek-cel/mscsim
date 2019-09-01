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
#ifndef CGI_AIRPORT_H
#define CGI_AIRPORT_H

////////////////////////////////////////////////////////////////////////////////

#include <osg/Group>
#include <osg/PositionAttitudeTransform>
#include <osg/Switch>

#include <cgi/cgi_Module.h>

////////////////////////////////////////////////////////////////////////////////

namespace cgi
{

/**
 * @brief Airport class.
 */
class Airport : public Module
{
public:

    /** Constructor. */
    Airport( const std::string &file, double lat, double lon, double alt,
             Module *parent = 0 );

    /** Destructor. */
    virtual ~Airport();

    /** Updates airport. */
    void update();

private:

    osg::ref_ptr<osg::PositionAttitudeTransform> _pat;

    osg::ref_ptr<osg::Switch> _switchLightsHELI;
    osg::ref_ptr<osg::Switch> _switchLightsRALS;
    osg::ref_ptr<osg::Switch> _switchLightsRCLS;
    osg::ref_ptr<osg::Switch> _switchLightsRELS;
    osg::ref_ptr<osg::Switch> _switchLightsTDZL;
    osg::ref_ptr<osg::Switch> _switchLightsTELS;
    osg::ref_ptr<osg::Switch> _switchLightsTWRL;
    osg::ref_ptr<osg::Switch> _switchLightsVGSI;

    osg::ref_ptr<osg::Switch> _switchGatesRwy18;
    osg::ref_ptr<osg::Switch> _switchGatesRwy36;
};

} // end of cgi namespace

////////////////////////////////////////////////////////////////////////////////

#endif // CGI_AIRPORT_H
