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
#ifndef AW101_AIRCRAFT_H
#define AW101_AIRCRAFT_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_Aircraft.h>

#include <fdm_aw101/aw101_Aerodynamics.h>
#include <fdm_aw101/aw101_Controls.h>
#include <fdm_aw101/aw101_LandingGear.h>
#include <fdm_aw101/aw101_Mass.h>
#include <fdm_aw101/aw101_Propulsion.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief AW101 aircraft class.
 */
class AW101_Aircraft : public Aircraft
{
public:

    /** Constructor. */
    AW101_Aircraft( Input *input );

    /** Destructor. */
    virtual ~AW101_Aircraft();

    /**
     * Initializes aircraft.
     * @param engineOn specifies if engine is running on startup
     */
    void initialize( bool engineOn = false );

    inline AW101_Aerodynamics* getAero() { return _aero; }
    inline AW101_Controls*     getCtrl() { return _ctrl; }
    inline AW101_LandingGear*  getGear() { return _gear; }
    inline AW101_Mass*         getMass() { return _mass; }
    inline AW101_Propulsion*   getProp() { return _prop; }

    inline const AW101_Aerodynamics* getAero() const { return _aero; }
    inline const AW101_Controls*     getCtrl() const { return _ctrl; }
    inline const AW101_LandingGear*  getGear() const { return _gear; }
    inline const AW101_Mass*         getMass() const { return _mass; }
    inline const AW101_Propulsion*   getProp() const { return _prop; }

private:

    AW101_Aerodynamics *_aero;  ///< aerodynamics model
    AW101_Controls     *_ctrl;  ///< controls model
    AW101_LandingGear  *_gear;  ///< landing gear model
    AW101_Mass         *_mass;  ///< mass and inertia model
    AW101_Propulsion   *_prop;  ///< propulsion model
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // AW101_AIRCRAFT_H
