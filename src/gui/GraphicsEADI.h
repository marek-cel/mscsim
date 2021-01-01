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
#ifndef GRAPHICSEADI_H
#define GRAPHICSEADI_H

////////////////////////////////////////////////////////////////////////////////

#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QGraphicsSvgItem>

#include <Defines.h>

////////////////////////////////////////////////////////////////////////////////

/**
 * @brief This class provides EADI widget.
 */
class GraphicsEADI : public QGraphicsView
{
    Q_OBJECT

public:

    enum FlightMode { FM_OFF = 0, FM_FD, FM_CMD };
    enum SpeedMode  { SM_OFF = 0, SM_FMC_SPD };

    enum LNAV { LNAV_OFF = 0, LNAV_HDG, LNAV_NAV, LNAV_NAV_ARM, LNAV_APR, LNAV_APR_ARM, LNAV_BC, LNAV_BC_ARM };
    enum VNAV { VNAV_OFF = 0, VNAV_ALT, VNAV_IAS, VNAV_VS, VNAV_ALT_SEL, VNAV_GS, VNAV_GS_ARM };

    /** @brief Constructor. */
    explicit GraphicsEADI( QWidget *parent = NULLPTR );

    /** @brief Destructor. */
    virtual ~GraphicsEADI();

    /** */
    void reinit();

    /** */
    inline void setFlightMode( FlightMode flightMode )
    {
        _flightMode = flightMode;
    }

    /** */
    inline void setSpeedMode( SpeedMode speedMode )
    {
        _speedMode = speedMode;
    }

    /** */
    inline void setLNAV( LNAV lnav )
    {
        _lnav = lnav;
    }

    /** */
    inline void setVNAV( VNAV vnav )
    {
        _vnav = vnav;
    }

    /** @param roll angle [deg] */
    inline void setRoll( float roll )
    {
        _adi->setRoll( roll );
    }

    /** @param pitch angle [deg] */
    inline void setPitch( float pitch )
    {
        _adi->setPitch( pitch );
    }

    /** @param normalized slip or skid (range from -1.0 to 1.0) */
    inline void setSlipSkid( float slipSkid )
    {
        _adi->setSlipSkid( slipSkid );
    }

    /**
     * @param normalized turn rate (range from -1.0 to 1.0),
     * hash marks positions are set to be -0.5 and 0.5 */
    inline void setTurnRate( float turnRate )
    {
        _adi->setTurnRate( turnRate );
    }

    /**
     * @param normalized horizontal deviation dot position (range from -1.0 to 1.0)
     * @param normalized vertical deviation dot position (range from -1.0 to 1.0)
     * @param deviation horizontal dot visibility
     * @param deviation vertical dot visibility */
    inline void setDots( float dotH, float dotV, bool visibleH, bool visibleV )
    {
        _adi->setDots( dotH, dotV, visibleH, visibleV );
    }

    /**
     * @param FD roll angle [deg]
     * @param FD pitch angle [deg]
     * @param FD visibility */
    inline void setFD( float roll, float pitch, bool visible = true )
    {
        _adi->setFD( roll, pitch, visible );
    }

    /** @param stall flag */
    inline void setStall( bool stall )
    {
        _adi->setStall( stall );
    }

    /** @param altitude (dimensionless numeric value) */
    inline void setAltitude( float altitude )
    {
        _alt->setAltitude( altitude );
    }

    /** @param airspeed (dimensionless numeric value) */
    inline void setAirspeed( float airspeed )
    {
        _asi->setAirspeed( airspeed );
    }

    /** @param Mach number */
    inline void setMachNo( float machNo )
    {
        _asi->setMachNo( machNo );
    }

    /** @param heading [deg] */
    inline void setHeading( float heading )
    {
        _hdg->setHeading( heading );
    }

    /** @param climb rate (dimensionless numeric value)  */
    inline void setClimbRate( float climbRate )
    {
        _vsi->setClimbRate( climbRate );
    }

    /** @param airspeed (dimensionless numeric value) */
    inline void setAirspeedSel( double airspeed )
    {
        _asi->setAirspeedSel( airspeed );
    }

    /** @param altitude (dimensionless numeric value) */
    inline void setAltitudeSel( double altitude )
    {
        _alt->setAltitudeSel( altitude );
    }

    /** @param heading [deg] */
    inline void setHeadingSel( float heading )
    {
        _hdg->setHeadingSel( heading );
    }

    /** @param vfe (dimensionless numeric value) */
    inline void setVfe( double vfe )
    {
        _asi->setVfe( vfe );
    }

    /** @param vne (dimensionless numeric value) */
    inline void setVne( double vne )
    {
        _asi->setVne( vne );
    }

protected:

    /** */
    void resizeEvent( QResizeEvent *event );

    /** */
    void timerEvent( QTimerEvent *event );

private:

    class ADI;
    class ALT;
    class ASI;
    class HDG;
    class VSI;

    int _timerId;                           ///<

    QGraphicsScene *_scene;                 ///< graphics scene

    GraphicsEADI::ADI *_adi;                ///<
    GraphicsEADI::ALT *_alt;                ///<
    GraphicsEADI::ASI *_asi;                ///<
    GraphicsEADI::HDG *_hdg;                ///<
    GraphicsEADI::VSI *_vsi;                ///<

    QGraphicsSvgItem *_itemBack;            ///< PFD background
    QGraphicsSvgItem *_itemMask;            ///< PFD mask

    QGraphicsTextItem *_itemFMA;            ///< FMA (Flight Mode Annunciator)
    QGraphicsTextItem *_itemSPD;

    QGraphicsTextItem *_itemLNAV;           ///< LNAV (Lateral Navigation Mode)
    QGraphicsTextItem *_itemVNAV;           ///< VNAV (Vertical Navigation Mode)

    QGraphicsTextItem *_itemLNAV_ARM;       ///< LNAV (Lateral Navigation Mode)
    QGraphicsTextItem *_itemVNAV_ARM;       ///< LNAV (Lateral Navigation Mode)

    FlightMode _flightMode;                 ///<
    SpeedMode  _speedMode;                  ///<

    LNAV _lnav;                             ///<
    VNAV _vnav;                             ///<

    float _scaleX;                          ///<
    float _scaleY;                          ///<

    QPointF _originalFMA;
    QPointF _originalSPD;

    QPointF _originalLNAV;
    QPointF _originalVNAV;

    QPointF _originalLNAV_ARM;
    QPointF _originalVNAV_ARM;

    const int _originalHeight;              ///< [px]
    const int _originalWidth;               ///< [px]

    const int _backZ;                       ///<
    const int _maskZ;                       ///<
    const int _textZ;                       ///<

    void init();

    void reset();

    void updateView();

    /** Attitude Director Indicator */
    class ADI
    {
    public:
        ADI( QGraphicsScene *scene );

        void init( float scaleX, float scaleY );
        void update( float scaleX, float scaleY );

        void setRoll( float roll );
        void setPitch( float pitch );
        void setSlipSkid( float slipSkid );
        void setTurnRate( float turnRate );
        void setDots( float dotH, float dotV, bool visibleH, bool visibleV );
        void setFD( float roll, float pitch, bool visible = true );
        void setStall( bool stall );

    private:

        QGraphicsScene *_scene;             ///< graphics scene

        QGraphicsSvgItem  *_itemBack;       ///< background
        QGraphicsSvgItem  *_itemLadd;       ///< pitch ladder
        QGraphicsSvgItem  *_itemRoll;       ///< roll mask
        QGraphicsSvgItem  *_itemSlip;       ///< slip indicator
        QGraphicsSvgItem  *_itemTurn;       ///< turn rate indicator
        QGraphicsSvgItem  *_itemDotH;       ///<
        QGraphicsSvgItem  *_itemDotV;       ///<
        QGraphicsSvgItem  *_itemFD;         ///< flight director
        QGraphicsSvgItem  *_itemStall;      ///< stall warning
        QGraphicsSvgItem  *_itemMask;       ///< adi mask
        QGraphicsSvgItem  *_itemScaleH;     ///<
        QGraphicsSvgItem  *_itemScaleV;     ///<

        float _roll;                        ///< [deg]
        float _pitch;                       ///< [deg]
        float _slipSkid;                    ///< -1.0 ... 1.0
        float _turnRate;                    ///< -1.0 ... 1.0
        float _dotH;                        ///< -1.0 ... 1.0
        float _dotV;                        ///< -1.0 ... 1.0
        float _fdRoll;                      ///< [deg]
        float _fdPitch;                     ///< [deg]

        bool _dotVisibleH;                  ///<
        bool _dotVisibleV;                  ///<
        bool _fdVisible;                    ///<

        bool _stall;                        ///<

        float _laddDeltaX_new;              ///<
        float _laddDeltaX_old;              ///<
        float _laddDeltaY_new;              ///<
        float _laddDeltaY_old;              ///<
        float _laddBackDeltaX_new;          ///<
        float _laddBackDeltaX_old;          ///<
        float _laddBackDeltaY_new;          ///<
        float _laddBackDeltaY_old;          ///<
        float _slipDeltaX_new;              ///<
        float _slipDeltaX_old;              ///<
        float _slipDeltaY_new;              ///<
        float _slipDeltaY_old;              ///<
        float _turnDeltaX_new;              ///<
        float _turnDeltaX_old;              ///<
        float _dotHDeltaX_new;              ///<
        float _dotHDeltaX_old;              ///<
        float _dotVDeltaY_new;              ///<
        float _dotVDeltaY_old;              ///<
        float _fdDeltaX_new;                ///<
        float _fdDeltaX_old;                ///<
        float _fdDeltaY_new;                ///<
        float _fdDeltaY_old;                ///<

        float _scaleX;                      ///<
        float _scaleY;                      ///<

        const float _originalPixPerDeg;     ///< [px/deg] pixels to move pitch ladder due to 1 deg pitch
        const float _deltaLaddBack_max;     ///< [px] max pitch ladder background deflection
        const float _deltaLaddBack_min;     ///< [px] min pitch ladder background deflection
        const float _maxSlipDeflection;     ///< [px] max slip indicator deflection
        const float _maxTurnDeflection;     ///< [px] max turn indicator deflection
        const float _maxDotsDeflection;     ///<

        QPointF _originalAdiCtr;            ///<
        QPointF _originalBackPos;           ///<
        QPointF _originalLaddPos;           ///<
        QPointF _originalRollPos;           ///<
        QPointF _originalSlipPos;           ///<
        QPointF _originalTurnPos;           ///<
        QPointF _originalDotHPos;           ///<
        QPointF _originalDotVPos;           ///<
        QPointF _originalFdPos;             ///<
        QPointF _originalStallPos;          ///<
        QPointF _originalScaleHPos;         ///<
        QPointF _originalScaleVPos;         ///<

        const int _backZ;                   ///<
        const int _laddZ;                   ///<
        const int _rollZ;                   ///<
        const int _slipZ;                   ///<
        const int _dotsZ;                   ///<
        const int _fdZ;                     ///<
        const int _scalesZ;                 ///<
        const int _maskZ;                   ///<
        const int _turnZ;                   ///<
        const int _stallZ;                  ///<

        void reset();

        void updateLadd( float delta, float sinRoll, float cosRoll );
        void updateLaddBack( float delta, float sinRoll, float cosRoll );
        void updateRoll();
        void updateSlipSkid( float sinRoll, float cosRoll );
        void updateTurnRate();
        void updateDots();
        void updateFD( float sinRoll, float cosRoll );
        void updateStall();
    };

    /** Altimeter */
    class ALT
    {
    public:
        ALT( QGraphicsScene *scene );

        void init( float scaleX, float scaleY );
        void update( float scaleX, float scaleY );

        void setAltitude( float altitude );
        void setAltitudeSel( double altitude );

    private:

        QGraphicsScene *_scene;             ///< graphics scene

        QGraphicsSvgItem  *_itemBack;       ///<
        QGraphicsSvgItem  *_itemScale1;     ///<
        QGraphicsSvgItem  *_itemScale2;     ///<
        QGraphicsTextItem *_itemLabel1;     ///<
        QGraphicsTextItem *_itemLabel2;     ///<
        QGraphicsTextItem *_itemLabel3;     ///<
        QGraphicsSvgItem  *_itemGround;     ///<
        QGraphicsSvgItem  *_itemBugAlt;     ///<
        QGraphicsSvgItem  *_itemFrame;      ///<
        QGraphicsTextItem *_itemAltitude;   ///<
        QGraphicsTextItem *_itemPressure;   ///<
        QGraphicsTextItem *_itemSetpoint;   ///<

        float _altitude;                    ///<
        float _altitude_sel;                ///<

        float _scale1DeltaY_new;            ///<
        float _scale1DeltaY_old;            ///<
        float _scale2DeltaY_new;            ///<
        float _scale2DeltaY_old;            ///<
        float _groundDeltaY_new;            ///<
        float _groundDeltaY_old;            ///<
        float _labelsDeltaY_new;            ///<
        float _labelsDeltaY_old;            ///<
        float _bugDeltaY_new;               ///<
        float _bugDeltaY_old;               ///<

        float _scaleX;                      ///<
        float _scaleY;                      ///<

        const float _originalPixPerAlt;     ///< [px/altitude unit]
        const float _originalScaleHeight;   ///< [px]
        const float _originalLabelsX;       ///< [px]
        const float _originalLabel1Y;       ///< [px]
        const float _originalLabel2Y;       ///< [px]
        const float _originalLabel3Y;       ///< [px]

        QPointF _originalBackPos;           ///<
        QPointF _originalScale1Pos;         ///<
        QPointF _originalScale2Pos;         ///<
        QPointF _originalGroundPos;         ///<
        QPointF _originalFramePos;          ///<
        QPointF _originalAltitudeCtr;       ///<
        QPointF _originalPressureCtr;       ///<
        QPointF _originalAltitudeSet;       ///<

        const int _backZ;                   ///<
        const int _scaleZ;                  ///<
        const int _labelsZ;                 ///<
        const int _groundZ;                 ///<
        const int _altBugZ;                 ///<
        const int _frameZ;                  ///<
        const int _frameTextZ;              ///<

        void reset();

        void updateAltitude();
        void updateAltitudeBug();
        void updateScale();
        void updateScaleLabels();
    };

    /** Airspeed Indicator */
    class ASI
    {
    public:
        ASI( QGraphicsScene *scene );

        void init( float scaleX, float scaleY );
        void update( float scaleX, float scaleY );

        void setAirspeed( float airspeed );
        void setMachNo( float machNo );
        void setAirspeedSel( double airspeed );
        void setVfe( double vfe );
        void setVne( double vne );

    private:

        QGraphicsScene *_scene;             ///< graphics scene

        QGraphicsSvgItem  *_itemBack;       ///<
        QGraphicsSvgItem  *_itemScale1;     ///<
        QGraphicsSvgItem  *_itemScale2;     ///<
        QGraphicsTextItem *_itemLabel1;     ///<
        QGraphicsTextItem *_itemLabel2;     ///<
        QGraphicsTextItem *_itemLabel3;     ///<
        QGraphicsTextItem *_itemLabel4;     ///<
        QGraphicsTextItem *_itemLabel5;     ///<
        QGraphicsTextItem *_itemLabel6;     ///<
        QGraphicsTextItem *_itemLabel7;     ///<
        QGraphicsSvgItem  *_itemBugIAS;     ///<
        QGraphicsSvgItem  *_itemFrame;      ///<
        QGraphicsRectItem *_itemVfe;        ///<
        QGraphicsSvgItem  *_itemVne;        ///<
        QGraphicsTextItem *_itemAirspeed;   ///<
        QGraphicsTextItem *_itemMachNo;     ///<
        QGraphicsTextItem *_itemSetpoint;   ///<

        QBrush _vfeBrush;                   ///<
        QPen _vfePen;                       ///<

        float _airspeed;                    ///<
        float _machNo;                      ///<
        float _airspeed_sel;                ///<
        float _vfe;                         ///<
        float _vne;                         ///<

        float _scale1DeltaY_new;            ///<
        float _scale1DeltaY_old;            ///<
        float _scale2DeltaY_new;            ///<
        float _scale2DeltaY_old;            ///<
        float _labelsDeltaY_new;            ///<
        float _labelsDeltaY_old;            ///<
        float _bugDeltaY_new;               ///<
        float _bugDeltaY_old;               ///<
        float _vneDeltaY_new;               ///<
        float _vneDeltaY_old;               ///<

        float _scaleX;                      ///<
        float _scaleY;                      ///<

        const float _originalPixPerSpd;     ///< [px/airspeed unit]
        const float _originalScaleHeight;   ///< [px]
        const float _originalLabelsX;       ///< [px]
        const float _originalLabel1Y;       ///< [px]
        const float _originalLabel2Y;       ///< [px]
        const float _originalLabel3Y;       ///< [px]
        const float _originalLabel4Y;       ///< [px]
        const float _originalLabel5Y;       ///< [px]
        const float _originalLabel6Y;       ///< [px]
        const float _originalLabel7Y;       ///< [px]
        const float _originalVfeWidth;

        QPointF _originalBackPos;           ///<
        QPointF _originalScale1Pos;         ///<
        QPointF _originalScale2Pos;         ///<
        QPointF _originalFramePos;          ///<
        QPointF _originalAirspeedCtr;       ///<
        QPointF _originalMachNoCtr;         ///<
        QPointF _originalAirspeedSet;       ///<
        QPointF _originalVfePos;            ///<

        const int _backZ;                   ///<
        const int _scaleZ;                  ///<
        const int _labelsZ;                 ///<
        const int _iasBugZ;                 ///<
        const int _iasVfeZ;                 ///<
        const int _iasVneZ;                 ///<
        const int _frameZ;                  ///<
        const int _frameTextZ;              ///<

        void reset();

        void updateAirspeed();
        void updateAirspeedBug();
        void updateScale();
        void updateScaleLabels();
        void updateVfe();
        void updateVne();
    };

    /** Heading Scale */
    class HDG
    {
    public:
        HDG( QGraphicsScene *scene );

        void init( float scaleX, float scaleY );
        void update( float scaleX, float scaleY );

        void setHeading( float heading );
        void setHeadingSel( float heading );

    private:

        QGraphicsScene *_scene;             ///< graphics scene

        QGraphicsSvgItem  *_itemBack;       ///<
        QGraphicsSvgItem  *_itemFace;       ///< heading face
        QGraphicsSvgItem  *_itemHdgBug;     ///<
        QGraphicsSvgItem  *_itemMarks;      ///< HSI markings
        QGraphicsTextItem *_itemFrameText;  ///<

        float _heading;                     ///< [deg]
        float _heading_sel;                 ///< [deg]

        float _scaleX;                      ///<
        float _scaleY;                      ///<

        QPointF _originalHsiCtr;            ///<
        QPointF _originalBackPos;           ///<
        QPointF _originalFacePos;           ///<
        QPointF _originalBugPos;            ///<
        QPointF _originalMarksPos;          ///<
        QPointF _originalFrameTextCtr;      ///<

        const int _backZ;                   ///<
        const int _faceZ;                   ///<
        const int _hdgBugZ;                 ///<
        const int _marksZ;                  ///<
        const int _frameTextZ;              ///<

        void reset();

        void updateHeading();
    };

    /** Vertical Speed Indicator */
    class VSI
    {
    public:
        VSI( QGraphicsScene *scene );

        void init( float scaleX, float scaleY );
        void update( float scaleX, float scaleY );

        void setClimbRate( float climbRate );

    private:

        QGraphicsScene *_scene;             ///< graphics scene

        QGraphicsSvgItem  *_itemScale;      ///< climb rate scale
        QGraphicsRectItem *_itemMarker;     ///<

        float _climbRate;                   ///<

        float _scaleX;                      ///<
        float _scaleY;                      ///<

        const float _originalMarkerWidth;
        const float _originalPixPerSpd1;    ///< [px/vertical speed unit] up to 100 vsu
        const float _originalPixPerSpd2;    ///< [px/vertical speed unit] from 100 to 200 vsu
        const float _originalPixPerSpd4;    ///< [px/vertical speed unit] from 200 to 400 vsu

        QPointF _originalScalePos;          ///<
        QPointF _originalMarkerPos;         ///<

        QBrush _markerBrush;                ///<

        QPen _markerPen;                    ///<

        const int _scaleZ;                  ///<
        const int _markerZ;                 ///<

        void reset();

        void updateVSI();
    };
};

////////////////////////////////////////////////////////////////////////////////

#endif // GRAPHICSEADI_H
