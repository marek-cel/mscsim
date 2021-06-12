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

#include <gui/GraphicsPedals.h>

#ifdef WIN32
#   include <float.h>
#endif

#include <cmath>
#include <cstdio>

////////////////////////////////////////////////////////////////////////////////

GraphicsPedals::GraphicsPedals( QWidget *parent ) :
    QGraphicsView ( parent ),

    _timerId ( 0 ),

    _scene ( NULLPTR ),

    _ctrlLineV ( NULLPTR ),
    _trimLineV ( NULLPTR ),
    _markLineH ( NULLPTR ),
    _markLineV ( NULLPTR ),

    _ctrlBrush ( QColor( 0x03, 0x2a, 0x63 ), Qt::SolidPattern ),
    _trimBrush ( QColor( 0x88, 0x88, 0x88 ), Qt::SolidPattern ),
    _markBrush ( QColor( 0xff, 0xff, 0xff ), Qt::SolidPattern ),

    _ctrlPen( _ctrlBrush, 1 ),
    _trimPen( _trimBrush, 1 ),
    _markPen( _markBrush, 1 ),

    _ctrlYaw ( 0 ),
    _trimYaw ( 0 )
{
    _scene = new QGraphicsScene( this );
    setScene( _scene );

    _scene->clear();

    init();

    _timerId = startTimer( 100 );
}

////////////////////////////////////////////////////////////////////////////////

GraphicsPedals::~GraphicsPedals()
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

void GraphicsPedals::reinit()
{
    if ( _scene )
    {
        _scene->clear();

        init();
    }
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsPedals::resizeEvent( QResizeEvent *event )
{
    ////////////////////////////////////
    QGraphicsView::resizeEvent( event );
    ////////////////////////////////////

    reinit();
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsPedals::timerEvent( QTimerEvent *event )
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

void GraphicsPedals::init()
{
    updateView();
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsPedals::reset()
{
    _ctrlLineV = NULLPTR;
    _trimLineV = NULLPTR;
    _markLineH = NULLPTR;
    _markLineV = NULLPTR;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsPedals::updateView()
{
    _scene->clear();

    short w = width();
    short h = height();

    double h_2 = h / 2.0;
    double w_2 = w / 2.0;

    short x_max = floor( w_2 ) - 1;

    short x_ctrl = floor( _ctrlYaw * ( w_2 - 1.0 ) / 100.0 + 0.5 );
    short x_trim = floor( _trimYaw * ( w_2 - 1.0 ) / 100.0 + 0.5 );

    if ( x_ctrl < -x_max ) x_ctrl = -x_max;
    if ( x_ctrl >  x_max ) x_ctrl =  x_max;
    if ( x_trim < -x_max ) x_trim = -x_max;
    if ( x_trim >  x_max ) x_trim =  x_max;

    _markLineH = _scene->addLine( -w_2, 0, w_2, 0, _markPen );
    _markLineV = _scene->addLine( 0, h_2, 0, -h_2, _markPen );

    _trimLineV = _scene->addLine( x_trim, h_2, x_trim, -h_2, _trimPen );
    _ctrlLineV = _scene->addLine( x_ctrl, h_2, x_ctrl, -h_2, _ctrlPen );

    _scene->update();

    centerOn( 0, 0 );
}
