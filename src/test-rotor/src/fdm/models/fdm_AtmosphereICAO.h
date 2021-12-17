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
#ifndef FDM_ATMOSPHERE_ICAO_H
#define FDM_ATMOSPHERE_ICAO_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_Defines.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief ICAO Standard Atmosphere class.
 *
 * @see Manual of the ICAO Standard Atmosphere, ICAO, Doc 7488/3, 1993
 */
class FDMEXPORT AtmosphereICAO
{
public:

    static const double _h_b[ 9 ];      ///< [m] altitude values
    static const double _t_b[ 9 ];      ///< [K] temperature values
    static const double _l_b[ 8 ];      ///< [K/m] temperature gradients

    static const double _g_0;           ///< [m/s^2]
    static const double _m_0;           ///< [kg/kmol]
    static const double _p_0;           ///< [Pa]

    /** @brief Constructor. */
    AtmosphereICAO();

    /** @brief Destructor. */
    virtual ~AtmosphereICAO();

    /**
     * @brief Updates atmosphere due to altitude.
     * @param altitude [m] altitude above sea level
     */
    virtual void update( double altitude );

};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_ATMOSPHERE_ICAO_H
