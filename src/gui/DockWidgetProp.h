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
#ifndef DOCKWIDGETPROP_H
#define DOCKWIDGETPROP_H

////////////////////////////////////////////////////////////////////////////////

#include <QDockWidget>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QSettings>

#include <fdm/fdm_Defines.h>

#include <Defines.h>

////////////////////////////////////////////////////////////////////////////////

namespace Ui
{
    class DockWidgetProp;
}

////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Propulsion dock widget class.
 */
class DockWidgetProp : public QDockWidget
{
    Q_OBJECT

public:

    /** Constructor. */
    explicit DockWidgetProp( QWidget *parent = NULLPTR );

    /** Destructor. */
    ~DockWidgetProp();

    bool getFuel     ( int index ) const;
    bool getIgnition ( int index ) const;
    bool getStarter  ( int index ) const;

    /** */
    void setAircraftType( int typeIndex );

    void setState( unsigned int index, bool working );
    void setAB( unsigned int index, bool afterburner );

    void setRPM  ( unsigned int index, float val );
    void setPROP ( unsigned int index, float val );
    void setNG   ( unsigned int index, float val );
    void setN1   ( unsigned int index, float val );
    void setN2   ( unsigned int index, float val );
    void setTRQ  ( unsigned int index, float val );
    void setEPR  ( unsigned int index, float val );
    void setMAP  ( unsigned int index, float val );
    void setEGT  ( unsigned int index, float val );
    void setCHT  ( unsigned int index, float val );
    void setTIT  ( unsigned int index, float val );
    void setTOT  ( unsigned int index, float val );
    void setITT  ( unsigned int index, float val );
    void setFF   ( unsigned int index, float val );

signals:

    void closed();

protected:

    void closeEvent( QCloseEvent *event );

private:

    Ui::DockWidgetProp *_ui;        ///<

    QLabel *_label[ FDM_MAX_ENGINES ];

    QPushButton *_pushButtonState    [ FDM_MAX_ENGINES ];
    QPushButton *_pushButtonAB       [ FDM_MAX_ENGINES ];
    QPushButton *_pushButtonFuel     [ FDM_MAX_ENGINES ];
    QPushButton *_pushButtonIgnition [ FDM_MAX_ENGINES ];
    QPushButton *_pushButtonStart    [ FDM_MAX_ENGINES ];

    QDoubleSpinBox *_spinRPM  [ FDM_MAX_ENGINES ];
    QDoubleSpinBox *_spinPROP [ FDM_MAX_ENGINES ];
    QDoubleSpinBox *_spinNG   [ FDM_MAX_ENGINES ];
    QDoubleSpinBox *_spinN1   [ FDM_MAX_ENGINES ];
    QDoubleSpinBox *_spinN2   [ FDM_MAX_ENGINES ];
    QDoubleSpinBox *_spinTRQ  [ FDM_MAX_ENGINES ];
    QDoubleSpinBox *_spinEPR  [ FDM_MAX_ENGINES ];
    QDoubleSpinBox *_spinMAP  [ FDM_MAX_ENGINES ];
    QDoubleSpinBox *_spinEGT  [ FDM_MAX_ENGINES ];
    QDoubleSpinBox *_spinCHT  [ FDM_MAX_ENGINES ];
    QDoubleSpinBox *_spinTIT  [ FDM_MAX_ENGINES ];
    QDoubleSpinBox *_spinTOT  [ FDM_MAX_ENGINES ];
    QDoubleSpinBox *_spinITT  [ FDM_MAX_ENGINES ];
    QDoubleSpinBox *_spinFF   [ FDM_MAX_ENGINES ];

    void settingsRead();
    void settingsRead_UnitCombos( QSettings &settings );

    void settingsSave();
    void settingsSave_UnitCombos( QSettings &settings );
};

////////////////////////////////////////////////////////////////////////////////

#endif // DOCKWIDGETPROP_H
