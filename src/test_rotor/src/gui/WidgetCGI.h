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
#ifndef WIDGETCGI_H
#define WIDGETCGI_H

////////////////////////////////////////////////////////////////////////////////

#include <osgQt/GraphicsWindowQt>
#include <osgViewer/Viewer>

#include <QDateTime>
#include <QGridLayout>
#include <QWidget>


#include <cgi/cgi_ManipulatorOrbit.h>
#include <cgi/cgi_SceneRoot.h>

#include <hid/hid_Assignment.h>

////////////////////////////////////////////////////////////////////////////////

/** This is widget wrapper for CGI. */
class WidgetCGI : public QWidget, public osgViewer::Viewer
{
    Q_OBJECT

public:

    /** */
    class KeyHandler : public osgGA::GUIEventHandler
    {
    public:

        /** */
        KeyHandler( WidgetCGI *widgetCGI );

        /** */
        inline const bool* getKeysState() const { return _keysState; }

        /** */
        bool handle( const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter & );

        void keyDn( hid::Assignment::Key key );
        void keyUp( hid::Assignment::Key key );

    private:

        WidgetCGI *_widgetCGI;

        bool _keysState[ HID_MAX_KEYS ];

        int getKeyIndex( int key );

        bool handleKeyDn( const osgGA::GUIEventAdapter &ea );
        bool handleKeyUp( const osgGA::GUIEventAdapter &ea );
    };

    /** Constructor. */
    WidgetCGI( QWidget *parent = 0 );

    /** Destructor. */
    virtual ~WidgetCGI();

    inline void keyDn( hid::Assignment::Key key ) { _keyHandler->keyDn( key ); }
    inline void keyUp( hid::Assignment::Key key ) { _keyHandler->keyUp( key ); }

    void setViewAft();
    void setViewFwd();
    void setViewLft();
    void setViewRgt();
    void setViewTop();

    void rotationStart();
    void rotationStop();

protected:

    /** */
    void timerEvent( QTimerEvent *event );

    /** */
    void paintEvent( QPaintEvent *event );

private:

    cgi::SceneRoot *_sceneRoot;
    QGridLayout    *_gridLayout;

    osg::ref_ptr<cgi::ManipulatorOrbit> _manipulator;

    osg::ref_ptr<osgQt::GraphicsWindowQt> _graphicsWindow;
    osg::ref_ptr<KeyHandler> _keyHandler;

    double _head_0;
    double _elev_0;

    bool _rotation;

    int _timerId;

    /** */
    QWidget* addViewWidget( osgQt::GraphicsWindowQt *graphicsWindow, osg::Node *scene );

    /** */
    osg::ref_ptr<osgQt::GraphicsWindowQt> createGraphicsWindow( int x, int y, int w, int h, const std::string &name = "", bool windowDecoration = false );
};

////////////////////////////////////////////////////////////////////////////////

#endif // WIDGETCGI_H
