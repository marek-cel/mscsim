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

UH60_LandingGear::UH60_LandingGear( const UH60_Aircraft *aircraft ) :
    LandingGear( aircraft ),
    _aircraft ( aircraft )
{}

////////////////////////////////////////////////////////////////////////////////

UH60_LandingGear::~UH60_LandingGear() {}

////////////////////////////////////////////////////////////////////////////////

void UH60_LandingGear::update()
{
    //////////////////////
    LandingGear::update();
    //////////////////////

    _brake_l = _aircraft->getCtrl()->getBrakeL();
    _brake_r = _aircraft->getCtrl()->getBrakeR();

    _ctrlAngle = 0.0;

    _antiskid = _aircraft->getDataInp()->controls.antiskid;
    _steering = false;
}
