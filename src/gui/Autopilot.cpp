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

#include <gui/Autopilot.h>

#include <Common.h>
#include <Data.h>

#include <fdm/fdm_Exception.h>
#include <fdm/utils/fdm_Units.h>

#include <hid/hid_Manager.h>

////////////////////////////////////////////////////////////////////////////////

Autopilot::Autopilot() :
    QObject ( NULLPTR ),

    _timer ( NULLPTR ),

    _autopilot      ( NULLPTR ),
    _autopilot_c172 ( NULLPTR ),

    _altitude  ( 100.0 ),
    _climbRate (   0.0 ),

    _timerId ( 0 )
{
    _timer = new QElapsedTimer();
    _timer->start();

    _timerId = startTimer( 1000.0 * FDM_TIME_STEP );
}

////////////////////////////////////////////////////////////////////////////////

Autopilot::~Autopilot()
{
    if ( _timerId ) killTimer( _timerId );

    DELPTR( _timer );
    DELPTR( _autopilot );
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::init()
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

void Autopilot::stop()
{
    DELPTR( _autopilot );
    _autopilot_c172 = NULLPTR;
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::update( bool btn_dn, bool btn_up )
{
    double timeStep = Data::get()->timeCoef * (double)_timer->restart() / 1000.0;

    if ( _autopilot_c172 )
    {
        _autopilot_c172->setHeadingILS( Data::get()->navigation.ils_heading );
        _autopilot_c172->update( timeStep,
                                 Data::get()->ownship.roll,
                                 Data::get()->ownship.pitch,
                                 Data::get()->ownship.heading,
                                 Data::get()->ownship.altitude_asl,
                                 Data::get()->ownship.ias,
                                 Data::get()->ownship.turnRate,
                                 Data::get()->ownship.yawRate,
                                 Data::get()->ownship.climbRate,
                                 Data::get()->navigation.dme_distance,
                                 Data::get()->navigation.nav_deviation,
                                 Data::get()->navigation.nav_cdi != Data::Navigation::NONE,
                                 Data::get()->navigation.ils_lc_deviation,
                                 Data::get()->navigation.ils_lc_visible,
                                 Data::get()->navigation.ils_gs_deviation,
                                 Data::get()->navigation.ils_gs_visible,
                                 btn_dn, btn_up );

        if ( hid::Manager::instance()->getAP_Disc() )
        {
            _autopilot_c172->disengage();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::onPressedAP()
{
    if ( isWorking() )
    {
        if ( _autopilot_c172 ) _autopilot_c172->onPressedAP();
    }
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::onPressedFD()
{
    if ( isWorking() )
    {
        if ( _autopilot_c172 )
        {
            if ( !_autopilot_c172->isActiveFD() )
                _autopilot_c172->setPitch( Data::get()->ownship.pitch );

            _autopilot_c172->onPressedFD();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::onPressedALT()
{
    if ( isWorking() )
    {
        if ( _autopilot_c172 )
        {
            if ( !_autopilot_c172->isActiveALT() )
                _altitude = Data::get()->ownship.altitude_asl;

            _autopilot_c172->setAltitude( _altitude );
            _autopilot_c172->onPressedALT();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::onPressedIAS()
{
    if ( isWorking() )
    {
        if ( _autopilot_c172 )
        {
            if ( !_autopilot_c172->isActiveIAS() )
                _autopilot_c172->setAirspeed( Data::get()->ownship.ias );

            _autopilot_c172->onPressedIAS();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::onPressedENG()
{
    if ( isWorking() )
    {
        if ( _autopilot_c172 )
        {
            if ( !_autopilot_c172->isActiveVS() )
            {
                _climbRate = Data::get()->ownship.climbRate;
            }

            _autopilot_c172->setClimbRate( _climbRate );
            _autopilot_c172->onPressedENG();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::onPressedARM()
{
    if ( isWorking() )
    {
        if ( _autopilot_c172 )
        {
            if ( !_autopilot_c172->isActiveARM() )
            {
                _altitude = Data::get()->ownship.altitude_asl;
            }

            _autopilot_c172->setAltitude( _altitude );
            _autopilot_c172->onPressedARM();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::onPressedHDG( double hdg )
{
    if ( isWorking() )
    {
        if ( _autopilot_c172 )
        {
            if ( !_autopilot_c172->isActiveFD() )
                _autopilot_c172->setPitch( Data::get()->ownship.pitch );

            _autopilot_c172->setHeading( hdg );
            _autopilot_c172->onPressedHDG();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::onPressedNAV( double crs )
{
    if ( isWorking() )
    {
        if ( _autopilot_c172 )
        {
            if ( !_autopilot_c172->isActiveFD() )
                _autopilot_c172->setPitch( Data::get()->ownship.pitch );

            _autopilot_c172->setCourse( crs );
            _autopilot_c172->onPressedNAV();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::onPressedAPR()
{
    if ( isWorking() )
    {
        if ( _autopilot_c172 )
        {
            if ( !_autopilot_c172->isActiveFD() )
                _autopilot_c172->setPitch( Data::get()->ownship.pitch );

            _autopilot_c172->onPressedAPR();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::onPressedBC()
{
    if ( isWorking() )
    {
        if ( _autopilot_c172 )
        {
            if ( !_autopilot_c172->isActiveFD() )
                _autopilot_c172->setPitch( Data::get()->ownship.pitch );

            _autopilot_c172->onPressedBC();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::onPressedYD()
{
    if ( isWorking() )
    {
        if ( _autopilot_c172 ) _autopilot_c172->onPressedYD();
    }
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::onPressedSoftRide()
{
    if ( isWorking() )
    {
        if ( _autopilot_c172 ) _autopilot_c172->onPressedSoftRide();
    }
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::onPressedHalfBank()
{
    if ( isWorking() )
    {
        if ( _autopilot_c172 ) _autopilot_c172->onPressedHalfBank();
    }
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::onPressedTest()
{
    if ( isWorking() )
    {
        if ( _autopilot_c172 ) _autopilot_c172->onPressedTest();
    }
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::onReleasedTest()
{
    if ( isWorking() )
    {
        if ( _autopilot_c172 ) _autopilot_c172->onReleasedTest();
    }
}

////////////////////////////////////////////////////////////////////////////////

double Autopilot::getAirspeed() const
{
    if ( _autopilot ) return _autopilot->getAirspeed();

    return 0.0;
}

////////////////////////////////////////////////////////////////////////////////

double Autopilot::getAltitude() const
{
    if ( _autopilot ) return _autopilot->getAltitude();

    return 0.0;
}

////////////////////////////////////////////////////////////////////////////////

double Autopilot::getClimbRate() const
{
    if ( _autopilot ) return _autopilot->getClimbRate();

    return 0.0;
}

////////////////////////////////////////////////////////////////////////////////

double Autopilot::getHeading() const
{
    if ( _autopilot ) return _autopilot->getHeading();

    return 0.0;
}

////////////////////////////////////////////////////////////////////////////////

double Autopilot::getCmdRoll() const
{
    if ( _autopilot ) return _autopilot->getCmdRoll();

    return 0.0;
}

////////////////////////////////////////////////////////////////////////////////

double Autopilot::getCmdPitch() const
{
    if ( _autopilot ) return _autopilot->getCmdPitch();

    return 0.0;
}

////////////////////////////////////////////////////////////////////////////////

double Autopilot::getCtrlRoll() const
{
    if ( _autopilot ) return _autopilot->getCtrlRoll();

    return 0.0;
}

////////////////////////////////////////////////////////////////////////////////

double Autopilot::getCtrlPitch() const
{
    if ( _autopilot ) return _autopilot->getCtrlPitch();

    return 0.0;
}

////////////////////////////////////////////////////////////////////////////////

double Autopilot::getCtrlYaw() const
{
    if ( _autopilot ) return _autopilot->getCtrlYaw();

    return 0.0;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::getLampAP() const
{
    if ( _autopilot_c172 ) return _autopilot_c172->getLampAP();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::getLampFD() const
{
    if ( _autopilot_c172 ) return _autopilot_c172->getLampFD();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::getLampYD() const
{
    if ( _autopilot_c172 ) return _autopilot_c172->getLampYD();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::getLampALT() const
{
    if ( _autopilot_c172 ) return _autopilot_c172->getLampALT();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::getLampIAS() const
{
    if ( _autopilot_c172 ) return _autopilot_c172->getLampIAS();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::getLampGS()  const
{
    if ( _autopilot_c172 ) return _autopilot_c172->getLampGS();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::getLampHDG() const
{
    if ( _autopilot_c172 ) return _autopilot_c172->getLampHDG();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::getLampNAV() const
{
    if ( _autopilot_c172 ) return _autopilot_c172->getLampNAV();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::getLampAPR() const
{
    if ( _autopilot_c172 ) return _autopilot_c172->getLampAPR();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::getLampBC() const
{
    if ( _autopilot_c172 ) return _autopilot_c172->getLampBC();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::getLampNAV_ARM() const
{
    if ( _autopilot_c172 ) return _autopilot_c172->getLampNAV_ARM();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::getLampAPR_ARM() const
{
    if ( _autopilot_c172 ) return _autopilot_c172->getLampAPR_ARM();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::getLampSR() const
{
    if ( _autopilot_c172 ) return _autopilot_c172->getLampSR();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::getLampHB() const
{
    if ( _autopilot_c172 ) return _autopilot_c172->getLampHB();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::getLampTRIM() const
{
    if ( _autopilot_c172 ) return _autopilot_c172->getLampTRIM();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::getLampVS() const
{
    if ( _autopilot_c172 ) return _autopilot_c172->getLampVS();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::getLampARM() const
{
    if ( _autopilot_c172 ) return _autopilot_c172->getLampARM();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

double Autopilot::getMinAltitude() const
{
    if ( _autopilot ) return _autopilot->getMinAltitude();

    return 100.0;
}

////////////////////////////////////////////////////////////////////////////////

double Autopilot::getMaxAltitude() const
{
    if ( _autopilot ) return _autopilot->getMaxAltitude();

    return 50000.0;
}

////////////////////////////////////////////////////////////////////////////////

double Autopilot::getMinClimbRate() const
{
    if ( _autopilot ) return _autopilot->getMinClimbRate();

    return -5000.0;
}

////////////////////////////////////////////////////////////////////////////////

double Autopilot::getMaxClimbRate() const
{
    if ( _autopilot ) return _autopilot->getMaxClimbRate();

    return  5000.0;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::isActiveAP() const
{
    if ( _autopilot ) return _autopilot->isActiveAP();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::isActiveFD() const
{
    if ( _autopilot ) return _autopilot->isActiveFD();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::isActiveYD() const
{
    if ( _autopilot ) return _autopilot->isActiveYD();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::isActiveALT() const
{
    if ( _autopilot_c172 ) return _autopilot_c172->isActiveALT();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::isActiveIAS() const
{
    if ( _autopilot_c172 ) return _autopilot_c172->isActiveIAS();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::isActiveVS() const
{
    if ( _autopilot_c172 ) return _autopilot_c172->isActiveVS();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::isActiveARM() const
{
    if ( _autopilot_c172 ) return _autopilot_c172->isActiveARM();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::isActiveGS() const
{
    if ( _autopilot_c172 ) return _autopilot_c172->isActiveGS();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::isActiveHDG() const
{
    if ( _autopilot_c172 ) return _autopilot_c172->isActiveHDG();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::isActiveNAV() const
{
    if ( _autopilot_c172 ) return _autopilot_c172->isActiveNAV();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::isActiveAPR() const
{
    if ( _autopilot_c172 ) return _autopilot_c172->isActiveAPR();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::isActiveBC() const
{
    if ( _autopilot_c172 ) return _autopilot_c172->isActiveBC();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::isInited() const
{
    return ( _autopilot != NULLPTR );
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::isWorking() const
{
    return ( Data::get()->stateOut == fdm::DataOut::Working
          || Data::get()->stateOut == fdm::DataOut::Frozen );
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::setAltitude( double altitude )
{
    _altitude = altitude;

    if ( _autopilot ) _autopilot->setAltitude( _altitude );
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::setClimbRate( double climbRate )
{
    _climbRate = climbRate;

    if ( _autopilot ) _autopilot->setClimbRate( _climbRate );
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::setCourse( double course )
{
    if ( _autopilot ) _autopilot->setCourse( course );
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::setHeading( double heading )
{
    if ( _autopilot ) _autopilot->setHeading( heading );
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::timerEvent( QTimerEvent *event )
{
    /////////////////////////////
    QObject::timerEvent( event );
    /////////////////////////////
}
