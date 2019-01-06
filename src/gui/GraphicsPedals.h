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
#ifndef GRAPHICSPEDALS_H
#define GRAPHICSPEDALS_H

////////////////////////////////////////////////////////////////////////////////

#include <QGraphicsLineItem>
#include <QGraphicsView>

////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Pedals graphics view class.
 */
class GraphicsPedals : public QGraphicsView
{
    Q_OBJECT

public:

    /** Constructor. */
    explicit GraphicsPedals( QWidget *parent = 0 );

    /** Destructor. */
    virtual ~GraphicsPedals();

    /** */
    void reinit();

    /** */
    inline void setCtrl( int ctrlYaw )
    {
        m_ctrlYaw  = ctrlYaw;
    }

    /** */
    inline void setTrim( int trimYaw )
    {
        m_trimYaw  = trimYaw;
    }

protected:

    /** */
    void resizeEvent( QResizeEvent *event );

    /** */
    void timerEvent( QTimerEvent *event );

private:

    int m_timerId;

    QGraphicsScene *m_scene;    ///< graphics scene

    QGraphicsLineItem *m_ctrlLineV; ///<
    QGraphicsLineItem *m_trimLineV; ///<
    QGraphicsLineItem *m_markLineH; ///<
    QGraphicsLineItem *m_markLineV; ///<

    QBrush m_ctrlBrush; ///<
    QBrush m_trimBrush; ///<
    QBrush m_markBrush; ///<

    QPen m_ctrlPen;     ///<
    QPen m_trimPen;     ///<
    QPen m_markPen;     ///<

    int m_ctrlYaw;      ///< [-]
    int m_trimYaw;      ///< [-]

    /** */
    void init();

    /** */
    void reset();

    /** */
    void updateView();
};

////////////////////////////////////////////////////////////////////////////////

#endif // GRAPHICSPEDALS_H
