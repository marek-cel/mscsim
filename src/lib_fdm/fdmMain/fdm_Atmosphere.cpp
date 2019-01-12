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

#include <fdmUtils/fdm_WGS84.h>

#include <iostream>
#include <math.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

// US Standard Atmosphere 1976, Table 3, p.3
const double Atmosphere::m_m_i[] = {
    28.0134,
    31.9988,
    39.948,
    44.00995,
    20.183,
    4.0026,
    83.8,
    131.3,
    16.04303,
    2.01594
};

// US Standard Atmosphere 1976, Table 3, p.3
const double Atmosphere::m_f_i[] = {
    0.78084,
    0.209476,
    0.00934,
    0.000314,
    0.00001818,
    0.00000524,
    0.00000114,
    0.000000087,
    0.000002,
    0.0000005
};

// US Standard Atmosphere 1976, Table 4, p.3
const double Atmosphere::m_h_b[] = {
    11000.0,
    20000.0,
    32000.0,
    47000.0,
    51000.0,
    71000.0,
    84852.0
};

// US Standard Atmosphere 1976, Table I, p.50-73
const double Atmosphere::m_p_b[] = {
    101325.0,
     22632.0,
      5474.8,
       868.01,
       110.9,
        66.938,
         3.9564
};

// US Standard Atmosphere 1976, Table I, p.50-73
const double Atmosphere::m_t_b[] = {
    288.15,
    216.65,
    216.65,
    228.65,
    270.65,
    270.65,
    214.65
};

// US Standard Atmosphere 1976, Table 4, p.3
const double Atmosphere::m_l_b[] = {
    -6.5e-3,
    0.0,
    1.0e-3,
    2.8e-3,
    0.0,
    -2.8e-3,
    -2.0e-3
};

// [kg/kmol] mean molecular weight, US Standard Atmosphere 1976, p.9
const double Atmosphere::m_m =
        ( Atmosphere::m_m_i[ Atmosphere::N2  ] * Atmosphere::m_f_i[ Atmosphere::N2  ]
        + Atmosphere::m_m_i[ Atmosphere::O2  ] * Atmosphere::m_f_i[ Atmosphere::O2  ]
        + Atmosphere::m_m_i[ Atmosphere::Ar  ] * Atmosphere::m_f_i[ Atmosphere::Ar  ]
        + Atmosphere::m_m_i[ Atmosphere::CO2 ] * Atmosphere::m_f_i[ Atmosphere::CO2 ]
        + Atmosphere::m_m_i[ Atmosphere::Ne  ] * Atmosphere::m_f_i[ Atmosphere::Ne  ]
        + Atmosphere::m_m_i[ Atmosphere::He  ] * Atmosphere::m_f_i[ Atmosphere::He  ]
        + Atmosphere::m_m_i[ Atmosphere::Kr  ] * Atmosphere::m_f_i[ Atmosphere::Kr  ]
        + Atmosphere::m_m_i[ Atmosphere::Xe  ] * Atmosphere::m_f_i[ Atmosphere::Xe  ]
        + Atmosphere::m_m_i[ Atmosphere::CH4 ] * Atmosphere::m_f_i[ Atmosphere::CH4 ]
        + Atmosphere::m_m_i[ Atmosphere::H2  ] * Atmosphere::m_f_i[ Atmosphere::H2  ] )
        /
        ( Atmosphere::m_f_i[ Atmosphere::N2  ]
        + Atmosphere::m_f_i[ Atmosphere::O2  ]
        + Atmosphere::m_f_i[ Atmosphere::Ar  ]
        + Atmosphere::m_f_i[ Atmosphere::CO2 ]
        + Atmosphere::m_f_i[ Atmosphere::Ne  ]
        + Atmosphere::m_f_i[ Atmosphere::He  ]
        + Atmosphere::m_f_i[ Atmosphere::Kr  ]
        + Atmosphere::m_f_i[ Atmosphere::Xe  ]
        + Atmosphere::m_f_i[ Atmosphere::CH4 ]
        + Atmosphere::m_f_i[ Atmosphere::H2  ] );

const double Atmosphere::m_r     = 8.31432e3;   // US Standard Atmosphere 1976, Table 2, p.2
const double Atmosphere::m_s     = 110.0;       // US Standard Atmosphere 1976, Table 2, p.2
const double Atmosphere::m_beta  = 1.458e-6;    // US Standard Atmosphere 1976, Table 2, p.2
const double Atmosphere::m_gamma = 1.4;         // US Standard Atmosphere 1976, Table 2, p.2

const double Atmosphere::m_std_sl_p = 101325.0; // US Standard Atmosphere 1976, Table 2, p.2
const double Atmosphere::m_std_sl_t = 288.15;   // US Standard Atmosphere 1976, Table 2, p.2

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
    double h_b = m_h_b[ 5 ];
    double p_b = m_p_b[ 6 ];
    double t_b = m_t_b[ 6 ];
    double l_b = 0.0;

    if ( altitude < m_h_b[ 0 ] )
    {
        h_b = 0.0;
        p_b = m_p_b[ 0 ];
        t_b = m_temperature_0;
        l_b = -( m_temperature_0 - m_t_b[ 1 ] ) / m_h_b[ 0 ];
    }
    else
    {
        for ( int i = 1; i < 7; i++ )
        {
            if ( altitude < m_h_b[ i ] )
            {
                h_b = m_h_b[ i - 1 ];
                p_b = m_p_b[ i ];
                t_b = m_t_b[ i ];
                l_b = m_l_b[ i ];

                break;
            }
        }

        if ( altitude > m_h_b[ 6 ] )
        {
            std::cerr << "WARNING! Atmosphere altitude above valid range." << std::endl;
        }
    }

    double delta_h = altitude - h_b;

    // [K] temperature, US Standard Atmosphere 1976, p.10
    m_temperature = t_b + l_b * delta_h;

    // [Pa] pressure, US Standard Atmosphere 1976, p.12
    if ( fabs( l_b ) < 1.0e-6 )
    {
        m_pressure = p_b * exp( -( WGS84::m_g * m_m * delta_h ) / ( m_r * t_b ) );
    }
    else
    {
        m_pressure = p_b * pow( t_b / m_temperature, ( WGS84::m_g * m_m ) / ( m_r * l_b ) );

        if ( altitude < m_h_b[ 0 ] )
        {
            m_pressure = ( m_pressure / m_std_sl_p ) * m_pressure_0;
        }
    }

    // [kg/m^3] density, US Standard Atmosphere 1976, p.15
    m_density = ( m_pressure * m_m ) / ( m_r * m_temperature );

    // [m/s] speed of sound, US Standard Atmosphere 1976, p.18
    m_speedOfSound = sqrt( ( m_gamma * m_r * m_temperature ) / m_m );

    // [Pa*s] dynamic viscosity, US Standard Atmosphere 1976, p.19
    m_viscosity = m_beta * pow( m_temperature, 3.0 / 2.0 ) / ( m_temperature + m_s );
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
        std::cerr << "WARNING! Atmosphere wrong value of sea level pressure." << std::endl;
    }
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
        std::cerr << "WARNING! Atmosphere wrong value of sea level temperature." << std::endl;
    }
}
