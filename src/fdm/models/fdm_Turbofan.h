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
#ifndef FDM_TURBOFAN_H
#define FDM_TURBOFAN_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/utils/fdm_Vector3.h>

#include <fdm/xml/fdm_XmlNode.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Twin-spool turbofan engine class.
 */
class FDMEXPORT Turbofan
{
public:

    /** Engine state enum. */
    enum State
    {
        Stopped  = 0,   ///< engine stopped
        Starting = 1,   ///< engine starting
        Running  = 2    ///< engine running
    };

    /** @brief Constructor. */
    Turbofan();

    /** @brief Destructor. */
    virtual ~Turbofan();

    /**
     * @brief Reads data.
     * @param dataNode XML node
     */
    virtual void readData( XmlNode &dataNode );

    /** */
    virtual void initialize( bool engineOn );

    /**
     * @brief Computes thrust.
     * @param machNumber [-] Mach number
     * @param densityAltitude [m] air density altitude
     */
    virtual void computeThrust( double machNumber, double densityAltitude );

    /**
     * @brief Integrates model.
     * @param timeStep [s] time step
     */
    virtual void integrate( double timeStep );

    /**
     * @brief Updates engine.
     * @param throttle [0.0,1.0] throttle lever position
     * @param temperature [deg C] air temperature
     * @param machNumber [-] Mach number
     * @param densityAltitude [m] air density altitude
     * @param fuel specifies if fuel is provided
     * @param starter specifies if starter is enabled
     */
    virtual void update( double throttle,
                         double temperature,
                         double machNumber,
                         double densityAltitude,
                         bool fuel,
                         bool starter );

    /**
     * @brief Returns propeller position expressed in BAS.
     * @return [m] propeller position expressed in BAS
     */
    inline const Vector3& getPos_BAS() const
    {
        return _pos_bas;
    }

    /**
     * @brief Returns engine fuel consumption.
     * @return [kg/s] engine fuel consumption
     */
    inline double getFuelFlow() const
    {
        return _fuelFlow;
    }

    /** */
    inline double getN1() const
    {
        return _n1;
    }

    /** */
    inline double getN2() const
    {
        return _n2;
    }

    /** */
    inline double getTIT() const
    {
        return _tit;
    }

    /**
     * @brief Returns engine thrust.
     * @return [N] propeller thrust
     */
    inline double getThrust() const
    {
        return _thrust;
    }

protected:

    State _state;           ///< engine state

    Vector3 _pos_bas;       ///< [m] nozzle position expressed in BAS

    double _n1;             ///< low  pressure compressor rpm
    double _n2;             ///< high pressure compressor rpm
    double _tit;            ///< [deg C] turbine inlet temperature (TIT)
    double _fuelFlow;       ///< [kg/s] fuel flow
    double _thrust;         ///< [N] thrust
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_TURBOFAN_H
