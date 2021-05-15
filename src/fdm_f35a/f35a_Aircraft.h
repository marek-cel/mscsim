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
#ifndef F35A_AIRCRAFT_H
#define F35A_AIRCRAFT_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_Aircraft.h>

#include <fdm_f35a/f35a_Aerodynamics.h>
#include <fdm_f35a/f35a_Controls.h>
#include <fdm_f35a/f35a_LandingGear.h>
#include <fdm_f35a/f35a_Mass.h>
#include <fdm_f35a/f35a_Propulsion.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief F-35A aircraft class.
 */
class F35A_Aircraft : public Aircraft
{
public:

    /** Constructor. */
    F35A_Aircraft( Input *input );

    /** Destructor. */
    virtual ~F35A_Aircraft();

    /**
     * Initializes aircraft.
     * @param engineOn specifies if engine is running on startup
     */
    void initialize( bool engineOn = false );

    inline F35A_Aerodynamics* getAero() { return _aero; }
    inline F35A_Controls*     getCtrl() { return _ctrl; }
    inline F35A_LandingGear*  getGear() { return _gear; }
    inline F35A_Mass*         getMass() { return _mass; }
    inline F35A_Propulsion*   getProp() { return _prop; }

    inline const F35A_Aerodynamics* getAero() const { return _aero; }
    inline const F35A_Controls*     getCtrl() const { return _ctrl; }
    inline const F35A_LandingGear*  getGear() const { return _gear; }
    inline const F35A_Mass*         getMass() const { return _mass; }
    inline const F35A_Propulsion*   getProp() const { return _prop; }

private:

    F35A_Aerodynamics *_aero;  ///< aerodynamics model
    F35A_Controls     *_ctrl;  ///< controls model
    F35A_LandingGear  *_gear;  ///< landing gear model
    F35A_Mass         *_mass;  ///< mass and inertia model
    F35A_Propulsion   *_prop;  ///< propulsion model
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // F35A_AIRCRAFT_H
