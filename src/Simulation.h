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
#ifndef SIMULATION_H
#define SIMULATION_H

////////////////////////////////////////////////////////////////////////////////

#include <QThread>
#include <QTimer>
#include <QElapsedTimer>

#include <fdm/fdm_Manager.h>

#include <Data.h>

////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Simulation class.
 */
class Simulation : public QThread
{
    Q_OBJECT

public:

    Simulation();

    virtual ~Simulation();

    void init();

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

    QTimer        *m_timeoutTimer;  ///<
    QElapsedTimer *m_elapsedTimer;  ///<

    fdm::Manager *m_fdm;            ///< flight dynamics model

    fdm::DataInp m_dataInp;         ///< flight dynamics model input data
    fdm::DataOut m_dataOut;         ///< flight dynamics model output data

    double m_timeStep;
    double m_timeCoef;

    int m_timerId;

private slots:

    void update();
};

////////////////////////////////////////////////////////////////////////////////

#endif // SIMULATION_H
