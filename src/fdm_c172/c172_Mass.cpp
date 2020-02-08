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

#include <fdm_c172/c172_Mass.h>
#include <fdm_c172/c172_Aircraft.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

C172_Mass::C172_Mass( const C172_Aircraft *aircraft ) :
    Mass( aircraft ),
    _aircraft ( aircraft )
{}

////////////////////////////////////////////////////////////////////////////////

C172_Mass::~C172_Mass() {}

////////////////////////////////////////////////////////////////////////////////

void C172_Mass::init()
{
    VarMass *pilot_l     = getVariableMassByName( "pilot_l" );
    VarMass *pilot_r     = getVariableMassByName( "pilot_r" );
    VarMass *fuel_tank_l = getVariableMassByName( "fuel_tank_l" );
    VarMass *fuel_tank_r = getVariableMassByName( "fuel_tank_r" );
    VarMass *cabin       = getVariableMassByName( "cabin" );
    VarMass *trunk       = getVariableMassByName( "cargo_trunk" );

    if ( pilot_l && pilot_r && fuel_tank_l && fuel_tank_r && cabin && trunk )
    {
        pilot_l->input = &_aircraft->getDataInp()->masses.pilot[ 0 ];
        pilot_r->input = &_aircraft->getDataInp()->masses.pilot[ 1 ];

        fuel_tank_l->input = &_aircraft->getDataInp()->masses.tank[ 0 ];
        fuel_tank_r->input = &_aircraft->getDataInp()->masses.tank[ 1 ];

        cabin->input = &_aircraft->getDataInp()->masses.cabin;
        trunk->input = &_aircraft->getDataInp()->masses.trunk;
    }
    else
    {
        Exception e;

        e.setType( Exception::UnknownException );
        e.setInfo( "Obtaining variable masses failed." );

        FDM_THROW( e );
    }

    /////////////
    Mass::init();
    /////////////
}
