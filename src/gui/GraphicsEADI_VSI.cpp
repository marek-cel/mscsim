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

////////////////////////////////////////////////////////////////////////////////

GraphicsEADI::VSI::VSI( QGraphicsScene *scene ) :
    m_scene ( scene ),

    m_itemScale  ( 0 ),
    m_itemMarker ( 0 ),

    m_climbRate ( 0.0f ),

    m_scaleX ( 1.0f ),
    m_scaleY ( 1.0f ),

    m_originalMarkeWidth (  4.0f ),
    m_originalPixPerSpd1 ( 30.0f ),
    m_originalPixPerSpd2 ( 20.0f ),
    m_originalPixPerSpd4 (  5.0f ),

    m_originalScalePos  ( 275.0f ,  50.0f ),
    m_originalMarkerPos ( 285.0f , 124.5f ),

    m_markerBrush ( QColor( 0xff, 0xff, 0xff ), Qt::SolidPattern ),
    m_markerPen ( m_markerBrush, 0 ),

    m_scaleZ  ( 70 ),
    m_markerZ ( 8000 )
{
    reset();
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::VSI::init( float scaleX, float scaleY )
{
    m_scaleX = scaleX;
    m_scaleY = scaleY;

    reset();

    m_itemScale = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_vsi_scale.svg" );
    m_itemScale->setCacheMode( QGraphicsItem::NoCache );
    m_itemScale->setZValue( m_scaleZ );
    m_itemScale->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemScale->moveBy( m_scaleX * m_originalScalePos.x(), m_scaleY * m_originalScalePos.y() );
    m_scene->addItem( m_itemScale );

    m_itemMarker = m_scene->addRect( m_scaleX * m_originalMarkerPos.x(),
                                     m_scaleY * m_originalMarkerPos.y(),
                                     m_scaleX * m_originalMarkeWidth,
                                     m_scaleY * 10,
                                     m_markerPen, m_markerBrush );
    m_itemMarker->setZValue( m_markerZ );

    update( scaleX, scaleY );
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::VSI::update( float scaleX, float scaleY )
{
    m_scaleX = scaleX;
    m_scaleY = scaleY;

    updateVSI();
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::VSI::setClimbRate( float climbRate )
{
    m_climbRate = climbRate;

    if      ( m_climbRate >  6.3f ) m_climbRate =  6.3f;
    else if ( m_climbRate < -6.3f ) m_climbRate = -6.3f;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::VSI::reset()
{
    m_itemScale = 0;

    m_climbRate = 0.0;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::VSI::updateVSI()
{
    //m_scene->clear();

    float climbRateAbs = fabs( m_climbRate );
    float arrowDeltaY = 0.0;

    if ( climbRateAbs <= 1.0f )
    {
        arrowDeltaY = m_originalPixPerSpd1 * climbRateAbs;
    }
    else if ( climbRateAbs <= 2.0f )
    {
        arrowDeltaY = m_originalPixPerSpd1 + m_originalPixPerSpd2 * ( climbRateAbs - 1.0f );
    }
    else
    {
        arrowDeltaY = m_originalPixPerSpd1 + m_originalPixPerSpd2 + m_originalPixPerSpd4 * ( climbRateAbs - 2.0f );
    }

    if ( m_climbRate > 0.0f )
    {
        m_itemMarker->setRect( m_scaleX * m_originalMarkerPos.x(),
                               m_scaleY * m_originalMarkerPos.y() - m_scaleY * arrowDeltaY,
                               m_scaleX * m_originalMarkeWidth,
                               m_scaleY * arrowDeltaY );
    }
    else
    {
        m_itemMarker->setRect( m_scaleX * m_originalMarkerPos.x(),
                               m_scaleY * m_originalMarkerPos.y(),
                               m_scaleX * m_originalMarkeWidth,
                               m_scaleY * arrowDeltaY );
    }
}
