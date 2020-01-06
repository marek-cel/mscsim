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

#include <gui/GraphicsStick.h>

#ifdef WIN32
#   include <float.h>
#endif

#include <cmath>
#include <cstdio>

////////////////////////////////////////////////////////////////////////////////

GraphicsStick::GraphicsStick( QWidget *parent ) :
    QGraphicsView ( parent ),

    _timerId ( 0 ),

    _scene ( NULLPTR ),

    _ctrlLineH ( NULLPTR ),
    _ctrlLineV ( NULLPTR ),
    _trimLineH ( NULLPTR ),
    _trimLineV ( NULLPTR ),
    _markLineH ( NULLPTR ),
    _markLineV ( NULLPTR ),

    _ctrlBrush ( QColor( 0x03, 0x2a, 0x63 ), Qt::SolidPattern ),
    _trimBrush ( QColor( 0x88, 0x88, 0x88 ), Qt::SolidPattern ),
    _markBrush ( QColor( 0xff, 0xff, 0xff ), Qt::SolidPattern ),

    _ctrlPen( _ctrlBrush, 1 ),
    _trimPen( _trimBrush, 1 ),
    _markPen( _markBrush, 1 ),

    _ctrlRoll  ( 0 ),
    _ctrlPitch ( 0 ),
    _trimRoll  ( 0 ),
    _trimPitch ( 0 )
{
    _scene = new QGraphicsScene( this );
    setScene( _scene );

    _scene->clear();

    init();

    _timerId = startTimer( 100 );
}

////////////////////////////////////////////////////////////////////////////////

GraphicsStick::~GraphicsStick()
{
    if ( _timerId ) killTimer( _timerId );

    if ( _scene )
    {
        _scene->clear();
        delete _scene;
        _scene = NULLPTR;
    }

    reset();
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsStick::reinit()
{
    if ( _scene )
    {
        _scene->clear();

        init();
    }
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsStick::resizeEvent( QResizeEvent *event )
{
    ////////////////////////////////////
    QGraphicsView::resizeEvent( event );
    ////////////////////////////////////

    reinit();
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsStick::timerEvent( QTimerEvent *event )
{
    ///////////////////////////////////
    QGraphicsView::timerEvent( event );
    ///////////////////////////////////

    if ( isVisible() )
    {
        updateView();
    }
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsStick::init()
{
    updateView();
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsStick::reset()
{
    _ctrlLineH = 0;
    _ctrlLineV = 0;
    _trimLineH = 0;
    _trimLineV = 0;
    _markLineH = 0;
    _markLineV = 0;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsStick::updateView()
{
    _scene->clear();

    short w = width();
    short h = height();

    float h_2 = h / 2.0f;
    float w_2 = w / 2.0f;

    short x_max = floor( w_2 ) - 1;
    short y_max = floor( h_2 ) - 1;

    short length = 8;

    short x_ctrl = floor( _ctrlRoll  * ( w_2 - 1.0f ) / 100.0f + 0.5f );
    short y_ctrl = floor( _ctrlPitch * ( h_2 - 1.0f ) / 100.0f + 0.5f );

    short x_trim = floor( _trimRoll  * ( w_2 - 1.0f ) / 100.0f + 0.5f );
    short y_trim = floor( _trimPitch * ( h_2 - 1.0f ) / 100.0f + 0.5f );

    if ( x_ctrl < -x_max ) x_ctrl = -x_max;
    if ( x_ctrl >  x_max ) x_ctrl =  x_max;
    if ( x_trim < -x_max ) x_trim = -x_max;
    if ( x_trim >  x_max ) x_trim =  x_max;

    if ( y_ctrl < -y_max ) y_ctrl = -y_max;
    if ( y_ctrl >  y_max ) y_ctrl =  y_max;
    if ( y_trim < -y_max ) y_trim = -y_max;
    if ( y_trim >  y_max ) y_trim =  y_max;

    _markLineH = _scene->addLine( -w_2, 0, w_2, 0, _markPen );
    _markLineV = _scene->addLine( 0, h_2, 0, -h_2, _markPen );

    _trimLineH = _scene->addLine( x_trim + length, y_trim, x_trim - length, y_trim, _trimPen );
    _trimLineV = _scene->addLine( x_trim, y_trim - length, x_trim, y_trim + length, _trimPen );

    _ctrlLineH = _scene->addLine( x_ctrl + length, y_ctrl, x_ctrl - length, y_ctrl, _ctrlPen );
    _ctrlLineV = _scene->addLine( x_ctrl, y_ctrl - length, x_ctrl, y_ctrl + length, _ctrlPen );

    _scene->update();

    centerOn( 0, 0 );
}
