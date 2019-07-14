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

#include <gui/DockWidgetData.h>
#include <ui_DockWidgetData.h>

#include <math.h>

#include <gui/gui_Defines.h>

////////////////////////////////////////////////////////////////////////////////

DockWidgetData::DockWidgetData( QWidget *parent ) :
    QDockWidget ( parent ),
    m_ui ( new Ui::DockWidgetData )
{
    m_ui->setupUi( this );

    reset();

    settingsRead();
}

////////////////////////////////////////////////////////////////////////////////

DockWidgetData::~DockWidgetData()
{
    settingsSave();
    
    if ( m_ui ) delete m_ui;
    m_ui = 0;
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::reset()
{
    m_altitude_asl = 0.0;
    m_altitude_agl = 0.0;

    m_airspeed   = 0.0;
    m_climbRate  = 0.0;
    m_machNumber = 0.0;

    m_rollAngle  = 0.0;
    m_pitchAngle = 0.0;
    m_heading    = 0.0;

    m_angleOfAttack = 0.0;
    m_sideslipAngle = 0.0;

    m_course    = 0.0;
    m_pathAngle = 0.0;

    m_rollRate  = 0.0;
    m_pitchRate = 0.0;
    m_yawRate   = 0.0;

    m_Gx = 0.0;
    m_Gy = 0.0;
    m_Gz = 0.0;

    m_maxGz = 0.0;

    setAltitudeASL( m_altitude_asl );
    setAltitudeAGL( m_altitude_agl );
    setAirspeed( m_airspeed );
    setClimbRate( m_climbRate );
    setMachNumber( m_machNumber );
    setRollAngle( m_rollAngle );
    setPitchAngle( m_pitchAngle );
    setHeading( m_heading );
    setAngleOfAttack( m_angleOfAttack );
    setSideslipAngle( m_sideslipAngle );
    setCourse( m_course );
    setPathAngle( m_pathAngle );
    setRollRate( m_rollRate );
    setPitchRate( m_pitchRate );
    setYawRate( m_yawRate );
    setGx( m_Gx );
    setGy( m_Gy );
    setGz( m_Gz );
    m_ui->spinMaxGz->setValue( m_ui->comboMaxGz->convert( m_maxGz ) );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::setAltitudeASL( double altitude_asl )
{
    m_altitude_asl = altitude_asl;

    m_ui->spinAltitudeASL->setValue( m_ui->comboAltitudeASL->convert( m_altitude_asl ) );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::setAltitudeAGL( double altitude_agl )
{
    m_altitude_agl = altitude_agl;

    m_ui->spinAltitudeAGL->setValue( m_ui->comboAltitudeAGL->convert( m_altitude_agl ) );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::setAirspeed( double airspeed )
{
    m_airspeed = airspeed;

    m_ui->spinAirspeed->setValue( m_ui->comboAirspeed->convert( m_airspeed ) );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::setClimbRate( double climbRate )
{
    m_climbRate = climbRate;

    m_ui->spinClimbRate->setValue( m_ui->comboClimbRate->convert( m_climbRate ) );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::setMachNumber( double machNumber )
{
    m_machNumber = machNumber;

    m_ui->spinMachNumber->setValue( m_ui->comboMachNumber->convert( m_machNumber ) );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::setRollAngle( double rollAngle )
{
    m_rollAngle = rollAngle;

    m_ui->spinRollAngle->setValue( m_ui->comboRollAngle->convert( m_rollAngle ) );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::setPitchAngle( double pitchAngle )
{
    m_pitchAngle = pitchAngle;

    m_ui->spinPitchAngle->setValue( m_ui->comboPitchAngle->convert( m_pitchAngle ) );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::setHeading( double heading )
{
    m_heading = heading;

    m_ui->spinHeading->setValue( m_ui->comboHeading->convert( m_heading ) );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::setAngleOfAttack( double angleOfAttack )
{
    m_angleOfAttack = angleOfAttack;

    m_ui->spinAngleOfAttack->setValue( m_ui->comboAngleOfAttack->convert( m_angleOfAttack ) );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::setSideslipAngle( double sideslipAngle )
{
    m_sideslipAngle = sideslipAngle;

    m_ui->spinSideslipAngle->setValue( m_ui->comboSideslipAngle->convert( m_sideslipAngle ) );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::setCourse( double course )
{
    m_course = course;

    m_ui->spinCourse->setValue( m_ui->comboCourse->convert( m_course ) );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::setPathAngle( double pathAngle )
{
    m_pathAngle = pathAngle;

    m_ui->spinPathAngle->setValue( m_ui->comboPathAngle->convert( m_pathAngle ) );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::setRollRate( double rollRate )
{
    m_rollRate = rollRate;

    m_ui->spinRollRate->setValue( m_ui->comboRollRate->convert( m_rollRate ) );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::setPitchRate( double pitchRate )
{
    m_pitchRate = pitchRate;

    m_ui->spinPitchRate->setValue( m_ui->comboPitchRate->convert( m_pitchRate ) );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::setYawRate( double yawRate )
{
    m_yawRate = yawRate;

    m_ui->spinYawRate->setValue( m_ui->comboYawRate->convert( m_yawRate ) );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::setGx( double Gx )
{
    m_Gx = Gx;

    m_ui->spinGx->setValue( m_ui->comboGx->convert( m_Gx ) );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::setGy( double Gy )
{
    m_Gy = Gy;

    m_ui->spinGy->setValue( m_ui->comboGy->convert( m_Gy ) );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::setGz( double Gz )
{
    m_Gz = Gz;

    if ( fabs(m_Gz) > fabs(m_maxGz) )
    {
        m_maxGz = m_Gz;
    }

    m_ui->spinGz->setValue( m_ui->comboGz->convert( m_Gz ) );
    m_ui->spinMaxGz->setValue( m_ui->comboMaxGz->convert( m_maxGz ) );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::settingsRead()
{
    QSettings settings( SIM_ORG_NAME, SIM_APP_NAME );
    
    settings.beginGroup( "dock_widget_data" );

    settingsRead_Highlight( settings );
    settingsRead_UnitCombos( settings );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::settingsRead_Highlight( QSettings &settings )
{
    settings.beginGroup( "highlight" );

    m_ui->spinAltitudeASL   ->setHighlighted( settings.value( "altitude_asl"    , 0 ).toBool() );
    m_ui->spinAltitudeAGL   ->setHighlighted( settings.value( "altitude_agl"    , 0 ).toBool() );
    m_ui->spinAirspeed      ->setHighlighted( settings.value( "airspeed"        , 0 ).toBool() );
    m_ui->spinClimbRate     ->setHighlighted( settings.value( "climb_rate"      , 0 ).toBool() );
    m_ui->spinMachNumber    ->setHighlighted( settings.value( "mach_number"     , 0 ).toBool() );
    m_ui->spinRollAngle     ->setHighlighted( settings.value( "roll_angle"      , 0 ).toBool() );
    m_ui->spinPitchAngle    ->setHighlighted( settings.value( "pitch_angle"     , 0 ).toBool() );
    m_ui->spinHeading       ->setHighlighted( settings.value( "heading"         , 0 ).toBool() );
    m_ui->spinAngleOfAttack ->setHighlighted( settings.value( "angle_of_attack" , 0 ).toBool() );
    m_ui->spinSideslipAngle ->setHighlighted( settings.value( "sideslip_angle"  , 0 ).toBool() );
    m_ui->spinPathAngle     ->setHighlighted( settings.value( "path_angle"      , 0 ).toBool() );
    m_ui->spinCourse        ->setHighlighted( settings.value( "course"          , 0 ).toBool() );
    m_ui->spinRollRate      ->setHighlighted( settings.value( "roll_rate"       , 0 ).toBool() );
    m_ui->spinPitchRate     ->setHighlighted( settings.value( "pitch_rate"      , 0 ).toBool() );
    m_ui->spinYawRate       ->setHighlighted( settings.value( "yaw_rate"        , 0 ).toBool() );
    m_ui->spinGx            ->setHighlighted( settings.value( "gx"              , 0 ).toBool() );
    m_ui->spinGy            ->setHighlighted( settings.value( "gy"              , 0 ).toBool() );
    m_ui->spinGz            ->setHighlighted( settings.value( "gz"              , 0 ).toBool() );
    m_ui->spinMaxGz         ->setHighlighted( settings.value( "max_gz"          , 0 ).toBool() );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::settingsRead_UnitCombos( QSettings &settings )
{
    settings.beginGroup( "unit_combos" );

    m_ui->comboAltitudeASL   ->setCurrentIndex( settings.value( "altitude_asl"    , 0 ).toInt() );
    m_ui->comboAltitudeAGL   ->setCurrentIndex( settings.value( "altitude_agl"    , 0 ).toInt() );
    m_ui->comboAirspeed      ->setCurrentIndex( settings.value( "airspeed"        , 0 ).toInt() );
    m_ui->comboClimbRate     ->setCurrentIndex( settings.value( "climb_rate"      , 0 ).toInt() );
    m_ui->comboMachNumber    ->setCurrentIndex( settings.value( "mach_number"     , 0 ).toInt() );
    m_ui->comboRollAngle     ->setCurrentIndex( settings.value( "roll_angle"      , 1 ).toInt() );
    m_ui->comboPitchAngle    ->setCurrentIndex( settings.value( "pitch_angle"     , 1 ).toInt() );
    m_ui->comboHeading       ->setCurrentIndex( settings.value( "heading"         , 1 ).toInt() );
    m_ui->comboAngleOfAttack ->setCurrentIndex( settings.value( "angle_of_attack" , 1 ).toInt() );
    m_ui->comboSideslipAngle ->setCurrentIndex( settings.value( "sideslip_angle"  , 1 ).toInt() );
    m_ui->comboPathAngle     ->setCurrentIndex( settings.value( "path_angle"      , 1 ).toInt() );
    m_ui->comboCourse        ->setCurrentIndex( settings.value( "course"          , 1 ).toInt() );
    m_ui->comboRollRate      ->setCurrentIndex( settings.value( "roll_rate"       , 1 ).toInt() );
    m_ui->comboPitchRate     ->setCurrentIndex( settings.value( "pitch_rate"      , 1 ).toInt() );
    m_ui->comboYawRate       ->setCurrentIndex( settings.value( "yaw_rate"        , 1 ).toInt() );
    m_ui->comboGx            ->setCurrentIndex( settings.value( "gx"              , 0 ).toInt() );
    m_ui->comboGy            ->setCurrentIndex( settings.value( "gy"              , 0 ).toInt() );
    m_ui->comboGz            ->setCurrentIndex( settings.value( "gz"              , 0 ).toInt() );
    m_ui->comboMaxGz         ->setCurrentIndex( settings.value( "max_gz"          , 0 ).toInt() );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::settingsSave()
{
    QSettings settings( SIM_ORG_NAME, SIM_APP_NAME );

    settings.beginGroup( "dock_widget_data" );

    settingsSave_Highlight( settings );
    settingsSave_UnitCombos( settings );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::settingsSave_Highlight( QSettings &settings )
{
    settings.beginGroup( "highlight" );

    settings.setValue( "altitude_asl"    , m_ui->spinAltitudeASL   ->isHighlighted() ? 1 : 0 );
    settings.setValue( "altitude_agl"    , m_ui->spinAltitudeAGL   ->isHighlighted() ? 1 : 0 );
    settings.setValue( "airspeed"        , m_ui->spinAirspeed      ->isHighlighted() ? 1 : 0 );
    settings.setValue( "climb_rate"      , m_ui->spinClimbRate     ->isHighlighted() ? 1 : 0 );
    settings.setValue( "mach_number"     , m_ui->spinMachNumber    ->isHighlighted() ? 1 : 0 );
    settings.setValue( "roll_angle"      , m_ui->spinRollAngle     ->isHighlighted() ? 1 : 0 );
    settings.setValue( "pitch_angle"     , m_ui->spinPitchAngle    ->isHighlighted() ? 1 : 0 );
    settings.setValue( "heading"         , m_ui->spinHeading       ->isHighlighted() ? 1 : 0 );
    settings.setValue( "angle_of_attack" , m_ui->spinAngleOfAttack ->isHighlighted() ? 1 : 0 );
    settings.setValue( "sideslip_angle"  , m_ui->spinSideslipAngle ->isHighlighted() ? 1 : 0 );
    settings.setValue( "path_angle"      , m_ui->spinPathAngle     ->isHighlighted() ? 1 : 0 );
    settings.setValue( "course"          , m_ui->spinCourse        ->isHighlighted() ? 1 : 0 );
    settings.setValue( "roll_rate"       , m_ui->spinRollRate      ->isHighlighted() ? 1 : 0 );
    settings.setValue( "pitch_rate"      , m_ui->spinPitchRate     ->isHighlighted() ? 1 : 0 );
    settings.setValue( "yaw_rate"        , m_ui->spinYawRate       ->isHighlighted() ? 1 : 0 );
    settings.setValue( "gx"              , m_ui->spinGx            ->isHighlighted() ? 1 : 0 );
    settings.setValue( "gy"              , m_ui->spinGy            ->isHighlighted() ? 1 : 0 );
    settings.setValue( "gz"              , m_ui->spinGz            ->isHighlighted() ? 1 : 0 );
    settings.setValue( "max_gz"          , m_ui->spinMaxGz         ->isHighlighted() ? 1 : 0 );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::settingsSave_UnitCombos( QSettings &settings )
{
    settings.beginGroup( "unit_combos" );

    settings.setValue( "altitude_asl"    , m_ui->comboAltitudeASL   ->currentIndex() );
    settings.setValue( "altitude_agl"    , m_ui->comboAltitudeAGL   ->currentIndex() );
    settings.setValue( "airspeed"        , m_ui->comboAirspeed      ->currentIndex() );
    settings.setValue( "climb_rate"      , m_ui->comboClimbRate     ->currentIndex() );
    settings.setValue( "mach_number"     , m_ui->comboMachNumber    ->currentIndex() );
    settings.setValue( "roll_angle"      , m_ui->comboRollAngle     ->currentIndex() );
    settings.setValue( "pitch_angle"     , m_ui->comboPitchAngle    ->currentIndex() );
    settings.setValue( "heading"         , m_ui->comboHeading       ->currentIndex() );
    settings.setValue( "angle_of_attack" , m_ui->comboAngleOfAttack ->currentIndex() );
    settings.setValue( "sideslip_angle"  , m_ui->comboSideslipAngle ->currentIndex() );
    settings.setValue( "path_angle"      , m_ui->comboPathAngle     ->currentIndex() );
    settings.setValue( "course"          , m_ui->comboCourse        ->currentIndex() );
    settings.setValue( "roll_rate"       , m_ui->comboRollRate      ->currentIndex() );
    settings.setValue( "pitch_rate"      , m_ui->comboPitchRate     ->currentIndex() );
    settings.setValue( "yaw_rate"        , m_ui->comboYawRate       ->currentIndex() );
    settings.setValue( "gx"              , m_ui->comboGx            ->currentIndex() );
    settings.setValue( "gy"              , m_ui->comboGy            ->currentIndex() );
    settings.setValue( "gz"              , m_ui->comboGz            ->currentIndex() );
    settings.setValue( "max_gz"          , m_ui->comboMaxGz         ->currentIndex() );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::on_comboAltitudeASL_currentIndexChanged( int /*index*/ )
{
    setAltitudeASL( m_altitude_asl );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::on_comboAltitudeAGL_currentIndexChanged( int /*index*/ )
{
    setAltitudeAGL( m_altitude_agl );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::on_comboAirspeed_currentIndexChanged( int /*index*/ )
{
    setAirspeed( m_airspeed );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::on_comboClimbRate_currentIndexChanged( int /*index*/ )
{
    setClimbRate( m_climbRate );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::on_comboMachNumber_currentIndexChanged( int /*index*/ )
{
    setMachNumber( m_machNumber );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::on_comboRollAngle_currentIndexChanged( int /*index*/ )
{
    setRollAngle( m_rollAngle );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::on_comboPitchAngle_currentIndexChanged( int /*index*/ )
{
    setPitchAngle( m_pitchAngle );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::on_comboHeading_currentIndexChanged( int /*index*/ )
{
    setHeading( m_heading );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::on_comboAngleOfAttack_currentIndexChanged( int /*index*/ )
{
    setAngleOfAttack( m_angleOfAttack );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::on_comboSideslipAngle_currentIndexChanged( int /*index*/ )
{
    setSideslipAngle( m_sideslipAngle );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::on_comboCourse_currentIndexChanged( int /*index*/ )
{
    setCourse( m_course );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::on_comboPathAngle_currentIndexChanged( int /*index*/ )
{
    setPathAngle( m_pathAngle );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::on_comboRollRate_currentIndexChanged( int /*index*/ )
{
    setRollRate( m_rollRate );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::on_comboPitchRate_currentIndexChanged( int /*index*/ )
{
    setPitchRate( m_pitchRate );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::on_comboYawRate_currentIndexChanged( int /*index*/ )
{
    setYawRate( m_yawRate );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::on_comboGx_currentIndexChanged( int /*index*/ )
{
    setGx( m_Gx );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::on_comboGy_currentIndexChanged( int /*index*/ )
{
    setGy( m_Gy );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::on_comboGz_currentIndexChanged( int /*index*/ )
{
    setGz( m_Gz );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::on_comboMaxGz_currentIndexChanged( int /*index*/ )
{
    setGz( m_Gz );
}
