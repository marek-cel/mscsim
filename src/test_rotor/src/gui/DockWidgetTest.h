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
#ifndef DOCKWIDGETTEST_H
#define DOCKWIDGETTEST_H

////////////////////////////////////////////////////////////////////////////////

#include <QDockWidget>

////////////////////////////////////////////////////////////////////////////////

namespace Ui
{
    class DockWidgetTest;
}

////////////////////////////////////////////////////////////////////////////////

class DockWidgetTest : public QDockWidget
{
    Q_OBJECT

public:

    explicit DockWidgetTest( QWidget *parent = nullptr );

    ~DockWidgetTest();

    void update( double timeStep );

    void setCCW( bool ccw );

signals:

    void closed();

protected:

    void closeEvent( QCloseEvent *event );

private:

    Ui::DockWidgetTest *_ui;        ///<

    bool _ccw;

private slots:

    void on_sliderTestConing_sliderMoved(int position);

    void on_sliderTestDiskPitch_sliderMoved(int position);

    void on_sliderTestDiskRoll_sliderMoved(int position);

    void on_sliderTestCollective_sliderMoved(int position);

    void on_sliderTestCyclicLon_sliderMoved(int position);

    void on_sliderTestCyclicLat_sliderMoved(int position);

    void on_sliderTestAzimuth_sliderMoved(int position);
};

////////////////////////////////////////////////////////////////////////////////

#endif // DOCKWIDGETTEST_H
