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

#include <fdm_test/test_Aircraft.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

TEST_Mass::TEST_Mass( const TEST_Aircraft *aircraft ) :
    Mass( aircraft ),
    _aircraft ( aircraft )
{}

////////////////////////////////////////////////////////////////////////////////

TEST_Mass::~TEST_Mass() {}

////////////////////////////////////////////////////////////////////////////////

void TEST_Mass::init()
{
    VarMass *pilot_l   = getVariableMassByName( "pilot_l" );
    VarMass *pilot_r   = getVariableMassByName( "pilot_r" );
    VarMass *fuel_tank = getVariableMassByName( "fuel_tank" );
    VarMass *cabin     = getVariableMassByName( "cabin" );

    if ( pilot_l && pilot_r && fuel_tank && cabin )
    {
        pilot_l->input   = &_aircraft->getDataInp()->masses.pilot_1;
        pilot_r->input   = &_aircraft->getDataInp()->masses.pilot_2;
        fuel_tank->input = &_aircraft->getDataInp()->masses.fuel_tank_1;
        cabin->input     = &_aircraft->getDataInp()->masses.cabin;
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
