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
#ifndef P51_COMPRESSOR_H
#define P51_COMPRESSOR_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/models/fdm_Compressor.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief P-51 compressor.
 */
class P51_Compressor : public Compressor
{
public:

    /** Constructor. */
    P51_Compressor();

    /** Destructor. */
    virtual ~P51_Compressor();

    /**
     * Reads data.
     * @param dataNode XML node
     */
    void readData( XmlNode &dataNode );

    /**
     * Updates compressor.
     * @param airPressure [Pa] air pressure
     * @param airDensity [kg/m^3] air density
     * @param airTemperature [K] air temperature
     * @param airFlow [kg/s] air flow
     * @param rpm [rpm] compressor rpm
     */
    void update( double airPressure, double airDensity, double airTemperature,
                 double airFlow, double rpm );

private:

    double _gearRatio_L;        ///< [-] low speed gear ratio (compressor rpm / engine rpm)
    double _gearRatio_H;        ///< [-] low speed gear ratio (compressor rpm / engine rpm)
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // P51_COMPRESSOR_H
