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

#include <fdm/models/fdm_AtmosphereUS76.h>

#include <cmath>

#include <fdm/fdm_Log.h>

#include <fdm/utils/fdm_WGS84.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

// US Standard Atmosphere 1976, Table 3, p.3
const double AtmosphereUS76::_m_i[] = {
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
const double AtmosphereUS76::_f_i[] = {
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
const double AtmosphereUS76::_h_b[] = {
    11000.0,
    20000.0,
    32000.0,
    47000.0,
    51000.0,
    71000.0,
    84852.0
};

// US Standard Atmosphere 1976, Table I, p.50-73
const double AtmosphereUS76::_p_b[] = {
    101325.0,
     22632.0,
      5474.8,
       868.01,
       110.9,
        66.938,
         3.9564
};

// US Standard Atmosphere 1976, Table I, p.50-73
const double AtmosphereUS76::_t_b[] = {
    288.15,
    216.65,
    216.65,
    228.65,
    270.65,
    270.65,
    214.65
};

// US Standard Atmosphere 1976, Table 4, p.3
const double AtmosphereUS76::_l_b[] = {
    -6.5e-3,
    0.0,
    1.0e-3,
    2.8e-3,
    0.0,
    -2.8e-3,
    -2.0e-3
};

// [kg/kmol] mean molecular weight, US Standard Atmosphere 1976, p.9
const double AtmosphereUS76::_m =
        ( AtmosphereUS76::_m_i[ AtmosphereUS76::N2  ] * AtmosphereUS76::_f_i[ AtmosphereUS76::N2  ]
        + AtmosphereUS76::_m_i[ AtmosphereUS76::O2  ] * AtmosphereUS76::_f_i[ AtmosphereUS76::O2  ]
        + AtmosphereUS76::_m_i[ AtmosphereUS76::Ar  ] * AtmosphereUS76::_f_i[ AtmosphereUS76::Ar  ]
        + AtmosphereUS76::_m_i[ AtmosphereUS76::CO2 ] * AtmosphereUS76::_f_i[ AtmosphereUS76::CO2 ]
        + AtmosphereUS76::_m_i[ AtmosphereUS76::Ne  ] * AtmosphereUS76::_f_i[ AtmosphereUS76::Ne  ]
        + AtmosphereUS76::_m_i[ AtmosphereUS76::He  ] * AtmosphereUS76::_f_i[ AtmosphereUS76::He  ]
        + AtmosphereUS76::_m_i[ AtmosphereUS76::Kr  ] * AtmosphereUS76::_f_i[ AtmosphereUS76::Kr  ]
        + AtmosphereUS76::_m_i[ AtmosphereUS76::Xe  ] * AtmosphereUS76::_f_i[ AtmosphereUS76::Xe  ]
        + AtmosphereUS76::_m_i[ AtmosphereUS76::CH4 ] * AtmosphereUS76::_f_i[ AtmosphereUS76::CH4 ]
        + AtmosphereUS76::_m_i[ AtmosphereUS76::H2  ] * AtmosphereUS76::_f_i[ AtmosphereUS76::H2  ] )
        /
        ( AtmosphereUS76::_f_i[ AtmosphereUS76::N2  ]
        + AtmosphereUS76::_f_i[ AtmosphereUS76::O2  ]
        + AtmosphereUS76::_f_i[ AtmosphereUS76::Ar  ]
        + AtmosphereUS76::_f_i[ AtmosphereUS76::CO2 ]
        + AtmosphereUS76::_f_i[ AtmosphereUS76::Ne  ]
        + AtmosphereUS76::_f_i[ AtmosphereUS76::He  ]
        + AtmosphereUS76::_f_i[ AtmosphereUS76::Kr  ]
        + AtmosphereUS76::_f_i[ AtmosphereUS76::Xe  ]
        + AtmosphereUS76::_f_i[ AtmosphereUS76::CH4 ]
        + AtmosphereUS76::_f_i[ AtmosphereUS76::H2  ] );

const double AtmosphereUS76::_r     = 8.31432e3;    // US Standard Atmosphere 1976, Table 2, p.2
const double AtmosphereUS76::_s     = 110.0;        // US Standard Atmosphere 1976, Table 2, p.2
const double AtmosphereUS76::_beta  = 1.458e-6;     // US Standard Atmosphere 1976, Table 2, p.2
const double AtmosphereUS76::_gamma = 1.4;          // US Standard Atmosphere 1976, Table 2, p.2

const double AtmosphereUS76::_std_sl_p = 101325.0;  // US Standard Atmosphere 1976, Table 2, p.2
const double AtmosphereUS76::_std_sl_t = 288.15;    // US Standard Atmosphere 1976, Table 2, p.2
const double AtmosphereUS76::_std_sl_rho = 1.225;

////////////////////////////////////////////////////////////////////////////////

double AtmosphereUS76::getDensityAltitude( double pressure, double temperature,
                                       double altitude )
{
    static double b = ( -_l_b[ 0 ]*_r  ) / ( WGS84::_g*_m + _l_b[ 0 ]*_r  );

    double result = altitude;

    if ( altitude < _h_b[ 0 ] )
    {
        double a = ( pressure / _std_sl_p ) / ( temperature / _std_sl_t );
        result = -( _std_sl_t / _l_b[ 0 ] ) * ( 1.0 - pow( a, b ) );
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

AtmosphereUS76::AtmosphereUS76() :
    _temperature_0 ( _std_sl_t ),
    _pressure_0    ( _std_sl_p )
{
    update( 0.0 );
}

////////////////////////////////////////////////////////////////////////////////

AtmosphereUS76::~AtmosphereUS76() {}

////////////////////////////////////////////////////////////////////////////////

void AtmosphereUS76::update( double altitude )
{
    double h_b = _h_b[ 5 ];
    double p_b = _p_b[ 6 ];
    double t_b = _t_b[ 6 ];
    double l_b = 0.0;

    if ( altitude < _h_b[ 0 ] )
    {
        h_b = 0.0;
        p_b = _p_b[ 0 ];
        t_b = _temperature_0;
        l_b = -( _temperature_0 - _t_b[ 1 ] ) / _h_b[ 0 ];
    }
    else
    {
        for ( int i = 1; i < 7; i++ )
        {
            if ( altitude < _h_b[ i ] )
            {
                h_b = _h_b[ i - 1 ];
                p_b = _p_b[ i ];
                t_b = _t_b[ i ];
                l_b = _l_b[ i ];

                break;
            }
        }

        if ( altitude > _h_b[ 6 ] )
        {
            Log::w() << "Atmosphere altitude above valid range." << std::endl;
        }
    }

    double delta_h = altitude - h_b;

    // [K] temperature, US Standard Atmosphere 1976, p.10
    _temperature = t_b + l_b * delta_h;

    // [Pa] pressure, US Standard Atmosphere 1976, p.12
    if ( fabs( l_b ) < 1.0e-6 )
    {
        _pressure = p_b * exp( -( WGS84::_g * _m * delta_h ) / ( _r * t_b ) );
    }
    else
    {
        _pressure = p_b * pow( t_b / _temperature, ( WGS84::_g * _m ) / ( _r * l_b ) );

        if ( altitude < _h_b[ 0 ] )
        {
            double c_h = ( _h_b[ 0 ] - altitude ) / _h_b[ 0 ];
            double c_p = ( c_h * _pressure_0 + ( 1.0 - c_h ) * _std_sl_p ) / _std_sl_p;

            _pressure = _pressure * c_p;
        }
    }

    // [kg/m^3] density, US Standard Atmosphere 1976, p.15
    _density = ( _pressure * _m ) / ( _r * _temperature );

    // [m/s] speed of sound, US Standard Atmosphere 1976, p.18
    _speedOfSound = sqrt( ( _gamma * _r * _temperature ) / _m );

    // [Pa*s] dynamic viscosity, US Standard Atmosphere 1976, p.19
    _dynViscosity = _beta * pow( _temperature, 3.0 / 2.0 ) / ( _temperature + _s );

    // [m^2/s] kinematic viscosity, US Standard Atmosphere 1976, p.19
    _kinViscosity = _dynViscosity / _density;
}

////////////////////////////////////////////////////////////////////////////////

void AtmosphereUS76::setPressureSL( double pressure_0 )
{
    if ( pressure_0 > 90000.0 && pressure_0 < 110000.0 )
    {
        _pressure_0 = pressure_0;
    }
    else
    {
        Log::w() << "Atmosphere wrong value of sea level pressure." << std::endl;
    }
}

////////////////////////////////////////////////////////////////////////////////

void AtmosphereUS76::setTemperatureSL( double temperature_0 )
{
    if ( temperature_0 > 173.15 && temperature_0 < 373.15 )
    {
        _temperature_0 = temperature_0;
    }
    else
    {
        Log::w() << "Atmosphere wrong value of sea level temperature." << std::endl;
    }
}
