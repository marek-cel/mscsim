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

#include <gui/DockWidgetCtrl.h>
#include <ui_DockWidgetCtrl.h>

#include <gui/Aircrafts.h>

////////////////////////////////////////////////////////////////////////////////

DockWidgetCtrl::DockWidgetCtrl( QWidget *parent ) :
    QDockWidget ( parent ),
    m_ui ( new Ui::DockWidgetCtrl )
{
    m_ui->setupUi( this );
}

////////////////////////////////////////////////////////////////////////////////

DockWidgetCtrl::~DockWidgetCtrl()
{
    if ( m_ui ) delete m_ui;
    m_ui = 0;
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetCtrl::setAircraftType( int typeIndex )
{
    Aircrafts::Controls controls = Aircrafts::instance()->getAircraft( typeIndex ).controls;

    m_ui->groupBoxCollective->setVisible( controls.collective );
    m_ui->groupBoxGear->setVisible( controls.landingGear );
    m_ui->groupBoxFlaps->setVisible( controls.flaps );
    m_ui->groupBoxAirbrake->setVisible( controls.airbrake );
    m_ui->groupBoxSpoilers->setVisible( controls.spoilers );

    if ( controls.throttle > 0 )
    {
        m_ui->groupBoxThrottle->setVisible( true );

        if ( controls.throttle > 1 )
        {
            m_ui->widgetThrottle_2->setVisible( true );
        }
        else
        {
            m_ui->widgetThrottle_2->setVisible( false );
        }

        if ( controls.throttle > 2 )
        {
            m_ui->widgetThrottle_3->setVisible( true );
        }
        else
        {
            m_ui->widgetThrottle_3->setVisible( false );
        }

        if ( controls.throttle > 3 )
        {
            m_ui->widgetThrottle_4->setVisible( true );
        }
        else
        {
            m_ui->widgetThrottle_4->setVisible( false );
        }
    }
    else
    {
        m_ui->groupBoxThrottle->setVisible( false );
    }

    if ( controls.propeller > 0 )
    {
        m_ui->groupBoxPropeller->setVisible( true );

        if ( controls.propeller > 1 )
        {
            m_ui->widgetPropeller_2->setVisible( true );
        }
        else
        {
            m_ui->widgetPropeller_2->setVisible( false );
        }

        if ( controls.propeller > 2 )
        {
            m_ui->widgetPropeller_3->setVisible( true );
        }
        else
        {
            m_ui->widgetPropeller_3->setVisible( false );
        }

        if ( controls.propeller > 3 )
        {
            m_ui->widgetPropeller_4->setVisible( true );
        }
        else
        {
            m_ui->widgetPropeller_4->setVisible( false );
        }
    }
    else
    {
        m_ui->groupBoxPropeller->setVisible( false );
    }

    if ( controls.mixture > 0 )
    {
        m_ui->groupBoxMixture->setVisible( true );

        if ( controls.mixture > 1 )
        {
            m_ui->widgetMixture_2->setVisible( true );
        }
        else
        {
            m_ui->widgetMixture_2->setVisible( false );
        }

        if ( controls.mixture > 2 )
        {
            m_ui->widgetMixture_3->setVisible( true );
        }
        else
        {
            m_ui->widgetMixture_3->setVisible( false );
        }

        if ( controls.mixture > 3 )
        {
            m_ui->widgetMixture_4->setVisible( true );
        }
        else
        {
            m_ui->widgetMixture_4->setVisible( false );
        }
    }
    else
    {
        m_ui->groupBoxMixture->setVisible( false );
    }
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetCtrl::setCollective( float collective )
{
    m_ui->barCollective->setValue( 100 * collective );
    m_ui->spinBoxCollective->setValue( 100 * collective );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetCtrl::setCtrlStick( float ctrlRoll, float ctrlPitch )
{
    m_ui->graphicsCtrlStick->setCtrl( 100 * ctrlRoll, 100 * ctrlPitch );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetCtrl::setTrimStick( float trimRoll, float trimPitch )
{
    m_ui->graphicsCtrlStick->setTrim( 100 * trimRoll, 100 * trimPitch );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetCtrl::setCtrlPedals( float ctrlPitch )
{
    m_ui->graphicsCtrlPedals->setCtrl( 100 * ctrlPitch );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetCtrl::setTrimPedals( float trimPitch )
{
    m_ui->graphicsCtrlPedals->setTrim( 100 * trimPitch );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetCtrl::setBrakes( float brakeL, float brakeR )
{
    m_ui->barBrakeL->setValue( 100 * brakeL );
    m_ui->barBrakeR->setValue( 100 * brakeR );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetCtrl::setGear( float gear )
{
    m_ui->barGear->setValue( 100 * gear );
    m_ui->spinBoxGear->setValue( 100 * gear );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetCtrl::setFlaps( float flaps )
{
    m_ui->barFlaps->setValue( 100 * flaps );
    m_ui->spinBoxFlaps->setValue( 100 * flaps );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetCtrl::setThrottle( float throttle_1,
                                  float throttle_2,
                                  float throttle_3,
                                  float throttle_4 )
{
    m_ui->barThrottle_1->setValue( 100 * throttle_1 );
    m_ui->barThrottle_2->setValue( 100 * throttle_2 );
    m_ui->barThrottle_3->setValue( 100 * throttle_3 );
    m_ui->barThrottle_4->setValue( 100 * throttle_4 );

    m_ui->spinBoxThrottle_1->setValue( 100 * throttle_1 );
    m_ui->spinBoxThrottle_2->setValue( 100 * throttle_2 );
    m_ui->spinBoxThrottle_3->setValue( 100 * throttle_3 );
    m_ui->spinBoxThrottle_4->setValue( 100 * throttle_4 );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetCtrl::setMixture( float mixture_1,
                                 float mixture_2,
                                 float mixture_3,
                                 float mixture_4 )
{
    m_ui->barMixture_1->setValue( 100 * mixture_1 );
    m_ui->barMixture_2->setValue( 100 * mixture_2 );
    m_ui->barMixture_3->setValue( 100 * mixture_3 );
    m_ui->barMixture_4->setValue( 100 * mixture_4 );

    m_ui->spinBoxMixture_1->setValue( 100 * mixture_1 );
    m_ui->spinBoxMixture_2->setValue( 100 * mixture_2 );
    m_ui->spinBoxMixture_3->setValue( 100 * mixture_3 );
    m_ui->spinBoxMixture_4->setValue( 100 * mixture_4 );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetCtrl::setPropeller( float propeller_1,
                                   float propeller_2,
                                   float propeller_3,
                                   float propeller_4 )
{
    m_ui->barPropeller_1->setValue( 100 * propeller_1 );
    m_ui->barPropeller_2->setValue( 100 * propeller_2 );
    m_ui->barPropeller_3->setValue( 100 * propeller_3 );
    m_ui->barPropeller_4->setValue( 100 * propeller_4 );

    m_ui->spinBoxPropeller_1->setValue( 100 * propeller_1 );
    m_ui->spinBoxPropeller_2->setValue( 100 * propeller_2 );
    m_ui->spinBoxPropeller_3->setValue( 100 * propeller_3 );
    m_ui->spinBoxPropeller_4->setValue( 100 * propeller_4 );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetCtrl::setAirbrake( float airbrake )
{
    m_ui->barAirbrake->setValue( 100 * airbrake );
    m_ui->spinBoxAirbrake->setValue( 100 * airbrake );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetCtrl::setSpoilers( float spoilers )
{
    m_ui->barSpoilers->setValue( 100 * spoilers );
    m_ui->spinBoxSpoilers->setValue( 100 * spoilers );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetCtrl::closeEvent( QCloseEvent *event )
{
    /////////////////////////////////
    QDockWidget::closeEvent( event );
    /////////////////////////////////

    emit closed();
}
