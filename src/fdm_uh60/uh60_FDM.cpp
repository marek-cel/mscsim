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

#include <fdm_uh60/uh60_FDM.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

UH60_FDM::UH60_FDM( const DataInp *dataInpPtr, DataOut *dataOutPtr, bool verbose ) :
    FDM( dataInpPtr, dataOutPtr, verbose )
{
    FDM::_aircraft = _aircraft = new UH60_Aircraft( _input );
}

////////////////////////////////////////////////////////////////////////////////

UH60_FDM::~UH60_FDM()
{
    FDM_DELPTR( _aircraft );
}

////////////////////////////////////////////////////////////////////////////////

void UH60_FDM::updateDataOut()
{
    /////////////////////
    FDM::updateDataOut();
    /////////////////////

//    // propulsion
//    _dataOut.engine[ 0 ].state = _aircraft->getProp()->getEngine()->getState() == Engine::Running;
//    _dataOut.engine[ 0 ].rpm = _aircraft->getProp()->getEngine()->getRPM();
//    _dataOut.engine[ 0 ].map = _aircraft->getProp()->getEngine()->getMAP();
//    _dataOut.engine[ 0 ].ff  = _aircraft->getProp()->getEngine()->getFuelFlow();

    // rotor
    _dataOut.rotor.mainRotor_omega       = _aircraft->getProp()->getMainRotorOmega();
    _dataOut.rotor.mainRotor_azimuth     = _aircraft->getProp()->getMainRotorPsi();
    _dataOut.rotor.mainRotor_coningAngle = _aircraft->getAero()->getMainRotor()->getConingAngle();
    _dataOut.rotor.mainRotor_diskRoll    = _aircraft->getAero()->getMainRotor()->getDiskRoll();
    _dataOut.rotor.mainRotor_diskPitch   = _aircraft->getAero()->getMainRotor()->getDiskPitch();
    _dataOut.rotor.mainRotor_collective  = _aircraft->getCtrl()->getCollective();
    _dataOut.rotor.mainRotor_cyclicLon   = _aircraft->getCtrl()->getCyclicLon();
    _dataOut.rotor.mainRotor_cyclicLat   = _aircraft->getCtrl()->getCyclicLat();
    _dataOut.rotor.tailRotor_azimuth     = _aircraft->getProp()->getTailRotorPsi();

    // blades
    double delta_psi = 2.0 * M_PI / (double)( _aircraft->getAero()->getMainRotor()->getNumberOfBlades() );
    for ( int i = 0; i < _aircraft->getAero()->getMainRotor()->getNumberOfBlades(); i++ )
    {
        double psi = _aircraft->getProp()->getMainRotorPsi() + i * delta_psi;

        double sinPsi = sin( psi );
        double cosPsi = cos( psi );

        double beta  = _aircraft->getAero()->getMainRotor()->getBeta0()
                     + _aircraft->getAero()->getMainRotor()->getBeta1c() * cosPsi
                     + _aircraft->getAero()->getMainRotor()->getBeta1s() * sinPsi;

        double theta = _aircraft->getAero()->getMainRotor()->getTheta0()
                     + _aircraft->getAero()->getMainRotor()->getTheta1c() * cosPsi
                     + _aircraft->getAero()->getMainRotor()->getTheta1s() * sinPsi;

        _dataOut.blade[ i ].flapping   = beta;
        _dataOut.blade[ i ].feathering = theta;
    }
}
