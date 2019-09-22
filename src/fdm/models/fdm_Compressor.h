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
#ifndef FDM_COMPRESSOR_H
#define FDM_COMPRESSOR_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_Base.h>

#include <fdm/utils/fdm_Table2D.h>

#include <fdm/xml/fdm_XmlNode.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Generic compressor base class.
 *
 * <h5>XML configuration file format:</h5>
 * @code
 * <compressor>
 *   <performance_map>
 *     { [rpm] compressor speed } ... { more values of compressor speed }
 *     { [kg/s] air flow } { [-] pressure ratio } ... { more values of pressure ratio }
 *     ... { more entries }
 *   </performance_map>
 * </compressor>
 * @endcode
 */
class FDMEXPORT Compressor : public Base
{
public:

    static const double _gamma; ///< [-] dry air heat capacity ratio

    /** Constructor. */
    Compressor();

    /** Destructor. */
    virtual ~Compressor();

    /**
     * Reads data.
     * @param dataNode XML node
     */
    virtual void readData( XmlNode &dataNode );

    /**
     * Updates compressor.
     * @param airPressure [Pa] air pressure
     * @param airDensity [kg/m^3] air density
     * @param airTemperature [K] air temperature
     * @param airFlow [kg/s] air flow
     * @param rpm [rpm] compressor rpm
     */
    virtual void update( double airPressure, double airDensity, double airTemperature,
                         double airFlow, double rpm );

    inline double getTemperature() const { return _temperature; }
    inline double getPressure()    const { return _pressure;    }
    inline double getDensity()     const { return _density;     }

protected:

    Table2D _performance_map;   ///< [-] performance map

    double _temperature;        ///< [K] compressed air temperature
    double _pressure;           ///< [Pa] compressed air static pressure
    double _density;            ///< [kg/m^3] compressed air density
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_COMPRESSOR_H
