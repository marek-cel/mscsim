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

#include <g1000/sim/g1000_GRS.h>
#include <g1000/sim/g1000_IFD.h>

#include <cmath>

////////////////////////////////////////////////////////////////////////////////

using namespace g1000;

////////////////////////////////////////////////////////////////////////////////

GRS::GRS( IFD *ifd ) :
    _ifd ( ifd ),

    _roll    ( 0.0 ),
    _pitch   ( 0.0 ),
    _heading ( 0.0 ),

    _slipSkid ( 0.0 )
{}

////////////////////////////////////////////////////////////////////////////////

GRS::~GRS() {}

////////////////////////////////////////////////////////////////////////////////

void GRS::update()
{
    double roll    = _ifd->input().roll;
    double pitch   = _ifd->input().pitch;
    double heading = _ifd->input().heading;

    double g_force_y = 0.0;
    double g_force_z = 0.0;

    _roll    = roll;
    _pitch   = pitch;
    _heading = heading;

    _slipSkid = atan2( -g_force_y, g_force_z );
}
