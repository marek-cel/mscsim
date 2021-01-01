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
#ifndef CGI_MANIPULATORMAP_H
#define CGI_MANIPULATORMAP_H

////////////////////////////////////////////////////////////////////////////////

#include <osgGA/CameraManipulator>

////////////////////////////////////////////////////////////////////////////////

namespace cgi
{

/**
 * @brief Map camera manipulator class.
 */
class ManipulatorMap : public osgGA::CameraManipulator
{
    typedef osgGA::CameraManipulator inherited;

public:

    /** @brief Constructor. */
    ManipulatorMap();

    virtual ~ManipulatorMap();

    void init( const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us );

    bool handle( const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us );

    void updateCamera( osg::Camera &camera );

    void setByMatrix( const osg::Matrixd & );
    void setByInverseMatrix( const osg::Matrixd & );
    osg::Matrixd getMatrix() const;
    osg::Matrixd getInverseMatrix() const;

    double getMouseLat() const { return _mouse_lat; }
    double getMouseLon() const { return _mouse_lon; }

    double getScale() const { return _scale; }

    void registerScaleChangeCallback( void(*fun)(double) );

    void setCenterX( double x );
    void setCenterY( double y );

    void setMapHeight( double map_height );

    void setMapMinX( double min );
    void setMapMinY( double min );
    void setMapMaxX( double max );
    void setMapMaxY( double max );

    void setScale( double scale );
    void setScaleMin( double min );
    void setScaleMax( double max );

    void setAllowThrow( bool allowThrow );

protected:

    osg::ref_ptr< const osgGA::GUIEventAdapter > _ga_t1;
    osg::ref_ptr< const osgGA::GUIEventAdapter > _ga_t0;

    osg::Vec3d _center;
    osg::Quat  _rotation;

    void(*_scaleChangeCallback)(double);

    double _map_height;     ///< [m] map height

    double _map_min_x;      ///< [m] map view left edge Mercator x-coordinate
    double _map_min_y;      ///< [m] map view right edge Mercator x-coordinate
    double _map_max_x;      ///< [m] map view bottom edge Mercator y-coordinate
    double _map_max_y;      ///< [m] map view top edge Mercator y-coordinate

    double _scale;          ///< [-]
    double _scale_min;      ///< [-]
    double _scale_max;      ///< [-]

    double _mouse_lat;      ///< [rad]
    double _mouse_lon;      ///< [rad]

    double _map_left;       ///< [m] current view left edge Mercator x-coordinate
    double _map_right;      ///< [m] current view right edge Mercator x-coordinate
    double _map_bottom;     ///< [m] current view bottom edge Mercator y-coordinate
    double _map_top;        ///< [m] current view top edge Mercator y-coordinate

    bool _thrown;
    bool _allowThrow;

    double _delta_frame_time;
    double _last_frame_time;

    double _wheelZoomFactor;

    bool handleFrame( const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us );
    bool handleResize( const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us );
    bool handleMouseMove( const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us );
    bool handleMouseDrag( const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us );
    bool handleMousePush( const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us );
    bool handleMouseRelease( const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us );
    bool handleMouseWheel( const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us );

    bool performMovement();
    bool performMovementLeftMouseButton( double delta_t, float dx, float dy );

    void panModel( float dx, float dy, float dz = 0.0 );
    void zoomModel( float dy );

    void addMouseEvent( const osgGA::GUIEventAdapter& ea );
    void flushMouseEventStack();

    bool isMouseMoving() const;
    float getThrowScale( double delta_t ) const;

    void updateCenterAndEdges( double w2h );
    void updateMousePosition( const osgGA::GUIEventAdapter &ea );
};

} // end of cgi namespace

////////////////////////////////////////////////////////////////////////////////

#endif // CGI_MANIPULATORMAP_H
