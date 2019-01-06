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

#include <fdmMain/fdm_Inertia.h>

#include <math.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

double Inertia::update( double timeStep, double currentValue,
                        double setpoint, double timeConstant )
{
    return currentValue + ( 1.0 - exp( -timeStep / timeConstant ) ) * ( setpoint - currentValue );
}

////////////////////////////////////////////////////////////////////////////////

Inertia::Inertia() :
    m_timeConstant( 1.0 ),
    m_currentValue( 0.0 )
{}

////////////////////////////////////////////////////////////////////////////////

Inertia::Inertia( double timeConstant, double currentValue ) :
    m_timeConstant( timeConstant ),
    m_currentValue( currentValue )
{}

////////////////////////////////////////////////////////////////////////////////

void Inertia::setCurrentValue( double currentValue )
{
    m_currentValue = currentValue;
}

////////////////////////////////////////////////////////////////////////////////

void Inertia::setTimeConstant( double timeConstant )
{
    m_timeConstant = timeConstant;
}

////////////////////////////////////////////////////////////////////////////////

void Inertia::update( double timeStep, double setpoint )
{
    m_currentValue = update( timeStep, m_currentValue, setpoint, m_timeConstant );
}
