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

#include <fdm/sys/fdm_PID.h>

#include <algorithm>
#include <limits>

#include <fdm/utils/fdm_Misc.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

PID::PID( double kp, double ki, double kd ) :
    _kp ( kp ),
    _ki ( ki ),
    _kd ( kd ),

    _min ( std::numeric_limits< double >::min() ),
    _max ( std::numeric_limits< double >::max() ),

    _error   ( 0.0 ),
    _error_i ( 0.0 ),
    _error_d ( 0.0 ),

    _value ( 0.0 ),
    _delta ( 0.0 ),

    _saturation ( false )
{}

////////////////////////////////////////////////////////////////////////////////

PID::PID( double kp, double ki, double kd, double min, double max ) :
    _kp ( kp ),
    _ki ( ki ),
    _kd ( kd ),

    _min ( min ),
    _max ( max ),

    _error   ( 0.0 ),
    _error_i ( 0.0 ),
    _error_d ( 0.0 ),

    _value ( 0.0 ),
    _delta ( 0.0 ),

    _saturation ( true )
{}

////////////////////////////////////////////////////////////////////////////////

PID::~PID() {}

////////////////////////////////////////////////////////////////////////////////

void PID::update( double timeStep, double error )
{
    if ( timeStep > 0.0 )
    {
        // integration with anti-windup filter
        _error_i = _error_i + ( error - _delta ) * timeStep;

        _error_d = ( timeStep > 0.0 ) ? ( error - _error ) / timeStep : 0.0;

        _error = error;

        double value = _kp * _error + _ki * _error_i + _kd * _error_d;

        // saturation
        if ( _saturation )
        {
            _value = Misc::satur( _min, _max, value );
        }
        else
        {
            _value = value;
        }

        _delta = value - _value;
    }
}

////////////////////////////////////////////////////////////////////////////////

void PID::setValue( double value )
{
    _error   = 0.0;
    _error_i = 0.0;
    _error_d = 0.0;

    _value = value;
    _delta = 0.0;
}
