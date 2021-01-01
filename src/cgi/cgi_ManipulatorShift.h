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
#ifndef CGI_MANIPULATORSHIFT_H
#define CGI_MANIPULATORSHIFT_H

////////////////////////////////////////////////////////////////////////////////

#include <osgGA/CameraManipulator>

////////////////////////////////////////////////////////////////////////////////

namespace cgi
{

/**
 * @brief Shift camera manipulator class.
 */
class ManipulatorShift : public osgGA::CameraManipulator
{
    typedef osgGA::CameraManipulator inherited;

public:

    /** @brief Constructor. */
    ManipulatorShift();

    void setByMatrix( const osg::Matrixd &matrix );
    void setByInverseMatrix( const osg::Matrixd &matrix );
    osg::Matrixd getMatrix() const;
    osg::Matrixd getInverseMatrix() const;

    bool handle( const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us );

    inline double getDistance() const { return _distance; }

    void setDistance( double distance );

    void setDistanceMin( double distance_min );
    void setDistanceMax( double distance_max );

private:

    osg::Matrixd _matrix;   ///<

    double _distance;       ///< [m]
    double _distance_min;   ///< [m]
    double _distance_max;   ///< [m]

    /** */
    void boundDistance();

    /** */
    void updateMatrix();
};

} // end of cgi namespace

////////////////////////////////////////////////////////////////////////////////

#endif // CGI_MANIPULATORSHIFT_H
