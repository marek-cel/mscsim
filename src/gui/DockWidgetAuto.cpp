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

#include <gui/DockWidgetAuto.h>
#include <ui_DockWidgetAuto.h>

#include <QSettings>

#include <Common.h>
#include <Data.h>

#include <fdm/utils/fdm_Units.h>
#include <fdm/xml/fdm_XmlDoc.h>

#include <hid/hid_Manager.h>

////////////////////////////////////////////////////////////////////////////////

DockWidgetAuto::DockWidgetAuto( QWidget *parent ) :
    QDockWidget ( parent ),
    _ui ( new Ui::DockWidgetAuto ),
    _ap ( NULLPTR ),
    _timerId ( 0 )
{
    _ui->setupUi( this );

    settingsRead();

    _timerId = startTimer( 1000.0 * GUI_TIME_STEP );
}

////////////////////////////////////////////////////////////////////////////////

DockWidgetAuto::~DockWidgetAuto()
{
    if ( _timerId ) killTimer( _timerId );

    settingsSave();

    DELPTR( _ui );
}

////////////////////////////////////////////////////////////////////////////////

double DockWidgetAuto::getCourse() const
{
    return _ui->spinBoxCRS->value();
}

////////////////////////////////////////////////////////////////////////////////

double DockWidgetAuto::getFreqNav() const
{
    return _ui->spinBoxFreqActive->value();
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::closeEvent( QCloseEvent *event )
{
    /////////////////////////////////
    QDockWidget::closeEvent( event );
    /////////////////////////////////

    emit closed();
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::timerEvent( QTimerEvent *event )
{
    /////////////////////////////////
    QDockWidget::timerEvent( event );
    /////////////////////////////////

    if ( Data::get()->stateOut == fdm::DataOut::Idle )
    {
        updateIdle();
    }
    else if ( _ap->isWorking() )
    {
        updateWork();
    }
}

////////////////////////////////////////////////////////////////////////////////

double DockWidgetAuto::normalizeHeading( double heading )
{
    if      ( heading < 0.0 )
        heading += 360.0;
    else if ( heading >= 360.0 )
        heading -= 360.0;

    return heading;
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::settingsRead()
{
    QSettings settings( SIM_ORG_NAME, SIM_APP_NAME );

    settings.beginGroup( "dock_widget_auto" );

    double freq_active  = settings.value( "freq_active"  , 108.0  ).toDouble();
    double freq_standby = settings.value( "freq_standby" , 117.95 ).toDouble();

    _ui->spinBoxFreqActive  ->setValue( freq_active  );
    _ui->spinBoxFreqStandby ->setValue( freq_standby );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::settingsSave()
{
    QSettings settings( SIM_ORG_NAME, SIM_APP_NAME );

    settings.beginGroup( "dock_widget_auto" );

    settings.setValue( "freq_active"  , _ui->spinBoxFreqActive  ->value() );
    settings.setValue( "freq_standby" , _ui->spinBoxFreqStandby ->value() );

    settings.endGroup();
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::updateIdle()
{
    _ui->labelAP->setEnabled( false );
    _ui->labelFD->setEnabled( false );
    _ui->labelYD->setEnabled( false );

    _ui->labelALT->setEnabled( false );
    _ui->labelIAS->setEnabled( false );

    _ui->labelHDG->setEnabled( false );
    _ui->labelNAV->setEnabled( false );
    _ui->labelAPR->setEnabled( false );
    _ui->labelBC->setEnabled( false );

    _ui->labelNAV_ARM->setEnabled( false );
    _ui->labelAPR_ARM->setEnabled( false );

    _ui->labelSR->setEnabled( false );
    _ui->labelHB->setEnabled( false );

    _ui->labelTRIM->setEnabled( false );

    _ui->labelVS->setEnabled( false );
    _ui->labelARM->setEnabled( false );

    _ap->setAltitude( fdm::Units::ft2m( 100.0 ) );
    _ap->setClimbRate( 0.0 );

    _ui->spinBoxCRS->setValue( 0.0 );
    _ui->spinBoxHDG->setValue( 0.0 );

    _ui->pushButtonShowALT->setChecked( true );
    _ui->spinBoxALT_VS->setValue( fdm::Units::m2ft( _ap->getAltitude() ) );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::updateWork()
{
    _ap->update( GUI_TIME_STEP, _ui->pushButtonDN->isDown(), _ui->pushButtonUP->isDown() );

    _ui->spinBoxALT_VS->setReadOnly( _ap->isInited() );

    _ui->labelAP->setEnabled( _ap->getLampAP() );
    _ui->labelFD->setEnabled( _ap->getLampFD() );
    _ui->labelYD->setEnabled( _ap->getLampYD() );

    _ui->labelALT->setEnabled( _ap->getLampALT() );
    _ui->labelIAS->setEnabled( _ap->getLampIAS() );

    _ui->labelHDG->setEnabled( _ap->getLampHDG() );
    _ui->labelNAV->setEnabled( _ap->getLampNAV() );
    _ui->labelAPR->setEnabled( _ap->getLampAPR() );
    _ui->labelBC ->setEnabled( _ap->getLampBC() );

    _ui->labelNAV_ARM->setEnabled( _ap->getLampNAV_ARM() );
    _ui->labelAPR_ARM->setEnabled( _ap->getLampAPR_ARM() );

    _ui->labelSR->setEnabled( _ap->getLampSR() );
    _ui->labelHB->setEnabled( _ap->getLampHB() );

    _ui->labelTRIM->setEnabled( _ap->getLampTRIM() );

    _ui->labelVS ->setEnabled( _ap->getLampVS() );
    _ui->labelARM->setEnabled( _ap->getLampARM() );

    if ( _ui->pushButtonShowALT->isChecked() )
        _ui->spinBoxALT_VS->setReadOnly( _ap->isActiveALT() );
    else
        _ui->spinBoxALT_VS->setReadOnly( false );

    if ( _ui->pushButtonDN->isDown() || _ui->pushButtonUP->isDown() )
    {
        if ( _ui->pushButtonShowVS->isChecked() )
            _ui->spinBoxALT_VS->setValue( fdm::Units::mps2fpm( _ap->getClimbRate() ) );
        else
            _ui->spinBoxALT_VS->setValue( fdm::Units::m2ft( _ap->getAltitude() ) );
    }
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::on_pushButtonAP_clicked()
{
    _ap->onPressedAP();
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::on_pushButtonFD_clicked()
{
    _ap->onPressedFD();
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::on_pushButtonALT_clicked()
{
    _ap->onPressedALT();

    if ( _ui->pushButtonShowALT->isChecked() )
        _ui->spinBoxALT_VS->setValue( fdm::Units::m2ft( _ap->getAltitude() ) );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::on_pushButtonIAS_clicked()
{
    _ap->onPressedIAS();
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::on_pushButtonENG_clicked()
{
    _ap->onPressedENG();

    if ( _ui->pushButtonShowVS->isChecked() )
        _ap->setClimbRate( fdm::Units::fpm2mps( _ui->spinBoxALT_VS->value() ) );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::on_pushButtonARM_clicked()
{
    _ap->onPressedARM();

    if ( _ui->pushButtonShowALT->isChecked() )
        _ap->setAltitude( fdm::Units::ft2m( _ui->spinBoxALT_VS->value() ) );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::on_pushButtonHDG_clicked()
{
    _ap->onPressedHDG( fdm::Units::deg2rad( _ui->spinBoxHDG->value() ) );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::on_pushButtonNAV_clicked()
{
    _ap->onPressedNAV( fdm::Units::deg2rad( _ui->spinBoxCRS->value() ) );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::on_pushButtonAPR_clicked()
{
    _ap->onPressedAPR();
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::on_pushButtonBC_clicked()
{
    _ap->onPressedBC();
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::on_pushButtonYD_clicked()
{
    _ap->onPressedYD();
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::on_pushButtonSoftRide_clicked()
{
    _ap->onPressedSoftRide();
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::on_pushButtonHalfBank_clicked()
{
    _ap->onPressedHalfBank();
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::on_pushButtonTest_pressed()
{
    _ap->onPressedTest();
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::on_pushButtonTest_released()
{
    _ap->onReleasedTest();
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::on_pushButtonSetCRS_clicked()
{
    if ( Data::get()->navigation.nav_cdi != Data::Navigation::NONE )
    {
        double bearing = Data::get()->navigation.nav_bearing;
        double bearing_rel = bearing - Data::get()->ownship.heading;

        if ( bearing_rel < -M_PI ) bearing_rel += 2.0 * M_PI;
        if ( bearing_rel >  M_PI ) bearing_rel -= 2.0 * M_PI;

        if ( bearing_rel < -M_PI_2 || M_PI_2 < bearing_rel )
            bearing += M_PI;

        double bearing_deg = floor( fdm::Units::rad2deg( bearing ) + 0.5 );

        while ( bearing_deg <   0.0 ) bearing_deg += 360.0;
        while ( bearing_deg > 360.0 ) bearing_deg -= 360.0;

        _ui->spinBoxCRS->setValue( bearing_deg );
    }
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::on_pushButtonSetHDG_clicked()
{
    double heading = floor( fdm::Units::rad2deg( Data::get()->ownship.heading ) + 0.5 );
    _ui->spinBoxHDG->setValue( heading );
}


////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::on_spinBoxCRS_valueChanged( double arg1 )
{
    if ( arg1 < 0.0 || arg1 >= 360.0 )
    {
        _ui->spinBoxCRS->setValue( normalizeHeading( arg1 ) );
    }
    else
    {
        _ap->setCourse( fdm::Units::deg2rad( arg1 ) );
    }
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::on_spinBoxHDG_valueChanged( double arg1 )
{
    if ( arg1 < 0.0 || arg1 >= 360.0 )
    {
        _ui->spinBoxHDG->setValue( normalizeHeading( arg1 ) );
    }
    else
    {
        _ap->setHeading( fdm::Units::deg2rad( arg1 ) );
    }
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::on_spinBoxALT_VS_editingFinished()
{
    double val = _ui->spinBoxALT_VS->value();

    if ( _ui->pushButtonShowVS->isChecked() )
    {
        _ap->setClimbRate( fdm::Units::fpm2mps( val ) );
    }
    else if ( _ui->pushButtonShowALT->isChecked() )
    {
        _ap->setAltitude( fdm::Units::ft2m( val ) );
    }
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::on_pushButtonShowVS_clicked( bool checked )
{
    if ( checked )
    {
        double climbRate = fdm::Units::mps2fpm( _ap->getClimbRate() );

        _ui->spinBoxALT_VS->setMinimum( fdm::Units::mps2fpm( _ap->getMinClimbRate() ) );
        _ui->spinBoxALT_VS->setMaximum( fdm::Units::mps2fpm( _ap->getMaxClimbRate() ) );

        _ui->spinBoxALT_VS->setSingleStep( 100.0 );
        _ui->spinBoxALT_VS->setValue( climbRate );
    }
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::on_pushButtonShowALT_clicked( bool checked )
{
    if ( checked )
    {
        double altitude = fdm::Units::m2ft( _ap->getAltitude() );

        _ui->spinBoxALT_VS->setMinimum( fdm::Units::m2ft( _ap->getMinAltitude() ) );
        _ui->spinBoxALT_VS->setMaximum( fdm::Units::m2ft( _ap->getMaxAltitude() ) );

        _ui->spinBoxALT_VS->setSingleStep( 100.0 );
        _ui->spinBoxALT_VS->setValue( altitude );
    }
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::on_spinBoxFreqStandby_editingFinished()
{
    double freq = 0.05 * (int)( 20.0 * _ui->spinBoxFreqStandby->value() );
    _ui->spinBoxFreqStandby->setValue( freq );
}


////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::on_pushButtonSwitchFreq_clicked()
{
    double standby = _ui->spinBoxFreqStandby->value();
    double active  = _ui->spinBoxFreqActive->value();

    _ui->spinBoxFreqStandby->setValue( active );
    _ui->spinBoxFreqActive->setValue( standby );
}
