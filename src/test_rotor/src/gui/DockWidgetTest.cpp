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

#include <gui/DockWidgetTest.h>
#include <ui_DockWidgetTest.h>

#include <cmath>

#include <fdm/utils/fdm_Units.h>

#include <Data.h>

////////////////////////////////////////////////////////////////////////////////

DockWidgetTest::DockWidgetTest( QWidget *parent ) :
    QDockWidget ( parent ),
    _ui ( new Ui::DockWidgetTest ),
    _ccw ( false )
{
    _ui->setupUi( this );
}

////////////////////////////////////////////////////////////////////////////////

DockWidgetTest::~DockWidgetTest()
{
    if ( _ui ) delete _ui;
    _ui = nullptr;
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetTest::update( double /*timeStep*/ )
{
    if ( _ui->pushButtonTest->isChecked() )
    {
        Data::get()->test = true;

        Data::get()->rotor.direction = _ccw ? Data::Rotor::CCW : Data::Rotor::CW;

        Data::get()->rotor.coningAngle = fdm::Units::deg2rad( _ui->spinBoxTestConing     ->value() );
        Data::get()->rotor.diskPitch   = fdm::Units::deg2rad( _ui->spinBoxTestDiskPitch  ->value() );
        Data::get()->rotor.diskRoll    = fdm::Units::deg2rad( _ui->spinBoxTestDiskRoll   ->value() );
        Data::get()->rotor.collective  = fdm::Units::deg2rad( _ui->spinBoxTestCollective ->value() );
        Data::get()->rotor.cyclicLon   = fdm::Units::deg2rad( _ui->spinBoxTestCyclicLon  ->value() );
        Data::get()->rotor.cyclicLat   = fdm::Units::deg2rad( _ui->spinBoxTestCyclicLat  ->value() );

        double beta_0  =  Data::get()->rotor.coningAngle;
        double beta_1c = -Data::get()->rotor.diskPitch;
        double beta_1s = ( _ccw ? -1.0 : 1.0 ) * Data::get()->rotor.diskRoll;

        double theta_0  = Data::get()->rotor.collective;
        double theta_1c = ( _ccw ? -1.0 : 1.0 ) * Data::get()->rotor.cyclicLat;
        double theta_1s = Data::get()->rotor.cyclicLon;

        unsigned int bladesCount = 4;
        double psiStep = 2.0*M_PI / (float)bladesCount;

        for ( unsigned int i = 0; i < bladesCount; i++ )
        {
            double psi = Data::get()->rotor.azimuth + (double)(i*psiStep);

            double cosPsi = cos( psi );
            double sinPsi = sin( psi );

            Data::get()->blade[ i ].beta  =  beta_0 +  beta_1c * cosPsi +  beta_1s * sinPsi;
            Data::get()->blade[ i ].theta = theta_0 + theta_1c * cosPsi + theta_1s * sinPsi;
        }
    }
    else
    {
        Data::get()->test = false;
    }
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetTest::setCCW( bool ccw )
{
    _ccw = ccw;
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetTest::closeEvent( QCloseEvent *event )
{
    /////////////////////////////////
    QDockWidget::closeEvent( event );
    /////////////////////////////////

    emit closed();
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetTest::on_sliderTestConing_sliderMoved(int position)
{
    const double min = -20.0;
    const double max =  20.0;

    double coef = 0.01 * (double)position;

    double val = min + coef * ( max - min );

    _ui->spinBoxTestConing->setValue( val );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetTest::on_sliderTestDiskPitch_sliderMoved(int position)
{
    const double min = -20.0;
    const double max =  20.0;

    double coef = 0.01 * (double)position;

    double val = min + coef * ( max - min );

    _ui->spinBoxTestDiskPitch->setValue( val );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetTest::on_sliderTestDiskRoll_sliderMoved(int position)
{
    const double min = -20.0;
    const double max =  20.0;

    double coef = 0.01 * (double)position;

    double val = min + coef * ( max - min );

    _ui->spinBoxTestDiskRoll->setValue( val );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetTest::on_sliderTestCollective_sliderMoved(int position)
{
    const double min = -20.0;
    const double max =  20.0;

    double coef = 0.01 * (double)position;

    double val = min + coef * ( max - min );

    _ui->spinBoxTestCollective->setValue( val );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetTest::on_sliderTestCyclicLon_sliderMoved(int position)
{
    const double min = -20.0;
    const double max =  20.0;

    double coef = 0.01 * (double)position;

    double val = min + coef * ( max - min );

    _ui->spinBoxTestCyclicLon->setValue( val );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetTest::on_sliderTestCyclicLat_sliderMoved(int position)
{
    const double min = -20.0;
    const double max =  20.0;

    double coef = 0.01 * (double)position;

    double val = min + coef * ( max - min );

    _ui->spinBoxTestCyclicLat->setValue( val );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetTest::on_sliderTestAzimuth_sliderMoved(int position)
{
    const double min =   0.0;
    const double max = 360.0;

    double coef = 0.01 * (double)position;

    double val = min + coef * ( max - min );

    Data::get()->rotor.azimuth = fdm::Units::deg2rad( val );

    _ui->spinBoxTestAzimuth->setValue( val );
}
