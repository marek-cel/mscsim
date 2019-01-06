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

#ifdef WIN32
#   include <float.h>
#endif

#include <math.h>
#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////

GraphicsEADI::HDG::HDG( QGraphicsScene *scene ) :
    m_scene ( scene ),

    m_itemBack      ( 0 ),
    m_itemFace      ( 0 ),
    m_itemMarks     ( 0 ),
    m_itemFrameText ( 0 ),

    m_frameTextColor ( 255, 255, 255 ),

    m_heading  ( 0.0f ),

    m_scaleX ( 1.0f ),
    m_scaleY ( 1.0f ),

    m_originalHsiCtr       ( 150.0f , 345.0f ),
    m_originalBackPos      (  60.0f,  240.0f ),
    m_originalFacePos      (  45.0f , 240.0f ),
    m_originalMarksPos     ( 134.0f , 219.0f ),
    m_originalFrameTextCtr ( 149.5f , 227.5f ),

    m_backZ      (  80 ),
    m_faceZ      (  90 ),
    m_marksZ     ( 110 ),
    m_frameTextZ ( 120 )
{
    m_frameTextFont.setFamily( "Courier" );
    m_frameTextFont.setPointSizeF( 10.0 );
    m_frameTextFont.setStretch( QFont::Condensed );
    m_frameTextFont.setWeight( QFont::Bold );

    reset();
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::HDG::init( float scaleX, float scaleY )
{
    m_scaleX = scaleX;
    m_scaleY = scaleY;

    reset();

    m_itemBack = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_hsi_back.svg" );
    m_itemBack->setCacheMode( QGraphicsItem::NoCache );
    m_itemBack->setZValue( m_backZ );
    m_itemBack->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemBack->moveBy( m_scaleX * m_originalBackPos.x(), m_scaleY * m_originalBackPos.y() );
    m_scene->addItem( m_itemBack );

    m_itemFace = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_hsi_face.svg" );
    m_itemFace->setCacheMode( QGraphicsItem::NoCache );
    m_itemFace->setZValue( m_faceZ );
    m_itemFace->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemFace->setTransformOriginPoint( m_originalHsiCtr - m_originalFacePos );
    m_itemFace->moveBy( m_scaleX * m_originalFacePos.x(), m_scaleY * m_originalFacePos.y() );
    m_scene->addItem( m_itemFace );

    m_itemMarks = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_hsi_marks.svg" );
    m_itemMarks->setCacheMode( QGraphicsItem::NoCache );
    m_itemMarks->setZValue( m_marksZ );
    m_itemMarks->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemMarks->moveBy( m_scaleX * m_originalMarksPos.x(), m_scaleY * m_originalMarksPos.y() );
    m_scene->addItem( m_itemMarks );

    m_itemFrameText = new QGraphicsTextItem( QString( "000" ) );
    m_itemFrameText->setCacheMode( QGraphicsItem::NoCache );
    m_itemFrameText->setZValue( m_frameTextZ );
    m_itemFrameText->setTextInteractionFlags( Qt::NoTextInteraction );
    m_itemFrameText->setDefaultTextColor( m_frameTextColor );
    m_itemFrameText->setFont( m_frameTextFont );
    m_itemFrameText->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemFrameText->moveBy( m_scaleX * ( m_originalFrameTextCtr.x() - m_itemFrameText->boundingRect().width()  / 2.0f ),
                             m_scaleY * ( m_originalFrameTextCtr.y() - m_itemFrameText->boundingRect().height() / 2.0f ) );
    m_scene->addItem( m_itemFrameText );

    update( scaleX, scaleY );
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::HDG::update( float scaleX, float scaleY )
{
    m_scaleX = scaleX;
    m_scaleY = scaleY;

    updateHeading();
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::HDG::setHeading( float heading )
{
    m_heading = heading;

    while ( m_heading < 0.0f )
    {
        m_heading += 360.0f;
    }

    while ( m_heading > 360.0f )
    {
        m_heading -= 360.0f;
    }
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::HDG::reset()
{
    m_itemBack      = 0;
    m_itemFace      = 0;
    m_itemMarks     = 0;
    m_itemFrameText = 0;

    m_heading  = 0.0f;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::HDG::updateHeading()
{
    m_itemFace->setRotation( - m_heading );

    float fHeading = floor( m_heading + 0.5f );

    m_itemFrameText->setPlainText( QString("%1").arg(fHeading, 3, 'f', 0, QChar('0')) );
}
