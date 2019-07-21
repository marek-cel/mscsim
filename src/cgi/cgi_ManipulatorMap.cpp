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

#include <cgi/cgi_ManipulatorMap.h>

#include <limits>

#include <cgi/cgi_Mercator.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

ManipulatorMap::ManipulatorMap() :
    osgGA::CameraManipulator(),

    m_scaleChangeCallback ( 0 ),

    m_map_height ( 1.0 ),

    m_map_min_x ( -1.0 ),
    m_map_min_y ( -1.0 ),
    m_map_max_x (  1.0 ),
    m_map_max_y (  1.0 ),

    m_scale ( 1.0 ),
    m_scale_min ( 0.0 ),
    m_scale_max ( std::numeric_limits< double >::max() ),

    m_mouse_lat ( 0.0 ),
    m_mouse_lon ( 0.0 ),

    m_map_left   ( -1.0 ),
    m_map_right  (  1.0 ),
    m_map_bottom ( -1.0 ),
    m_map_top    (  1.0 ),

    m_thrown ( false ),
    m_allowThrow ( true ),

    m_delta_frame_time ( 0.01 ),
    m_last_frame_time  ( 0.0 ),

    m_wheelZoomFactor ( 0.1 )
{}

////////////////////////////////////////////////////////////////////////////////

ManipulatorMap::~ManipulatorMap() {}

////////////////////////////////////////////////////////////////////////////////

void ManipulatorMap::init( const osgGA::GUIEventAdapter &/*ea*/, osgGA::GUIActionAdapter &us )
{
    flushMouseEventStack();

    m_thrown = false;
    us.requestContinuousUpdate( false );
}

////////////////////////////////////////////////////////////////////////////////

bool ManipulatorMap::handle( const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us )
{
    double y_norm = std::max( 0.0f, std::min( 1.0f, ea.getY() / ea.getWindowHeight() ) );
    double x_norm = std::max( 0.0f, std::min( 1.0f, ea.getX() / ea.getWindowWidth()  ) );

    double y_merc = ( m_map_top - m_map_bottom ) * y_norm + m_map_bottom;
    double x_merc = ( m_map_right - m_map_left ) * x_norm + m_map_left;

    m_mouse_lat = Mercator::getLat( y_merc );
    m_mouse_lon = Mercator::getLon( x_merc );

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
    /////////////////////////////////////////////////
    osgGA::CameraManipulator::updateCamera( camera );
    /////////////////////////////////////////////////

    double w2h = (double)(camera.getGraphicsContext()->getTraits()->width)
               / (double)(camera.getGraphicsContext()->getTraits()->height);

    double delta_y_2 = 0.5 * m_map_height * m_scale;
    double delta_x_2 = delta_y_2 * w2h;

    if ( m_center.x() - delta_x_2 < m_map_min_x ) m_center.x() = m_map_min_x + delta_x_2;
    if ( m_center.x() + delta_x_2 > m_map_max_x ) m_center.x() = m_map_max_x - delta_x_2;
    if ( m_center.y() - delta_y_2 < m_map_min_y ) m_center.y() = m_map_min_y + delta_y_2;
    if ( m_center.y() + delta_y_2 > m_map_max_y ) m_center.y() = m_map_max_y - delta_y_2;

    m_map_left   = m_center.x() - delta_x_2;
    m_map_right  = m_center.x() + delta_x_2;
    m_map_bottom = m_center.y() - delta_y_2;
    m_map_top    = m_center.y() + delta_y_2;

    camera.setProjectionMatrixAsOrtho2D( m_map_left, m_map_right,
                                         m_map_bottom, m_map_top );
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
    m_scaleChangeCallback = fun;
}

////////////////////////////////////////////////////////////////////////////////

void ManipulatorMap::setMapHeight( double map_height )
{
    if ( map_height > 0.0 )
    {
        m_map_height = map_height;
    }
}

////////////////////////////////////////////////////////////////////////////////

void ManipulatorMap::setMapMinX( double min )
{
    if ( min < m_map_max_x )
    {
        m_map_min_x = min;
    }

    m_map_left = m_map_min_x;
}

////////////////////////////////////////////////////////////////////////////////

void ManipulatorMap::setMapMinY( double min )
{
    if ( min < m_map_max_y )
    {
        m_map_min_y = min;
    }

    m_map_bottom = m_map_min_y;
}

////////////////////////////////////////////////////////////////////////////////

void ManipulatorMap::setMapMaxX( double max )
{
    if ( max > m_map_min_x )
    {
        m_map_max_x = max;
    }

    m_map_right = m_map_max_x;
}

////////////////////////////////////////////////////////////////////////////////

void ManipulatorMap::setMapMaxY( double max )
{
    if ( max > m_map_min_y )
    {
        m_map_max_y = max;
    }

    m_map_top = m_map_max_y;
}

////////////////////////////////////////////////////////////////////////////////

void ManipulatorMap::setScaleMin( double min )
{
    if ( min >= 0.0 && min < m_scale_max )
    {
        m_scale_min = min;
    }
}

////////////////////////////////////////////////////////////////////////////////

void ManipulatorMap::setScaleMax( double max )
{
    if ( max >= 0.0 && max > m_scale_min )
    {
        m_scale_max = max;
    }
}

////////////////////////////////////////////////////////////////////////////////

void ManipulatorMap::setAllowThrow( bool allowThrow )
{
    m_allowThrow = allowThrow;
}

////////////////////////////////////////////////////////////////////////////////

bool ManipulatorMap::handleFrame( const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us )
{
    double current_frame_time = ea.getTime();

    m_delta_frame_time = current_frame_time - m_last_frame_time;
    m_last_frame_time = current_frame_time;

    if ( m_thrown && performMovement() )
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

bool ManipulatorMap::handleMouseMove( const osgGA::GUIEventAdapter &/*ea*/, osgGA::GUIActionAdapter &/*us*/ )
{
    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool ManipulatorMap::handleMouseDrag( const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us )
{
    addMouseEvent( ea );

    if ( performMovement() ) us.requestRedraw();

    us.requestContinuousUpdate( false );
    m_thrown = false;

    return true;
}

////////////////////////////////////////////////////////////////////////////////

bool ManipulatorMap::handleMousePush( const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us )
{
    flushMouseEventStack();
    addMouseEvent( ea );

    if ( performMovement() ) us.requestRedraw();

    us.requestContinuousUpdate( false );
    m_thrown = false;

    return true;
}

////////////////////////////////////////////////////////////////////////////////

bool ManipulatorMap::handleMouseRelease( const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us )
{
    if ( ea.getButtonMask() == 0 )
    {

        double timeSinceLastRecordEvent = m_ga_t0.valid() ? ( ea.getTime() - m_ga_t0->getTime() ) : DBL_MAX;

        if( timeSinceLastRecordEvent > 0.02 ) flushMouseEventStack();

        if ( isMouseMoving() )
        {

            if ( performMovement() && m_allowThrow )
            {
                us.requestRedraw();
                us.requestContinuousUpdate( true );
                m_thrown = true;
            }

            return true;
        }
    }

    flushMouseEventStack();
    addMouseEvent( ea );

    if ( performMovement() ) us.requestRedraw();

    us.requestContinuousUpdate( false );
    m_thrown = false;

    return true;
}

////////////////////////////////////////////////////////////////////////////////

bool ManipulatorMap::handleMouseWheel( const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us )
{
    osgGA::GUIEventAdapter::ScrollingMotion sm = ea.getScrollingMotion();

    switch ( sm )
    {
    case osgGA::GUIEventAdapter::SCROLL_UP:
        zoomModel( -m_wheelZoomFactor );
        us.requestRedraw();
        return true;

    case osgGA::GUIEventAdapter::SCROLL_DOWN:
        zoomModel(  m_wheelZoomFactor );
        us.requestRedraw();
        return true;

    default:
        return false;
   }
}

////////////////////////////////////////////////////////////////////////////////

bool ManipulatorMap::performMovement()
{
    if ( m_ga_t0.valid() && m_ga_t1.valid() )
    {
        // get delta time
        double delta_t = std::max( 0.0, m_ga_t0->getTime() - m_ga_t1->getTime() );

        float dx = m_ga_t0->getXnormalized() - m_ga_t1->getXnormalized();
        float dy = m_ga_t0->getYnormalized() - m_ga_t1->getYnormalized();

        if ( m_ga_t1->getButtonMask() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON )
        {
            return performMovementLeftMouseButton( delta_t, dx, dy );
        }
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool ManipulatorMap::performMovementLeftMouseButton( double delta_t, float dx, float dy )
{
    float scale = -0.5 * ( 0.5 * m_map_height * m_scale ) * getThrowScale( delta_t );;
    panModel( dx*scale, dy*scale );
    return true;
}

////////////////////////////////////////////////////////////////////////////////

void ManipulatorMap::panModel( float dx, float dy, float dz )
{
    osg::Matrix rotation_matrix;
    rotation_matrix.makeRotate( m_rotation );

    osg::Vec3d dv( dx, dy, dz );

    m_center += dv * rotation_matrix;
}

////////////////////////////////////////////////////////////////////////////////

void ManipulatorMap::zoomModel( const float dy )
{
    m_scale *= 1.0 + dy;

    if ( m_scale > m_scale_max ) m_scale = m_scale_max;
    if ( m_scale < m_scale_min ) m_scale = m_scale_min;

    if ( m_scaleChangeCallback )
    {
        (*m_scaleChangeCallback)( m_scale );
    }
}

////////////////////////////////////////////////////////////////////////////////

void ManipulatorMap::addMouseEvent( const osgGA::GUIEventAdapter& ea )
{
    m_ga_t1 = m_ga_t0;
    m_ga_t0 = &ea;
}

////////////////////////////////////////////////////////////////////////////////

void ManipulatorMap::flushMouseEventStack()
{
    m_ga_t1 = 0;
    m_ga_t0 = 0;
}

////////////////////////////////////////////////////////////////////////////////

bool ManipulatorMap::isMouseMoving() const
{
    if ( m_ga_t0.valid() && m_ga_t1.valid() )
    {
        static const float velocity = 0.1f;

        float dx = m_ga_t0->getXnormalized() - m_ga_t1->getXnormalized();
        float dy = m_ga_t0->getYnormalized() - m_ga_t1->getYnormalized();
        float len = sqrt( dx*dx + dy*dy );
        float dt = m_ga_t0->getTime() - m_ga_t1->getTime();

        return ( len > dt*velocity );
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////

float ManipulatorMap::getThrowScale( double delta_t ) const
{
    if ( m_thrown )
    {
        if ( delta_t == 0.0 )
        {
            return 0.0f;
        }

        return (float)( m_delta_frame_time / delta_t );
    }

    return 1.0f;
}
