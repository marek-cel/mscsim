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

#include <fdmMain/fdm_Atmosphere.h>

#include <iostream>
#include <math.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

const double Atmosphere::m_g = 9.80665;
const double Atmosphere::m_r = 8.31432e3;
const double Atmosphere::m_s = 110.4;
const double Atmosphere::m_b = 1.458e-6;

const double Atmosphere::m_std_sl_t = 288.15;
const double Atmosphere::m_std_sl_p = 101325.0;

////////////////////////////////////////////////////////////////////////////////

Atmosphere::Atmosphere() :
    m_temperature_0 ( m_std_sl_t ),
    m_pressure_0    ( m_std_sl_p )
{
    update( 0.0 );
}

////////////////////////////////////////////////////////////////////////////////

Atmosphere::~Atmosphere() {}

////////////////////////////////////////////////////////////////////////////////

void Atmosphere::update( double altitude )
{
    if ( altitude <= 11000.0 )
    {
        // [K/m] temperature rate
        double tempRate = ( m_temperature_0 - 216.65 ) / 11000.0;

        // [K] temperature
        m_temperature = m_temperature_0 - altitude * tempRate;

        // [Pa] pressure
        m_pressure = m_pressure_0
                   * pow( 1.0 - tempRate * altitude / m_temperature_0, 5.2561 );
    }
    else
    {
        // [K] temperature
        if ( altitude > 11000.0 && altitude <= 20000.0 )
            m_temperature = 216.65;
        else if ( altitude > 20000.0 && altitude <= 32000.0 )
            m_temperature = 216.65 + ( altitude - 20000.0 ) * 1.0e-3;
        else if ( altitude > 32000.0 && altitude <= 47000.0 )
            m_temperature = 228.65 + ( altitude - 32000.0 ) * 2.8e-3;
        else if ( altitude > 47000.0 && altitude <= 51000.0 )
            m_temperature = 270.65;
        else if ( altitude > 51000.0 && altitude <= 71000.0 )
            m_temperature = 270.65 - ( altitude - 51000.0 ) * 2.8e-3;
        else if ( altitude > 71000.0 && altitude <= 82000.0 )
            m_temperature = 214.65 - ( altitude - 71000.0 ) * 2.0e-3;
        else
            m_temperature = 192.65;

        // [Pa] pressure
        m_pressure = m_pressure_0
                   * exp( -m_g * ( altitude - 11000.0 ) / ( 287.05 * m_temperature ) )
                   * pow( 1.0 - 0.0065 * 11000.0 / m_temperature_0, 5.2561 );
    }

    // [kg/m^3] density
    m_density = m_pressure / 287.05 / m_temperature;

    // [m/s] speed of sound
    m_speedOfSound = 331.3 * sqrt( 1.0 + ( m_temperature - 273.15 ) / 273.15 );

    // [Pa*s] dynamic viscosity
    //m_viscosity = m_b * pow( m_temperature, 3.0 / 2.0 ) / ( m_temperature + m_s );
}

////////////////////////////////////////////////////////////////////////////////

void Atmosphere::setTemperatureSL( double temperature_0 )
{
    if ( temperature_0 > 173.15 && temperature_0 < 373.15 )
    {
        m_temperature_0 = temperature_0;
    }
    else
    {
        std::cerr << "WARNING! FDM Atmosphere wrong value of sea level temperature." << std::endl;
    }
}

////////////////////////////////////////////////////////////////////////////////

void Atmosphere::setPressureSL( double pressure_0 )
{
    if ( pressure_0 > 90000.0 && pressure_0 < 110000.0 )
    {
        m_pressure_0 = pressure_0;
    }
    else
    {
        std::cerr << "WARNING! FDM Atmosphere wrong value of sea level pressure." << std::endl;
    }
}
