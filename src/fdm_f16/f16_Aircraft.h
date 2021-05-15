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
#ifndef F16_AIRCRAFT_H
#define F16_AIRCRAFT_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_Aircraft.h>

#include <fdm_f16/f16_Aerodynamics.h>
#include <fdm_f16/f16_Controls.h>
#include <fdm_f16/f16_LandingGear.h>
#include <fdm_f16/f16_Mass.h>
#include <fdm_f16/f16_Propulsion.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief F-16 aircraft class.
 *
 * @see Flight Manual HAF Series Aircraft F-16C/D Blocks 50 and 52+. Lockheed Martin Corporation, T.O. GR1F-16CJ-1, 2003
 * @see Suplemental Flight Manual HAF Series Aircraft F-16C/D Blocks 50 and 52+. Lockheed Martin Corporation, T.O. GR1F-16CJ-1-1, 2003
 * @see Nguyen L., et al.: Simulator Study of Stall/Post-Stall Characteristics of a Fighter Airplane With Relaxed Longitudinal Static Stability, NASA-TP-1538, 1979
 * @see Gilbert W., et al.: Simulator Study of the Effectiveness of an Automatic Control System Designed to Improve the High-Angle-of-Attack Characteristics of a Fighter Airplane, NASA-TN-D-8176, 1976
 * @see Marchand M.: Pitch Rate Flight Control for the F-16 Aircraft to improve Air-to-Air Combat, AD-A055-417, 1977
 * @see Droste T., Walker J.: The General Dynamics Case Study on the F-16 Fly-by-Wire Flight Control System
 * @see Dameron G.: A Real-Time Simulator for Man-In-The-Loop Testing of Aircraft Control Systems, AD-A202-599, 1988
 */
class F16_Aircraft : public Aircraft
{
public:

    /** Constructor. */
    F16_Aircraft( Input *input );

    /** Destructor. */
    virtual ~F16_Aircraft();

    /**
     * Initializes aircraft.
     * @param engineOn specifies if engine is running on startup
     */
    void initialize( bool engineOn = false );

    inline F16_Aerodynamics* getAero() { return _aero; }
    inline F16_Controls*     getCtrl() { return _ctrl; }
    inline F16_LandingGear*  getGear() { return _gear; }
    inline F16_Mass*         getMass() { return _mass; }
    inline F16_Propulsion*   getProp() { return _prop; }

    inline const F16_Aerodynamics* getAero() const { return _aero; }
    inline const F16_Controls*     getCtrl() const { return _ctrl; }
    inline const F16_LandingGear*  getGear() const { return _gear; }
    inline const F16_Mass*         getMass() const { return _mass; }
    inline const F16_Propulsion*   getProp() const { return _prop; }

private:

    F16_Aerodynamics *_aero;    ///< aerodynamics model
    F16_Controls     *_ctrl;    ///< controls model
    F16_LandingGear  *_gear;    ///< landing gear model
    F16_Mass         *_mass;    ///< mass and inertia model
    F16_Propulsion   *_prop;    ///< propulsion model
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // F16_AIRCRAFT_H
