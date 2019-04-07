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
#ifndef FDM_TURBOJET_H
#define FDM_TURBOJET_H

////////////////////////////////////////////////////////////////////////////////

#include <fdmMain/fdm_Engine.h>

#include <fdmUtils/fdm_Table.h>
#include <fdmUtils/fdm_Table2D.h>
#include <fdmUtils/fdm_Vector3.h>

#include <fdmXml/fdm_XmlNode.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Twin-spool low by-pass ratio turbojet class.
 *
 * @see Nguyen L., et al.: Simulator Study of Stall/Post-Stall Characteristics of a Fighter Airplane With Relaxed Longitudinal Static Stability, NASA-TP-1538
 * @see Gilbert W., et al.: Simulator Study of the Effectiveness of an Automatic Control System Designed to Improve the High-Angle-of-Attack Characteristics of a Fighter Airplane, NASA-TN-D-8176
 *
 * <h5>XML configuration file format:</h5>
 * @code
 * <turbojet>
 *   <position> { [m] x-coordinate } { [m] y-coordinate } { [m] z-coordinate } </position>
 *   <thrust_mil> { [N] maximum military thrust } </thrust_mil>
 *   <thrust_max> { [N] maximum thrust (afterburner) } </thrust_max>
 *   <ab_threshold> { afterburner throttle threshold } </ab_threshold>
 *   <time_constant_n1> { N1 time constant } </time_constant_n1>
 *   <time_constant_n2> { N2 time constant } </time_constant_n2>
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
 *   <tsfc_ab> { [kg/(N*s)] thrust specific fuel consumption (afterburner) } </tsfc_ab>
 *   <thrust_factor_idle>
 *     { [kg/m^3] air density } ... { more values of air density }
 *     { [-] Mach number } { [-] thrust factor } ... { more values of thrust }
 *     ... { more entries }
 *   </thrust_factor_idle>
 *   <thrust_factor_mil>
 *     { [kg/m^3] air density } ... { more values of air density }
 *     { [-] Mach number } { [-] thrust factor } ... { more values of thrust }
 *     ... { more entries }
 *   </thrust_factor_mil>
 *   <thrust_factor_max>
 *     { [kg/m^3] air density } ... { more values of air density }
 *     { [-] Mach number } { [-] thrust factor } ... { more values of thrust }
 *     ... { more entries }
 *   </thrust_factor_max>
 * </turbojet>
 * @endcode
 */
class FDMEXPORT Turbojet : public Engine
{
public:

    /** Constructor. */
    Turbojet();

    /** Destructor. */
    virtual ~Turbojet();

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
     * @param airDensity [kg/m^3] air density
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
     * @param fuel specifies if fuel is provided
     * @param starter specifies if starter is enabled
     */
    virtual void update( double throttle, double temperature,
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

    /** */
    inline bool getAfterburner() const
    {
        return m_afterburner;
    }

protected:

    Vector3 m_pos_bas;      ///< [m] nozzle position expressed in BAS

    Table m_n1_throttle;    ///< low  pressure compressor rpm vs throttle position
    Table m_n2_throttle;    ///< high pressure compressor rpm vs throttle position

    Table m_tit_n2;         ///< turbine inlet temperature (TIT) vs high pressure compressor rpm

    Table2D m_tf_idle;      ///< [-] idle thrust factor
    Table2D m_tf_mil;       ///< [-] military thrust factor
    Table2D m_tf_max;       ///< [-] maximum thrust factor

    double m_thrust_mil;    ///< [N] specific military thrust
    double m_thrust_max;    ///< [N] specific maximum thrust

    double m_ab_threshold;  ///< [-] throttle afterburner threshold

    double m_n1_tc;         ///< [s] low  pressure compressor rpm time constant
    double m_n2_tc;         ///< [s] high pressure compressor rpm time constant

    double m_tit_tc;        ///< [s] turbine inlet temperature (TIT) time constant

    double m_tsfc;          ///< [kg/(N*s)] thrust specific fuel consumption
    double m_tsfc_ab;       ///< [kg/(N*s)] thrust specific fuel consumption (afterburner)

    double m_n1_idle;       ///< low  pressure compressor idle rpm
    double m_n2_idle;       ///< high pressure compressor idle rpm
    double m_n1_ab;         ///< low  pressure compressor afterburner rpm
    double m_n2_ab;         ///< high pressure compressor afterburner rpm
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

    bool m_afterburner;     ///< specifies if afterburner is engaged

    double getTimeConstant( double delta_n, double n_max, double tc );

    double getThrustTimeConstantInverse( double delta_pow );
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_TURBOJET_H
