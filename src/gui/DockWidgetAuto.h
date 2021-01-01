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
#ifndef DOCKWIDGETAUTO_H
#define DOCKWIDGETAUTO_H

////////////////////////////////////////////////////////////////////////////////

#include <QDockWidget>

#include <Autopilot.h>
#include <Defines.h>

////////////////////////////////////////////////////////////////////////////////

namespace Ui
{
    class DockWidgetAuto;
}

////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Autopilot dock widget class.
 *
 * Based on Bendix King KMC 321 Mode Controller and KAS 297C Altitude/Vertical
 * Speed Preselect
 */
class DockWidgetAuto : public QDockWidget
{
    Q_OBJECT

public:

    /** @brief Constructor. */
    explicit DockWidgetAuto( QWidget *parent = NULLPTR );

    /** @brief Destructor. */
    virtual ~DockWidgetAuto();

    /** */
    void init();

    double getCourse() const;
    double getFreqNav() const;

    inline void setAutopilot( Autopilot *ap ) { _ap = ap; }

signals:

    void closed();

protected:

    void closeEvent( QCloseEvent *event );

    /** */
    void timerEvent( QTimerEvent *event );

private:

    Ui::DockWidgetAuto *_ui;    ///<

    Autopilot *_ap;             ///< autopilot object

    int _timerId;               ///< timer ID

    double normalizeHeading( double heading );

    void settingsRead();
    void settingsSave();

    void updateIdle();
    void updateWork();

private slots:

    void on_pushButtonAP_clicked();
    void on_pushButtonFD_clicked();

    void on_pushButtonALT_clicked();
    void on_pushButtonIAS_clicked();
    void on_pushButtonENG_clicked();
    void on_pushButtonARM_clicked();

    void on_pushButtonHDG_clicked();
    void on_pushButtonNAV_clicked();
    void on_pushButtonAPR_clicked();
    void on_pushButtonBC_clicked();

    void on_pushButtonYD_clicked();

    void on_pushButtonSoftRide_clicked();
    void on_pushButtonHalfBank_clicked();

    void on_pushButtonTest_pressed();
    void on_pushButtonTest_released();

    void on_pushButtonSetCRS_clicked();
    void on_pushButtonSetHDG_clicked();

    void on_spinBoxCRS_valueChanged( double arg1 );
    void on_spinBoxHDG_valueChanged( double arg1 );

    void on_spinBoxALT_VS_editingFinished();

    void on_pushButtonShowVS_clicked( bool checked );
    void on_pushButtonShowALT_clicked( bool checked );

    void on_spinBoxFreqStandby_editingFinished();
    void on_pushButtonSwitchFreq_clicked();
};

////////////////////////////////////////////////////////////////////////////////

#endif // DOCKWIDGETAUTO_H
