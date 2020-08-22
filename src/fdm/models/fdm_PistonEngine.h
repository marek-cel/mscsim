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
#ifndef FDM_PISTONENGINE_H
#define FDM_PISTONENGINE_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/models/fdm_Engine.h>

#include <fdm/utils/fdm_Table1.h>
#include <fdm/xml/fdm_XmlNode.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Piston engine class.
 *
 * XML configuration file format:
 * @code
 * <engine>
 *   <rpm_min> { [rpm] minimum rpm } </rpm_min>
 *   <rpm_max> { [rpm] maximum rpm } </rpm_max>
 *   <starter> { [N*m] starter torque } </starter>
 *   <displacement> { [m^3] engine displacement } </displacement>
 *   <inertia> { [kg*m^2] polar moment of inertia } </inertia>
 *   <sfc> { [kg/(W*s)] specific fuel consumption } </sfc>
 *   <power_rpm>
 *     { [rpm] engine rpm } { [W] power }
 *     ... { more entries }
 *   </power_rpm>
 *   <power_throttle>
 *     { [-] throttle } { [-] power coefficient }
 *     ... { more entries }
 *   </power_throttle>
 *   <power_altitude>
 *     { [h] altitude } { [-] power coefficient }
 *     ... { more entries }
 *   </power_altitude>
 *   <mixture>
 *     { [-] mixture lever position } { [-] mixture }
 *     ... { more entries }
 *   </mixture>
 *   <power_factor>
 *     { [-] fuel to air ratio } { [-] power factor }
 *     ... { more entries }
 *   </power_factor>
 *   <map_throttle>
 *     { [-] throttle } { [-] manifold absolute pressure ratio due to throttle }
 *     ... { more entries }
 *   </map_throttle>
 *   <map_rpm>
 *     { [rpm] engine rpm } { [-] manifold absolute pressure ratio due to rpm }
 *     ... { more entries }
 *   </map_rpm>
 * </engine>
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
     * @param mixtureLever  [0.0,1.0] mixture lever position
     * @param rpm           [rpm]     engine rpm
     * @param airPressure   [Pa]      air pressure
     * @param airDensity    [kg/m^3]  air density
     * @param densityAlt    [m]       air density altitude
     * @param fuel          specifies if fuel is provided
     * @param starter       specifies if starter is enabled
     * @param magneto_l     specifies if left magneto is enabled
     * @param magneto_r     specifies if right magneto is enabled
     */
    virtual void update( double throttleLever,
                         double mixtureLever,
                         double rpm,
                         double airPressure,
                         double airDensity,
                         double densityAlt,
                         bool fuel,
                         bool starter,
                         bool magneto_l = true,
                         bool magneto_r = true );

    /**
     * Returns engine air flow.
     * @return [kg/s] engine air flow
     */
    inline double getAirFlow() const
    {
        return _airFlow;
    }

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

    void setRPM( double rpm );

protected:

    Table1 _power_rpm;          ///< [W] power vs engine rpm
    Table1 _power_throttle;     ///< [-] power coefficient vs throttle
    Table1 _power_altitude;     ///< [-] power coefficient vs altitude
    Table1 _mixture;            ///< [-] mixture vs mixture lever position
    Table1 _power_factor;       ///< [-] power factor vs fuel to air ratio
    Table1 _map_throttle;       ///< [-] manifold absolute pressure ratio due to throttle
    Table1 _map_rpm;            ///< [-] manifold absolute pressure ratio due to engine rpm

    double _rpm_min;            ///< [rpm] engine minimum rpm
    double _rpm_max;            ///< [rpm] engine maximum rpm
    double _starter;            ///< [N*m] starter torque
    double _displacement;       ///< [m^3] displacement
    double _inertia;            ///< [kg*m^2] polar moment of inertia
    double _specFuelCons;       ///< [kg/(W*s)] specific fuel consumption

    double _rpm;                ///< [rpm] engine rpm
    double _map;                ///< [Pa] manifold absolute pressure
    double _power;              ///< [W] net power
    double _torque;             ///< [N*m] torque
    double _airFlow;            ///< [kg/s] air flow
    double _fuelFlow;           ///< [kg/s] fuel flow

    /**
     * Computes manifold absolute pressure.
     * @param throttleLever [0.0,1.0] throttle lever position
     * @param rpm [rpm] engine rpm
     * @param airPressure [Pa] air pressure
     * @return [Pa] manifold absolute pressure
     */
    virtual double getManifoldAbsolutePressure( double throttleLever,
                                                double rpm, double airPressure );

    /**
     * Computes fuel to air ratio.
     * @param mixture [-] mixture
     * @param airDensity [kg/m^3] air density
     * @return [-] fuel to air ratio
     */
    virtual double getFuelToAirRatio( double mixture, double airDensity );

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
     * Computes engine net power.
     * @param throttleLever [0.0,1.0] throttle lever position
     * @param mixtureLever [0.0,1.0] mixture lever position
     * @param rpm [rpm] engine rpm
     * @param airDensity [kg/m^3] air density
     * @param densityAltitude [m] air density altitude
     * @param fuel specifies if fuel is provided
     * @param magneto_l specifies if left magneto is enabled
     * @param magneto_r specifies if right magneto is enabled
     * @return [W] net power
     */
    virtual double getNetPower( double throttleLever, double mixtureLever, double rpm,
                                double airDensity, double densityAltitude,
                                bool fuel, bool magneto_l, bool magneto_r );
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_PISTONENGINE_H
