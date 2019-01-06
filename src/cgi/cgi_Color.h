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
#ifndef CGI_COLOR_H
#define CGI_COLOR_H

////////////////////////////////////////////////////////////////////////////////

#include <osg/Vec3>

////////////////////////////////////////////////////////////////////////////////

namespace cgi
{

/**
 * @brief Colors class.
 */
class Color
{
public:

    static const osg::Vec3 amber;       ///< amber
    static const osg::Vec3 black;       ///< black  (according to W3C)
    static const osg::Vec3 cyan;        ///< cyan   (according to W3C)
    static const osg::Vec3 green;       ///< green  (according to W3C)
    static const osg::Vec3 grey;        ///< grey   (according to W3C)
    static const osg::Vec3 lime;        ///< lime   (according to W3C)
    static const osg::Vec3 orange;      ///< orange (according to W3C)
    static const osg::Vec3 red;         ///< red    (according to W3C)
    static const osg::Vec3 white;       ///< white  (according to W3C)
    static const osg::Vec3 yellow;      ///< yellow (according to W3C)

    static const osg::Vec3 hud;         ///<

    static const osg::Vec3 sky[ 9 ];    ///<
    static const osg::Vec3 fog[ 9 ];    ///<

    static const osg::Vec3 sun[ 11 ];   ///< sun light color
    static const osg::Vec3 moon;        ///< moon light color
};

} // end of cgi namespace

////////////////////////////////////////////////////////////////////////////////

#endif // CGI_COLOR_H
