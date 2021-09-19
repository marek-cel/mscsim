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

#include <Autopilot.h>

#include <Data.h>

#include <fdm/fdm_Exception.h>
#include <fdm/utils/fdm_Units.h>

#include <hid/hid_Manager.h>

#include <sim/Log.h>

////////////////////////////////////////////////////////////////////////////////

Autopilot::Autopilot() :
    _autopilot ( NULLPTR ),

    _c172_gfc700_ap ( NULLPTR ),
    _c172_kap140_ap ( NULLPTR ),
    _c172_kfc325_ap ( NULLPTR ),

    _altitude  ( 100.0 ),
    _climbRate (   0.0 )
{}

////////////////////////////////////////////////////////////////////////////////

Autopilot::~Autopilot()
{
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
                //_autopilot = _c172_gfc700_ap = new fdm::C172_GFC700_AP();
                //_autopilot = _c172_kap140_ap = new fdm::C172_KAP140_AP();
                _autopilot = _c172_kfc325_ap = new fdm::C172_KFC325_AP();
                break;

            default:
                _autopilot = NULLPTR;
                break;
            }

            if ( _autopilot ) _autopilot->initialize();
        }
    }
    catch ( const fdm::Exception &e )
    {
        Log::e() << e.getInfo() << std::endl;

        fdm::Exception et = e;
        while ( et.hasCause() )
        {
            et = et.getCause();
            Log::e() << et.getInfo() << std::endl;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::stop()
{
    DELPTR( _autopilot );

    _c172_gfc700_ap = NULLPTR;
    _c172_kap140_ap = NULLPTR;
    _c172_kfc325_ap = NULLPTR;
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::update( double timeStep )
{
    if ( _autopilot )
    {
        if ( _c172_kfc325_ap )
        {
            _c172_kfc325_ap->setHeadingILS( Data::get()->navigation.ils_heading );
        }

        _autopilot->update( timeStep,
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
                            Data::get()->navigation.ils_gs_visible );

        if ( hid::Manager::instance()->getAP_Disc() )
        {
            _autopilot->disengage();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::update( double timeStep, bool btn_dn, bool btn_up )
{
    if ( _c172_kfc325_ap ) _c172_kfc325_ap->update( timeStep, btn_dn, btn_up );
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::onPressedAP()
{
    if ( isWorking() )
    {
        if ( _c172_gfc700_ap ) _c172_gfc700_ap->onPressedAP();
        if ( _c172_kap140_ap ) _c172_kap140_ap->onPressedAP();
        if ( _c172_kfc325_ap ) _c172_kfc325_ap->onPressedAP();
    }
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::onPressedFD()
{
    if ( isWorking() )
    {
        // GFC 700
        if ( _c172_gfc700_ap )
        {
            if ( !_c172_gfc700_ap->isActiveFD() )
            {
                _c172_gfc700_ap->setPitch( Data::get()->ownship.pitch );
                _c172_gfc700_ap->setRoll( Data::get()->ownship.roll );
            }

            _c172_gfc700_ap->onPressedFD();
        }

        // KAP 140
        if ( _c172_kap140_ap )
        {
            if ( !_c172_kap140_ap->isActiveFD() )
            {
                _c172_kap140_ap->setPitch( Data::get()->ownship.pitch );
            }

            _c172_kap140_ap->onPressedFD();
        }

        // KFC 325
        if ( _c172_kfc325_ap )
        {
            if ( !_c172_kfc325_ap->isActiveFD() )
            {
                _c172_kfc325_ap->setPitch( Data::get()->ownship.pitch );
            }

            _c172_kfc325_ap->onPressedFD();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::onPressedALT()
{
    if ( isWorking() )
    {
        // GFC 700
        if ( _c172_gfc700_ap )
        {
            // TODO
        }

        // KFC 325
        if ( _c172_kfc325_ap )
        {
            if ( !_c172_kfc325_ap->isActiveALT() )
                _altitude = Data::get()->ownship.altitude_asl;

            _c172_kfc325_ap->setAltitude( _altitude );
            _c172_kfc325_ap->onPressedALT();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::onPressedIAS()
{
    if ( isWorking() )
    {
        // KFC 325
        if ( _c172_kfc325_ap )
        {
            if ( !_c172_kfc325_ap->isActiveIAS() )
                _c172_kfc325_ap->setAirspeed( Data::get()->ownship.ias );

            _c172_kfc325_ap->onPressedIAS();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::onPressedENG()
{
    if ( isWorking() )
    {
        // KFC 325
        if ( _c172_kfc325_ap )
        {
            if ( !_c172_kfc325_ap->isActiveVS() )
            {
                _climbRate = Data::get()->ownship.climbRate;
            }

            _c172_kfc325_ap->setClimbRate( _climbRate );
            _c172_kfc325_ap->onPressedENG();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::onPressedARM()
{
    if ( isWorking() )
    {
        // KFC 325
        if ( _c172_kfc325_ap )
        {
            if ( !_c172_kfc325_ap->isActiveARM() )
            {
                _altitude = Data::get()->ownship.altitude_asl;
            }

            _c172_kfc325_ap->setAltitude( _altitude );
            _c172_kfc325_ap->onPressedARM();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::onPressedVNV()
{
    if ( isWorking() )
    {
        // GFC 700
        if ( _c172_gfc700_ap )
        {

        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::onPressedFLC()
{
    if ( isWorking() )
    {
        // GFC 700
        if ( _c172_gfc700_ap )
        {
            // TODO
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::onPressedVS()
{
    if ( isWorking() )
    {
        // GFC 700
        if ( _c172_gfc700_ap )
        {
            // TODO
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::onPressedHDG( double hdg )
{
    if ( isWorking() )
    {
        // GFC 700
        if ( _c172_gfc700_ap )
        {
            // TODO
        }

        // KFC 325
        if ( _c172_kfc325_ap )
        {
            if ( !_c172_kfc325_ap->isActiveFD() )
                _c172_kfc325_ap->setPitch( Data::get()->ownship.pitch );

            _c172_kfc325_ap->setHeading( hdg );
            _c172_kfc325_ap->onPressedHDG();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::onPressedNAV( double crs )
{
    if ( isWorking() )
    {
        // GFC 700
        if ( _c172_gfc700_ap )
        {
            // TODO
        }

        // KFC 325
        if ( _c172_kfc325_ap )
        {
            if ( !_c172_kfc325_ap->isActiveFD() )
                _c172_kfc325_ap->setPitch( Data::get()->ownship.pitch );

            _c172_kfc325_ap->setCourse( crs );
            _c172_kfc325_ap->onPressedNAV();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::onPressedAPR()
{
    if ( isWorking() )
    {
        // GFC 700
        if ( _c172_gfc700_ap )
        {
            // TODO
        }

        // KFC 325
        if ( _c172_kfc325_ap )
        {
            if ( !_c172_kfc325_ap->isActiveFD() )
                _c172_kfc325_ap->setPitch( Data::get()->ownship.pitch );

            _c172_kfc325_ap->onPressedAPR();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::onPressedBC()
{
    if ( isWorking() )
    {
        // GFC 700
        if ( _c172_gfc700_ap )
        {
            // TODO
        }

        // KFC 325
        if ( _c172_kfc325_ap )
        {
            if ( !_c172_kfc325_ap->isActiveFD() )
                _c172_kfc325_ap->setPitch( Data::get()->ownship.pitch );

            _c172_kfc325_ap->onPressedBC();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::onPressedREV()
{
    if ( isWorking() )
    {
        // KAP 140
        if ( _c172_kap140_ap )
        {

        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::onPressedYD()
{
    if ( isWorking() )
    {
        if ( _c172_kfc325_ap ) _c172_kfc325_ap->onPressedYD();
    }
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::onPressedSoftRide()
{
    if ( isWorking() )
    {
        if ( _c172_kfc325_ap ) _c172_kfc325_ap->onPressedSoftRide();
    }
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::onPressedHalfBank()
{
    if ( isWorking() )
    {
        if ( _c172_kfc325_ap ) _c172_kfc325_ap->onPressedHalfBank();
    }
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::onPressedTest()
{
    if ( isWorking() )
    {
        if ( _c172_kfc325_ap ) _c172_kfc325_ap->onPressedTest();
    }
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::onReleasedTest()
{
    if ( isWorking() )
    {
        if ( _c172_kfc325_ap ) _c172_kfc325_ap->onReleasedTest();
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
    if ( _c172_kfc325_ap ) return _c172_kfc325_ap->getLampAP();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::getLampFD() const
{
    if ( _c172_kfc325_ap ) return _c172_kfc325_ap->getLampFD();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::getLampYD() const
{
    if ( _c172_kfc325_ap ) return _c172_kfc325_ap->getLampYD();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::getLampALT() const
{
    if ( _c172_kfc325_ap ) return _c172_kfc325_ap->getLampALT();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::getLampIAS() const
{
    if ( _c172_kfc325_ap ) return _c172_kfc325_ap->getLampIAS();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::getLampGS()  const
{
    if ( _c172_kfc325_ap ) return _c172_kfc325_ap->getLampGS();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::getLampHDG() const
{
    if ( _c172_kfc325_ap ) return _c172_kfc325_ap->getLampHDG();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::getLampNAV() const
{
    if ( _c172_kfc325_ap ) return _c172_kfc325_ap->getLampNAV();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::getLampAPR() const
{
    if ( _c172_kfc325_ap ) return _c172_kfc325_ap->getLampAPR();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::getLampBC() const
{
    if ( _c172_kfc325_ap ) return _c172_kfc325_ap->getLampBC();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::getLampNAV_ARM() const
{
    if ( _c172_kfc325_ap ) return _c172_kfc325_ap->getLampNAV_ARM();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::getLampAPR_ARM() const
{
    if ( _c172_kfc325_ap ) return _c172_kfc325_ap->getLampAPR_ARM();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::getLampSR() const
{
    if ( _c172_kfc325_ap ) return _c172_kfc325_ap->getLampSR();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::getLampHB() const
{
    if ( _c172_kfc325_ap ) return _c172_kfc325_ap->getLampHB();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::getLampTRIM() const
{
    if ( _c172_kfc325_ap ) return _c172_kfc325_ap->getLampTRIM();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::getLampVS() const
{
    if ( _c172_kfc325_ap ) return _c172_kfc325_ap->getLampVS();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::getLampARM() const
{
    if ( _c172_kfc325_ap ) return _c172_kfc325_ap->getLampARM();

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
    if ( _c172_kfc325_ap ) return _c172_kfc325_ap->isActiveALT();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::isActiveIAS() const
{
    if ( _c172_kfc325_ap ) return _c172_kfc325_ap->isActiveIAS();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::isActiveVS() const
{
    if ( _c172_kfc325_ap ) return _c172_kfc325_ap->isActiveVS();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::isActiveARM() const
{
    if ( _c172_kfc325_ap ) return _c172_kfc325_ap->isActiveARM();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::isActiveGS() const
{
    if ( _c172_kfc325_ap ) return _c172_kfc325_ap->isActiveGS();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::isActiveHDG() const
{
    if ( _c172_kfc325_ap ) return _c172_kfc325_ap->isActiveHDG();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::isActiveNAV() const
{
    if ( _c172_kfc325_ap ) return _c172_kfc325_ap->isActiveNAV();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::isActiveAPR() const
{
    if ( _c172_kfc325_ap ) return _c172_kfc325_ap->isActiveAPR();

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Autopilot::isActiveBC() const
{
    if ( _c172_kfc325_ap ) return _c172_kfc325_ap->isActiveBC();

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
    return ( Data::get()->stateOut == fdm::DataOut::Working );
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
