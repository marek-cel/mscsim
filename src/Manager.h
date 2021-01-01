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
#ifndef MANAGER_H
#define MANAGER_H

////////////////////////////////////////////////////////////////////////////////

#include <QObject>

#include <g1000/sim/g1000_IFD.h>
#include <gui/MainWindow.h>
#include <nav/nav_Manager.h>
#include <sfx/sfx_Thread.h>

#include <Autopilot.h>
#include <Simulation.h>

////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Simulation manager class.
 */
class Manager : public QObject
{
    Q_OBJECT

public:

    Manager();

    /** @brief Destructor. */
    virtual ~Manager();

    void init();

signals:

    void dataInpUpdated( const Data::DataBuf *data );

protected:

    void timerEvent( QTimerEvent *event );

private:

    Autopilot    *_ap;          ///< autopilot
    nav::Manager *_nav;         ///< navigation
    sfx::Thread  *_sfx;         ///< SFX
    Simulation   *_sim;         ///< simulation
    MainWindow   *_win;         ///< GUI

    g1000::IFD *_g1000_ifd;     ///< G1000 Integrated Flight Deck
    g1000::Input _g1000_input;  ///< G1000 Integrated Flight Deck input data

    QElapsedTimer *_timerSim;   ///< elapsed timer
    QElapsedTimer *_timerOut;   ///< elapsed timer

    double _timeStep;           ///< [s] time step

    int _timerId;               ///< timer Id

    void updatedInputG1000();
    void updatedInputG1000( const fdm::DataOut &dataOut );

private slots:

    void onDataOutUpdated( const fdm::DataOut &dataOut );
};

////////////////////////////////////////////////////////////////////////////////

#endif // MANAGER_H
