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
#ifndef UH60_AIRCRAFT_H
#define UH60_AIRCRAFT_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_Aircraft.h>

#include <fdm_uh60/uh60_Aerodynamics.h>
#include <fdm_uh60/uh60_Controls.h>
#include <fdm_uh60/uh60_LandingGear.h>
#include <fdm_uh60/uh60_Mass.h>
#include <fdm_uh60/uh60_Propulsion.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief UH-60 aircraft class.
 *
 * @see Operator's Manual for UH-60A, UH-60L and EH-60A Helicopters. Department of the Army, TM 1-1520-237-10, 1996
 * @see Aviation Unit and Intermediate Maintenance for Army Models UH-60 Helicopters, Department of the Army, TM 1-1520-237-23-1, 2006
 * @see Howlett J.: UH-60A Black Hawk Engineering Simulation Program. NASA, CR-166309, 1981
 * @see Hilbert K.: A Mathematical Model of the UH-60 Helicopter. NASA, TM-85890, 1984
 */
class UH60_Aircraft : public Aircraft
{
public:

    /** Constructor. */
    UH60_Aircraft( Input *input );

    /** Destructor. */
    virtual ~UH60_Aircraft();

    /**
     * Initializes aircraft.
     * @param engineOn specifies if engine is running on startup
     */
    void initialize( bool engineOn = false );

    inline UH60_Aerodynamics* getAero() { return _aero; }
    inline UH60_Controls*     getCtrl() { return _ctrl; }
    inline UH60_LandingGear*  getGear() { return _gear; }
    inline UH60_Mass*         getMass() { return _mass; }
    inline UH60_Propulsion*   getProp() { return _prop; }

    inline const UH60_Aerodynamics* getAero() const { return _aero; }
    inline const UH60_Controls*     getCtrl() const { return _ctrl; }
    inline const UH60_LandingGear*  getGear() const { return _gear; }
    inline const UH60_Mass*         getMass() const { return _mass; }
    inline const UH60_Propulsion*   getProp() const { return _prop; }

private:

    UH60_Aerodynamics *_aero;   ///< aerodynamics model
    UH60_Controls     *_ctrl;   ///< controls model
    UH60_LandingGear  *_gear;   ///< landing gear model
    UH60_Mass         *_mass;   ///< mass and inertia model
    UH60_Propulsion   *_prop;   ///< propulsion model
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // UH60_AIRCRAFT_H
