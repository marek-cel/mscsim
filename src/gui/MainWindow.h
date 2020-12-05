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

#include <QElapsedTimer>
#include <QMainWindow>
#include <QSettings>
#include <QShortcut>
#include <QTimer>

#include <Data.h>

#include <Autopilot.h>

#include <gui/Aircrafts.h>

#include <gui/DialogConf.h>
#include <gui/DialogEnvr.h>
#include <gui/DialogInit.h>
#include <gui/DialogMass.h>

#include <gui/DockWidgetAuto.h>
#include <gui/DockWidgetCtrl.h>
#include <gui/DockWidgetData.h>
#include <gui/DockWidgetEFIS.h>
#include <gui/DockWidgetMain.h>
#include <gui/DockWidgetMap.h>
#include <gui/DockWidgetProp.h>

#include <gui/WidgetPFD.h>

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

    static const QString _tmp_file;     ///< temporary record file

    /** @brief Constructor. */
    explicit MainWindow( QWidget *parent = NULLPTR );

    /** @brief Destructor. */
    virtual ~MainWindow();

    /** */
    void setup( Autopilot *ap, g1000::IFD *ifd );

    /** */
    void init();

    /**
     * @brief Returns current aircraft data
     * @return current aircraft data
     */
    Aircrafts::Aircraft getCurrentAircraft() const;

    inline bool getABS() const { return _dockCtrl->getABS(); }
    inline bool getNWS() const { return _dockCtrl->getNWS(); }

    inline double getCourse()  const { return _dockAuto->getCourse();  }
    inline double getFreqNav() const { return _dockAuto->getFreqNav(); }

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

    typedef Data::CGI::ViewType ViewType;

    Ui::MainWindow *_ui;                ///< UI object

    Autopilot *_ap;                     ///< autopilot object
    g1000::IFD *_ifd;                   ///< G1000 IFD object

    QDateTime _dateTime;                ///< current date and time (initial date and time incremented every step)
    QTime _flightTime;                  ///< flight time

    DialogConf *_dialogConf;            ///< settings dialog
    DialogEnvr *_dialogEnvr;            ///< environment conditions dialog
    DialogInit *_dialogInit;            ///< initial conditions dialog
    DialogMass *_dialogMass;            ///< mass configuration dialog

    DockWidgetAuto *_dockAuto;          ///< autopilot dock widget
    DockWidgetCtrl *_dockCtrl;          ///< controls dock widget
    DockWidgetData *_dockData;          ///< data dock widget
    DockWidgetEFIS *_dockEFIS;          ///< EFIS dock widget
    DockWidgetMain *_dockMain;          ///< main dock widget
    DockWidgetMap  *_dockMap;           ///< map dock widget
    DockWidgetProp *_dockProp;          ///< propulsion dock widget

    WidgetPFD *_widgetPFD;              ///< PFD widget

    QShortcut *_scCycleViews;           ///< key shortcut
    QShortcut *_scToggleHud;            ///< key shortcut
    QShortcut *_scFullScreen;           ///< key shortcut
    QShortcut *_scTimeFaster;           ///< key shortcut
    QShortcut *_scTimeSlower;           ///< key shortcut
    QShortcut *_scTimeNormal;           ///< key shortcut

    QString _rec_file;                  ///<

    ViewType _viewType;                 ///< specifies view type

    bool _showHUD;                      ///< specifies if HUD is visible
    bool _showTraces;                   ///< specifies if traces are visible

    bool _showRotorBlur;                ///< specifies if rotor blur is visible
    bool _showBladesDatum;              ///< specifies if blades datum plane is visible
    bool _showBladesPaths;              ///< specifies if blades tips paths are visible

    double _timeCoef;                   ///< [-] time compression coefficient

    int _timerId;                       ///< timer ID

    int _typeIndex;                     ///< aircraft type index

    fdm::DataInp::StateInp _stateInp;   ///< simulation input state
    fdm::DataOut::StateOut _stateOut;   ///< simulation output state

    hid::Assignment::Key getKey( int key );

    void flightRecordOpen();
    void flightRecordSave();

    void setStateIdle();
    void setStateInit();
    void setStateWork();
    void setStatePause();
    void setStateStop();

    void setViewChase();
    void setViewOrbit();
    void setViewPilot();
    void setViewWorld();

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

    void on_actionShowPFD_triggered();

    void on_actionFlightOpen_triggered();
    void on_actionFlightSave_triggered();

    void on_actionStateInpIdle_triggered();
    void on_actionStateInpInit_triggered();
    void on_actionStateInpWork_triggered();
    void on_actionStateInpPause_triggered();
    void on_actionStateInpStop_triggered();

    void on_actionQuit_triggered();

    void on_actionViewChase_triggered();
    void on_actionViewOrbit_triggered();
    void on_actionViewPilot_triggered();
    void on_actionViewWorld_triggered();

    void on_actionShowHUD_triggered( bool checked );
    void on_actionShowTraces_triggered( bool checked );

    void on_actionShowRotorBlur_triggered( bool checked );
    void on_actionShowBladesDatum_triggered( bool checked );
    void on_actionShowBladesPaths_triggered( bool checked );

    void on_actionTimeFaster_triggered();
    void on_actionTimeSlower_triggered();
    void on_actionTimeNormal_triggered();

    void shorcutCycleViews_activated();
    void shorcutToggleHud_activated();
    void shorcutFullScreen_activated();

    void dialogInit_typeIndexChanged( int typeIndex );
    void dockMain_stateInpChanged( fdm::DataInp::StateInp stateInp );

    void dockAuto_closed();
    void dockCtrl_closed();
    void dockData_closed();
    void dockEFIS_closed();
    void dockMain_closed();
    void dockMap_closed();
    void dockProp_closed();

    void widgetPFD_closed();
};

////////////////////////////////////////////////////////////////////////////////

#endif // MAINWINDOW_H
