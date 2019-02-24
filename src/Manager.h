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
#ifndef MANAGER_H
#define MANAGER_H

////////////////////////////////////////////////////////////////////////////////

#include <QObject>

#include <gui/MainWindow.h>

#include <Navigation.h>
#include <Simulation.h>

////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Simulation class.
 */
class Manager : public QObject
{
    Q_OBJECT

public:

    Manager();

    virtual ~Manager();

    void init();

signals:

    void dataInpUpdated( const Data::DataBuf *data );

protected:

    void timerEvent( QTimerEvent *event );

private:

    Navigation *m_nav;          ///< navigation
    Simulation *m_sim;          ///< simulation
    MainWindow *m_win;          ///< GUI

    QElapsedTimer *m_timer;     ///< elapsed timer

    int m_timerId;              ///< timer Id

    double m_timeStep;          ///< [s] time step

private slots:

    void onDataOutUpdated( const fdm::DataOut &dataOut );
};

////////////////////////////////////////////////////////////////////////////////

#endif // MANAGER_H
