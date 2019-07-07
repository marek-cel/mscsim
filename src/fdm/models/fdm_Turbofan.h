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
#ifndef FDM_TURBOFAN_H
#define FDM_TURBOFAN_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/models/fdm_Engine.h>

#include <fdm/utils/fdm_Table.h>
#include <fdm/utils/fdm_Table2D.h>
#include <fdm/utils/fdm_Vector3.h>

#include <fdm/xml/fdm_XmlNode.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Twin-spool turbofan engine class.
 *
 * @see Nguyen L., et al.: Simulator Study of Stall/Post-Stall Characteristics of a Fighter Airplane With Relaxed Longitudinal Static Stability, NASA-TP-1538
 * @see Gilbert W., et al.: Simulator Study of the Effectiveness of an Automatic Control System Designed to Improve the High-Angle-of-Attack Characteristics of a Fighter Airplane, NASA-TN-D-8176
 *
 * <h5>XML configuration file format:</h5>
 * @code
 * <turbofan_ab>
 *   <position> { [m] x-coordinate } { [m] y-coordinate } { [m] z-coordinate } </position>
 *   <thrust_max> { [N] maximum military thrust } </thrust_max>
 *   <time_constant_n1>
 *     { [-] difference of N1 setpoint and N1 } { [s] N1 time constant }
 *     ... { more entries }
 *   </time_constant_n1>
 *   <time_constant_n2>
 *     { [-] difference of N2 setpoint and N2 } { [s] N2 time constant }
 *     ... { more entries }
 *   </time_constant_n2>
 *   <time_constant_thrust>
 *     { [-] difference of normalized engine power command and engine power } { [s] thrust time constant }
 *     ... { more entries }
 *   </time_constant_thrust>
 *   <time_constant_tit> { TIT time constant } </time_constant_tit>
 *   <n1_throttle>
 *     { throttle position } { N1 }
 *     ... { more entries }
 *   </n1_throttle>
 *   <n2_throttle>
 *     { throttle position } { N2 }
 *     ... { more entries }
 *   </n2_throttle>
 *   <tit_n2>
 *     { N2 } { [deg C] TIT }
 *     ... { more entries }
 *   </tit_n2>
 *   <tsfc> { [kg/(N*s)] thrust specific fuel consumption } </tsfc>
 *   <thrust_factor_idle>
 *     { [kg/m^3] air density } ... { more values of air density }
 *     { [-] Mach number } { [-] thrust factor } ... { more values of thrust }
 *     ... { more entries }
 *   </thrust_factor_idle>
 *   <thrust_factor_max>
 *     { [kg/m^3] air density } ... { more values of air density }
 *     { [-] Mach number } { [-] thrust factor } ... { more values of thrust }
 *     ... { more entries }
 *   </thrust_factor_max>
 * </turbofan_ab>
 * @endcode
 */
class FDMEXPORT Turbofan : public Engine
{
public:

    /** Constructor. */
    Turbofan();

    /** Destructor. */
    virtual ~Turbofan();

    /**
     * Reads data.
     * @param dataNode XML node
     */
    virtual void readData( XmlNode &dataNode );

    /** */
    virtual void initialize( bool engineOn );

    /**
     * Computes thrust.
     * @param machNumber [-] Mach number
     * @param densityAltitude [m] air density altitude
     */
    virtual void computeThrust( double machNumber, double densityAltitude );

    /**
     * Integrates model.
     * @param timeStep [s] time step
     */
    virtual void integrate( double timeStep );

    /**
     * Updates engine.
     * @param throttle [0.0,1.0] throttle lever position
     * @param temperature [deg C] air temperature
     * @param machNumber [-] Mach number
     * @param densityAltitude [m] air density altitude
     * @param fuel specifies if fuel is provided
     * @param starter specifies if starter is enabled
     */
    virtual void update( double throttle, double temperature,
                         double machNumber, double densityAltitude,
                         bool fuel, bool starter );

    /**
     * Returns propeller position expressed in BAS.
     * @return [m] propeller position expressed in BAS
     */
    inline const Vector3& getPos_BAS() const
    {
        return m_pos_bas;
    }

    /**
     * Returns engine fuel consumption.
     * @return [kg/s] engine fuel consumption
     */
    inline double getFuelFlow() const
    {
        return m_fuelFlow;
    }

    /** */
    inline double getN1() const
    {
        return m_n1;
    }

    /** */
    inline double getN2() const
    {
        return m_n2;
    }

    /** */
    inline double getTIT() const
    {
        return m_tit;
    }

    /**
     * Returns engine thrust.
     * @return [N] propeller thrust
     */
    inline double getThrust() const
    {
        return m_thrust;
    }

protected:

    Vector3 m_pos_bas;      ///< [m] nozzle position expressed in BAS

    Table m_n1_throttle;    ///< low  pressure compressor rpm vs throttle position
    Table m_n2_throttle;    ///< high pressure compressor rpm vs throttle position

    Table m_tit_n2;         ///< turbine inlet temperature (TIT) vs high pressure compressor rpm

    Table2D m_tf_idle;      ///< [-] idle thrust factor
    Table2D m_tf_max;       ///< [-] maximum thrust factor

    Table m_tc_n1;
    Table m_tc_n2;

    Table m_tc_thrust;      ///< thrust time constant [s] vs difference of normalized engine power command and engine power [-]

    double m_thrust_max;    ///< [N] specific maximum military thrust

    double m_tc_tit;        ///< [s] turbine inlet temperature (TIT) time constant

    double m_tsfc;          ///< [kg/(N*s)] thrust specific fuel consumption

    double m_n1_idle;       ///< low  pressure compressor idle rpm
    double m_n2_idle;       ///< high pressure compressor idle rpm
    double m_n1_max;        ///< low  pressure compressor max rpm
    double m_n2_max;        ///< high pressure compressor max rpm

    double m_n1_setpoint;   ///< low  pressure compressor rpm setpoint
    double m_n2_setpoint;   ///< high pressure compressor rpm setpoint

    double m_tit_setpoint;  ///< turbine inlet temperature (TIT) setpoint

    double m_pow_command;   ///< <0.0;1.0> normalized engine power command
    double m_pow;           ///< <0.0;1.0> normalized engine power

    double m_thrust_tc_inv; ///< [1/s] inverse of thrust time constant
    double m_tit_tc_actual; ///< [s] actual turbine inlet temperature (TIT) time constant

    double m_temperature;   ///< [deg C] air temperature

    // output

    double m_n1;            ///< low  pressure compressor rpm
    double m_n2;            ///< high pressure compressor rpm
    double m_tit;           ///< [deg C] turbine inlet temperature (TIT)
    double m_fuelFlow;      ///< [kg/s] fuel flow
    double m_thrust;        ///< [N] thrust

    double getTimeConstant( double delta_n, double n_max, double tc );
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_TURBOFAN_H
