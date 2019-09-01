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
#ifndef FDM_PISTONENGINE_H
#define FDM_PISTONENGINE_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/models/fdm_Engine.h>

#include <fdm/utils/fdm_Table.h>
#include <fdm/xml/fdm_XmlNode.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Piston engine class.
 *
 * <h5>XML configuration file format:</h5>
 * @code
 * <piston_engine>
 *   <power_max> { [W] engine maximum power } </power_max>
 *   <starter> { [N*m] starter torque } </starter>
 *   <rpm_min> { [rpm] minimum rpm } </rpm_min>
 *   <rpm_max> { [rpm] maximum rpm } </rpm_max>
 *   <sfc> { [kg/(W*s)] specific fuel consumption } </sfc>
 *   <inertia> { [kg*m^2] polar moment of inertia } </inertia>
 *   <mixture>
 *     { [-] mixture } { [-] mixture lever position }
 *     ... { more entries }
 *   </mixture>
 *   <throttle>
 *     { [-] throttle } { [-] throttle lever position }
 *     ... { more entries }
 *   </throttle>
 *   <power_factor>
 *     { [-] fuel to air ratio } { [-] power factor }
 *     ... { more entries }
 *   </power_factor>
 * </piston_engine>
 * @endcode
 *
 * @see Allerton D.: Principles of Flight Simulation, 2009, p.128
 */
class FDMEXPORT PistonEngine : public Engine
{
public:

    /** Constructor. */
    PistonEngine();

    /** Destructor. */
    virtual ~PistonEngine();

    /**
     * Reads data.
     * @param dataNode XML node
     */
    virtual void readData( XmlNode &dataNode );

    /**
     * Updates engine.
     * @param throttleLever [0.0,1.0] throttle lever position
     * @param mixtureLever [0.0,1.0] mixture lever position
     * @param rpm [rpm] engine rpm
     * @param airPressure [Pa] air pressure
     * @param airDensity [kg/m^3] air density
     * @param fuel specifies if fuel is provided
     * @param starter specifies if starter is enabled
     * @param magneto_l specifies if left magneto is enabled
     * @param magneto_r specifies if right magneto is enabled
     */
    virtual void update( double throttleLever, double mixtureLever, double rpm,
                         double airPressure, double airDensity,
                         bool fuel, bool starter,
                         bool magneto_l = true, bool magneto_r = true );

    /**
     * Returns engine fuel consumption.
     * @return [kg/s] engine fuel consumption
     */
    inline double getFuelFlow() const
    {
        return _fuelFlow;
    }

    /**
     * Returns engine polar moment of inertia.
     * @return [kg*m^2] engine polar moment of inertia
     */
    inline double getInertia() const
    {
        return _inertia;
    }

    /**
     * Returns engine manifold absolute pressure.
     * @return [Pa] engine manifold absolute pressure
     */
    inline double getMAP() const
    {
        return _map;
    }

    /**
     * Returns engine net power.
     * @return [W] engine net power
     */
    inline double getPower()  const
    {
        return _power;
    }

    /**
     * Returns engine rpm.
     * @return [rpm] engine rpm
     */
    inline double getRPM() const
    {
        return _rpm;
    }

    /**
     * Returns engine torque.
     * @return [N*m] engine torque
     */
    inline double getTorque() const
    {
        return _torque;
    }

protected:

    Table _mixture;             ///< [-] mixture vs mixture lever position
    Table _throttle;            ///< [-] throttle vs throttle lever position

    Table _powerFactor;         ///< [-] power factor

    double _power_max;          ///< [W] maximum power
    double _starter;            ///< [N*m] starter torque
    double _rpm_min;            ///< [rpm] engine minimum rpm
    double _rpm_max;            ///< [rpm] engine maximum rpm
    double _specFuelCons;       ///< [kg/(W*s)] specific fuel consumption
    double _inertia;            ///< [kg*m^2] polar moment of inertia

    double _rpm;                ///< [rpm] engine rpm
    double _map;                ///< [Pa] manifold absolute pressure
    double _power;              ///< [W] net power
    double _torque;             ///< [N*m] torque
    double _fuelFlow;           ///< [kg/s] fuel flow

    /**
     * Computes manifold absolute pressure.
     * @param throttle <0.0;1.0> throttle
     * @param rpm [rpm] engine rpm
     * @param airPressure [Pa] air pressure
     * @return [Pa] manifold absolute pressure
     */
    virtual double getManifoldAbsolutePressure( double throttle, double rpm,
                                                double airPressure );

    /**
     * Computes engine power factor.
     * @param fuel specifies if fuel is provided
     * @param mixture [-] mixture
     * @param airDensity [kg/m^3] air density
     * @param magneto_l specifies if left magneto is enabled
     * @param magneto_r specifies if right magneto is enabled
     * @return [-] power factor
     */
    virtual double getPowerFactor( double mixture, double airDensity, bool fuel,
                                   bool magneto_l = true, bool magneto_r = true );

    /**
     * Computes engine power losses.
     * @param rpm [rpm] engine rpm
     * @return [W] power losses
     */
    virtual double getPowerLosses( double rpm );

    /**
     * Computes engine static power (the power produced by combustion).
     * @param rpm [rpm] engine rpm
     * @param map [Pa] manifold absolute pressure
     * @return [W] static power
     */
    virtual double getStaticPower( double rpm, double map );
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_PISTONENGINE_H
