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

////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Stick graphics view class.
 */
class GraphicsStick : public QGraphicsView
{
    Q_OBJECT

public:

    /** Constructor. */
    explicit GraphicsStick( QWidget *parent = 0 );

    /** Destructor. */
    virtual ~GraphicsStick();

    /** */
    void reinit();

    /** */
    inline void setCtrl( int ctrlRoll, int ctrlPitch )
    {
        m_ctrlRoll  = ctrlRoll;
        m_ctrlPitch = ctrlPitch;
    }

    /** */
    inline void setTrim( int trimRoll, int trimPitch )
    {
        m_trimRoll  = trimRoll;
        m_trimPitch = trimPitch;
    }

protected:

    /** */
    void resizeEvent( QResizeEvent *event );

    /** */
    void timerEvent( QTimerEvent *event );

private:

    int m_timerId;

    QGraphicsScene *m_scene;    ///< graphics scene

    QGraphicsLineItem *m_ctrlLineH; ///<
    QGraphicsLineItem *m_ctrlLineV; ///<
    QGraphicsLineItem *m_trimLineH; ///<
    QGraphicsLineItem *m_trimLineV; ///<
    QGraphicsLineItem *m_markLineH; ///<
    QGraphicsLineItem *m_markLineV; ///<

    QBrush m_ctrlBrush; ///<
    QBrush m_trimBrush; ///<
    QBrush m_markBrush; ///<

    QPen m_ctrlPen;     ///<
    QPen m_trimPen;     ///<
    QPen m_markPen;     ///<

    int m_ctrlRoll;     ///< [-]
    int m_ctrlPitch;    ///< [-]

    int m_trimRoll;     ///< [-]
    int m_trimPitch;    ///< [-]

    /** */
    void init();

    /** */
    void reset();

    /** */
    void updateView();
};

////////////////////////////////////////////////////////////////////////////////

#endif // GRAPHICSSTICK_H
