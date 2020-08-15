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

#include <fdm_f35a/f35a_Mass.h>
#include <fdm_f35a/f35a_Aircraft.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

F35A_Mass::F35A_Mass( const F35A_Aircraft *aircraft, DataNode *rootNode ) :
    Mass( aircraft, rootNode ),
    _aircraft ( aircraft )
{}

////////////////////////////////////////////////////////////////////////////////

F35A_Mass::~F35A_Mass() {}

////////////////////////////////////////////////////////////////////////////////

void F35A_Mass::initialize()
{
    VarMass *pilot     = getVariableMassByName( "pilot" );
    VarMass *fuel_tank = getVariableMassByName( "fuel_tank" );

    if ( pilot && fuel_tank )
    {
        pilot->input = &_aircraft->getDataInp()->masses.pilot[ 0 ];

        fuel_tank->input = &_aircraft->getDataInp()->masses.tank[ 0 ];
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
