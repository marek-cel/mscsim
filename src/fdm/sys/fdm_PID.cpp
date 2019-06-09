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
    m_kp ( kp ),
    m_ki ( ki ),
    m_kd ( kd ),

    m_min ( std::numeric_limits< double >::min() ),
    m_max ( std::numeric_limits< double >::max() ),

    m_error   ( 0.0 ),
    m_error_i ( 0.0 ),
    m_error_d ( 0.0 ),

    m_value ( 0.0 ),
    m_delta ( 0.0 ),

    m_saturation ( false )
{}

////////////////////////////////////////////////////////////////////////////////

PID::PID( double kp, double ki, double kd, double min, double max ) :
    m_kp ( kp ),
    m_ki ( ki ),
    m_kd ( kd ),

    m_min ( min ),
    m_max ( max ),

    m_error   ( 0.0 ),
    m_error_i ( 0.0 ),
    m_error_d ( 0.0 ),

    m_value ( 0.0 ),
    m_delta ( 0.0 ),

    m_saturation ( true )
{}

////////////////////////////////////////////////////////////////////////////////

PID::~PID() {}

////////////////////////////////////////////////////////////////////////////////

void PID::update( double timeStep, double error )
{
    if ( timeStep > 0.0 )
    {
        // integration with anti-windup filter
        m_error_i = m_error_i + ( error - m_delta ) * timeStep;

        m_error_d = ( timeStep > 0.0 ) ? ( error - m_error ) / timeStep : 0.0;

        m_error = error;

        double value = m_kp * m_error + m_ki * m_error_i + m_kd * m_error_d;

        // saturation
        if ( m_saturation )
        {
            m_value = Misc::satur( m_min, m_max, value );
        }
        else
        {
            m_value = value;
        }

        m_delta = value - m_value;
    }
}

////////////////////////////////////////////////////////////////////////////////

void PID::setValue( double value )
{
    m_error   = 0.0;
    m_error_i = 0.0;
    m_error_d = 0.0;

    m_value = value;
    m_delta = 0.0;
}
