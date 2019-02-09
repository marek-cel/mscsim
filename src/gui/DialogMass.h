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
#ifndef DIALOGMASS_H
#define DIALOGMASS_H

////////////////////////////////////////////////////////////////////////////////

#include <QDialog>
#include <QSettings>

////////////////////////////////////////////////////////////////////////////////

namespace Ui
{
    class DialogMass;
}

////////////////////////////////////////////////////////////////////////////////

/**
 * @brief This is 'Fuel and Payload' dialog class.
 */
class DialogMass : public QDialog
{
    Q_OBJECT

public:

    /** Constructor. */
    explicit DialogMass( QWidget *parent = 0 );
    
    /** Destructor. */
    virtual ~DialogMass();

    void readData();
    void saveData();

    inline double getPilot()  const { return m_pilot;  }
    inline double getPilotL() const { return m_pilotL; }
    inline double getPilotR() const { return m_pilotR; }
    inline double getPilotF() const { return m_pilotF; }
    inline double getPilotA() const { return m_pilotA; }

    inline double getFuel()  const { return m_fuel;  }
    inline double getFuelL() const { return m_fuelL; }
    inline double getFuelR() const { return m_fuelR; }
    inline double getFuelF() const { return m_fuelF; }
    inline double getFuelA() const { return m_fuelA; }

    inline double getCabin()  const { return m_cabin; }
    inline double getTrunk()  const { return m_trunk; }

    void setAircraftType( int type );

private:

    Ui::DialogMass *m_ui;

    int m_type;

    double m_pilot;     ///< [kg]
    double m_pilotL;    ///< [kg]
    double m_pilotR;    ///< [kg]
    double m_pilotF;    ///< [kg]
    double m_pilotA;    ///< [kg]

    double m_fuel;      ///< [kg]
    double m_fuelL;     ///< [kg]
    double m_fuelR;     ///< [kg]
    double m_fuelF;     ///< [kg]
    double m_fuelA;     ///< [kg]

    double m_cabin;     ///< [kg]
    double m_trunk;     ///< [kg]

    void settingsRead();
    void settingsRead_MassData( QSettings &settings );
    void settingsRead_UnitCombos( QSettings &settings );

    void settingsSave();
    void settingsSave_MassData( QSettings &settings );
    void settingsSave_UnitCombos( QSettings &settings );

private slots:

    void on_comboBoxPilot_currentIndexChanged( int index );
    void on_comboBoxPilotL_currentIndexChanged( int index );
    void on_comboBoxPilotR_currentIndexChanged( int index );
    void on_comboBoxPilotF_currentIndexChanged( int index );
    void on_comboBoxPilotA_currentIndexChanged( int index );

    void on_comboBoxFuel_currentIndexChanged( int index );
    void on_comboBoxFuelL_currentIndexChanged( int index );
    void on_comboBoxFuelR_currentIndexChanged( int index );
    void on_comboBoxFuelF_currentIndexChanged( int index );
    void on_comboBoxFuelA_currentIndexChanged( int index );

    void on_comboBoxCabin_currentIndexChanged( int index );
    void on_comboBoxTrunk_currentIndexChanged( int index );
};

////////////////////////////////////////////////////////////////////////////////

#endif // DIALOGMASS_H
