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

#include <gui/WidgetCGI.h>

#include <osg/PositionAttitudeTransform>
#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osgViewer/ViewerEventHandlers>

#include <g1000/g1000_Defines.h>

////////////////////////////////////////////////////////////////////////////////

WidgetCGI::WidgetCGI( QWidget *parent ) :
    WidgetOSG ( parent ),
    _layout ( NULLPTR )
{
    QWidget *widget = addViewWidget();

    _layout = new QGridLayout( this );
    _layout->setContentsMargins( 1, 1, 1, 1 );
    _layout->addWidget( widget, 0, 0 );

    setLayout( _layout );
}

////////////////////////////////////////////////////////////////////////////////

WidgetCGI::~WidgetCGI() {}

////////////////////////////////////////////////////////////////////////////////

QWidget* WidgetCGI::addViewWidget()
{
    createCamera();

    setSceneData( new osg::Group() );
    addEventHandler( new osgViewer::StatsHandler );
    //setCameraManipulator( new osgGA::TrackballManipulator() );

    setKeyEventSetsDone( 0 );

    assignSceneDataToCameras();

    return _gwin->getGLWidget();
}

////////////////////////////////////////////////////////////////////////////////

void WidgetCGI::createCamera()
{
    osg::ref_ptr<osg::Camera> camera = getCamera();

    camera->setGraphicsContext( _gwin );

    const osg::GraphicsContext::Traits *traits = _gwin->getTraits();

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
