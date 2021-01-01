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

#include <fdm_xh/xh_Aircraft.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

XH_Aircraft::XH_Aircraft( Input *input ) :
    Aircraft( input )
{
    Aircraft::_aero = _aero = new XH_Aerodynamics ( this, _input );
    Aircraft::_ctrl = _ctrl = new XH_Controls     ( this, _input );
    Aircraft::_gear = _gear = new XH_LandingGear  ( this, _input );
    Aircraft::_mass = _mass = new XH_Mass         ( this, _input );
    Aircraft::_prop = _prop = new XH_Propulsion   ( this, _input );

    _stateVect = StateVector( FDM_STATE_DIMENSION + 1 );
    _statePrev = StateVector( FDM_STATE_DIMENSION + 1 );
    _derivVect = StateVector( FDM_STATE_DIMENSION + 1 );

    readFile( Path::get( "fdm/xh/xh_fdm.xml" ).c_str() );
}

////////////////////////////////////////////////////////////////////////////////

XH_Aircraft::~XH_Aircraft()
{
    FDM_DELPTR( _aero );
    FDM_DELPTR( _ctrl );
    FDM_DELPTR( _gear );
    FDM_DELPTR( _mass );
    FDM_DELPTR( _prop );
}

////////////////////////////////////////////////////////////////////////////////

void XH_Aircraft::initialize( bool engineOn )
{
    /////////////////////////////////
    Aircraft::initialize( engineOn );
    /////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////

void XH_Aircraft::computeStateDeriv( const StateVector &stateVect,
                                     StateVector *derivVect )
{
    ////////////////////////////////////////////////////
    Aircraft::computeStateDeriv( stateVect, derivVect );
    ////////////////////////////////////////////////////

    (*derivVect)( 13 ) = 1.0;
}

////////////////////////////////////////////////////////////////////////////////

void XH_Aircraft::updateVariables( const StateVector &stateVect,
                                   const StateVector &derivVect )
{
    //////////////////////////////////////////////////
    Aircraft::updateVariables( stateVect, derivVect );
    //////////////////////////////////////////////////
}

