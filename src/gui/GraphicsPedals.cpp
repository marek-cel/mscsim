/****************************************************************************//*
 * Copyright (C) 2019 Marek M. Cel
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

#include <math.h>
#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////

GraphicsPedals::GraphicsPedals( QWidget *parent ) :
    QGraphicsView ( parent ),

    m_timerId ( 0 ),

    m_scene ( 0 ),

    m_ctrlLineV ( 0 ),
    m_trimLineV ( 0 ),
    m_markLineH ( 0 ),
    m_markLineV ( 0 ),

    m_ctrlBrush ( QColor( 0x03, 0x2a, 0x63 ), Qt::SolidPattern ),
    m_trimBrush ( QColor( 0x88, 0x88, 0x88 ), Qt::SolidPattern ),
    m_markBrush ( QColor( 0xff, 0xff, 0xff ), Qt::SolidPattern ),

    m_ctrlPen( m_ctrlBrush, 1 ),
    m_trimPen( m_trimBrush, 1 ),
    m_markPen( m_markBrush, 1 ),

    m_ctrlYaw ( 0 ),
    m_trimYaw ( 0 )
{
    m_scene = new QGraphicsScene( this );
    setScene( m_scene );

    m_scene->clear();

    init();

    m_timerId = startTimer( 100 );
}

////////////////////////////////////////////////////////////////////////////////

GraphicsPedals::~GraphicsPedals()
{
    if ( m_timerId ) killTimer( m_timerId );

    if ( m_scene )
    {
        m_scene->clear();
        delete m_scene;
        m_scene = 0;
    }

    reset();
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsPedals::reinit()
{
    if ( m_scene )
    {
        m_scene->clear();

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
    m_ctrlLineV = 0;
    m_trimLineV = 0;
    m_markLineH = 0;
    m_markLineV = 0;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsPedals::updateView()
{
    m_scene->clear();

    short w = width();
    short h = height();

    float h_2 = h / 2.0f;
    float w_2 = w / 2.0f;

    short x_max = floor( w_2 ) - 1;

    short x_ctrl = floor( m_ctrlYaw * ( w_2 - 1.0f ) / 100.0f + 0.5f );
    short x_trim = floor( m_trimYaw * ( w_2 - 1.0f ) / 100.0f + 0.5f );

    if ( x_ctrl < -x_max ) x_ctrl = -x_max;
    if ( x_ctrl >  x_max ) x_ctrl =  x_max;
    if ( x_trim < -x_max ) x_trim = -x_max;
    if ( x_trim >  x_max ) x_trim =  x_max;

    m_markLineH = m_scene->addLine( -w_2, 0, w_2, 0, m_markPen );
    m_markLineV = m_scene->addLine( 0, h_2, 0, -h_2, m_markPen );

    m_trimLineV = m_scene->addLine( x_trim, h_2, x_trim, -h_2, m_trimPen );
    m_ctrlLineV = m_scene->addLine( x_ctrl, h_2, x_ctrl, -h_2, m_ctrlPen );

    m_scene->update();

    centerOn( 0, 0 );
}
