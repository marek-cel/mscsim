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
#ifndef FDM_INERTIA_H
#define FDM_INERTIA_H

////////////////////////////////////////////////////////////////////////////////

#include <fdmMain/fdm_Defines.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Inertia class.
 */
class FDMEXPORT Inertia
{
public:

    static double update( double timeStep, double currentValue,
                          double setpoint, double timeConstant );

    Inertia();

    Inertia( double timeConstant, double currentValue = 0.0 );

    inline double getCurrentValue() const { return m_currentValue; }
    inline double getTimeConstant() const { return m_timeConstant; }

    void setCurrentValue( double currentValue );
    void setTimeConstant( double timeConstant );

    void update( double timeStep, double setpoint );

private:

    double m_timeConstant;  ///< time constant
    double m_currentValue;  ///< current value

};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_INERTIA_H
