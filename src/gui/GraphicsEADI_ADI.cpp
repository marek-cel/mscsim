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

GraphicsEADI::ADI::ADI( QGraphicsScene *scene ) :
    m_scene ( scene ),

    m_itemBack   ( 0 ),
    m_itemLadd   ( 0 ),
    m_itemRoll   ( 0 ),
    m_itemSlip   ( 0 ),
    m_itemTurn   ( 0 ),
    m_itemDotH   ( 0 ),
    m_itemDotV   ( 0 ),
    m_itemFD     ( 0 ),
    m_itemStall  ( 0 ),
    m_itemMask   ( 0 ),
    m_itemScaleH ( 0 ),
    m_itemScaleV ( 0 ),

    m_roll     ( 0.0f ),
    m_pitch    ( 0.0f ),
    m_slipSkid ( 0.0f ),
    m_turnRate ( 0.0f ),
    m_dotH     ( 0.0f ),
    m_dotV     ( 0.0f ),
    m_fdRoll   ( 0.0f ),
    m_fdPitch  ( 0.0f ),

    m_dotVisibleH ( false ),
    m_dotVisibleV ( false ),
    m_fdVisible   ( false ),

    m_stall ( false ),

    m_laddDeltaX_new     ( 0.0f ),
    m_laddDeltaX_old     ( 0.0f ),
    m_laddDeltaY_new     ( 0.0f ),
    m_laddDeltaY_old     ( 0.0f ),
    m_laddBackDeltaX_new ( 0.0f ),
    m_laddBackDeltaX_old ( 0.0f ),
    m_laddBackDeltaY_new ( 0.0f ),
    m_laddBackDeltaY_old ( 0.0f ),
    m_slipDeltaX_new     ( 0.0f ),
    m_slipDeltaX_old     ( 0.0f ),
    m_slipDeltaY_new     ( 0.0f ),
    m_slipDeltaY_old     ( 0.0f ),
    m_turnDeltaX_new     ( 0.0f ),
    m_turnDeltaX_old     ( 0.0f ),
    m_dotHDeltaX_new     ( 0.0f ),
    m_dotHDeltaX_old     ( 0.0f ),
    m_dotVDeltaY_new     ( 0.0f ),
    m_dotVDeltaY_old     ( 0.0f ),
    m_fdDeltaX_new       ( 0.0f ),
    m_fdDeltaX_old       ( 0.0f ),
    m_fdDeltaY_new       ( 0.0f ),
    m_fdDeltaY_old       ( 0.0f ),

    m_scaleX ( 1.0f ),
    m_scaleY ( 1.0f ),

    m_originalPixPerDeg (   3.0f ),
    m_deltaLaddBack_max (  52.5f ),
    m_deltaLaddBack_min ( -52.5f ),
    m_maxSlipDeflection (  20.0f ),
    m_maxTurnDeflection (  55.0f ),
    m_maxDotsDeflection (  50.0f ),

    m_originalAdiCtr    ( 150.0f ,  125.0f ),
    m_originalBackPos   (  45.0f ,  -85.0f ),
    m_originalLaddPos   ( 110.0f , -175.0f ),
    m_originalRollPos   (  45.0f ,   20.0f ),
    m_originalSlipPos   ( 145.5f ,   68.0f ),
    m_originalTurnPos   ( 142.5f ,  206.0f ),
    m_originalDotHPos   ( 145.0f ,  188.0f ),
    m_originalDotVPos   ( 213.0f ,  120.0f ),
    m_originalFdPos     ( 107.0f ,  124.5f ),
    m_originalStallPos  ( 122.0f ,   91.0f ),
    m_originalScaleHPos (   0.0f ,    0.0f ),
    m_originalScaleVPos (   0.0f ,    0.0f ),

    m_backZ   ( 10 ),
    m_laddZ   ( 20 ),
    m_rollZ   ( 30 ),
    m_slipZ   ( 40 ),
    m_dotsZ   ( 50 ),
    m_fdZ     ( 50 ),
    m_scalesZ ( 51 ),
    m_maskZ   ( 60 ),
    m_turnZ   ( 70 ),
    m_stallZ  ( 80 )
{
    reset();
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ADI::init( float scaleX, float scaleY )
{
    m_scaleX = scaleX;
    m_scaleY = scaleY;

    reset();

    m_itemBack = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_adi_back.svg" );
    m_itemBack->setCacheMode( QGraphicsItem::NoCache );
    m_itemBack->setZValue( m_backZ );
    m_itemBack->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemBack->setTransformOriginPoint( m_originalAdiCtr - m_originalBackPos );
    m_itemBack->moveBy( m_scaleX * m_originalBackPos.x(), m_scaleY * m_originalBackPos.y() );
    m_scene->addItem( m_itemBack );

    m_itemLadd = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_adi_ladd.svg" );
    m_itemLadd->setCacheMode( QGraphicsItem::NoCache );
    m_itemLadd->setZValue( m_laddZ );
    m_itemLadd->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemLadd->setTransformOriginPoint( m_originalAdiCtr - m_originalLaddPos );
    m_itemLadd->moveBy( m_scaleX * m_originalLaddPos.x(), m_scaleY * m_originalLaddPos.y() );
    m_scene->addItem( m_itemLadd );

    m_itemRoll = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_adi_roll.svg" );
    m_itemRoll->setCacheMode( QGraphicsItem::NoCache );
    m_itemRoll->setZValue( m_rollZ );
    m_itemRoll->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemRoll->setTransformOriginPoint( m_originalAdiCtr - m_originalRollPos );
    m_itemRoll->moveBy( m_scaleX * m_originalRollPos.x(), m_scaleY * m_originalRollPos.y() );
    m_scene->addItem( m_itemRoll );

    m_itemSlip = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_adi_slip.svg" );
    m_itemSlip->setCacheMode( QGraphicsItem::NoCache );
    m_itemSlip->setZValue( m_slipZ );
    m_itemSlip->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemSlip->setTransformOriginPoint( m_originalAdiCtr - m_originalSlipPos );
    m_itemSlip->moveBy( m_scaleX * m_originalSlipPos.x(), m_scaleY * m_originalSlipPos.y() );
    m_scene->addItem( m_itemSlip );

    m_itemTurn = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_adi_turn.svg" );
    m_itemTurn->setCacheMode( QGraphicsItem::NoCache );
    m_itemTurn->setZValue( m_turnZ );
    m_itemTurn->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemTurn->moveBy( m_scaleX * m_originalTurnPos.x(), m_scaleY * m_originalTurnPos.y() );
    m_scene->addItem( m_itemTurn );

    m_itemDotH = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_adi_doth.svg" );
    m_itemDotH->setCacheMode( QGraphicsItem::NoCache );
    m_itemDotH->setZValue( m_dotsZ - 1 );
    m_itemDotH->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemDotH->moveBy( m_scaleX * m_originalDotHPos.x(), m_scaleY * m_originalDotHPos.y() );
    m_scene->addItem( m_itemDotH );

    m_itemDotV = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_adi_dotv.svg" );
    m_itemDotV->setCacheMode( QGraphicsItem::NoCache );
    m_itemDotV->setZValue( m_dotsZ - 1 );
    m_itemDotV->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemDotV->moveBy( m_scaleX * m_originalDotVPos.x(), m_scaleY * m_originalDotVPos.y() );
    m_scene->addItem( m_itemDotV );

    m_itemFD = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_adi_fd.svg" );
    m_itemFD->setCacheMode( QGraphicsItem::NoCache );
    m_itemFD->setZValue( m_fdZ );
    m_itemFD->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemFD->setTransformOriginPoint( m_originalAdiCtr - m_originalFdPos );
    m_itemFD->moveBy( m_scaleX * m_originalFdPos.x(), m_scaleY * m_originalFdPos.y() );
    m_scene->addItem( m_itemFD );

    m_itemStall = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_adi_stall.svg" );
    m_itemStall->setCacheMode( QGraphicsItem::NoCache );
    m_itemStall->setZValue( m_stallZ );
    m_itemStall->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemStall->moveBy( m_scaleX * m_originalStallPos.x(), m_scaleY * m_originalStallPos.y() );
    m_scene->addItem( m_itemStall );

    m_itemScaleH = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_adi_scaleh.svg" );
    m_itemScaleH->setCacheMode( QGraphicsItem::NoCache );
    m_itemScaleH->setZValue( m_scalesZ );
    m_itemScaleH->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemScaleH->moveBy( m_scaleX * m_originalScaleHPos.x(), m_scaleY * m_originalScaleHPos.y() );
    m_scene->addItem( m_itemScaleH );

    m_itemScaleV = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_adi_scalev.svg" );
    m_itemScaleV->setCacheMode( QGraphicsItem::NoCache );
    m_itemScaleV->setZValue( m_scalesZ );
    m_itemScaleV->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemScaleV->moveBy( m_scaleX * m_originalScaleVPos.x(), m_scaleY * m_originalScaleVPos.y() );
    m_scene->addItem( m_itemScaleV );

    m_itemMask = new QGraphicsSvgItem( ":/gui/images/efis/eadi/eadi_adi_mask.svg" );
    m_itemMask->setCacheMode( QGraphicsItem::NoCache );
    m_itemMask->setZValue( m_maskZ );
    m_itemMask->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_scene->addItem( m_itemMask );

    update( scaleX, scaleY );
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ADI::update( float scaleX, float scaleY )
{
    m_scaleX = scaleX;
    m_scaleY = scaleY;

    float delta  = m_originalPixPerDeg * m_pitch;

#   ifndef M_PI
    float roll_rad = 3.14159265358979323846 * m_roll / 180.0f;
#   else
    float roll_rad = M_PI * m_roll / 180.0f;
#   endif

    float sinRoll = sin( roll_rad );
    float cosRoll = cos( roll_rad );

    updateLadd( delta, sinRoll, cosRoll );
    updateLaddBack( delta, sinRoll, cosRoll );
    updateRoll();
    updateSlipSkid( sinRoll, cosRoll );
    updateTurnRate();
    updateDots();
    updateFD( sinRoll, cosRoll );
    updateStall();

    m_laddDeltaX_old     = m_laddDeltaX_new;
    m_laddDeltaY_old     = m_laddDeltaY_new;
    m_laddBackDeltaX_old = m_laddBackDeltaX_new;
    m_laddBackDeltaY_old = m_laddBackDeltaY_new;
    m_slipDeltaX_old     = m_slipDeltaX_new;
    m_slipDeltaY_old     = m_slipDeltaY_new;
    m_turnDeltaX_old     = m_turnDeltaX_new;
    m_dotHDeltaX_old     = m_dotHDeltaX_new;
    m_dotVDeltaY_old     = m_dotVDeltaY_new;
    m_fdDeltaX_old       = m_fdDeltaX_new;
    m_fdDeltaY_old       = m_fdDeltaY_new;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ADI::setRoll( float roll )
{
    m_roll = roll;

    if      ( m_roll < -180.0f ) m_roll = -180.0f;
    else if ( m_roll >  180.0f ) m_roll =  180.0f;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ADI::setPitch( float pitch )
{
    m_pitch = pitch;

    if      ( m_pitch < -90.0f ) m_pitch = -90.0f;
    else if ( m_pitch >  90.0f ) m_pitch =  90.0f;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ADI::setSlipSkid( float slipSkid )
{
    m_slipSkid = slipSkid;

    if      ( m_slipSkid < -1.0f ) m_slipSkid = -1.0f;
    else if ( m_slipSkid >  1.0f ) m_slipSkid =  1.0f;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ADI::setTurnRate( float turnRate )
{
    m_turnRate = turnRate;

    if      ( m_turnRate < -1.0f ) m_turnRate = -1.0f;
    else if ( m_turnRate >  1.0f ) m_turnRate =  1.0f;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ADI::setDots( float dotH, float dotV, bool visibleH, bool visibleV )
{
    m_dotH = dotH;
    m_dotV = dotV;

    if      ( m_dotH < -1.0f ) m_dotH = -1.0f;
    else if ( m_dotH >  1.0f ) m_dotH =  1.0f;

    if      ( m_dotV < -1.0f ) m_dotV = -1.0f;
    else if ( m_dotV >  1.0f ) m_dotV =  1.0f;

    m_dotVisibleH = visibleH;
    m_dotVisibleV = visibleV;
}


////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ADI::setFD( float roll, float pitch, bool visible )
{
    m_fdRoll  = roll;
    m_fdPitch = pitch;

    if      ( m_fdRoll < -180.0f ) m_fdRoll = -180.0f;
    else if ( m_fdRoll >  180.0f ) m_fdRoll =  180.0f;

    if      ( m_fdPitch < -90.0f ) m_fdPitch = -90.0f;
    else if ( m_fdPitch >  90.0f ) m_fdPitch =  90.0f;

    m_fdVisible = visible;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ADI::setStall( bool stall )
{
    m_stall = stall;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ADI::reset()
{
    m_itemBack   = 0;
    m_itemLadd   = 0;
    m_itemRoll   = 0;
    m_itemSlip   = 0;
    m_itemTurn   = 0;
    m_itemDotH   = 0;
    m_itemDotV   = 0;
    m_itemFD     = 0;
    m_itemMask   = 0;
    m_itemScaleH = 0;
    m_itemScaleV = 0;

    m_roll     = 0.0f;
    m_pitch    = 0.0f;
    m_slipSkid = 0.0f;
    m_turnRate = 0.0f;
    m_dotH     = 0.0f;
    m_dotV     = 0.0f;
    m_fdRoll   = 0.0f;
    m_fdPitch  = 0.0f;

    m_dotVisibleH  = false;
    m_dotVisibleV  = false;
    m_fdVisible    = false;

    m_stall = false;

    m_laddDeltaX_new     = 0.0f;
    m_laddDeltaX_old     = 0.0f;
    m_laddDeltaY_new     = 0.0f;
    m_laddDeltaY_old     = 0.0f;
    m_laddBackDeltaX_new = 0.0f;
    m_laddBackDeltaX_old = 0.0f;
    m_laddBackDeltaY_new = 0.0f;
    m_laddBackDeltaY_old = 0.0f;
    m_slipDeltaX_new     = 0.0f;
    m_slipDeltaX_old     = 0.0f;
    m_slipDeltaY_new     = 0.0f;
    m_slipDeltaY_old     = 0.0f;
    m_turnDeltaX_new     = 0.0f;
    m_turnDeltaX_old     = 0.0f;
    m_dotHDeltaX_new     = 0.0f;
    m_dotHDeltaX_old     = 0.0f;
    m_dotVDeltaY_new     = 0.0f;
    m_dotVDeltaY_old     = 0.0f;
    m_fdDeltaX_new       = 0.0f;
    m_fdDeltaX_old       = 0.0f;
    m_fdDeltaY_new       = 0.0f;
    m_fdDeltaY_old       = 0.0f;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ADI::updateLadd( float delta, float sinRoll, float cosRoll )
{
    m_itemLadd->setRotation( -m_roll );

    m_laddDeltaX_new = m_scaleX * delta * sinRoll;
    m_laddDeltaY_new = m_scaleY * delta * cosRoll;

    m_itemLadd->moveBy( m_laddDeltaX_new - m_laddDeltaX_old, m_laddDeltaY_new - m_laddDeltaY_old );
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ADI::updateLaddBack( float delta, float sinRoll, float cosRoll )
{
    m_itemBack->setRotation( -m_roll );

    float deltaLaddBack = 0.0;

    if ( delta > m_deltaLaddBack_max )
    {
        deltaLaddBack = m_deltaLaddBack_max;
    }
    else if ( delta < m_deltaLaddBack_min )
    {
        deltaLaddBack = m_deltaLaddBack_min;
    }
    else
    {
        deltaLaddBack = delta;
    }

    m_laddBackDeltaX_new = m_scaleX * deltaLaddBack * sinRoll;
    m_laddBackDeltaY_new = m_scaleY * deltaLaddBack * cosRoll;

    m_itemBack->moveBy( m_laddBackDeltaX_new - m_laddBackDeltaX_old, m_laddBackDeltaY_new - m_laddBackDeltaY_old );
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ADI::updateRoll()
{
    m_itemRoll->setRotation( -m_roll );
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ADI::updateSlipSkid( float sinRoll, float cosRoll )
{
    m_itemSlip->setRotation( -m_roll );

    float deltaSlip = -m_maxSlipDeflection * m_slipSkid;

    m_slipDeltaX_new =  m_scaleX * deltaSlip * cosRoll;
    m_slipDeltaY_new = -m_scaleY * deltaSlip * sinRoll;

    m_itemSlip->moveBy( m_slipDeltaX_new - m_slipDeltaX_old, m_slipDeltaY_new - m_slipDeltaY_old );
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ADI::updateTurnRate()
{
    m_turnDeltaX_new = m_scaleX * m_maxTurnDeflection * m_turnRate;

    m_itemTurn->moveBy( m_turnDeltaX_new - m_turnDeltaX_old, 0.0 );
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ADI::updateDots()
{
    if ( m_dotVisibleH )
    {
        m_itemDotH->setVisible( true );
        m_itemScaleH->setVisible( true );

        m_dotHDeltaX_new = m_scaleX * m_maxDotsDeflection * m_dotH;

        m_itemDotH->moveBy( m_dotHDeltaX_new - m_dotHDeltaX_old, 0.0f );
    }
    else
    {
        m_itemDotH->setVisible( false );
        m_itemScaleH->setVisible( false );

        m_dotHDeltaX_new = m_dotHDeltaX_old;
    }

    if ( m_dotVisibleV )
    {
        m_itemDotV->setVisible( true );
        m_itemScaleV->setVisible( true );

        m_dotVDeltaY_new = m_scaleY * m_maxDotsDeflection * m_dotV;

        m_itemDotV->moveBy( 0.0f, m_dotVDeltaY_old - m_dotVDeltaY_new );
    }
    else
    {
        m_itemDotV->setVisible( false );
        m_itemScaleV->setVisible( false );

        m_dotVDeltaY_new = m_dotVDeltaY_old;
    }
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ADI::updateFD( float sinRoll, float cosRoll )
{
    if ( m_fdVisible )
    {
        m_itemFD->setVisible( true );

        m_itemFD->setRotation( m_fdRoll - m_roll );

        float pitch = m_pitch - m_fdPitch;

        if      ( pitch < -17.0f ) pitch = -17.0f;
        else if ( pitch >  17.0f ) pitch =  17.0f;

        float delta = m_originalPixPerDeg * pitch;

        m_fdDeltaX_new = m_scaleX * delta * sinRoll;
        m_fdDeltaY_new = m_scaleY * delta * cosRoll;

        m_itemFD->moveBy( m_fdDeltaX_new - m_fdDeltaX_old,
                          m_fdDeltaY_new - m_fdDeltaY_old );
    }
    else
    {
        m_itemFD->setVisible( false );
    }
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsEADI::ADI::updateStall()
{
    if ( m_stall )
    {
        m_itemStall->setVisible( true );
    }
    else
    {
        m_itemStall->setVisible( false );
    }
}
