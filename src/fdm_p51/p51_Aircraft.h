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
#ifndef P51_AIRCRAFT_H
#define P51_AIRCRAFT_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_Aircraft.h>

#include <fdm_p51/p51_Aerodynamics.h>
#include <fdm_p51/p51_Controls.h>
#include <fdm_p51/p51_LandingGear.h>
#include <fdm_p51/p51_Mass.h>
#include <fdm_p51/p51_Propulsion.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief P-51 aircraft class.
 *
 * @see Erection and Maintenance Instructions for Army Models P-51 (...). North American Aviation, AN 01-60JE-2, 1944
 */
class P51_Aircraft : public Aircraft
{
public:

    /** Constructor. */
    P51_Aircraft( Input *input );

    /** Destructor. */
    virtual ~P51_Aircraft();

    /**
     * Initializes aircraft.
     * @param engineOn specifies if engine is running on startup
     */
    void initialize( bool engineOn = false );

    inline P51_Aerodynamics* getAero() { return _aero; }
    inline P51_Controls*     getCtrl() { return _ctrl; }
    inline P51_LandingGear*  getGear() { return _gear; }
    inline P51_Mass*         getMass() { return _mass; }
    inline P51_Propulsion*   getProp() { return _prop; }

    inline const P51_Aerodynamics* getAero() const { return _aero; }
    inline const P51_Controls*     getCtrl() const { return _ctrl; }
    inline const P51_LandingGear*  getGear() const { return _gear; }
    inline const P51_Mass*         getMass() const { return _mass; }
    inline const P51_Propulsion*   getProp() const { return _prop; }

private:

    P51_Aerodynamics *_aero;    ///< aerodynamics model
    P51_Controls     *_ctrl;    ///< controls model
    P51_LandingGear  *_gear;    ///< landing gear model
    P51_Mass         *_mass;    ///< mass and inertia model
    P51_Propulsion   *_prop;    ///< propulsion model
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // P51_AIRCRAFT_H
