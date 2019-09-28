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

#include <gui/GraphicsEHSI.h>

#ifdef WIN32
#   include <float.h>
#endif

#include <math.h>
#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////

GraphicsEHSI::GraphicsEHSI( QWidget *parent ) :
    QGraphicsView ( parent ),

    _timerId ( 0 ),

    _scene ( NULLPTR ),

    _itemBack ( NULLPTR ),
    _itemMask ( NULLPTR ),
    _itemMark ( NULLPTR ),

    _itemBrgArrow ( NULLPTR ),
    _itemCrsArrow ( NULLPTR ),
    _itemDevBar   ( NULLPTR ),
    _itemDevScale ( NULLPTR ),
    _itemHdgBug   ( NULLPTR ),
    _itemHdgScale ( NULLPTR ),

    _itemCrsText ( NULLPTR ),
    _itemHdgText ( NULLPTR ),
    _itemDmeText ( NULLPTR ),

    _crsTextColor (   0, 255,   0 ),
    _hdgTextColor ( 255,   0, 255 ),
    _dmeTextColor ( 255, 255, 255 ),

    _heading    ( 0.0f ),
    _headingBug ( 0.0f ),
    _course     ( 0.0f ),
    _bearing    ( 0.0f ),
    _deviation  ( 0.0f ),
    _distance   ( 0.0f ),

    _bearingVisible   ( true ),
    _deviationVisible ( true ),
    _distanceVisible  ( true ),

    _devBarDeltaX_new ( 0.0f ),
    _devBarDeltaX_old ( 0.0f ),
    _devBarDeltaY_new ( 0.0f ),
    _devBarDeltaY_old ( 0.0f ),

    _scaleX ( 1.0f ),
    _scaleY ( 1.0f ),

    _originalPixPerDev ( 52.5f ),

    _originalNavCtr ( 150.0f, 150.0f ),

    _originalCrsTextCtr (  50.0f,  25.0f ),
    _originalHdgTextCtr ( 250.0f,  25.0f ),
    _originalDmeTextCtr ( 250.0f, 275.0f ),

    _originalHeight ( 300 ),
    _originalWidth  ( 300 ),

    _backZ (   0 ),
    _maskZ ( 100 ),
    _markZ ( 200 ),

    _brgArrowZ (  60 ),
    _crsArrowZ (  70 ),
    _crsTextZ  ( 130 ),
    _devBarZ   (  50 ),
    _devScaleZ (  10 ),
    _hdgBugZ   ( 120 ),
    _hdgScaleZ ( 110 ),
    _hdgTextZ  ( 130 ),
    _dmeTextZ  ( 130 )
{
#   ifdef WIN32
    _crsTextFont.setFamily( "Courier" );
    _crsTextFont.setPointSizeF( 12.0f );
    _crsTextFont.setStretch( QFont::Condensed );
    _crsTextFont.setWeight( QFont::Bold );

    _hdgTextFont.setFamily( "Courier" );
    _hdgTextFont.setPointSizeF( 12.0f );
    _hdgTextFont.setStretch( QFont::Condensed );
    _hdgTextFont.setWeight( QFont::Bold );

    _dmeTextFont.setFamily( "Courier" );
    _dmeTextFont.setPointSizeF( 10.0f );
    _dmeTextFont.setStretch( QFont::Condensed );
    _dmeTextFont.setWeight( QFont::Bold );
#   else
    _crsTextFont.setFamily( "courier" );
    _crsTextFont.setPointSizeF( 12.0f );
    _crsTextFont.setStretch( QFont::Condensed );
    _crsTextFont.setWeight( QFont::Bold );

    _hdgTextFont.setFamily( "courier" );
    _hdgTextFont.setPointSizeF( 12.0f );
    _hdgTextFont.setStretch( QFont::Condensed );
    _hdgTextFont.setWeight( QFont::Bold );

    _dmeTextFont.setFamily( "courier" );
    _dmeTextFont.setPointSizeF( 10.0f );
    _dmeTextFont.setStretch( QFont::Condensed );
    _dmeTextFont.setWeight( QFont::Bold );
#   endif

    reset();

    _scene = new QGraphicsScene( this );
    setScene( _scene );

    _scene->clear();

    init();

    _timerId = startTimer( 50 );
}

////////////////////////////////////////////////////////////////////////////////

GraphicsEHSI::~GraphicsEHSI()
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

void GraphicsEHSI::reinit()
{
    if ( _scene )
    {
        _scene->clear();

        init();
    }
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEHSI::setHeading( float heading )
{
    _heading = heading;

    while ( _heading <   0.0f ) _heading += 360.0f;
    while ( _heading > 360.0f ) _heading -= 360.0f;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEHSI::setHeadingBug( float headingBug )
{
    _headingBug = headingBug;

    while ( _headingBug <   0.0f ) _headingBug += 360.0f;
    while ( _headingBug > 360.0f ) _headingBug -= 360.0f;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEHSI::setCourse( float course )
{
    _course = course;

    while ( _course <   0.0f ) _course += 360.0f;
    while ( _course > 360.0f ) _course -= 360.0f;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEHSI::setBearing( float bearing, bool visible )
{
    _bearing        = bearing;
    _bearingVisible = visible;

    while ( _bearing <   0.0f ) _bearing += 360.0f;
    while ( _bearing > 360.0f ) _bearing -= 360.0f;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEHSI::setDeviation( float deviation, bool visible )
{
    _deviation        = deviation;
    _deviationVisible = visible;

    if ( _deviation < -1.0f ) _deviation = -1.0f;
    if ( _deviation >  1.0f ) _deviation =  1.0f;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEHSI::setDistance( float distance, bool visible )
{
    _distance        = fabs( distance );
    _distanceVisible = visible;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEHSI::resizeEvent( QResizeEvent *event )
{
    ////////////////////////////////////
    QGraphicsView::resizeEvent( event );
    ////////////////////////////////////

    reinit();
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEHSI::timerEvent( QTimerEvent *event )
{
    ///////////////////////////////////
    QGraphicsView::timerEvent( event );
    ///////////////////////////////////

    if ( isVisible() )
    {
        updateView();

        _devBarDeltaX_old = _devBarDeltaX_new;
        _devBarDeltaY_old = _devBarDeltaY_new;
    }
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEHSI::init()
{
    _scaleX = (float)width()  / (float)_originalWidth;
    _scaleY = (float)height() / (float)_originalHeight;

    _itemBack = new QGraphicsSvgItem( ":/gui/images/efis/ehsi/ehsi_back.svg" );
    _itemBack->setCacheMode( QGraphicsItem::NoCache );
    _itemBack->setZValue( _backZ );
    _itemBack->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _scene->addItem( _itemBack );

    _itemMask = new QGraphicsSvgItem( ":/gui/images/efis/ehsi/ehsi_mask.svg" );
    _itemMask->setCacheMode( QGraphicsItem::NoCache );
    _itemMask->setZValue( _maskZ );
    _itemMask->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _scene->addItem( _itemMask );

    _itemMark = new QGraphicsSvgItem( ":/gui/images/efis/ehsi/ehsi_mark.svg" );
    _itemMark->setCacheMode( QGraphicsItem::NoCache );
    _itemMark->setZValue( _markZ );
    _itemMark->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _scene->addItem( _itemMark );

    _itemBrgArrow = new QGraphicsSvgItem( ":/gui/images/efis/ehsi/ehsi_brg_arrow.svg" );
    _itemBrgArrow->setCacheMode( QGraphicsItem::NoCache );
    _itemBrgArrow->setZValue( _brgArrowZ );
    _itemBrgArrow->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemBrgArrow->setTransformOriginPoint( _originalNavCtr );
    _scene->addItem( _itemBrgArrow );

    _itemCrsArrow = new QGraphicsSvgItem( ":/gui/images/efis/ehsi/ehsi_crs_arrow.svg" );
    _itemCrsArrow->setCacheMode( QGraphicsItem::NoCache );
    _itemCrsArrow->setZValue( _crsArrowZ );
    _itemCrsArrow->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemCrsArrow->setTransformOriginPoint( _originalNavCtr );
    _scene->addItem( _itemCrsArrow );

    _itemDevBar = new QGraphicsSvgItem( ":/gui/images/efis/ehsi/ehsi_dev_bar.svg" );
    _itemDevBar->setCacheMode( QGraphicsItem::NoCache );
    _itemDevBar->setZValue( _devBarZ );
    _itemDevBar->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemDevBar->setTransformOriginPoint( _originalNavCtr );
    _scene->addItem( _itemDevBar );

    _itemDevScale = new QGraphicsSvgItem( ":/gui/images/efis/ehsi/ehsi_dev_scale.svg" );
    _itemDevScale->setCacheMode( QGraphicsItem::NoCache );
    _itemDevScale->setZValue( _devScaleZ );
    _itemDevScale->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemDevScale->setTransformOriginPoint( _originalNavCtr );
    _scene->addItem( _itemDevScale );

    _itemHdgBug = new QGraphicsSvgItem( ":/gui/images/efis/ehsi/ehsi_hdg_bug.svg" );
    _itemHdgBug->setCacheMode( QGraphicsItem::NoCache );
    _itemHdgBug->setZValue( _hdgBugZ );
    _itemHdgBug->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemHdgBug->setTransformOriginPoint( _originalNavCtr );
    _scene->addItem( _itemHdgBug );

    _itemHdgScale = new QGraphicsSvgItem( ":/gui/images/efis/ehsi/ehsi_hdg_scale.svg" );
    _itemHdgScale->setCacheMode( QGraphicsItem::NoCache );
    _itemHdgScale->setZValue( _hdgScaleZ );
    _itemHdgScale->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemHdgScale->setTransformOriginPoint( _originalNavCtr );
    _scene->addItem( _itemHdgScale );

    _itemCrsText = 0;

    _itemCrsText = new QGraphicsTextItem( QString( "CRS 999" ) );
    _itemCrsText->setCacheMode( QGraphicsItem::NoCache );
    _itemCrsText->setZValue( _crsTextZ );
    _itemCrsText->setDefaultTextColor( _crsTextColor );
    _itemCrsText->setFont( _crsTextFont );
    _itemCrsText->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemCrsText->moveBy( _scaleX * ( _originalCrsTextCtr.x() - _itemCrsText->boundingRect().width()  / 2.0f ),
                          _scaleY * ( _originalCrsTextCtr.y() - _itemCrsText->boundingRect().height() / 2.0f ) );
    _scene->addItem( _itemCrsText );

    _itemHdgText = new QGraphicsTextItem( QString( "HDG 999" ) );
    _itemHdgText->setCacheMode( QGraphicsItem::NoCache );
    _itemHdgText->setZValue( _hdgTextZ );
    _itemHdgText->setDefaultTextColor( _hdgTextColor );
    _itemHdgText->setFont( _hdgTextFont );
    _itemHdgText->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemHdgText->moveBy( _scaleX * ( _originalHdgTextCtr.x() - _itemHdgText->boundingRect().width()  / 2.0f ),
                          _scaleY * ( _originalHdgTextCtr.y() - _itemHdgText->boundingRect().height() / 2.0f ) );
    _scene->addItem( _itemHdgText );

    _itemDmeText = new QGraphicsTextItem( QString( "99.9 NM" ) );
    _itemDmeText->setCacheMode( QGraphicsItem::NoCache );
    _itemDmeText->setZValue( _dmeTextZ );
    _itemDmeText->setDefaultTextColor( _dmeTextColor );
    _itemDmeText->setFont( _dmeTextFont );
    _itemDmeText->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemDmeText->moveBy( _scaleX * ( _originalDmeTextCtr.x() - _itemDmeText->boundingRect().width()  / 2.0f ),
                          _scaleY * ( _originalDmeTextCtr.y() - _itemDmeText->boundingRect().height() / 2.0f ) );
    _scene->addItem( _itemDmeText );

    updateView();
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEHSI::reset()
{
    _itemBrgArrow = 0;
    _itemCrsArrow = 0;
    _itemDevBar   = 0;
    _itemDevScale = 0;
    _itemHdgBug   = 0;
    _itemHdgScale = 0;

    _itemCrsText = 0;
    _itemHdgText = 0;
    _itemDmeText = 0;

    _heading    = 0.0f;
    _headingBug = 0.0f;
    _course     = 0.0f;
    _bearing    = 0.0f;
    _deviation  = 0.0f;
    _distance   = 0.0f;

    _bearingVisible   = true;
    _deviationVisible = true;
    _distanceVisible  = true;

    _devBarDeltaX_new = 0.0f;
    _devBarDeltaX_old = 0.0f;
    _devBarDeltaY_new = 0.0f;
    _devBarDeltaY_old = 0.0f;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEHSI::updateView()
{
    _scaleX = (float)width()  / (float)_originalWidth;
    _scaleY = (float)height() / (float)_originalHeight;

    _itemCrsArrow->setRotation( -_heading + _course );
    _itemHdgBug->setRotation( -_heading + _headingBug );
    _itemHdgScale->setRotation( -_heading );

    if ( _bearingVisible )
    {
        _itemBrgArrow->setVisible( true );
        _itemBrgArrow->setRotation( -_heading + _bearing );
    }
    else
    {
        _itemBrgArrow->setVisible( false );
    }

    if ( _deviationVisible )
    {
        _itemDevBar->setVisible( true );
        _itemDevScale->setVisible( true );

        float angle_deg = -_heading + _course;
#       ifndef M_PI
        float angle_rad = 3.14159265358979323846 * angle_deg / 180.0f;
#       else
        float angle_rad = M_PI * angle_deg / 180.0f;
#       endif


        float sinAngle = sin( angle_rad );
        float cosAngle = cos( angle_rad );

        _itemDevBar->setRotation( angle_deg );
        _itemDevScale->setRotation( angle_deg );

        float delta  = _originalPixPerDev * _deviation;

        _devBarDeltaX_new = _scaleX * delta * cosAngle;
        _devBarDeltaY_new = _scaleY * delta * sinAngle;

        _itemDevBar->moveBy( _devBarDeltaX_new - _devBarDeltaX_old, _devBarDeltaY_new - _devBarDeltaY_old );
    }
    else
    {
        _itemDevBar->setVisible( false );
        _itemDevScale->setVisible( false );

        _devBarDeltaX_new = _devBarDeltaX_old;
        _devBarDeltaY_new = _devBarDeltaY_old;
    }

    _itemCrsText->setPlainText( QString("CRS %1").arg( _course     , 3, 'f', 0, QChar('0') ) );
    _itemHdgText->setPlainText( QString("HDG %1").arg( _headingBug , 3, 'f', 0, QChar('0') ) );

    if ( _distanceVisible )
    {
        _itemDmeText->setVisible( true );
        _itemDmeText->setPlainText( QString("%1 NM").arg( _distance, 5, 'f', 1, QChar(' ') ) );
    }
    else
    {
        _itemDmeText->setVisible( false );
    }

    _scene->update();

    centerOn( width() / 2.0f , height() / 2.0f );
}
