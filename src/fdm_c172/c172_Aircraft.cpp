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

#include <fdm_c172/c172_Aircraft.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

C172_Aircraft::C172_Aircraft( const DataInp *dataInp, DataOut *dataOut ) :
    Aircraft( dataInp, dataOut ),

    _aero ( 0 ),
    _ctrl ( 0 ),
    _gear ( 0 ),
    _mass ( 0 ),
    _prop ( 0 )
{
    Aircraft::_aero = _aero = new C172_Aerodynamics( this );
    Aircraft::_ctrl = _ctrl = new C172_Controls( this );
    Aircraft::_gear = _gear = new C172_LandingGear( this );
    Aircraft::_mass = _mass = new C172_Mass( this );
    Aircraft::_prop = _prop = new C172_Propulsion( this );
}

////////////////////////////////////////////////////////////////////////////////

C172_Aircraft::~C172_Aircraft()
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

void C172_Aircraft::init( bool engineOn )
{
    readData( Path::get( "data/fdm/c172/c172_fdm.xml" ) );

    ///////////////////////////
    Aircraft::init( engineOn );
    ///////////////////////////
}

////////////////////////////////////////////////////////////////////////////////

void C172_Aircraft::updateOutputData()
{
    /////////////////////////////
    Aircraft::updateOutputData();
    /////////////////////////////

    // controls
    _dataOut->controls.ailerons = _ctrl->getAilerons();
    _dataOut->controls.elevator = _ctrl->getElevator();
    _dataOut->controls.rudder   = _ctrl->getRudder();
    _dataOut->controls.flaps    = _ctrl->getFlaps();

    // propulsion
    _dataOut->engine[ 0 ].state = _prop->getEngine()->getState() == Engine::Running;
    _dataOut->engine[ 0 ].rpm = _prop->getEngine()->getRPM();
    _dataOut->engine[ 0 ].map = _prop->getEngine()->getMAP();
    _dataOut->engine[ 0 ].fuelFlow = _prop->getEngine()->getFuelFlow();
}
