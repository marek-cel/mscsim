/****************************************************************************//*
 * Copyright (C) 2020 Marek M. Cel
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
#ifndef GRAPHICSPEDALS_H
#define GRAPHICSPEDALS_H

////////////////////////////////////////////////////////////////////////////////

#include <QGraphicsLineItem>
#include <QGraphicsView>

#include "gui_Defines.h"

////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Pedals graphics view class.
 */
class GraphicsPedals : public QGraphicsView
{
    Q_OBJECT

public:

    /** Constructor. */
    explicit GraphicsPedals( QWidget *parent = NULLPTR );

    /** Destructor. */
    virtual ~GraphicsPedals();

    /** */
    void reinit();

    /** */
    inline void setCtrl( int ctrlYaw )
    {
        _ctrlYaw  = ctrlYaw;
    }

    /** */
    inline void setTrim( int trimYaw )
    {
        _trimYaw  = trimYaw;
    }

protected:

    /** */
    void resizeEvent( QResizeEvent *event );

    /** */
    void timerEvent( QTimerEvent *event );

private:

    int _timerId;

    QGraphicsScene *_scene;         ///< graphics scene

    QGraphicsLineItem *_ctrlLineV;  ///<
    QGraphicsLineItem *_trimLineV;  ///<
    QGraphicsLineItem *_markLineH;  ///<
    QGraphicsLineItem *_markLineV;  ///<

    QBrush _ctrlBrush;              ///<
    QBrush _trimBrush;              ///<
    QBrush _markBrush;              ///<

    QPen _ctrlPen;                  ///<
    QPen _trimPen;                  ///<
    QPen _markPen;                  ///<

    int _ctrlYaw;                   ///< [-]
    int _trimYaw;                   ///< [-]

    /** */
    void init();

    /** */
    void reset();

    /** */
    void updateView();
};

////////////////////////////////////////////////////////////////////////////////

#endif // GRAPHICSPEDALS_H
