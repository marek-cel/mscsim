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

#include <gui/DialogInit.h>
#include <ui_DialogInit.h>

#include <cmath>

#include <fdm/utils/fdm_Units.h>
#include <fdm/utils/fdm_WGS84.h>

#include <gui/Aircrafts.h>
#include <gui/Airports.h>

#include <gui/DialogTime.h>
#include <gui/gui_Defines.h>

#include <hid/hid_Manager.h>

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

    for ( int i = 0; i < Airports::instance()->getCount(); i++ )
    {
        _ui->comboAirports->addItem( QIcon(), Airports::instance()->getAirport( i ).name );
    }

    _ui->comboAirports->setCurrentIndex( -1 );

    settingsRead();
}

////////////////////////////////////////////////////////////////////////////////

DialogInit::~DialogInit()
{
    settingsSave();

    DELPTR( _ui );
}

////////////////////////////////////////////////////////////////////////////////

void DialogInit::readData()
{
    _ui->comboAircrafts->setCurrentIndex( _typeIndex );
    _ui->comboAirports->setCurrentIndex( -1 );
    _ui->comboLocations->setCurrentIndex( -1 );

    _ui->spinDistance->setEnabled( false );
    _ui->spinDistance->setValue( 0.0 );

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

    if ( _ui->spinDistance->value() > 0.0 )
    {
        int indexApt = _ui->comboAirports->currentIndex();
        int indexLoc = _ui->comboLocations->currentIndex();

        Airports::Airport airport;

        if ( indexApt >= 0 && indexApt < Airports::instance()->getCount() )
        {
            airport = Airports::instance()->getAirport( indexApt );
        }

        if ( indexLoc >= 0 && indexLoc < airport.locations.count() )
        {
            double dist = _ui->comboDistance->invert( _ui->spinDistance->value() );

            fdm::Vector3 pos_bas( -dist, 0.0, 0.0 );

            double psi = _ui->comboInitPsi->invert( _ui->spinInitPsi->value() );

            fdm::Vector3 pos_ned = fdm::Matrix3x3( fdm::Angles( 0.0, 0.0, -psi ) ) * pos_bas;

            fdm::WGS84::Geo pos_geo;

            pos_geo.lat = _ui->comboInitLat->invert( _ui->spinInitLat->value() );
            pos_geo.lon = _ui->comboInitLon->invert( _ui->spinInitLon->value() );
            pos_geo.alt = 0.0;//Locations::instance()->getLocation( index ).elev;

            fdm::WGS84 wgs( pos_geo );

            fdm::Vector3 pos_wgs = wgs.getPos_WGS() + wgs.getNED2WGS() * pos_ned;

            wgs.setPos_WGS( pos_wgs );

            _lat = wgs.getPos_Geo().lat;
            _lon = wgs.getPos_Geo().lon;

            double slope = airport.locations.at( indexLoc ).slope;

            if ( dist < 0.0 ) slope = 0.0;

            _alt = airport.locations.at( indexLoc ).elev + dist * tan( slope );
        }
    }
    else
    {
        _lat = _ui->comboInitLat->invert( _ui->spinInitLat->value() );
        _lon = _ui->comboInitLon->invert( _ui->spinInitLon->value() );
        _alt = _ui->comboInitAlt->invert( _ui->spinInitAlt->value() );
    }

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

    if ( _typeIndex >= Aircrafts::instance()->getCount() )
    {
        _typeIndex = 0;
    }

    Airports::Location location = Airports::instance()->getDefault();

    _lat = settings.value( "lat", location.lat ).toFloat();
    _lon = settings.value( "lon", location.lon ).toFloat();
    _alt = settings.value( "alt", location.alt ).toFloat();
    _psi = settings.value( "psi", location.hdg ).toFloat();
    _ias = settings.value( "ias", 0.0 ).toFloat();

    short noon = 12 + floor( 12.0 * _lon / M_PI );

    short date_y = settings.value( "date_y", 2000 ).toInt();
    short date_m = settings.value( "date_m",    1 ).toInt();
    short date_d = settings.value( "date_d",    1 ).toInt();
    short time_h = settings.value( "time_h", noon ).toInt();
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

    _ui->comboDistance->setCurrentIndex( settings.value( "distance", 0 ).toInt() );

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

    settings.setValue( "distance", _ui->comboDistance->currentIndex() );

    settings.setValue( "lat", _ui->comboInitLat->currentIndex() );
    settings.setValue( "lon", _ui->comboInitLon->currentIndex() );
    settings.setValue( "alt", _ui->comboInitAlt->currentIndex() );
    settings.setValue( "psi", _ui->comboInitPsi->currentIndex() );
    settings.setValue( "ias", _ui->comboInitIAS->currentIndex() );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DialogInit::on_comboAirports_currentIndexChanged( int index )
{
    _ui->comboLocations->clear();

    if ( index >= 0 && index < Airports::instance()->getCount() )
    {
        Airports::Airport airport = Airports::instance()->getAirport( index );

        for ( int i = 0; i < airport.locations.count(); i++ )
        {
            _ui->comboLocations->addItem( QIcon(), airport.locations.at( i ).name );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void DialogInit::on_comboLocations_currentIndexChanged( int index )
{
    int indexApt = _ui->comboAirports->currentIndex();

    Airports::Airport airport;

    if ( indexApt >= 0 && indexApt < Airports::instance()->getCount() )
    {
        airport = Airports::instance()->getAirport( indexApt );
    }

    if ( index >= 0 && index < airport.locations.count() )
    {
        Airports::Location location = airport.locations.at( index );

        double lat = location.lat;
        double lon = location.lon;
        double alt = location.alt;
        double hdg = location.hdg;

        _ui->spinInitLat->setValue( _ui->comboInitLat->convert( lat ) );
        _ui->spinInitLon->setValue( _ui->comboInitLon->convert( lon ) );
        _ui->spinInitAlt->setValue( _ui->comboInitAlt->convert( ( alt > 1.0e-3 ) ? alt : 0.0f ) );
        _ui->spinInitPsi->setValue( _ui->comboInitPsi->convert( hdg ) );

        if ( location.runway )
        {
            _ui->labelDistance ->setEnabled( true );
            _ui->spinDistance  ->setEnabled( true );
            _ui->comboDistance ->setEnabled( true );
        }
        else
        {
            _ui->labelDistance ->setEnabled( false );
            _ui->spinDistance  ->setEnabled( false );
            _ui->comboDistance ->setEnabled( false );

            _ui->spinDistance->setValue( 0.0 );
        }
    }
    else
    {
        _ui->labelDistance ->setEnabled( false );
        _ui->spinDistance  ->setEnabled( false );
        _ui->comboDistance ->setEnabled( false );

        _ui->spinDistance->setValue( 0.0 );
    }
}

////////////////////////////////////////////////////////////////////////////////

void DialogInit::on_spinInitLat_valueChanged( double /*arg1*/ )
{
    _ui->labelDistance ->setEnabled( false );
    _ui->spinDistance  ->setEnabled( false );
    _ui->comboDistance ->setEnabled( false );

    _ui->spinDistance->setValue( 0.0 );
}

////////////////////////////////////////////////////////////////////////////////

void DialogInit::on_spinInitLon_valueChanged( double /*arg1*/ )
{
    _ui->labelDistance ->setEnabled( false );
    _ui->spinDistance  ->setEnabled( false );
    _ui->comboDistance ->setEnabled( false );

    _ui->spinDistance->setValue( 0.0 );
}

////////////////////////////////////////////////////////////////////////////////

void DialogInit::on_spinInitAlt_valueChanged( double /*arg1*/ )
{
    _ui->labelDistance ->setEnabled( false );
    _ui->spinDistance  ->setEnabled( false );
    _ui->comboDistance ->setEnabled( false );

    _ui->spinDistance->setValue( 0.0 );
}

////////////////////////////////////////////////////////////////////////////////

void DialogInit::on_spinInitPsi_valueChanged( double /*arg1*/ )
{
    _ui->labelDistance ->setEnabled( false );
    _ui->spinDistance  ->setEnabled( false );
    _ui->comboDistance ->setEnabled( false );

    _ui->spinDistance->setValue( 0.0 );
}

////////////////////////////////////////////////////////////////////////////////

void DialogInit::on_comboDistance_currentIndexChanged( int )
{
    float value = _ui->comboDistance->invertPrev( _ui->spinDistance->value() );

    float max  = fdm::Units::nmi2m( 10.0 );
    float step = fdm::Units::nmi2m( 1.0 );

    _ui->spinDistance->setMinimum( -_ui->comboDistance->convert( max ) );
    _ui->spinDistance->setMaximum(  _ui->comboDistance->convert( max ) );

    _ui->spinDistance->setSingleStep( _ui->comboDistance->convert( step ) );

    _ui->spinDistance->setValue( _ui->comboDistance->convert( value ) );
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
        _ui->spinInitPsi->setDecimals( 3 );
        _ui->spinInitPsi->setMaximum( 2.0 * M_PI );
    }
    else
    {
        // deg
        _ui->spinInitPsi->setDecimals( 1 );
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
