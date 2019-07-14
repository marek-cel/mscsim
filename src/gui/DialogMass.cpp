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

    m_pilot_1 ( 0.0 ),
    m_pilot_2 ( 0.0 ),
    m_fuel_tank_1 ( 0.0 ),
    m_fuel_tank_2 ( 0.0 ),
    m_fuel_tank_3 ( 0.0 ),
    m_fuel_tank_4 ( 0.0 ),
    m_cabin ( 0.0 ),
    m_trunk ( 0.0 )
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
    m_ui->spinBoxPilot_1->setValue( m_ui->comboBoxPilot_1->convert( m_pilot_1 ) );
    m_ui->spinBoxPilot_2->setValue( m_ui->comboBoxPilot_2->convert( m_pilot_2 ) );

    m_ui->spinBoxFuelTank_1->setValue( m_ui->comboBoxFuelTank_1->convert( m_fuel_tank_1 ) );
    m_ui->spinBoxFuelTank_2->setValue( m_ui->comboBoxFuelTank_2->convert( m_fuel_tank_2 ) );
    m_ui->spinBoxFuelTank_3->setValue( m_ui->comboBoxFuelTank_3->convert( m_fuel_tank_3 ) );
    m_ui->spinBoxFuelTank_4->setValue( m_ui->comboBoxFuelTank_4->convert( m_fuel_tank_4 ) );

    m_ui->spinBoxCabin->setValue( m_ui->comboBoxCabin->convert( m_cabin ) );
    m_ui->spinBoxTrunk->setValue( m_ui->comboBoxTrunk->convert( m_trunk ) );
}

////////////////////////////////////////////////////////////////////////////////

void DialogMass::saveData()
{
    m_pilot_1 = m_ui->comboBoxPilot_1->invert( m_ui->spinBoxPilot_1->value() );
    m_pilot_2 = m_ui->comboBoxPilot_2->invert( m_ui->spinBoxPilot_2->value() );

    m_fuel_tank_1 = m_ui->comboBoxFuelTank_1->invert( m_ui->spinBoxFuelTank_1->value() );
    m_fuel_tank_2 = m_ui->comboBoxFuelTank_2->invert( m_ui->spinBoxFuelTank_2->value() );
    m_fuel_tank_3 = m_ui->comboBoxFuelTank_3->invert( m_ui->spinBoxFuelTank_3->value() );
    m_fuel_tank_4 = m_ui->comboBoxFuelTank_4->invert( m_ui->spinBoxFuelTank_4->value() );

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

        m_pilot_1 = masses.pilot_1.def;
        m_pilot_2 = masses.pilot_2.def;

        m_fuel_tank_1 = masses.fuel_tank_1.def;
        m_fuel_tank_2 = masses.fuel_tank_2.def;
        m_fuel_tank_3 = masses.fuel_tank_3.def;
        m_fuel_tank_4 = masses.fuel_tank_4.def;

        m_cabin = masses.cabin.def;
        m_trunk = masses.trunk.def;
    }

    m_ui->spinBoxPilot_1->setMaximum( m_ui->comboBoxPilot_1->convert( masses.pilot_1.max ) );
    m_ui->spinBoxPilot_2->setMaximum( m_ui->comboBoxPilot_2->convert( masses.pilot_2.max ) );

    m_ui->spinBoxFuelTank_1->setMaximum( m_ui->comboBoxFuelTank_1->convert( masses.fuel_tank_1.max ) );
    m_ui->spinBoxFuelTank_2->setMaximum( m_ui->comboBoxFuelTank_2->convert( masses.fuel_tank_2.max ) );
    m_ui->spinBoxFuelTank_3->setMaximum( m_ui->comboBoxFuelTank_3->convert( masses.fuel_tank_3.max ) );
    m_ui->spinBoxFuelTank_4->setMaximum( m_ui->comboBoxFuelTank_4->convert( masses.fuel_tank_4.max ) );

    m_ui->spinBoxCabin->setMaximum( m_ui->comboBoxCabin->convert( masses.cabin.max ) );
    m_ui->spinBoxTrunk->setMaximum( m_ui->comboBoxTrunk->convert( masses.trunk.max ) );

    m_ui->labelPilot_1->setVisible( masses.pilot_1.enabled );
    m_ui->labelPilot_2->setVisible( masses.pilot_2.enabled );

    m_ui->labelFuelTank_1->setVisible( masses.fuel_tank_1.enabled );
    m_ui->labelFuelTank_2->setVisible( masses.fuel_tank_2.enabled );
    m_ui->labelFuelTank_3->setVisible( masses.fuel_tank_3.enabled );
    m_ui->labelFuelTank_4->setVisible( masses.fuel_tank_4.enabled );

    m_ui->labelCabin->setVisible( masses.cabin.enabled );
    m_ui->labelTrunk->setVisible( masses.trunk.enabled );

    m_ui->labelPilot_1->setText( masses.pilot_1.name + ":" );
    m_ui->labelPilot_2->setText( masses.pilot_2.name + ":" );

    m_ui->labelFuelTank_1->setText( masses.fuel_tank_1.name + ":" );
    m_ui->labelFuelTank_2->setText( masses.fuel_tank_2.name + ":" );
    m_ui->labelFuelTank_3->setText( masses.fuel_tank_3.name + ":" );
    m_ui->labelFuelTank_4->setText( masses.fuel_tank_4.name + ":" );

    m_ui->labelCabin->setText( masses.cabin.name + ":" );
    m_ui->labelTrunk->setText( masses.trunk.name + ":" );

    m_ui->spinBoxPilot_1->setVisible( masses.pilot_1.enabled );
    m_ui->spinBoxPilot_2->setVisible( masses.pilot_2.enabled );

    m_ui->spinBoxFuelTank_1->setVisible( masses.fuel_tank_1.enabled );
    m_ui->spinBoxFuelTank_2->setVisible( masses.fuel_tank_2.enabled );
    m_ui->spinBoxFuelTank_3->setVisible( masses.fuel_tank_3.enabled );
    m_ui->spinBoxFuelTank_4->setVisible( masses.fuel_tank_4.enabled );

    m_ui->spinBoxCabin->setVisible( masses.cabin.enabled );
    m_ui->spinBoxTrunk->setVisible( masses.trunk.enabled );

    m_ui->comboBoxPilot_1->setVisible( masses.pilot_1.enabled );
    m_ui->comboBoxPilot_2->setVisible( masses.pilot_2.enabled );

    m_ui->comboBoxFuelTank_1->setVisible( masses.fuel_tank_1.enabled );
    m_ui->comboBoxFuelTank_2->setVisible( masses.fuel_tank_2.enabled );
    m_ui->comboBoxFuelTank_3->setVisible( masses.fuel_tank_3.enabled );
    m_ui->comboBoxFuelTank_4->setVisible( masses.fuel_tank_4.enabled );

    m_ui->comboBoxCabin->setVisible( masses.cabin.enabled );
    m_ui->comboBoxTrunk->setVisible( masses.trunk.enabled );

    readData();
}

////////////////////////////////////////////////////////////////////////////////

void DialogMass::settingsRead()
{
    QSettings settings( SIM_ORG_NAME, SIM_APP_NAME );

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

    m_pilot_1 = settings.value( "pilot_1", 0.0 ).toDouble();
    m_pilot_2 = settings.value( "pilot_2", 0.0 ).toDouble();

    m_fuel_tank_1 = settings.value( "fuel_tank_1", 0.0 ).toDouble();
    m_fuel_tank_2 = settings.value( "fuel_tank_2", 0.0 ).toDouble();
    m_fuel_tank_3 = settings.value( "fuel_tank_3", 0.0 ).toDouble();
    m_fuel_tank_4 = settings.value( "fuel_tank_4", 0.0 ).toDouble();

    m_cabin = settings.value( "cabin", 0.0 ).toDouble();
    m_trunk = settings.value( "trunk", 0.0 ).toDouble();

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DialogMass::settingsRead_UnitCombos( QSettings &settings )
{
    settings.beginGroup( "unit_combos" );

    m_ui->comboBoxPilot_1->setCurrentIndex( settings.value( "pilot_1", 0 ).toInt() );
    m_ui->comboBoxPilot_2->setCurrentIndex( settings.value( "pilot_2", 0 ).toInt() );

    m_ui->comboBoxFuelTank_1->setCurrentIndex( settings.value( "fuel_tank_1", 0 ).toInt() );
    m_ui->comboBoxFuelTank_2->setCurrentIndex( settings.value( "fuel_tank_2", 0 ).toInt() );
    m_ui->comboBoxFuelTank_3->setCurrentIndex( settings.value( "fuel_tank_3", 0 ).toInt() );
    m_ui->comboBoxFuelTank_4->setCurrentIndex( settings.value( "fuel_tank_4", 0 ).toInt() );

    m_ui->comboBoxCabin->setCurrentIndex( settings.value( "cabin", 0 ).toInt() );
    m_ui->comboBoxTrunk->setCurrentIndex( settings.value( "trunk", 0 ).toInt() );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DialogMass::settingsSave()
{
    QSettings settings( SIM_ORG_NAME, SIM_APP_NAME );

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

    settings.setValue( "pilot_1", m_pilot_1 );
    settings.setValue( "pilot_2", m_pilot_2 );

    settings.setValue( "fuel_tank_1", m_fuel_tank_1 );
    settings.setValue( "fuel_tank_2", m_fuel_tank_2 );
    settings.setValue( "fuel_tank_3", m_fuel_tank_3 );
    settings.setValue( "fuel_tank_4", m_fuel_tank_4 );

    settings.setValue( "cabin", m_cabin );
    settings.setValue( "trunk", m_trunk );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DialogMass::settingsSave_UnitCombos( QSettings &settings )
{
    settings.beginGroup( "unit_combos" );

    settings.setValue( "pilot_1", m_ui->comboBoxPilot_1->currentIndex() );
    settings.setValue( "pilot_2", m_ui->comboBoxPilot_2->currentIndex() );

    settings.setValue( "fuel_tank_1", m_ui->comboBoxFuelTank_1->currentIndex() );
    settings.setValue( "fuel_tank_2", m_ui->comboBoxFuelTank_2->currentIndex() );
    settings.setValue( "fuel_tank_3", m_ui->comboBoxFuelTank_3->currentIndex() );
    settings.setValue( "fuel_tank_4", m_ui->comboBoxFuelTank_4->currentIndex() );

    settings.setValue( "cabin", m_ui->comboBoxCabin->currentIndex() );
    settings.setValue( "trunk", m_ui->comboBoxTrunk->currentIndex() );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DialogMass::on_comboBoxPilot_1_currentIndexChanged( int )
{
    double mass_kg = m_ui->comboBoxPilot_1->invertPrev( m_ui->spinBoxPilot_1->value() );

    Aircrafts::Masses::Mass mass = Aircrafts::instance()->getAircraft( m_type ).masses.pilot_1;
    m_ui->spinBoxPilot_1->setMaximum( m_ui->comboBoxPilot_1->convert( mass.max ) );
    m_ui->spinBoxPilot_1->setValue( m_ui->comboBoxPilot_1->convert( mass_kg ) );
}

////////////////////////////////////////////////////////////////////////////////

void DialogMass::on_comboBoxPilot_2_currentIndexChanged( int )
{
    double mass_kg = m_ui->comboBoxPilot_1->invertPrev( m_ui->spinBoxPilot_2->value() );

    Aircrafts::Masses::Mass mass = Aircrafts::instance()->getAircraft( m_type ).masses.pilot_2;
    m_ui->spinBoxPilot_2->setMaximum( m_ui->comboBoxPilot_2->convert( mass.max ) );
    m_ui->spinBoxPilot_2->setValue( m_ui->comboBoxPilot_2->convert( mass_kg ) );
}

////////////////////////////////////////////////////////////////////////////////

void DialogMass::on_comboBoxFuelTank_1_currentIndexChanged( int )
{
    double mass_kg = m_ui->comboBoxFuelTank_1->invertPrev( m_ui->spinBoxFuelTank_1->value() );

    Aircrafts::Masses::Mass mass = Aircrafts::instance()->getAircraft( m_type ).masses.fuel_tank_1;
    m_ui->spinBoxFuelTank_1->setMaximum( m_ui->comboBoxFuelTank_1->convert( mass.max ) );
    m_ui->spinBoxFuelTank_1->setValue( m_ui->comboBoxFuelTank_1->convert( mass_kg ) );
}

////////////////////////////////////////////////////////////////////////////////

void DialogMass::on_comboBoxFuelTank_2_currentIndexChanged( int )
{
    double mass_kg = m_ui->comboBoxFuelTank_2->invertPrev( m_ui->spinBoxFuelTank_2->value() );

    Aircrafts::Masses::Mass mass = Aircrafts::instance()->getAircraft( m_type ).masses.fuel_tank_2;
    m_ui->spinBoxFuelTank_2->setMaximum( m_ui->comboBoxFuelTank_2->convert( mass.max ) );
    m_ui->spinBoxFuelTank_2->setValue( m_ui->comboBoxFuelTank_2->convert( mass_kg ) );
}

////////////////////////////////////////////////////////////////////////////////

void DialogMass::on_comboBoxFuelTank_3_currentIndexChanged( int )
{
    double mass_kg = m_ui->comboBoxFuelTank_3->invertPrev( m_ui->spinBoxFuelTank_3->value() );

    Aircrafts::Masses::Mass mass = Aircrafts::instance()->getAircraft( m_type ).masses.fuel_tank_3;
    m_ui->spinBoxFuelTank_3->setMaximum( m_ui->comboBoxFuelTank_3->convert( mass.max ) );
    m_ui->spinBoxFuelTank_3->setValue( m_ui->comboBoxFuelTank_3->convert( mass_kg ) );
}

////////////////////////////////////////////////////////////////////////////////

void DialogMass::on_comboBoxFuelTank_4_currentIndexChanged( int )
{
    double mass_kg = m_ui->comboBoxFuelTank_4->invertPrev( m_ui->spinBoxFuelTank_4->value() );

    Aircrafts::Masses::Mass mass = Aircrafts::instance()->getAircraft( m_type ).masses.fuel_tank_4;
    m_ui->spinBoxFuelTank_4->setMaximum( m_ui->comboBoxFuelTank_4->convert( mass.max ) );
    m_ui->spinBoxFuelTank_4->setValue( m_ui->comboBoxFuelTank_4->convert( mass_kg ) );
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
