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
#ifndef XF_AIRCRAFT_H
#define XF_AIRCRAFT_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_Aircraft.h>

#include <fdm_xf/xf_Aerodynamics.h>
#include <fdm_xf/xf_Controls.h>
#include <fdm_xf/xf_LandingGear.h>
#include <fdm_xf/xf_Mass.h>
#include <fdm_xf/xf_Propulsion.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief X/F aircraft class.
 */
class XF_Aircraft : public Aircraft
{
public:

    /** Constructor. */
    XF_Aircraft( Input *input );

    /** Destructor. */
    virtual ~XF_Aircraft();

    /**
     * Initializes aircraft.
     * @param engineOn specifies if engine is running on startup
     */
    void initialize( bool engineOn = false );

    inline XF_Aerodynamics* getAero() { return _aero; }
    inline XF_Controls*     getCtrl() { return _ctrl; }
    inline XF_LandingGear*  getGear() { return _gear; }
    inline XF_Mass*         getMass() { return _mass; }
    inline XF_Propulsion*   getProp() { return _prop; }

    inline const XF_Aerodynamics* getAero() const { return _aero; }
    inline const XF_Controls*     getCtrl() const { return _ctrl; }
    inline const XF_LandingGear*  getGear() const { return _gear; }
    inline const XF_Mass*         getMass() const { return _mass; }
    inline const XF_Propulsion*   getProp() const { return _prop; }

private:

    XF_Aerodynamics *_aero;    ///< aerodynamics model
    XF_Controls     *_ctrl;    ///< controls model
    XF_LandingGear  *_gear;    ///< landing gear model
    XF_Mass         *_mass;    ///< mass and inertia model
    XF_Propulsion   *_prop;    ///< propulsion model
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // XF_AIRCRAFT_H
