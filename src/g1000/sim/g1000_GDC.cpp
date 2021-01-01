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

#include <g1000/sim/g1000_GDC.h>
#include <g1000/sim/g1000_IFD.h>

#include <g1000/g1000_Log.h>

#include <g1000/utils/g1000_Const.h>
#include <g1000/utils/g1000_Misc.h>

////////////////////////////////////////////////////////////////////////////////

using namespace g1000;

////////////////////////////////////////////////////////////////////////////////

GDC::GDC( IFD *ifd ) :
    _ifd ( ifd ),

    _altitude  ( 0.0 ),
    _climbRate ( 0.0 ),

    _ias ( 0.0 ),
    _tas ( 0.0 )
{}

////////////////////////////////////////////////////////////////////////////////

GDC::~GDC() {}

////////////////////////////////////////////////////////////////////////////////

void GDC::update()
{
    double timeStep = _ifd->timeStep();

    double airspeed_u_bas = _ifd->input().airspeed_u_bas;

    double air_pressure = _ifd->input().air_pressure;
    double air_density  = _ifd->input().air_density;
    double ref_pressure = _ifd->input().ref_pressure;

    double altitude_prev = _altitude;

    _altitude = ( Const::_std_sl_t / Const::_dt_dh )
        * ( 1.0 - pow( air_pressure / ref_pressure, Const::_dt_dh * Const::_r_spec_air / Const::_g ) );

    double delta_h = _altitude - altitude_prev;

    if ( timeStep > 0.0 && fabs( delta_h ) > 1.0e-3 )
    {
        double climbRate = ( delta_h ) / timeStep;
        _climbRate = Misc::inertia( climbRate, _climbRate, timeStep, 0.5 );
    }

    double q = 0.5 * air_density * Misc::pow2( airspeed_u_bas );

    _ias = sqrt( 2.0 * q / Const::_std_sl_rho );
    _tas = _ias * sqrt( Const::_std_sl_rho / air_density );
}
