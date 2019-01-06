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

#include <fdmMain/fdm_Environment.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Environment::Environment() :
    m_atmosphere ( 0 )
{
    m_atmosphere = new Atmosphere();
}

////////////////////////////////////////////////////////////////////////////////

Environment::~Environment()
{
    if ( m_atmosphere ) delete m_atmosphere;
    m_atmosphere = 0;
}

////////////////////////////////////////////////////////////////////////////////

void Environment::update( double altitude_asl )
{
    m_atmosphere->update( altitude_asl );

    m_temperature  = m_atmosphere->getTemperature();
    m_pressure     = m_atmosphere->getPressure();
    m_density      = m_atmosphere->getDensity();
    m_speedOfSound = m_atmosphere->getSpeedOfSound();
}
