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

#include <fdm_c130/c130_LandingGear.h>
#include <fdm_c130/c130_Aircraft.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

C130_LandingGear::C130_LandingGear( const C130_Aircraft *aircraft ) :
    LandingGear( aircraft ),
    _aircraft ( aircraft )
{}

////////////////////////////////////////////////////////////////////////////////

C130_LandingGear::~C130_LandingGear() {}

////////////////////////////////////////////////////////////////////////////////

void C130_LandingGear::init()
{
    Wheel *wheel_n  = getWheelByName( "wheel_n"  );
    Wheel *wheel_lf = getWheelByName( "wheel_lf" );
    Wheel *wheel_lr = getWheelByName( "wheel_lr" );
    Wheel *wheel_rf = getWheelByName( "wheel_rf" );
    Wheel *wheel_rr = getWheelByName( "wheel_rr" );

    if ( wheel_n && wheel_lf && wheel_lr && wheel_rf && wheel_rr )
    {
        wheel_n  ->input = &_aircraft->getDataInp()->controls.landing_gear;
        wheel_lf ->input = &_aircraft->getDataInp()->controls.landing_gear;
        wheel_lr ->input = &_aircraft->getDataInp()->controls.landing_gear;
        wheel_rf ->input = &_aircraft->getDataInp()->controls.landing_gear;
        wheel_rr ->input = &_aircraft->getDataInp()->controls.landing_gear;
    }
    else
    {
        Exception e;

        e.setType( Exception::UnknownException );
        e.setInfo( "Obtaining wheels failed." );

        FDM_THROW( e );
    }

    ////////////////////
    LandingGear::init();
    ////////////////////
}

////////////////////////////////////////////////////////////////////////////////

void C130_LandingGear::update()
{
    //////////////////////
    LandingGear::update();
    //////////////////////

    _brake_l = _aircraft->getCtrl()->getBrakeL();
    _brake_r = _aircraft->getCtrl()->getBrakeR();

    _ctrlAngle = 0.0;

    _antiskid = _aircraft->getDataInp()->controls.abs;
    _steering = _aircraft->getDataInp()->controls.nws;
}
