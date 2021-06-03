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

#include <gui/GraphicsEADI.h>

#ifdef WIN32
#   include <float.h>
#endif

#include <cmath>

#include <gui/Colors.h>
#include <gui/Fonts.h>

////////////////////////////////////////////////////////////////////////////////

GraphicsEADI::GraphicsEADI( QWidget *parent ) :
    QGraphicsView ( parent ),

    _timerId ( 0 ),

    _scene ( Q_NULLPTR ),

    _adi ( Q_NULLPTR ),
    _alt ( Q_NULLPTR ),
    _asi ( Q_NULLPTR ),
    _hdg ( Q_NULLPTR ),
    _vsi ( Q_NULLPTR ),

    _itemBack ( Q_NULLPTR ),
    _itemMask ( Q_NULLPTR ),

    _itemFMA ( Q_NULLPTR ),
    _itemSPD ( Q_NULLPTR ),

    _itemLNAV ( Q_NULLPTR ),
    _itemVNAV ( Q_NULLPTR ),

    _itemLNAV_ARM ( Q_NULLPTR ),
    _itemVNAV_ARM ( Q_NULLPTR ),

    _fltMode ( FltMode::Off ),
    _spdMode ( SpdMode::Off ),

    _lnav ( LNAV::Off ),
    _vnav ( VNAV::Off ),

    _scaleX ( 1.0 ),
    _scaleY ( 1.0 ),

    _originalFMA ( 150.0, 42.0 ),
    _originalSPD ( 100.0, 12.0 ),

    _originalLNAV ( 150.0, 12.0 ),
    _originalVNAV ( 200.0, 12.0 ),

    _originalLNAV_ARM ( 150.0, 22.0 ),
    _originalVNAV_ARM ( 200.0, 22.0 ),

    _originalHeight ( 300 ),
    _originalWidth  ( 300 ),

    _backZ (   0 ),
    _maskZ ( 100 ),
    _textZ ( 120 )
{
    reset();

    _scene = new QGraphicsScene( this );
    setScene( _scene );

    _scene->clear();

    _adi = new GraphicsEADI::ADI( _scene );
    _alt = new GraphicsEADI::ALT( _scene );
    _asi = new GraphicsEADI::ASI( _scene );
    _hdg = new GraphicsEADI::HDG( _scene );
    _vsi = new GraphicsEADI::VSI( _scene );

    init();

    _timerId = startTimer( 50 );
}

////////////////////////////////////////////////////////////////////////////////

GraphicsEADI::~GraphicsEADI()
{
    if ( _timerId ) killTimer( _timerId );

    if ( _scene )
    {
        _scene->clear();
        delete _scene;
        _scene = Q_NULLPTR;
    }

    reset();

    if ( _adi ) delete _adi;
    _adi = Q_NULLPTR;

    if ( _alt ) delete _alt;
    _alt = Q_NULLPTR;

    if ( _asi ) delete _asi;
    _asi = Q_NULLPTR;

    if ( _hdg ) delete _hdg;
    _hdg = Q_NULLPTR;

    if ( _vsi ) delete _vsi;
    _vsi = Q_NULLPTR;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::reinit()
{
    if ( _scene )
    {
        _scene->clear();

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
    _scaleX = static_cast< double >( width()  ) / static_cast< double >( _originalWidth  );
    _scaleY = static_cast< double >( height() ) / static_cast< double >( _originalHeight );

    reset();

    _adi->init( _scaleX, _scaleY );
    _alt->init( _scaleX, _scaleY );
    _asi->init( _scaleX, _scaleY );
    _hdg->init( _scaleX, _scaleY );
    _vsi->init( _scaleX, _scaleY );

    _itemBack = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_back.svg" );
    _itemBack->setCacheMode( QGraphicsItem::NoCache );
    _itemBack->setZValue( _backZ );
    _itemBack->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _scene->addItem( _itemBack );

    _itemMask = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_mask.svg" );
    _itemMask->setCacheMode( QGraphicsItem::NoCache );
    _itemMask->setZValue( _maskZ );
    _itemMask->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _scene->addItem( _itemMask );

    _itemFMA = new QGraphicsTextItem( QString( "  CMD  " ) );
    _itemFMA->setCacheMode( QGraphicsItem::NoCache );
    _itemFMA->setZValue( _textZ );
    _itemFMA->setTextInteractionFlags( Qt::NoTextInteraction );
    _itemFMA->setDefaultTextColor( Colors::_lime );
    _itemFMA->setFont( Fonts::medium() );
    _itemFMA->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemFMA->moveBy( _scaleX * ( _originalFMA.x() - _itemFMA->boundingRect().width()  / 2.0 ),
                      _scaleY * ( _originalFMA.y() - _itemFMA->boundingRect().height() / 2.0 ) );
    _scene->addItem( _itemFMA );

    _itemSPD = new QGraphicsTextItem( QString( "FMC SPD" ) );
    _itemSPD->setCacheMode( QGraphicsItem::NoCache );
    _itemSPD->setZValue( _textZ );
    _itemSPD->setTextInteractionFlags( Qt::NoTextInteraction );
    _itemSPD->setDefaultTextColor( Colors::_lime );
    _itemSPD->setFont( Fonts::xsmall() );
    _itemSPD->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemSPD->moveBy( _scaleX * ( _originalSPD.x() - _itemSPD->boundingRect().width()  / 2.0 ),
                      _scaleY * ( _originalSPD.y() - _itemSPD->boundingRect().height() / 2.0 ) );
    _scene->addItem( _itemSPD );

    _itemLNAV = new QGraphicsTextItem( QString( "HDG SEL" ) );
    _itemLNAV->setCacheMode( QGraphicsItem::NoCache );
    _itemLNAV->setZValue( _textZ );
    _itemLNAV->setTextInteractionFlags( Qt::NoTextInteraction );
    _itemLNAV->setDefaultTextColor( Colors::_lime );
    _itemLNAV->setFont( Fonts::xsmall() );
    _itemLNAV->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemLNAV->moveBy( _scaleX * ( _originalLNAV.x() - _itemLNAV->boundingRect().width()  / 2.0 ),
                       _scaleY * ( _originalLNAV.y() - _itemLNAV->boundingRect().height() / 2.0 ) );
    _scene->addItem( _itemLNAV );

    _itemVNAV = new QGraphicsTextItem( QString( "ALT SEL" ) );
    _itemVNAV->setCacheMode( QGraphicsItem::NoCache );
    _itemVNAV->setZValue( _textZ );
    _itemVNAV->setTextInteractionFlags( Qt::NoTextInteraction );
    _itemVNAV->setDefaultTextColor( Colors::_lime );
    _itemVNAV->setFont( Fonts::xsmall() );
    _itemVNAV->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemVNAV->moveBy( _scaleX * ( _originalVNAV.x() - _itemVNAV->boundingRect().width()  / 2.0 ),
                       _scaleY * ( _originalVNAV.y() - _itemVNAV->boundingRect().height() / 2.0 ) );
    _scene->addItem( _itemVNAV );

    _itemLNAV_ARM = new QGraphicsTextItem( QString( "VOR/LOC" ) );
    _itemLNAV_ARM->setCacheMode( QGraphicsItem::NoCache );
    _itemLNAV_ARM->setZValue( _textZ );
    _itemLNAV_ARM->setTextInteractionFlags( Qt::NoTextInteraction );
    _itemLNAV_ARM->setDefaultTextColor( Colors::_white );
    _itemLNAV_ARM->setFont( Fonts::xsmall() );
    _itemLNAV_ARM->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemLNAV_ARM->moveBy( _scaleX * ( _originalLNAV_ARM.x() - _itemLNAV_ARM->boundingRect().width()  / 2.0 ),
                           _scaleY * ( _originalLNAV_ARM.y() - _itemLNAV_ARM->boundingRect().height() / 2.0 ) );
    _scene->addItem( _itemLNAV_ARM );

    _itemVNAV_ARM = new QGraphicsTextItem( QString( "GS PATH" ) );
    _itemVNAV_ARM->setCacheMode( QGraphicsItem::NoCache );
    _itemVNAV_ARM->setZValue( _textZ );
    _itemVNAV_ARM->setTextInteractionFlags( Qt::NoTextInteraction );
    _itemVNAV_ARM->setDefaultTextColor( Colors::_white );
    _itemVNAV_ARM->setFont( Fonts::xsmall() );
    _itemVNAV_ARM->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemVNAV_ARM->moveBy( _scaleX * ( _originalVNAV_ARM.x() - _itemVNAV_ARM->boundingRect().width()  / 2.0 ),
                           _scaleY * ( _originalVNAV_ARM.y() - _itemVNAV_ARM->boundingRect().height() / 2.0 ) );
    _scene->addItem( _itemVNAV_ARM );

    updateView();
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::reset()
{
    _itemBack = Q_NULLPTR;
    _itemMask = Q_NULLPTR;

    _itemFMA = Q_NULLPTR;
    _itemSPD = Q_NULLPTR;

    _itemLNAV = Q_NULLPTR;
    _itemVNAV = Q_NULLPTR;

    _itemLNAV_ARM = Q_NULLPTR;
    _itemVNAV_ARM = Q_NULLPTR;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::updateView()
{
    _scaleX = static_cast< double >( width()  ) / static_cast< double >( _originalWidth  );
    _scaleY = static_cast< double >( height() ) / static_cast< double >( _originalHeight );

    _adi->update( _scaleX, _scaleY );
    _alt->update( _scaleX, _scaleY );
    _vsi->update( _scaleX, _scaleY );
    _asi->update( _scaleX, _scaleY );
    _hdg->update( _scaleX, _scaleY );

    switch ( _fltMode )
    {
        case FltMode::FD:  _itemFMA->setPlainText( "  FD   " ); break;
        case FltMode::CMD: _itemFMA->setPlainText( "  CMD  " ); break;
        default:           _itemFMA->setPlainText( "       " ); break;
    }

    switch ( _spdMode )
    {
        case SpdMode::FMC_SPD: _itemSPD->setPlainText( "FMC SPD" ); break;
        default:               _itemSPD->setPlainText( "       " ); break;
    }

    switch ( _lnav )
    {
        case LNAV::HDG:     _itemLNAV->setPlainText( "HDG SEL" ); _itemLNAV_ARM->setPlainText( "       " ); break;
        case LNAV::NAV:     _itemLNAV->setPlainText( "VOR/LOC" ); _itemLNAV_ARM->setPlainText( "       " ); break;
        case LNAV::NAV_ARM: _itemLNAV->setPlainText( "HDG SEL" ); _itemLNAV_ARM->setPlainText( "VOR/LOC" ); break;
        case LNAV::APR:     _itemLNAV->setPlainText( "  APR  " ); _itemLNAV_ARM->setPlainText( "       " ); break;
        case LNAV::APR_ARM: _itemLNAV->setPlainText( "  APR  " ); _itemLNAV_ARM->setPlainText( "  APR  " ); break;
        case LNAV::BC:      _itemLNAV->setPlainText( "  BC   " ); _itemLNAV_ARM->setPlainText( "       " ); break;
        case LNAV::BC_ARM:  _itemLNAV->setPlainText( "  BC   " ); _itemLNAV_ARM->setPlainText( "  BC   " ); break;
        default:            _itemLNAV->setPlainText( "       " ); _itemLNAV_ARM->setPlainText( "       " ); break;
    }

    switch ( _vnav )
    {
        case VNAV::ALT:     _itemVNAV->setPlainText( "  ALT  " ); _itemVNAV_ARM->setPlainText( "       " ); break;
        case VNAV::IAS:     _itemVNAV->setPlainText( "  IAS  " ); _itemVNAV_ARM->setPlainText( "       " ); break;
        case VNAV::VS:      _itemVNAV->setPlainText( "  VS   " ); _itemVNAV_ARM->setPlainText( "       " ); break;
        case VNAV::ALT_SEL: _itemVNAV->setPlainText( "ALT SEL" ); _itemVNAV_ARM->setPlainText( "       " ); break;
        case VNAV::GS:      _itemVNAV->setPlainText( "GS PATH" ); _itemVNAV_ARM->setPlainText( "       " ); break;
        case VNAV::GS_ARM:  _itemVNAV->setPlainText( "GS PATH" ); _itemVNAV_ARM->setPlainText( "GS PATH" ); break;
        default:            _itemVNAV->setPlainText( "       " ); _itemVNAV_ARM->setPlainText( "       " ); break;
    }

    _scene->update();

    centerOn( width() / 2.0 , height() / 2.0 );
}

////////////////////////////////////////////////////////////////////////////////

GraphicsEADI::ADI::ADI( QGraphicsScene *scene ) :
    _scene ( scene ),

    _itemBack   ( Q_NULLPTR ),
    _itemLadd   ( Q_NULLPTR ),
    _itemRoll   ( Q_NULLPTR ),
    _itemSlip   ( Q_NULLPTR ),
    _itemTurn   ( Q_NULLPTR ),
    _itemDotH   ( Q_NULLPTR ),
    _itemDotV   ( Q_NULLPTR ),
    _itemFD     ( Q_NULLPTR ),
    _itemStall  ( Q_NULLPTR ),
    _itemMask   ( Q_NULLPTR ),
    _itemScaleH ( Q_NULLPTR ),
    _itemScaleV ( Q_NULLPTR ),
    _itemFPM    ( Q_NULLPTR ),
    _itemFPMX   ( Q_NULLPTR ),

    _roll     ( 0.0 ),
    _pitch    ( 0.0 ),
    _slipSkid ( 0.0 ),
    _turnRate ( 0.0 ),
    _dotH     ( 0.0 ),
    _dotV     ( 0.0 ),
    _fdRoll   ( 0.0 ),
    _fdPitch  ( 0.0 ),
    _angleOfAttack ( 0.0 ),
    _sideslipAngle ( 0.0 ),

    _fpmValid ( false ),

    _fpmVisible  ( false ),
    _dotVisibleH ( false ),
    _dotVisibleV ( false ),
    _fdVisible   ( false ),

    _stall ( false ),

    _laddDeltaX_new     ( 0.0 ),
    _laddDeltaX_old     ( 0.0 ),
    _laddDeltaY_new     ( 0.0 ),
    _laddDeltaY_old     ( 0.0 ),
    _laddBackDeltaX_new ( 0.0 ),
    _laddBackDeltaX_old ( 0.0 ),
    _laddBackDeltaY_new ( 0.0 ),
    _laddBackDeltaY_old ( 0.0 ),
    _slipDeltaX_new     ( 0.0 ),
    _slipDeltaX_old     ( 0.0 ),
    _slipDeltaY_new     ( 0.0 ),
    _slipDeltaY_old     ( 0.0 ),
    _turnDeltaX_new     ( 0.0 ),
    _turnDeltaX_old     ( 0.0 ),
    _dotHDeltaX_new     ( 0.0 ),
    _dotHDeltaX_old     ( 0.0 ),
    _dotVDeltaY_new     ( 0.0 ),
    _dotVDeltaY_old     ( 0.0 ),
    _fdDeltaX_new       ( 0.0 ),
    _fdDeltaX_old       ( 0.0 ),
    _fdDeltaY_new       ( 0.0 ),
    _fdDeltaY_old       ( 0.0 ),
    _fpmDeltaX_new      ( 0.0 ),
    _fpmDeltaX_old      ( 0.0 ),
    _fpmDeltaY_new      ( 0.0 ),
    _fpmDeltaY_old      ( 0.0 ),
    _fpmxDeltaX_new     ( 0.0 ),
    _fpmxDeltaX_old     ( 0.0 ),
    _fpmxDeltaY_new     ( 0.0 ),
    _fpmxDeltaY_old     ( 0.0 ),

    _scaleX ( 1.0 ),
    _scaleY ( 1.0 ),

    _originalPixPerDeg (   3.0 ),
    _deltaLaddBack_max (  52.5f ),
    _deltaLaddBack_min ( -52.5f ),
    _maxSlipDeflection (  20.0 ),
    _maxTurnDeflection (  55.0 ),
    _maxDotsDeflection (  50.0 ),

    _originalAdiCtr    ( 150.0 ,  125.0 ),
    _originalBackPos   (  45.0 ,  -85.0 ),
    _originalLaddPos   ( 110.0 , -175.0 ),
    _originalRollPos   (  45.0 ,   20.0 ),
    _originalSlipPos   ( 145.5 ,   68.0 ),
    _originalTurnPos   ( 142.5 ,  206.0 ),
    _originalDotHPos   ( 145.0 ,  188.0 ),
    _originalDotVPos   ( 213.0 ,  120.0 ),
    _originalFdPos     ( 107.0 ,  124.5 ),
    _originalStallPos  ( 122.0 ,   91.0 ),
    _originalScaleHPos (   0.0 ,    0.0 ),
    _originalScaleVPos (   0.0 ,    0.0 ),
    _originalFpmPos    ( 135.0 ,  113.0 ),

    _backZ   ( 10 ),
    _laddZ   ( 20 ),
    _rollZ   ( 30 ),
    _slipZ   ( 40 ),
    _fpmZ    ( 40 ),
    _dotsZ   ( 50 ),
    _fdZ     ( 50 ),
    _scalesZ ( 51 ),
    _maskZ   ( 60 ),
    _turnZ   ( 70 ),
    _stallZ  ( 80 )
{
    reset();
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ADI::init( double scaleX, double scaleY )
{
    _scaleX = scaleX;
    _scaleY = scaleY;

    reset();

    _itemBack = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_adi_back.svg" );
    _itemBack->setCacheMode( QGraphicsItem::NoCache );
    _itemBack->setZValue( _backZ );
    _itemBack->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemBack->setTransformOriginPoint( _originalAdiCtr - _originalBackPos );
    _itemBack->moveBy( _scaleX * _originalBackPos.x(), _scaleY * _originalBackPos.y() );
    _scene->addItem( _itemBack );

    _itemLadd = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_adi_ladd.svg" );
    _itemLadd->setCacheMode( QGraphicsItem::NoCache );
    _itemLadd->setZValue( _laddZ );
    _itemLadd->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemLadd->setTransformOriginPoint( _originalAdiCtr - _originalLaddPos );
    _itemLadd->moveBy( _scaleX * _originalLaddPos.x(), _scaleY * _originalLaddPos.y() );
    _scene->addItem( _itemLadd );

    _itemRoll = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_adi_roll.svg" );
    _itemRoll->setCacheMode( QGraphicsItem::NoCache );
    _itemRoll->setZValue( _rollZ );
    _itemRoll->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemRoll->setTransformOriginPoint( _originalAdiCtr - _originalRollPos );
    _itemRoll->moveBy( _scaleX * _originalRollPos.x(), _scaleY * _originalRollPos.y() );
    _scene->addItem( _itemRoll );

    _itemSlip = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_adi_slip.svg" );
    _itemSlip->setCacheMode( QGraphicsItem::NoCache );
    _itemSlip->setZValue( _slipZ );
    _itemSlip->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemSlip->setTransformOriginPoint( _originalAdiCtr - _originalSlipPos );
    _itemSlip->moveBy( _scaleX * _originalSlipPos.x(), _scaleY * _originalSlipPos.y() );
    _scene->addItem( _itemSlip );

    _itemTurn = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_adi_turn.svg" );
    _itemTurn->setCacheMode( QGraphicsItem::NoCache );
    _itemTurn->setZValue( _turnZ );
    _itemTurn->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemTurn->moveBy( _scaleX * _originalTurnPos.x(), _scaleY * _originalTurnPos.y() );
    _scene->addItem( _itemTurn );

    _itemDotH = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_adi_doth.svg" );
    _itemDotH->setCacheMode( QGraphicsItem::NoCache );
    _itemDotH->setZValue( _dotsZ - 1 );
    _itemDotH->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemDotH->moveBy( _scaleX * _originalDotHPos.x(), _scaleY * _originalDotHPos.y() );
    _scene->addItem( _itemDotH );

    _itemDotV = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_adi_dotv.svg" );
    _itemDotV->setCacheMode( QGraphicsItem::NoCache );
    _itemDotV->setZValue( _dotsZ - 1 );
    _itemDotV->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemDotV->moveBy( _scaleX * _originalDotVPos.x(), _scaleY * _originalDotVPos.y() );
    _scene->addItem( _itemDotV );

    _itemFD = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_adi_fd.svg" );
    _itemFD->setCacheMode( QGraphicsItem::NoCache );
    _itemFD->setZValue( _fdZ );
    _itemFD->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemFD->setTransformOriginPoint( _originalAdiCtr - _originalFdPos );
    _itemFD->moveBy( _scaleX * _originalFdPos.x(), _scaleY * _originalFdPos.y() );
    _scene->addItem( _itemFD );

    _itemStall = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_adi_stall.svg" );
    _itemStall->setCacheMode( QGraphicsItem::NoCache );
    _itemStall->setZValue( _stallZ );
    _itemStall->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemStall->moveBy( _scaleX * _originalStallPos.x(), _scaleY * _originalStallPos.y() );
    _scene->addItem( _itemStall );

    _itemScaleH = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_adi_scaleh.svg" );
    _itemScaleH->setCacheMode( QGraphicsItem::NoCache );
    _itemScaleH->setZValue( _scalesZ );
    _itemScaleH->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemScaleH->moveBy( _scaleX * _originalScaleHPos.x(), _scaleY * _originalScaleHPos.y() );
    _scene->addItem( _itemScaleH );

    _itemScaleV = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_adi_scalev.svg" );
    _itemScaleV->setCacheMode( QGraphicsItem::NoCache );
    _itemScaleV->setZValue( _scalesZ );
    _itemScaleV->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemScaleV->moveBy( _scaleX * _originalScaleVPos.x(), _scaleY * _originalScaleVPos.y() );
    _scene->addItem( _itemScaleV );

    _itemMask = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_adi_mask.svg" );
    _itemMask->setCacheMode( QGraphicsItem::NoCache );
    _itemMask->setZValue( _maskZ );
    _itemMask->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _scene->addItem( _itemMask );

    _itemFPM = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_adi_fpm.svg" );
    _itemFPM->setCacheMode( QGraphicsItem::NoCache );
    _itemFPM->setZValue( _fpmZ );
    _itemFPM->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemFPM->moveBy( _scaleX * _originalFpmPos.x(), _scaleY * _originalFpmPos.y() );
    _scene->addItem( _itemFPM );

    _itemFPMX = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_adi_fpmx.svg" );
    _itemFPMX->setCacheMode( QGraphicsItem::NoCache );
    _itemFPMX->setZValue( _fpmZ );
    _itemFPMX->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemFPMX->moveBy( _scaleX * _originalFpmPos.x(), _scaleY * _originalFpmPos.y() );
    _scene->addItem( _itemFPMX );

    update( scaleX, scaleY );
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ADI::update( double scaleX, double scaleY )
{
    _scaleX = scaleX;
    _scaleY = scaleY;

    double delta = _originalPixPerDeg * _pitch;

#   ifndef M_PI
    double roll_rad = 3.14159265358979323846 * _roll / 180.0;
#   else
    double roll_rad = M_PI * _roll / 180.0;
#   endif

    double sinRoll = sin( roll_rad );
    double cosRoll = cos( roll_rad );

    updateLadd( delta, sinRoll, cosRoll );
    updateLaddBack( delta, sinRoll, cosRoll );
    updateRoll();
    updateSlipSkid( sinRoll, cosRoll );
    updateTurnRate();
    updateDots();
    updateFD( sinRoll, cosRoll );
    updateStall();
    updateFPM();

    _laddDeltaX_old     = _laddDeltaX_new;
    _laddDeltaY_old     = _laddDeltaY_new;
    _laddBackDeltaX_old = _laddBackDeltaX_new;
    _laddBackDeltaY_old = _laddBackDeltaY_new;
    _slipDeltaX_old     = _slipDeltaX_new;
    _slipDeltaY_old     = _slipDeltaY_new;
    _turnDeltaX_old     = _turnDeltaX_new;
    _dotHDeltaX_old     = _dotHDeltaX_new;
    _dotVDeltaY_old     = _dotVDeltaY_new;
    _fdDeltaX_old       = _fdDeltaX_new;
    _fdDeltaY_old       = _fdDeltaY_new;

    _fpmDeltaX_old     = _fpmDeltaX_new;
    _fpmDeltaY_old     = _fpmDeltaY_new;
    _fpmxDeltaX_old     = _fpmxDeltaX_new;
    _fpmxDeltaY_old     = _fpmxDeltaY_new;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ADI::setRoll( double roll )
{
    _roll = roll;

    if      ( _roll < -180.0 ) _roll = -180.0;
    else if ( _roll >  180.0 ) _roll =  180.0;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ADI::setPitch( double pitch )
{
    _pitch = pitch;

    if      ( _pitch < -90.0 ) _pitch = -90.0;
    else if ( _pitch >  90.0 ) _pitch =  90.0;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ADI::setFPM( double aoa, double sideslip, bool visible )
{
    _angleOfAttack = aoa;
    _sideslipAngle = sideslip;

    _fpmValid = true;

    if ( _angleOfAttack < -15.0 )
    {
        _angleOfAttack = -15.0;
        _fpmValid = false;
    }
    else if ( _angleOfAttack > 15.0 )
    {
        _angleOfAttack = 15.0;
        _fpmValid = false;
    }

    if ( _sideslipAngle < -10.0 )
    {
        _sideslipAngle = -10.0;
        _fpmValid = false;
    }
    else if ( _sideslipAngle > 10.0 )
    {
        _sideslipAngle = 10.0;
        _fpmValid = false;
    }

    _fpmVisible = visible;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ADI::setSlipSkid( double slipSkid )
{
    _slipSkid = slipSkid;

    if      ( _slipSkid < -1.0 ) _slipSkid = -1.0;
    else if ( _slipSkid >  1.0 ) _slipSkid =  1.0;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ADI::setTurnRate( double turnRate )
{
    _turnRate = turnRate;

    if      ( _turnRate < -1.0 ) _turnRate = -1.0;
    else if ( _turnRate >  1.0 ) _turnRate =  1.0;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ADI::setDots( double dotH, double dotV, bool visibleH, bool visibleV )
{
    _dotH = dotH;
    _dotV = dotV;

    if      ( _dotH < -1.0 ) _dotH = -1.0;
    else if ( _dotH >  1.0 ) _dotH =  1.0;

    if      ( _dotV < -1.0 ) _dotV = -1.0;
    else if ( _dotV >  1.0 ) _dotV =  1.0;

    _dotVisibleH = visibleH;
    _dotVisibleV = visibleV;
}


////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ADI::setFD( double roll, double pitch, bool visible )
{
    _fdRoll  = roll;
    _fdPitch = pitch;

    if      ( _fdRoll < -180.0 ) _fdRoll = -180.0;
    else if ( _fdRoll >  180.0 ) _fdRoll =  180.0;

    if      ( _fdPitch < -90.0 ) _fdPitch = -90.0;
    else if ( _fdPitch >  90.0 ) _fdPitch =  90.0;

    _fdVisible = visible;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ADI::setStall( bool stall )
{
    _stall = stall;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ADI::reset()
{
    _itemBack   = Q_NULLPTR;
    _itemLadd   = Q_NULLPTR;
    _itemRoll   = Q_NULLPTR;
    _itemSlip   = Q_NULLPTR;
    _itemTurn   = Q_NULLPTR;
    _itemDotH   = Q_NULLPTR;
    _itemDotV   = Q_NULLPTR;
    _itemFD     = Q_NULLPTR;
    _itemMask   = Q_NULLPTR;
    _itemScaleH = Q_NULLPTR;
    _itemScaleV = Q_NULLPTR;
    _itemFPM    = Q_NULLPTR;
    _itemFPMX   = Q_NULLPTR;

    _roll     = 0.0;
    _pitch    = 0.0;
    _slipSkid = 0.0;
    _turnRate = 0.0;
    _dotH     = 0.0;
    _dotV     = 0.0;
    _fdRoll   = 0.0;
    _fdPitch  = 0.0;
    _angleOfAttack = 0.0;
    _sideslipAngle = 0.0;

    _fpmValid = false;

    _fpmVisible   = false;
    _dotVisibleH  = false;
    _dotVisibleV  = false;
    _fdVisible    = false;

    _stall = false;

    _laddDeltaX_new     = 0.0;
    _laddDeltaX_old     = 0.0;
    _laddDeltaY_new     = 0.0;
    _laddDeltaY_old     = 0.0;
    _laddBackDeltaX_new = 0.0;
    _laddBackDeltaX_old = 0.0;
    _laddBackDeltaY_new = 0.0;
    _laddBackDeltaY_old = 0.0;
    _slipDeltaX_new     = 0.0;
    _slipDeltaX_old     = 0.0;
    _slipDeltaY_new     = 0.0;
    _slipDeltaY_old     = 0.0;
    _turnDeltaX_new     = 0.0;
    _turnDeltaX_old     = 0.0;
    _dotHDeltaX_new     = 0.0;
    _dotHDeltaX_old     = 0.0;
    _dotVDeltaY_new     = 0.0;
    _dotVDeltaY_old     = 0.0;
    _fdDeltaX_new       = 0.0;
    _fdDeltaX_old       = 0.0;
    _fdDeltaY_new       = 0.0;
    _fdDeltaY_old       = 0.0;
    _fpmDeltaX_new      = 0.0;
    _fpmDeltaX_old      = 0.0;
    _fpmDeltaY_new      = 0.0;
    _fpmDeltaY_old      = 0.0;
    _fpmxDeltaX_new     = 0.0;
    _fpmxDeltaX_old     = 0.0;
    _fpmxDeltaY_new     = 0.0;
    _fpmxDeltaY_old     = 0.0;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ADI::updateLadd( double delta, double sinRoll, double cosRoll )
{
    _itemLadd->setRotation( -_roll );

    _laddDeltaX_new = _scaleX * delta * sinRoll;
    _laddDeltaY_new = _scaleY * delta * cosRoll;

    _itemLadd->moveBy( _laddDeltaX_new - _laddDeltaX_old, _laddDeltaY_new - _laddDeltaY_old );
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ADI::updateLaddBack( double delta, double sinRoll, double cosRoll )
{
    _itemBack->setRotation( -_roll );

    double deltaLaddBack = 0.0;

    if ( delta > _deltaLaddBack_max )
    {
        deltaLaddBack = _deltaLaddBack_max;
    }
    else if ( delta < _deltaLaddBack_min )
    {
        deltaLaddBack = _deltaLaddBack_min;
    }
    else
    {
        deltaLaddBack = delta;
    }

    _laddBackDeltaX_new = _scaleX * deltaLaddBack * sinRoll;
    _laddBackDeltaY_new = _scaleY * deltaLaddBack * cosRoll;

    _itemBack->moveBy( _laddBackDeltaX_new - _laddBackDeltaX_old, _laddBackDeltaY_new - _laddBackDeltaY_old );
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ADI::updateRoll()
{
    _itemRoll->setRotation( -_roll );
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ADI::updateSlipSkid( double sinRoll, double cosRoll )
{
    _itemSlip->setRotation( -_roll );

    double deltaSlip = -_maxSlipDeflection * _slipSkid;

    _slipDeltaX_new =  _scaleX * deltaSlip * cosRoll;
    _slipDeltaY_new = -_scaleY * deltaSlip * sinRoll;

    _itemSlip->moveBy( _slipDeltaX_new - _slipDeltaX_old, _slipDeltaY_new - _slipDeltaY_old );
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ADI::updateTurnRate()
{
    _turnDeltaX_new = _scaleX * _maxTurnDeflection * _turnRate;

    _itemTurn->moveBy( _turnDeltaX_new - _turnDeltaX_old, 0.0 );
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ADI::updateDots()
{
    if ( _dotVisibleH )
    {
        _itemDotH->setVisible( true );
        _itemScaleH->setVisible( true );

        _dotHDeltaX_new = _scaleX * _maxDotsDeflection * _dotH;

        _itemDotH->moveBy( _dotHDeltaX_new - _dotHDeltaX_old, 0.0 );
    }
    else
    {
        _itemDotH->setVisible( false );
        _itemScaleH->setVisible( false );

        _dotHDeltaX_new = _dotHDeltaX_old;
    }

    if ( _dotVisibleV )
    {
        _itemDotV->setVisible( true );
        _itemScaleV->setVisible( true );

        _dotVDeltaY_new = _scaleY * _maxDotsDeflection * _dotV;

        _itemDotV->moveBy( 0.0, _dotVDeltaY_old - _dotVDeltaY_new );
    }
    else
    {
        _itemDotV->setVisible( false );
        _itemScaleV->setVisible( false );

        _dotVDeltaY_new = _dotVDeltaY_old;
    }
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ADI::updateFD( double sinRoll, double cosRoll )
{
    if ( _fdVisible )
    {
        _itemFD->setVisible( true );

        _itemFD->setRotation( _fdRoll - _roll );

        double pitch = _pitch - _fdPitch;

        if      ( pitch < -17.0 ) pitch = -17.0;
        else if ( pitch >  17.0 ) pitch =  17.0;

        double delta = _originalPixPerDeg * pitch;

        _fdDeltaX_new = _scaleX * delta * sinRoll;
        _fdDeltaY_new = _scaleY * delta * cosRoll;

        _itemFD->moveBy( _fdDeltaX_new - _fdDeltaX_old,
                         _fdDeltaY_new - _fdDeltaY_old );
    }
    else
    {
        _itemFD->setVisible( false );
    }
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ADI::updateStall()
{
    if ( _stall )
    {
        _itemStall->setVisible( true );
    }
    else
    {
        _itemStall->setVisible( false );
    }
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ADI::updateFPM()
{
    if ( _fpmVisible )
    {
        _itemFPM->setVisible( true );

        _fpmDeltaX_new = _scaleX * _originalPixPerDeg * _sideslipAngle;
        _fpmDeltaY_new = _scaleY * _originalPixPerDeg * _angleOfAttack;

        _itemFPM->moveBy( _fpmDeltaX_new - _fpmDeltaX_old, _fpmDeltaY_old - _fpmDeltaY_new );

        if ( !_fpmValid )
        {
            _itemFPMX->setVisible( true );

            _fpmxDeltaX_new = _fpmDeltaX_new;
            _fpmxDeltaY_new = _fpmDeltaY_new;

            _itemFPMX->moveBy( _fpmxDeltaX_new - _fpmxDeltaX_old, _fpmxDeltaY_old - _fpmxDeltaY_new );
        }
        else
        {
            _itemFPMX->setVisible( false );
            _fpmxDeltaX_new = _fpmxDeltaX_old;
            _fpmxDeltaY_new = _fpmxDeltaY_old;
        }
    }
    else
    {
        _itemFPM->setVisible( false );
        _fpmDeltaX_new = _fpmDeltaX_old;
        _fpmDeltaY_new = _fpmDeltaY_old;

        _itemFPMX->setVisible( false );
        _fpmxDeltaX_new = _fpmxDeltaX_old;
        _fpmxDeltaY_new = _fpmxDeltaY_old;
    }
}

////////////////////////////////////////////////////////////////////////////////

GraphicsEADI::ALT::ALT( QGraphicsScene *scene ) :
    _scene ( scene ),

    _itemBack     ( Q_NULLPTR ),
    _itemScale1   ( Q_NULLPTR ),
    _itemScale2   ( Q_NULLPTR ),
    _itemLabel1   ( Q_NULLPTR ),
    _itemLabel2   ( Q_NULLPTR ),
    _itemLabel3   ( Q_NULLPTR ),
    _itemGround   ( Q_NULLPTR ),
    _itemBugAlt   ( Q_NULLPTR ),
    _itemFrame    ( Q_NULLPTR ),
    _itemAltitude ( Q_NULLPTR ),
    _itemPressure ( Q_NULLPTR ),
    _itemSetpoint ( Q_NULLPTR ),

    _altitude ( 0.0 ),
    _pressure ( 0.0 ),
    _altitude_sel ( 0.0 ),

    _pressMode ( GraphicsEADI::PressureMode::STD ),

    _scale1DeltaY_new ( 0.0 ),
    _scale1DeltaY_old ( 0.0 ),
    _scale2DeltaY_new ( 0.0 ),
    _scale2DeltaY_old ( 0.0 ),
    _groundDeltaY_new ( 0.0 ),
    _groundDeltaY_old ( 0.0 ),
    _labelsDeltaY_new ( 0.0 ),
    _labelsDeltaY_old ( 0.0 ),
    _bugDeltaY_new    ( 0.0 ),
    _bugDeltaY_old    ( 0.0 ),

    _scaleX ( 1.0 ),
    _scaleY ( 1.0 ),

    _originalPixPerAlt   ( 0.150 ),
    _originalScaleHeight ( 300.0 ),
    _originalLabelsX     ( 253.0 ),
    _originalLabel1Y     (  50.0 ),
    _originalLabel2Y     ( 125.0 ),
    _originalLabel3Y     ( 200.0 ),

    _originalBackPos     ( 231.0 ,   37.5 ),
    _originalScale1Pos   ( 231.0 , -174.5 ),
    _originalScale2Pos   ( 231.0 , -474.5 ),
    _originalGroundPos   ( 231.5 ,  124.5 ),
    _originalFramePos    ( 225.0 ,  110.0 ),
    _originalAltitudeCtr ( 254.0 ,  126.0 ),
    _originalPressureCtr ( 254.0 ,  225.0 ),
    _originalAltitudeSet ( 250.0 ,   27.0 ),

    _backZ      (  70 ),
    _scaleZ     (  77 ),
    _labelsZ    (  78 ),
    _groundZ    (  79 ),
    _altBugZ    ( 100 ),
    _frameZ     ( 110 ),
    _frameTextZ ( 120 )
{
    reset();
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ALT::init( double scaleX, double scaleY )
{
    _scaleX = scaleX;
    _scaleY = scaleY;

    reset();

    _itemBack = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_alt_back.svg" );
    _itemBack->setCacheMode( QGraphicsItem::NoCache );
    _itemBack->setZValue( _backZ );
    _itemBack->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemBack->moveBy( _scaleX * _originalBackPos.x(), _scaleY * _originalBackPos.y() );
    _scene->addItem( _itemBack );

    _itemScale1 = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_alt_scale.svg" );
    _itemScale1->setCacheMode( QGraphicsItem::NoCache );
    _itemScale1->setZValue( _scaleZ );
    _itemScale1->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemScale1->moveBy( _scaleX * _originalScale1Pos.x(), _scaleY * _originalScale1Pos.y() );
    _scene->addItem( _itemScale1 );

    _itemScale2 = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_alt_scale.svg" );
    _itemScale2->setCacheMode( QGraphicsItem::NoCache );
    _itemScale2->setZValue( _scaleZ );
    _itemScale2->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemScale2->moveBy( _scaleX * _originalScale2Pos.x(), _scaleY * _originalScale2Pos.y() );
    _scene->addItem( _itemScale2 );

    _itemLabel1 = new QGraphicsTextItem( QString( "99999" ) );
    _itemLabel1->setCacheMode( QGraphicsItem::NoCache );
    _itemLabel1->setZValue( _labelsZ );
    _itemLabel1->setDefaultTextColor( Colors::_white );
    _itemLabel1->setFont( Fonts::small() );
    _itemLabel1->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemLabel1->moveBy( _scaleX * ( _originalLabelsX - _itemLabel1->boundingRect().width()  / 2.0 ),
                         _scaleY * ( _originalLabel1Y - _itemLabel1->boundingRect().height() / 2.0 ) );
    _scene->addItem( _itemLabel1 );

    _itemLabel2 = new QGraphicsTextItem( QString( "99999" ) );
    _itemLabel2->setCacheMode( QGraphicsItem::NoCache );
    _itemLabel2->setZValue( _labelsZ );
    _itemLabel2->setDefaultTextColor( Colors::_white );
    _itemLabel2->setFont( Fonts::small() );
    _itemLabel2->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemLabel2->moveBy( _scaleX * ( _originalLabelsX - _itemLabel2->boundingRect().width()  / 2.0 ),
                         _scaleY * ( _originalLabel2Y - _itemLabel2->boundingRect().height() / 2.0 ) );
    _scene->addItem( _itemLabel2 );

    _itemLabel3 = new QGraphicsTextItem( QString( "99999" ) );
    _itemLabel3->setCacheMode( QGraphicsItem::NoCache );
    _itemLabel3->setZValue( _labelsZ );
    _itemLabel3->setDefaultTextColor( Colors::_white );
    _itemLabel3->setFont( Fonts::small() );
    _itemLabel3->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemLabel3->moveBy( _scaleX * ( _originalLabelsX - _itemLabel3->boundingRect().width()  / 2.0 ),
                         _scaleY * ( _originalLabel3Y - _itemLabel3->boundingRect().height() / 2.0 ) );
    _scene->addItem( _itemLabel3 );

    _itemGround = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_alt_ground.svg" );
    _itemGround->setCacheMode( QGraphicsItem::NoCache );
    _itemGround->setZValue( _groundZ );
    _itemGround->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemGround->moveBy( _scaleX * _originalGroundPos.x(), _scaleY * _originalGroundPos.y() );
    _scene->addItem( _itemGround );

    _itemBugAlt = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_alt_bug.svg" );
    _itemBugAlt->setCacheMode( QGraphicsItem::NoCache );
    _itemBugAlt->setZValue( _altBugZ );
    _itemBugAlt->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemBugAlt->moveBy( _scaleX * _originalFramePos.x(), _scaleY * _originalFramePos.y() );
    _scene->addItem( _itemBugAlt );

    _itemFrame = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_alt_frame.svg" );
    _itemFrame->setCacheMode( QGraphicsItem::NoCache );
    _itemFrame->setZValue( _frameZ );
    _itemFrame->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemFrame->moveBy( _scaleX * _originalFramePos.x(), _scaleY * _originalFramePos.y() );
    _scene->addItem( _itemFrame );

    _itemAltitude = new QGraphicsTextItem( QString( "    0" ) );
    _itemAltitude->setCacheMode( QGraphicsItem::NoCache );
    _itemAltitude->setZValue( _frameTextZ );
    _itemAltitude->setDefaultTextColor( Colors::_white );
    _itemAltitude->setFont( Fonts::medium() );
    _itemAltitude->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemAltitude->moveBy( _scaleX * ( _originalAltitudeCtr.x() - _itemAltitude->boundingRect().width()  / 2.0 ),
                           _scaleY * ( _originalAltitudeCtr.y() - _itemAltitude->boundingRect().height() / 2.0 ) );
    _scene->addItem( _itemAltitude );

    _itemPressure = new QGraphicsTextItem( QString( "  STD  " ) );
    _itemPressure->setCacheMode( QGraphicsItem::NoCache );
    _itemPressure->setZValue( _frameTextZ );
    _itemPressure->setDefaultTextColor( Colors::_lime );
    _itemPressure->setFont( Fonts::medium() );
    _itemPressure->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemPressure->moveBy( _scaleX * ( _originalPressureCtr.x() - _itemPressure->boundingRect().width()  / 2.0 ),
                           _scaleY * ( _originalPressureCtr.y() - _itemPressure->boundingRect().height() / 2.0 ) );
    _scene->addItem( _itemPressure );

    _itemSetpoint = new QGraphicsTextItem( QString( "    0" ) );
    _itemSetpoint->setCacheMode( QGraphicsItem::NoCache );
    _itemSetpoint->setZValue( _frameTextZ );
    _itemSetpoint->setDefaultTextColor( Colors::_magenta );
    _itemSetpoint->setFont( Fonts::medium() );
    _itemSetpoint->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemSetpoint->moveBy( _scaleX * ( _originalAltitudeSet.x() - _itemAltitude->boundingRect().width()  / 2.0 ),
                           _scaleY * ( _originalAltitudeSet.y() - _itemAltitude->boundingRect().height() / 2.0 ) );
    _scene->addItem( _itemSetpoint );

    update( scaleX, scaleY );
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ALT::update( double scaleX, double scaleY )
{
    _scaleX = scaleX;
    _scaleY = scaleY;

    updateAltitude();
    updatePressure();

    _scale1DeltaY_old = _scale1DeltaY_new;
    _scale2DeltaY_old = _scale2DeltaY_new;
    _groundDeltaY_old = _groundDeltaY_new;
    _labelsDeltaY_old = _labelsDeltaY_new;
    _bugDeltaY_old    = _bugDeltaY_new;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ALT::setAltitude( double altitude )
{
    _altitude = altitude;

    if      ( _altitude <     0.0 ) _altitude =     0.0;
    else if ( _altitude > 99999.0 ) _altitude = 99999.0;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ALT::setPressure( double pressure, GraphicsEADI::PressureMode pressMode )
{
    _pressure = pressure;

    if      ( _pressure <    0.0 ) _pressure =    0.0;
    else if ( _pressure > 2000.0 ) _pressure = 2000.0;

    _pressMode = pressMode;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ALT::setAltitudeSel( double altitude )
{
    _altitude_sel = altitude;

    if      ( _altitude_sel <     0.0 ) _altitude_sel =     0.0;
    else if ( _altitude_sel > 99999.0 ) _altitude_sel = 99999.0;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ALT::reset()
{
    _itemBack     = Q_NULLPTR;
    _itemScale1   = Q_NULLPTR;
    _itemScale2   = Q_NULLPTR;
    _itemLabel1   = Q_NULLPTR;
    _itemLabel2   = Q_NULLPTR;
    _itemLabel3   = Q_NULLPTR;
    _itemGround   = Q_NULLPTR;
    _itemBugAlt   = Q_NULLPTR;
    _itemFrame    = Q_NULLPTR;
    _itemAltitude = Q_NULLPTR;
    _itemPressure = Q_NULLPTR;
    _itemSetpoint = Q_NULLPTR;

    _altitude = 0.0;
    _pressure = 0.0;
    _altitude_sel = 0.0;

    _pressMode = GraphicsEADI::PressureMode::STD;

    _scale1DeltaY_new = 0.0;
    _scale1DeltaY_old = 0.0;
    _scale2DeltaY_new = 0.0;
    _scale2DeltaY_old = 0.0;
    _groundDeltaY_new = 0.0;
    _groundDeltaY_old = 0.0;
    _labelsDeltaY_new = 0.0;
    _labelsDeltaY_old = 0.0;
    _bugDeltaY_new    = 0.0;
    _bugDeltaY_old    = 0.0;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ALT::updateAltitude()
{
    _itemAltitude->setPlainText( QString("%1").arg(_altitude     , 5, 'f', 0, QChar(' ')) );
    _itemSetpoint->setPlainText( QString("%1").arg(_altitude_sel , 5, 'f', 0, QChar(' ')) );

    updateScale();
    updateScaleLabels();
    updateAltitudeBug();
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ALT::updatePressure()
{
    if ( _pressMode == GraphicsEADI::PressureMode::STD )
    {
        _itemPressure->setPlainText( QString( "  STD  " ) );
    }
    else if ( _pressMode == GraphicsEADI::PressureMode::MB )
    {
        _itemPressure->setPlainText( QString::number( _pressure, 'f', 0 ) + QString( " MB" ) );
    }
    else if ( _pressMode == GraphicsEADI::PressureMode::IN )
    {
        _itemPressure->setPlainText( QString::number( _pressure, 'f', 2 ) + QString( " IN" ) );
    }
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ALT::updateAltitudeBug()
{
    _bugDeltaY_new = _scaleY * _originalPixPerAlt * ( _altitude - _altitude_sel );

    if      ( _bugDeltaY_new < -_scaleY * 85.0 ) _bugDeltaY_new = -_scaleY * 85.0;
    else if ( _bugDeltaY_new >  _scaleY * 85.0 ) _bugDeltaY_new =  _scaleY * 85.0;

    _itemBugAlt->moveBy( 0.0, _bugDeltaY_new - _bugDeltaY_old );
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ALT::updateScale()
{
    _scale1DeltaY_new = _scaleY * _originalPixPerAlt * _altitude;
    _scale2DeltaY_new = _scale1DeltaY_new;
    _groundDeltaY_new = _scale1DeltaY_new;

    double scaleSingleHeight = _scaleY * _originalScaleHeight;
    double scaleDoubleHeight = _scaleY * _originalScaleHeight * 2.0;

    while ( _scale1DeltaY_new > scaleSingleHeight + _scaleY * 74.5 )
    {
        _scale1DeltaY_new = _scale1DeltaY_new - scaleDoubleHeight;
    }

    while ( _scale2DeltaY_new > scaleDoubleHeight + _scaleY * 74.5 )
    {
        _scale2DeltaY_new = _scale2DeltaY_new - scaleDoubleHeight;
    }

    if ( _groundDeltaY_new > _scaleY * 100.0 ) _groundDeltaY_new = _scaleY * 100.0;

    _itemScale1->moveBy( 0.0, _scale1DeltaY_new - _scale1DeltaY_old );
    _itemScale2->moveBy( 0.0, _scale2DeltaY_new - _scale2DeltaY_old );
    _itemGround->moveBy( 0.0, _groundDeltaY_new - _groundDeltaY_old );
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ALT::updateScaleLabels()
{
    int tmp = floor( _altitude + 0.5 );
    int alt = tmp - ( tmp % 500 );

    double alt1 = static_cast< double >( alt ) + 500.0;
    double alt2 = static_cast< double >( alt );
    double alt3 = static_cast< double >( alt ) - 500.0;

    _labelsDeltaY_new = _scaleY * _originalPixPerAlt * _altitude;

    while ( _labelsDeltaY_new > _scaleY * 37.5 )
    {
        _labelsDeltaY_new = _labelsDeltaY_new - _scaleY * 75.0;
    }

    if ( _labelsDeltaY_new < 0.0 && _altitude > alt2 )
    {
        alt1 += 500.0;
        alt2 += 500.0;
        alt3 += 500.0;
    }

    _itemLabel1->moveBy( 0.0, _labelsDeltaY_new - _labelsDeltaY_old );
    _itemLabel2->moveBy( 0.0, _labelsDeltaY_new - _labelsDeltaY_old );
    _itemLabel3->moveBy( 0.0, _labelsDeltaY_new - _labelsDeltaY_old );

    if ( alt1 > 0.0 && alt1 <= 100000.0 )
    {
        _itemLabel1->setVisible( true );
        _itemLabel1->setPlainText( QString("%1").arg(alt1, 5, 'f', 0, QChar(' ')) );
    }
    else
    {
        _itemLabel1->setVisible( false );
    }

    if ( alt2 > 0.0 && alt2 <= 100000.0 )
    {
        _itemLabel2->setVisible( true );
        _itemLabel2->setPlainText( QString("%1").arg(alt2, 5, 'f', 0, QChar(' ')) );
    }
    else
    {
        _itemLabel2->setVisible( false );
    }

    if ( alt3 > 0.0 && alt3 <= 100000.0 )
    {
        _itemLabel3->setVisible( true );
        _itemLabel3->setPlainText( QString("%1").arg(alt3, 5, 'f', 0, QChar(' ')) );
    }
    else
    {
        _itemLabel3->setVisible( false );
    }
}

////////////////////////////////////////////////////////////////////////////////

GraphicsEADI::ASI::ASI( QGraphicsScene *scene ) :
    _scene ( scene ),

    _itemBack     ( Q_NULLPTR ),
    _itemScale1   ( Q_NULLPTR ),
    _itemScale2   ( Q_NULLPTR ),
    _itemLabel1   ( Q_NULLPTR ),
    _itemLabel2   ( Q_NULLPTR ),
    _itemLabel3   ( Q_NULLPTR ),
    _itemLabel4   ( Q_NULLPTR ),
    _itemLabel5   ( Q_NULLPTR ),
    _itemLabel6   ( Q_NULLPTR ),
    _itemLabel7   ( Q_NULLPTR ),
    _itemBugIAS   ( Q_NULLPTR ),
    _itemFrame    ( Q_NULLPTR ),
    _itemVfe      ( Q_NULLPTR ),
    _itemVne      ( Q_NULLPTR ),
    _itemAirspeed ( Q_NULLPTR ),
    _itemMachNo   ( Q_NULLPTR ),
    _itemSetpoint ( Q_NULLPTR ),

    _vfeBrush ( QColor( 0xff, 0xff, 0xff ), Qt::SolidPattern ),
    _vfePen( _vfeBrush, 1 ),

    _airspeed ( 0.0 ),
    _machNo   ( 0.0 ),

    _airspeed_sel ( 0.0 ),

    _vfe ( 0.0 ),
    _vne ( 0.0 ),

    _scale1DeltaY_new ( 0.0 ),
    _scale1DeltaY_old ( 0.0 ),
    _scale2DeltaY_new ( 0.0 ),
    _scale2DeltaY_old ( 0.0 ),
    _labelsDeltaY_new ( 0.0 ),
    _labelsDeltaY_old ( 0.0 ),
    _bugDeltaY_new    ( 0.0 ),
    _bugDeltaY_old    ( 0.0 ),
    _vneDeltaY_new    ( 0.0 ),
    _vneDeltaY_old    ( 0.0 ),

    _scaleX ( 1.0 ),
    _scaleY ( 1.0 ),

    _originalPixPerSpd   (   1.5 ),
    _originalScaleHeight ( 300.0 ),
    _originalLabelsX     (  40.0 ),
    _originalLabel1Y     (  35.0 ),
    _originalLabel2Y     (  65.0 ),
    _originalLabel3Y     (  95.0 ),
    _originalLabel4Y     ( 125.0 ),
    _originalLabel5Y     ( 155.0 ),
    _originalLabel6Y     ( 185.0 ),
    _originalLabel7Y     ( 215.0 ),
    _originalVfeWidth    (   1.0 ),

    _originalBackPos     ( 25.0 ,   37.5 ),
    _originalScale1Pos   ( 56.0 , -174.5 ),
    _originalScale2Pos   ( 56.0 , -474.5 ),
    _originalFramePos    (  0.0 ,  110.0 ),
    _originalAirspeedCtr ( 40.0 ,  126.0 ),
    _originalMachNoCtr   ( 43.0 ,  225.0 ),
    _originalAirspeedSet ( 47.0 ,   27.0 ),
    _originalVfePos      ( 59.5 ,  124.5 ),

    _backZ      (  70 ),
    _scaleZ     (  80 ),
    _labelsZ    (  90 ),
    _iasBugZ    ( 110 ),
    _iasVfeZ    (  90 ),
    _iasVneZ    (  90 ),
    _frameZ     ( 110 ),
    _frameTextZ ( 120 )
{
    reset();
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ASI::init( double scaleX, double scaleY )
{
    _scaleX = scaleX;
    _scaleY = scaleY;

    reset();

    _itemBack = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_asi_back.svg" );
    _itemBack->setCacheMode( QGraphicsItem::NoCache );
    _itemBack->setZValue( _backZ );
    _itemBack->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemBack->moveBy( _scaleX * _originalBackPos.x(), _scaleY * _originalBackPos.y() );
    _scene->addItem( _itemBack );

    _itemScale1 = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_asi_scale.svg" );
    _itemScale1->setCacheMode( QGraphicsItem::NoCache );
    _itemScale1->setZValue( _scaleZ );
    _itemScale1->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemScale1->moveBy( _scaleX * _originalScale1Pos.x(), _scaleY * _originalScale1Pos.y() );
    _scene->addItem( _itemScale1 );

    _itemScale2 = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_asi_scale.svg" );
    _itemScale2->setCacheMode( QGraphicsItem::NoCache );
    _itemScale2->setZValue( _scaleZ );
    _itemScale2->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemScale2->moveBy( _scaleX * _originalScale2Pos.x(), _scaleY * _originalScale2Pos.y() );
    _scene->addItem( _itemScale2 );

    _itemLabel1 = new QGraphicsTextItem( QString( "999" ) );
    _itemLabel1->setCacheMode( QGraphicsItem::NoCache );
    _itemLabel1->setZValue( _labelsZ );
    _itemLabel1->setDefaultTextColor( Colors::_white );
    _itemLabel1->setFont( Fonts::small() );
    _itemLabel1->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemLabel1->moveBy( _scaleX * ( _originalLabelsX - _itemLabel1->boundingRect().width()  / 2.0 ),
                         _scaleY * ( _originalLabel1Y - _itemLabel1->boundingRect().height() / 2.0 ) );
    _scene->addItem( _itemLabel1 );

    _itemLabel2 = new QGraphicsTextItem( QString( "999" ) );
    _itemLabel2->setCacheMode( QGraphicsItem::NoCache );
    _itemLabel2->setZValue( _labelsZ );
    _itemLabel2->setDefaultTextColor( Colors::_white );
    _itemLabel2->setFont( Fonts::small() );
    _itemLabel2->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemLabel2->moveBy( _scaleX * ( _originalLabelsX - _itemLabel2->boundingRect().width()  / 2.0 ),
                         _scaleY * ( _originalLabel2Y - _itemLabel2->boundingRect().height() / 2.0 ) );
    _scene->addItem( _itemLabel2 );

    _itemLabel3 = new QGraphicsTextItem( QString( "999" ) );
    _itemLabel3->setCacheMode( QGraphicsItem::NoCache );
    _itemLabel3->setZValue( _labelsZ );
    _itemLabel3->setDefaultTextColor( Colors::_white );
    _itemLabel3->setFont( Fonts::small() );
    _itemLabel3->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemLabel3->moveBy( _scaleX * ( _originalLabelsX - _itemLabel3->boundingRect().width()  / 2.0 ),
                         _scaleY * ( _originalLabel3Y - _itemLabel3->boundingRect().height() / 2.0 ) );
    _scene->addItem( _itemLabel3 );

    _itemLabel4 = new QGraphicsTextItem( QString( "999" ) );
    _itemLabel4->setCacheMode( QGraphicsItem::NoCache );
    _itemLabel4->setZValue( _labelsZ );
    _itemLabel4->setDefaultTextColor( Colors::_white );
    _itemLabel4->setFont( Fonts::small() );
    _itemLabel4->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemLabel4->moveBy( _scaleX * ( _originalLabelsX - _itemLabel4->boundingRect().width()  / 2.0 ),
                         _scaleY * ( _originalLabel4Y - _itemLabel4->boundingRect().height() / 2.0 ) );
    _scene->addItem( _itemLabel4 );

    _itemLabel5 = new QGraphicsTextItem( QString( "999" ) );
    _itemLabel5->setCacheMode( QGraphicsItem::NoCache );
    _itemLabel5->setZValue( _labelsZ );
    _itemLabel5->setDefaultTextColor( Colors::_white );
    _itemLabel5->setFont( Fonts::small() );
    _itemLabel5->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemLabel5->moveBy( _scaleX * ( _originalLabelsX - _itemLabel5->boundingRect().width()  / 2.0 ),
                         _scaleY * ( _originalLabel5Y - _itemLabel5->boundingRect().height() / 2.0 ) );
    _scene->addItem( _itemLabel5 );

    _itemLabel6 = new QGraphicsTextItem( QString( "999" ) );
    _itemLabel6->setCacheMode( QGraphicsItem::NoCache );
    _itemLabel6->setZValue( _labelsZ );
    _itemLabel6->setDefaultTextColor( Colors::_white );
    _itemLabel6->setFont( Fonts::small() );
    _itemLabel6->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemLabel6->moveBy( _scaleX * ( _originalLabelsX - _itemLabel6->boundingRect().width()  / 2.0 ),
                         _scaleY * ( _originalLabel6Y - _itemLabel6->boundingRect().height() / 2.0 ) );
    _scene->addItem( _itemLabel6 );

    _itemLabel7 = new QGraphicsTextItem( QString( "999" ) );
    _itemLabel7->setCacheMode( QGraphicsItem::NoCache );
    _itemLabel7->setZValue( _labelsZ );
    _itemLabel7->setDefaultTextColor( Colors::_white );
    _itemLabel7->setFont( Fonts::small() );
    _itemLabel7->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemLabel7->moveBy( _scaleX * ( _originalLabelsX - _itemLabel7->boundingRect().width()  / 2.0 ),
                         _scaleY * ( _originalLabel7Y - _itemLabel7->boundingRect().height() / 2.0 ) );
    _scene->addItem( _itemLabel7 );

    _itemBugIAS = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_asi_bug.svg" );
    _itemBugIAS->setCacheMode( QGraphicsItem::NoCache );
    _itemBugIAS->setZValue( _iasBugZ );
    _itemBugIAS->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemBugIAS->moveBy( _scaleX * _originalFramePos.x(), _scaleY * _originalFramePos.y() );
    _scene->addItem( _itemBugIAS );

    _itemFrame = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_asi_frame.svg" );
    _itemFrame->setCacheMode( QGraphicsItem::NoCache );
    _itemFrame->setZValue( _frameZ );
    _itemFrame->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemFrame->moveBy( _scaleX * _originalFramePos.x(), _scaleY * _originalFramePos.y() );
    _scene->addItem( _itemFrame );

    _itemVfe = _scene->addRect( _scaleX * _originalVfePos.x(),
                                _scaleY * _originalVfePos.y(),
                                _scaleX * _originalVfeWidth,
                                _scaleY * 10,
                                _vfePen, _vfeBrush );
    _itemVfe->setZValue( _iasVfeZ );

    _itemVne = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_asi_vne.svg" );
    _itemVne->setCacheMode( QGraphicsItem::NoCache );
    _itemVne->setZValue( _iasVneZ );
    _itemVne->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemVne->moveBy( _scaleX * _originalScale1Pos.x(), _scaleY * _originalScale1Pos.y() );
    _scene->addItem( _itemVne );

    _itemAirspeed = new QGraphicsTextItem( QString( "000" ) );
    _itemAirspeed->setCacheMode( QGraphicsItem::NoCache );
    _itemAirspeed->setZValue( _frameTextZ );
    _itemAirspeed->setTextInteractionFlags( Qt::NoTextInteraction );
    _itemAirspeed->setDefaultTextColor( Colors::_white );
    _itemAirspeed->setFont( Fonts::medium() );
    _itemAirspeed->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemAirspeed->moveBy( _scaleX * ( _originalAirspeedCtr.x() - _itemAirspeed->boundingRect().width()  / 2.0 ),
                           _scaleY * ( _originalAirspeedCtr.y() - _itemAirspeed->boundingRect().height() / 2.0 ) );
    _scene->addItem( _itemAirspeed );

    _itemMachNo = new QGraphicsTextItem( QString( ".000" ) );
    _itemMachNo->setCacheMode( QGraphicsItem::NoCache );
    _itemMachNo->setZValue( _frameTextZ );
    _itemMachNo->setTextInteractionFlags( Qt::NoTextInteraction );
    _itemMachNo->setDefaultTextColor( Colors::_white );
    _itemMachNo->setFont( Fonts::medium() );
    _itemMachNo->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemMachNo->moveBy( _scaleX * ( _originalMachNoCtr.x() - _itemMachNo->boundingRect().width()  / 2.0 ),
                         _scaleY * ( _originalMachNoCtr.y() - _itemMachNo->boundingRect().height() / 2.0 ) );
    _scene->addItem( _itemMachNo );

    _itemSetpoint = new QGraphicsTextItem( QString( "000" ) );
    _itemSetpoint->setCacheMode( QGraphicsItem::NoCache );
    _itemSetpoint->setZValue( _frameTextZ );
    _itemSetpoint->setTextInteractionFlags( Qt::NoTextInteraction );
    _itemSetpoint->setDefaultTextColor( Colors::_magenta );
    _itemSetpoint->setFont( Fonts::medium() );
    _itemSetpoint->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemSetpoint->moveBy( _scaleX * ( _originalAirspeedSet.x() - _itemMachNo->boundingRect().width()  / 2.0 ),
                           _scaleY * ( _originalAirspeedSet.y() - _itemMachNo->boundingRect().height() / 2.0 ) );
    _scene->addItem( _itemSetpoint );

    update( scaleX, scaleY );
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ASI::update( double scaleX, double scaleY )
{
    _scaleX = scaleX;
    _scaleY = scaleY;

    updateAirspeed();

    _scale1DeltaY_old = _scale1DeltaY_new;
    _scale2DeltaY_old = _scale2DeltaY_new;
    _labelsDeltaY_old = _labelsDeltaY_new;
    _bugDeltaY_old    = _bugDeltaY_new;
    _vneDeltaY_old    = _vneDeltaY_new;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ASI::setAirspeed( double airspeed )
{
    _airspeed = airspeed;

    if      ( _airspeed <    0.0 ) _airspeed =    0.0;
    else if ( _airspeed > 9999.0 ) _airspeed = 9999.0;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ASI::setMachNo( double machNo )
{
    _machNo = machNo;

    if      ( _machNo <  0.0 ) _machNo =  0.0;
    else if ( _machNo > 99.9 ) _machNo = 99.9;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ASI::setAirspeedSel( double airspeed )
{
    _airspeed_sel = airspeed;

    if      ( _airspeed_sel <    0.0 ) _airspeed_sel =    0.0;
    else if ( _airspeed_sel > 9999.0 ) _airspeed_sel = 9999.0;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ASI::setVfe( double vfe )
{
    _vfe = vfe;

    if      ( _vfe <    0.0 ) _vfe =    0.0;
    else if ( _vfe > 9999.0 ) _vfe = 9999.0;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ASI::setVne( double vne )
{
    _vne = vne;

    if      ( _vne <    0.0 ) _vne =    0.0;
    else if ( _vne > 9999.0 ) _vne = 9999.0;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ASI::reset()
{
    _itemBack     = Q_NULLPTR;
    _itemScale1   = Q_NULLPTR;
    _itemScale2   = Q_NULLPTR;
    _itemLabel1   = Q_NULLPTR;
    _itemLabel2   = Q_NULLPTR;
    _itemLabel3   = Q_NULLPTR;
    _itemLabel4   = Q_NULLPTR;
    _itemLabel5   = Q_NULLPTR;
    _itemLabel6   = Q_NULLPTR;
    _itemLabel7   = Q_NULLPTR;
    _itemBugIAS   = Q_NULLPTR;
    _itemFrame    = Q_NULLPTR;
    _itemVfe      = Q_NULLPTR;
    _itemVne      = Q_NULLPTR;
    _itemAirspeed = Q_NULLPTR;
    _itemMachNo   = Q_NULLPTR;
    _itemSetpoint = Q_NULLPTR;

    _airspeed = 0.0;
    _machNo   = 0.0;

    _airspeed_sel = 0.0;

    _scale1DeltaY_new = 0.0;
    _scale1DeltaY_old = 0.0;
    _scale2DeltaY_new = 0.0;
    _scale2DeltaY_old = 0.0;
    _labelsDeltaY_new = 0.0;
    _labelsDeltaY_old = 0.0;
    _bugDeltaY_new    = 0.0;
    _bugDeltaY_old    = 0.0;
    _vneDeltaY_new    = 0.0;
    _vneDeltaY_old    = 0.0;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ASI::updateAirspeed()
{
    _itemAirspeed->setPlainText( QString("%1").arg(_airspeed     , 3, 'f', 0, QChar(' ')) );
    _itemSetpoint->setPlainText( QString("%1").arg(_airspeed_sel , 3, 'f', 0, QChar(' ')) );

    if ( _machNo < 1.0 )
    {
        double machNo = 1000.0 * _machNo;
        _itemMachNo->setPlainText( QString(".%1").arg(machNo, 3, 'f', 0, QChar('0')) );
    }
    else
    {
        if ( _machNo < 10.0 )
        {
            _itemMachNo->setPlainText( QString::number( _machNo, 'f', 2 ) );
        }
        else
        {
            _itemMachNo->setPlainText( QString::number( _machNo, 'f', 1 ) );
        }
    }

    updateScale();
    updateScaleLabels();
    updateAirspeedBug();
    updateVfe();
    updateVne();
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ASI::updateAirspeedBug()
{
    _bugDeltaY_new = _scaleY * _originalPixPerSpd * ( _airspeed - _airspeed_sel );

    if      ( _bugDeltaY_new < -_scaleY * 85.0 ) _bugDeltaY_new = -_scaleY * 85.0;
    else if ( _bugDeltaY_new >  _scaleY * 85.0 ) _bugDeltaY_new =  _scaleY * 85.0;

    _itemBugIAS->moveBy( 0.0, _bugDeltaY_new - _bugDeltaY_old );
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ASI::updateScale()
{
    _scale1DeltaY_new = _scaleY * _originalPixPerSpd * _airspeed;
    _scale2DeltaY_new = _scale1DeltaY_new;

    double scaleSingleHeight = _scaleY * _originalScaleHeight;
    double scaleDoubleHeight = _scaleY * _originalScaleHeight * 2.0;

    while ( _scale1DeltaY_new > scaleSingleHeight + _scaleY * 74.5 )
    {
        _scale1DeltaY_new = _scale1DeltaY_new - scaleDoubleHeight;
    }

    while ( _scale2DeltaY_new > scaleDoubleHeight + _scaleY * 74.5 )
    {
        _scale2DeltaY_new = _scale2DeltaY_new - scaleDoubleHeight;
    }

    _itemScale1->moveBy( 0.0, _scale1DeltaY_new - _scale1DeltaY_old );
    _itemScale2->moveBy( 0.0, _scale2DeltaY_new - _scale2DeltaY_old );
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ASI::updateScaleLabels()
{
    _labelsDeltaY_new = _scaleY * _originalPixPerSpd * _airspeed;

    int tmp = floor( _airspeed + 0.5f );
    int spd = tmp - ( tmp % 20 );

    double spd1 = static_cast< double >( spd ) + 60.0;
    double spd2 = static_cast< double >( spd ) + 40.0;
    double spd3 = static_cast< double >( spd ) + 20.0;
    double spd4 = static_cast< double >( spd );
    double spd5 = static_cast< double >( spd ) - 20.0;
    double spd6 = static_cast< double >( spd ) - 40.0;
    double spd7 = static_cast< double >( spd ) - 60.0;

    while ( _labelsDeltaY_new > _scaleY * 15.0 )
    {
        _labelsDeltaY_new = _labelsDeltaY_new - _scaleY * 30.0;
    }

    if ( _labelsDeltaY_new < 0.0 && _airspeed > spd4 )
    {
        spd1 += 20.0;
        spd2 += 20.0;
        spd3 += 20.0;
        spd4 += 20.0;
        spd5 += 20.0;
        spd6 += 20.0;
        spd7 += 20.0;
    }

    _itemLabel1->moveBy( 0.0, _labelsDeltaY_new - _labelsDeltaY_old );
    _itemLabel2->moveBy( 0.0, _labelsDeltaY_new - _labelsDeltaY_old );
    _itemLabel3->moveBy( 0.0, _labelsDeltaY_new - _labelsDeltaY_old );
    _itemLabel4->moveBy( 0.0, _labelsDeltaY_new - _labelsDeltaY_old );
    _itemLabel5->moveBy( 0.0, _labelsDeltaY_new - _labelsDeltaY_old );
    _itemLabel6->moveBy( 0.0, _labelsDeltaY_new - _labelsDeltaY_old );
    _itemLabel7->moveBy( 0.0, _labelsDeltaY_new - _labelsDeltaY_old );

    if ( spd1 >= 0.0 && spd1 <= 10000.0 )
    {
        _itemLabel1->setVisible( true );
        _itemLabel1->setPlainText( QString("%1").arg(spd1, 3, 'f', 0, QChar(' ')) );
    }
    else
    {
        _itemLabel1->setVisible( false );
    }

    if ( spd2 >= 0.0 && spd2 <= 10000.0 )
    {
        _itemLabel2->setVisible( true );
        _itemLabel2->setPlainText( QString("%1").arg(spd2, 3, 'f', 0, QChar(' ')) );
    }
    else
    {
        _itemLabel2->setVisible( false );
    }

    if ( spd3 >= 0.0 && spd3 <= 10000.0 )
    {
        _itemLabel3->setVisible( true );
        _itemLabel3->setPlainText( QString("%1").arg(spd3, 3, 'f', 0, QChar(' ')) );
    }
    else
    {
        _itemLabel3->setVisible( false );
    }

    if ( spd4 >= 0.0 && spd4 <= 10000.0 )
    {
        _itemLabel4->setVisible( true );
        _itemLabel4->setPlainText( QString("%1").arg(spd4, 3, 'f', 0, QChar(' ')) );
    }
    else
    {
        _itemLabel4->setVisible( false );
    }

    if ( spd5 >= 0.0 && spd5 <= 10000.0 )
    {
        _itemLabel5->setVisible( true );
        _itemLabel5->setPlainText( QString("%1").arg(spd5, 3, 'f', 0, QChar(' ')) );
    }
    else
    {
        _itemLabel5->setVisible( false );
    }

    if ( spd6 >= 0.0 && spd6 <= 10000.0 )
    {
        _itemLabel6->setVisible( true );
        _itemLabel6->setPlainText( QString("%1").arg(spd6, 3, 'f', 0, QChar(' ')) );
    }
    else
    {
        _itemLabel6->setVisible( false );
    }

    if ( spd7 >= 0.0 && spd7 <= 10000.0 )
    {
        _itemLabel7->setVisible( true );
        _itemLabel7->setPlainText( QString("%1").arg(spd7, 3, 'f', 0, QChar(' ')) );
    }
    else
    {
        _itemLabel7->setVisible( false );
    }
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ASI::updateVfe()
{
    double height = _scaleY * _originalPixPerSpd * _vfe;
    double offset = _scaleY * _originalPixPerSpd * _airspeed;

    _itemVfe->setRect( _scaleX * _originalVfePos.x(),
                       _scaleY * _originalVfePos.y() - height + offset,
                       _scaleX * _originalVfeWidth,
                       height );
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ASI::updateVne()
{
    _vneDeltaY_new = _scaleY * _originalPixPerSpd * ( _airspeed - _vne );

    _itemVne->moveBy( 0.0, _vneDeltaY_new - _vneDeltaY_old );
}

////////////////////////////////////////////////////////////////////////////////

GraphicsEADI::HDG::HDG( QGraphicsScene *scene ) :
    _scene ( scene ),

    _itemBack      ( Q_NULLPTR ),
    _itemFace      ( Q_NULLPTR ),
    _itemHdgBug    ( Q_NULLPTR ),
    _itemMarks     ( Q_NULLPTR ),
    _itemFrameText ( Q_NULLPTR ),

    _heading ( 0.0 ),
    _heading_sel ( 0.0 ),

    _scaleX ( 1.0 ),
    _scaleY ( 1.0 ),

    _originalHsiCtr       ( 150.0 , 345.0 ),
    _originalBackPos      (   0.0,  210.0 ),
    _originalFacePos      (  38.0 , 233.0 ),
    _originalMarksPos     ( 134.0 , 217.0 ),
    _originalFrameTextCtr ( 149.5 , 225.5 ),

    _backZ      (  91 ),
    _faceZ      (  92 ),
    _hdgBugZ    ( 100 ),
    _marksZ     ( 110 ),
    _frameTextZ ( 120 )
{
    reset();
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::HDG::init( double scaleX, double scaleY )
{
    _scaleX = scaleX;
    _scaleY = scaleY;

    reset();

    _itemBack = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_hsi_back.svg" );
    _itemBack->setCacheMode( QGraphicsItem::NoCache );
    _itemBack->setZValue( _backZ );
    _itemBack->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemBack->moveBy( _scaleX * _originalBackPos.x(), _scaleY * _originalBackPos.y() );
    _scene->addItem( _itemBack );

    _itemFace = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_hsi_face.svg" );
    _itemFace->setCacheMode( QGraphicsItem::NoCache );
    _itemFace->setZValue( _faceZ );
    _itemFace->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemFace->setTransformOriginPoint( _originalHsiCtr - _originalFacePos );
    _itemFace->moveBy( _scaleX * _originalFacePos.x(), _scaleY * _originalFacePos.y() );
    _scene->addItem( _itemFace );

    _itemHdgBug = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_hsi_bug.svg" );
    _itemHdgBug->setCacheMode( QGraphicsItem::NoCache );
    _itemHdgBug->setZValue( _hdgBugZ );
    _itemHdgBug->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemHdgBug->setTransformOriginPoint( _originalHsiCtr - _originalFacePos );
    _itemHdgBug->moveBy( _scaleX * _originalFacePos.x(), _scaleY * _originalFacePos.y() );
    _scene->addItem( _itemHdgBug );

    _itemMarks = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_hsi_marks.svg" );
    _itemMarks->setCacheMode( QGraphicsItem::NoCache );
    _itemMarks->setZValue( _marksZ );
    _itemMarks->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemMarks->moveBy( _scaleX * _originalMarksPos.x(), _scaleY * _originalMarksPos.y() );
    _scene->addItem( _itemMarks );

    _itemFrameText = new QGraphicsTextItem( QString( "000" ) );
    _itemFrameText->setCacheMode( QGraphicsItem::NoCache );
    _itemFrameText->setZValue( _frameTextZ );
    _itemFrameText->setTextInteractionFlags( Qt::NoTextInteraction );
    _itemFrameText->setDefaultTextColor( Colors::_white );
    _itemFrameText->setFont( Fonts::medium() );
    _itemFrameText->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemFrameText->moveBy( _scaleX * ( _originalFrameTextCtr.x() - _itemFrameText->boundingRect().width()  / 2.0 ),
                            _scaleY * ( _originalFrameTextCtr.y() - _itemFrameText->boundingRect().height() / 2.0 ) );
    _scene->addItem( _itemFrameText );

    update( scaleX, scaleY );
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::HDG::update( double scaleX, double scaleY )
{
    _scaleX = scaleX;
    _scaleY = scaleY;

    updateHeading();
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::HDG::setHeading( double heading )
{
    _heading = heading;

    while ( _heading <   0.0 ) _heading += 360.0;
    while ( _heading > 360.0 ) _heading -= 360.0;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::HDG::setHeadingSel( double heading )
{
    _heading_sel = heading;

    while ( _heading_sel <   0.0 ) _heading_sel += 360.0;
    while ( _heading_sel > 360.0 ) _heading_sel -= 360.0;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::HDG::reset()
{
    _itemBack      = Q_NULLPTR;
    _itemFace      = Q_NULLPTR;
    _itemHdgBug    = Q_NULLPTR;
    _itemMarks     = Q_NULLPTR;
    _itemFrameText = Q_NULLPTR;

    _heading = 0.0;
    _heading_sel = 0.0;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::HDG::updateHeading()
{
    _itemFace->setRotation( -_heading );
    _itemHdgBug->setRotation( -_heading + _heading_sel );

    double fHeading = floor( _heading + 0.5 );

    _itemFrameText->setPlainText( QString("%1").arg(fHeading, 3, 'f', 0, QChar('0')) );
}

////////////////////////////////////////////////////////////////////////////////

GraphicsEADI::VSI::VSI( QGraphicsScene *scene ) :
    _scene ( scene ),

    _itemScale  ( Q_NULLPTR ),
    _itemMarker ( Q_NULLPTR ),

    _climbRate ( 0.0 ),

    _scaleX ( 1.0 ),
    _scaleY ( 1.0 ),

    _originalMarkerWidth (  4.0 ),
    _originalPixPerSpd1  ( 30.0 ),
    _originalPixPerSpd2  ( 20.0 ),
    _originalPixPerSpd4  (  5.0 ),

    _originalScalePos  ( 275.0 ,  50.0 ),
    _originalMarkerPos ( 285.0 , 124.5 ),

    _markerBrush ( QColor( 0xff, 0xff, 0xff ), Qt::SolidPattern ),
    _markerPen ( _markerBrush, 0 ),

    _scaleZ  ( 70 ),
    _markerZ ( 8000 )
{
    reset();
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::VSI::init( double scaleX, double scaleY )
{
    _scaleX = scaleX;
    _scaleY = scaleY;

    reset();

    _itemScale = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_vsi_scale.svg" );
    _itemScale->setCacheMode( QGraphicsItem::NoCache );
    _itemScale->setZValue( _scaleZ );
    _itemScale->setTransform( QTransform::fromScale( _scaleX, _scaleY ), true );
    _itemScale->moveBy( _scaleX * _originalScalePos.x(), _scaleY * _originalScalePos.y() );
    _scene->addItem( _itemScale );

    _itemMarker = _scene->addRect( _scaleX * _originalMarkerPos.x(),
                                   _scaleY * _originalMarkerPos.y(),
                                   _scaleX * _originalMarkerWidth,
                                   _scaleY * 10,
                                   _markerPen, _markerBrush );
    _itemMarker->setZValue( _markerZ );

    update( scaleX, scaleY );
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::VSI::update( double scaleX, double scaleY )
{
    _scaleX = scaleX;
    _scaleY = scaleY;

    updateVSI();
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::VSI::setClimbRate( double climbRate )
{
    _climbRate = climbRate;

    if      ( _climbRate >  6.8 ) _climbRate =  6.8;
    else if ( _climbRate < -6.8 ) _climbRate = -6.8;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::VSI::reset()
{
    _itemScale = Q_NULLPTR;
    _climbRate = 0.0;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::VSI::updateVSI()
{
    //m_scene->clear();

    double climbRateAbs = fabs( _climbRate );
    double arrowDeltaY = 0.0;

    if ( climbRateAbs <= 1.0 )
    {
        arrowDeltaY = _originalPixPerSpd1 * climbRateAbs;
    }
    else if ( climbRateAbs <= 2.0 )
    {
        arrowDeltaY = _originalPixPerSpd1 + _originalPixPerSpd2 * ( climbRateAbs - 1.0 );
    }
    else
    {
        arrowDeltaY = _originalPixPerSpd1 + _originalPixPerSpd2 + _originalPixPerSpd4 * ( climbRateAbs - 2.0 );
    }

    if ( _climbRate > 0.0 )
    {
        _itemMarker->setRect( _scaleX * _originalMarkerPos.x(),
                              _scaleY * _originalMarkerPos.y() - _scaleY * arrowDeltaY,
                              _scaleX * _originalMarkerWidth,
                              _scaleY * arrowDeltaY );
    }
    else
    {
        _itemMarker->setRect( _scaleX * _originalMarkerPos.x(),
                              _scaleY * _originalMarkerPos.y(),
                              _scaleX * _originalMarkerWidth,
                              _scaleY * arrowDeltaY );
    }
}
