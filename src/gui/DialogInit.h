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
#ifndef DIALOGINIT_H
#define DIALOGINIT_H

////////////////////////////////////////////////////////////////////////////////

#include <QDateTime>
#include <QDialog>
#include <QDomElement>
#include <QSettings>

#include <defs.h>

////////////////////////////////////////////////////////////////////////////////

namespace Ui
{
    class DialogInit;
}

////////////////////////////////////////////////////////////////////////////////

/**
 * @brief This is 'Initial Conditions' dialog class.
 */
class DialogInit : public QDialog
{
    Q_OBJECT

public:

    /** @brief Constructor. */
    explicit DialogInit( QWidget *parent = NULLPTR );
    
    /** @brief Destructor. */
    virtual ~DialogInit();

    inline QDateTime getDateTime() const { return _dateTime; }

    inline int getTypeIndex() const { return _typeIndex; }

    inline double getLat() const { return _lat; }
    inline double getLon() const { return _lon; }
    inline double getAlt() const { return _alt; }
    inline double getPsi() const { return _psi; }
    inline double getIAS() const { return _ias; }

    inline bool getEngine() const { return _engine; }

    void readData();
    void saveData();

signals:

    void typeIndexChanged( int );

private:

    Ui::DialogInit *_ui;    ///<

    QDateTime _dateTime;    ///<

    int _typeIndex;         ///< aircraft type index

    double _lat;            ///< [rad]
    double _lon;            ///< [rad]
    double _alt;            ///< [m] altitude above ground level
    double _psi;            ///< [rad] heading
    double _ias;            ///< [m/s] airspeed

    bool _engine;           ///< specifies if engine is on at start

    void settingsRead();
    void settingsRead_InitData( QSettings &settings );
    void settingsRead_UnitCombos( QSettings &settings );

    void settingsSave();
    void settingsSave_InitData( QSettings &settings );
    void settingsSave_UnitCombos( QSettings &settings );

private slots:

    void on_comboAirports_currentIndexChanged( int index );
    void on_comboLocations_currentIndexChanged( int index );

    void on_spinInitLat_valueChanged( double arg1 );
    void on_spinInitLon_valueChanged( double arg1 );
    void on_spinInitAlt_valueChanged( double arg1 );
    void on_spinInitPsi_valueChanged( double arg1 );

    void on_comboDistance_currentIndexChanged( int index );

    void on_comboInitLat_currentIndexChanged( int index );
    void on_comboInitLon_currentIndexChanged( int index );
    void on_comboInitAlt_currentIndexChanged( int index );
    void on_comboInitPsi_currentIndexChanged( int index );
    void on_comboInitIAS_currentIndexChanged( int index );

    void on_pushButtonTime_clicked();

    void on_checkBoxEngineOn_toggled( bool checked );
};

////////////////////////////////////////////////////////////////////////////////

#endif // DIALOGINIT_H
