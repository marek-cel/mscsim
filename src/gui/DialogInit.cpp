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

#include <gui/DialogTime.h>

#include <gui/gui_Defines.h>

////////////////////////////////////////////////////////////////////////////////

DialogInit::DialogInit( QWidget *parent ) :
    QDialog ( parent ),
    _ui ( new Ui::DialogInit ),

    _typeIndex ( 0 ),

    _lat ( 0.0f ),
    _lon ( 0.0f ),
    _alt ( 0.0f ),
    _psi ( 0.0f ),
    _ias ( 0.0f ),

    _engine ( true )
{
    _ui->setupUi( this );

    for ( int i = 0; i < Aircrafts::instance()->getCount(); i++ )
    {
        _ui->comboAircrafts->addItem( QIcon(), Aircrafts::instance()->getAircraft( i ).name );
    }

    for ( int i = 0; i < Locations::instance()->getCount(); i++ )
    {
        _ui->comboLocations->addItem( QIcon(), Locations::instance()->getLocation( i ).name );
    }

    settingsRead();
}

////////////////////////////////////////////////////////////////////////////////

DialogInit::~DialogInit()
{
    settingsSave();

    SIM_DELETE( _ui );
}

////////////////////////////////////////////////////////////////////////////////

void DialogInit::readData()
{
    _ui->comboAircrafts->setCurrentIndex( _typeIndex );
    _ui->comboLocations->setCurrentIndex( -1 );

    _ui->spinInitLat->setValue( _ui->comboInitLat->convert( _lat ) );
    _ui->spinInitLon->setValue( _ui->comboInitLon->convert( _lon ) );
    _ui->spinInitAlt->setValue( _ui->comboInitAlt->convert( _alt ) );
    _ui->spinInitPsi->setValue( _ui->comboInitPsi->convert( _psi ) );
    _ui->spinInitIAS->setValue( _ui->comboInitIAS->convert( _ias ) );

    _ui->dateInit->setDate( _dateTime.date() );
    _ui->timeInit->setTime( _dateTime.time() );

    _ui->checkBoxEngineOn->setChecked( _engine );
}

////////////////////////////////////////////////////////////////////////////////

void DialogInit::saveData()
{
    int typeIndexPrev = _typeIndex;

    _typeIndex = _ui->comboAircrafts->currentIndex();

    _lat = _ui->comboInitLat->invert( _ui->spinInitLat->value() );
    _lon = _ui->comboInitLon->invert( _ui->spinInitLon->value() );
    _alt = _ui->comboInitAlt->invert( _ui->spinInitAlt->value() );
    _psi = _ui->comboInitPsi->invert( _ui->spinInitPsi->value() );
    _ias = _ui->comboInitIAS->invert( _ui->spinInitIAS->value() );

    _dateTime.setDate( _ui->dateInit->date() );
    _dateTime.setTime( _ui->timeInit->time() );

    _engine = _ui->checkBoxEngineOn->isChecked();

    if ( typeIndexPrev != _typeIndex )
    {
        emit typeIndexChanged( _typeIndex );
    }
}

////////////////////////////////////////////////////////////////////////////////

void DialogInit::settingsRead()
{
    QSettings settings( SIM_ORG_NAME, SIM_APP_NAME );

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

    _typeIndex = settings.value( "type_index", 0 ).toInt();

    if ( Locations::instance()->getCount() > 0 )
    {
        _lat = settings.value( "lat", Locations::instance()->getLocation( 0 ).lat ).toFloat();
        _lon = settings.value( "lon", Locations::instance()->getLocation( 0 ).lon ).toFloat();
        _alt = settings.value( "alt", Locations::instance()->getLocation( 0 ).alt ).toFloat();
        _psi = settings.value( "psi", Locations::instance()->getLocation( 0 ).hdg ).toFloat();
        _ias = settings.value( "ias", 0.0 ).toFloat();
    }
    else
    {
        _lat = settings.value( "lat", 0.0 ).toFloat();
        _lon = settings.value( "lon", 0.0 ).toFloat();
        _alt = settings.value( "alt", 0.0 ).toFloat();
        _psi = settings.value( "psi", 0.0 ).toFloat();
        _ias = settings.value( "ias", 0.0 ).toFloat();
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

    _dateTime.setDate( date );
    _dateTime.setTime( time );

    _engine = settings.value( "engine", 1 ).toInt() != 0;

    _ui->comboAircrafts->setCurrentIndex( _typeIndex );

    _ui->spinInitLat->setValue( _ui->comboInitLat->convert( _lat ) );
    _ui->spinInitLon->setValue( _ui->comboInitLon->convert( _lon ) );
    _ui->spinInitAlt->setValue( _ui->comboInitAlt->convert( _alt ) );
    _ui->spinInitPsi->setValue( _ui->comboInitPsi->convert( _psi ) );
    _ui->spinInitIAS->setValue( _ui->comboInitIAS->convert( _ias ) );

    _ui->dateInit->setDate( _dateTime.date() );
    _ui->timeInit->setTime( _dateTime.time() );

    _ui->checkBoxEngineOn->setChecked( _engine );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DialogInit::settingsRead_UnitCombos( QSettings &settings )
{
    settings.beginGroup( "unit_combos" );

    _ui->comboInitLat->setCurrentIndex( settings.value( "lat", 1 ).toInt() );
    _ui->comboInitLon->setCurrentIndex( settings.value( "lon", 1 ).toInt() );
    _ui->comboInitAlt->setCurrentIndex( settings.value( "alt", 0 ).toInt() );
    _ui->comboInitPsi->setCurrentIndex( settings.value( "psi", 1 ).toInt() );
    _ui->comboInitIAS->setCurrentIndex( settings.value( "ias", 0 ).toInt() );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DialogInit::settingsSave()
{
    QSettings settings( SIM_ORG_NAME, SIM_APP_NAME );

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

    settings.setValue( "type_index" , _typeIndex );

    settings.setValue( "lat", _lat );
    settings.setValue( "lon", _lon );
    settings.setValue( "alt", _alt );
    settings.setValue( "psi", _psi );
    settings.setValue( "ias", _ias );

    settings.setValue( "date_y", _dateTime.date().year()   );
    settings.setValue( "date_m", _dateTime.date().month()  );
    settings.setValue( "date_d", _dateTime.date().day()    );
    settings.setValue( "time_h", _dateTime.time().hour()   );
    settings.setValue( "time_m", _dateTime.time().minute() );

    settings.setValue( "engine", _engine ? 1 : 0 );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DialogInit::settingsSave_UnitCombos( QSettings &settings )
{
    settings.beginGroup( "unit_combos" );

    settings.setValue( "lat", _ui->comboInitLat->currentIndex() );
    settings.setValue( "lon", _ui->comboInitLon->currentIndex() );
    settings.setValue( "alt", _ui->comboInitAlt->currentIndex() );
    settings.setValue( "psi", _ui->comboInitPsi->currentIndex() );
    settings.setValue( "ias", _ui->comboInitIAS->currentIndex() );

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

        _ui->spinInitLat->setValue( _ui->comboInitLat->convert( lat ) );
        _ui->spinInitLon->setValue( _ui->comboInitLon->convert( lon ) );
        _ui->spinInitAlt->setValue( _ui->comboInitAlt->convert( ( alt > 1.0e-6 ) ? alt : 0.0f ) );
        _ui->spinInitPsi->setValue( _ui->comboInitPsi->convert( hdg ) );
    }
}

////////////////////////////////////////////////////////////////////////////////

void DialogInit::on_comboInitLat_currentIndexChanged( int index )
{
    float value = _ui->comboInitLat->invertPrev( _ui->spinInitLat->value() );

    if ( index == 0 )
    {
        // rad
        _ui->spinInitLat->setDecimals( 8 );
        _ui->spinInitLat->setMinimum( -M_PI / 2.0 );
        _ui->spinInitLat->setMaximum(  M_PI / 2.0 );
    }
    else
    {
        // deg
        _ui->spinInitLat->setDecimals( 6 );
        _ui->spinInitLat->setMinimum( -90.0 );
        _ui->spinInitLat->setMaximum(  90.0 );
    }

    _ui->spinInitLat->setValue( _ui->comboInitLat->convert( value ) );
}

////////////////////////////////////////////////////////////////////////////////

void DialogInit::on_comboInitLon_currentIndexChanged( int index )
{
    float value = _ui->comboInitLon->invertPrev( _ui->spinInitLon->value() );

    if ( index == 0 )
    {
        // rad
        _ui->spinInitLon->setDecimals( 8 );
        _ui->spinInitLon->setMinimum( -M_PI );
        _ui->spinInitLon->setMaximum(  M_PI );
    }
    else
    {
        // deg
        _ui->spinInitLon->setDecimals( 6 );
        _ui->spinInitLon->setMinimum( -180.0 );
        _ui->spinInitLon->setMaximum(  180.0 );
    }

    _ui->spinInitLon->setValue( _ui->comboInitLon->convert( value ) );
}

////////////////////////////////////////////////////////////////////////////////

void DialogInit::on_comboInitAlt_currentIndexChanged( int /*index*/ )
{
    float value = _ui->comboInitAlt->invertPrev( _ui->spinInitAlt->value() );

    _ui->spinInitAlt->setValue( _ui->comboInitAlt->convert( value ) );
}

////////////////////////////////////////////////////////////////////////////////

void DialogInit::on_comboInitPsi_currentIndexChanged( int index )
{
    float value = _ui->comboInitPsi->invertPrev( _ui->spinInitPsi->value() );

    if ( index == 0 )
    {
        // rad
        _ui->spinInitPsi->setDecimals( 2 );
        _ui->spinInitPsi->setMaximum( 2.0 * M_PI );
    }
    else
    {
        // deg
        _ui->spinInitPsi->setDecimals( 0 );
        _ui->spinInitPsi->setMaximum( 360.0 );
    }

    _ui->spinInitPsi->setValue( _ui->comboInitPsi->convert( value ) );
}

////////////////////////////////////////////////////////////////////////////////

void DialogInit::on_comboInitIAS_currentIndexChanged( int /*index*/ )
{
    float value = _ui->comboInitIAS->invertPrev( _ui->spinInitIAS->value() );

    _ui->spinInitIAS->setValue( _ui->comboInitIAS->convert( value ) );
}

////////////////////////////////////////////////////////////////////////////////

void DialogInit::on_pushButtonTime_clicked()
{
    QTime time_utc = _ui->timeInit->time();
    time_utc = DialogTime::getTimeUTC( _ui->spinInitLon->value(), time_utc, this );
    _ui->timeInit->setTime( time_utc );
}

////////////////////////////////////////////////////////////////////////////////

void DialogInit::on_checkBoxEngineOn_toggled( bool checked )
{
    _engine = checked;
}
