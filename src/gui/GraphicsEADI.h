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
#ifndef GRAPHICSEADI_H
#define GRAPHICSEADI_H

////////////////////////////////////////////////////////////////////////////////

#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QGraphicsSvgItem>

////////////////////////////////////////////////////////////////////////////////

/**
 * @brief This class provides EADI widget.
 */
class GraphicsEADI : public QGraphicsView
{
    Q_OBJECT

public:

    /** Constructor. */
    explicit GraphicsEADI( QWidget *parent = 0 );

    /** Destructor. */
    virtual ~GraphicsEADI();

    /** */
    void reinit();

    /** @param roll angle [deg] */
    inline void setRoll( float roll )
    {
        m_adi->setRoll( roll );
    }

    /** @param pitch angle [deg] */
    inline void setPitch( float pitch )
    {
        m_adi->setPitch( pitch );
    }

    /** @param normalized slip or skid (range from -1.0 to 1.0) */
    inline void setSlipSkid( float slipSkid )
    {
        m_adi->setSlipSkid( slipSkid );
    }

    /**
     * @param normalized turn rate (range from -1.0 to 1.0),
     * hash marks positions are set to be -0.5 and 0.5 */
    inline void setTurnRate( float turnRate )
    {
        m_adi->setTurnRate( turnRate );
    }

    /**
     * @param normalized horizontal deviation bar position (range from -1.0 to 1.0)
     * @param normalized vertical deviation bar position (range from -1.0 to 1.0)
     * @param deviation bars visibility */
    inline void setBars( float barH, float barV, bool visible = true )
    {
        m_adi->setBars( barH, barV, visible );
    }

    /**
     * @param normalized horizontal deviation dot position (range from -1.0 to 1.0)
     * @param normalized vertical deviation dot position (range from -1.0 to 1.0)
     * @param deviation horizontal dot visibility
     * @param deviation vertical dot visibility */
    inline void setDots( float dotH, float dotV, bool visibleH, bool visibleV )
    {
        m_adi->setDots( dotH, dotV, visibleH, visibleV );
    }

    /** @param altitude (dimensionless numeric value) */
    inline void setAltitude( float altitude )
    {
        m_alt->setAltitude( altitude );
    }

    /** @param airspeed (dimensionless numeric value) */
    inline void setAirspeed( float airspeed )
    {
        m_asi->setAirspeed( airspeed );
    }

    /** @param Mach number */
    inline void setMachNo( float machNo )
    {
        m_asi->setMachNo( machNo );
    }

    /** @param heading [deg] */
    inline void setHeading( float heading )
    {
        m_hdg->setHeading( heading );
    }

    /** @param climb rate (dimensionless numeric value)  */
    inline void setClimbRate( float climbRate )
    {
        m_vsi->setClimbRate( climbRate );
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

    int m_timerId;

    QGraphicsScene *m_scene;        ///< graphics scene

    GraphicsEADI::ADI *m_adi;
    GraphicsEADI::ALT *m_alt;
    GraphicsEADI::ASI *m_asi;
    GraphicsEADI::HDG *m_hdg;
    GraphicsEADI::VSI *m_vsi;

    QGraphicsSvgItem *m_itemBack;   ///< PFD background
    QGraphicsSvgItem *m_itemMask;   ///< PFD mask

    float m_scaleX; ///<
    float m_scaleY; ///<

    const int m_originalHeight;     ///< [px]
    const int m_originalWidth;      ///< [px]

    const int m_backZ;
    const int m_maskZ;

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
        void setBars( float barH, float barV, bool visible = true );
        void setDots( float dotH, float dotV, bool visibleH, bool visibleV );

    private:
        QGraphicsScene *m_scene;            ///< graphics scene

        QGraphicsSvgItem  *m_itemBack;      ///< background
        QGraphicsSvgItem  *m_itemLadd;      ///< pitch ladder
        QGraphicsSvgItem  *m_itemRoll;      ///< roll mask
        QGraphicsSvgItem  *m_itemSlip;      ///< slip indicator
        QGraphicsSvgItem  *m_itemTurn;      ///< turn rate indicator
        QGraphicsSvgItem  *m_itemBarH;
        QGraphicsSvgItem  *m_itemBarV;
        QGraphicsSvgItem  *m_itemDotH;
        QGraphicsSvgItem  *m_itemDotV;
        QGraphicsSvgItem  *m_itemMask;      ///< adi mask
        QGraphicsSvgItem  *m_itemScaleH;
        QGraphicsSvgItem  *m_itemScaleV;

        float m_roll;           ///< [deg]
        float m_pitch;          ///< [deg]
        float m_slipSkid;       ///< -1.0 ... 1.0
        float m_turnRate;       ///< -1.0 ... 1.0
        float m_barH;
        float m_barV;
        float m_dotH;
        float m_dotV;

        bool m_barsVisible;
        bool m_dotVisibleH;
        bool m_dotVisibleV;

        float m_laddDeltaX_new;
        float m_laddDeltaX_old;
        float m_laddBackDeltaX_new;
        float m_laddBackDeltaX_old;
        float m_laddBackDeltaY_new;
        float m_laddBackDeltaY_old;
        float m_laddDeltaY_new;
        float m_laddDeltaY_old;
        float m_slipDeltaX_new;
        float m_slipDeltaX_old;
        float m_slipDeltaY_new;
        float m_slipDeltaY_old;
        float m_turnDeltaX_new;
        float m_turnDeltaX_old;
        float m_barHDeltaX_new;
        float m_barHDeltaX_old;
        float m_barVDeltaY_new;
        float m_barVDeltaY_old;
        float m_dotHDeltaX_new;
        float m_dotHDeltaX_old;
        float m_dotVDeltaY_new;
        float m_dotVDeltaY_old;

        float m_scaleX;
        float m_scaleY;

        const float m_originalPixPerDeg;    ///< [px/deg] pixels to move pitch ladder due to 1 deg pitch
        const float m_deltaLaddBack_max;    ///< [px] max pitch ladder background deflection
        const float m_deltaLaddBack_min;    ///< [px] min pitch ladder background deflection
        const float m_maxSlipDeflection;    ///< [px] max slip indicator deflection
        const float m_maxTurnDeflection;    ///< [px] max turn indicator deflection
        const float m_maxBarsDeflection;
        const float m_maxDotsDeflection;

        QPointF m_originalAdiCtr;
        QPointF m_originalBackPos;
        QPointF m_originalLaddPos;
        QPointF m_originalRollPos;
        QPointF m_originalSlipPos;
        QPointF m_originalTurnPos;
        QPointF m_originalBarHPos;
        QPointF m_originalBarVPos;
        QPointF m_originalDotHPos;
        QPointF m_originalDotVPos;
        QPointF m_originalScaleHPos;
        QPointF m_originalScaleVPos;

        const int m_backZ;
        const int m_laddZ;
        const int m_rollZ;
        const int m_slipZ;
        const int m_barsZ;
        const int m_dotsZ;
        const int m_scalesZ;
        const int m_maskZ;
        const int m_turnZ;

        void reset();

        void updateLadd( float delta, float sinRoll, float cosRoll );
        void updateLaddBack( float delta, float sinRoll, float cosRoll );
        void updateRoll();
        void updateSlipSkid( float sinRoll, float cosRoll );
        void updateTurnRate();
        void updateBars();
        void updateDots();
    };

    /** Altimeter */
    class ALT
    {
    public:
        ALT( QGraphicsScene *scene );

        void init( float scaleX, float scaleY );
        void update( float scaleX, float scaleY );

        void setAltitude( float altitude );

    private:

        QGraphicsScene *m_scene;    ///< graphics scene

        QGraphicsSvgItem  *m_itemBack;
        QGraphicsSvgItem  *m_itemScale1;
        QGraphicsSvgItem  *m_itemScale2;
        QGraphicsTextItem *m_itemLabel1;
        QGraphicsTextItem *m_itemLabel2;
        QGraphicsTextItem *m_itemLabel3;
        QGraphicsSvgItem  *m_itemGround;
        QGraphicsSvgItem  *m_itemFrame;
        QGraphicsTextItem *m_itemAltitude;
        QGraphicsTextItem *m_itemPressure;

        QColor m_frameTextColor;
        QColor m_pressTextColor;
        QColor m_labelsColor;

        QFont  m_frameTextFont;
        QFont  m_labelsFont;

        float m_altitude;

        float m_scale1DeltaY_new;
        float m_scale1DeltaY_old;
        float m_scale2DeltaY_new;
        float m_scale2DeltaY_old;
        float m_groundDeltaY_new;
        float m_groundDeltaY_old;
        float m_labelsDeltaY_new;
        float m_labelsDeltaY_old;

        float m_scaleX;     ///<
        float m_scaleY;     ///<

        const float m_originalPixPerAlt;    ///< [px/altitude unit]
        const float m_originalScaleHeight;  ///< [px]
        const float m_originalLabelsX;      ///< [px]
        const float m_originalLabel1Y;      ///< [px]
        const float m_originalLabel2Y;      ///< [px]
        const float m_originalLabel3Y;      ///< [px]

        QPointF m_originalBackPos;
        QPointF m_originalScale1Pos;
        QPointF m_originalScale2Pos;
        QPointF m_originalGroundPos;
        QPointF m_originalFramePos;
        QPointF m_originalAltitudeCtr;
        QPointF m_originalPressureCtr;

        const int m_backZ;
        const int m_scaleZ;
        const int m_labelsZ;
        const int m_groundZ;
        const int m_frameZ;
        const int m_frameTextZ;

        void reset();

        void updateAltitude();
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

    private:

        QGraphicsScene *m_scene;    ///< graphics scene

        QGraphicsSvgItem  *m_itemBack;
        QGraphicsSvgItem  *m_itemScale1;
        QGraphicsSvgItem  *m_itemScale2;
        QGraphicsTextItem *m_itemLabel1;
        QGraphicsTextItem *m_itemLabel2;
        QGraphicsTextItem *m_itemLabel3;
        QGraphicsTextItem *m_itemLabel4;
        QGraphicsTextItem *m_itemLabel5;
        QGraphicsTextItem *m_itemLabel6;
        QGraphicsTextItem *m_itemLabel7;
        QGraphicsSvgItem  *m_itemFrame;
        QGraphicsTextItem *m_itemAirspeed;
        QGraphicsTextItem *m_itemMachNo;

        QColor m_frameTextColor;
        QColor m_labelsColor;

        QFont  m_frameTextFont;
        QFont  m_labelsFont;

        float m_airspeed;
        float m_machNo;

        float m_scale1DeltaY_new;
        float m_scale1DeltaY_old;
        float m_scale2DeltaY_new;
        float m_scale2DeltaY_old;
        float m_labelsDeltaY_new;
        float m_labelsDeltaY_old;

        float m_scaleX;
        float m_scaleY;

        const float m_originalPixPerSpd;    ///< [px/airspeed unit]
        const float m_originalScaleHeight;  ///< [px]
        const float m_originalLabelsX;      ///< [px]
        const float m_originalLabel1Y;      ///< [px]
        const float m_originalLabel2Y;      ///< [px]
        const float m_originalLabel3Y;      ///< [px]
        const float m_originalLabel4Y;      ///< [px]
        const float m_originalLabel5Y;      ///< [px]
        const float m_originalLabel6Y;      ///< [px]
        const float m_originalLabel7Y;      ///< [px]

        QPointF m_originalBackPos;
        QPointF m_originalScale1Pos;
        QPointF m_originalScale2Pos;
        QPointF m_originalFramePos;
        QPointF m_originalAirspeedCtr;
        QPointF m_originalMachNoCtr;

        const int m_backZ;
        const int m_scaleZ;
        const int m_labelsZ;
        const int m_frameZ;
        const int m_frameTextZ;

        void reset();

        void updateAirspeed();
        void updateScale();
        void updateScaleLabels();
    };

    /** Heading Scale */
    class HDG
    {
    public:
        HDG( QGraphicsScene *scene );

        void init( float scaleX, float scaleY );
        void update( float scaleX, float scaleY );

        void setHeading( float heading );

    private:

        QGraphicsScene *m_scene;    ///< graphics scene

        QGraphicsSvgItem  *m_itemBack;
        QGraphicsSvgItem  *m_itemFace;          ///< heading face
        QGraphicsSvgItem  *m_itemMarks;         ///< HSI markings
        QGraphicsTextItem *m_itemFrameText;     ///<

        QColor m_frameTextColor;

        QFont  m_frameTextFont;

        float m_heading;    ///< [deg]

        float m_scaleX;     ///<
        float m_scaleY;     ///<

        QPointF m_originalHsiCtr;
        QPointF m_originalBackPos;
        QPointF m_originalFacePos;
        QPointF m_originalMarksPos;
        QPointF m_originalFrameTextCtr;

        const int m_backZ;
        const int m_faceZ;
        const int m_marksZ;
        const int m_frameTextZ;

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

        QGraphicsScene *m_scene;            ///< graphics scene

        QGraphicsSvgItem  *m_itemScale;     ///< climb rate scale
        QGraphicsRectItem *m_itemMarker;    ///<

        float m_climbRate;

        float m_scaleX;
        float m_scaleY;

        const float m_originalMarkeWidth;
        const float m_originalPixPerSpd1;   ///< [px/vertical speed unit] up to 100 vsu
        const float m_originalPixPerSpd2;   ///< [px/vertical speed unit] from 100 to 200 vsu
        const float m_originalPixPerSpd4;   ///< [px/vertical speed unit] from 200 to 400 vsu

        QPointF m_originalScalePos;
        QPointF m_originalMarkerPos;

        QBrush m_markerBrush; ///<

        QPen m_markerPen;     ///<

        const int m_scaleZ;
        const int m_markerZ;

        void reset();

        void updateVSI();
    };
};

////////////////////////////////////////////////////////////////////////////////

#endif // GRAPHICSEADI_H
