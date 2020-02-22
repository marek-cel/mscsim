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

class DockWidgetCtrl : public QDockWidget
{
    Q_OBJECT

public:

    explicit DockWidgetCtrl( QWidget *parent = nullptr );

    ~DockWidgetCtrl();

    void update( double timeStep );

    double getCollective();
    double getCyclicLat();
    double getCyclicLon();

    double getAzimuth();
    double getOmega();

signals:

    void closed();

protected:

    void closeEvent( QCloseEvent *event );

private:

    Ui::DockWidgetCtrl *_ui;        ///<

    void settingsRead();
    void settingsSave();

private slots:

    void on_sliderCollective_sliderMoved(int position);

    void on_sliderCyclicLon_sliderMoved(int position);

    void on_sliderCyclicLat_sliderMoved(int position);
};

////////////////////////////////////////////////////////////////////////////////

#endif // DOCKWIDGETCTRL_H
