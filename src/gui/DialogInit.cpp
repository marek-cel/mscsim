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

#include <gui/DialogInit.h>
#include <ui_DialogInit.h>

#include <math.h>

#include <gui/Aircrafts.h>
#include <gui/Locations.h>

#include <gui/gui_Defines.h>

////////////////////////////////////////////////////////////////////////////////

DialogInit::DialogInit( QWidget *parent ) :
    QDialog ( parent ),
    m_ui ( new Ui::DialogInit ),

    m_typeIndex ( 0 ),

    m_lat ( 0.0f ),
    m_lon ( 0.0f ),
    m_alt ( 0.0f ),
    m_psi ( 0.0f ),
    m_ias ( 0.0f ),

    m_engine ( true )
{
    m_ui->setupUi( this );

    for ( int i = 0; i < Aircrafts::instance()->getCount(); i++ )
    {
        m_ui->comboAircrafts->addItem( QIcon(), Aircrafts::instance()->getAircraft( i ).name );
    }

    for ( int i = 0; i < Locations::instance()->getCount(); i++ )
    {
        m_ui->comboLocations->addItem( QIcon(), Locations::instance()->getLocation( i ).name );
    }

    settingsRead();
}

////////////////////////////////////////////////////////////////////////////////

DialogInit::~DialogInit()
{
    settingsSave();

    if ( m_ui ) delete m_ui;
    m_ui = 0;
}

////////////////////////////////////////////////////////////////////////////////

void DialogInit::readData()
{
    m_ui->comboAircrafts->setCurrentIndex( m_typeIndex );
    m_ui->comboLocations->setCurrentIndex( -1 );

    m_ui->spinInitLat->setValue( m_ui->comboInitLat->convert( m_lat ) );
    m_ui->spinInitLon->setValue( m_ui->comboInitLon->convert( m_lon ) );
    m_ui->spinInitAlt->setValue( m_ui->comboInitAlt->convert( m_alt ) );
    m_ui->spinInitPsi->setValue( m_ui->comboInitPsi->convert( m_psi ) );
    m_ui->spinInitIAS->setValue( m_ui->comboInitIAS->convert( m_ias ) );

    m_ui->dateInit->setDate( m_dateTime.date() );
    m_ui->timeInit->setTime( m_dateTime.time() );

    m_ui->checkBoxEngineOn->setChecked( m_engine );
}

////////////////////////////////////////////////////////////////////////////////

void DialogInit::saveData()
{
    int typeIndexPrev = m_typeIndex;

    m_typeIndex = m_ui->comboAircrafts->currentIndex();

    m_lat = m_ui->comboInitLat->invert( m_ui->spinInitLat->value() );
    m_lon = m_ui->comboInitLon->invert( m_ui->spinInitLon->value() );
    m_alt = m_ui->comboInitAlt->invert( m_ui->spinInitAlt->value() );
    m_psi = m_ui->comboInitPsi->invert( m_ui->spinInitPsi->value() );
    m_ias = m_ui->comboInitIAS->invert( m_ui->spinInitIAS->value() );

    m_dateTime.setDate( m_ui->dateInit->date() );
    m_dateTime.setTime( m_ui->timeInit->time() );

    m_engine = m_ui->checkBoxEngineOn->isChecked();

    if ( typeIndexPrev != m_typeIndex )
    {
        emit typeIndexChanged( m_typeIndex );
    }
}

////////////////////////////////////////////////////////////////////////////////

void DialogInit::settingsRead()
{
    QSettings settings( GUI_ORG_NAME, GUI_APP_NAME );

    settings.beginGroup( "dialog_init" );

    restoreGeometry( settings.value( "geometry" ).toByteArray() );

    settingsRead_InitData( settings );
    settingsRead_UnitCombos( settings );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DialogInit::settingsRead_InitData( QSettings &settings )
{
    settings.beginGroup( "init_data" );

    m_typeIndex = settings.value( "type_index", 0 ).toInt();

    if ( Locations::instance()->getCount() > 0 )
    {
        m_lat = settings.value( "lat", Locations::instance()->getLocation( 0 ).lat ).toFloat();
        m_lon = settings.value( "lon", Locations::instance()->getLocation( 0 ).lon ).toFloat();
        m_alt = settings.value( "alt", Locations::instance()->getLocation( 0 ).alt ).toFloat();
        m_psi = settings.value( "psi", Locations::instance()->getLocation( 0 ).hdg ).toFloat();
        m_ias = settings.value( "ias", 0.0 ).toFloat();
    }
    else
    {
        m_lat = settings.value( "lat", 0.0 ).toFloat();
        m_lon = settings.value( "lon", 0.0 ).toFloat();
        m_alt = settings.value( "alt", 0.0 ).toFloat();
        m_psi = settings.value( "psi", 0.0 ).toFloat();
        m_ias = settings.value( "ias", 0.0 ).toFloat();
    }

    short date_y = settings.value( "date_y", 2000 ).toInt();
    short date_m = settings.value( "date_m",    1 ).toInt();
    short date_d = settings.value( "date_d",    1 ).toInt();
    short time_h = settings.value( "time_h",   12 ).toInt();
    short time_m = settings.value( "time_m",    0 ).toInt();

    QDate date;
    QTime time;

    date.setYMD( date_y, date_m, date_d );
    time.setHMS( time_h, time_m, 0 );

    m_dateTime.setDate( date );
    m_dateTime.setTime( time );

    m_engine = settings.value( "engine", 1 ).toInt() != 0;

    m_ui->comboAircrafts->setCurrentIndex( m_typeIndex );

    m_ui->spinInitLat->setValue( m_ui->comboInitLat->convert( m_lat ) );
    m_ui->spinInitLon->setValue( m_ui->comboInitLon->convert( m_lon ) );
    m_ui->spinInitAlt->setValue( m_ui->comboInitAlt->convert( m_alt ) );
    m_ui->spinInitPsi->setValue( m_ui->comboInitPsi->convert( m_psi ) );
    m_ui->spinInitIAS->setValue( m_ui->comboInitIAS->convert( m_ias ) );

    m_ui->dateInit->setDate( m_dateTime.date() );
    m_ui->timeInit->setTime( m_dateTime.time() );

    m_ui->checkBoxEngineOn->setChecked( m_engine );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DialogInit::settingsRead_UnitCombos( QSettings &settings )
{
    settings.beginGroup( "unit_combos" );

    m_ui->comboInitLat->setCurrentIndex( settings.value( "lat", 1 ).toInt() );
    m_ui->comboInitLon->setCurrentIndex( settings.value( "lon", 1 ).toInt() );
    m_ui->comboInitAlt->setCurrentIndex( settings.value( "alt", 0 ).toInt() );
    m_ui->comboInitPsi->setCurrentIndex( settings.value( "psi", 1 ).toInt() );
    m_ui->comboInitIAS->setCurrentIndex( settings.value( "ias", 0 ).toInt() );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DialogInit::settingsSave()
{
    QSettings settings( GUI_ORG_NAME, GUI_APP_NAME );

    settings.beginGroup( "dialog_init" );

    settings.setValue( "geometry" , saveGeometry() );

    settingsSave_InitData( settings );
    settingsSave_UnitCombos( settings );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DialogInit::settingsSave_InitData( QSettings &settings )
{
    settings.beginGroup( "init_data" );

    settings.setValue( "type_index" , m_typeIndex );

    settings.setValue( "lat", m_lat );
    settings.setValue( "lon", m_lon );
    settings.setValue( "alt", m_alt );
    settings.setValue( "psi", m_psi );
    settings.setValue( "ias", m_ias );

    settings.setValue( "date_y", m_dateTime.date().year()   );
    settings.setValue( "date_m", m_dateTime.date().month()  );
    settings.setValue( "date_d", m_dateTime.date().day()    );
    settings.setValue( "time_h", m_dateTime.time().hour()   );
    settings.setValue( "time_m", m_dateTime.time().minute() );

    settings.setValue( "engine", m_engine ? 1 : 0 );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DialogInit::settingsSave_UnitCombos( QSettings &settings )
{
    settings.beginGroup( "unit_combos" );

    settings.setValue( "lat", m_ui->comboInitLat->currentIndex() );
    settings.setValue( "lon", m_ui->comboInitLon->currentIndex() );
    settings.setValue( "alt", m_ui->comboInitAlt->currentIndex() );
    settings.setValue( "psi", m_ui->comboInitPsi->currentIndex() );
    settings.setValue( "ias", m_ui->comboInitIAS->currentIndex() );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DialogInit::on_comboLocations_currentIndexChanged( int index )
{
    if ( index > -1 )
    {
        float lat = Locations::instance()->getLocation( index ).lat;
        float lon = Locations::instance()->getLocation( index ).lon;
        float alt = Locations::instance()->getLocation( index ).alt;
        float hdg = Locations::instance()->getLocation( index ).hdg;

        m_ui->spinInitLat->setValue( m_ui->comboInitLat->convert( lat ) );
        m_ui->spinInitLon->setValue( m_ui->comboInitLon->convert( lon ) );
        m_ui->spinInitAlt->setValue( m_ui->comboInitAlt->convert( ( alt > 1.0e-6 ) ? alt : 0.0f ) );
        m_ui->spinInitPsi->setValue( m_ui->comboInitPsi->convert( hdg ) );
    }
}

////////////////////////////////////////////////////////////////////////////////

void DialogInit::on_comboInitLat_currentIndexChanged( int index )
{
    float value = m_ui->comboInitLat->invertPrev( m_ui->spinInitLat->value() );

    if ( index == 0 )
    {
        // rad
        m_ui->spinInitLat->setDecimals( 8 );
        m_ui->spinInitLat->setMinimum( -M_PI / 2.0 );
        m_ui->spinInitLat->setMaximum(  M_PI / 2.0 );
    }
    else
    {
        // deg
        m_ui->spinInitLat->setDecimals( 6 );
        m_ui->spinInitLat->setMinimum( -90.0 );
        m_ui->spinInitLat->setMaximum(  90.0 );
    }

    m_ui->spinInitLat->setValue( m_ui->comboInitLat->convert( value ) );
}

////////////////////////////////////////////////////////////////////////////////

void DialogInit::on_comboInitLon_currentIndexChanged( int index )
{
    float value = m_ui->comboInitLon->invertPrev( m_ui->spinInitLon->value() );

    if ( index == 0 )
    {
        // rad
        m_ui->spinInitLon->setDecimals( 8 );
        m_ui->spinInitLon->setMinimum( -M_PI );
        m_ui->spinInitLon->setMaximum(  M_PI );
    }
    else
    {
        // deg
        m_ui->spinInitLon->setDecimals( 6 );
        m_ui->spinInitLon->setMinimum( -180.0 );
        m_ui->spinInitLon->setMaximum(  180.0 );
    }

    m_ui->spinInitLon->setValue( m_ui->comboInitLon->convert( value ) );
}

////////////////////////////////////////////////////////////////////////////////

void DialogInit::on_comboInitAlt_currentIndexChanged( int /*index*/ )
{
    float value = m_ui->comboInitAlt->invertPrev( m_ui->spinInitAlt->value() );

    m_ui->spinInitAlt->setValue( m_ui->comboInitAlt->convert( value ) );
}

////////////////////////////////////////////////////////////////////////////////

void DialogInit::on_comboInitPsi_currentIndexChanged( int index )
{
    float value = m_ui->comboInitPsi->invertPrev( m_ui->spinInitPsi->value() );

    if ( index == 0 )
    {
        // rad
        m_ui->spinInitPsi->setDecimals( 2 );
        m_ui->spinInitPsi->setMaximum( 2.0 * M_PI );
    }
    else
    {
        // deg
        m_ui->spinInitPsi->setDecimals( 0 );
        m_ui->spinInitPsi->setMaximum( 360.0 );
    }

    m_ui->spinInitPsi->setValue( m_ui->comboInitPsi->convert( value ) );
}

////////////////////////////////////////////////////////////////////////////////

void DialogInit::on_comboInitIAS_currentIndexChanged( int /*index*/ )
{
    float value = m_ui->comboInitIAS->invertPrev( m_ui->spinInitIAS->value() );

    m_ui->spinInitIAS->setValue( m_ui->comboInitIAS->convert( value ) );
}

////////////////////////////////////////////////////////////////////////////////

void DialogInit::on_checkBoxEngineOn_toggled( bool checked )
{
    m_engine = checked;
}
