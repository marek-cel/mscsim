/****************************************************************************//*
 * Copyright (C) 2021 Marek M. Cel
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

#include <fdm_c172/c172_Aircraft.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

C172_Aircraft::C172_Aircraft( Input *input ) :
    Aircraft( input ),

    _aero ( 0 ),
    _ctrl ( 0 ),
    _gear ( 0 ),
    _mass ( 0 ),
    _prop ( 0 )
{
    Aircraft::_aero = _aero = new C172_Aerodynamics ( this, _input );
    Aircraft::_ctrl = _ctrl = new C172_Controls     ( this, _input );
    Aircraft::_gear = _gear = new C172_LandingGear  ( this, _input );
    Aircraft::_mass = _mass = new C172_Mass         ( this, _input );
    Aircraft::_prop = _prop = new C172_Propulsion   ( this, _input );

    readFile( Path::get( "fdm/c172/c172_fdm.xml" ).c_str() );
}

////////////////////////////////////////////////////////////////////////////////

C172_Aircraft::~C172_Aircraft()
{
    FDM_DELPTR( _aero );
    FDM_DELPTR( _ctrl );
    FDM_DELPTR( _gear );
    FDM_DELPTR( _mass );
    FDM_DELPTR( _prop );
}

////////////////////////////////////////////////////////////////////////////////

void C172_Aircraft::initialize( bool engineOn )
{
    /////////////////////////////////
    Aircraft::initialize( engineOn );
    /////////////////////////////////
}
