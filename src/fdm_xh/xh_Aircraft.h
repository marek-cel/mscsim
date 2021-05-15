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
#ifndef XH_AIRCRAFT_H
#define XH_AIRCRAFT_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_Aircraft.h>

#include <fdm_xh/xh_Aerodynamics.h>
#include <fdm_xh/xh_Controls.h>
#include <fdm_xh/xh_LandingGear.h>
#include <fdm_xh/xh_Mass.h>
#include <fdm_xh/xh_Propulsion.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief X/H aircraft class.
 */
class XH_Aircraft : public Aircraft
{
public:

    /** Constructor. */
    XH_Aircraft( Input *input );

    /** Destructor. */
    virtual ~XH_Aircraft();

    /**
     * Initializes aircraft.
     * @param engineOn specifies if engine is running on startup
     */
    void initialize( bool engineOn = false );

    inline XH_Aerodynamics* getAero() { return _aero; }
    inline XH_Controls*     getCtrl() { return _ctrl; }
    inline XH_LandingGear*  getGear() { return _gear; }
    inline XH_Mass*         getMass() { return _mass; }
    inline XH_Propulsion*   getProp() { return _prop; }

    inline const XH_Aerodynamics* getAero() const { return _aero; }
    inline const XH_Controls*     getCtrl() const { return _ctrl; }
    inline const XH_LandingGear*  getGear() const { return _gear; }
    inline const XH_Mass*         getMass() const { return _mass; }
    inline const XH_Propulsion*   getProp() const { return _prop; }

private:

    friend class Aircraft::Integrator;

    XH_Aerodynamics *_aero;   ///< aerodynamics model
    XH_Controls     *_ctrl;   ///< controls model
    XH_LandingGear  *_gear;   ///< landing gear model
    XH_Mass         *_mass;   ///< mass and inertia model
    XH_Propulsion   *_prop;   ///< propulsion model

    /**
     * Computes state vector derivatives due to given state vector.
     * @param stateVect state vector
     * @param derivVect resulting state vector derivative
     */
    void computeStateDeriv( const StateVector &stateVect,
                            StateVector *derivVect );

    /**
     * Updates aircraft state variables.
     * @param stateVect state vector
     * @param derivVect state vector derivative
     */
    void updateVariables( const StateVector &stateVect,
                          const StateVector &derivVect );
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // XH_AIRCRAFT_H
