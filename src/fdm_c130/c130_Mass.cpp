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

#include <fdm_c130/c130_Mass.h>
#include <fdm_c130/c130_Aircraft.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

C130_Mass::C130_Mass( const C130_Aircraft *aircraft, DataNode *rootNode ) :
    Mass( aircraft, rootNode ),
    _aircraft ( aircraft )
{}

////////////////////////////////////////////////////////////////////////////////

C130_Mass::~C130_Mass() {}

////////////////////////////////////////////////////////////////////////////////

void C130_Mass::initialize()
{
    VarMass *pilot_l         = getVariableMassByName( "pilot_l" );
    VarMass *pilot_r         = getVariableMassByName( "pilot_r" );
    VarMass *fuel_tank_1     = getVariableMassByName( "fuel_tank_1" );
    VarMass *fuel_tank_2     = getVariableMassByName( "fuel_tank_2" );
    VarMass *fuel_tank_3     = getVariableMassByName( "fuel_tank_3" );
    VarMass *fuel_tank_4     = getVariableMassByName( "fuel_tank_4" );
    VarMass *fuel_tank_aux_l = getVariableMassByName( "fuel_tank_aux_l" );
    VarMass *fuel_tank_aux_r = getVariableMassByName( "fuel_tank_aux_r" );
    VarMass *fuel_tank_ext_l = getVariableMassByName( "fuel_tank_ext_l" );
    VarMass *fuel_tank_ext_r = getVariableMassByName( "fuel_tank_ext_r" );
    VarMass *cabin           = getVariableMassByName( "cabin" );

    if ( pilot_l && pilot_r
      && fuel_tank_1 && fuel_tank_2 && fuel_tank_3 && fuel_tank_4
      && fuel_tank_aux_l && fuel_tank_aux_r && fuel_tank_ext_l && fuel_tank_ext_r
      && cabin )
    {
        pilot_l->input = &_aircraft->getDataInp()->masses.pilot[ 0 ];
        pilot_r->input = &_aircraft->getDataInp()->masses.pilot[ 1 ];

        fuel_tank_1->input = &_aircraft->getDataInp()->masses.tank[ 0 ];
        fuel_tank_2->input = &_aircraft->getDataInp()->masses.tank[ 1 ];
        fuel_tank_3->input = &_aircraft->getDataInp()->masses.tank[ 2 ];
        fuel_tank_4->input = &_aircraft->getDataInp()->masses.tank[ 3 ];

        fuel_tank_aux_l->input = &_aircraft->getDataInp()->masses.tank[ 4 ];
        fuel_tank_aux_r->input = &_aircraft->getDataInp()->masses.tank[ 5 ];
        fuel_tank_ext_l->input = &_aircraft->getDataInp()->masses.tank[ 6 ];
        fuel_tank_ext_r->input = &_aircraft->getDataInp()->masses.tank[ 7 ];

        cabin->input = &_aircraft->getDataInp()->masses.cabin;
    }
    else
    {
        Exception e;

        e.setType( Exception::UnknownException );
        e.setInfo( "Obtaining variable masses failed." );

        FDM_THROW( e );
    }

    ///////////////////
    Mass::initialize();
    ///////////////////
}
