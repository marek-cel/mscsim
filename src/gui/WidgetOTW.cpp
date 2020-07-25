/****************************************************************************//*
 * Copyright (C) 2020 Marek M. Cel
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

#include <gui/WidgetOTW.h>

#include <osgGA/StateSetManipulator>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <fdm/utils/fdm_WGS84.h>

#include <cgi/cgi_Defines.h>
#include <cgi/cgi_Manager.h>
#include <cgi/cgi_WGS84.h>

#include <gui/gui_Defines.h>

////////////////////////////////////////////////////////////////////////////////

const double WidgetOTW::_zNear = 0.55;
const double WidgetOTW::_zFar  = CGI_SKYDOME_RADIUS + 0.1f * CGI_SKYDOME_RADIUS;

////////////////////////////////////////////////////////////////////////////////

WidgetOTW::WidgetOTW( QWidget *parent ) :
    WidgetOSG ( parent ),
    _timerId ( 0 ),
    _camManipulatorInited ( false )
{
    _keyHandler = new KeyHandler( this );

#   ifdef SIM_NEW_OSG_QT
    QObject::connect( this, &WidgetOTW::initialized, [ this ]
    {
        getOsgViewer()->setThreadingModel( osgViewer::ViewerBase::SingleThreaded );
        //getOsgViewer()->setThreadingModel( osgViewer::ViewerBase::ThreadPerContext );

        createCameraOTW();
        createCameraHUD();

        // add the state manipulator
        getOsgViewer()->addEventHandler( new osgGA::StateSetManipulator( getOsgViewer()->getCamera()->getOrCreateStateSet() ) );

        // add the thread model handler
        getOsgViewer()->addEventHandler( new osgViewer::ThreadingHandler );

        // add the window size toggle handler
        getOsgViewer()->addEventHandler( new osgViewer::WindowSizeHandler );

        // add the stats handler
        getOsgViewer()->addEventHandler( new osgViewer::StatsHandler );

        // add the record camera path handler
        getOsgViewer()->addEventHandler( new osgViewer::RecordCameraPathHandler );

        // add the LOD Scale handler
        getOsgViewer()->addEventHandler( new osgViewer::LODScaleHandler );

        // add the screen capture handler
        getOsgViewer()->addEventHandler( new osgViewer::ScreenCaptureHandler );

        getOsgViewer()->getEventHandlers().push_front( _keyHandler.get() );

        getOsgViewer()->setKeyEventSetsDone( 0 );

        ///////////////////////////////////////////////////////////////////////
        getOsgViewer()->setSceneData( cgi::Manager::instance()->getNodeOTW() );
        ///////////////////////////////////////////////////////////////////////

        getOsgViewer()->assignSceneDataToCameras();

        _initialized = true;

        setCameraManipulatorPilot();
    });
#   else
    QWidget *widget = addViewWidget();

    _layout = new QGridLayout( this );
    _layout->setContentsMargins( 1, 1, 1, 1 );
    _layout->addWidget( widget, 0, 0 );

    setLayout( _layout );

    getEventHandlers().push_front( _keyHandler.get() );

    setCameraManipulatorPilot();
#   endif
}

////////////////////////////////////////////////////////////////////////////////

WidgetOTW::~WidgetOTW()
{
    if ( _timerId ) killTimer( _timerId );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetOTW::init()
{
    _timerId = startTimer( 1000.0 * CGI_TIME_STEP );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetOTW::setCameraManipulatorChase()
{
    if ( _initialized )
    {
        cgi::Manager::instance()->setCameraManipulatorChase();
        getOsgViewer()->setCameraManipulator( cgi::Manager::instance()->getCameraManipulator() );

        _camManipulatorInited = false;
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetOTW::setCameraManipulatorOrbit()
{
    if ( _initialized )
    {
        cgi::Manager::instance()->setCameraManipulatorOrbit();
        getOsgViewer()->setCameraManipulator( cgi::Manager::instance()->getCameraManipulator() );

        _camManipulatorInited = false;
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetOTW::setCameraManipulatorPilot()
{
    if ( _initialized )
    {
        cgi::Manager::instance()->setCameraManipulatorPilot();
        getOsgViewer()->setCameraManipulator( cgi::Manager::instance()->getCameraManipulator() );

        _camManipulatorInited = false;
    }
}

////////////////////////////////////////////////////////////////////////////////

void WidgetOTW::setCameraManipulatorWorld()
{
    if ( _initialized )
    {
        osg::Vec3d eye = cgi::Manager::instance()->getCameraManipulator()->getMatrix().getTrans();
        osg::Vec3d center( 0.0, 0.0, 0.0 );
        osg::Vec3d up( 0.0, 0.0, 1.0 );

        cgi::Manager::instance()->setCameraManipulatorWorld();
        getOsgViewer()->setCameraManipulator( cgi::Manager::instance()->getCameraManipulator() );

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
}

////////////////////////////////////////////////////////////////////////////////

void WidgetOTW::setDistanceMin( double distance_min )
{
    cgi::Manager::instance()->setDistanceMin( distance_min );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetOTW::setDistanceDef( double distance_def )
{
    cgi::Manager::instance()->setDistanceDef( distance_def );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetOTW::timerEvent( QTimerEvent *event )
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

#ifndef SIM_NEW_OSG_QT
QWidget* WidgetOTW::addViewWidget()
{
    createCameraOTW();
    createCameraHUD();

    setSceneData( cgi::Manager::instance()->getNodeOTW() );
    addEventHandler( new osgViewer::StatsHandler );
    setCameraManipulator( cgi::Manager::instance()->getCameraManipulator() );

    setKeyEventSetsDone( 0 );

    assignSceneDataToCameras();

    return _gwin->getGLWidget();
}
#endif

////////////////////////////////////////////////////////////////////////////////

void WidgetOTW::createCameraOTW()
{
    osg::ref_ptr<osg::Camera> cameraOTW = getOsgViewer()->getCamera();

#   ifndef SIM_NEW_OSG_QT
    cameraOTW->setGraphicsContext( _gwin );
#   endif

    const osg::GraphicsContext::Traits *traits = cameraOTW->getGraphicsContext()->getTraits();

    double w2h = (double)(traits->width) / (double)(traits->height);

#   ifndef SIM_TEST_WORLD
    cameraOTW->setClearColor( osg::Vec4( 0.0, 0.0, 0.0, 1.0 ) );
#   endif
    cameraOTW->setViewport( new osg::Viewport( 0, 0, traits->width, traits->height ) );
    cameraOTW->setProjectionMatrixAsPerspective( CGI_FOV_Y, w2h, _zNear, _zFar );
    cameraOTW->setNearFarRatio( _zNear / _zFar );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetOTW::createCameraHUD()
{
    osg::ref_ptr<osg::Camera> cameraHUD = new osg::Camera();

#   ifndef SIM_NEW_OSG_QT
    cameraHUD->setGraphicsContext( _gwin );
#   endif

    osg::GraphicsContext* context = getOsgViewer()->getCamera()->getGraphicsContext();

    cameraHUD->setGraphicsContext( context );

    const osg::GraphicsContext::Traits *traits = context->getTraits();

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

    getOsgViewer()->addSlave( cameraHUD, false );
}
