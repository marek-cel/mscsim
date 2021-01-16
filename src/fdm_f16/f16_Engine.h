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
#ifndef F16_ENGINE_H
#define F16_ENGINE_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/utils/fdm_Table1.h>
#include <fdm/utils/fdm_Table2.h>
#include <fdm/utils/fdm_Vector3.h>

#include <fdm/xml/fdm_XmlNode.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief F-16 engine class.
 *
 * @see Nguyen L., et al.: Simulator Study of Stall/Post-Stall Characteristics of a Fighter Airplane With Relaxed Longitudinal Static Stability, NASA-TP-1538
 * @see Gilbert W., et al.: Simulator Study of the Effectiveness of an Automatic Control System Designed to Improve the High-Angle-of-Attack Characteristics of a Fighter Airplane, NASA-TN-D-8176
 *
 * XML configuration file format:
 * @code
 * <turbofan_ab>
 *   <position> { [m] x-coordinate } { [m] y-coordinate } { [m] z-coordinate } </position>
 *   <thrust_mil> { [N] maximum military thrust } </thrust_mil>
 *   <thrust_ab> { [N] maximum afterburner thrust } </thrust_ab>
 *   <ab_threshold> { afterburner throttle threshold } </ab_threshold>
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
 *   <thrust_factor_ab>
 *     { [kg/m^3] air density } ... { more values of air density }
 *     { [-] Mach number } { [-] thrust factor } ... { more values of thrust }
 *     ... { more entries }
 *   </thrust_factor_ab>
 * </turbofan_ab>
 * @endcode
 */
class F16_Engine
{
public:

    /** Engine state enum. */
    enum State
    {
        Stopped  = 0,   ///< engine stopped
        Starting = 1,   ///< engine starting
        Running  = 2    ///< engine running
    };

    /** Constructor. */
    F16_Engine();

    /** Destructor. */
    virtual ~F16_Engine();

    /**
     * Reads data.
     * @param dataNode XML node
     */
    void readData( XmlNode &dataNode );

    /** */
    void initialize( bool engineOn );

    /**
     * Computes thrust.
     * @param machNumber [-] Mach number
     * @param airDensity [kg/m^3] air density
     */
    void computeThrust( double machNumber, double airDensity );

    /**
     * Integrates model.
     * @param timeStep [s] time step
     */
    void integrate( double timeStep );

    /**
     * Updates engine.
     * @param throttle [0.0,1.0] throttle lever position
     * @param temperature [deg C] air temperature
     * @param machNumber [-] Mach number
     * @param airDensity [kg/m^3] air density
     * @param fuel specifies if fuel is provided
     * @param starter specifies if starter is enabled
     */
    void update( double throttle, double temperature,
                 double machNumber, double airDensity,
                 bool fuel, bool starter );

    /**
     * Returns engine state.
     * @return engine state
     */
    inline State getState() const
    {
        return _state;
    }

    /**
     * Returns propeller position expressed in BAS.
     * @return [m] propeller position expressed in BAS
     */
    inline const Vector3& getPos_BAS() const
    {
        return _pos_bas;
    }

    /**
     * Returns engine fuel consumption.
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
     * Returns engine thrust.
     * @return [N] propeller thrust
     */
    inline double getThrust() const
    {
        return _thrust;
    }

    /** */
    inline bool getAfterburner() const
    {
        return _afterburner;
    }

protected:

    State _state;           ///< engine state

    Vector3 _pos_bas;       ///< [m] nozzle position expressed in BAS

    Table1 _n1_throttle;    ///< low  pressure compressor rpm vs throttle position
    Table1 _n2_throttle;    ///< high pressure compressor rpm vs throttle position

    Table1 _tit_n2;         ///< turbine inlet temperature (TIT) vs high pressure compressor rpm

    Table2 _tf_idle;        ///< [-] idle thrust factor
    Table2 _tf_mil;         ///< [-] military thrust factor
    Table2 _tf_ab;          ///< [-] afterburner thrust factor

    Table1 _tc_n1;          ///<
    Table1 _tc_n2;          ///<

    Table1 _tc_thrust;      ///< thrust time constant [s] vs difference of normalized engine power command and engine power [-]

    double _thrust_mil;     ///< [N] specific maximum military thrust
    double _thrust_ab;      ///< [N] specific maximum afterburner thrust

    double _ab_threshold;   ///< [-] throttle afterburner threshold

    double _tc_tit;         ///< [s] turbine inlet temperature (TIT) time constant

    double _tsfc;           ///< [kg/(N*s)] thrust specific fuel consumption
    double _tsfc_ab;        ///< [kg/(N*s)] thrust specific fuel consumption (afterburner)

    double _n1_idle;        ///< low  pressure compressor idle rpm
    double _n2_idle;        ///< high pressure compressor idle rpm
    double _n1_ab;          ///< low  pressure compressor afterburner rpm
    double _n2_ab;          ///< high pressure compressor afterburner rpm
    double _n1_max;         ///< low  pressure compressor max rpm
    double _n2_max;         ///< high pressure compressor max rpm

    double _n1_setpoint;    ///< low  pressure compressor rpm setpoint
    double _n2_setpoint;    ///< high pressure compressor rpm setpoint

    double _tit_setpoint;   ///< turbine inlet temperature (TIT) setpoint

    double _pow_command;    ///< <0.0;1.0> normalized engine power command
    double _pow;            ///< <0.0;1.0> normalized engine power

    double _thrust_tc_inv;  ///< [1/s] inverse of thrust time constant
    double _tit_tc_actual;  ///< [s] actual turbine inlet temperature (TIT) time constant

    double _temperature;    ///< [deg C] air temperature

    // output

    double _n1;             ///< low  pressure compressor rpm
    double _n2;             ///< high pressure compressor rpm
    double _tit;            ///< [deg C] turbine inlet temperature (TIT)
    double _fuelFlow;       ///< [kg/s] fuel flow
    double _thrust;         ///< [N] thrust

    bool _afterburner;      ///< specifies if afterburner is engaged

    double getTimeConst( double delta_n, double n_max, double tc );
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // F16_ENGINE_H
