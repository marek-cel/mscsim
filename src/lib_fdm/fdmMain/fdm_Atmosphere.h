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
#ifndef FDM_ATMOSPHERE_H
#define FDM_ATMOSPHERE_H

////////////////////////////////////////////////////////////////////////////////

#include <fdmMain/fdm_Defines.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief US Standard Atmosphere class.
 *
 * @see US Standard Atmosphere 1976, NASA, TM-X-74335
 */
class FDMEXPORT Atmosphere
{
public:

    static const double m_g;        ///< [m/s^2] mean gravity acceleration
    static const double m_r;        ///< [N*m/(kmol*K)] gas constant
    static const double m_s;        ///< [K] Sutherland constant
    static const double m_b;        ///< [kg/(s*m*K^0.5)] quantity constant in the expression for dynamic viscosity

    static const double m_std_sl_t; ///< [K]  standard sea level temperature (15 deg C)
    static const double m_std_sl_p; ///< [Pa] standard sea level pressure (1013.25 hPa)

    /** Constructor. */
    Atmosphere();

    /** Destructor. */
    virtual ~Atmosphere();

    /**
     * Updates atmosphere due to altitude.
     * @param altitude [m] altitude above sea level
     */
    virtual void update( double altitude );

    /**
     * Sets sea level air temperature value.
     * @param slAirTemperature [K] sea level air temperature
     */
    virtual void setTemperatureSL( double temperature_0 );

    /**
     * Sets sea level air pressure value.
     * @param slAirPressure [Pa] sea level air pressure
     */
    virtual void setPressureSL( double pressure_0 );

    inline double getTemperature()  const { return m_temperature;  }
    inline double getPressure()     const { return m_pressure;     }
    inline double getDensity()      const { return m_density;      }
    inline double getSpeedOfSound() const { return m_speedOfSound; }

private:

    double m_temperature_0;     ///< [K] sea level air temperature
    double m_pressure_0;        ///< [Pa] sea level air pressure

    double m_temperature;       ///< [K] air temperature
    double m_pressure;          ///< [Pa] air static pressure
    double m_density;           ///< [kg/m^3] air density
    double m_speedOfSound;      ///< [m/s] speed of sound
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_ATMOSPHERE_H
