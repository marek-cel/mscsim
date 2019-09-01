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
#ifndef FDM_PID_H
#define FDM_PID_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_Defines.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Proportional-Integral-Derivative controller with anti-windup filter class.
 *
 * Transfer function:
 * G(s)  =  Kp + Ki*(1/s) + Kd*s
 *
 * @see Duzinkiewicz K., et al.: Zadania do cwiczen laboratoryjnych T10: Sterowanie predkoscia obrotowa silnika pradu stalego, 2016. [in Polish]
 */
class FDMEXPORT PID
{
public:

    /** Constructor. */
    PID( double kp, double ki, double kd );

    /** Constructor. */
    PID( double kp, double ki, double kd, double min, double max );

    /** Destructor. */
    virtual ~PID();

    /** Updates controller. */
    void update( double timeStep, double error );

    /** Returns controller output. */
    inline double getValue() const { return _value; }

    inline void setKp( double kp ) { _kp = kp; }
    inline void setKi( double ki ) { _ki = ki; }
    inline void setKd( double kd ) { _kd = kd; }

    inline void setMin( double min ) { _min = min; }
    inline void setMax( double max ) { _max = max; }

    inline void setSaturation( bool saturation ) { _saturation = saturation; }

    /** Sets controller output (resets integer). */
    void setValue( double value );

protected:

    double _kp;         ///< proportional gain
    double _ki;         ///< integral gain
    double _kd;         ///< derivative gain

    double _min;        ///< minimum output value
    double _max;        ///< maximum output value

    double _error;      ///< error
    double _error_i;    ///< error integral sum
    double _error_d;    ///< error derivative

    double _value;      ///< output value
    double _delta;      ///< difference between raw and saturated output values

    bool _saturation;   ///< saturation
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_PID_H
