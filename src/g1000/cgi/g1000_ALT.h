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
#ifndef G1000_ALT_H
#define G1000_ALT_H

////////////////////////////////////////////////////////////////////////////////

#include <osg/PositionAttitudeTransform>

#include <osgText/Text>

#include <g1000/cgi/g1000_Module.h>

////////////////////////////////////////////////////////////////////////////////

namespace g1000
{

/**
 * @brief Altimeter.
 */
class ALT : public Module
{
public:

    static const double _x_offset;
    static const double _y_offset;

    static const osg::Vec3 _colorBack;

    static const double _z_back;
    static const double _z_box;
    static const double _z_bug;
    static const double _z_frame;
    static const double _z_scale;

    static const double _alt2pt;

    static const int _depth_sorted_bin_scale;
    static const int _depth_sorted_bin_disk;

    /** Constructor. */
    ALT( IFD *ifd );

    /** Destructor. */
    virtual ~ALT();

    /** Updates. */
    void update();

private:

    osg::ref_ptr<osg::PositionAttitudeTransform> _pat;
    osg::ref_ptr<osg::PositionAttitudeTransform> _patDisk;
    osg::ref_ptr<osg::PositionAttitudeTransform> _patScale;
    osg::ref_ptr<osg::PositionAttitudeTransform> _patBug;

    osg::ref_ptr<osgText::Text> _textAlt_100;
    osg::ref_ptr<osgText::Text> _textPress;
    osg::ref_ptr<osgText::Text> _textSelect;

    void createAltitudeBug();
    void createBack();
    void createBox();
    void createBoxSelect();
    void createBoxPress();
    void createDisk();
    void createDiskMask();
    void createFrame();
    void createScale();
    void createScaleMask();
};

} // end of g1000 namespace

////////////////////////////////////////////////////////////////////////////////

#endif // G1000_ALT_H
