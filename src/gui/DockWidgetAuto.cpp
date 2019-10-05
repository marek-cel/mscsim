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

#include <Common.h>
#include <Data.h>

#include <fdm/fdm_Exception.h>
#include <fdm/utils/fdm_Units.h>
#include <fdm/xml/fdm_XmlDoc.h>

////////////////////////////////////////////////////////////////////////////////

DockWidgetAuto::DockWidgetAuto( QWidget *parent ) :
    QDockWidget ( parent ),
    _ui ( new Ui::DockWidgetAuto ),

    _autopilot      ( NULLPTR ),
    _autopilot_c172 ( NULLPTR ),

    _altitude ( 100.0 ),
    _climbRate ( 0.0 ),

    _timerId ( 0 )
{
    _ui->setupUi( this );

    _timerId = startTimer( 1000.0 * GUI_TIME_STEP );
}

////////////////////////////////////////////////////////////////////////////////

DockWidgetAuto::~DockWidgetAuto()
{
    if ( _timerId ) killTimer( _timerId );

    SIM_DELETE( _autopilot );

    SIM_DELETE( _ui );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::init()
{
    try
    {
        if ( !_autopilot )
        {
            switch ( Data::get()->aircraftType )
            {
            case fdm::DataInp::C172:
                _autopilot = _autopilot_c172 = new fdm::C172_Autopilot();
                break;

            default:
                _autopilot = NULLPTR;
                break;
            }

            if ( _autopilot ) _autopilot->init();
        }
    }
    catch ( fdm::Exception &e )
    {
        Log::e() << e.getInfo() << std::endl;

        while ( e.hasCause() )
        {
            e = e.getCause();
            Log::e() << e.getInfo() << std::endl;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::stop()
{
    SIM_DELETE( _autopilot );
    _autopilot_c172 = NULLPTR;
}

////////////////////////////////////////////////////////////////////////////////

double DockWidgetAuto::getAirspeed() const
{
    if ( _autopilot )
        return _autopilot->getAirspeed();

    return 0.0;
}

////////////////////////////////////////////////////////////////////////////////

double DockWidgetAuto::getAltitude() const
{
    if ( _autopilot )
        return _autopilot->getAltitude();

    return 0.0;
}

////////////////////////////////////////////////////////////////////////////////

double DockWidgetAuto::getCourse() const
{
    return _ui->spinBox_CRS->value();
}

////////////////////////////////////////////////////////////////////////////////

double DockWidgetAuto::getHeading() const
{
    if ( _autopilot )
        return _autopilot->getHeading();

    return 0.0;
}

////////////////////////////////////////////////////////////////////////////////

double DockWidgetAuto::getCmdRoll() const
{
    if ( _autopilot )
        return _autopilot->getCmdRoll();

    return 0.0;
}

////////////////////////////////////////////////////////////////////////////////

double DockWidgetAuto::getCmdPitch() const
{
    if ( _autopilot )
        return _autopilot->getCmdPitch();

    return 0.0;
}

////////////////////////////////////////////////////////////////////////////////

double DockWidgetAuto::getCtrlRoll() const
{
    if ( _autopilot )
        return _autopilot->getCtrlRoll();

    return 0.0;
}

////////////////////////////////////////////////////////////////////////////////

double DockWidgetAuto::getCtrlPitch() const
{
    if ( _autopilot )
        return _autopilot->getCtrlPitch();

    return 0.0;
}

////////////////////////////////////////////////////////////////////////////////

double DockWidgetAuto::getCtrlYaw() const
{
    if ( _autopilot )
        return _autopilot->getCtrlYaw();

    return 0.0;
}

////////////////////////////////////////////////////////////////////////////////

bool DockWidgetAuto::isActiveAP() const
{
    if ( _autopilot )
        return _autopilot->isActiveAP();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool DockWidgetAuto::isActiveFD() const
{
    if ( _autopilot )
        return _autopilot->isActiveFD();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool DockWidgetAuto::isActiveYD() const
{
    if ( _autopilot )
        return _autopilot->isActiveYD();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool DockWidgetAuto::isActiveALT() const
{
    if ( _autopilot )
        return _autopilot->isActiveALT();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool DockWidgetAuto::isActiveIAS() const
{
    if ( _autopilot )
        return _autopilot->isActiveIAS();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool DockWidgetAuto::isActiveVS() const
{
    if ( _autopilot )
        return _autopilot->isActiveVS();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool DockWidgetAuto::isActiveARM() const
{
    if ( _autopilot )
        return _autopilot->isActiveARM();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool DockWidgetAuto::isActiveGS() const
{
    if ( _autopilot )
        return _autopilot->isActiveGS();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool DockWidgetAuto::isActiveHDG() const
{
    if ( _autopilot )
        return _autopilot->isActiveHDG();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool DockWidgetAuto::isActiveNAV() const
{
    if ( _autopilot )
        return _autopilot->isActiveNAV();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool DockWidgetAuto::isActiveAPR() const
{
    if ( _autopilot )
        return _autopilot->isActiveAPR();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool DockWidgetAuto::isActiveBC() const
{
    if ( _autopilot )
        return _autopilot->isActiveBC();

    return false;
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
    else if ( isWorking() )
    {
        updateWork();
    }
}

////////////////////////////////////////////////////////////////////////////////

bool DockWidgetAuto::isWorking()
{
    return ( Data::get()->stateOut == fdm::DataOut::Working
          || Data::get()->stateOut == fdm::DataOut::Frozen );
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

    _altitude = fdm::Units::ft2m( 100.0 );
    _climbRate = 0.0;

    _ui->spinBox_CRS->setValue( 0.0 );
    _ui->spinBox_HDG->setValue( 0.0 );

    _ui->pushButtonShowALT->setChecked( true );
    _ui->spinBoxALT_VS->setValue( fdm::Units::m2ft( _altitude ) );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::updateWork()
{
    if ( _autopilot )
    {
        _autopilot->update( GUI_TIME_STEP,
                            Data::get()->ownship.roll,
                            Data::get()->ownship.pitch,
                            Data::get()->ownship.heading,
                            Data::get()->ownship.yawRate,
                            Data::get()->ownship.altitude_asl,
                            Data::get()->ownship.airspeed,
                            Data::get()->ownship.climbRate,
                            Data::get()->navigation.nav_deviation,
                            Data::get()->navigation.ils_gs_deviation,
                            _ui->pushButtonDN->isDown(),
                            _ui->pushButtonUP->isDown() );

        if ( _autopilot_c172 )
        {
            _ui->labelAP->setEnabled( _autopilot_c172->getLampAP() );
            _ui->labelFD->setEnabled( _autopilot_c172->getLampFD() );
            _ui->labelYD->setEnabled( _autopilot_c172->getLampYD() );

            _ui->labelALT->setEnabled( _autopilot_c172->getLampALT() );
            _ui->labelIAS->setEnabled( _autopilot_c172->getLampIAS() );

            _ui->labelHDG->setEnabled( _autopilot_c172->getLampHDG() );
            _ui->labelNAV->setEnabled( _autopilot_c172->getLampNAV() );
            _ui->labelAPR->setEnabled( _autopilot_c172->getLampAPR() );
            _ui->labelBC->setEnabled( _autopilot_c172->getLampBC() );

            _ui->labelNAV_ARM->setEnabled( _autopilot_c172->getLampNAV_ARM() );
            _ui->labelAPR_ARM->setEnabled( _autopilot_c172->getLampAPR_ARM() );

            _ui->labelSR->setEnabled( _autopilot_c172->getLampSR() );
            _ui->labelHB->setEnabled( _autopilot_c172->getLampHB() );

            _ui->labelTRIM->setEnabled( _autopilot_c172->getLampTRIM() );

            _ui->labelVS->setEnabled( _autopilot_c172->getLampVS() );
            _ui->labelARM->setEnabled( _autopilot_c172->getLampARM() );
        }

        if ( _ui->pushButtonDN->isDown() || _ui->pushButtonUP->isDown() )
        {
            _altitude  = _autopilot->getAltitude();
            _climbRate = _autopilot->getClimbRate();

            if ( _ui->pushButtonShowVS->isChecked() )
                _ui->spinBoxALT_VS->setValue( fdm::Units::mps2fpm( _climbRate ) );
            else
                _ui->spinBoxALT_VS->setValue( fdm::Units::m2ft( _altitude ) );
        }

        if ( _ui->pushButtonShowALT->isChecked() )
            _ui->spinBoxALT_VS->setReadOnly( _autopilot->isActiveALT() );
        else
            _ui->spinBoxALT_VS->setReadOnly( false );
    }
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::on_pushButtonAP_clicked()
{
    if ( isWorking() )
    {
        if ( _autopilot_c172 ) _autopilot_c172->onPressedAP();
    }
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::on_pushButtonFD_clicked()
{
    if ( _autopilot && isWorking() )
    {
        if ( !_autopilot->isActiveFD() )
            _autopilot->setPitch( Data::get()->ownship.pitch );

        if ( _autopilot_c172 ) _autopilot_c172->onPressedFD();
    }
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::on_pushButtonALT_clicked()
{
    if ( isWorking() )
    {
        if ( !_autopilot->isActiveALT() )
            _altitude = Data::get()->ownship.altitude_asl;

        if ( _autopilot ) _autopilot->setAltitude( _altitude );
        if ( _autopilot_c172 ) _autopilot_c172->onPressedALT();

        if ( _ui->pushButtonShowALT->isChecked() )
        {
            _ui->spinBoxALT_VS->setValue( fdm::Units::m2ft( _altitude ) );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::on_pushButtonIAS_clicked()
{
    if ( _autopilot && isWorking() )
    {
        if ( !_autopilot->isActiveIAS() )
            _autopilot->setAirspeed( Data::get()->ownship.airspeed );

        if ( _autopilot_c172 ) _autopilot_c172->onPressedIAS();
    }
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::on_pushButtonENG_clicked()
{
    if ( isWorking() )
    {
        if ( !_autopilot->isActiveVS() )
        {
            _climbRate = Data::get()->ownship.climbRate;

            if ( _ui->pushButtonShowVS->isChecked() )
                _climbRate = fdm::Units::fpm2mps( _ui->spinBoxALT_VS->value() );
        }

        if ( _autopilot ) _autopilot->setClimbRate( _climbRate );
        if ( _autopilot_c172 ) _autopilot_c172->onPressedENG();
    }
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::on_pushButtonARM_clicked()
{
    if ( isWorking() )
    {
        if ( !_autopilot->isActiveARM() )
        {
            _altitude = Data::get()->ownship.altitude_asl;

            if ( _ui->pushButtonShowALT->isChecked() )
                _altitude = fdm::Units::ft2m( _ui->spinBoxALT_VS->value() );
        }

        if ( _autopilot ) _autopilot->setAltitude( _altitude );
        if ( _autopilot_c172 ) _autopilot_c172->onPressedARM();
    }
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::on_pushButtonHDG_clicked()
{
    if ( _autopilot && isWorking() )
    {
        _autopilot->setHeading( fdm::Units::deg2rad( _ui->spinBox_HDG->value() ) );
        if ( _autopilot_c172 ) _autopilot_c172->onPressedHDG();
    }
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::on_pushButtonNAV_clicked()
{
    if ( isWorking() )
    {
        if ( _autopilot_c172 ) _autopilot_c172->onPressedNAV();
    }
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::on_pushButtonAPR_clicked()
{
    if ( isWorking() )
    {
        if ( _autopilot_c172 ) _autopilot_c172->onPressedAPR();
    }
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::on_pushButtonBC_clicked()
{
    if ( isWorking() )
    {
        if ( _autopilot_c172 ) _autopilot_c172->onPressedBC();
    }
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::on_pushButtonYD_clicked()
{
    if ( isWorking() )
    {
        if ( _autopilot_c172 ) _autopilot_c172->onPressedYD();
    }
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::on_pushButtonSoftRide_clicked()
{
    if ( isWorking() )
    {
        if ( _autopilot_c172 ) _autopilot_c172->onPressedSoftRide();
    }
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::on_pushButtonHalfBank_clicked()
{
    if ( isWorking() )
    {
        if ( _autopilot_c172 ) _autopilot_c172->onPressedHalfBank();
    }
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::on_pushButtonTest_pressed()
{
    if ( isWorking() )
    {
        if ( _autopilot_c172 ) _autopilot_c172->onPressedTest();
    }
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::on_pushButtonTest_released()
{
    if ( isWorking() )
    {
        if ( _autopilot_c172 ) _autopilot_c172->onReleasedTest();
    }
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::on_spinBox_CRS_valueChanged( double arg1 )
{
//    if ( _autopilot )
//        _autopilot->setCourse( fdm::Units::deg2rad( arg1 ) );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::on_spinBox_HDG_valueChanged( double arg1 )
{
    if ( _autopilot )
        _autopilot->setHeading( fdm::Units::deg2rad( arg1 ) );
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::on_spinBoxALT_VS_editingFinished()
{
    if ( _autopilot )
    {
        double val = _ui->spinBoxALT_VS->value();

        if ( _ui->pushButtonShowVS->isChecked() )
        {
            _climbRate = fdm::Units::fpm2mps( val );
        }
        else if ( _ui->pushButtonShowALT->isChecked() )
        {
            _altitude = fdm::Units::ft2m( val );
        }

        _autopilot->setAltitude( _altitude );
        _autopilot->setClimbRate( _climbRate );
    }
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::on_pushButtonShowVS_clicked( bool checked )
{
    if ( checked )
    {
        double climbRate = fdm::Units::mps2fpm( _climbRate );

        if ( _autopilot )
        {
            _ui->spinBoxALT_VS->setMinimum( fdm::Units::mps2fpm( _autopilot->getMinClimbRate() ) );
            _ui->spinBoxALT_VS->setMaximum( fdm::Units::mps2fpm( _autopilot->getMaxClimbRate() ) );
        }
        else
        {
            _ui->spinBoxALT_VS->setMinimum( -5000.0 );
            _ui->spinBoxALT_VS->setMaximum(  5000.0 );
        }

        _ui->spinBoxALT_VS->setSingleStep( 100.0 );
        _ui->spinBoxALT_VS->setValue( climbRate );
    }
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::on_pushButtonShowALT_clicked( bool checked )
{
    if ( checked )
    {
        double altitude = fdm::Units::m2ft( _altitude );

        if ( _autopilot )
        {
            _ui->spinBoxALT_VS->setMinimum( fdm::Units::m2ft( _autopilot->getMinAltitude() ) );
            _ui->spinBoxALT_VS->setMaximum( fdm::Units::m2ft( _autopilot->getMaxAltitude() ) );
        }
        else
        {
            _ui->spinBoxALT_VS->setMinimum(   100.0 );
            _ui->spinBoxALT_VS->setMaximum( 50000.0 );
        }

        _ui->spinBoxALT_VS->setSingleStep( 100.0 );
        _ui->spinBoxALT_VS->setValue( altitude );
    }
}

////////////////////////////////////////////////////////////////////////////////

void DockWidgetAuto::on_pushButtonSwitchFreq_clicked()
{
    double standby = _ui->spinBoxFreqStandby->value();
    double active  = _ui->spinBoxFreqActive->value();

    _ui->spinBoxFreqStandby->setValue( active );
    _ui->spinBoxFreqActive->setValue( standby );
}
