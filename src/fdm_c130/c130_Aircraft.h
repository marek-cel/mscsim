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
#ifndef C130_AIRCRAFT_H
#define C130_AIRCRAFT_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_Aircraft.h>

#include <fdm_c130/c130_Aerodynamics.h>
#include <fdm_c130/c130_Controls.h>
#include <fdm_c130/c130_LandingGear.h>
#include <fdm_c130/c130_Mass.h>
#include <fdm_c130/c130_Propulsion.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief C-130 aircraft class.
 *
 * @see NATOPS Flight Manual Navy Model C-130T Aircraft. Department of the Navy, 01‑75GAL‑1, 2006
 * @see Flight Manual C-130 USCG Series Aircraft. Lockheed Martin, T.O. 1C-130H-1, 2001
 */
class C130_Aircraft : public Aircraft
{
public:

    /** Constructor. */
    C130_Aircraft( Input *input );

    /** Destructor. */
    virtual ~C130_Aircraft();

    /**
     * Initializes aircraft.
     * @param engineOn specifies if engine is running on startup
     */
    void initialize( bool engineOn = false );

    inline C130_Aerodynamics* getAero() { return _aero; }
    inline C130_Controls*     getCtrl() { return _ctrl; }
    inline C130_LandingGear*  getGear() { return _gear; }
    inline C130_Mass*         getMass() { return _mass; }
    inline C130_Propulsion*   getProp() { return _prop; }

    inline const C130_Aerodynamics* getAero() const { return _aero; }
    inline const C130_Controls*     getCtrl() const { return _ctrl; }
    inline const C130_LandingGear*  getGear() const { return _gear; }
    inline const C130_Mass*         getMass() const { return _mass; }
    inline const C130_Propulsion*   getProp() const { return _prop; }

private:

    C130_Aerodynamics *_aero;   ///< aerodynamics model
    C130_Controls     *_ctrl;   ///< controls model
    C130_LandingGear  *_gear;   ///< landing gear model
    C130_Mass         *_mass;   ///< mass and inertia model
    C130_Propulsion   *_prop;   ///< propulsion model
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // C130_AIRCRAFT_H
