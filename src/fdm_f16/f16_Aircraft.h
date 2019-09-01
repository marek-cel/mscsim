/****************************************************************************//*
 * Copyright (C) 2019 Marek M. Cel
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
#ifndef F16_AIRCRAFT_H
#define F16_AIRCRAFT_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/main/fdm_Aircraft.h>

#include <fdm_f16/f16_Aerodynamics.h>
#include <fdm_f16/f16_Controls.h>
#include <fdm_f16/f16_LandingGear.h>
#include <fdm_f16/f16_Mass.h>
#include <fdm_f16/f16_Propulsion.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief F-16 aircraft class.
 */
class F16_Aircraft : public Aircraft
{
public:

    /** Constructor. */
    F16_Aircraft( const DataInp *dataInp, DataOut *dataOut );

    /** Destructor. */
    ~F16_Aircraft();

    /** Initializes aircraft. */
    void init( bool engineOn = false );

    /** Updates output data. */
    void updateOutputData();

    inline F16_Aerodynamics* getAero() { return _aero; }
    inline F16_Controls*     getCtrl() { return _ctrl; }
    inline F16_LandingGear*  getGear() { return _gear; }
    inline F16_Mass*         getMass() { return _mass; }
    inline F16_Propulsion*   getProp() { return _prop; }

    inline const F16_Aerodynamics* getAero() const { return _aero; }
    inline const F16_Controls*     getCtrl() const { return _ctrl; }
    inline const F16_LandingGear*  getGear() const { return _gear; }
    inline const F16_Mass*         getMass() const { return _mass; }
    inline const F16_Propulsion*   getProp() const { return _prop; }

private:

    F16_Aerodynamics *_aero;    ///< aerodynamics model
    F16_Controls     *_ctrl;    ///< controls model
    F16_LandingGear  *_gear;    ///< landing gear model
    F16_Mass         *_mass;    ///< mass and inertia model
    F16_Propulsion   *_prop;    ///< propulsion model
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // F16_AIRCRAFT_H
