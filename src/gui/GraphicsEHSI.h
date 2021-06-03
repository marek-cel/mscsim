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
#ifndef GRAPHICSEHSI_H
#define GRAPHICSEHSI_H

////////////////////////////////////////////////////////////////////////////////

#include <QGraphicsView>
#include <QGraphicsSvgItem>

#include <defs.h>

////////////////////////////////////////////////////////////////////////////////

/**
 * @brief This class provides EHSI widget.
 */
class GraphicsEHSI : public QGraphicsView
{
    Q_OBJECT

public:

    /** Course Deviation Indicator (CDI). */
    enum class CDI
    {
        Off = 0,
        TO,
        FROM
    };

    /** @brief Constructor. */
    explicit GraphicsEHSI( QWidget *parent = NULLPTR );

    /** @brief Destructor. */
    virtual ~GraphicsEHSI();

    /** */
    void reinit();

    /** @param heading [deg] */
    void setHeading( double heading );

    /** @param course [deg] */
    void setCourse( double course );

    /** @param bearing [deg] */
    void setBearing( double bearing, bool visible = false );

    /** @param deviation [-] */
    void setDeviation( double deviation, CDI cdi = CDI::Off );

    /** @param distance [nm] */
    void setDistance( double distance, bool visible = false );

    /** @param heading [deg] */
    void setHeadingSel( double heading );

protected:

    /** */
    void resizeEvent( QResizeEvent *event );

    /** */
    void timerEvent( QTimerEvent *event );

private:

    int _timerId;

    QGraphicsScene *_scene;             ///< graphics scene

    QGraphicsSvgItem *_itemBack;        ///< NAV background
    QGraphicsSvgItem *_itemMask;        ///< NAV mask
    QGraphicsSvgItem *_itemMark;        ///<

    QGraphicsSvgItem *_itemBrgArrow;    ///<
    QGraphicsSvgItem *_itemCrsArrow;    ///<
    QGraphicsSvgItem *_itemDevBar;      ///<
    QGraphicsSvgItem *_itemDevScale;    ///<
    QGraphicsSvgItem *_itemHdgBug;      ///<
    QGraphicsSvgItem *_itemHdgScale;    ///<
    QGraphicsSvgItem *_itemCdiTo;       ///<
    QGraphicsSvgItem *_itemCdiFrom;     ///<

    QGraphicsTextItem *_itemCrsText;    ///<
    QGraphicsTextItem *_itemHdgText;    ///<
    QGraphicsTextItem *_itemDmeText;    ///<

    double _heading;                    ///< [deg]
    double _course;                     ///<
    double _bearing;                    ///<
    double _deviation;                  ///<
    double _distance;                   ///<

    double _heading_sel;                ///< [deg]

    CDI _cdi;                           ///<

    bool _bearingVisible;               ///<
    bool _distanceVisible;              ///<

    double _devBarDeltaX_new;           ///<
    double _devBarDeltaX_old;           ///<
    double _devBarDeltaY_new;           ///<
    double _devBarDeltaY_old;           ///<

    double _scaleX;                     ///<
    double _scaleY;                     ///<

    double _originalPixPerDev;          ///<

    QPointF _originalNavCtr;            ///<

    QPointF _originalCrsTextCtr;        ///<
    QPointF _originalHdgTextCtr;        ///<
    QPointF _originalDmeTextCtr;        ///<

    const int _originalHeight;          ///< [px]
    const int _originalWidth;           ///< [px]

    const int _backZ;                   ///<
    const int _maskZ;                   ///<
    const int _markZ;                   ///<

    const int _brgArrowZ;               ///<
    const int _crsArrowZ;               ///<
    const int _crsTextZ;                ///<
    const int _devBarZ;                 ///<
    const int _devScaleZ;               ///<
    const int _hdgBugZ;                 ///<
    const int _hdgScaleZ;               ///<
    const int _hdgTextZ;                ///<
    const int _dmeTextZ;                ///<

    /** */
    void init();

    /** */
    void reset();

    /** */
    void updateView();
};

////////////////////////////////////////////////////////////////////////////////

#endif // GRAPHICSEHSI_H
