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
#ifndef FDM_BLADE_H
#define FDM_BLADE_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_Defines.h>

#include <fdm/utils/fdm_Table.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Rotor blade model class.
 *
 * This model is based on blade element theory.
 *
 * @see Bramwell A.: Bramwells Helicopter Dynamics, 2001
 * @see Stepniewski W.: Rotary-Wing Aerodynamics. Volume I: Basic Theories of Rotor Aerodynamics, 1984
 * @see Johnson W.: Helicopter Theory, 1980
 */
class FDMEXPORT Blade
{
public:

    /** Constructor. */
    Blade();

    /** Destructor. */
    virtual ~Blade();

protected:

    double _radius;     ///< [m] rotor radius
    double _chord;      ///< [m] blade chord
    double _offset;     ///< [m] flapping hinge offset
    double _twist;      ///< [rad/m] blade twist angle
    double _mass;       ///< [kg] blage mass

    Table _cd;          ///< [-] blade section drag coefficient vs angle of attack
    Table _cl;          ///< [-] blade section lift coefficient vs angle of attack

    double _area;       ///< [m^2] rotor area
    double _span;       ///< [m] blade span
    double _inertia;    ///< [kg*m^2] blade inertia around flapping hinge
    double _moment;     ///< [kg*m] blade first mass moment

};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_BLADE_H
