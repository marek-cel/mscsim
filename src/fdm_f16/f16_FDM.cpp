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

#include <fdm_f16/f16_FDM.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

F16_FDM::F16_FDM( const DataInp *dataInpPtr, DataOut *dataOutPtr, bool verbose ) :
    FDM( dataInpPtr, dataOutPtr, verbose )
{
    FDM::_aircraft = _aircraft = new F16_Aircraft( _input );
}

////////////////////////////////////////////////////////////////////////////////

F16_FDM::~F16_FDM()
{
    FDM_DELPTR( _aircraft );
}

////////////////////////////////////////////////////////////////////////////////

void F16_FDM::updateDataOut()
{
    /////////////////////
    FDM::updateDataOut();
    /////////////////////

    // controls
    _dataOut.controls.elevator  = _aircraft->getCtrl()->getFLCS()->getElevator();
    _dataOut.controls.elevons   = _aircraft->getCtrl()->getFLCS()->getElevons();
    _dataOut.controls.rudder    = _aircraft->getCtrl()->getFLCS()->getRudder();
    _dataOut.controls.flaps     = _aircraft->getCtrl()->getFLCS()->getFlapsTE();
    _dataOut.controls.flaperons = _aircraft->getCtrl()->getFLCS()->getAilerons(); // sic!
    _dataOut.controls.lef       = _aircraft->getCtrl()->getFLCS()->getFlapsLE();
    _dataOut.controls.airbrake  = _aircraft->getCtrl()->getAirbrake();

    // propulsion
    _dataOut.engine[ 0 ].state = _aircraft->getProp()->getEngine()->getState() == F16_Engine::Running;
    _dataOut.engine[ 0 ].afterburner = _aircraft->getProp()->getEngine()->getAfterburner();
    _dataOut.engine[ 0 ].n2  = _aircraft->getProp()->getEngine()->getN2();
    _dataOut.engine[ 0 ].tit = _aircraft->getProp()->getEngine()->getTIT();
    _dataOut.engine[ 0 ].fuelFlow = _aircraft->getProp()->getEngine()->getFuelFlow();
}
