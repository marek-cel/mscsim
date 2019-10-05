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

#include <fdm_c172/c172_Autopilot.h>

#include <fdm/fdm_Path.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

C172_Autopilot::C172_Autopilot() {}

////////////////////////////////////////////////////////////////////////////////

C172_Autopilot::~C172_Autopilot() {}

////////////////////////////////////////////////////////////////////////////////

void C172_Autopilot::init()
{
    readData( Path::get( "data/fdm/c172/c172_ap.xml" ) );

    //////////////////
    Autopilot::init();
    //////////////////
}

////////////////////////////////////////////////////////////////////////////////

void C172_Autopilot::onPressedAP()
{
    _engaged = !_engaged;

    if ( _engaged ) _yawDamper = true;
}

////////////////////////////////////////////////////////////////////////////////

void C172_Autopilot::onPressedFD()
{
    if ( _fd->isEngaged() )
    {
        _fd->disengage();

        if ( !_engaged )
        {
            _fd->setHorMode( FlightDirector::HM_FD );
            _fd->setVerMode( FlightDirector::VM_FD );
        }

    }
    else
    {
        _fd->engage();
    }
}

////////////////////////////////////////////////////////////////////////////////

void C172_Autopilot::onPressedALT()
{
    _fd->toggleVerMode( FlightDirector::VM_ALT );
}

////////////////////////////////////////////////////////////////////////////////

void C172_Autopilot::onPressedIAS()
{
    _fd->toggleVerMode( FlightDirector::VM_IAS );
}

////////////////////////////////////////////////////////////////////////////////

void C172_Autopilot::onPressedENG()
{
    _fd->setVerMode( FlightDirector::VM_VS ); // or toggle ??
}

////////////////////////////////////////////////////////////////////////////////

void C172_Autopilot::onPressedARM()
{
    _fd->setVerMode( FlightDirector::VM_ARM ); // or toggle ??
}

////////////////////////////////////////////////////////////////////////////////

void C172_Autopilot::onPressedHDG()
{
    _fd->engage();
    _fd->toggleHorMode( FlightDirector::HM_HDG );
}

////////////////////////////////////////////////////////////////////////////////

void C172_Autopilot::onPressedNAV()
{
    _fd->engage();
    _fd->toggleHorMode( FlightDirector::HM_NAV );
}

////////////////////////////////////////////////////////////////////////////////

void C172_Autopilot::onPressedAPR()
{
    _fd->engage();

    if ( isActiveAPR() )
    {
        _fd->setHorMode( FlightDirector::HM_FD );

        if ( isActiveGS() )
            _fd->setVerMode( FlightDirector::VM_FD );
    }
    else
        _fd->setHorMode( FlightDirector::HM_APR );
}

////////////////////////////////////////////////////////////////////////////////

void C172_Autopilot::onPressedBC()
{
    _fd->engage();

    if ( isActiveBC() )
    {
        _fd->setHorMode( FlightDirector::HM_FD );

        if ( isActiveGS() )
            _fd->setVerMode( FlightDirector::VM_FD );
    }
    else
        _fd->setHorMode( FlightDirector::HM_BC );
}

////////////////////////////////////////////////////////////////////////////////

void C172_Autopilot::onPressedYD()
{
    _yawDamper = !_yawDamper;
}

////////////////////////////////////////////////////////////////////////////////

void C172_Autopilot::onPressedSoftRide()
{
    _softRide = !_softRide;
}

////////////////////////////////////////////////////////////////////////////////

void C172_Autopilot::onPressedHalfBank()
{
    _halfBank = !_halfBank;

    if ( _halfBank )
        _fd->enableHalfBank();
    else
        _fd->disableHalfBank();
}

////////////////////////////////////////////////////////////////////////////////

void C172_Autopilot::onPressedTest()
{
    _testing = true;
}

////////////////////////////////////////////////////////////////////////////////

void C172_Autopilot::onReleasedTest()
{
    _testing = false;
}
