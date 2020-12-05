/****************************************************************************//*
 * Copyright (C) 2020 Marek M. Cel
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
#ifndef FDM_ENVIRONMENT_H
#define FDM_ENVIRONMENT_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/models/fdm_Atmosphere.h>

#include <fdm/utils/fdm_Vector3.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Environment model interface class.
 */
class FDMEXPORT Environment
{
public:

    /** @brief Constructor. */
    Environment();

    /** @brief Destructor. */
    virtual ~Environment();

    /**
     * @brief Updates environment.
     * @param altitude_asl [m] altitude above sea level
     */
    virtual void update( double altitude_asl );

    inline double getTemperature()     const { return _temperature;     }
    inline double getPressure()        const { return _pressure;        }
    inline double getDensity()         const { return _density;         }
    inline double getSpeedOfSound()    const { return _speedOfSound;    }
    inline double getDensityAltitude() const { return _densityAltitude; }

    inline Vector3 getWind_NED() const { return _wind_ned; }

    /**
     * @brief Sets sea level air temperature value.
     * @param slAirTemperature [K] sea level air temperature
     */
    inline void setTemperatureSL( double temperature_0 )
    {
        _atmosphere->setTemperatureSL( temperature_0 );
    }

    /**
     * @brief Sets sea level air pressure value.
     * @param slAirPressure [Pa] sea level air pressure
     */
    inline void setPressureSL( double pressure_0 )
    {
        _atmosphere->setPressureSL( pressure_0 );
    }

    inline void setWindDirection( double wind_direction )
    {
        _wind_direction = wind_direction;
    }

    inline void setWindSpeed( double wind_speed )
    {
        _wind_speed = wind_speed;
    }

protected:

    Atmosphere *_atmosphere;    ///< atmosphere object

    double _temperature;        ///< [K] air temperature
    double _pressure;           ///< [Pa] air static pressure
    double _density;            ///< [kg/m^3] air density
    double _speedOfSound;       ///< [m/s] speed of sound
    double _densityAltitude;    ///< [m] density altitude

    double _wind_direction;     ///< [rad] wind direction (from which it originates)
    double _wind_speed;         ///< [m/s] wind speed

    Vector3 _wind_ned;          ///< [m/s] wind speed vector expressed in BAS
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_ENVIRONMENT_H
