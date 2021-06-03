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
#ifndef SIMULATION_H
#define SIMULATION_H

////////////////////////////////////////////////////////////////////////////////

#include <QThread>
#include <QTimer>
#include <QElapsedTimer>

#include <fdm/fdm_Manager.h>

#include <Data.h>
#include <defs.h>

////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Simulation class.
 */
class Simulation : public QThread
{
    Q_OBJECT

public:

    /** Constructor. */
    Simulation();

    /** Destructor. */
    virtual ~Simulation();

    /** */
    void init();

    /** */
    void run();

public slots:

    /** */
    void onDataInpUpdated( const Data::DataBuf *data );

signals:

    /** */
    void dataOutUpdated( const fdm::DataOut &dataOut );

protected:

    /** */
    void timerEvent( QTimerEvent *event );

private:

    QTimer        *_timeoutTimer;   ///<
    QElapsedTimer *_elapsedTimer;   ///<

    fdm::Manager *_fdm;             ///< flight dynamics model

    fdm::DataInp _dataInp;          ///< flight dynamics model input data
    fdm::DataOut _dataOut;          ///< flight dynamics model output data

    double _timeStep;               ///<
    double _timeCoef;               ///<

    int _timerId;                   ///<

private slots:

    void update();
};

////////////////////////////////////////////////////////////////////////////////

#endif // SIMULATION_H
