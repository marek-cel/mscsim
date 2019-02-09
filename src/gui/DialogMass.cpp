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

#include <gui/DialogMass.h>
#include <ui_DialogMass.h>

#include <gui/Aircrafts.h>
#include <gui/gui_Defines.h>

////////////////////////////////////////////////////////////////////////////////

DialogMass::DialogMass( QWidget *parent ) :
    QDialog ( parent ),
    m_ui ( new Ui::DialogMass ),

    m_type ( -1 ),

    m_pilot  ( 0.0 ),
    m_pilotL ( 0.0 ),
    m_pilotR ( 0.0 ),
    m_pilotF ( 0.0 ),
    m_pilotA ( 0.0 ),
    m_fuel   ( 0.0 ),
    m_fuelL  ( 0.0 ),
    m_fuelR  ( 0.0 ),
    m_fuelF  ( 0.0 ),
    m_fuelA  ( 0.0 ),
    m_cabin  ( 0.0 ),
    m_trunk  ( 0.0 )
{
    m_ui->setupUi( this );

    settingsRead();
}

////////////////////////////////////////////////////////////////////////////////

DialogMass::~DialogMass()
{
    settingsSave();

    if ( m_ui ) delete m_ui;
    m_ui = 0;
}

////////////////////////////////////////////////////////////////////////////////

void DialogMass::readData()
{
    m_ui->spinBoxPilot  ->setValue( m_ui->comboBoxPilot  ->convert( m_pilot  ) );
    m_ui->spinBoxPilotL ->setValue( m_ui->comboBoxPilotL ->convert( m_pilotL ) );
    m_ui->spinBoxPilotR ->setValue( m_ui->comboBoxPilotR ->convert( m_pilotR ) );
    m_ui->spinBoxPilotF ->setValue( m_ui->comboBoxPilotF ->convert( m_pilotF ) );
    m_ui->spinBoxPilotA ->setValue( m_ui->comboBoxPilotA ->convert( m_pilotA ) );

    m_ui->spinBoxFuel  ->setValue( m_ui->comboBoxFuel  ->convert( m_fuel  ) );
    m_ui->spinBoxFuelL ->setValue( m_ui->comboBoxFuelL ->convert( m_fuelL ) );
    m_ui->spinBoxFuelR ->setValue( m_ui->comboBoxFuelR ->convert( m_fuelR ) );
    m_ui->spinBoxFuelF ->setValue( m_ui->comboBoxFuelF ->convert( m_fuelF ) );
    m_ui->spinBoxFuelA ->setValue( m_ui->comboBoxFuelA ->convert( m_fuelA ) );

    m_ui->spinBoxCabin->setValue( m_ui->comboBoxCabin->convert( m_cabin ) );
    m_ui->spinBoxTrunk->setValue( m_ui->comboBoxTrunk->convert( m_trunk ) );
}

////////////////////////////////////////////////////////////////////////////////

void DialogMass::saveData()
{
    m_pilot  = m_ui->comboBoxPilot  ->invert( m_ui->spinBoxPilot  ->value() );
    m_pilotL = m_ui->comboBoxPilotL ->invert( m_ui->spinBoxPilotL ->value() );
    m_pilotR = m_ui->comboBoxPilotR ->invert( m_ui->spinBoxPilotR ->value() );
    m_pilotF = m_ui->comboBoxPilotF ->invert( m_ui->spinBoxPilotF ->value() );
    m_pilotA = m_ui->comboBoxPilotA ->invert( m_ui->spinBoxPilotA ->value() );

    m_fuel  = m_ui->comboBoxFuel  ->invert( m_ui->spinBoxFuel  ->value() );
    m_fuelL = m_ui->comboBoxFuelL ->invert( m_ui->spinBoxFuelL ->value() );
    m_fuelR = m_ui->comboBoxFuelR ->invert( m_ui->spinBoxFuelR ->value() );
    m_fuelF = m_ui->comboBoxFuelF ->invert( m_ui->spinBoxFuelF ->value() );
    m_fuelA = m_ui->comboBoxFuelA ->invert( m_ui->spinBoxFuelA ->value() );

    m_cabin = m_ui->comboBoxCabin->invert( m_ui->spinBoxCabin->value() );
    m_trunk = m_ui->comboBoxTrunk->invert( m_ui->spinBoxTrunk->value() );
}

////////////////////////////////////////////////////////////////////////////////

void DialogMass::setAircraftType( int type )
{
    Aircrafts::Masses masses = Aircrafts::instance()->getAircraft( type ).masses;

    if ( m_type != type )
    {
        m_type = type;

        m_pilot  = masses.pilot.def;
        m_pilotL = masses.pilotL.def;
        m_pilotR = masses.pilotR.def;
        m_pilotF = masses.pilotF.def;
        m_pilotA = masses.pilotA.def;

        m_fuel  = masses.fuel.def;
        m_fuelL = masses.fuelL.def;
        m_fuelR = masses.fuelR.def;
        m_fuelF = masses.fuelF.def;
        m_fuelA = masses.fuelA.def;

        m_cabin = masses.cabin.def;
        m_trunk = masses.trunk.def;
    }

    m_ui->spinBoxPilot  ->setMaximum( m_ui->comboBoxPilot  ->convert( masses.pilot.max  ) );
    m_ui->spinBoxPilotL ->setMaximum( m_ui->comboBoxPilotL ->convert( masses.pilotL.max ) );
    m_ui->spinBoxPilotR ->setMaximum( m_ui->comboBoxPilotR ->convert( masses.pilotR.max ) );
    m_ui->spinBoxPilotF ->setMaximum( m_ui->comboBoxPilotF ->convert( masses.pilotF.max ) );
    m_ui->spinBoxPilotA ->setMaximum( m_ui->comboBoxPilotA ->convert( masses.pilotA.max ) );

    m_ui->spinBoxFuel  ->setMaximum( m_ui->comboBoxFuel  ->convert( masses.fuel.max  ) );
    m_ui->spinBoxFuelL ->setMaximum( m_ui->comboBoxFuelL ->convert( masses.fuelL.max ) );
    m_ui->spinBoxFuelR ->setMaximum( m_ui->comboBoxFuelR ->convert( masses.fuelR.max ) );
    m_ui->spinBoxFuelF ->setMaximum( m_ui->comboBoxFuelF ->convert( masses.fuelF.max ) );
    m_ui->spinBoxFuelA ->setMaximum( m_ui->comboBoxFuelA ->convert( masses.fuelA.max ) );

    m_ui->spinBoxCabin->setMaximum( m_ui->comboBoxCabin->convert( masses.cabin.max ) );
    m_ui->spinBoxTrunk->setMaximum( m_ui->comboBoxTrunk->convert( masses.trunk.max ) );

    m_ui->labelPilot  ->setVisible( masses.pilot.enabled  );
    m_ui->labelPilotL ->setVisible( masses.pilotL.enabled );
    m_ui->labelPilotR ->setVisible( masses.pilotR.enabled );
    m_ui->labelPilotF ->setVisible( masses.pilotF.enabled );
    m_ui->labelPilotA ->setVisible( masses.pilotA.enabled );

    m_ui->labelFuel  ->setVisible( masses.fuel.enabled  );
    m_ui->labelFuelL ->setVisible( masses.fuelL.enabled );
    m_ui->labelFuelR ->setVisible( masses.fuelR.enabled );
    m_ui->labelFuelF ->setVisible( masses.fuelF.enabled );
    m_ui->labelFuelA ->setVisible( masses.fuelA.enabled );

    m_ui->labelCabin->setVisible( masses.cabin.enabled );
    m_ui->labelTrunk->setVisible( masses.trunk.enabled );

    m_ui->spinBoxPilot  ->setVisible( masses.pilot.enabled  );
    m_ui->spinBoxPilotL ->setVisible( masses.pilotL.enabled );
    m_ui->spinBoxPilotR ->setVisible( masses.pilotR.enabled );
    m_ui->spinBoxPilotF ->setVisible( masses.pilotF.enabled );
    m_ui->spinBoxPilotA ->setVisible( masses.pilotA.enabled );

    m_ui->spinBoxFuel  ->setVisible( masses.fuel.enabled  );
    m_ui->spinBoxFuelL ->setVisible( masses.fuelL.enabled );
    m_ui->spinBoxFuelR ->setVisible( masses.fuelR.enabled );
    m_ui->spinBoxFuelF ->setVisible( masses.fuelF.enabled );
    m_ui->spinBoxFuelA ->setVisible( masses.fuelA.enabled );

    m_ui->spinBoxCabin->setVisible( masses.cabin.enabled );
    m_ui->spinBoxTrunk->setVisible( masses.trunk.enabled );

    m_ui->comboBoxPilot  ->setVisible( masses.pilot.enabled  );
    m_ui->comboBoxPilotL ->setVisible( masses.pilotL.enabled );
    m_ui->comboBoxPilotR ->setVisible( masses.pilotR.enabled );
    m_ui->comboBoxPilotF ->setVisible( masses.pilotF.enabled );
    m_ui->comboBoxPilotA ->setVisible( masses.pilotA.enabled );

    m_ui->comboBoxFuel  ->setVisible( masses.fuel.enabled  );
    m_ui->comboBoxFuelL ->setVisible( masses.fuelL.enabled );
    m_ui->comboBoxFuelR ->setVisible( masses.fuelR.enabled );
    m_ui->comboBoxFuelF ->setVisible( masses.fuelF.enabled );
    m_ui->comboBoxFuelA ->setVisible( masses.fuelA.enabled );

    m_ui->comboBoxCabin->setVisible( masses.cabin.enabled );
    m_ui->comboBoxTrunk->setVisible( masses.trunk.enabled );

    readData();
}

////////////////////////////////////////////////////////////////////////////////

void DialogMass::settingsRead()
{
    QSettings settings( GUI_ORG_NAME, GUI_APP_NAME );

    settings.beginGroup( "dialog_mass" );

    restoreGeometry( settings.value( "geometry" ).toByteArray() );

    m_type = settings.value( "aircraft_type", -1 ).toInt();

    settingsRead_MassData( settings );
    settingsRead_UnitCombos( settings );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DialogMass::settingsRead_MassData( QSettings &settings )
{
    settings.beginGroup( "mass_data" );

    m_pilot  = settings.value( "pilot"   , 0.0 ).toDouble();
    m_pilotL = settings.value( "pilot_l" , 0.0 ).toDouble();
    m_pilotR = settings.value( "pilot_r" , 0.0 ).toDouble();
    m_pilotF = settings.value( "pilot_f" , 0.0 ).toDouble();
    m_pilotA = settings.value( "pilot_a" , 0.0 ).toDouble();

    m_fuel  = settings.value( "fuel"   , 0.0 ).toDouble();
    m_fuelL = settings.value( "fuel_l" , 0.0 ).toDouble();
    m_fuelR = settings.value( "fuel_r" , 0.0 ).toDouble();
    m_fuelF = settings.value( "fuel_f" , 0.0 ).toDouble();
    m_fuelA = settings.value( "fuel_a" , 0.0 ).toDouble();

    m_cabin = settings.value( "cabin", 0.0 ).toDouble();
    m_trunk = settings.value( "trunk", 0.0 ).toDouble();

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DialogMass::settingsRead_UnitCombos( QSettings &settings )
{
    settings.beginGroup( "unit_combos" );

    m_ui->comboBoxPilot  ->setCurrentIndex( settings.value( "pilot"   , 0 ).toInt() );
    m_ui->comboBoxPilotL ->setCurrentIndex( settings.value( "pilot_l" , 0 ).toInt() );
    m_ui->comboBoxPilotR ->setCurrentIndex( settings.value( "pilot_r" , 0 ).toInt() );
    m_ui->comboBoxPilotF ->setCurrentIndex( settings.value( "pilot_f" , 0 ).toInt() );
    m_ui->comboBoxPilotA ->setCurrentIndex( settings.value( "pilot_a" , 0 ).toInt() );

    m_ui->comboBoxFuel  ->setCurrentIndex( settings.value( "fuel"   , 0 ).toInt() );
    m_ui->comboBoxFuelL ->setCurrentIndex( settings.value( "fuel_l" , 0 ).toInt() );
    m_ui->comboBoxFuelR ->setCurrentIndex( settings.value( "fuel_r" , 0 ).toInt() );
    m_ui->comboBoxFuelF ->setCurrentIndex( settings.value( "fuel_f" , 0 ).toInt() );
    m_ui->comboBoxFuelA ->setCurrentIndex( settings.value( "fuel_a" , 0 ).toInt() );

    m_ui->comboBoxCabin->setCurrentIndex( settings.value( "cabin", 0 ).toInt() );
    m_ui->comboBoxTrunk->setCurrentIndex( settings.value( "trunk", 0 ).toInt() );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DialogMass::settingsSave()
{
    QSettings settings( GUI_ORG_NAME, GUI_APP_NAME );

    settings.beginGroup( "dialog_mass" );

    settings.setValue( "geometry" , saveGeometry() );

    settings.setValue( "aircraft_type", m_type );

    settingsSave_MassData( settings );
    settingsSave_UnitCombos( settings );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DialogMass::settingsSave_MassData( QSettings &settings )
{
    settings.beginGroup( "mass_data" );

    settings.setValue( "pilot"   , m_pilot  );
    settings.setValue( "pilot_l" , m_pilotL );
    settings.setValue( "pilot_r" , m_pilotR );
    settings.setValue( "pilot_f" , m_pilotF );
    settings.setValue( "pilot_a" , m_pilotA );

    settings.setValue( "fuel"   , m_fuel  );
    settings.setValue( "fuel_l" , m_fuelL );
    settings.setValue( "fuel_r" , m_fuelR );
    settings.setValue( "fuel_f" , m_fuelF );
    settings.setValue( "fuel_a" , m_fuelA );

    settings.setValue( "cabin", m_cabin );
    settings.setValue( "trunk", m_trunk );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DialogMass::settingsSave_UnitCombos( QSettings &settings )
{
    settings.beginGroup( "unit_combos" );

    settings.setValue( "pilot"   , m_ui->comboBoxPilot->currentIndex()  );
    settings.setValue( "pilot_l" , m_ui->comboBoxPilotL->currentIndex() );
    settings.setValue( "pilot_r" , m_ui->comboBoxPilotR->currentIndex() );
    settings.setValue( "pilot_f" , m_ui->comboBoxPilotF->currentIndex() );
    settings.setValue( "pilot_a" , m_ui->comboBoxPilotA->currentIndex() );

    settings.setValue( "fuel"   , m_ui->comboBoxFuel->currentIndex()  );
    settings.setValue( "fuel_l" , m_ui->comboBoxFuelL->currentIndex() );
    settings.setValue( "fuel_r" , m_ui->comboBoxFuelR->currentIndex() );
    settings.setValue( "fuel_f" , m_ui->comboBoxFuelF->currentIndex() );
    settings.setValue( "fuel_a" , m_ui->comboBoxFuelA->currentIndex() );

    settings.setValue( "cabin", m_ui->comboBoxCabin->currentIndex() );
    settings.setValue( "trunk", m_ui->comboBoxTrunk->currentIndex() );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DialogMass::on_comboBoxPilot_currentIndexChanged( int )
{
    double mass_kg = m_ui->comboBoxPilot->invertPrev( m_ui->spinBoxPilot->value() );

    Aircrafts::Masses::Mass mass = Aircrafts::instance()->getAircraft( m_type ).masses.pilot;
    m_ui->spinBoxPilot->setMaximum( m_ui->comboBoxPilot->convert( mass.max ) );
    m_ui->spinBoxPilot->setValue( m_ui->comboBoxPilot->convert( mass_kg ) );
}

////////////////////////////////////////////////////////////////////////////////

void DialogMass::on_comboBoxPilotL_currentIndexChanged( int )
{
    double mass_kg = m_ui->comboBoxPilotL->invertPrev( m_ui->spinBoxPilotL->value() );

    Aircrafts::Masses::Mass mass = Aircrafts::instance()->getAircraft( m_type ).masses.pilotL;
    m_ui->spinBoxPilotL->setMaximum( m_ui->comboBoxPilotL->convert( mass.max ) );
    m_ui->spinBoxPilotL->setValue( m_ui->comboBoxPilotL->convert( mass_kg ) );
}

////////////////////////////////////////////////////////////////////////////////

void DialogMass::on_comboBoxPilotR_currentIndexChanged( int )
{
    double mass_kg = m_ui->comboBoxPilotR->invertPrev( m_ui->spinBoxPilotR->value() );

    Aircrafts::Masses::Mass mass = Aircrafts::instance()->getAircraft( m_type ).masses.pilotR;
    m_ui->spinBoxPilotR->setMaximum( m_ui->comboBoxPilotR->convert( mass.max ) );
    m_ui->spinBoxPilotR->setValue( m_ui->comboBoxPilotR->convert( mass_kg ) );
}

////////////////////////////////////////////////////////////////////////////////

void DialogMass::on_comboBoxPilotF_currentIndexChanged( int )
{
    double mass_kg = m_ui->comboBoxPilotF->invertPrev( m_ui->spinBoxPilotF->value() );

    Aircrafts::Masses::Mass mass = Aircrafts::instance()->getAircraft( m_type ).masses.pilotF;
    m_ui->spinBoxPilotF->setMaximum( m_ui->comboBoxPilotF->convert( mass.max ) );
    m_ui->spinBoxPilotF->setValue( m_ui->comboBoxPilotF->convert( mass_kg ) );
}

////////////////////////////////////////////////////////////////////////////////

void DialogMass::on_comboBoxPilotA_currentIndexChanged( int )
{
    double mass_kg = m_ui->comboBoxPilotA->invertPrev( m_ui->spinBoxPilotA->value() );

    Aircrafts::Masses::Mass mass = Aircrafts::instance()->getAircraft( m_type ).masses.pilotA;
    m_ui->spinBoxPilotA->setMaximum( m_ui->comboBoxPilotA->convert( mass.max ) );
    m_ui->spinBoxPilotA->setValue( m_ui->comboBoxPilotA->convert( mass_kg ) );
}

////////////////////////////////////////////////////////////////////////////////

void DialogMass::on_comboBoxFuel_currentIndexChanged( int )
{
    double mass_kg = m_ui->comboBoxFuel->invertPrev( m_ui->spinBoxFuel->value() );

    Aircrafts::Masses::Mass mass = Aircrafts::instance()->getAircraft( m_type ).masses.fuel;
    m_ui->spinBoxFuel->setMaximum( m_ui->comboBoxFuel->convert( mass.max ) );
    m_ui->spinBoxFuel->setValue( m_ui->comboBoxFuel->convert( mass_kg ) );
}

////////////////////////////////////////////////////////////////////////////////

void DialogMass::on_comboBoxFuelL_currentIndexChanged( int )
{
    double mass_kg = m_ui->comboBoxFuelL->invertPrev( m_ui->spinBoxFuelL->value() );

    Aircrafts::Masses::Mass mass = Aircrafts::instance()->getAircraft( m_type ).masses.fuelL;
    m_ui->spinBoxFuelL->setMaximum( m_ui->comboBoxFuelL->convert( mass.max ) );
    m_ui->spinBoxFuelL->setValue( m_ui->comboBoxFuelL->convert( mass_kg ) );
}

////////////////////////////////////////////////////////////////////////////////

void DialogMass::on_comboBoxFuelR_currentIndexChanged( int )
{
    double mass_kg = m_ui->comboBoxFuelR->invertPrev( m_ui->spinBoxFuelR->value() );

    Aircrafts::Masses::Mass mass = Aircrafts::instance()->getAircraft( m_type ).masses.fuelR;
    m_ui->spinBoxFuelR->setMaximum( m_ui->comboBoxFuelR->convert( mass.max ) );
    m_ui->spinBoxFuelR->setValue( m_ui->comboBoxFuelR->convert( mass_kg ) );
}

////////////////////////////////////////////////////////////////////////////////

void DialogMass::on_comboBoxFuelF_currentIndexChanged( int )
{
    double mass_kg = m_ui->comboBoxFuelF->invertPrev( m_ui->spinBoxFuelF->value() );

    Aircrafts::Masses::Mass mass = Aircrafts::instance()->getAircraft( m_type ).masses.fuelF;
    m_ui->spinBoxFuelF->setMaximum( m_ui->comboBoxFuelF->convert( mass.max ) );
    m_ui->spinBoxFuelF->setValue( m_ui->comboBoxFuelF->convert( mass_kg ) );
}

////////////////////////////////////////////////////////////////////////////////

void DialogMass::on_comboBoxFuelA_currentIndexChanged( int )
{
    double mass_kg = m_ui->comboBoxFuelA->invertPrev( m_ui->spinBoxFuelA->value() );

    Aircrafts::Masses::Mass mass = Aircrafts::instance()->getAircraft( m_type ).masses.fuelA;
    m_ui->spinBoxFuelA->setMaximum( m_ui->comboBoxFuelA->convert( mass.max ) );
    m_ui->spinBoxFuelA->setValue( m_ui->comboBoxFuelA->convert( mass_kg ) );
}

////////////////////////////////////////////////////////////////////////////////

void DialogMass::on_comboBoxCabin_currentIndexChanged( int )
{
    double mass_kg = m_ui->comboBoxCabin->invertPrev( m_ui->spinBoxCabin->value() );

    Aircrafts::Masses::Mass mass = Aircrafts::instance()->getAircraft( m_type ).masses.cabin;
    m_ui->spinBoxCabin->setMaximum( m_ui->comboBoxCabin->convert( mass.max ) );
    m_ui->spinBoxCabin->setValue( m_ui->comboBoxCabin->convert( mass_kg ) );
}

////////////////////////////////////////////////////////////////////////////////

void DialogMass::on_comboBoxTrunk_currentIndexChanged( int )
{
    double mass_kg = m_ui->comboBoxTrunk->invertPrev( m_ui->spinBoxTrunk->value() );

    Aircrafts::Masses::Mass mass = Aircrafts::instance()->getAircraft( m_type ).masses.trunk;
    m_ui->spinBoxTrunk->setMaximum( m_ui->comboBoxTrunk->convert( mass.max ) );
    m_ui->spinBoxTrunk->setValue( m_ui->comboBoxTrunk->convert( mass_kg ) );
}
