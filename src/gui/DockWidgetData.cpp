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
    _ui ( new Ui::DockWidgetData )
{
    _ui->setupUi( this );

    reset();

    settingsRead();
}

////////////////////////////////////////////////////////////////////////////////

DockWidgetData::~DockWidgetData()
{
    settingsSave();
    
    DELETE( _ui );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::reset()
{
    _altitude_asl = 0.0;
    _altitude_agl = 0.0;

    _airspeed   = 0.0;
    _climbRate  = 0.0;
    _machNumber = 0.0;

    _rollAngle  = 0.0;
    _pitchAngle = 0.0;
    _heading    = 0.0;

    _angleOfAttack = 0.0;
    _sideslipAngle = 0.0;

    _course    = 0.0;
    _pathAngle = 0.0;

    _rollRate  = 0.0;
    _pitchRate = 0.0;
    _yawRate   = 0.0;

    _Gx = 0.0;
    _Gy = 0.0;
    _Gz = 0.0;

    _maxGz = 0.0;

    setAltitudeASL( _altitude_asl );
    setAltitudeAGL( _altitude_agl );
    setAirspeed( _airspeed );
    setClimbRate( _climbRate );
    setMachNumber( _machNumber );
    setRollAngle( _rollAngle );
    setPitchAngle( _pitchAngle );
    setHeading( _heading );
    setAngleOfAttack( _angleOfAttack );
    setSideslipAngle( _sideslipAngle );
    setCourse( _course );
    setPathAngle( _pathAngle );
    setRollRate( _rollRate );
    setPitchRate( _pitchRate );
    setYawRate( _yawRate );
    setGx( _Gx );
    setGy( _Gy );
    setGz( _Gz );
    _ui->spinMaxGz->setValue( _ui->comboMaxGz->convert( _maxGz ) );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::setAltitudeASL( double altitude_asl )
{
    _altitude_asl = altitude_asl;
    _ui->spinAltitudeASL->setValue( _ui->comboAltitudeASL->convert( _altitude_asl ) );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::setAltitudeAGL( double altitude_agl )
{
    _altitude_agl = altitude_agl;
    _ui->spinAltitudeAGL->setValue( _ui->comboAltitudeAGL->convert( _altitude_agl ) );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::setAirspeed( double airspeed )
{
    _airspeed = airspeed;
    _ui->spinAirspeed->setValue( _ui->comboAirspeed->convert( _airspeed ) );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::setClimbRate( double climbRate )
{
    _climbRate = climbRate;
    _ui->spinClimbRate->setValue( _ui->comboClimbRate->convert( _climbRate ) );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::setMachNumber( double machNumber )
{
    _machNumber = machNumber;
    _ui->spinMachNumber->setValue( _ui->comboMachNumber->convert( _machNumber ) );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::setRollAngle( double rollAngle )
{
    _rollAngle = rollAngle;
    _ui->spinRollAngle->setValue( _ui->comboRollAngle->convert( _rollAngle ) );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::setPitchAngle( double pitchAngle )
{
    _pitchAngle = pitchAngle;
    _ui->spinPitchAngle->setValue( _ui->comboPitchAngle->convert( _pitchAngle ) );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::setHeading( double heading )
{
    _heading = heading;
    _ui->spinHeading->setValue( _ui->comboHeading->convert( _heading ) );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::setAngleOfAttack( double angleOfAttack )
{
    _angleOfAttack = angleOfAttack;
    _ui->spinAngleOfAttack->setValue( _ui->comboAngleOfAttack->convert( _angleOfAttack ) );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::setSideslipAngle( double sideslipAngle )
{
    _sideslipAngle = sideslipAngle;
    _ui->spinSideslipAngle->setValue( _ui->comboSideslipAngle->convert( _sideslipAngle ) );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::setCourse( double course )
{
    _course = course;
    _ui->spinCourse->setValue( _ui->comboCourse->convert( _course ) );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::setPathAngle( double pathAngle )
{
    _pathAngle = pathAngle;
    _ui->spinPathAngle->setValue( _ui->comboPathAngle->convert( _pathAngle ) );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::setRollRate( double rollRate )
{
    _rollRate = rollRate;
    _ui->spinRollRate->setValue( _ui->comboRollRate->convert( _rollRate ) );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::setPitchRate( double pitchRate )
{
    _pitchRate = pitchRate;
    _ui->spinPitchRate->setValue( _ui->comboPitchRate->convert( _pitchRate ) );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::setYawRate( double yawRate )
{
    _yawRate = yawRate;
    _ui->spinYawRate->setValue( _ui->comboYawRate->convert( _yawRate ) );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::setGx( double Gx )
{
    _Gx = Gx;
    _ui->spinGx->setValue( _ui->comboGx->convert( _Gx ) );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::setGy( double Gy )
{
    _Gy = Gy;
    _ui->spinGy->setValue( _ui->comboGy->convert( _Gy ) );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::setGz( double Gz )
{
    _Gz = Gz;

    if ( fabs(_Gz) > fabs(_maxGz) )
    {
        _maxGz = _Gz;
    }

    _ui->spinGz->setValue( _ui->comboGz->convert( _Gz ) );
    _ui->spinMaxGz->setValue( _ui->comboMaxGz->convert( _maxGz ) );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::closeEvent( QCloseEvent *event )
{
    /////////////////////////////////
    QDockWidget::closeEvent( event );
    /////////////////////////////////

    emit closed();
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

    _ui->spinAltitudeASL   ->setHighlighted( settings.value( "altitude_asl"    , 0 ).toBool() );
    _ui->spinAltitudeAGL   ->setHighlighted( settings.value( "altitude_agl"    , 0 ).toBool() );
    _ui->spinAirspeed      ->setHighlighted( settings.value( "airspeed"        , 0 ).toBool() );
    _ui->spinClimbRate     ->setHighlighted( settings.value( "climb_rate"      , 0 ).toBool() );
    _ui->spinMachNumber    ->setHighlighted( settings.value( "mach_number"     , 0 ).toBool() );
    _ui->spinRollAngle     ->setHighlighted( settings.value( "roll_angle"      , 0 ).toBool() );
    _ui->spinPitchAngle    ->setHighlighted( settings.value( "pitch_angle"     , 0 ).toBool() );
    _ui->spinHeading       ->setHighlighted( settings.value( "heading"         , 0 ).toBool() );
    _ui->spinAngleOfAttack ->setHighlighted( settings.value( "angle_of_attack" , 0 ).toBool() );
    _ui->spinSideslipAngle ->setHighlighted( settings.value( "sideslip_angle"  , 0 ).toBool() );
    _ui->spinPathAngle     ->setHighlighted( settings.value( "path_angle"      , 0 ).toBool() );
    _ui->spinCourse        ->setHighlighted( settings.value( "course"          , 0 ).toBool() );
    _ui->spinRollRate      ->setHighlighted( settings.value( "roll_rate"       , 0 ).toBool() );
    _ui->spinPitchRate     ->setHighlighted( settings.value( "pitch_rate"      , 0 ).toBool() );
    _ui->spinYawRate       ->setHighlighted( settings.value( "yaw_rate"        , 0 ).toBool() );
    _ui->spinGx            ->setHighlighted( settings.value( "gx"              , 0 ).toBool() );
    _ui->spinGy            ->setHighlighted( settings.value( "gy"              , 0 ).toBool() );
    _ui->spinGz            ->setHighlighted( settings.value( "gz"              , 0 ).toBool() );
    _ui->spinMaxGz         ->setHighlighted( settings.value( "max_gz"          , 0 ).toBool() );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::settingsRead_UnitCombos( QSettings &settings )
{
    settings.beginGroup( "unit_combos" );

    _ui->comboAltitudeASL   ->setCurrentIndex( settings.value( "altitude_asl"    , 0 ).toInt() );
    _ui->comboAltitudeAGL   ->setCurrentIndex( settings.value( "altitude_agl"    , 0 ).toInt() );
    _ui->comboAirspeed      ->setCurrentIndex( settings.value( "airspeed"        , 0 ).toInt() );
    _ui->comboClimbRate     ->setCurrentIndex( settings.value( "climb_rate"      , 0 ).toInt() );
    _ui->comboMachNumber    ->setCurrentIndex( settings.value( "mach_number"     , 0 ).toInt() );
    _ui->comboRollAngle     ->setCurrentIndex( settings.value( "roll_angle"      , 1 ).toInt() );
    _ui->comboPitchAngle    ->setCurrentIndex( settings.value( "pitch_angle"     , 1 ).toInt() );
    _ui->comboHeading       ->setCurrentIndex( settings.value( "heading"         , 1 ).toInt() );
    _ui->comboAngleOfAttack ->setCurrentIndex( settings.value( "angle_of_attack" , 1 ).toInt() );
    _ui->comboSideslipAngle ->setCurrentIndex( settings.value( "sideslip_angle"  , 1 ).toInt() );
    _ui->comboPathAngle     ->setCurrentIndex( settings.value( "path_angle"      , 1 ).toInt() );
    _ui->comboCourse        ->setCurrentIndex( settings.value( "course"          , 1 ).toInt() );
    _ui->comboRollRate      ->setCurrentIndex( settings.value( "roll_rate"       , 1 ).toInt() );
    _ui->comboPitchRate     ->setCurrentIndex( settings.value( "pitch_rate"      , 1 ).toInt() );
    _ui->comboYawRate       ->setCurrentIndex( settings.value( "yaw_rate"        , 1 ).toInt() );
    _ui->comboGx            ->setCurrentIndex( settings.value( "gx"              , 0 ).toInt() );
    _ui->comboGy            ->setCurrentIndex( settings.value( "gy"              , 0 ).toInt() );
    _ui->comboGz            ->setCurrentIndex( settings.value( "gz"              , 0 ).toInt() );
    _ui->comboMaxGz         ->setCurrentIndex( settings.value( "max_gz"          , 0 ).toInt() );

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

    settings.setValue( "altitude_asl"    , _ui->spinAltitudeASL   ->isHighlighted() ? 1 : 0 );
    settings.setValue( "altitude_agl"    , _ui->spinAltitudeAGL   ->isHighlighted() ? 1 : 0 );
    settings.setValue( "airspeed"        , _ui->spinAirspeed      ->isHighlighted() ? 1 : 0 );
    settings.setValue( "climb_rate"      , _ui->spinClimbRate     ->isHighlighted() ? 1 : 0 );
    settings.setValue( "mach_number"     , _ui->spinMachNumber    ->isHighlighted() ? 1 : 0 );
    settings.setValue( "roll_angle"      , _ui->spinRollAngle     ->isHighlighted() ? 1 : 0 );
    settings.setValue( "pitch_angle"     , _ui->spinPitchAngle    ->isHighlighted() ? 1 : 0 );
    settings.setValue( "heading"         , _ui->spinHeading       ->isHighlighted() ? 1 : 0 );
    settings.setValue( "angle_of_attack" , _ui->spinAngleOfAttack ->isHighlighted() ? 1 : 0 );
    settings.setValue( "sideslip_angle"  , _ui->spinSideslipAngle ->isHighlighted() ? 1 : 0 );
    settings.setValue( "path_angle"      , _ui->spinPathAngle     ->isHighlighted() ? 1 : 0 );
    settings.setValue( "course"          , _ui->spinCourse        ->isHighlighted() ? 1 : 0 );
    settings.setValue( "roll_rate"       , _ui->spinRollRate      ->isHighlighted() ? 1 : 0 );
    settings.setValue( "pitch_rate"      , _ui->spinPitchRate     ->isHighlighted() ? 1 : 0 );
    settings.setValue( "yaw_rate"        , _ui->spinYawRate       ->isHighlighted() ? 1 : 0 );
    settings.setValue( "gx"              , _ui->spinGx            ->isHighlighted() ? 1 : 0 );
    settings.setValue( "gy"              , _ui->spinGy            ->isHighlighted() ? 1 : 0 );
    settings.setValue( "gz"              , _ui->spinGz            ->isHighlighted() ? 1 : 0 );
    settings.setValue( "max_gz"          , _ui->spinMaxGz         ->isHighlighted() ? 1 : 0 );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::settingsSave_UnitCombos( QSettings &settings )
{
    settings.beginGroup( "unit_combos" );

    settings.setValue( "altitude_asl"    , _ui->comboAltitudeASL   ->currentIndex() );
    settings.setValue( "altitude_agl"    , _ui->comboAltitudeAGL   ->currentIndex() );
    settings.setValue( "airspeed"        , _ui->comboAirspeed      ->currentIndex() );
    settings.setValue( "climb_rate"      , _ui->comboClimbRate     ->currentIndex() );
    settings.setValue( "mach_number"     , _ui->comboMachNumber    ->currentIndex() );
    settings.setValue( "roll_angle"      , _ui->comboRollAngle     ->currentIndex() );
    settings.setValue( "pitch_angle"     , _ui->comboPitchAngle    ->currentIndex() );
    settings.setValue( "heading"         , _ui->comboHeading       ->currentIndex() );
    settings.setValue( "angle_of_attack" , _ui->comboAngleOfAttack ->currentIndex() );
    settings.setValue( "sideslip_angle"  , _ui->comboSideslipAngle ->currentIndex() );
    settings.setValue( "path_angle"      , _ui->comboPathAngle     ->currentIndex() );
    settings.setValue( "course"          , _ui->comboCourse        ->currentIndex() );
    settings.setValue( "roll_rate"       , _ui->comboRollRate      ->currentIndex() );
    settings.setValue( "pitch_rate"      , _ui->comboPitchRate     ->currentIndex() );
    settings.setValue( "yaw_rate"        , _ui->comboYawRate       ->currentIndex() );
    settings.setValue( "gx"              , _ui->comboGx            ->currentIndex() );
    settings.setValue( "gy"              , _ui->comboGy            ->currentIndex() );
    settings.setValue( "gz"              , _ui->comboGz            ->currentIndex() );
    settings.setValue( "max_gz"          , _ui->comboMaxGz         ->currentIndex() );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::on_comboAltitudeASL_currentIndexChanged( int /*index*/ )
{
    setAltitudeASL( _altitude_asl );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::on_comboAltitudeAGL_currentIndexChanged( int /*index*/ )
{
    setAltitudeAGL( _altitude_agl );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::on_comboAirspeed_currentIndexChanged( int /*index*/ )
{
    setAirspeed( _airspeed );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::on_comboClimbRate_currentIndexChanged( int /*index*/ )
{
    setClimbRate( _climbRate );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::on_comboMachNumber_currentIndexChanged( int /*index*/ )
{
    setMachNumber( _machNumber );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::on_comboRollAngle_currentIndexChanged( int /*index*/ )
{
    setRollAngle( _rollAngle );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::on_comboPitchAngle_currentIndexChanged( int /*index*/ )
{
    setPitchAngle( _pitchAngle );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::on_comboHeading_currentIndexChanged( int /*index*/ )
{
    setHeading( _heading );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::on_comboAngleOfAttack_currentIndexChanged( int /*index*/ )
{
    setAngleOfAttack( _angleOfAttack );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::on_comboSideslipAngle_currentIndexChanged( int /*index*/ )
{
    setSideslipAngle( _sideslipAngle );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::on_comboCourse_currentIndexChanged( int /*index*/ )
{
    setCourse( _course );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::on_comboPathAngle_currentIndexChanged( int /*index*/ )
{
    setPathAngle( _pathAngle );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::on_comboRollRate_currentIndexChanged( int /*index*/ )
{
    setRollRate( _rollRate );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::on_comboPitchRate_currentIndexChanged( int /*index*/ )
{
    setPitchRate( _pitchRate );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::on_comboYawRate_currentIndexChanged( int /*index*/ )
{
    setYawRate( _yawRate );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::on_comboGx_currentIndexChanged( int /*index*/ )
{
    setGx( _Gx );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::on_comboGy_currentIndexChanged( int /*index*/ )
{
    setGy( _Gy );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::on_comboGz_currentIndexChanged( int /*index*/ )
{
    setGz( _Gz );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetData::on_comboMaxGz_currentIndexChanged( int /*index*/ )
{
    setGz( _Gz );
}
