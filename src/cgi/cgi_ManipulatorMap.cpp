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

#include <cgi/cgi_ManipulatorMap.h>

#include <limits>

#include <defs.h>

#include <cgi/cgi_Mercator.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

ManipulatorMap::ManipulatorMap() :
    inherited(),

    _scaleChangeCallback ( NULLPTR ),

    _map_height ( 1.0 ),

    _map_min_x ( -1.0 ),
    _map_min_y ( -1.0 ),
    _map_max_x (  1.0 ),
    _map_max_y (  1.0 ),

    _scale ( 1.0 ),
    _scale_min ( 0.0 ),
    _scale_max ( std::numeric_limits< double >::max() ),

    _mouse_lat ( 0.0 ),
    _mouse_lon ( 0.0 ),

    _map_left   ( -1.0 ),
    _map_right  (  1.0 ),
    _map_bottom ( -1.0 ),
    _map_top    (  1.0 ),

    _thrown ( false ),
    _allowThrow ( true ),

    _delta_frame_time ( 0.01 ),
    _last_frame_time  ( 0.0 ),

    _wheelZoomFactor ( 0.1 )
{}

////////////////////////////////////////////////////////////////////////////////

ManipulatorMap::~ManipulatorMap() {}

////////////////////////////////////////////////////////////////////////////////

void ManipulatorMap::init( const osgGA::GUIEventAdapter &/*ea*/, osgGA::GUIActionAdapter &us )
{
    flushMouseEventStack();

    _thrown = false;
    us.requestContinuousUpdate( false );
}

////////////////////////////////////////////////////////////////////////////////

bool ManipulatorMap::handle( const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us )
{
    switch ( ea.getEventType() )
    {

    case osgGA::GUIEventAdapter::FRAME:
        return handleFrame( ea, us );

    case osgGA::GUIEventAdapter::RESIZE:
        return handleResize( ea, us );

    default:
        break;
   }

    if ( ea.getHandled() )
    {
        return false;
    }

    switch ( ea.getEventType() )
    {
    case osgGA::GUIEventAdapter::MOVE:
        return handleMouseMove( ea, us );

    case osgGA::GUIEventAdapter::DRAG:
        return handleMouseDrag( ea, us );

    case osgGA::GUIEventAdapter::PUSH:
        return handleMousePush( ea, us );

    case osgGA::GUIEventAdapter::RELEASE:
        return handleMouseRelease( ea, us );

    case osgGA::GUIEventAdapter::SCROLL:
        return handleMouseWheel( ea, us );

    default:
        return false;
    }
}

////////////////////////////////////////////////////////////////////////////////

void ManipulatorMap::updateCamera( osg::Camera &camera )
{
    //////////////////////////////////
    inherited::updateCamera( camera );
    //////////////////////////////////

    double w2h = (double)(camera.getGraphicsContext()->getTraits()->width)
               / (double)(camera.getGraphicsContext()->getTraits()->height);

    updateCenterAndEdges( w2h );

    camera.setProjectionMatrixAsOrtho2D( _map_left, _map_right,
                                         _map_bottom, _map_top );
}

////////////////////////////////////////////////////////////////////////////////

void ManipulatorMap::setByMatrix( const osg::Matrixd &/*matrix*/ ) {}

////////////////////////////////////////////////////////////////////////////////

void ManipulatorMap::setByInverseMatrix( const osg::Matrixd &/*matrix*/ ) {}

////////////////////////////////////////////////////////////////////////////////

osg::Matrixd ManipulatorMap::getMatrix() const
{
    return osg::Matrixd::identity();
}

////////////////////////////////////////////////////////////////////////////////

osg::Matrixd ManipulatorMap::getInverseMatrix() const
{
    return osg::Matrixd::identity();
}

////////////////////////////////////////////////////////////////////////////////

void ManipulatorMap::registerScaleChangeCallback( void(*fun)(double) )
{
    _scaleChangeCallback = fun;
}

////////////////////////////////////////////////////////////////////////////////

void ManipulatorMap::setCenterX( double x )
{
    _center.x() = x;
}

////////////////////////////////////////////////////////////////////////////////

void ManipulatorMap::setCenterY( double y )
{
    _center.y() = y;
}

////////////////////////////////////////////////////////////////////////////////

void ManipulatorMap::setMapHeight( double map_height )
{
    if ( map_height > 0.0 )
    {
        _map_height = map_height;
    }
}

////////////////////////////////////////////////////////////////////////////////

void ManipulatorMap::setMapMinX( double min )
{
    if ( min < _map_max_x )
    {
        _map_min_x = min;
    }

    _map_left = _map_min_x;
}

////////////////////////////////////////////////////////////////////////////////

void ManipulatorMap::setMapMinY( double min )
{
    if ( min < _map_max_y )
    {
        _map_min_y = min;
    }

    _map_bottom = _map_min_y;
}

////////////////////////////////////////////////////////////////////////////////

void ManipulatorMap::setMapMaxX( double max )
{
    if ( max > _map_min_x )
    {
        _map_max_x = max;
    }

    _map_right = _map_max_x;
}

////////////////////////////////////////////////////////////////////////////////

void ManipulatorMap::setMapMaxY( double max )
{
    if ( max > _map_min_y )
    {
        _map_max_y = max;
    }

    _map_top = _map_max_y;
}

////////////////////////////////////////////////////////////////////////////////

void ManipulatorMap::setScale( double scale )
{
    _scale = scale;

    if ( _scale > _scale_max ) _scale = _scale_max;
    if ( _scale < _scale_min ) _scale = _scale_min;

    if ( _scaleChangeCallback )
    {
        (*_scaleChangeCallback)( _scale );
    }
}

////////////////////////////////////////////////////////////////////////////////

void ManipulatorMap::setScaleMin( double min )
{
    if ( min >= 0.0 && min < _scale_max )
    {
        _scale_min = min;
    }
}

////////////////////////////////////////////////////////////////////////////////

void ManipulatorMap::setScaleMax( double max )
{
    if ( max >= 0.0 && max > _scale_min )
    {
        _scale_max = max;
    }
}

////////////////////////////////////////////////////////////////////////////////

void ManipulatorMap::setAllowThrow( bool allowThrow )
{
    _allowThrow = allowThrow;
}

////////////////////////////////////////////////////////////////////////////////

bool ManipulatorMap::handleFrame( const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us )
{
    double current_frame_time = ea.getTime();

    _delta_frame_time = current_frame_time - _last_frame_time;
    _last_frame_time = current_frame_time;

    if ( _thrown && performMovement() )
    {
        us.requestRedraw();
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool ManipulatorMap::handleResize( const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us )
{
    init( ea, us );
    us.requestRedraw();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool ManipulatorMap::handleMouseMove( const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &/*us*/ )
{
    updateMousePosition( ea );
    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool ManipulatorMap::handleMouseDrag( const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us )
{
    updateMousePosition( ea );
    addMouseEvent( ea );

    if ( performMovement() ) us.requestRedraw();

    us.requestContinuousUpdate( false );
    _thrown = false;

    return true;
}

////////////////////////////////////////////////////////////////////////////////

bool ManipulatorMap::handleMousePush( const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us )
{
    updateMousePosition( ea );
    flushMouseEventStack();
    addMouseEvent( ea );

    if ( performMovement() ) us.requestRedraw();

    us.requestContinuousUpdate( false );
    _thrown = false;

    return true;
}

////////////////////////////////////////////////////////////////////////////////

bool ManipulatorMap::handleMouseRelease( const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us )
{
    updateMousePosition( ea );

    if ( ea.getButtonMask() == 0 )
    {
        double timeSinceLastRecordEvent = _ga_t0.valid() ? ( ea.getTime() - _ga_t0->getTime() ) : DBL_MAX;

        if( timeSinceLastRecordEvent > 0.02 ) flushMouseEventStack();

        if ( isMouseMoving() )
        {
            if ( performMovement() && _allowThrow )
            {
                us.requestRedraw();
                us.requestContinuousUpdate( true );
                _thrown = true;
            }

            return true;
        }
    }

    flushMouseEventStack();
    addMouseEvent( ea );

    if ( performMovement() ) us.requestRedraw();

    us.requestContinuousUpdate( false );
    _thrown = false;

    return true;
}

////////////////////////////////////////////////////////////////////////////////

bool ManipulatorMap::handleMouseWheel( const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us )
{
    updateMousePosition( ea );

    osgGA::GUIEventAdapter::ScrollingMotion sm = ea.getScrollingMotion();

    switch ( sm )
    {
    case osgGA::GUIEventAdapter::SCROLL_UP:
        zoomModel( -_wheelZoomFactor );
        us.requestRedraw();
        return true;

    case osgGA::GUIEventAdapter::SCROLL_DOWN:
        zoomModel(  _wheelZoomFactor );
        us.requestRedraw();
        return true;

    default:
        return false;
   }
}

////////////////////////////////////////////////////////////////////////////////

bool ManipulatorMap::performMovement()
{
    if ( _ga_t0.valid() && _ga_t1.valid() )
    {
        // get delta time
        double delta_t = std::max( 0.0, _ga_t0->getTime() - _ga_t1->getTime() );

        float dx = _ga_t0->getXnormalized() - _ga_t1->getXnormalized();
        float dy = _ga_t0->getYnormalized() - _ga_t1->getYnormalized();

        if ( _ga_t1->getButtonMask() & osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON ) // due to contex menu issues
        {
            return performMovementLeftMouseButton( delta_t, dx, dy );
        }
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool ManipulatorMap::performMovementLeftMouseButton( double delta_t, float dx, float dy )
{
    double scale = -0.5 * ( 0.5 * _map_height * _scale ) * getThrowScale( delta_t );;
    panModel( dx*scale, dy*scale );
    return true;
}

////////////////////////////////////////////////////////////////////////////////

void ManipulatorMap::panModel( float dx, float dy, float dz )
{
    osg::Matrix rotation_matrix;
    rotation_matrix.makeRotate( _rotation );

    osg::Vec3d dv( dx, dy, dz );

    _center += dv * rotation_matrix;
}

////////////////////////////////////////////////////////////////////////////////

void ManipulatorMap::zoomModel( const float dy )
{
    setScale( _scale * ( 1.0 + dy ) );
}

////////////////////////////////////////////////////////////////////////////////

void ManipulatorMap::addMouseEvent( const osgGA::GUIEventAdapter& ea )
{
    _ga_t1 = _ga_t0;
    _ga_t0 = &ea;
}

////////////////////////////////////////////////////////////////////////////////

void ManipulatorMap::flushMouseEventStack()
{
    _ga_t1 = 0;
    _ga_t0 = 0;
}

////////////////////////////////////////////////////////////////////////////////

bool ManipulatorMap::isMouseMoving() const
{
    if ( _ga_t0.valid() && _ga_t1.valid() )
    {
        static const float velocity = 0.1f;

        float dx = _ga_t0->getXnormalized() - _ga_t1->getXnormalized();
        float dy = _ga_t0->getYnormalized() - _ga_t1->getYnormalized();
        float len = sqrt( dx*dx + dy*dy );
        float dt = _ga_t0->getTime() - _ga_t1->getTime();

        return ( len > dt*velocity );
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////

float ManipulatorMap::getThrowScale( double delta_t ) const
{
    if ( _thrown )
    {
        if ( delta_t == 0.0 )
        {
            return 0.0f;
        }

        return (float)( _delta_frame_time / delta_t );
    }

    return 1.0f;
}

////////////////////////////////////////////////////////////////////////////////

void ManipulatorMap::updateCenterAndEdges( double w2h )
{
    double delta_y_2 = 0.5 * _map_height * _scale;
    double delta_x_2 = delta_y_2 * w2h;

    if ( _center.x() - delta_x_2 < _map_min_x )
    {
        _thrown = false;
        _center.x() = _map_min_x + delta_x_2;
    }

    if ( _center.x() + delta_x_2 > _map_max_x )
    {
        _thrown = false;
        _center.x() = _map_max_x - delta_x_2;
    }

    if ( _center.y() - delta_y_2 < _map_min_y )
    {
        _thrown = false;
        _center.y() = _map_min_y + delta_y_2;
    }

    if ( _center.y() + delta_y_2 > _map_max_y )
    {
        _thrown = false;
        _center.y() = _map_max_y - delta_y_2;
    }

    _map_left   = _center.x() - delta_x_2;
    _map_right  = _center.x() + delta_x_2;
    _map_bottom = _center.y() - delta_y_2;
    _map_top    = _center.y() + delta_y_2;
}

////////////////////////////////////////////////////////////////////////////////

void ManipulatorMap::updateMousePosition( const osgGA::GUIEventAdapter &ea )
{
    double x_norm = std::max( 0.0f, std::min( 1.0f, ea.getX() / ea.getWindowWidth()  ) );
    double y_norm = std::max( 0.0f, std::min( 1.0f, ea.getY() / ea.getWindowHeight() ) );

    double x_merc = ( _map_right - _map_left ) * x_norm + _map_left;
    double y_merc = ( _map_top - _map_bottom ) * y_norm + _map_bottom;

    _mouse_lat = Mercator::lat( y_merc );
    _mouse_lon = Mercator::lon( x_merc );
}
