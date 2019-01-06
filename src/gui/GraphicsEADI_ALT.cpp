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

GraphicsEADI::ALT::ALT( QGraphicsScene *scene ) :
    m_scene ( scene ),

    m_itemBack     ( 0 ),
    m_itemScale1   ( 0 ),
    m_itemScale2   ( 0 ),
    m_itemLabel1   ( 0 ),
    m_itemLabel2   ( 0 ),
    m_itemLabel3   ( 0 ),
    m_itemGround   ( 0 ),
    m_itemFrame    ( 0 ),
    m_itemAltitude ( 0 ),
    m_itemPressure ( 0 ),

    m_frameTextColor ( 255, 255, 255 ),
    m_pressTextColor (   0, 255,   0 ),
    m_labelsColor    ( 255, 255, 255 ),

    m_altitude ( 0.0f ),

    m_scale1DeltaY_new ( 0.0f ),
    m_scale1DeltaY_old ( 0.0f ),
    m_scale2DeltaY_new ( 0.0f ),
    m_scale2DeltaY_old ( 0.0f ),
    m_groundDeltaY_new ( 0.0f ),
    m_groundDeltaY_old ( 0.0f ),
    m_labelsDeltaY_new ( 0.0f ),
    m_labelsDeltaY_old ( 0.0f ),

    m_scaleX ( 1.0f ),
    m_scaleY ( 1.0f ),

    m_originalPixPerAlt   ( 0.150f ),
    m_originalScaleHeight ( 300.0f ),
    m_originalLabelsX     ( 250.0f ),
    m_originalLabel1Y     (  50.0f ),
    m_originalLabel2Y     ( 125.0f ),
    m_originalLabel3Y     ( 200.0f ),

    m_originalBackPos     ( 231.0f ,   37.5f ),
    m_originalScale1Pos   ( 231.0f , -174.5f ),
    m_originalScale2Pos   ( 231.0f , -474.5f ),
    m_originalGroundPos   ( 231.5f ,  124.5f ),
    m_originalFramePos    ( 225.0f ,  110.0f ),
    m_originalAltitudeCtr ( 254.0f ,  126.0f ),
    m_originalPressureCtr ( 254.0f ,  225.0f ),

    m_backZ      (  70 ),
    m_scaleZ     (  77 ),
    m_labelsZ    (  78 ),
    m_groundZ    (  79 ),
    m_frameZ     ( 110 ),
    m_frameTextZ ( 120 )
{
#   ifdef WIN32
    m_frameTextFont.setFamily( "Courier" );
    m_frameTextFont.setPointSizeF( 9.0f );
    m_frameTextFont.setStretch( QFont::Condensed );
    m_frameTextFont.setWeight( QFont::Bold );

    m_labelsFont.setFamily( "Courier" );
    m_labelsFont.setPointSizeF( 7.0f );
    m_labelsFont.setStretch( QFont::Condensed );
    m_labelsFont.setWeight( QFont::Bold );
#   else
    m_frameTextFont.setFamily( "courier" );
    m_frameTextFont.setPointSizeF( 10.0f );
    m_frameTextFont.setStretch( QFont::Condensed );
    m_frameTextFont.setWeight( QFont::Bold );

    m_labelsFont.setFamily( "courier" );
    m_labelsFont.setPointSizeF( 8.0f );
    m_labelsFont.setStretch( QFont::Condensed );
    m_labelsFont.setWeight( QFont::Bold );
#   endif

    reset();
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ALT::init( float scaleX, float scaleY )
{
    m_scaleX = scaleX;
    m_scaleY = scaleY;

    reset();

    m_itemBack = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_alt_back.svg" );
    m_itemBack->setCacheMode( QGraphicsItem::NoCache );
    m_itemBack->setZValue( m_backZ );
    m_itemBack->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemBack->moveBy( m_scaleX * m_originalBackPos.x(), m_scaleY * m_originalBackPos.y() );
    m_scene->addItem( m_itemBack );

    m_itemScale1 = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_alt_scale.svg" );
    m_itemScale1->setCacheMode( QGraphicsItem::NoCache );
    m_itemScale1->setZValue( m_scaleZ );
    m_itemScale1->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemScale1->moveBy( m_scaleX * m_originalScale1Pos.x(), m_scaleY * m_originalScale1Pos.y() );
    m_scene->addItem( m_itemScale1 );

    m_itemScale2 = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_alt_scale.svg" );
    m_itemScale2->setCacheMode( QGraphicsItem::NoCache );
    m_itemScale2->setZValue( m_scaleZ );
    m_itemScale2->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemScale2->moveBy( m_scaleX * m_originalScale2Pos.x(), m_scaleY * m_originalScale2Pos.y() );
    m_scene->addItem( m_itemScale2 );

    m_itemLabel1 = new QGraphicsTextItem( QString( "99999" ) );
    m_itemLabel1->setCacheMode( QGraphicsItem::NoCache );
    m_itemLabel1->setZValue( m_labelsZ );
    m_itemLabel1->setDefaultTextColor( m_labelsColor );
    m_itemLabel1->setFont( m_labelsFont );
    m_itemLabel1->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemLabel1->moveBy( m_scaleX * ( m_originalLabelsX - m_itemLabel1->boundingRect().width()  / 2.0f ),
                         m_scaleY * ( m_originalLabel1Y - m_itemLabel1->boundingRect().height() / 2.0f ) );
    m_scene->addItem( m_itemLabel1 );

    m_itemLabel2 = new QGraphicsTextItem( QString( "99999" ) );
    m_itemLabel2->setCacheMode( QGraphicsItem::NoCache );
    m_itemLabel2->setZValue( m_labelsZ );
    m_itemLabel2->setDefaultTextColor( m_labelsColor );
    m_itemLabel2->setFont( m_labelsFont );
    m_itemLabel2->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemLabel2->moveBy( m_scaleX * ( m_originalLabelsX - m_itemLabel2->boundingRect().width()  / 2.0f ),
                         m_scaleY * ( m_originalLabel2Y - m_itemLabel2->boundingRect().height() / 2.0f ) );
    m_scene->addItem( m_itemLabel2 );

    m_itemLabel3 = new QGraphicsTextItem( QString( "99999" ) );
    m_itemLabel3->setCacheMode( QGraphicsItem::NoCache );
    m_itemLabel3->setZValue( m_labelsZ );
    m_itemLabel3->setDefaultTextColor( m_labelsColor );
    m_itemLabel3->setFont( m_labelsFont );
    m_itemLabel3->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemLabel3->moveBy( m_scaleX * ( m_originalLabelsX - m_itemLabel3->boundingRect().width()  / 2.0f ),
                         m_scaleY * ( m_originalLabel3Y - m_itemLabel3->boundingRect().height() / 2.0f ) );
    m_scene->addItem( m_itemLabel3 );

    m_itemGround = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_alt_ground.svg" );
    m_itemGround->setCacheMode( QGraphicsItem::NoCache );
    m_itemGround->setZValue( m_groundZ );
    m_itemGround->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemGround->moveBy( m_scaleX * m_originalGroundPos.x(), m_scaleY * m_originalGroundPos.y() );
    m_scene->addItem( m_itemGround );

    m_itemFrame = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_alt_frame.svg" );
    m_itemFrame->setCacheMode( QGraphicsItem::NoCache );
    m_itemFrame->setZValue( m_frameZ );
    m_itemFrame->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemFrame->moveBy( m_scaleX * m_originalFramePos.x(), m_scaleY * m_originalFramePos.y() );
    m_scene->addItem( m_itemFrame );

    m_itemAltitude = new QGraphicsTextItem( QString( "    0" ) );
    m_itemAltitude->setCacheMode( QGraphicsItem::NoCache );
    m_itemAltitude->setZValue( m_frameTextZ );
    m_itemAltitude->setDefaultTextColor( m_frameTextColor );
    m_itemAltitude->setFont( m_frameTextFont );
    m_itemAltitude->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemAltitude->moveBy( m_scaleX * ( m_originalAltitudeCtr.x() - m_itemAltitude->boundingRect().width()  / 2.0f ),
                           m_scaleY * ( m_originalAltitudeCtr.y() - m_itemAltitude->boundingRect().height() / 2.0f ) );
    m_scene->addItem( m_itemAltitude );

    m_itemPressure = new QGraphicsTextItem( QString( "  STD  " ) );
    m_itemPressure->setCacheMode( QGraphicsItem::NoCache );
    m_itemPressure->setZValue( m_frameTextZ );
    m_itemPressure->setDefaultTextColor( m_pressTextColor );
    m_itemPressure->setFont( m_frameTextFont );
    m_itemPressure->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemPressure->moveBy( m_scaleX * ( m_originalPressureCtr.x() - m_itemPressure->boundingRect().width()  / 2.0f ),
                           m_scaleY * ( m_originalPressureCtr.y() - m_itemPressure->boundingRect().height() / 2.0f ) );
    m_scene->addItem( m_itemPressure );

    update( scaleX, scaleY );
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ALT::update( float scaleX, float scaleY )
{
    m_scaleX = scaleX;
    m_scaleY = scaleY;

    updateAltitude();

    m_scale1DeltaY_old = m_scale1DeltaY_new;
    m_scale2DeltaY_old = m_scale2DeltaY_new;
    m_groundDeltaY_old = m_groundDeltaY_new;
    m_labelsDeltaY_old = m_labelsDeltaY_new;

}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ALT::setAltitude( float altitude )
{
    m_altitude = altitude;

    if      ( m_altitude <     0.0f ) m_altitude =     0.0f;
    else if ( m_altitude > 99999.0f ) m_altitude = 99999.0f;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ALT::reset()
{
    m_itemBack     = 0;
    m_itemScale1   = 0;
    m_itemScale2   = 0;
    m_itemLabel1   = 0;
    m_itemLabel2   = 0;
    m_itemLabel3   = 0;
    m_itemGround   = 0;
    m_itemFrame    = 0;
    m_itemAltitude = 0;
    m_itemPressure = 0;

    m_altitude = 0.0f;

    m_scale1DeltaY_new = 0.0f;
    m_scale1DeltaY_old = 0.0f;
    m_scale2DeltaY_new = 0.0f;
    m_scale2DeltaY_old = 0.0f;
    m_groundDeltaY_new = 0.0f;
    m_groundDeltaY_old = 0.0f;
    m_labelsDeltaY_new = 0.0f;
    m_labelsDeltaY_old = 0.0f;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ALT::updateAltitude()
{
    m_itemAltitude->setPlainText( QString("%1").arg(m_altitude, 5, 'f', 0, QChar(' ')) );

    updateScale();
    updateScaleLabels();
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ALT::updateScale()
{
    m_scale1DeltaY_new = m_scaleY * m_originalPixPerAlt * m_altitude;
    m_scale2DeltaY_new = m_scale1DeltaY_new;
    m_groundDeltaY_new = m_scale1DeltaY_new;

    float scaleSingleHeight = m_scaleY * m_originalScaleHeight;
    float scaleDoubleHeight = m_scaleY * m_originalScaleHeight * 2.0f;

    while ( m_scale1DeltaY_new > scaleSingleHeight + m_scaleY * 74.5f )
    {
        m_scale1DeltaY_new = m_scale1DeltaY_new - scaleDoubleHeight;
    }

    while ( m_scale2DeltaY_new > scaleDoubleHeight + m_scaleY * 74.5f )
    {
        m_scale2DeltaY_new = m_scale2DeltaY_new - scaleDoubleHeight;
    }

    if ( m_groundDeltaY_new > m_scaleY * 100.0f ) m_groundDeltaY_new = m_scaleY * 100.0f;

    m_itemScale1->moveBy( 0.0f, m_scale1DeltaY_new - m_scale1DeltaY_old );
    m_itemScale2->moveBy( 0.0f, m_scale2DeltaY_new - m_scale2DeltaY_old );
    m_itemGround->moveBy( 0.0f, m_groundDeltaY_new - m_groundDeltaY_old );
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ALT::updateScaleLabels()
{
    int tmp = floor( m_altitude + 0.5f );
    int alt = tmp - ( tmp % 500 );

    float alt1 = (float)alt + 500.0f;
    float alt2 = (float)alt;
    float alt3 = (float)alt - 500.0f;

    m_labelsDeltaY_new = m_scaleY * m_originalPixPerAlt * m_altitude;

    while ( m_labelsDeltaY_new > m_scaleY * 37.5f )
    {
        m_labelsDeltaY_new = m_labelsDeltaY_new - m_scaleY * 75.0f;
    }

    if ( m_labelsDeltaY_new < 0.0f && m_altitude > alt2 )
    {
        alt1 += 500.0f;
        alt2 += 500.0f;
        alt3 += 500.0f;
    }

    m_itemLabel1->moveBy( 0.0f, m_labelsDeltaY_new - m_labelsDeltaY_old );
    m_itemLabel2->moveBy( 0.0f, m_labelsDeltaY_new - m_labelsDeltaY_old );
    m_itemLabel3->moveBy( 0.0f, m_labelsDeltaY_new - m_labelsDeltaY_old );

    if ( alt1 > 0.0f && alt1 <= 100000.0f )
    {
        m_itemLabel1->setVisible( true );
        m_itemLabel1->setPlainText( QString("%1").arg(alt1, 5, 'f', 0, QChar(' ')) );
    }
    else
    {
        m_itemLabel1->setVisible( false );
    }

    if ( alt2 > 0.0f && alt2 <= 100000.0f )
    {
        m_itemLabel2->setVisible( true );
        m_itemLabel2->setPlainText( QString("%1").arg(alt2, 5, 'f', 0, QChar(' ')) );
    }
    else
    {
        m_itemLabel2->setVisible( false );
    }

    if ( alt3 > 0.0f && alt3 <= 100000.0f )
    {
        m_itemLabel3->setVisible( true );
        m_itemLabel3->setPlainText( QString("%1").arg(alt3, 5, 'f', 0, QChar(' ')) );
    }
    else
    {
        m_itemLabel3->setVisible( false );
    }
}
