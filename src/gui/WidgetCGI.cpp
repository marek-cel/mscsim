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

#include <gui/WidgetCGI.h>

#include <osgGA/StateSetManipulator>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <g1000/g1000_Defines.h>

////////////////////////////////////////////////////////////////////////////////

WidgetCGI::WidgetCGI( QWidget *parent ) :
    WidgetOSG ( parent )
{
#   ifdef USE_OSGQOPENGL
    QObject::connect( this, &osgQOpenGLWidget::initialized, [ this ]
    {
        getOsgViewer()->setThreadingModel( osgViewer::ViewerBase::SingleThreaded );
        //getOsgViewer()->setThreadingModel( osgViewer::ViewerBase::ThreadPerContext );

        createCameraCGI();

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

        getOsgViewer()->setKeyEventSetsDone( 0 );

        /////////////////////////////////////////////////
        getOsgViewer()->setSceneData( new osg::Group() );
        /////////////////////////////////////////////////

        getOsgViewer()->assignSceneDataToCameras();

        _initialized = true;
    });
#   else
    QWidget *widget = addViewWidget();

    _layout = new QGridLayout( this );
    _layout->setContentsMargins( 1, 1, 1, 1 );
    _layout->addWidget( widget, 0, 0 );

    setLayout( _layout );
#   endif
}

////////////////////////////////////////////////////////////////////////////////

WidgetCGI::~WidgetCGI() {}

////////////////////////////////////////////////////////////////////////////////

#ifndef USE_OSGQOPENGL
QWidget* WidgetCGI::addViewWidget()
{
    createCameraCGI();

    setSceneData( new osg::Group() );
    addEventHandler( new osgViewer::StatsHandler );
    //setCameraManipulator( new osgGA::TrackballManipulator() );

    setKeyEventSetsDone( 0 );

    assignSceneDataToCameras();

    return _gwin->getGLWidget();
}
#endif

////////////////////////////////////////////////////////////////////////////////

void WidgetCGI::createCameraCGI()
{
    osg::ref_ptr<osg::Camera> camera = getOsgViewer()->getCamera();

#   ifndef USE_OSGQOPENGL
    camera->setGraphicsContext( _gwin );
#   endif

    const osg::GraphicsContext::Traits *traits = camera->getGraphicsContext()->getTraits();

    double w2h = (double)(traits->width) / (double)(traits->height);

    camera->setClearColor( osg::Vec4( 0.0, 0.0, 0.0, 1.0 ) );
    camera->setViewport( new osg::Viewport( 0, 0, traits->width, traits->height ) );

    if ( 0 )
    {
        camera->setProjectionMatrixAsPerspective( G1000_GDU_FOV_V, w2h, 1.0, 10000.0 );
    }
    else
    {
        camera->setViewport( new osg::Viewport( 0, 0, traits->width, traits->height ) );
        camera->setProjectionMatrixAsOrtho2D( -G1000_GDU_HEIGHT_2 * w2h , G1000_GDU_HEIGHT_2 * w2h,
                                              -G1000_GDU_HEIGHT_2       , G1000_GDU_HEIGHT_2 );
        camera->setViewMatrix( osg::Matrix::identity() );
    }
}
