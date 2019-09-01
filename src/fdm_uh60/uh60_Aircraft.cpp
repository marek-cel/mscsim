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

#include <fdm_uh60/uh60_Aircraft.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

UH60_Aircraft::UH60_Aircraft( const DataInp *dataInp, DataOut *dataOut ) :
    Aircraft( dataInp, dataOut )
{
    Aircraft::_aero = _aero = new UH60_Aerodynamics( this );
    Aircraft::_ctrl = _ctrl = new UH60_Controls( this );
    Aircraft::_gear = _gear = new UH60_LandingGear( this );
    Aircraft::_mass = _mass = new UH60_Mass( this );
    Aircraft::_prop = _prop = new UH60_Propulsion( this );
}

////////////////////////////////////////////////////////////////////////////////

UH60_Aircraft::~UH60_Aircraft()
{
    if ( _aero ) delete _aero;
    _aero = 0;

    if ( _ctrl ) delete _ctrl;
    _ctrl = 0;

    if ( _gear ) delete _gear;
    _gear = 0;

    if ( _mass ) delete _mass;
    _mass = 0;

    if ( _prop ) delete _prop;
    _prop = 0;
}

////////////////////////////////////////////////////////////////////////////////

void UH60_Aircraft::init( bool engineOn )
{
    readData( Path::get( "data/fdm/uh60/uh60_fdm.xml" ) );

    ///////////////////////////
    Aircraft::init( engineOn );
    ///////////////////////////
}

////////////////////////////////////////////////////////////////////////////////

void UH60_Aircraft::updateOutputData()
{
    /////////////////////////////
    Aircraft::updateOutputData();
    /////////////////////////////

    // controls
    _dataOut->controls.elevator = _ctrl->getElevator();

//    // propulsion
//    _dataOut->engine[ 0 ].state = _prop->getEngine()->getState() == Engine::Running;
//    _dataOut->engine[ 0 ].rpm = _prop->getEngine()->getRPM();
//    _dataOut->engine[ 0 ].map = _prop->getEngine()->getMAP();
//    _dataOut->engine[ 0 ].ff  = _prop->getEngine()->getFuelFlow();

    // rotor
    _dataOut->rotor.mainRotor_azimuth     = _prop->getMainRotorPsi();
    _dataOut->rotor.mainRotor_coningAngle = _aero->getMainRotor()->getConingAngle();
    _dataOut->rotor.mainRotor_diskRoll    = _aero->getMainRotor()->getDiskRoll();
    _dataOut->rotor.mainRotor_diskPitch   = _aero->getMainRotor()->getDiskPitch();
    _dataOut->rotor.mainRotor_collective  = _ctrl->getCollective();
    _dataOut->rotor.mainRotor_cyclicLon   = _ctrl->getCyclicLon();
    _dataOut->rotor.mainRotor_cyclicLat   = _ctrl->getCyclicLat();
    _dataOut->rotor.tailRotor_azimuth     = _prop->getTailRotorPsi();
}
