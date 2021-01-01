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
#ifndef G1000_COLORS_H
#define G1000_COLORS_H

////////////////////////////////////////////////////////////////////////////////

#include <osg/Vec3>

////////////////////////////////////////////////////////////////////////////////

namespace g1000
{

/**
 * @brief Colors class.
 */
class Colors
{
public:

    static const osg::Vec3 _black;      ///< black
    static const osg::Vec3 _white;      ///< white

    static const osg::Vec3 _cyan;       ///< cyan    (according to W3C)
    static const osg::Vec3 _green;      ///< green   (according to W3C)
    static const osg::Vec3 _grey;       ///< grey    (according to W3C)
    static const osg::Vec3 _lime;       ///< lime    (according to W3C)
    static const osg::Vec3 _magenta;    ///< magenta (according to W3C)
    static const osg::Vec3 _maroon;     ///< maroon  (according to W3C)
    static const osg::Vec3 _navy;       ///< navy    (according to W3C)
    static const osg::Vec3 _olive;      ///< olive   (according to W3C)
    static const osg::Vec3 _orange;     ///< orange  (according to W3C)
    static const osg::Vec3 _purple;     ///< purple  (according to W3C)
    static const osg::Vec3 _red;        ///< red     (according to W3C)
    static const osg::Vec3 _teal;       ///< teal    (according to W3C)
    static const osg::Vec3 _yellow;     ///< yellow  (according to W3C)
};

} // end of g1000 namespace

////////////////////////////////////////////////////////////////////////////////

#endif // G1000_COLORS_H
