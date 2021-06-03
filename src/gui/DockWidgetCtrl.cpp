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

#include <gui/DockWidgetCtrl.h>
#include <ui_DockWidgetCtrl.h>

#include <gui/Aircrafts.h>

////////////////////////////////////////////////////////////////////////////////

DockWidgetCtrl::DockWidgetCtrl( QWidget *parent ) :
    QDockWidget ( parent ),
    _ui ( new Ui::DockWidgetCtrl )
{
    _ui->setupUi( this );
}

////////////////////////////////////////////////////////////////////////////////

DockWidgetCtrl::~DockWidgetCtrl()
{
    DELPTR( _ui );
}

////////////////////////////////////////////////////////////////////////////////

bool DockWidgetCtrl::getABS() const
{
    return _ui->pushButtonABS->isChecked();
}

////////////////////////////////////////////////////////////////////////////////

bool DockWidgetCtrl::getNWS() const
{
    return _ui->pushButtonNWS->isChecked();
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetCtrl::setAircraftType( int typeIndex )
{
    Aircrafts::Controls controls = Aircrafts::instance()->getAircraft( typeIndex ).controls;

    _ui->groupBoxCollective->setVisible( controls.collective );
    _ui->groupBoxGear->setVisible( controls.landingGear );
    _ui->groupBoxFlaps->setVisible( controls.flaps );
    _ui->groupBoxAirbrake->setVisible( controls.airbrake );
    _ui->groupBoxSpoilers->setVisible( controls.spoilers );

    if ( controls.throttle > 0 )
    {
        _ui->groupBoxThrottle->setVisible( true );

        if ( controls.throttle > 1 )
        {
            _ui->widgetThrottle_2->setVisible( true );
        }
        else
        {
            _ui->widgetThrottle_2->setVisible( false );
        }

        if ( controls.throttle > 2 )
        {
            _ui->widgetThrottle_3->setVisible( true );
        }
        else
        {
            _ui->widgetThrottle_3->setVisible( false );
        }

        if ( controls.throttle > 3 )
        {
            _ui->widgetThrottle_4->setVisible( true );
        }
        else
        {
            _ui->widgetThrottle_4->setVisible( false );
        }
    }
    else
    {
        _ui->groupBoxThrottle->setVisible( false );
    }

    if ( controls.propeller > 0 )
    {
        _ui->groupBoxPropeller->setVisible( true );

        if ( controls.propeller > 1 )
        {
            _ui->widgetPropeller_2->setVisible( true );
        }
        else
        {
            _ui->widgetPropeller_2->setVisible( false );
        }

        if ( controls.propeller > 2 )
        {
            _ui->widgetPropeller_3->setVisible( true );
        }
        else
        {
            _ui->widgetPropeller_3->setVisible( false );
        }

        if ( controls.propeller > 3 )
        {
            _ui->widgetPropeller_4->setVisible( true );
        }
        else
        {
            _ui->widgetPropeller_4->setVisible( false );
        }
    }
    else
    {
        _ui->groupBoxPropeller->setVisible( false );
    }

    if ( controls.mixture > 0 )
    {
        _ui->groupBoxMixture->setVisible( true );

        if ( controls.mixture > 1 )
        {
            _ui->widgetMixture_2->setVisible( true );
        }
        else
        {
            _ui->widgetMixture_2->setVisible( false );
        }

        if ( controls.mixture > 2 )
        {
            _ui->widgetMixture_3->setVisible( true );
        }
        else
        {
            _ui->widgetMixture_3->setVisible( false );
        }

        if ( controls.mixture > 3 )
        {
            _ui->widgetMixture_4->setVisible( true );
        }
        else
        {
            _ui->widgetMixture_4->setVisible( false );
        }
    }
    else
    {
        _ui->groupBoxMixture->setVisible( false );
    }
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetCtrl::setCollective( double collective )
{
    _ui->barCollective->setValue( 100 * collective );
    _ui->spinBoxCollective->setValue( 100 * collective );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetCtrl::setCtrlStick( double ctrlRoll, double ctrlPitch )
{
    _ui->graphicsCtrlStick->setCtrl( 100 * ctrlRoll, 100 * ctrlPitch );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetCtrl::setTrimStick( double trimRoll, double trimPitch )
{
    _ui->graphicsCtrlStick->setTrim( 100 * trimRoll, 100 * trimPitch );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetCtrl::setCtrlPedals( double ctrlPitch )
{
    _ui->graphicsCtrlPedals->setCtrl( 100 * ctrlPitch );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetCtrl::setTrimPedals( double trimPitch )
{
    _ui->graphicsCtrlPedals->setTrim( 100 * trimPitch );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetCtrl::setBrakes( double brakeL, double brakeR )
{
    _ui->barBrakeL->setValue( 100 * brakeL );
    _ui->barBrakeR->setValue( 100 * brakeR );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetCtrl::setGear( double gear )
{
    _ui->barGear->setValue( 100 * gear );
    _ui->spinBoxGear->setValue( 100 * gear );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetCtrl::setFlaps( double flaps )
{
    _ui->barFlaps->setValue( 100 * flaps );
    _ui->spinBoxFlaps->setValue( 100 * flaps );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetCtrl::setThrottle( double throttle_1,
                                  double throttle_2,
                                  double throttle_3,
                                  double throttle_4 )
{
    _ui->barThrottle_1->setValue( 100 * throttle_1 );
    _ui->barThrottle_2->setValue( 100 * throttle_2 );
    _ui->barThrottle_3->setValue( 100 * throttle_3 );
    _ui->barThrottle_4->setValue( 100 * throttle_4 );

    _ui->spinBoxThrottle_1->setValue( 100 * throttle_1 );
    _ui->spinBoxThrottle_2->setValue( 100 * throttle_2 );
    _ui->spinBoxThrottle_3->setValue( 100 * throttle_3 );
    _ui->spinBoxThrottle_4->setValue( 100 * throttle_4 );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetCtrl::setMixture( double mixture_1,
                                 double mixture_2,
                                 double mixture_3,
                                 double mixture_4 )
{
    _ui->barMixture_1->setValue( 100 * mixture_1 );
    _ui->barMixture_2->setValue( 100 * mixture_2 );
    _ui->barMixture_3->setValue( 100 * mixture_3 );
    _ui->barMixture_4->setValue( 100 * mixture_4 );

    _ui->spinBoxMixture_1->setValue( 100 * mixture_1 );
    _ui->spinBoxMixture_2->setValue( 100 * mixture_2 );
    _ui->spinBoxMixture_3->setValue( 100 * mixture_3 );
    _ui->spinBoxMixture_4->setValue( 100 * mixture_4 );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetCtrl::setPropeller( double propeller_1,
                                   double propeller_2,
                                   double propeller_3,
                                   double propeller_4 )
{
    _ui->barPropeller_1->setValue( 100 * propeller_1 );
    _ui->barPropeller_2->setValue( 100 * propeller_2 );
    _ui->barPropeller_3->setValue( 100 * propeller_3 );
    _ui->barPropeller_4->setValue( 100 * propeller_4 );

    _ui->spinBoxPropeller_1->setValue( 100 * propeller_1 );
    _ui->spinBoxPropeller_2->setValue( 100 * propeller_2 );
    _ui->spinBoxPropeller_3->setValue( 100 * propeller_3 );
    _ui->spinBoxPropeller_4->setValue( 100 * propeller_4 );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetCtrl::setAirbrake( double airbrake )
{
    _ui->barAirbrake->setValue( 100 * airbrake );
    _ui->spinBoxAirbrake->setValue( 100 * airbrake );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetCtrl::setSpoilers( double spoilers )
{
    _ui->barSpoilers->setValue( 100 * spoilers );
    _ui->spinBoxSpoilers->setValue( 100 * spoilers );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetCtrl::closeEvent( QCloseEvent *event )
{
    /////////////////////////////////
    QDockWidget::closeEvent( event );
    /////////////////////////////////

    emit closed();
}
