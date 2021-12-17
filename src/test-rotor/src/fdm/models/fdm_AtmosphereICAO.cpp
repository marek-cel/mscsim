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

#include <fdm/models/fdm_AtmosphereICAO.h>

#include <cmath>

#include <fdm/fdm_Log.h>

#include <fdm/utils/fdm_WGS84.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

// Manual of the ICAO Standard Atmosphere, Table D, p.E-xii
const double AtmosphereICAO::_h_b[] = {
    -5000.0,
        0.0,
    11000.0,
    20000.0,
    32000.0,
    47000.0,
    51000.0,
    71000.0,
    80000.0
};

// Manual of the ICAO Standard Atmosphere, Table D, p.E-xii
const double AtmosphereICAO::_t_b[] = {
    320.65,     //  -5000.0
    288.15,     //      0.0
    216.65,     //  11000.0
    216.65,     //  20000.0
    228.65,     //  32000.0
    270.65,     //  47000.0
    270.65,     //  51000.0
    214.65,     //  71000.0
    196.65      //  80000.0
};

// Manual of the ICAO Standard Atmosphere, Table D, p.E-xii
const double AtmosphereICAO::_l_b[] = {
    -6.5e-3,    //  -5000.0
    -6.5e-3,    //      0.0
    0.0,        //  11000.0
    1.0e-3,     //  20000.0
    2.8e-3,     //  32000.0
    0.0,        //  47000.0
    -2.8e-3,    //  51000.0
    -2.0e-3     //  71000.0
};

const double AtmosphereICAO::_g_0 = 9.80665;    // Manual of the ICAO Standard Atmosphere, Table A, p.E-viii
const double AtmosphereICAO::_m_0 = 28.96442;   // Manual of the ICAO Standard Atmosphere, Table A, p.E-viii

////////////////////////////////////////////////////////////////////////////////

AtmosphereICAO::AtmosphereICAO()
{
    update( 0.0 );
}

////////////////////////////////////////////////////////////////////////////////

AtmosphereICAO::~AtmosphereICAO() {}

////////////////////////////////////////////////////////////////////////////////

void AtmosphereICAO::update( double altitude )
{
    // TODO
}
