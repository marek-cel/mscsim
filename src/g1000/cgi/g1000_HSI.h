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
#ifndef G1000_HSI_H
#define G1000_HSI_H

////////////////////////////////////////////////////////////////////////////////

#include <osg/PositionAttitudeTransform>

#include <osgText/Text>

#include <g1000/cgi/g1000_Module.h>

////////////////////////////////////////////////////////////////////////////////

namespace g1000
{

/**
 * @brief PFD Horizontal Situation Indicator
 */
class HSI : public Module
{
public:

    static const double _x_offset;
    static const double _y_offset;

    static const osg::Vec3 _colorFace;

    static const double _z_aircraft_symbol;
    static const double _z_box;
    static const double _z_bug;
    static const double _z_compass_face;
    static const double _z_course_pointer;
    static const double _z_heading_bug;

    /** Constructor. */
    HSI( IFD *ifd );

    /** Destructor. */
    virtual ~HSI();

    /** Updates. */
    void update();

private:

    osg::ref_ptr<osg::PositionAttitudeTransform> _pat;
    osg::ref_ptr<osg::PositionAttitudeTransform> _patHDG;
    osg::ref_ptr<osg::PositionAttitudeTransform> _patCRS;
    osg::ref_ptr<osg::PositionAttitudeTransform> _patBug;

    osg::ref_ptr<osgText::Text> _headingText;

    osg::ref_ptr<osgText::Text> _textCRS;
    osg::ref_ptr<osgText::Text> _textHDG;

    void createAircraftSymbol();
    void createBoxCRS();
    void createBoxHDG();
    void createCompass();
    void createCoursePointer();
    void createHeadingBox();
    void createHeadingBug();
};

} // end of g1000 namespace

////////////////////////////////////////////////////////////////////////////////

#endif // G1000_HSI_H
