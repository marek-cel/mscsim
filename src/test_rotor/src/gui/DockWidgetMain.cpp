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

#include <gui/DockWidgetMain.h>
#include <ui_DockWidgetMain.h>

#include <QSettings>

#include <fdm/fdm_Log.h>
#include <fdm/utils/fdm_Units.h>

#include <Data.h>
#include <defs.h>

////////////////////////////////////////////////////////////////////////////////

DockWidgetMain::DockWidgetMain( QWidget *parent ) :
    QDockWidget ( parent ),
    _ui ( new Ui::DockWidgetMain ),
    _dockTest ( nullptr ),
    _widgetCGI ( nullptr ),
    _test ( nullptr ),
    _ccw ( false )
{
    _ui->setupUi( this );

    _test = new fdm::Test();

    settingsRead();
}

////////////////////////////////////////////////////////////////////////////////

DockWidgetMain::~DockWidgetMain()
{
    settingsSave();

    if ( _test ) delete _test;
    _test = nullptr;

    if ( _ui ) delete _ui;
    _ui = nullptr;
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::update( double timeStep )
{
    _ui->spinBoxTimeStep->setValue( timeStep );

    Data::get()->blades_count = _ui->spinBoxBladesCount->value();
    Data::get()->rotor.direction = _ccw ? Data::Rotor::CCW : Data::Rotor::CW;

    Data::get()->other.scale_main = 1.0;
    Data::get()->other.scale_span = _ui->spinBoxScaleSpan->value();

    if ( !Data::get()->test )
    {
        Data::get()->state.bas_u = _ui->spinBoxVelU->value();
        Data::get()->state.bas_v = _ui->spinBoxVelV->value();
        Data::get()->state.bas_w = _ui->spinBoxVelW->value();

        Data::get()->state.bas_p = fdm::Units::deg2rad( _ui->spinBoxOmgP->value() );
        Data::get()->state.bas_q = fdm::Units::deg2rad( _ui->spinBoxOmgQ->value() );
        Data::get()->state.bas_r = fdm::Units::deg2rad( _ui->spinBoxOmgR->value() );

        Data::get()->state.ned_psi = fdm::Units::deg2rad( _ui->spinBoxAttPsi->value() );
        Data::get()->state.ned_tht = fdm::Units::deg2rad( _ui->spinBoxAttTht->value() );
        Data::get()->state.ned_phi = fdm::Units::deg2rad( _ui->spinBoxAttPhi->value() );

        Data::get()->state.wind_vel = _ui->spinBoxWindVel->value();
        Data::get()->state.wind_dir = fdm::Units::deg2rad( _ui->spinBoxWindDir->value() );

        if ( _test )
        {
            if ( _ui->pushButtonWork->isChecked() )
            {
                _test->update( timeStep );
            }

            _test->updateData();
        }
        else
        {
            for ( int i = 0; i < 3; i++ )
            {

            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

double DockWidgetMain::getTimeCoef()
{
    return _ui->spinBoxTimeCoef->value();
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::setDockTest( DockWidgetTest *dockTest )
{
    _dockTest = dockTest;
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::setWidgetCGI( WidgetCGI *widgetCGI )
{
    _widgetCGI = widgetCGI;
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::closeEvent( QCloseEvent *event )
{
    /////////////////////////////////
    QDockWidget::closeEvent( event );
    /////////////////////////////////

    emit closed();
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::settingsRead()
{
    QSettings settings( TEST_ORG_NAME, TEST_APP_NAME );

    settings.beginGroup( "dock_main" );

    double t_coef = settings.value( "t_coef", 1.0 ).toDouble();

    _ui->spinBoxTimeCoef->setValue( t_coef );

    double scale_span = settings.value( "scale_span", 1.0 ).toDouble();

    _ui->spinBoxScaleSpan->setValue( scale_span );

    int blades_count = settings.value( "blades_count", 4 ).toInt();

    _ui->spinBoxBladesCount->setValue( blades_count );

    bool ccw = settings.value( "ccw", 0 ).toBool();

    _ui->radioButtonCW->setChecked( !ccw );
    _ui->radioButtonCCW->setChecked( ccw );

    _ccw = ccw;

    double bas_u = settings.value( "bas_u", 0.0 ).toDouble();
    double bas_v = settings.value( "bas_v", 0.0 ).toDouble();
    double bas_w = settings.value( "bas_w", 0.0 ).toDouble();

    _ui->spinBoxVelU->setValue( bas_u );
    _ui->spinBoxVelV->setValue( bas_v );
    _ui->spinBoxVelW->setValue( bas_w );

    double bas_p = settings.value( "bas_p", 0.0 ).toDouble();
    double bas_q = settings.value( "bas_q", 0.0 ).toDouble();
    double bas_r = settings.value( "bas_r", 0.0 ).toDouble();

    _ui->spinBoxOmgP->setValue( bas_p );
    _ui->spinBoxOmgQ->setValue( bas_q );
    _ui->spinBoxOmgR->setValue( bas_r );

    double ned_psi = settings.value( "ned_psi", 0.0 ).toDouble();
    double ned_tht = settings.value( "ned_tht", 0.0 ).toDouble();
    double ned_phi = settings.value( "ned_phi", 0.0 ).toDouble();

    _ui->spinBoxAttPsi->setValue( ned_psi );
    _ui->spinBoxAttTht->setValue( ned_tht );
    _ui->spinBoxAttPhi->setValue( ned_phi );

    double wind_vel = settings.value( "wind_vel", 0.0 ).toDouble();
    double wind_dir = settings.value( "wind_dir", 0.0 ).toDouble();

    _ui->spinBoxWindVel->setValue( wind_vel );
    _ui->spinBoxWindDir->setValue( wind_dir );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::settingsSave()
{
    QSettings settings( TEST_ORG_NAME, TEST_APP_NAME );

    settings.beginGroup( "dock_main" );

    settings.setValue( "t_coef", _ui->spinBoxTimeCoef->value() );

    settings.setValue( "scale_span", _ui->spinBoxScaleSpan->value() );

    settings.setValue( "blades_count", _ui->spinBoxBladesCount->value() );

    settings.setValue( "ccw", _ccw ? 1 : 0 );

    settings.setValue( "bas_u", _ui->spinBoxVelU->value() );
    settings.setValue( "bas_v", _ui->spinBoxVelV->value() );
    settings.setValue( "bas_w", _ui->spinBoxVelW->value() );

    settings.setValue( "bas_p", _ui->spinBoxOmgP->value() );
    settings.setValue( "bas_q", _ui->spinBoxOmgQ->value() );
    settings.setValue( "bas_r", _ui->spinBoxOmgR->value() );

    settings.setValue( "ned_psi", _ui->spinBoxAttPsi->value() );
    settings.setValue( "ned_tht", _ui->spinBoxAttTht->value() );
    settings.setValue( "ned_phi", _ui->spinBoxAttPhi->value() );

    settings.setValue( "wind_vel", _ui->spinBoxWindVel->value() );
    settings.setValue( "wind_dir", _ui->spinBoxWindDir->value() );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::on_pushButtonInit_clicked()
{
    Data::get()->phase = Data::Init;

    _ui->pushButtonWork->setEnabled( true );
    _ui->pushButtonPause->setEnabled( true );

    _ui->groupBoxDirection->setEnabled( false );

    _test->initBlade( _ccw ? fdm::MainRotor::CCW : fdm::MainRotor::CW,
                      _ui->spinBoxBladesCount->value() );

    if ( !_test->isInited() )
    {
        _ui->pushButtonStop->setChecked( true );
        _test->stop();
    }
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::on_pushButtonWork_clicked()
{
    Data::get()->phase = Data::Work;

    _ui->pushButtonInit->setEnabled( false );
    _ui->pushButtonPause->setEnabled( true );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::on_pushButtonPause_clicked()
{
    Data::get()->phase = Data::Pause;

    _ui->pushButtonInit->setEnabled( false );
    _ui->pushButtonWork->setEnabled( true );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::on_pushButtonStop_clicked()
{
    Data::get()->phase = Data::Stop;

    _ui->pushButtonInit->setEnabled( true );
    _ui->pushButtonWork->setEnabled( false );
    _ui->pushButtonPause->setEnabled( false );

    _ui->groupBoxDirection->setEnabled( true );

    _test->stop();

    if ( _widgetCGI )
    {
        _widgetCGI->rotationStop();
    }
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::on_radioButtonCCW_toggled(bool checked)
{
    _ccw = checked;

    if ( _dockTest )
    {
        _dockTest->setCCW( _ccw );
    }
}

