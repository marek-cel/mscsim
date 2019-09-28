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
#ifndef GRAPHICSSTICK_H
#define GRAPHICSSTICK_H

////////////////////////////////////////////////////////////////////////////////

#include <QGraphicsLineItem>
#include <QGraphicsView>

#include "gui_Defines.h"

////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Stick graphics view class.
 */
class GraphicsStick : public QGraphicsView
{
    Q_OBJECT

public:

    /** Constructor. */
    explicit GraphicsStick( QWidget *parent = NULLPTR );

    /** Destructor. */
    virtual ~GraphicsStick();

    /** */
    void reinit();

    /** */
    inline void setCtrl( int ctrlRoll, int ctrlPitch )
    {
        _ctrlRoll  = ctrlRoll;
        _ctrlPitch = ctrlPitch;
    }

    /** */
    inline void setTrim( int trimRoll, int trimPitch )
    {
        _trimRoll  = trimRoll;
        _trimPitch = trimPitch;
    }

protected:

    /** */
    void resizeEvent( QResizeEvent *event );

    /** */
    void timerEvent( QTimerEvent *event );

private:

    int _timerId;                   ///<

    QGraphicsScene *_scene;         ///< graphics scene

    QGraphicsLineItem *_ctrlLineH;  ///<
    QGraphicsLineItem *_ctrlLineV;  ///<
    QGraphicsLineItem *_trimLineH;  ///<
    QGraphicsLineItem *_trimLineV;  ///<
    QGraphicsLineItem *_markLineH;  ///<
    QGraphicsLineItem *_markLineV;  ///<

    QBrush _ctrlBrush;              ///<
    QBrush _trimBrush;              ///<
    QBrush _markBrush;              ///<

    QPen _ctrlPen;                  ///<
    QPen _trimPen;                  ///<
    QPen _markPen;                  ///<

    int _ctrlRoll;                  ///< [-]
    int _ctrlPitch;                 ///< [-]

    int _trimRoll;                  ///< [-]
    int _trimPitch;                 ///< [-]

    /** */
    void init();

    /** */
    void reset();

    /** */
    void updateView();
};

////////////////////////////////////////////////////////////////////////////////

#endif // GRAPHICSSTICK_H
