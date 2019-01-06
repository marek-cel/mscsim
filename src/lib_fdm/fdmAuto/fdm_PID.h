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

#include <fdmMain/fdm_Defines.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Proportional-Integral-Derivative controller with anti-windup filter class.
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
    inline double getValue() const { return m_value; }

    inline void setKp( double kp ) { m_kp = kp; }
    inline void setKi( double ki ) { m_ki = ki; }
    inline void setKd( double kd ) { m_kd = kd; }

    inline void setMin( double min ) { m_min = min; }
    inline void setMax( double max ) { m_max = max; }

    inline void setSaturation( bool saturation ) { m_saturation = saturation; }

    /** Sets controller output (resets integer). */
    void setValue( double value );

private:

    double m_kp;        ///< proportional gain
    double m_ki;        ///< integral gain
    double m_kd;        ///< derivative gain

    double m_min;       ///< minimum output value
    double m_max;       ///< maximum output value

    double m_error;     ///< error
    double m_error_i;   ///< error integral sum
    double m_error_d;   ///< error derivative

    double m_value;     ///< output value
    double m_delta;     ///< difference between raw and saturated output values

    bool m_saturation;  ///< saturation
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_PID_H
