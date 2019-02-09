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

#include <fdm_f16c/f16c_Aircraft.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

F16C_LandingGear::F16C_LandingGear( const F16C_Aircraft *aircraft ) :
    LandingGear( aircraft ),
    m_aircraft ( aircraft )
{}

////////////////////////////////////////////////////////////////////////////////

F16C_LandingGear::~F16C_LandingGear() {}

////////////////////////////////////////////////////////////////////////////////

void F16C_LandingGear::readData( XmlNode &dataNode )
{
    //////////////////////////////////
    LandingGear::readData( dataNode );
    //////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////

void F16C_LandingGear::update()
{
    m_brake_l = m_aircraft->getCtrl()->getBrakeL();
    m_brake_r = m_aircraft->getCtrl()->getBrakeR();

    m_ctrlAngle = 0.0;

    m_antiskid = true;
    m_steering = false;

    m_onGround = m_for_bas.getLength2() > 0.0;
}
