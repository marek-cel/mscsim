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
#ifndef CGI_CAMERA_H
#define CGI_CAMERA_H

////////////////////////////////////////////////////////////////////////////////

#include <Data.h>

#include <cgi/cgi_ManipulatorOrbit.h>
#include <cgi/cgi_ManipulatorShift.h>
#include <cgi/cgi_ManipulatorWorld.h>

////////////////////////////////////////////////////////////////////////////////

namespace cgi
{

/**
 * @brief Camera control class.
 */
class Camera
{
public:

    static const double _downAngle;     ///< [rad]

    /** Constructor. */
    Camera();

    /** Destructor. */
    virtual ~Camera();

    /** Updates camera position and attitude. */
    void update();

    /** */
    inline osgGA::CameraManipulator* getManipulator()
    {
        return _manipulator.get();
    }

    inline osg::Vec3d getPosition() const { return _position; }
    inline osg::Quat  getAttitude() const { return _attitude; }

    /** Returns true if orbit manipulator track node is valid. */
    inline bool isTrackNodeValid()
    {
        return _trackNode.valid();
    }

    /** Returns true if world manipulator node is valid. */
    inline bool isWorldNodeValid()
    {
        return _worldNode.valid();
    }

    /** Sets default distance to aircraft. */
    void setDistanceDef( double distance_def );

    /** Sets minimum distance to aircraft. */
    void setDistanceMin( double distance_min );

    /** Sets orbit manipulator track node. */
    void setTrackNode( osg::Node *node );

    /** Sets world manipulator node. */
    void setWorldNode( osg::Node *node );

    /** Sets chase view. */
    inline void setViewChase()
    {
        _viewType = Data::Camera::ViewChase;
        _manipulator = _manipulatorShift.get();
    }

    /** Sets orbit view. */
    inline void setViewOrbit()
    {
        _viewType = Data::Camera::ViewOrbit;
        _manipulator = _manipulatorOrbit.get();
    }

    /** Sets pilot view. */
    inline void setViewPilot()
    {
        _viewType = Data::Camera::ViewPilot;
        _manipulator = _manipulatorShift.get();
    }

    /** Sets world view. */
    inline void setViewWorld()
    {
        _viewType = Data::Camera::ViewWorld;
        _manipulator = _manipulatorWorld.get();
    }

private:

    typedef Data::Camera::ViewType ViewType;

    osg::ref_ptr<osgGA::CameraManipulator> _manipulator;    ///< current manipulator

    osg::ref_ptr<ManipulatorOrbit> _manipulatorOrbit;       ///< orbit view manipulator
    osg::ref_ptr<ManipulatorShift> _manipulatorShift;       ///< shift view manipulator
    osg::ref_ptr<ManipulatorWorld> _manipulatorWorld;       ///< world view manipulator

    osg::ref_ptr<osg::Node> _trackNode;     ///< orbit manipulator track node
    osg::ref_ptr<osg::Node> _worldNode;     ///< world manipulator node

    osg::Vec3d _position;   ///<
    osg::Quat  _attitude;   ///<

    ViewType _viewType;     ///<
};

} // end of cgi namespace

////////////////////////////////////////////////////////////////////////////////

#endif // CGI_CAMERA_H
