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

#include <gui/GraphicsWindowQt.h>

#include <gui/KeyMap.h>

#include <sim/Log.h>

////////////////////////////////////////////////////////////////////////////////

GraphicsWindowQt::GraphicsWindowQt( osg::GraphicsContext::Traits *traits ) :
    _widget ( NULLPTR ),
    _realized ( false )
{
    _traits = traits;

    osg::ref_ptr<WinData> winData = _traits.get() ? dynamic_cast< WinData* >( _traits->inheritedWindowData.get() ) : NULLPTR;

    if ( !_widget )
    {
        _widget = winData.valid() ? winData->_widget : NULLPTR;
    }

    _ownsWidget = _widget == NULLPTR;

    if ( !_widget )
    {
        QGLWidget *shareWidget = NULLPTR;
        osg::ref_ptr<GraphicsWindowQt> sharedContext =
                dynamic_cast< GraphicsWindowQt* >( _traits->sharedContext.get() );

        if ( sharedContext.valid() )
        {
            shareWidget = sharedContext->getGLWidget();
        }

        Qt::WindowFlags flags_tmp = Qt::Window | Qt::CustomizeWindowHint;
        if ( _traits->windowDecoration )
        {
            flags_tmp |= Qt::WindowTitleHint
                      |  Qt::WindowMinMaxButtonsHint
                      |  Qt::WindowSystemMenuHint;

#           if (QT_VERSION_CHECK(4, 5, 0) <= QT_VERSION)
            flags_tmp |= Qt::WindowCloseButtonHint;
#           endif
        }

        QGLFormat format( QGLFormat::defaultFormat() );

        format.setAlphaBufferSize  ( _traits->alpha         );
        format.setRedBufferSize    ( _traits->red           );
        format.setGreenBufferSize  ( _traits->green         );
        format.setBlueBufferSize   ( _traits->blue          );
        format.setDepthBufferSize  ( _traits->depth         );
        format.setStencilBufferSize( _traits->stencil       );
        format.setSampleBuffers    ( _traits->sampleBuffers );
        format.setSamples          ( _traits->samples       );

        format.setAlpha   ( _traits->alpha   > 0 );
        format.setDepth   ( _traits->depth   > 0 );
        format.setStencil ( _traits->stencil > 0 );

        format.setDoubleBuffer ( _traits->doubleBuffer );
        format.setSwapInterval ( _traits->vsync            ? 1 : 0 );
        format.setStereo       ( _traits->quadBufferStereo ? 1 : 0 );

        _widget = new GLWidget( format, NULLPTR, shareWidget, flags_tmp );
    }

    if ( _ownsWidget )
    {
        _widget->setWindowTitle( _traits->windowName.c_str() );
        _widget->move( _traits->x, _traits->y );

        if ( !_traits->supportsResize )
            _widget->setFixedSize( _traits->width, _traits->height );
        else
            _widget->resize( _traits->width, _traits->height );
    }

    _widget->setAutoBufferSwap( false );
    _widget->setMouseTracking( true );
    _widget->setFocusPolicy( Qt::WheelFocus );
    _widget->setGraphicsWindow( this );

    useCursor( _traits->useCursor );

    setState( new osg::State );
    getState()->setGraphicsContext( this );

    if ( _traits.valid() && _traits->sharedContext.valid() )
    {
        getState()->setContextID( _traits->sharedContext->getState()->getContextID() );
        incrementContextIDUsageCount( getState()->getContextID() );
    }
    else
    {
        getState()->setContextID( osg::GraphicsContext::createNewContextID() );
    }

#   if OPENSCENEGRAPH_SOVERSION < 140
    getEventQueue()->syncWindowRectangleWithGraphcisContext();
#   else
    getEventQueue()->syncWindowRectangleWithGraphicsContext();
#   endif
}

////////////////////////////////////////////////////////////////////////////////

GraphicsWindowQt::~GraphicsWindowQt()
{
    if ( _widget )
    {
        _widget->_gwin = NULLPTR;
        _widget = NULLPTR;
    }
}

////////////////////////////////////////////////////////////////////////////////

bool GraphicsWindowQt::setWindowRectangleImplementation( int x, int y, int w, int h )
{
    if ( _widget == NULLPTR ) return false;

    _widget->setGeometry( x, y, w, h );

    return true;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsWindowQt::getWindowRectangle( int &x, int &y, int &w, int &h )
{
    if ( _widget )
    {
        const QRect &geom = _widget->geometry();

        x = geom.x();
        y = geom.y();
        w = geom.width();
        h = geom.height();
    }
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsWindowQt::grabFocus()
{
    if ( _widget ) _widget->setFocus( Qt::ActiveWindowFocusReason );
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsWindowQt::grabFocusIfPointerInWindow()
{
    if ( _widget->underMouse() ) _widget->setFocus( Qt::ActiveWindowFocusReason );
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsWindowQt::raiseWindow()
{
    if ( _widget ) _widget->raise();
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsWindowQt::useCursor( bool cursorOn )
{
    if ( _widget )
    {
        _traits->useCursor = cursorOn;

        if ( !cursorOn )
            _widget->setCursor( Qt::BlankCursor );
        else
            _widget->setCursor( _currentCursor );
    }
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsWindowQt::setCursor( MouseCursor cursor )
{
    if ( cursor == InheritCursor && _widget )
    {
        _widget->unsetCursor();
    }

    switch ( cursor )
    {
        case NoCursor:          _currentCursor = Qt::BlankCursor;     break;
        case RightArrowCursor:
        case LeftArrowCursor:   _currentCursor = Qt::ArrowCursor;     break;
        case InfoCursor:        _currentCursor = Qt::SizeAllCursor;   break;
        case DestroyCursor:     _currentCursor = Qt::ForbiddenCursor; break;
        case HelpCursor:        _currentCursor = Qt::WhatsThisCursor; break;
        case CycleCursor:       _currentCursor = Qt::ForbiddenCursor; break;
        case SprayCursor:       _currentCursor = Qt::SizeAllCursor;   break;
        case WaitCursor:        _currentCursor = Qt::WaitCursor;      break;
        case TextCursor:        _currentCursor = Qt::IBeamCursor;     break;
        case CrosshairCursor:   _currentCursor = Qt::CrossCursor;     break;
        case HandCursor:        _currentCursor = Qt::OpenHandCursor;  break;
        case UpDownCursor:      _currentCursor = Qt::SizeVerCursor;   break;
        case LeftRightCursor:   _currentCursor = Qt::SizeHorCursor;   break;
        case TopSideCursor:
        case BottomSideCursor:  _currentCursor = Qt::UpArrowCursor;   break;
        case LeftSideCursor:
        case RightSideCursor:   _currentCursor = Qt::SizeHorCursor;   break;
        case TopLeftCorner:     _currentCursor = Qt::SizeBDiagCursor; break;
        case TopRightCorner:    _currentCursor = Qt::SizeFDiagCursor; break;
        case BottomRightCorner: _currentCursor = Qt::SizeBDiagCursor; break;
        case BottomLeftCorner:  _currentCursor = Qt::SizeFDiagCursor; break;
        default: break;
    };

    if ( _widget ) _widget->setCursor( _currentCursor );
}

////////////////////////////////////////////////////////////////////////////////

bool GraphicsWindowQt::valid() const
{
    return _widget && _widget->isValid();
}

////////////////////////////////////////////////////////////////////////////////

bool GraphicsWindowQt::realizeImplementation()
{
    const QGLContext *savedContext = QGLContext::currentContext();

    if ( !valid() )
    {
        _widget->glInit();
    }

    _realized = true;
    bool result = makeCurrent();
    _realized = false;

    if ( !result )
    {
        if ( savedContext )
        {
            const_cast< QGLContext* >( savedContext )->makeCurrent();
        }

        Log::w() << "Window realize: Can make context current." << std::endl;

        return false;
    }

    _realized = true;

#   if OPENSCENEGRAPH_SOVERSION < 140
    getEventQueue()->syncWindowRectangleWithGraphcisContext();
#   else
    getEventQueue()->syncWindowRectangleWithGraphicsContext();
#   endif

    if ( !releaseContext() )
    {
        Log::w() << "Window realize: Can not release context." << std::endl;
    }

    if ( savedContext )
    {
        const_cast< QGLContext* >( savedContext )->makeCurrent();
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////

bool GraphicsWindowQt::isRealizedImplementation() const
{
    return _realized;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsWindowQt::closeImplementation()
{
    if ( _widget ) _widget->close();

    _realized = false;
}

////////////////////////////////////////////////////////////////////////////////

bool GraphicsWindowQt::makeCurrentImplementation()
{
    if ( _widget->getNumDeferredEvents() > 0 )
    {
        _widget->processDeferredEvents();
    }

    _widget->makeCurrent();

    return true;
}

////////////////////////////////////////////////////////////////////////////////

bool GraphicsWindowQt::releaseContextImplementation()
{
    _widget->doneCurrent();
    return true;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsWindowQt::swapBuffersImplementation()
{
    _widget->swapBuffers();

    if ( _widget->getNumDeferredEvents() > 0 )
    {
        _widget->processDeferredEvents();
    }

    if ( QGLContext::currentContext() != _widget->context() )
    {
        _widget->makeCurrent();
    }
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsWindowQt::runOperations()
{
    if ( _widget->getNumDeferredEvents() > 0 )
    {
        _widget->processDeferredEvents();
    }

    if ( QGLContext::currentContext() != _widget->context() )
    {
        _widget->makeCurrent();
    }

    GraphicsWindow::runOperations();
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsWindowQt::requestWarpPointer( float x, float y )
{
    if ( _widget )
    {
        QCursor::setPos( _widget->mapToGlobal( QPoint( (int)x, (int)y ) ) );
    }
}

////////////////////////////////////////////////////////////////////////////////

GraphicsWindowQt::GLWidget::GLWidget( const QGLFormat &format,
                                      QWidget *parent, const QGLWidget *shareWidget,
                                      Qt::WindowFlags flags ) :
    QGLWidget ( format, parent, shareWidget, flags ),
    _gwin ( NULLPTR )
{}

////////////////////////////////////////////////////////////////////////////////

GraphicsWindowQt::GLWidget::~GLWidget()
{
    if ( _gwin )
    {
        _gwin->close();
        _gwin->_widget = NULLPTR;
        _gwin = NULLPTR;
    }
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsWindowQt::GLWidget::setKeyboardModifiers( QInputEvent *event )
{
    int modkey = event->modifiers() & ( Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier );

    unsigned int mask = 0;

    if ( modkey & Qt::ShiftModifier   ) mask |= osgGA::GUIEventAdapter::MODKEY_SHIFT;
    if ( modkey & Qt::ControlModifier ) mask |= osgGA::GUIEventAdapter::MODKEY_CTRL;
    if ( modkey & Qt::AltModifier     ) mask |= osgGA::GUIEventAdapter::MODKEY_ALT;

    _gwin->getEventQueue()->getCurrentEventState()->setModKeyMask( mask );
}

////////////////////////////////////////////////////////////////////////////////

bool GraphicsWindowQt::GLWidget::event( QEvent *event )
{
    if      ( event->type() == QEvent::Hide )
    {
        enqueueDeferredEvent( QEvent::Hide, QEvent::Show );
        return true;
    }
    else if ( event->type() == QEvent::Show )
    {
        enqueueDeferredEvent( QEvent::Show, QEvent::Hide );
        return true;
    }
    else
    if ( event->type() == QEvent::ParentChange )
    {
        enqueueDeferredEvent( QEvent::ParentChange );
        return true;
    }

    return QGLWidget::event( event );
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsWindowQt::GLWidget::keyPressEvent( QKeyEvent *event )
{
    setKeyboardModifiers( event );
    _gwin->getEventQueue()->keyPress( KeyMap::remapOSG( event->key() ) );
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsWindowQt::GLWidget::keyReleaseEvent( QKeyEvent *event )
{
    if( event->isAutoRepeat() )
    {
        event->ignore();
    }
    else
    {
        setKeyboardModifiers( event );
        _gwin->getEventQueue()->keyRelease( KeyMap::remapOSG( event->key() ) );
    }
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsWindowQt::GLWidget::mousePressEvent( QMouseEvent *event )
{
    int button = 0;

    switch ( event->button() )
    {
        case Qt::LeftButton:  button = 1; break;
        case Qt::MidButton:   button = 2; break;
        case Qt::RightButton: button = 3; break;
        case Qt::NoButton:    button = 0; break;
        default:              button = 0; break;
    }

    setKeyboardModifiers( event );

    _gwin->getEventQueue()->mouseButtonPress( event->x(), event->y(), button );
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsWindowQt::GLWidget::mouseReleaseEvent( QMouseEvent *event )
{
    int button = 0;

    switch ( event->button() )
    {
        case Qt::LeftButton:  button = 1; break;
        case Qt::MidButton:   button = 2; break;
        case Qt::RightButton: button = 3; break;
        case Qt::NoButton:    button = 0; break;
        default:              button = 0; break;
    }

    setKeyboardModifiers( event );

    _gwin->getEventQueue()->mouseButtonRelease( event->x(), event->y(), button );
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsWindowQt::GLWidget::mouseDoubleClickEvent( QMouseEvent *event )
{
    int button = 0;

    switch ( event->button() )
    {
        case Qt::LeftButton:  button = 1; break;
        case Qt::MidButton:   button = 2; break;
        case Qt::RightButton: button = 3; break;
        case Qt::NoButton:    button = 0; break;
        default:              button = 0; break;
    }

    setKeyboardModifiers( event );

    _gwin->getEventQueue()->mouseDoubleButtonPress( event->x(), event->y(), button );
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsWindowQt::GLWidget::mouseMoveEvent( QMouseEvent *event )
{
    setKeyboardModifiers( event );
    _gwin->getEventQueue()->mouseMotion( event->x(), event->y() );
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsWindowQt::GLWidget::moveEvent( QMoveEvent *event )
{
    const QPoint &pos = event->pos();

    _gwin->resized( pos.x(), pos.y(), width(), height() );
    _gwin->getEventQueue()->windowResize( pos.x(), pos.y(), width(), height() );
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsWindowQt::GLWidget::resizeEvent( QResizeEvent *event )
{
    const QSize &size = event->size();

    _gwin->resized( x(), y(), size.width(), size.height() );
    _gwin->getEventQueue()->windowResize( x(), y(), size.width(), size.height() );
    _gwin->requestRedraw();
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsWindowQt::GLWidget::wheelEvent( QWheelEvent *event )
{
    setKeyboardModifiers( event );

    if ( event->orientation() == Qt::Vertical )
    {
        _gwin->getEventQueue()->mouseScroll( event->delta() > 0 ? osgGA::GUIEventAdapter::SCROLL_UP   : osgGA::GUIEventAdapter::SCROLL_DOWN  );
    }
    else
    {
        _gwin->getEventQueue()->mouseScroll( event->delta() > 0 ? osgGA::GUIEventAdapter::SCROLL_LEFT : osgGA::GUIEventAdapter::SCROLL_RIGHT );
    }
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsWindowQt::GLWidget::glDraw()
{
    _gwin->requestRedraw();
}

////////////////////////////////////////////////////////////////////////////////

int GraphicsWindowQt::GLWidget::getNumDeferredEvents()
{
    QMutexLocker lock( &_deferredEventQueueMutex );
    return _deferredEventQueue.count();
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsWindowQt::GLWidget::enqueueDeferredEvent( QEvent::Type eventType,
                                                    QEvent::Type removeEventType )
{
    QMutexLocker lock( &_deferredEventQueueMutex );

    if ( removeEventType != QEvent::None )
    {
        if ( _deferredEventQueue.removeOne( removeEventType ) )
        {
            _eventCompressor.remove( eventType );
        }
    }

    if ( _eventCompressor.find( eventType ) == _eventCompressor.end() )
    {
        _deferredEventQueue.enqueue( eventType );
        _eventCompressor.insert( eventType );
    }
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsWindowQt::GLWidget::processDeferredEvents()
{
    QQueue< QEvent::Type > deferredEventQueueCopy;

    QMutexLocker lock( &_deferredEventQueueMutex );

    deferredEventQueueCopy = _deferredEventQueue;

    _eventCompressor.clear();
    _deferredEventQueue.clear();

    while (!deferredEventQueueCopy.isEmpty())
    {
        QEvent event( deferredEventQueueCopy.dequeue() );
        QGLWidget::event( &event );
    }
}

