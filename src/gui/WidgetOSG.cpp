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

#include <gui/WidgetOSG.h>

#include <QDebug>

////////////////////////////////////////////////////////////////////////////////

WidgetOSG::WidgetOSG( QWidget *parent ) :
#   ifdef USE_OSGQOPENGL
    osgQOpenGLWidget ( parent ),
    _initialized ( false )
#   else
    QWidget ( parent ),
    _layout ( NULLPTR ),
    _initialized ( true )
#   endif
{
#   ifdef SIM_OSG_DEBUG_INFO
    osg::setNotifyLevel( osg::DEBUG_INFO );
#   else
    osg::setNotifyLevel( osg::WARN );
#   endif

#   ifndef USE_OSGQOPENGL
    setThreadingModel( osgViewer::ViewerBase::SingleThreaded );
    //setThreadingModel( osgViewer::ViewerBase::ThreadPerContext );

    _gwin = createGraphicsWindow( x(), y(), width(), height() );
#   endif
}

////////////////////////////////////////////////////////////////////////////////

WidgetOSG::~WidgetOSG() {}

////////////////////////////////////////////////////////////////////////////////

osgViewer::Viewer* WidgetOSG::getOsgViewer()
{
#   ifdef USE_OSGQOPENGL
    return osgQOpenGLWidget::getOsgViewer();
#   else
    return this;
#   endif
}

////////////////////////////////////////////////////////////////////////////////

void WidgetOSG::setSceneData( osg::Node *node )
{
#   ifdef USE_OSGQOPENGL
    if ( _initialized )
    {
        getOsgViewer()->setSceneData( node );
    }
#   else
    osgViewer::Viewer::setSceneData( node );
#   endif
}

////////////////////////////////////////////////////////////////////////////////

#ifndef USE_OSGQOPENGL
void WidgetOSG::paintEvent( QPaintEvent *event )
{
    /////////////////////////////
    QWidget::paintEvent( event );
    /////////////////////////////

    frame();
}

////////////////////////////////////////////////////////////////////////////////

osg::ref_ptr<GraphicsWindowQt> WidgetOSG::createGraphicsWindow( int x, int y, int w, int h )
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
    traits->vsync            = true;

    osg::ref_ptr<GraphicsWindowQt> graphicsWindow = new GraphicsWindowQt( traits.get() );

    return graphicsWindow;
}
#endif

