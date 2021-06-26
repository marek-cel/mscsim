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
#ifndef DOCKWIDGETCTRL_H
#define DOCKWIDGETCTRL_H

////////////////////////////////////////////////////////////////////////////////

#include <QDockWidget>

#include <defs.h>

////////////////////////////////////////////////////////////////////////////////

namespace Ui
{
    class DockWidgetCtrl;
}

////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Controls dock widget class.
 */
class DockWidgetCtrl : public QDockWidget
{
    Q_OBJECT

public:

    /** @brief Constructor. */
    explicit DockWidgetCtrl( QWidget *parent = NULLPTR );

    /** @brief Destructor. */
    virtual ~DockWidgetCtrl();

    bool getABS() const;
    bool getNWS() const;

    /** */
    void setAircraftType( int typeIndex );

    /** */
    void setCollective( double collective );

    /** */
    void setCtrlStick( double ctrlRoll, double ctrlPitch );

    /** */
    void setTrimStick( double trimRoll, double trimPitch );

    /** */
    void setCtrlPedals( double ctrlYaw );

    /** */
    void setTrimPedals( double trimPitch );

    /** */
    void setBrakes( double brakeL, double brakeR );

    /** */
    void setGear( double gear );

    /** */
    void setFlaps( double flaps );

    /** */
    void setThrottle( double throttle_1,
                      double throttle_2,
                      double throttle_3,
                      double throttle_4 );

    /** */
    void setMixture( double mixture_1,
                     double mixture_2,
                     double mixture_3,
                     double mixture_4 );

    /** */
    void setPropeller( double propeller_1,
                       double propeller_2,
                       double propeller_3,
                       double propeller_4 );

    /** */
    void setAirbrake( double airbrake );

    /** */
    void setSpoilers( double spoilers );

signals:

    void closed();

protected:

    void closeEvent( QCloseEvent *event );

private:

    Ui::DockWidgetCtrl *_ui;        ///<
};

////////////////////////////////////////////////////////////////////////////////

#endif // DOCKWIDGETCTRL_H
