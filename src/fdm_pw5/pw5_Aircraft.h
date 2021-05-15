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
#ifndef PW5_AIRCRAFT_H
#define PW5_AIRCRAFT_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_Aircraft.h>

#include <fdm_pw5/pw5_Aerodynamics.h>
#include <fdm_pw5/pw5_Controls.h>
#include <fdm_pw5/pw5_LandingGear.h>
#include <fdm_pw5/pw5_Mass.h>
#include <fdm_pw5/pw5_Propulsion.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief PW-5 aircraft class.
 *
 * @see B1-PW-5 Instrukcja Uzytkowania w Locie, B1-PW-5/IWL/I/2001, 2001
 * @see B1-PW-5 Instrukcja Obslugi Technicznej i Napraw, B1-PW-5/IOT/I/2001, 2001
 * @see PW-5 Sailplane Flight Manual, PW-5/IWL/I/97, 1997
 * @see Skrzydla w miniaturze nr 17: PW-5 Smyk, BM-2, Tipsy Junior
 */
class PW5_Aircraft : public Aircraft
{
public:

    /** Constructor. */
    PW5_Aircraft( Input *input );

    /** Destructor. */
    virtual ~PW5_Aircraft();

    /**
     * Initializes aircraft.
     * @param engineOn specifies if engine is running on startup
     */
    void initialize( bool engineOn = false );

    inline PW5_Aerodynamics* getAero() { return _aero; }
    inline PW5_Controls*     getCtrl() { return _ctrl; }
    inline PW5_LandingGear*  getGear() { return _gear; }
    inline PW5_Mass*         getMass() { return _mass; }
    inline PW5_Propulsion*   getProp() { return _prop; }

    inline const PW5_Aerodynamics* getAero() const { return _aero; }
    inline const PW5_Controls*     getCtrl() const { return _ctrl; }
    inline const PW5_LandingGear*  getGear() const { return _gear; }
    inline const PW5_Mass*         getMass() const { return _mass; }
    inline const PW5_Propulsion*   getProp() const { return _prop; }

private:

    PW5_Aerodynamics *_aero;    ///< aerodynamics model
    PW5_Controls     *_ctrl;    ///< controls model
    PW5_LandingGear  *_gear;    ///< landing gear model
    PW5_Mass         *_mass;    ///< mass and inertia model
    PW5_Propulsion   *_prop;    ///< propulsion model
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // PW5_AIRCRAFT_H
