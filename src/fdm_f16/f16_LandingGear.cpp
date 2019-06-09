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

#include <fdm_f16/f16_Aircraft.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

F16_LandingGear::F16_LandingGear( const F16_Aircraft *aircraft ) :
    LandingGear( aircraft ),
    m_aircraft ( aircraft )
{}

////////////////////////////////////////////////////////////////////////////////

F16_LandingGear::~F16_LandingGear() {}

////////////////////////////////////////////////////////////////////////////////

void F16_LandingGear::init()
{
    Wheel *wheel_n = getWheelByName( "wheel_n" );
    Wheel *wheel_l = getWheelByName( "wheel_l" );
    Wheel *wheel_r = getWheelByName( "wheel_r" );

    if ( 0 != wheel_n
      && 0 != wheel_l
      && 0 != wheel_r )
    {
        wheel_n->input = &m_aircraft->getDataInp()->controls.landing_gear;
        wheel_l->input = &m_aircraft->getDataInp()->controls.landing_gear;
        wheel_r->input = &m_aircraft->getDataInp()->controls.landing_gear;
    }
    else
    {
        Exception e;

        e.setType( Exception::UnknownException );
        e.setInfo( "ERROR! Obtaining wheels failed." );

        FDM_THROW( e );
    }

    ////////////////////
    LandingGear::init();
    ////////////////////
}

////////////////////////////////////////////////////////////////////////////////

void F16_LandingGear::update()
{
    //////////////////////
    LandingGear::update();
    //////////////////////

    m_brake_l = m_aircraft->getCtrl()->getBrakeL();
    m_brake_r = m_aircraft->getCtrl()->getBrakeR();

    m_ctrlAngle = m_aircraft->getCtrl()->getNoseWheel();

    m_antiskid = m_aircraft->getDataInp()->controls.antiskid;
    m_steering = m_aircraft->getDataInp()->controls.nw_steering;
}
