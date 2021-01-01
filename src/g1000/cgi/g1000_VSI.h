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
#ifndef G1000_VSI_H
#define G1000_VSI_H

////////////////////////////////////////////////////////////////////////////////

#include <osg/PositionAttitudeTransform>

#include <osgText/Text>

#include <g1000/cgi/g1000_Module.h>

////////////////////////////////////////////////////////////////////////////////

namespace g1000
{

/**
 * @brief PFD Vertical Speed Indicator.
 */
class VSI : public Module
{
public:

    static const double _x_offset;
    static const double _y_offset;

    static const osg::Vec3 _colorBack;

    static const double _z_back;
    static const double _z_box;
    static const double _z_bug;
    static const double _z_frame;
    static const double _z_ptr;
    static const double _z_scale;

    static const double _vs2pt;

    /** Constructor. */
    VSI( IFD *ifd );

    /** Destructor. */
    virtual ~VSI();

    /** Updates. */
    void update();

private:

    osg::ref_ptr<osg::PositionAttitudeTransform> _pat;
    osg::ref_ptr<osg::PositionAttitudeTransform> _patPointer;
    osg::ref_ptr<osg::PositionAttitudeTransform> _patBug;

    osg::ref_ptr<osgText::Text> _textVS;
    osg::ref_ptr<osgText::Text> _textSelect;

    void createBack();
    void createBoxSelect();
    void createBug();
    void createFrame();
    void createPointer();
    void createScale();
};

} // end of g1000 namespace

////////////////////////////////////////////////////////////////////////////////

#endif // G1000_VSI_H
