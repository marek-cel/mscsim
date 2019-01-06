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

#include <gui/GraphicsEADI.h>

////////////////////////////////////////////////////////////////////////////////

GraphicsEADI::GraphicsEADI( QWidget *parent ) :
    QGraphicsView ( parent ),

    m_timerId ( 0 ),

    m_scene ( 0 ),

    m_adi ( 0 ),
    m_alt ( 0 ),
    m_asi ( 0 ),
    m_hdg ( 0 ),
    m_vsi ( 0 ),

    m_itemBack ( 0 ),
    m_itemMask ( 0 ),

    m_scaleX ( 1.0f ),
    m_scaleY ( 1.0f ),

    m_originalHeight ( 300 ),
    m_originalWidth  ( 300 ),

    m_backZ (   0 ),
    m_maskZ ( 100 )
{
    reset();

    m_scene = new QGraphicsScene( this );
    setScene( m_scene );

    m_scene->clear();

    m_adi = new GraphicsEADI::ADI( m_scene );
    m_alt = new GraphicsEADI::ALT( m_scene );
    m_asi = new GraphicsEADI::ASI( m_scene );
    m_hdg = new GraphicsEADI::HDG( m_scene );
    m_vsi = new GraphicsEADI::VSI( m_scene );

    init();

    m_timerId = startTimer( 50 );
}

////////////////////////////////////////////////////////////////////////////////

GraphicsEADI::~GraphicsEADI()
{
    if ( m_timerId ) killTimer( m_timerId );

    if ( m_scene )
    {
        m_scene->clear();
        delete m_scene;
        m_scene = 0;
    }

    reset();

    if ( m_adi ) delete m_adi;
    m_adi = 0;

    if ( m_alt ) delete m_alt;
    m_alt = 0;

    if ( m_asi ) delete m_asi;
    m_asi = 0;

    if ( m_hdg ) delete m_hdg;
    m_hdg = 0;

    if ( m_vsi ) delete m_vsi;
    m_vsi = 0;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::reinit()
{
    if ( m_scene )
    {
        m_scene->clear();

        init();
    }
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::resizeEvent( QResizeEvent *event )
{
    ////////////////////////////////////
    QGraphicsView::resizeEvent( event );
    ////////////////////////////////////

    reinit();
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::timerEvent( QTimerEvent *event )
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

void GraphicsEADI::init()
{
    m_scaleX = (float)width()  / (float)m_originalWidth;
    m_scaleY = (float)height() / (float)m_originalHeight;

    m_adi->init( m_scaleX, m_scaleY );
    m_alt->init( m_scaleX, m_scaleY );
    m_asi->init( m_scaleX, m_scaleY );
    m_hdg->init( m_scaleX, m_scaleY );
    m_vsi->init( m_scaleX, m_scaleY );

    m_itemBack = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_back.svg" );
    m_itemBack->setCacheMode( QGraphicsItem::NoCache );
    m_itemBack->setZValue( m_backZ );
    m_itemBack->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_scene->addItem( m_itemBack );

    m_itemMask = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_mask.svg" );
    m_itemMask->setCacheMode( QGraphicsItem::NoCache );
    m_itemMask->setZValue( m_maskZ );
    m_itemMask->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_scene->addItem( m_itemMask );

    updateView();
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::reset()
{
    m_itemBack = 0;
    m_itemMask = 0;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::updateView()
{
    m_scaleX = (float)width()  / (float)m_originalWidth;
    m_scaleY = (float)height() / (float)m_originalHeight;

    m_adi->update( m_scaleX, m_scaleY );
    m_alt->update( m_scaleX, m_scaleY );
    m_vsi->update( m_scaleX, m_scaleY );
    m_asi->update( m_scaleX, m_scaleY );
    m_hdg->update( m_scaleX, m_scaleY );

    m_scene->update();

    centerOn( width() / 2.0f , height() / 2.0f );
}
