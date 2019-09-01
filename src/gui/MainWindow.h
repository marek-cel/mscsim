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

#include <Data.h>

#include "DialogConf.h"
#include "DialogEnvr.h"
#include "DialogInit.h"
#include "DialogMass.h"

#include "DockWidgetAuto.h"
#include "DockWidgetCtrl.h"
#include "DockWidgetData.h"
#include "DockWidgetEFIS.h"
#include "DockWidgetMain.h"
#include "DockWidgetMap.h"
#include "DockWidgetProp.h"

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
    void keyPressEvent( QKeyEvent *event );

    /** */
    void keyReleaseEvent( QKeyEvent *event );

    /** */
    void closeEvent( QCloseEvent *event );

    /** */
    void timerEvent( QTimerEvent *event );

private:

    typedef Data::Camera::ViewType ViewType;

    Ui::MainWindow *_ui;                ///<

    QDateTime _dateTime;                ///< current date and time (initial date and time incremented every step)
    QTime _flightTime;                  ///< flight time

    DialogConf *_dialogConf;            ///< settings dialog
    DialogEnvr *_dialogEnvr;            ///< environment conditions dialog
    DialogInit *_dialogInit;            ///< initial conditions dialog
    DialogMass *_dialogMass;            ///< mass configuration dialog

    DockWidgetAuto *_dockAuto;          ///<
    DockWidgetCtrl *_dockCtrl;          ///<
    DockWidgetData *_dockData;          ///<
    DockWidgetEFIS *_dockEFIS;          ///<
    DockWidgetMain *_dockMain;          ///<
    DockWidgetMap  *_dockMap;           ///<
    DockWidgetProp *_dockProp;          ///<

    QShortcut *_scFullScreen;           ///<
    QShortcut *_scTimeFaster;           ///<
    QShortcut *_scTimeSlower;           ///<

    ViewType _viewType;                 ///<
    bool _showHUD;                      ///<

    double _timeCoef;                   ///<

    int _timerId;                       ///< timer ID

    int _typeIndex;                     ///<

    fdm::DataInp::PhaseInp _phaseInp;   ///< simulation input phase
    fdm::DataOut::StateOut _stateOut;   ///< simulation output state

    hid::Assignment::Key getKey( int key );

    void setPhaseIdle();
    void setPhaseInit();
    void setPhaseWork();
    void setPhasePause();
    void setPhaseStop();

    void setAircraftType( int typeIndex );

    void setSideDockAreasFullHeight( bool left, bool rght );
    void setWidescreenDockLayout( bool enabled );

    void settingsRead();
    void settingsRead_Airport( QSettings &settings );
    void settingsRead_View( QSettings &settings );

    void settingsSave();
    void settingsSave_Airport( QSettings &settings );
    void settingsSave_View( QSettings &settings );

    void updateDateTime();

    void updateDockAuto();
    void updateDockCtrl();
    void updateDockData();
    void updateDockEFIS();
    void updateDockMain();
    void updateDockProp();

    void updateMenu();
    void updateStatusBar();

    void updateOutputData();

private slots:

    void on_actionDialogConf_triggered();
    void on_actionDialogEnvr_triggered();
    void on_actionDialogInit_triggered();
    void on_actionDialogMass_triggered();

    void on_actionDockAuto_toggled( bool checked );
    void on_actionDockCtrl_toggled( bool checked );
    void on_actionDockData_toggled( bool checked );
    void on_actionDockEFIS_toggled( bool checked );
    void on_actionDockMain_toggled( bool checked );
    void on_actionDockMap_toggled( bool checked );
    void on_actionDockProp_toggled( bool checked );

    void on_actionPhaseInpIdle_triggered();
    void on_actionPhaseInpInit_triggered();
    void on_actionPhaseInpWork_triggered();
    void on_actionPhaseInpPause_triggered();
    void on_actionPhaseInpStop_triggered();

    void on_actionQuit_triggered();

    void on_actionViewChase_triggered();
    void on_actionViewOrbit_triggered();
    void on_actionViewPilot_triggered();
    void on_actionViewWorld_triggered();

    void on_actionShowHUD_triggered( bool checked );

    void on_actionTimeFaster_triggered();
    void on_actionTimeSlower_triggered();

    void on_shorcutFullScreen_triggered();

    void dialogInit_typeIndexChanged( int typeIndex );
    void dockMain_phaseInpChanged( fdm::DataInp::PhaseInp phaseInp );

    void dockAuto_closed();
    void dockCtrl_closed();
    void dockData_closed();
    void dockEFIS_closed();
    void dockMain_closed();
    void dockMap_closed();
    void dockProp_closed();
};

////////////////////////////////////////////////////////////////////////////////

#endif // MAINWINDOW_H
