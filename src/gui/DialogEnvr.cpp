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

#include <gui/DialogEnvr.h>
#include <ui_DialogEnvr.h>

#include <math.h>

#include <cgi/cgi_Defines.h>
#include <gui/gui_Defines.h>

////////////////////////////////////////////////////////////////////////////////

DialogEnvr::DialogEnvr( QWidget *parent ) :
    QDialog ( parent ),
    m_ui ( new Ui::DialogEnvr ),

    m_temperature_0 ( 288.15 ),
    m_pressure_0 ( 101325.0 ),

    m_windDirection ( 0.0 ),
    m_windSpeed     ( 0.0 ),
    m_turbulence    ( 0.0 ),

    m_windShear ( fdm::DataInp::Environment::None ),

    m_visibility ( 0.0 )
{
    m_ui->setupUi( this );

    settingsRead();
}

////////////////////////////////////////////////////////////////////////////////

DialogEnvr::~DialogEnvr()
{
    settingsSave();

    if ( m_ui ) delete m_ui;
    m_ui = 0;
}

////////////////////////////////////////////////////////////////////////////////

void DialogEnvr::readData()
{
    m_ui->spinBoxSeaLevelTemp->setValue( m_ui->comboSeaLevelTemp->convert( m_temperature_0 ) );
    m_ui->spinBoxSeaLevelPress->setValue( m_ui->comboSeaLevelPress->convert( m_pressure_0 ) );

    m_ui->spinBoxWindDir->setValue( m_ui->comboWindDir->convert( m_windDirection ) );
    m_ui->spinBoxWindSpeed->setValue( m_ui->comboWindSpeed->convert( m_windSpeed ) );
    m_ui->sliderTurbulence->setValue( 100.0f * m_turbulence );

    m_ui->sliderVisibility->setValue( m_visibility / 100.0f );
}

////////////////////////////////////////////////////////////////////////////////

void DialogEnvr::saveData()
{
    m_temperature_0 = m_ui->comboSeaLevelTemp->invert( m_ui->spinBoxSeaLevelTemp->value() );
    m_pressure_0    = m_ui->comboSeaLevelPress->invert( m_ui->spinBoxSeaLevelPress->value() );

    m_windDirection = m_ui->comboWindDir->invert( m_ui->spinBoxWindDir->value() );
    m_windSpeed     = m_ui->comboWindSpeed->invert( m_ui->spinBoxWindSpeed->value() );
    m_turbulence    = (double)m_ui->sliderTurbulence->value() / 100.0;

    m_visibility = 100.0f * m_ui->sliderVisibility->value();
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

    m_temperature_0 = settings.value( "sea_level_temp"  , 288.15f   ).toFloat();
    m_pressure_0    = settings.value( "sea_level_press" , 101325.0f ).toFloat();

    m_windDirection = settings.value( "wind_dir"   , 0.0 ).toFloat();
    m_windSpeed     = settings.value( "wind_speed" , 0.0 ).toFloat();
    m_turbulence    = settings.value( "turbulence" , 0.0 ).toFloat();

    m_visibility = settings.value( "visibility", CGI_SKYDOME_RADIUS ).toFloat();

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DialogEnvr::settingsRead_UnitCombos( QSettings &settings )
{
    settings.beginGroup( "unit_combos" );

    m_ui->comboSeaLevelTemp->setCurrentIndex( settings.value( "sea_level_temp", 1 ).toInt() );
    m_ui->comboSeaLevelPress->setCurrentIndex( settings.value( "sea_level_press", 1 ).toInt() );

    m_ui->comboWindDir->setCurrentIndex( settings.value( "wind_dir", 1 ).toInt() );
    m_ui->comboWindSpeed->setCurrentIndex( settings.value( "wind_speed", 0 ).toInt() );

    m_ui->comboVisibility->setCurrentIndex( settings.value( "visibility", 2 ).toInt() );

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

    settings.setValue( "sea_level_temp"  , m_temperature_0 );
    settings.setValue( "sea_level_press" , m_pressure_0 );

    settings.setValue( "wind_dir"   , m_windDirection );
    settings.setValue( "wind_speed" , m_windSpeed );
    settings.setValue( "turbulence" , m_turbulence );

    settings.setValue( "visibility", m_visibility );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DialogEnvr::settingsSave_UnitCombos( QSettings &settings )
{
    settings.beginGroup( "unit_combos" );

    settings.setValue( "sea_level_temp"  , m_ui->comboSeaLevelTemp->currentIndex() );
    settings.setValue( "sea_level_press" , m_ui->comboSeaLevelPress->currentIndex() );

    settings.setValue( "wind_dir"   , m_ui->comboWindDir->currentIndex() );
    settings.setValue( "wind_speed" , m_ui->comboWindSpeed->currentIndex() );

    settings.setValue( "visibility", m_ui->comboVisibility->currentIndex() );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DialogEnvr::on_comboSeaLevelTemp_currentIndexChanged( int /*index*/ )
{
    float value = m_ui->comboSeaLevelTemp->invertPrev( m_ui->spinBoxSeaLevelTemp->value() );

    m_ui->spinBoxSeaLevelTemp->setMinimum( m_ui->comboSeaLevelTemp->convert( 273.15f - 50.0f ) );
    m_ui->spinBoxSeaLevelTemp->setMaximum( m_ui->comboSeaLevelTemp->convert( 273.15f + 50.0f ) );

    m_ui->spinBoxSeaLevelTemp->setValue( m_ui->comboSeaLevelTemp->convert( value ) );
}

////////////////////////////////////////////////////////////////////////////////

void DialogEnvr::on_comboSeaLevelPress_currentIndexChanged( int index )
{
    float value = m_ui->comboSeaLevelPress->invertPrev( m_ui->spinBoxSeaLevelPress->value() );

    if ( index == 0 )
    {
        m_ui->spinBoxSeaLevelPress->setDecimals( 0 );
    }
    else if ( index == 2 )
    {
        m_ui->spinBoxSeaLevelPress->setDecimals( 5 );
    }
    else if ( index == 3 )
    {
        m_ui->spinBoxSeaLevelPress->setDecimals( 3 );
    }
    else
    {
        m_ui->spinBoxSeaLevelPress->setDecimals( 2 );
    }

    m_ui->spinBoxSeaLevelPress->setMinimum( m_ui->comboSeaLevelPress->convert( 101325.0f - 5000.0f ) );
    m_ui->spinBoxSeaLevelPress->setMaximum( m_ui->comboSeaLevelPress->convert( 101325.0f + 5000.0f ) );

    m_ui->spinBoxSeaLevelPress->setValue( m_ui->comboSeaLevelPress->convert( value ) );
}

////////////////////////////////////////////////////////////////////////////////

void DialogEnvr::on_comboWindDir_currentIndexChanged( int index )
{
    float value = m_ui->comboWindDir->invertPrev( m_ui->spinBoxWindDir->value() );

    if ( index == 0 )
    {
        // rad
        m_ui->spinBoxWindDir->setDecimals( 2 );
        m_ui->spinBoxWindDir->setMaximum( 2.0 * M_PI );
    }
    else
    {
        // deg
        m_ui->spinBoxWindDir->setDecimals( 0 );
        m_ui->spinBoxWindDir->setMaximum( 360.0 );
    }

    m_ui->spinBoxWindDir->setValue( m_ui->comboWindDir->convert( value ) );
}

////////////////////////////////////////////////////////////////////////////////

void DialogEnvr::on_comboWindSpeed_currentIndexChanged( int /*index*/ )
{
    float value = m_ui->comboWindSpeed->invertPrev( m_ui->spinBoxWindSpeed->value() );

    m_ui->spinBoxWindSpeed->setMaximum( m_ui->comboWindSpeed->convert( 30.0f ) );
    m_ui->spinBoxWindSpeed->setValue( m_ui->comboWindSpeed->convert( value ) );
}

////////////////////////////////////////////////////////////////////////////////

void DialogEnvr::on_comboVisibility_currentIndexChanged( int index )
{
    float value = 100.0f * m_ui->sliderVisibility->value();

    if ( index == 0 || index == 1 )
    {
        m_ui->spinBoxVisibility->setDecimals( 0 );
    }
    else
    {
        m_ui->spinBoxVisibility->setDecimals( 1 );
    }

    m_ui->spinBoxVisibility->setMaximum( m_ui->comboVisibility->convert( CGI_SKYDOME_RADIUS ) );
    m_ui->spinBoxVisibility->setValue( m_ui->comboVisibility->convert( value ) );
}

////////////////////////////////////////////////////////////////////////////////

void DialogEnvr::on_sliderTurbulence_valueChanged( int value )
{
    m_ui->spinBoxTurbulence->setValue( value );
}

////////////////////////////////////////////////////////////////////////////////

void DialogEnvr::on_sliderVisibility_valueChanged( int value )
{
    m_ui->spinBoxVisibility->setValue( m_ui->comboVisibility->convert( 100.0f * value ) );
}
