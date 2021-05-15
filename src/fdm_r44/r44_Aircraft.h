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
#ifndef R44_AIRCRAFT_H
#define R44_AIRCRAFT_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_Aircraft.h>

#include <fdm_r44/r44_Aerodynamics.h>
#include <fdm_r44/r44_Controls.h>
#include <fdm_r44/r44_LandingGear.h>
#include <fdm_r44/r44_Mass.h>
#include <fdm_r44/r44_Propulsion.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief R44 aircraft class.
 */
class R44_Aircraft : public Aircraft
{
public:

    /** Constructor. */
    R44_Aircraft( Input *input );

    /** Destructor. */
    virtual ~R44_Aircraft();

    /**
     * Initializes aircraft.
     * @param engineOn specifies if engine is running on startup
     */
    void initialize( bool engineOn = false );

    inline R44_Aerodynamics* getAero() { return _aero; }
    inline R44_Controls*     getCtrl() { return _ctrl; }
    inline R44_LandingGear*  getGear() { return _gear; }
    inline R44_Mass*         getMass() { return _mass; }
    inline R44_Propulsion*   getProp() { return _prop; }

    inline const R44_Aerodynamics* getAero() const { return _aero; }
    inline const R44_Controls*     getCtrl() const { return _ctrl; }
    inline const R44_LandingGear*  getGear() const { return _gear; }
    inline const R44_Mass*         getMass() const { return _mass; }
    inline const R44_Propulsion*   getProp() const { return _prop; }

private:

    R44_Aerodynamics *_aero;    ///< aerodynamics model
    R44_Controls     *_ctrl;    ///< controls model
    R44_LandingGear  *_gear;    ///< landing gear model
    R44_Mass         *_mass;    ///< mass and inertia model
    R44_Propulsion   *_prop;    ///< propulsion model
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // R44_AIRCRAFT_H
