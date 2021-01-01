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
#ifndef G1000_CONST_H
#define G1000_CONST_H

////////////////////////////////////////////////////////////////////////////////

namespace g1000
{

/**
 * @brief Constant values class.
 *
 * @see Department of Defence World Geodetic System 1984, NIMA, TR-8350.2, 2000
 * @see US Standard Atmosphere 1976, NASA, TM-X-74335
 */
class Const
{
public:

    static const double _g;             ///< [m/s^2] standard gravitional acceleration

    static const double _r;             ///< [J/(mol*K)] universal gas constant
    static const double _r_spec_air;    ///< [J/(kg*K)] specific gas constant (air)

    static const double _dt_dh;         ///< [K/m] temperature gradient (below 11000 m)

    static const double _std_sl_t;      ///< [K]  standard sea level temperature (15 deg C)
    static const double _std_sl_p;      ///< [Pa] standard sea level pressure (1013.25 hPa)
    static const double _std_sl_rho;    ///< [kg/m^3] standard sea level density (1.225 kg/m^3)
};

} // end of g1000 namespace

////////////////////////////////////////////////////////////////////////////////

#endif // G1000_CONST_H
