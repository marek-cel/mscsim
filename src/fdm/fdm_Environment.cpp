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

#include <fdm/fdm_Environment.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Environment::Environment() :
    _atmosphere ( FDM_NULLPTR ),

    _temperature     ( 0.0 ),
    _pressure        ( 0.0 ),
    _density         ( 0.0 ),
    _speedOfSound    ( 0.0 ),
    _densityAltitude ( 0.0 ),

    _wind_direction ( 0.0 ),
    _wind_speed     ( 0.0 )
{
    _atmosphere = new Atmosphere();
}

////////////////////////////////////////////////////////////////////////////////

Environment::~Environment()
{
    FDM_DELPTR( _atmosphere );
}

////////////////////////////////////////////////////////////////////////////////

void Environment::update( double altitude_asl )
{
    _atmosphere->update( altitude_asl );

    _temperature  = _atmosphere->getTemperature();
    _pressure     = _atmosphere->getPressure();
    _density      = _atmosphere->getDensity();
    _speedOfSound = _atmosphere->getSpeedOfSound();

    _densityAltitude = Atmosphere::getDensityAltitude( _pressure, _temperature,
                                                       altitude_asl );

    _wind_ned.x() = -cos( _wind_direction ) * _wind_speed;
    _wind_ned.y() = -sin( _wind_direction ) * _wind_speed;
}
