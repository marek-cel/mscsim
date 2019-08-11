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

#include <fdm_p51/p51_Aircraft.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

P51_Aircraft::P51_Aircraft( const DataInp *dataInp, DataOut *dataOut ) :
    Aircraft( dataInp, dataOut ),

    m_aero ( 0 ),
    m_ctrl ( 0 ),
    m_gear ( 0 ),
    m_mass ( 0 ),
    m_prop ( 0 )
{
    Aircraft::m_aero = m_aero = new P51_Aerodynamics( this );
    Aircraft::m_ctrl = m_ctrl = new P51_Controls( this );
    Aircraft::m_gear = m_gear = new P51_LandingGear( this );
    Aircraft::m_mass = m_mass = new P51_Mass( this );
    Aircraft::m_prop = m_prop = new P51_Propulsion( this );
}

////////////////////////////////////////////////////////////////////////////////

P51_Aircraft::~P51_Aircraft()
{
    if ( m_aero ) delete m_aero;
    m_aero = 0;

    if ( m_ctrl ) delete m_ctrl;
    m_ctrl = 0;

    if ( m_gear ) delete m_gear;
    m_gear = 0;

    if ( m_mass ) delete m_mass;
    m_mass = 0;

    if ( m_prop ) delete m_prop;
    m_prop = 0;
}

////////////////////////////////////////////////////////////////////////////////

void P51_Aircraft::init( bool engineOn )
{
    readData( Path::get( "data/fdm/p51/p51_fdm.xml" ) );

    ///////////////////////////
    Aircraft::init( engineOn );
    ///////////////////////////
}

////////////////////////////////////////////////////////////////////////////////

void P51_Aircraft::updateOutputData()
{
    /////////////////////////////
    Aircraft::updateOutputData();
    /////////////////////////////

    // controls
    m_dataOut->controls.ailerons = m_ctrl->getAilerons();
    m_dataOut->controls.elevator = m_ctrl->getElevator();
    m_dataOut->controls.rudder   = m_ctrl->getRudder();
    m_dataOut->controls.flaps    = m_ctrl->getFlaps();

    // propulsion
    m_dataOut->engine[ 0 ].state = m_prop->getEngine()->getState() == Engine::Running;
    m_dataOut->engine[ 0 ].rpm = m_prop->getEngine()->getRPM();
    m_dataOut->engine[ 0 ].map = m_prop->getEngine()->getMAP();
    m_dataOut->engine[ 0 ].ff  = m_prop->getEngine()->getFuelFlow();
}
