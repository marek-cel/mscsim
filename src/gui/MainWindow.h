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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

////////////////////////////////////////////////////////////////////////////////

#include <QElapsedTimer>
#include <QMainWindow>
#include <QSettings>
#include <QShortcut>
#include <QTimer>

#include "DialogConf.h"
#include "DialogEnvr.h"
#include "DialogInit.h"
#include "DialogMass.h"

#include "DockWidgetCtrl.h"
#include "DockWidgetData.h"
#include "DockWidgetEFIS.h"
#include "DockWidgetProp.h"
#include "DockWidgetMain.h"

////////////////////////////////////////////////////////////////////////////////

namespace Ui
{
    class MainWindow;
}

////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Main window class.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    /** Constructor. */
    explicit MainWindow( QWidget *parent = 0 );

    /** Destructor. */
    virtual ~MainWindow();

    /** */
    void init();

protected:

    /** */
    void closeEvent( QCloseEvent *event );

    /** */
    void timerEvent( QTimerEvent *event );

private:

    Ui::MainWindow *m_ui;

    QDateTime m_dateTime;           ///< current date and time (initial date and time incremented every step)
    QTime m_flightTime;             ///< flight time

    DialogConf *m_dialogConf;       ///< settings dialog
    DialogEnvr *m_dialogEnvr;       ///< environment conditions dialog
    DialogInit *m_dialogInit;       ///< initial conditions dialog
    DialogMass *m_dialogMass;       ///< mass configuration dialog

    DockWidgetCtrl *m_dockCtrl;     ///<
    DockWidgetData *m_dockData;     ///<
    DockWidgetEFIS *m_dockEFIS;     ///<
    DockWidgetMain *m_dockMain;     ///<
    DockWidgetProp *m_dockProp;     ///<

    QShortcut *m_scTimeFaster;      ///<
    QShortcut *m_scTimeSlower;      ///<

    bool m_viewOrbit;               ///<
    bool m_viewPilot;               ///<
    bool m_viewWorld;               ///<
    bool m_showHUD;                 ///<

    double m_timeCoef;              ///<

    int m_timerId;                  ///< timer ID

    fdm::DataInp::PhaseInp m_phaseInp; ///< simulation input phase
    fdm::DataOut::StateOut m_stateOut; ///< simulation output state

    void setPhaseIdle();
    void setPhaseInit();
    void setPhaseWork();
    void setPhasePause();
    void setPhaseStop();

    void setSideDockAreasFullHeight( bool left, bool rght );

    void settingsRead();
    void settingsRead_Airport( QSettings &settings );
    void settingsRead_View( QSettings &settings );

    void settingsSave();
    void settingsSave_Airport( QSettings &settings );
    void settingsSave_View( QSettings &settings );

    void updateDateTime();

    void updateDockCtrl();
    void updateDockData();
    void updateDockEFIS();
    void updateDockMain();
    void updateDockProp();

    void updateMenu();
    void updateStatusBar();
    void updateWidgetCGI();

    void updateOutputData();

private slots:

    void on_actionDialogConf_triggered();
    void on_actionDialogEnvr_triggered();
    void on_actionDialogInit_triggered();
    void on_actionDialogMass_triggered();

    void on_actionDockCtrl_triggered();
    void on_actionDockData_triggered();
    void on_actionDockEFIS_triggered();
    void on_actionDockMain_triggered();
    void on_actionDockProp_triggered();

    void on_actionPhaseInpIdle_triggered();
    void on_actionPhaseInpInit_triggered();
    void on_actionPhaseInpWork_triggered();
    void on_actionPhaseInpPause_triggered();
    void on_actionPhaseInpStop_triggered();

    void on_actionQuit_triggered();

    void on_actionViewOrbit_triggered();
    void on_actionViewPilot_triggered();
    void on_actionViewWorld_triggered();

    void on_actionShowHUD_triggered( bool checked );

    void on_actionTimeFaster_triggered();
    void on_actionTimeSlower_triggered();

    void dialogInit_typeIndexChanged( int typeIndex );
    void dockMain_phaseInpChanged( fdm::DataInp::PhaseInp phaseInp );
};

////////////////////////////////////////////////////////////////////////////////

#endif // MAINWINDOW_H
