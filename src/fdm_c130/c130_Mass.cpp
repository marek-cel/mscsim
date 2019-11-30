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

#include <fdm_c130/c130_Mass.h>
#include <fdm_c130/c130_Aircraft.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

C130_Mass::C130_Mass( const C130_Aircraft *aircraft ) :
    Mass( aircraft ),
    _aircraft ( aircraft )
{}

////////////////////////////////////////////////////////////////////////////////

C130_Mass::~C130_Mass() {}

////////////////////////////////////////////////////////////////////////////////

void C130_Mass::init()
{
    VarMass *pilot_l     = getVariableMassByName( "pilot_l" );
    VarMass *pilot_r     = getVariableMassByName( "pilot_r" );
    VarMass *fuel_tank_l = getVariableMassByName( "fuel_tank_l" );
    VarMass *fuel_tank_r = getVariableMassByName( "fuel_tank_r" );
    VarMass *cabin       = getVariableMassByName( "cabin" );

    if ( pilot_l && pilot_r && fuel_tank_l && fuel_tank_r && cabin )
    {
        pilot_l->input = &_aircraft->getDataInp()->masses.pilot_1;
        pilot_r->input = &_aircraft->getDataInp()->masses.pilot_2;

        fuel_tank_l->input = &_aircraft->getDataInp()->masses.fuel_tank_1;
        fuel_tank_r->input = &_aircraft->getDataInp()->masses.fuel_tank_2;

        cabin->input = &_aircraft->getDataInp()->masses.cabin;
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
