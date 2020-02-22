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

#include <gui/DockWidgetCtrl.h>
#include <ui_DockWidgetCtrl.h>

#include <QSettings>

#include <fdm/utils/fdm_Units.h>

#include <Data.h>
#include <defs.h>

////////////////////////////////////////////////////////////////////////////////

DockWidgetCtrl::DockWidgetCtrl( QWidget *parent ) :
    QDockWidget ( parent ),
    _ui ( new Ui::DockWidgetCtrl )
{
    _ui->setupUi( this );

    settingsRead();
}

////////////////////////////////////////////////////////////////////////////////

DockWidgetCtrl::~DockWidgetCtrl()
{
    settingsSave();

    if ( _ui ) delete _ui;
    _ui = nullptr;
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetCtrl::update( double timeStep )
{
    if ( _ui->pushButtonIntegrate->isChecked() )
    {
        double psi = fdm::Units::deg2rad( _ui->spinBoxRotorPsi->value() );

        if ( Data::get()->phase == Data::Work )
        {
            double omega = 2.0 * M_PI * _ui->spinBoxRotorRpm->value() / 60.0;

            psi += timeStep * omega;

            while ( psi > 2.0 * M_PI )
            {
                psi -= 2.0 * M_PI;
            }
        }
        else if ( Data::get()->phase == Data::Stop )
        {
            psi = 0.0;
        }

        _ui->spinBoxRotorPsi->setValue( fdm::Units::rad2deg( psi ) );
    }

    double collective = fdm::Units::rad2deg( Data::get()->rotor.collective );
    double cyclicLon  = fdm::Units::rad2deg( Data::get()->rotor.cyclicLon  );
    double cyclicLat  = fdm::Units::rad2deg( Data::get()->rotor.cyclicLat  );

    _ui->spinBoxCollective ->setValue( collective );
    _ui->spinBoxCyclicLon  ->setValue( cyclicLon  );
    _ui->spinBoxCyclicLat  ->setValue( cyclicLat  );

    double normCollective = ( collective - MIN_COLLECTIVE_DEG )  / ( MAX_COLLECTIVE_DEG - MIN_COLLECTIVE_DEG );
    double normCyclicLon  = ( cyclicLon / MAX_CYCLIC_DEG + 1.0 ) / 2.0;
    double normCyclicLat  = ( cyclicLat / MAX_CYCLIC_DEG + 1.0 ) / 2.0;

    _ui->sliderCollective ->setValue( 100 * normCollective );
    _ui->sliderCyclicLon  ->setValue( 100 * normCyclicLon  );
    _ui->sliderCyclicLat  ->setValue( 100 * normCyclicLat  );
}

////////////////////////////////////////////////////////////////////////////////

double DockWidgetCtrl::getAzimuth()
{
    return fdm::Units::deg2rad( _ui->spinBoxRotorPsi->value() );
}

////////////////////////////////////////////////////////////////////////////////

double DockWidgetCtrl::getOmega()
{
    return 2.0 * M_PI * _ui->spinBoxRotorRpm->value() / 60.0;
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetCtrl::closeEvent( QCloseEvent *event )
{
    /////////////////////////////////
    QDockWidget::closeEvent( event );
    /////////////////////////////////

    emit closed();
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetCtrl::settingsRead()
{
    QSettings settings( TEST_ORG_NAME, TEST_APP_NAME );

    settings.beginGroup( "dock_ctrl" );

    double rpm = settings.value( "rpm", 0.0 ).toDouble();
    double psi = settings.value( "psi", 0.0 ).toDouble();

    bool integrate = settings.value( "intergrate_psi", 0 ).toBool();

    _ui->spinBoxRotorRpm->setValue( rpm );
    _ui->spinBoxRotorPsi->setValue( psi );

    _ui->pushButtonIntegrate->setChecked( integrate );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetCtrl::settingsSave()
{
    QSettings settings( TEST_ORG_NAME, TEST_APP_NAME );

    settings.beginGroup( "dock_ctrl" );

    settings.setValue( "rpm", _ui->spinBoxRotorRpm->value() );
    settings.setValue( "psi", _ui->spinBoxRotorPsi->value() );

    settings.setValue( "intergrate_psi", _ui->pushButtonIntegrate->isChecked() );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetCtrl::on_sliderCollective_sliderMoved(int position)
{
    const double min =   0.0;
    const double max =  20.0;

    double coef = 0.01 * (double)position;

    double val = min + coef * ( max - min );

    _ui->spinBoxCollective->setValue( val );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetCtrl::on_sliderCyclicLon_sliderMoved(int position)
{
    const double min = -20.0;
    const double max =  20.0;

    double coef = 0.01 * (double)position;

    double val = min + coef * ( max - min );

    _ui->spinBoxCyclicLon->setValue( val );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetCtrl::on_sliderCyclicLat_sliderMoved(int position)
{
    const double min = -20.0;
    const double max =  20.0;

    double coef = 0.01 * (double)position;

    double val = min + coef * ( max - min );

    _ui->spinBoxCyclicLat->setValue( val );
}
