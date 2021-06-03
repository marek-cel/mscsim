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

#include <gui/DialogEnvr.h>
#include <ui_DialogEnvr.h>

#include <cmath>

#include <cgi/cgi_Defines.h>
#include <gui/gui_Defines.h>

////////////////////////////////////////////////////////////////////////////////

DialogEnvr::DialogEnvr( QWidget *parent ) :
    QDialog ( parent ),
    _ui ( new Ui::DialogEnvr ),

    _temperature_0 ( 288.15 ),
    _pressure_0 ( 101325.0 ),

    _windDirection ( 0.0 ),
    _windSpeed     ( 0.0 ),

    _turbulence ( fdm::DataInp::Environment::TurbulenceNone ),
    _windShear  ( fdm::DataInp::Environment::WindShearNone  ),

    _visibility ( 0.0 ),

    _cloudsType ( Data::CGI::Environment::Clouds::Block )
{
    _ui->setupUi( this );

    double area_sq_m = M_PI * CGI_SKYDOME_RADIUS * CGI_SKYDOME_RADIUS;
    double area_sq_km = area_sq_m / 1000.0 / 1000.0;
    double max_clouds = 100.0 * CGI_CLOUDS_MAX_COUNT / area_sq_km;
    _ui->doubleSpinBoxCloudsBlockCount->setMaximum( max_clouds );

    memset( &_blockClouds, 0, sizeof(BlockClouds) );
    memset( &_layerClouds, 0, sizeof(LayerClouds) );

    _blockClouds.count = 0;
    _blockClouds.base_asl = 500.0;
    _blockClouds.thickness = 500.0;

    _layerClouds.cover = Data::CGI::Environment::Clouds::Data::Layer::SKC;
    _layerClouds.base_asl = 100.0;

    settingsRead();
}

////////////////////////////////////////////////////////////////////////////////

DialogEnvr::~DialogEnvr()
{
    settingsSave();

    DELPTR( _ui );
}

////////////////////////////////////////////////////////////////////////////////

void DialogEnvr::readData()
{
    _ui->spinBoxSeaLevelTemp->setValue( _ui->comboSeaLevelTemp->convert( _temperature_0 ) );
    _ui->spinBoxSeaLevelPress->setValue( _ui->comboSeaLevelPress->convert( _pressure_0 ) );

    _ui->spinBoxWindDir->setValue( _ui->comboWindDir->convert( _windDirection ) );
    _ui->spinBoxWindSpeed->setValue( _ui->comboWindSpeed->convert( _windSpeed ) );

    _ui->sliderVisibility->setValue( _visibility / 100.0f );

    if ( _cloudsType == Data::CGI::Environment::Clouds::Block )
    {
        _ui->radioButtonCloudsTypeBlock->setChecked( true );
    }
    else if ( _cloudsType == Data::CGI::Environment::Clouds::Layer )
    {
        _ui->radioButtonCloudsTypeLayer->setChecked( true );
    }

    double area_sq_m = M_PI * CGI_SKYDOME_RADIUS * CGI_SKYDOME_RADIUS;
    double area_sq_km = area_sq_m / 1000.0 / 1000.0;
    _ui->doubleSpinBoxCloudsBlockCount->setValue( 100.0 * _blockClouds.count / area_sq_km );
    _ui->spinBoxCloudsBlockBaseASL->setValue( _ui->comboCloudsBlockBaseASL->convert( _blockClouds.base_asl ) );
    _ui->spinBoxCloudsBlockThickness->setValue( _ui->comboCloudsBlockThickness->convert( _blockClouds.thickness ) );

    _ui->comboCloudsLayerCover->setCurrentIndex( _layerClouds.cover );
    _ui->spinBoxCloudsLayerBaseASL->setValue( _ui->comboCloudsLayerBaseASL->convert( _layerClouds.base_asl ) );
}

////////////////////////////////////////////////////////////////////////////////

void DialogEnvr::saveData()
{
    _temperature_0 = _ui->comboSeaLevelTemp->invert( _ui->spinBoxSeaLevelTemp->value() );
    _pressure_0    = _ui->comboSeaLevelPress->invert( _ui->spinBoxSeaLevelPress->value() );

    _windDirection = _ui->comboWindDir->invert( _ui->spinBoxWindDir->value() );
    _windSpeed     = _ui->comboWindSpeed->invert( _ui->spinBoxWindSpeed->value() );

    _visibility = 100.0f * _ui->sliderVisibility->value();

    if ( _ui->radioButtonCloudsTypeBlock->isChecked() )
        _cloudsType = Data::CGI::Environment::Clouds::Block;
    else
        _cloudsType = Data::CGI::Environment::Clouds::Layer;

    double area_sq_m = M_PI * CGI_SKYDOME_RADIUS * CGI_SKYDOME_RADIUS;
    double area_sq_km = area_sq_m / 1000.0 / 1000.0;
    _blockClouds.count     = _ui->doubleSpinBoxCloudsBlockCount->value() * area_sq_km / 100.0;
    _blockClouds.base_asl  = _ui->comboCloudsBlockBaseASL->invert( _ui->spinBoxCloudsBlockBaseASL->value() );
    _blockClouds.thickness = _ui->comboCloudsBlockThickness->invert( _ui->spinBoxCloudsBlockThickness->value() );

    _layerClouds.cover = intToCover( _ui->comboCloudsLayerCover->currentIndex() );
    _layerClouds.base_asl = _ui->comboCloudsLayerBaseASL->invert( _ui->spinBoxCloudsLayerBaseASL->value() );
}

////////////////////////////////////////////////////////////////////////////////

DialogEnvr::Cover DialogEnvr::intToCover( int index, Cover def )
{
    switch ( index )
    {
    case Data::CGI::Environment::Clouds::Data::Layer::SKC:
        return Data::CGI::Environment::Clouds::Data::Layer::SKC;
        break;

    case Data::CGI::Environment::Clouds::Data::Layer::FEW:
        return Data::CGI::Environment::Clouds::Data::Layer::FEW;
        break;

    case Data::CGI::Environment::Clouds::Data::Layer::SCT:
        return Data::CGI::Environment::Clouds::Data::Layer::SCT;
        break;

    case Data::CGI::Environment::Clouds::Data::Layer::BKN:
        return Data::CGI::Environment::Clouds::Data::Layer::BKN;
        break;

    case Data::CGI::Environment::Clouds::Data::Layer::OVC:
        return Data::CGI::Environment::Clouds::Data::Layer::OVC;
        break;
    }

    return def;
}

////////////////////////////////////////////////////////////////////////////////

void DialogEnvr::settingsRead()
{
    QSettings settings( SIM_ORG_NAME, SIM_APP_NAME );

    settings.beginGroup( "dialog_envr" );

    restoreGeometry( settings.value( "geometry" ).toByteArray() );

    settingsRead_EnvrData( settings );
    settingsRead_UnitCombos( settings );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DialogEnvr::settingsRead_EnvrData( QSettings &settings )
{
    settings.beginGroup( "envr_data" );

    _temperature_0 = settings.value( "sea_level_temp"  , 288.15f   ).toFloat();
    _pressure_0    = settings.value( "sea_level_press" , 101325.0f ).toFloat();

    _windDirection = settings.value( "wind_dir"   , 0.0 ).toFloat();
    _windSpeed     = settings.value( "wind_speed" , 0.0 ).toFloat();

    _visibility = settings.value( "visibility", CGI_SKYDOME_RADIUS ).toFloat();

    switch ( settings.value( "clouds_type", Data::CGI::Environment::Clouds::Block ).toInt() )
    {
    case Data::CGI::Environment::Clouds::Block:
        _cloudsType = Data::CGI::Environment::Clouds::Block;
        break;

    case Data::CGI::Environment::Clouds::Layer:
        _cloudsType = Data::CGI::Environment::Clouds::Layer;
        break;
    }

    _blockClouds.count     = settings.value( "clouds_block_count" , _blockClouds.count     ).toInt();
    _blockClouds.base_asl  = settings.value( "clouds_block_base"  , _blockClouds.base_asl  ).toFloat();
    _blockClouds.thickness = settings.value( "clouds_block_thick" , _blockClouds.thickness ).toFloat();

    _layerClouds.cover = intToCover( settings.value( "clouds_layer_cover",
                               Data::CGI::Environment::Clouds::Data::Layer::SKC ).toInt() );
    _layerClouds.base_asl  = settings.value( "clouds_layer_base"  , _layerClouds.base_asl  ).toFloat();

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DialogEnvr::settingsRead_UnitCombos( QSettings &settings )
{
    settings.beginGroup( "unit_combos" );

    _ui->comboSeaLevelTemp->setCurrentIndex( settings.value( "sea_level_temp", 1 ).toInt() );
    _ui->comboSeaLevelPress->setCurrentIndex( settings.value( "sea_level_press", 1 ).toInt() );

    _ui->comboWindDir->setCurrentIndex( settings.value( "wind_dir", 1 ).toInt() );
    _ui->comboWindSpeed->setCurrentIndex( settings.value( "wind_speed", 0 ).toInt() );

    _ui->comboVisibility->setCurrentIndex( settings.value( "visibility", 2 ).toInt() );

    _ui->comboCloudsBlockBaseASL->setCurrentIndex( settings.value( "clouds_block_base", 0 ).toInt() );
    _ui->comboCloudsBlockThickness->setCurrentIndex( settings.value( "clouds_block_thick", 0 ).toInt() );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DialogEnvr::settingsSave()
{
    QSettings settings( SIM_ORG_NAME, SIM_APP_NAME );

    settings.beginGroup( "dialog_envr" );

    settings.setValue( "geometry" , saveGeometry() );

    settingsSave_EnvrData( settings );
    settingsSave_UnitCombos( settings );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DialogEnvr::settingsSave_EnvrData( QSettings &settings )
{
    settings.beginGroup( "envr_data" );

    settings.setValue( "sea_level_temp"  , _temperature_0 );
    settings.setValue( "sea_level_press" , _pressure_0 );

    settings.setValue( "wind_dir"   , _windDirection );
    settings.setValue( "wind_speed" , _windSpeed );
    settings.setValue( "turbulence" , _turbulence );

    settings.setValue( "visibility", _visibility );

    settings.setValue( "clouds_type", _cloudsType );

    settings.setValue( "clouds_block_count" , _blockClouds.count     );
    settings.setValue( "clouds_block_base"  , _blockClouds.base_asl  );
    settings.setValue( "clouds_block_thick" , _blockClouds.thickness );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DialogEnvr::settingsSave_UnitCombos( QSettings &settings )
{
    settings.beginGroup( "unit_combos" );

    settings.setValue( "sea_level_temp"  , _ui->comboSeaLevelTemp->currentIndex() );
    settings.setValue( "sea_level_press" , _ui->comboSeaLevelPress->currentIndex() );

    settings.setValue( "wind_dir"   , _ui->comboWindDir->currentIndex() );
    settings.setValue( "wind_speed" , _ui->comboWindSpeed->currentIndex() );

    settings.setValue( "visibility", _ui->comboVisibility->currentIndex() );

    settings.setValue( "clouds_block_base"  , _ui->comboCloudsBlockBaseASL->currentIndex() );
    settings.setValue( "clouds_block_thick" , _ui->comboCloudsBlockThickness->currentIndex() );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DialogEnvr::on_comboSeaLevelTemp_currentIndexChanged( int /*index*/ )
{
    double value = _ui->comboSeaLevelTemp->invertPrev( _ui->spinBoxSeaLevelTemp->value() );

    _ui->spinBoxSeaLevelTemp->setMinimum( _ui->comboSeaLevelTemp->convert( 273.15 - 50.0 ) );
    _ui->spinBoxSeaLevelTemp->setMaximum( _ui->comboSeaLevelTemp->convert( 273.15 + 50.0 ) );

    _ui->spinBoxSeaLevelTemp->setValue( _ui->comboSeaLevelTemp->convert( value ) );
}

////////////////////////////////////////////////////////////////////////////////

void DialogEnvr::on_comboSeaLevelPress_currentIndexChanged( int index )
{
    double value = _ui->comboSeaLevelPress->invertPrev( _ui->spinBoxSeaLevelPress->value() );

    if ( index == 0 )
    {
        _ui->spinBoxSeaLevelPress->setDecimals( 0 );
    }
    else if ( index == 2 )
    {
        _ui->spinBoxSeaLevelPress->setDecimals( 5 );
    }
    else if ( index == 3 )
    {
        _ui->spinBoxSeaLevelPress->setDecimals( 3 );
    }
    else
    {
        _ui->spinBoxSeaLevelPress->setDecimals( 2 );
    }

    _ui->spinBoxSeaLevelPress->setMinimum( _ui->comboSeaLevelPress->convert( 101325.0 - 5000.0 ) );
    _ui->spinBoxSeaLevelPress->setMaximum( _ui->comboSeaLevelPress->convert( 101325.0 + 5000.0 ) );

    _ui->spinBoxSeaLevelPress->setValue( _ui->comboSeaLevelPress->convert( value ) );
}

////////////////////////////////////////////////////////////////////////////////

void DialogEnvr::on_comboWindDir_currentIndexChanged( int index )
{
    double value = _ui->comboWindDir->invertPrev( _ui->spinBoxWindDir->value() );

    if ( index == 0 )
    {
        // rad
        _ui->spinBoxWindDir->setDecimals( 2 );
        _ui->spinBoxWindDir->setMaximum( 2.0 * M_PI );
    }
    else
    {
        // deg
        _ui->spinBoxWindDir->setDecimals( 0 );
        _ui->spinBoxWindDir->setMaximum( 360.0 );
    }

    _ui->spinBoxWindDir->setValue( _ui->comboWindDir->convert( value ) );
}

////////////////////////////////////////////////////////////////////////////////

void DialogEnvr::on_comboWindSpeed_currentIndexChanged( int /*index*/ )
{
    double value = _ui->comboWindSpeed->invertPrev( _ui->spinBoxWindSpeed->value() );

    _ui->spinBoxWindSpeed->setMaximum( _ui->comboWindSpeed->convert( 30.0 ) );
    _ui->spinBoxWindSpeed->setValue( _ui->comboWindSpeed->convert( value ) );
}

////////////////////////////////////////////////////////////////////////////////

void DialogEnvr::on_comboVisibility_currentIndexChanged( int index )
{
    double value = 100.0 * _ui->sliderVisibility->value();

    if ( index == 0 || index == 1 )
    {
        _ui->spinBoxVisibility->setDecimals( 0 );
    }
    else
    {
        _ui->spinBoxVisibility->setDecimals( 1 );
    }

    _ui->spinBoxVisibility->setMaximum( _ui->comboVisibility->convert( CGI_SKYDOME_RADIUS ) );
    _ui->spinBoxVisibility->setValue( _ui->comboVisibility->convert( value ) );
}

////////////////////////////////////////////////////////////////////////////////

void DialogEnvr::on_sliderVisibility_valueChanged( int value )
{
    _ui->spinBoxVisibility->setValue( _ui->comboVisibility->convert( 100.0 * value ) );
}

////////////////////////////////////////////////////////////////////////////////

void DialogEnvr::on_comboCloudsBlockBaseASL_currentIndexChanged( int /*index*/ )
{
    double value = _ui->comboCloudsBlockBaseASL->invertPrev( _ui->spinBoxCloudsBlockBaseASL->value() );

    _ui->spinBoxCloudsBlockBaseASL->setMinimum( _ui->comboCloudsBlockBaseASL->convert( 0.0 ) );
    _ui->spinBoxCloudsBlockBaseASL->setMaximum( _ui->comboCloudsBlockBaseASL->convert( 10000.0 ) );
    _ui->spinBoxCloudsBlockBaseASL->setValue( _ui->comboCloudsBlockBaseASL->convert( value ) );
}

////////////////////////////////////////////////////////////////////////////////

void DialogEnvr::on_comboCloudsBlockThickness_currentIndexChanged( int /*index*/ )
{
    double value = _ui->comboCloudsBlockThickness->invertPrev( _ui->spinBoxCloudsBlockThickness->value() );

    _ui->spinBoxCloudsBlockThickness->setMinimum( _ui->comboCloudsBlockThickness->convert( 100.0 ) );
    _ui->spinBoxCloudsBlockThickness->setMaximum( _ui->comboCloudsBlockThickness->convert( 10000.0 ) );
    _ui->spinBoxCloudsBlockThickness->setValue( _ui->comboCloudsBlockThickness->convert( value ) );
}

////////////////////////////////////////////////////////////////////////////////

void DialogEnvr::on_radioButtonCloudsTypeBlock_clicked( bool checked )
{
    if ( checked )
    {
        _ui->stackedWidgetClouds->setCurrentIndex( 0 );
    }
    else
    {
        _ui->stackedWidgetClouds->setCurrentIndex( 1 );
    }
}

////////////////////////////////////////////////////////////////////////////////

void DialogEnvr::on_radioButtonCloudsTypeLayer_clicked( bool checked )
{
    if ( checked )
    {
        _ui->stackedWidgetClouds->setCurrentIndex( 1 );
    }
    else
    {
        _ui->stackedWidgetClouds->setCurrentIndex( 0 );
    }
}
