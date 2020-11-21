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
#ifndef FDM_PID_H
#define FDM_PID_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_Defines.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Proportional-Integral-Derivative controller.
 *
 * Transfer function (parallel):
 * G(s)  =  kp + ki*( 1/s ) + kd*s
 *
 * Transfer function (serial):
 * G(s)  =  k*( 1 + 1/( s*tau_i ) )*( 1 + s*tau_d )
 *
 * Transfer function (standard/ideal):
 * G(s)  =  Kp*( 1 + 1/( s*Ti ) + s*Td )
 *
 * @see Skup Z.: Podstawy automatyki i sterowania, 2012, p.118. [in Polish]
 * @see Kaczorek T.: Teoria ukladow regulacji automatycznej, 1970, p.280. [in Polish]
 * @see McCormack A., Godfrey K.: Rule-Based Autotuning Based on Frequency Domain Identification, 1998
 * @see Duzinkiewicz K., et al.: Zadania do cwiczen laboratoryjnych T10: Sterowanie predkoscia obrotowa silnika pradu stalego, 2016. [in Polish]
 * @see Brdys M., et al.: Silnik pradu stalego (NI Elvis 2) - Dobieranie nastaw regulatorow P, PI, PI. Filtr przeciwnasyceniowy Anti-windup, 2010. [in Polish]
 * @see Anirban G., Vinod J.: Anti-windup Schemes for Proportional Integral and Proportional Resonant Controller, 2010
 * @see https://en.wikipedia.org/wiki/PID_controller
 * @see https://en.wikipedia.org/wiki/Integral_windup
 * @see https://en.wikipedia.org/wiki/Ziegler%E2%80%93Nichols_method
 * @see https://www.scilab.org/pid-anti-windup-schemes
 */
class FDMEXPORT PID
{
public:

    /** Anti-Windup methods. */
    enum AntiWindup
    {
        None = 0,       ///< anti-windup innactive
        Calculation,    ///< back calculation of integral term
        Conditional,    ///< conditional integration
        Filtering       ///< feedback filtering
    };

    /**
     * Constructor. Disables saturation.
     * @param kp proportional gain
     * @param ki integral gain
     * @param kd derivative gain
     */
    PID( double kp, double ki, double kd );

    /**
     * Constructor. Enables saturation.
     * @param kp proportional gain
     * @param ki integral gain
     * @param kd derivative gain
     * @param min minimal value for saturation
     * @param max maximal value for saturation
     */
    PID( double kp, double ki, double kd, double min, double max );

    /** Destructor. */
    virtual ~PID();

    /** Updates controller. */
    virtual void update( double timeStep, double error );

    /** Resets controller. */
    virtual void reset();

    /** Returns controller output. */
    inline double getValue() const { return _value; }

    inline double getKp() const { return _kp; }
    inline double getKi() const { return _ki; }
    inline double getKd() const { return _kd; }

    inline double getKaw() const { return _kaw; }

    inline double getMin() const { return _min; }
    inline double getMax() const { return _max; }

    /**
     * Sets parameters of parallel form.
     * @param kp
     * @param ki
     * @param kd
     */
    virtual void setParallel( double kp, double ki, double kd );

    /**
     * Sets parameters of serial form.
     * @param k
     * @param tau_i
     * @param tau_d
     */
    virtual void setSerial( double k, double tau_i, double tau_d );

    /**
     * Sets parameters of standard (ideal) form.
     * @param Kp
     * @param Ti
     * @param Td
     */
    virtual void setStandard( double Kp, double Ti, double Td );

    /**
     * Sets controller current error.
     * @param error current error value
     */
    virtual void setError( double error );

    /**
     * Sets controller output (resets error integral sum).
     * @param value output value
     */
    virtual void setValue( double value );

    virtual void setValue( double timeStep, double error, double value );

    inline void setKp( double kp ) { _kp = kp; }
    inline void setKi( double ki ) { _ki = ki; }
    inline void setKd( double kd ) { _kd = kd; }

    inline void setKaw( double kaw ) { _kaw = kaw; }

    inline void setMin( double min ) { _min = min; }
    inline void setMax( double max ) { _max = max; }

    inline void setAntiWindup( AntiWindup antiWindup ) { _antiWindup = antiWindup; }

    inline void setSaturation( bool saturation ) { _saturation  = saturation; }

protected:

    AntiWindup _antiWindup; ///< anti-windup method

    double _kp;             ///< proportional gain
    double _ki;             ///< integral gain
    double _kd;             ///< derivative gain

    double _kaw;            ///< anti-windup gain

    double _min;            ///< minimum output value
    double _max;            ///< maximum output value

    double _error;          ///< error
    double _error_i;        ///< error integral sum
    double _error_d;        ///< error derivative

    double _value;          ///< output value
    double _delta;          ///< difference between raw and saturated output values

    bool _saturation;       ///< specifies if saturation is enabled
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_PID_H
