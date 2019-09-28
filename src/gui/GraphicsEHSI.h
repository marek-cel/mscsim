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
#ifndef GRAPHICSEHSI_H
#define GRAPHICSEHSI_H

////////////////////////////////////////////////////////////////////////////////

#include <QGraphicsView>
#include <QGraphicsSvgItem>

#include "gui_Defines.h"

////////////////////////////////////////////////////////////////////////////////

/**
 * @brief This class provides EHSI widget.
 */
class GraphicsEHSI : public QGraphicsView
{
    Q_OBJECT

public:

    /** Constructor. */
    explicit GraphicsEHSI( QWidget *parent = NULLPTR );

    /** Destructor. */
    virtual ~GraphicsEHSI();

    /** */
    void reinit();

    /** @param heading [deg] */
    void setHeading( float heading );

    /** @param heading bug [deg] */
    void setHeadingBug( float headingBug );

    /** @param course [deg] */
    void setCourse( float course );

    /** @param bearing [deg] */
    void setBearing( float bearing, bool visible = false );

    /** @param deviation [-] */
    void setDeviation( float deviation, bool visible = false );

    /** @param distance [nm] */
    void setDistance( float distance, bool visible = false );

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
    QGraphicsSvgItem *_itemMark;

    QGraphicsSvgItem *_itemBrgArrow;    ///<
    QGraphicsSvgItem *_itemCrsArrow;    ///<
    QGraphicsSvgItem *_itemDevBar;      ///<
    QGraphicsSvgItem *_itemDevScale;    ///<
    QGraphicsSvgItem *_itemHdgBug;      ///<
    QGraphicsSvgItem *_itemHdgScale;    ///<

    QGraphicsTextItem *_itemCrsText;    ///<
    QGraphicsTextItem *_itemHdgText;    ///<
    QGraphicsTextItem *_itemDmeText;    ///<

    QColor _crsTextColor;               ///<
    QColor _hdgTextColor;               ///<
    QColor _dmeTextColor;               ///<

    QFont  _crsTextFont;                ///<
    QFont  _hdgTextFont;                ///<
    QFont  _dmeTextFont;                ///<

    float _heading;                     ///< [deg]
    float _headingBug;                  ///< [deg]
    float _course;                      ///<
    float _bearing;                     ///<
    float _deviation;                   ///<
    float _distance;                    ///<

    bool _bearingVisible;               ///<
    bool _deviationVisible;             ///<
    bool _distanceVisible;              ///<

    float _devBarDeltaX_new;            ///<
    float _devBarDeltaX_old;            ///<
    float _devBarDeltaY_new;            ///<
    float _devBarDeltaY_old;            ///<

    float _scaleX;                      ///<
    float _scaleY;                      ///<

    float _originalPixPerDev;           ///<

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
