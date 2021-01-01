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

#include <fdm_uh60/uh60_Aircraft.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

UH60_Aircraft::UH60_Aircraft( Input *input ) :
    Aircraft( input )
{
    Aircraft::_aero = _aero = new UH60_Aerodynamics ( this, _input );
    Aircraft::_ctrl = _ctrl = new UH60_Controls     ( this, _input );
    Aircraft::_gear = _gear = new UH60_LandingGear  ( this, _input );
    Aircraft::_mass = _mass = new UH60_Mass         ( this, _input );
    Aircraft::_prop = _prop = new UH60_Propulsion   ( this, _input );

    readFile( Path::get( "fdm/uh60/uh60_fdm.xml" ).c_str() );
}

////////////////////////////////////////////////////////////////////////////////

UH60_Aircraft::~UH60_Aircraft()
{
    FDM_DELPTR( _aero );
    FDM_DELPTR( _ctrl );
    FDM_DELPTR( _gear );
    FDM_DELPTR( _mass );
    FDM_DELPTR( _prop );
}

////////////////////////////////////////////////////////////////////////////////

void UH60_Aircraft::initialize( bool engineOn )
{
    /////////////////////////////////
    Aircraft::initialize( engineOn );
    /////////////////////////////////
}
