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

#include <fdm_f16/f16_Aircraft.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

F16_Aircraft::F16_Aircraft( const DataInp *dataInp, DataOut *dataOut ) :
    Aircraft( dataInp, dataOut ),

    _aero ( 0 ),
    _ctrl ( 0 ),
    _gear ( 0 ),
    _mass ( 0 ),
    _prop ( 0 )
{
    Aircraft::_aero = _aero = new F16_Aerodynamics( this );
    Aircraft::_ctrl = _ctrl = new F16_Controls( this );
    Aircraft::_gear = _gear = new F16_LandingGear( this );
    Aircraft::_mass = _mass = new F16_Mass( this );
    Aircraft::_prop = _prop = new F16_Propulsion( this );
}

////////////////////////////////////////////////////////////////////////////////

F16_Aircraft::~F16_Aircraft()
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

void F16_Aircraft::init( bool engineOn )
{
    readData( Path::get( "data/fdm/f16/f16_fdm.xml" ) );

    ///////////////////////////
    Aircraft::init( engineOn );
    ///////////////////////////
}

////////////////////////////////////////////////////////////////////////////////

void F16_Aircraft::updateOutputData()
{
    /////////////////////////////
    Aircraft::updateOutputData();
    /////////////////////////////

    // controls
    _dataOut->controls.elevator  = _ctrl->getFLCS()->getElevator();
    _dataOut->controls.elevons   = _ctrl->getFLCS()->getElevons();
    _dataOut->controls.rudder    = _ctrl->getFLCS()->getRudder();
    _dataOut->controls.flaps     = _ctrl->getFLCS()->getFlapsTE();
    _dataOut->controls.flaperons = _ctrl->getFLCS()->getAilerons(); // sic!
    _dataOut->controls.lef       = _ctrl->getFLCS()->getFlapsLE();
    _dataOut->controls.airbrake  = _ctrl->getAirbrake();

    // propulsion
    _dataOut->engine[ 0 ].state = _prop->getEngine()->getState() == Engine::Running;
    _dataOut->engine[ 0 ].afterburner = _prop->getEngine()->getAfterburner();
    _dataOut->engine[ 0 ].n2  = _prop->getEngine()->getN2();
    _dataOut->engine[ 0 ].tit = _prop->getEngine()->getTIT();
    _dataOut->engine[ 0 ].fuelFlow = _prop->getEngine()->getFuelFlow();
}
