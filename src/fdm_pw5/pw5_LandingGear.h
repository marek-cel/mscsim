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
#ifndef PW5_LANDINGGEAR_H
#define PW5_LANDINGGEAR_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_LandingGear.h>

#include <fdm/models/fdm_WingRunner.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

class PW5_Aircraft;    // aircraft class forward declaration

/**
 * @brief PW-5 landing gear class.
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
 *   <wing_runner>
 *     <wing> { [m] x-coordinate } { [m] y-coordinate } { [m] z-coordinate } </wing>
 *     <feet> { [m] x-coordinate } { [m] y-coordinate } { [m] z-coordinate } </feet>
 *     <stiffness> { [N/m] stiffness (linear spring) coefficient } </stiffness>
 *     <damping> { [N/(m/s)] damping coefficient  } </damping>
 *   </wing_runner>
 *   ... { more wing runners }
 * </landing_gear>
 * @endcode
 */
class PW5_LandingGear : public LandingGear
{
public:

    typedef std::vector< WingRunner > WingRunners;

    /** Constructor. */
    PW5_LandingGear( const PW5_Aircraft *aircraft, Input *input );

    /** Destructor. */
    virtual ~PW5_LandingGear();

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

    const PW5_Aircraft *_aircraft;      ///< aircraft model main object

    Wheels _wheels;                     ///< wheels container
    WingRunners _runners;               ///< wing runners conatiner
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // PW5_LANDINGGEAR_H
