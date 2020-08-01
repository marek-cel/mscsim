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

#include <fdm_xh/xh_Aircraft.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

XH_Aircraft::XH_Aircraft( const DataInp *dataInp, DataOut *dataOut ) :
    Aircraft( dataInp, dataOut )
{
    Aircraft::_aero = _aero = new XH_Aerodynamics ( this );
    Aircraft::_ctrl = _ctrl = new XH_Controls     ( this );
    Aircraft::_gear = _gear = new XH_LandingGear  ( this );
    Aircraft::_mass = _mass = new XH_Mass         ( this );
    Aircraft::_prop = _prop = new XH_Propulsion   ( this );

    _stateVect = StateVector( FDM_STATE_DIMENSION + 1 );
    _statePrev = StateVector( FDM_STATE_DIMENSION + 1 );
    _derivVect = StateVector( FDM_STATE_DIMENSION + 1 );

    readFile( Path::get( "data/fdm/xh/xh_fdm.xml" ).c_str() );
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

void XH_Aircraft::updateOutputData()
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
    _dataOut->rotor.mainRotor_omega       = _prop->getMainRotorOmega();
    _dataOut->rotor.mainRotor_azimuth     = _prop->getMainRotorPsi();
    _dataOut->rotor.mainRotor_coningAngle = _aero->getMainRotor()->getConingAngle();
    _dataOut->rotor.mainRotor_diskRoll    = _aero->getMainRotor()->getDiskRoll();
    _dataOut->rotor.mainRotor_diskPitch   = _aero->getMainRotor()->getDiskPitch();
    _dataOut->rotor.mainRotor_collective  = _ctrl->getCollective();
    _dataOut->rotor.mainRotor_cyclicLon   = _ctrl->getCyclicLon();
    _dataOut->rotor.mainRotor_cyclicLat   = _ctrl->getCyclicLat();
    _dataOut->rotor.tailRotor_azimuth     = _prop->getTailRotorPsi();

    // blades
    for ( int i = 0; i < _aero->getMainRotor()->getNumberOfBlades(); i++ )
    {
        _dataOut->blade[ i ].flapping   = _aero->getMainRotor()->getBlade( i )->getBeta();
        _dataOut->blade[ i ].feathering = _aero->getMainRotor()->getBlade( i )->getTheta();
    }
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

