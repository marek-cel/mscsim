/****************************************************************************//*
 * Copyright (C) 2020 Marek M. Cel
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

#include <fdm_c130/c130_Aircraft.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

C130_Aircraft::C130_Aircraft( const DataInp *dataInp, DataOut *dataOut ) :
    Aircraft( dataInp, dataOut ),

    _aero ( 0 ),
    _ctrl ( 0 ),
    _gear ( 0 ),
    _mass ( 0 ),
    _prop ( 0 )
{
    Aircraft::_aero = _aero = new C130_Aerodynamics( this );
    Aircraft::_ctrl = _ctrl = new C130_Controls( this );
    Aircraft::_gear = _gear = new C130_LandingGear( this );
    Aircraft::_mass = _mass = new C130_Mass( this );
    Aircraft::_prop = _prop = new C130_Propulsion( this );

    readFile( Path::get( "data/fdm/c130/c130_fdm.xml" ) );
}

////////////////////////////////////////////////////////////////////////////////

C130_Aircraft::~C130_Aircraft()
{
    FDM_DELPTR( _aero );
    FDM_DELPTR( _ctrl );
    FDM_DELPTR( _gear );
    FDM_DELPTR( _mass );
    FDM_DELPTR( _prop );
}

////////////////////////////////////////////////////////////////////////////////

void C130_Aircraft::initialize( bool engineOn )
{
    /////////////////////////////////
    Aircraft::initialize( engineOn );
    /////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////

void C130_Aircraft::updateOutputData()
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
    for ( int i = 0; i < _prop->getEnginesCount(); i++ )
    {
        _dataOut->engine[ i ].state    = _prop->getEngine( i )->getState() == Engine::Running;
        _dataOut->engine[ i ].rpm      = _prop->getEngine( i )->getRPM();
        _dataOut->engine[ i ].prop     = _prop->getPropeller( i )->getRPM();
        _dataOut->engine[ i ].map      = _prop->getEngine( i )->getMAP();
        _dataOut->engine[ i ].fuelFlow = _prop->getEngine( i )->getFuelFlow();
    }
}
