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

#include <fdm_pw5/pw5_FDM.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

PW5_FDM::PW5_FDM( const DataInp *dataInpPtr, DataOut *dataOutPtr, bool verbose ) :
    FDM( dataInpPtr, dataOutPtr, verbose )
{
    FDM::_aircraft = _aircraft = new PW5_Aircraft( _input );

    _init_g_coef_p = 0.001;
    _init_g_coef_q = 0.001;
    _init_g_coef_n = 0.002;
}

////////////////////////////////////////////////////////////////////////////////

PW5_FDM::~PW5_FDM()
{
    FDM_DELPTR( _aircraft );
}

////////////////////////////////////////////////////////////////////////////////

void PW5_FDM::updateDataOut()
{
    /////////////////////
    FDM::updateDataOut();
    /////////////////////

    // controls
    _dataOut.controls.ailerons = _aircraft->getCtrl()->getAilerons();
    _dataOut.controls.elevator = _aircraft->getCtrl()->getElevator();
    _dataOut.controls.rudder   = _aircraft->getCtrl()->getRudder();
}
