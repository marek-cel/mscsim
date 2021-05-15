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
#ifndef C172_AIRCRAFT_H
#define C172_AIRCRAFT_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_Aircraft.h>

#include <fdm_c172/c172_Aerodynamics.h>
#include <fdm_c172/c172_Controls.h>
#include <fdm_c172/c172_LandingGear.h>
#include <fdm_c172/c172_Mass.h>
#include <fdm_c172/c172_Propulsion.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Cessna 172 aircraft class.
 *
 * @see Skyhawk Model 172S Specification & Description. Cessna Aircraft Company, 2012
 * @see Information Manual Skyhawk SP. Cessna Aircraft Company, 172SPHBUS-00, 2007
 */
class C172_Aircraft : public Aircraft
{
public:

    /** Constructor. */
    C172_Aircraft( Input *input );

    /** Destructor. */
    virtual ~C172_Aircraft();

    /**
     * Initializes aircraft.
     * @param engineOn specifies if engine is running on startup
     */
    void initialize( bool engineOn = false );

    inline C172_Aerodynamics* getAero() { return _aero; }
    inline C172_Controls*     getCtrl() { return _ctrl; }
    inline C172_LandingGear*  getGear() { return _gear; }
    inline C172_Mass*         getMass() { return _mass; }
    inline C172_Propulsion*   getProp() { return _prop; }

    inline const C172_Aerodynamics* getAero() const { return _aero; }
    inline const C172_Controls*     getCtrl() const { return _ctrl; }
    inline const C172_LandingGear*  getGear() const { return _gear; }
    inline const C172_Mass*         getMass() const { return _mass; }
    inline const C172_Propulsion*   getProp() const { return _prop; }

private:

    C172_Aerodynamics *_aero;   ///< aerodynamics model
    C172_Controls     *_ctrl;   ///< controls model
    C172_LandingGear  *_gear;   ///< landing gear model
    C172_Mass         *_mass;   ///< mass and inertia model
    C172_Propulsion   *_prop;   ///< propulsion model
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // C172_AIRCRAFT_H
