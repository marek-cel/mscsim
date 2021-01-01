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

#include <fdm_c130/c130_FDM.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

C130_FDM::C130_FDM( const DataInp *dataInpPtr, DataOut *dataOutPtr, bool verbose ) :
    FDM( dataInpPtr, dataOutPtr, verbose )
{
    FDM::_aircraft = _aircraft = new C130_Aircraft( _input );
}

////////////////////////////////////////////////////////////////////////////////

C130_FDM::~C130_FDM()
{
    FDM_DELPTR( _aircraft );
}

////////////////////////////////////////////////////////////////////////////////

void C130_FDM::updateDataOut()
{
    /////////////////////
    FDM::updateDataOut();
    /////////////////////

    // controls
    _dataOut.controls.ailerons = _aircraft->getCtrl()->getAilerons();
    _dataOut.controls.elevator = _aircraft->getCtrl()->getElevator();
    _dataOut.controls.rudder   = _aircraft->getCtrl()->getRudder();
    _dataOut.controls.flaps    = _aircraft->getCtrl()->getFlaps();

    // propulsion
    for ( int i = 0; i < _aircraft->getProp()->getEnginesCount(); i++ )
    {
        _dataOut.engine[ i ].state    = _aircraft->getProp()->getEngine( i )->getState() == C130_Engine::Running;
        _dataOut.engine[ i ].rpm      = _aircraft->getProp()->getEngine( i )->getRPM();
        _dataOut.engine[ i ].prop     = _aircraft->getProp()->getPropeller( i )->getRPM();
        _dataOut.engine[ i ].map      = _aircraft->getProp()->getEngine( i )->getMAP();
        _dataOut.engine[ i ].fuelFlow = _aircraft->getProp()->getEngine( i )->getFuelFlow();
    }
}
