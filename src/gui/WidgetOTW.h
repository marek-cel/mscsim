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
#ifndef WIDGETOTW_H
#define WIDGETOTW_H

////////////////////////////////////////////////////////////////////////////////

#include <QDateTime>
#include <QGridLayout>
#include <QWidget>

#include <osgViewer/Viewer>
#include <osgGA/GUIEventHandler>
#include <osgQt/GraphicsWindowQt>

#include <hid/hid_Assignment.h>

#include "gui_Defines.h"
#include "KeyHandler.h"

////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Out-the-Window widget class.
 */
class WidgetOTW : public QWidget, public osgViewer::Viewer
{
    Q_OBJECT

public:

    static const double _zNear;
    static const double _zFar;

    /** Constructor. */
    WidgetOTW( QWidget *parent = NULLPTR );

    /** Destructor. */
    virtual ~WidgetOTW();

    inline void keyDn( hid::Assignment::Key key ) { _keyHandler->keyDn( key ); }
    inline void keyUp( hid::Assignment::Key key ) { _keyHandler->keyUp( key ); }

    /** */
    void setCameraManipulatorChase();

    /** */
    void setCameraManipulatorOrbit();

    /** */
    void setCameraManipulatorPilot();

    /** */
    void setCameraManipulatorWorld();

    /** */
    void setDistanceDef( double distance_def );

    /** */
    void setDistanceMin( double distance_min );

protected:

    /** */
    void paintEvent( QPaintEvent *event );

    /** */
    void timerEvent( QTimerEvent *event );

private:

    QGridLayout *_gridLayout;

    osg::ref_ptr<osgQt::GraphicsWindowQt> _graphicsWindow;
    osg::ref_ptr<KeyHandler> _keyHandler;

    int _timerId;                   ///< timer ID

    bool _camManipulatorInited;

    /** */
    QWidget* addViewWidget();

    void createCameraOTW();

    /** */
    void createCameraHUD();

    /** */
    osg::ref_ptr<osgQt::GraphicsWindowQt> createGraphicsWindow( int x, int y, int w, int h );
};

////////////////////////////////////////////////////////////////////////////////

#endif // WIDGETOTW_H
