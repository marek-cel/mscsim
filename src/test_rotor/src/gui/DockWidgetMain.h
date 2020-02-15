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
#ifndef DOCKWIDGETMAIN_H
#define DOCKWIDGETMAIN_H

////////////////////////////////////////////////////////////////////////////////

#include <QDockWidget>

#include <QElapsedTimer>

#include <fdm_test/fdm_Test.h>

////////////////////////////////////////////////////////////////////////////////

namespace Ui
{
    class DockWidgetMain;
}

////////////////////////////////////////////////////////////////////////////////

class DockWidgetMain : public QDockWidget
{
    Q_OBJECT

public:

    explicit DockWidgetMain( QWidget *parent = nullptr );

    virtual ~DockWidgetMain();

    void update( double timeStep );

private:

    Ui::DockWidgetMain *_ui;        ///<

    fdm::Test *_test;
    bool _ccw;

    void settingsRead();
    void settingsSave();

private slots:

    void on_pushButtonInit_clicked();
    void on_pushButtonWork_clicked();
    void on_pushButtonPause_clicked();
    void on_pushButtonStop_clicked();

    void on_radioButtonCCW_toggled(bool checked);
};

////////////////////////////////////////////////////////////////////////////////

#endif // DOCKWIDGETMAIN_H
