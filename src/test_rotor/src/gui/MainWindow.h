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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

////////////////////////////////////////////////////////////////////////////////

#include <QMainWindow>

#include <gui/DialogCtrl.h>

#include <gui/DockWidgetCtrl.h>
#include <gui/DockWidgetData.h>
#include <gui/DockWidgetMain.h>
#include <gui/DockWidgetTest.h>

////////////////////////////////////////////////////////////////////////////////

namespace Ui
{
    class MainWindow;
}

////////////////////////////////////////////////////////////////////////////////

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow( QWidget *parent = nullptr );

    ~MainWindow();

protected:

    /** */
    void keyPressEvent( QKeyEvent *event );

    /** */
    void keyReleaseEvent( QKeyEvent *event );

    /** */
    void timerEvent( QTimerEvent *event );

private:

    Ui::MainWindow *_ui;

    DialogCtrl *_dialogCtrl;            ///<

    DockWidgetCtrl *_dockCtrl;          ///<
    DockWidgetData *_dockData;          ///<
    DockWidgetMain *_dockMain;          ///<
    DockWidgetTest *_dockTest;          ///<

    QElapsedTimer *_timer;
    double _timeStep;

    int _timerId;

    void settingsRead();
    void settingsSave();

    void updateDataBlades();

private slots:

    void on_actionControls_triggered();

    void on_actionDockCtrl_toggled( bool checked );
    void on_actionDockData_toggled( bool checked );
    void on_actionDockMain_toggled( bool checked );
    void on_actionDockTest_toggled( bool checked );

    void on_actionViewAft_triggered();
    void on_actionViewFwd_triggered();
    void on_actionViewLft_triggered();
    void on_actionViewRgt_triggered();
    void on_actionViewTop_triggered();

    void on_actionRotationStart_triggered();
    void on_actionRotationStop_triggered();

    void on_actionShowVectorsSpan_toggled(bool arg1);
    void on_actionShowVectorsMain_toggled(bool arg1);
    void on_actionShowBladesDatum_toggled(bool arg1);
    void on_actionShowBladesTrace_toggled(bool arg1);

    void dockCtrl_closed();
    void dockData_closed();
    void dockMain_closed();
    void dockTest_closed();
};

////////////////////////////////////////////////////////////////////////////////

#endif // MAINWINDOW_H
