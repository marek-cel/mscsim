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

#include <gui/WidgetCGI.h>

#include <osg/PositionAttitudeTransform>
#include <osgDB/ReadFile>
#include <osgViewer/ViewerEventHandlers>

#include <fdm/utils/fdm_WGS84.h>

#include <cgi/cgi_Defines.h>
#include <cgi/cgi_Manager.h>
#include <cgi/cgi_WGS84.h>

#include <gui/gui_Defines.h>

////////////////////////////////////////////////////////////////////////////////

const double WidgetCGI::_zNear = 0.55;
const double WidgetCGI::_zFar  = CGI_SKYDOME_RADIUS + 0.1f * CGI_SKYDOME_RADIUS;

////////////////////////////////////////////////////////////////////////////////

WidgetCGI::WidgetCGI( QWidget *parent ) :
    QWidget ( parent ),
    _gridLayout ( 0 ),
    _timerId ( 0 ),
    _camManipulatorInited ( false )
{
#   ifdef SIM_OSG_DEBUG_INFO
    osg::setNotifyLevel( osg::DEBUG_INFO );
#   else
    osg::setNotifyLevel( osg::WARN );
#   endif

    cgi::Manager::instance()->setCameraManipulatorPilot();

    setThreadingModel( osgViewer::ViewerBase::SingleThreaded );
    //setThreadingModel( osgViewer::ViewerBase::ThreadPerContext );

    _graphicsWindow = createGraphicsWindow( x(), y(), width(), height() );

    QWidget *widget = addViewWidget();

    _gridLayout = new QGridLayout( this );
    _gridLayout->setContentsMargins( 1, 1, 1, 1 );
    _gridLayout->addWidget( widget, 0, 0 );

    _keyHandler = new KeyHandler( this );
    getEventHandlers().push_front( _keyHandler.get() );

    setLayout( _gridLayout );

    _timerId = startTimer( 1000.0 * CGI_TIME_STEP );
}

////////////////////////////////////////////////////////////////////////////////

WidgetCGI::~WidgetCGI()
{
    if ( _timerId ) killTimer( _timerId );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetCGI::setCameraManipulatorChase()
{
    cgi::Manager::instance()->setCameraManipulatorChase();
    setCameraManipulator( cgi::Manager::instance()->getCameraManipulator() );

    _camManipulatorInited = false;
}

////////////////////////////////////////////////////////////////////////////////

void WidgetCGI::setCameraManipulatorOrbit()
{
    cgi::Manager::instance()->setCameraManipulatorOrbit();
    setCameraManipulator( cgi::Manager::instance()->getCameraManipulator() );

    _camManipulatorInited = false;
}

////////////////////////////////////////////////////////////////////////////////

void WidgetCGI::setCameraManipulatorPilot()
{
    cgi::Manager::instance()->setCameraManipulatorPilot();
    setCameraManipulator( cgi::Manager::instance()->getCameraManipulator() );

    _camManipulatorInited = false;
}

////////////////////////////////////////////////////////////////////////////////

void WidgetCGI::setCameraManipulatorWorld()
{
    osg::Vec3d eye = cgi::Manager::instance()->getCameraManipulator()->getMatrix().getTrans();
    osg::Vec3d center( 0.0, 0.0, 0.0 );
    osg::Vec3d up( 0.0, 0.0, 1.0 );

    cgi::Manager::instance()->setCameraManipulatorWorld();
    setCameraManipulator( cgi::Manager::instance()->getCameraManipulator() );

    if ( _camManipulatorInited )
    {
        cgi::WGS84 eye_wgs( eye );

        if ( eye_wgs.getAlt() < 10000.0 )
        {
            eye_wgs.setAlt( 10000.0 );
        }

        eye = eye_wgs.getPosition();
        up  = eye_wgs.getAttitude().inverse() * osg::Vec3( 1.0, 0.0, 0.0 );

        osg::ref_ptr<cgi::ManipulatorWorld> manipulator =
                dynamic_cast<cgi::ManipulatorWorld*>( cgi::Manager::instance()->getCameraManipulator() );

        if ( manipulator.valid() )
        {
            manipulator->setTransformation( eye, center, up );
        }
    }

    _camManipulatorInited = false;
}

////////////////////////////////////////////////////////////////////////////////

void WidgetCGI::setDistanceMin( double distance_min )
{
    cgi::Manager::instance()->setDistanceMin( distance_min );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetCGI::setDistanceDef( double distance_def )
{
    cgi::Manager::instance()->setDistanceDef( distance_def );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetCGI::paintEvent( QPaintEvent *event )
{
    /////////////////////////////
    QWidget::paintEvent( event );
    /////////////////////////////

    frame();
}

////////////////////////////////////////////////////////////////////////////////

void WidgetCGI::timerEvent( QTimerEvent *event )
{
    /////////////////////////////
    QWidget::timerEvent( event );
    /////////////////////////////

    update();

    if ( !_camManipulatorInited )
    {
        osg::ref_ptr<cgi::ManipulatorOrbit> manipulator =
                dynamic_cast<cgi::ManipulatorOrbit*>( cgi::Manager::instance()->getCameraManipulator() );

        if ( manipulator.valid() )
        {
            manipulator->setDistance( 50.0 );
        }

        _camManipulatorInited = true;
    }

    cgi::Manager::instance()->updateHUD();
    cgi::Manager::instance()->updateOTW();

    _keyHandler->update();
}

////////////////////////////////////////////////////////////////////////////////

QWidget* WidgetCGI::addViewWidget()
{
    createCameraOTW();
    createCameraHUD();

    setSceneData( cgi::Manager::instance()->getNodeOTW() );
    addEventHandler( new osgViewer::StatsHandler );
    setCameraManipulator( cgi::Manager::instance()->getCameraManipulator() );

    setKeyEventSetsDone( 0 );

    assignSceneDataToCameras();

    return _graphicsWindow->getGLWidget();
}

////////////////////////////////////////////////////////////////////////////////

void WidgetCGI::createCameraOTW()
{
    osg::ref_ptr<osg::Camera> cameraOTW = getCamera();

    cameraOTW->setGraphicsContext( _graphicsWindow );

    const osg::GraphicsContext::Traits *traits = _graphicsWindow->getTraits();

    double w2h = (double)(traits->width) / (double)(traits->height);

    cameraOTW->setClearColor( osg::Vec4( 0.0, 0.0, 0.0, 1.0 ) );
    cameraOTW->setViewport( new osg::Viewport( 0, 0, traits->width, traits->height ) );
    cameraOTW->setProjectionMatrixAsPerspective( CGI_FOV_Y, w2h, _zNear, _zFar );
    cameraOTW->setNearFarRatio( _zNear / _zFar );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetCGI::createCameraHUD()
{
    osg::ref_ptr<osg::Camera> cameraHUD = new osg::Camera();

    cameraHUD->setGraphicsContext( _graphicsWindow );

    const osg::GraphicsContext::Traits *traits = _graphicsWindow->getTraits();

    double w2h = (double)(traits->width) / (double)(traits->height);

    cameraHUD->setProjectionMatrixAsOrtho2D( -CGI_HUD_Y_2 * w2h, CGI_HUD_Y_2 * w2h, -CGI_HUD_Y_2, CGI_HUD_Y_2 );
    cameraHUD->setReferenceFrame( osg::Transform::ABSOLUTE_RF );
    cameraHUD->setViewMatrix( osg::Matrix::identity() );
    cameraHUD->setClearMask( GL_DEPTH_BUFFER_BIT );
    cameraHUD->setRenderOrder( osg::Camera::POST_RENDER );
    cameraHUD->setAllowEventFocus( false );
    cameraHUD->setProjectionResizePolicy( osg::Camera::HORIZONTAL );
    cameraHUD->addChild( cgi::Manager::instance()->getNodeHUD() );
    cameraHUD->setViewport( new osg::Viewport( 0, 0, traits->width, traits->height ) );

    addSlave( cameraHUD, false );
}

////////////////////////////////////////////////////////////////////////////////

osg::ref_ptr<osgQt::GraphicsWindowQt> WidgetCGI::createGraphicsWindow( int x, int y, int w, int h )
{
    osg::DisplaySettings *displaySettings = osg::DisplaySettings::instance().get();
    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits();

    traits->windowName       = "";
    traits->windowDecoration = false;
    traits->x                = x;
    traits->y                = y;
    traits->width            = w;
    traits->height           = h;
    traits->doubleBuffer     = true;
    traits->alpha            = displaySettings->getMinimumNumAlphaBits();
    traits->stencil          = displaySettings->getMinimumNumStencilBits();
    traits->sampleBuffers    = displaySettings->getMultiSamples();
    traits->samples          = 4;
#   ifdef SIM_VERTICALSYNC
    traits->vsync            = true;
#   else
    traits->vsync            = false;
#   endif

    osg::ref_ptr<osgQt::GraphicsWindowQt> graphicsWindow = new osgQt::GraphicsWindowQt( traits.get() );

    return graphicsWindow;
}
