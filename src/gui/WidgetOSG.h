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
#ifndef WIDGETOSG_H
#define WIDGETOSG_H

////////////////////////////////////////////////////////////////////////////////

#include <QDateTime>
#include <QGridLayout>
#include <QWidget>

#include <osgViewer/Viewer>
#include <osgGA/GUIEventHandler>

#include <defs.h>

#ifdef USE_OSGQOPENGL
#   include <osgQOpenGL/osgQOpenGLWidget>
#else
#   include <gui/GraphicsWindowQt.h>
#endif

////////////////////////////////////////////////////////////////////////////////

/**
 * @brief OSG common widget class.
 */
#ifdef USE_OSGQOPENGL
class WidgetOSG : public osgQOpenGLWidget
#else
class WidgetOSG : public QWidget, public osgViewer::Viewer
#endif
{
    Q_OBJECT

public:

    /** @brief Constructor. */
    explicit WidgetOSG( QWidget *parent = NULLPTR );

    /** @brief Destructor. */
    virtual ~WidgetOSG();

    virtual osgViewer::Viewer* getOsgViewer();

    virtual void setSceneData( osg::Node *node );

protected:

#   ifndef USE_OSGQOPENGL
    QGridLayout *_layout;                   ///<

    osg::ref_ptr<GraphicsWindowQt> _gwin;   ///<
#   endif

    bool _initialized;                      ///<

#   ifndef USE_OSGQOPENGL
    /** */
    virtual void paintEvent( QPaintEvent *event );

    /** */
    virtual osg::ref_ptr<GraphicsWindowQt> createGraphicsWindow( int x, int y, int w, int h );
#   endif
};

////////////////////////////////////////////////////////////////////////////////

#endif // WIDGETOSG_H
