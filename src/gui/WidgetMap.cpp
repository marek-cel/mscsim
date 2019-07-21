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

#include <gui/WidgetMap.h>

#include <osgDB/ReadFile>
#include <osgViewer/ViewerEventHandlers>

#include <cgi/cgi_Defines.h>
#include <cgi/cgi_Manager.h>
#include <cgi/cgi_Mercator.h>

#include <gui/gui_Defines.h>

////////////////////////////////////////////////////////////////////////////////

void scaleChangeCallback( double scale )
{
    cgi::Manager::instance()->setMapScale( scale );
}

////////////////////////////////////////////////////////////////////////////////

WidgetMap::WidgetMap( QWidget *parent ) :
    QWidget ( parent ),
    m_gridLayout ( 0 ),
    m_timerId ( 0 ),
    m_camManipulatorInited ( false )
{
    setMouseTracking( true );

    setThreadingModel( osgViewer::ViewerBase::SingleThreaded );
    //setThreadingModel( osgViewer::ViewerBase::ThreadPerContext );

    m_graphicsWindow = createGraphicsWindow( x(), y(), width(), height() );

    m_manipulator = new cgi::ManipulatorMap();
    m_manipulator->registerScaleChangeCallback( &scaleChangeCallback );
    m_manipulator->setScaleMin( 1.0e-5 );//m_manipulator->setScaleMin( 5.0e-5 );
    m_manipulator->setScaleMax( 1.0 );
    m_manipulator->setMapHeight( 2.0 * CGI_MAP_Y_2 );
    m_manipulator->setMapMinX( -cgi::Mercator::maxX );
    m_manipulator->setMapMaxX(  cgi::Mercator::maxX );
    m_manipulator->setMapMinY( -cgi::Mercator::maxY );
    m_manipulator->setMapMaxY(  cgi::Mercator::maxY );
    m_manipulator->setAllowThrow( true );

    QWidget *widget = addViewWidget();

    m_gridLayout = new QGridLayout( this );
    m_gridLayout->setContentsMargins( 1, 1, 1, 1 );
    m_gridLayout->addWidget( widget, 0, 0 );

    setLayout( m_gridLayout );

    m_timerId = startTimer( GUI_TIME_STEP );
}

////////////////////////////////////////////////////////////////////////////////

WidgetMap::~WidgetMap()
{
    if ( m_timerId ) killTimer( m_timerId );
}

////////////////////////////////////////////////////////////////////////////////

bool WidgetMap::event( QEvent *event )
{
    return QWidget::event( event );
}

////////////////////////////////////////////////////////////////////////////////

void WidgetMap::paintEvent( QPaintEvent *event )
{
    /////////////////////////////
    QWidget::paintEvent( event );
    /////////////////////////////

    frame();
}

////////////////////////////////////////////////////////////////////////////////

void WidgetMap::timerEvent( QTimerEvent *event )
{
    /////////////////////////////
    QWidget::timerEvent( event );
    /////////////////////////////

    update();

    cgi::Manager::instance()->updateMap();

    emit positionChanged( m_manipulator->getMouseLat(),
                          m_manipulator->getMouseLon() );
}

////////////////////////////////////////////////////////////////////////////////

QWidget* WidgetMap::addViewWidget()
{
    createCameraMap();

    setSceneData( cgi::Manager::instance()->getNodeMap() );
    addEventHandler( new osgViewer::StatsHandler );
    setCameraManipulator( m_manipulator.get() );

    setKeyEventSetsDone( 0 );

    assignSceneDataToCameras();

    return m_graphicsWindow->getGLWidget();
}

////////////////////////////////////////////////////////////////////////////////

void WidgetMap::createCameraMap()
{
    osg::ref_ptr<osg::Camera> cameraMap = getCamera();

    cameraMap->setGraphicsContext( m_graphicsWindow );

    const osg::GraphicsContext::Traits *traits = m_graphicsWindow->getTraits();

    double w2h = (double)(traits->width) / (double)(traits->height);

    cameraMap->setClearColor( osg::Vec4( cgi::Map::colorOceans, 1.0 ) );
    //cameraMap->setClearColor( osg::Vec4( 0.0, 0.0, 0.0, 1.0 ) );
    cameraMap->setViewport( new osg::Viewport( 0, 0, traits->width, traits->height ) );

    if ( 0 )
    {
        cameraMap->setProjectionMatrixAsPerspective( CGI_MAP_FOV_Y, w2h, 1.0, 10000.0 );
    }
    else
    {
        cameraMap->setViewport( new osg::Viewport( 0, 0, traits->width, traits->height ) );
        cameraMap->setProjectionMatrixAsOrtho2D( -CGI_MAP_Y_2 * w2h, CGI_MAP_Y_2 * w2h, -CGI_MAP_Y_2, CGI_MAP_Y_2 );
        cameraMap->setViewMatrix( osg::Matrix::identity() );
    }
}

////////////////////////////////////////////////////////////////////////////////

osg::ref_ptr<osgQt::GraphicsWindowQt> WidgetMap::createGraphicsWindow( int x, int y, int w, int h,
                                                                       const std::string &name,
                                                                       bool windowDecoration )
{
    osg::DisplaySettings *displaySettings = osg::DisplaySettings::instance().get();
    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits();

    traits->windowName       = name;
    traits->windowDecoration = windowDecoration;
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
