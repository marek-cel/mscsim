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
#ifndef DOCKWIDGETCTRL_H
#define DOCKWIDGETCTRL_H

////////////////////////////////////////////////////////////////////////////////

#include <QDockWidget>

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

    /** Constructor. */
    explicit DockWidgetCtrl( QWidget *parent = 0 );

    /** Destructor. */
    ~DockWidgetCtrl();

    /** */
    void setAircraftType( int typeIndex );

    /** */
    void setCollective( float collective );

    /** */
    void setCtrlStick( float ctrlRoll, float ctrlPitch );

    /** */
    void setTrimStick( float trimRoll, float trimPitch );

    /** */
    void setCtrlPedals( float ctrlYaw );

    /** */
    void setTrimPedals( float trimPitch );

    /** */
    void setBrakes( float brakeL, float brakeR );

    /** */
    void setGear( float gear );

    /** */
    void setFlaps( float flaps );

    /** */
    void setThrottle( float throttle_1,
                      float throttle_2,
                      float throttle_3,
                      float throttle_4 );

    /** */
    void setMixture( float mixture_1,
                     float mixture_2,
                     float mixture_3,
                     float mixture_4 );

    /** */
    void setPropeller( float propeller_1,
                       float propeller_2,
                       float propeller_3,
                       float propeller_4 );

    /** */
    void setAirbrake( float airbrake );

    /** */
    void setSpoilers( float spoilers );

private:

    Ui::DockWidgetCtrl *m_ui;
};

////////////////////////////////////////////////////////////////////////////////

#endif // DOCKWIDGETCTRL_H
