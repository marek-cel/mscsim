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
#ifndef WIDGETGDU_H
#define WIDGETGDU_H

////////////////////////////////////////////////////////////////////////////////

#include <QWidget>

#include <osg/Node>

#include <Autopilot.h>
#include <defs.h>

////////////////////////////////////////////////////////////////////////////////

namespace Ui
{
    class WidgetGDU;
}

////////////////////////////////////////////////////////////////////////////////

/** Based on GDU-1044B */
class WidgetGDU : public QWidget
{
    Q_OBJECT

public:

    explicit WidgetGDU( QWidget *parent = NULLPTR );

    ~WidgetGDU();

    void update();

    void setSceneRoot( osg::Node *sceneRoot );

    inline void setAutopilot( Autopilot *ap ) { _ap = ap; }

private:

    Ui::WidgetGDU *_ui;

    Autopilot *_ap;             ///< autopilot object

private slots:

    void on_buttonKnob_Vol_NAV_clicked();
    void on_buttonKnob_Vol_COM_clicked();

    void on_buttonKnob_NAV_clicked();
    void on_buttonKnob_COM_clicked();
    void on_buttonKnob_HDG_clicked();
    void on_buttonKnob_CRS_clicked();
    void on_buttonKnob_ALT_clicked();
    void on_buttonKnob_FMS_clicked();

    void on_buttonKey_NAV_clicked();
    void on_buttonKey_COM_clicked();

    void on_buttonKey_AP_AP_clicked();
    void on_buttonKey_AP_FD_clicked();
    void on_buttonKey_AP_HDG_clicked();
    void on_buttonKey_AP_ALT_clicked();
    void on_buttonKey_AP_NAV_clicked();
    void on_buttonKey_AP_VNV_clicked();
    void on_buttonKey_AP_APR_clicked();
    void on_buttonKey_AP_BC_clicked();
    void on_buttonKey_AP_VS_clicked();
    void on_buttonKey_AP_FLC_clicked();

    void on_buttonKey_FMS_DEST_clicked();
    void on_buttonKey_FMS_MENU_clicked();
    void on_buttonKey_FMS_FPL_clicked();
    void on_buttonKey_FMS_PROC_clicked();
    void on_buttonKey_FMS_CLR_clicked();
    void on_buttonKey_FMS_ENT_clicked();

    void on_buttonKeySoft_01_clicked();
    void on_buttonKeySoft_02_clicked();
    void on_buttonKeySoft_03_clicked();
    void on_buttonKeySoft_04_clicked();
    void on_buttonKeySoft_05_clicked();
    void on_buttonKeySoft_06_clicked();
    void on_buttonKeySoft_07_clicked();
    void on_buttonKeySoft_08_clicked();
    void on_buttonKeySoft_09_clicked();
    void on_buttonKeySoft_10_clicked();
    void on_buttonKeySoft_11_clicked();
    void on_buttonKeySoft_12_clicked();
};

////////////////////////////////////////////////////////////////////////////////

#endif // WIDGETGDU_H
