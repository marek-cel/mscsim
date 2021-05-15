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
#ifndef C130_LANDINGGEAR_H
#define C130_LANDINGGEAR_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_LandingGear.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

class C130_Aircraft;    // aircraft class forward declaration

/**
 * @brief C-130 landing gear class.
 *
 * XML configuration file format:
 * @code
 * <landing_gear>
 *   <wheel [steerable="{ 0|1 }"] [caster="{ 0|1 }"] [brake_group="{ 0|1|2 }]">
 *     <attachment_point> { [m] x-coordinate } { [m] y-coordinate } { [m] z-coordinate } </attachment_point>
 *     <unloaded_wheel> { [m] x-coordinate } { [m] y-coordinate } { [m] z-coordinate } </unloaded_wheel>
 *     <stiffness> { [N/m] strut stiffness (linear spring) coefficient } </stiffness>
 *     <damping> { [N/(m/s)] strut damping coefficient  } </damping>
 *     <friction_static> { [-] static friction coefficient } </friction_static>
 *     <friction_kinetic> { [-] kinetic friction coefficient } </friction_kinetic>
 *     <friction_rolling> { [-] rolling friction coefficient } </friction_rolling>
 *     [<max_angle> { [rad] max steering angle } </max_angle>]
 *   </wheel>
 *   ... { more wheels }
 * </landing_gear>
 * @endcode
 */
class C130_LandingGear : public LandingGear
{
public:

    /** Constructor. */
    C130_LandingGear( const C130_Aircraft *aircraft, Input *input );

    /** Destructor. */
    virtual ~C130_LandingGear();

    /**
     * Reads data.
     * @param dataNode XML node
     */
    void readData( XmlNode &dataNode );

    /** Computes force and moment. */
    void computeForceAndMoment();

    /** Updates model. */
    void update();

private:

    const C130_Aircraft *_aircraft;     ///< aircraft model main object

    Wheels _wheels;                     ///< wheels container
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // C130_LANDINGGEAR_H
