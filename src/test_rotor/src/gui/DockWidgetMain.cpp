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
    _test ( nullptr ),
    _ccw ( false )
{
    _ui->setupUi( this );

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
    Data::get()->rotor.direction = _ccw ? Data::Rotor::CCW : Data::Rotor::CW;

    if ( !Data::get()->test )
    {
        Data::get()->state.ned_psi = fdm::Units::deg2rad( _ui->spinBoxAttPsi->value() );
        Data::get()->state.ned_tht = fdm::Units::deg2rad( _ui->spinBoxAttTht->value() );
        Data::get()->state.ned_phi = fdm::Units::deg2rad( _ui->spinBoxAttPhi->value() );

        if ( _test )
        {
            if ( _ui->pushButtonWork->isChecked() )
            {
                _test->update( timeStep );
            }

            _test->updateData();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::setDockTest( DockWidgetTest *dockTest )
{
    _dockTest = dockTest;
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::settingsRead()
{
    QSettings settings( TEST_ORG_NAME, TEST_APP_NAME );

    settings.beginGroup( "dock_main" );

    bool ccw = settings.value( "ccw", 0 ).toBool();

    _ui->radioButtonCW->setChecked( !ccw );
    _ui->radioButtonCCW->setChecked( ccw );

    _ccw = ccw;

    double ned_psi = settings.value( "ned_psi", 0.0 ).toDouble();
    double ned_tht = settings.value( "ned_tht", 0.0 ).toDouble();
    double ned_phi = settings.value( "ned_phi", 0.0 ).toDouble();

    _ui->spinBoxAttPsi->setValue( ned_psi );
    _ui->spinBoxAttTht->setValue( ned_tht );
    _ui->spinBoxAttPhi->setValue( ned_phi );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::settingsSave()
{
    QSettings settings( TEST_ORG_NAME, TEST_APP_NAME );

    settings.beginGroup( "dock_main" );

    settings.setValue( "ccw", _ccw ? 1 : 0 );

    settings.setValue( "ned_psi", _ui->spinBoxAttPsi->value() );
    settings.setValue( "ned_tht", _ui->spinBoxAttTht->value() );
    settings.setValue( "ned_phi", _ui->spinBoxAttPhi->value() );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::on_pushButtonInit_clicked()
{
    _ui->pushButtonWork->setEnabled( true );
    _ui->pushButtonPause->setEnabled( true );

    _ui->groupBoxDirection->setEnabled( false );

    if ( _test == nullptr )
    {
        try
        {
            _test = new fdm::Test( _ccw ? fdm::MainRotor::CCW : fdm::MainRotor::CW );
        }
        catch ( fdm::Exception &e )
        {
            _ui->pushButtonStop->setChecked( true );

            fdm::Log::e() << e.getInfo() << std::endl;

            while ( e.hasCause() )
            {
                e = e.getCause();
                fdm::Log::e() << e.getInfo() << std::endl;
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::on_pushButtonWork_clicked()
{
    _ui->pushButtonInit->setEnabled( false );
    _ui->pushButtonPause->setEnabled( true );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::on_pushButtonPause_clicked()
{
    _ui->pushButtonInit->setEnabled( false );
    _ui->pushButtonWork->setEnabled( true );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetMain::on_pushButtonStop_clicked()
{
    _ui->pushButtonInit->setEnabled( true );
    _ui->pushButtonWork->setEnabled( false );
    _ui->pushButtonPause->setEnabled( false );

    _ui->groupBoxDirection->setEnabled( true );

    if ( _test ) delete _test;
    _test = nullptr;
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

