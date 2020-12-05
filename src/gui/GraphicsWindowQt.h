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
#ifndef GRAPHICSWINDOWQT_H
#define GRAPHICSWINDOWQT_H

////////////////////////////////////////////////////////////////////////////////

#include <osg/Version>

#include <osgViewer/GraphicsWindow>

#include <QEvent>
#include <QGLWidget>
#include <QInputEvent>
#include <QMutex>
#include <QQueue>
#include <QSet>

#include <Defines.h>

////////////////////////////////////////////////////////////////////////////////

class GraphicsWindowQt : public osgViewer::GraphicsWindow
{
    class GLWidget;

    friend class GLWidget;

public:

    struct WinData : public osg::Referenced
    {
        WinData( GLWidget *widget = NULLPTR, QWidget *parent = NULLPTR ) :
            _widget ( widget ),
            _parent ( parent )
        {}

        GLWidget *_widget;      ///<
        QWidget  *_parent;      ///<
    };

    /**
     * @brief Constructor.
     * @param traits
     */
    GraphicsWindowQt( osg::GraphicsContext::Traits *traits );

    /** @brief Destructor. */
    virtual ~GraphicsWindowQt();

    virtual bool setWindowRectangleImplementation( int x, int y, int w, int h );
    virtual void getWindowRectangle( int &x, int &y, int &w, int &h );
    virtual void grabFocus();
    virtual void grabFocusIfPointerInWindow();
    virtual void raiseWindow();
    virtual void useCursor( bool cursorOn );
    virtual void setCursor( MouseCursor cursor );

    virtual bool valid() const;
    virtual bool realizeImplementation();
    virtual bool isRealizedImplementation() const;
    virtual void closeImplementation();
    virtual bool makeCurrentImplementation();
    virtual bool releaseContextImplementation();
    virtual void swapBuffersImplementation();
    virtual void runOperations();

    virtual void requestWarpPointer( float x, float y );

    inline       GLWidget* getGLWidget()       { return _widget; }
    inline const GLWidget* getGLWidget() const { return _widget; }

private:

    class GLWidget : public QGLWidget
    {
        friend class GraphicsWindowQt;

    public:

        GLWidget( const QGLFormat &format,
                  QWidget *parent = NULLPTR, const QGLWidget *shareWidget = NULLPTR,
                  Qt::WindowFlags flags = 0 );

        virtual ~GLWidget();

        inline       GraphicsWindowQt* getGraphicsWindow()       { return _gwin; }
        inline const GraphicsWindowQt* getGraphicsWindow() const { return _gwin; }

        inline void setGraphicsWindow( GraphicsWindowQt *gwin ) { _gwin = gwin; }

        void setKeyboardModifiers( QInputEvent *event );

    protected:

        virtual bool event( QEvent *event );

        virtual void keyPressEvent   ( QKeyEvent *event );
        virtual void keyReleaseEvent ( QKeyEvent *event );

        virtual void mousePressEvent       ( QMouseEvent *event );
        virtual void mouseReleaseEvent     ( QMouseEvent *event );
        virtual void mouseDoubleClickEvent ( QMouseEvent *event );
        virtual void mouseMoveEvent        ( QMouseEvent *event );

        virtual void moveEvent( QMoveEvent *event );

        virtual void resizeEvent( QResizeEvent *event );

        virtual void wheelEvent( QWheelEvent *event );

        virtual void glDraw();

    private:

        GraphicsWindowQt *_gwin;                        ///<

        QMutex _deferredEventQueueMutex;                ///<

        QQueue < QEvent::Type > _deferredEventQueue;    ///<
        QSet   < QEvent::Type > _eventCompressor;       ///<

        int getNumDeferredEvents();

        void enqueueDeferredEvent( QEvent::Type eventType,
                                   QEvent::Type removeEventType = QEvent::None );

        void processDeferredEvents();
    };

    GLWidget *_widget;          ///<
    QCursor _currentCursor;     ///<

    bool _ownsWidget;           ///<
    bool _realized;             ///<
};

////////////////////////////////////////////////////////////////////////////////

#endif // GRAPHICSWINDOWQT_H
