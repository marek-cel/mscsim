/****************************************************************************//*
 * Copyright (C) 2020 Marek M. Cel
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
#ifndef TEST_AIRCRAFT_H
#define TEST_AIRCRAFT_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/main/fdm_Aircraft.h>

#include <fdm_test/test_Aerodynamics.h>
#include <fdm_test/test_Controls.h>
#include <fdm_test/test_LandingGear.h>
#include <fdm_test/test_Mass.h>
#include <fdm_test/test_Propulsion.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/** */
class TEST_Aircraft : public Aircraft
{
public:

    /** Constructor. */
    TEST_Aircraft( const DataInp *dataInp, DataOut *dataOut );

    /** Destructor. */
    ~TEST_Aircraft();

    /** Initializes aircraft. */
    void init( bool engineOn = false );

    /** Updates output data. */
    void updateOutputData();

    inline TEST_Aerodynamics* getAero() { return _aero; }
    inline TEST_Controls*     getCtrl() { return _ctrl; }
    inline TEST_LandingGear*  getGear() { return _gear; }
    inline TEST_Mass*         getMass() { return _mass; }
    inline TEST_Propulsion*   getProp() { return _prop; }

    inline const TEST_Aerodynamics* getAero() const { return _aero; }
    inline const TEST_Controls*     getCtrl() const { return _ctrl; }
    inline const TEST_LandingGear*  getGear() const { return _gear; }
    inline const TEST_Mass*         getMass() const { return _mass; }
    inline const TEST_Propulsion*   getProp() const { return _prop; }

private:

    TEST_Aerodynamics *_aero;   ///< aerodynamics model
    TEST_Controls     *_ctrl;   ///< controls model
    TEST_LandingGear  *_gear;   ///< landing gear model
    TEST_Mass         *_mass;   ///< mass and inertia model
    TEST_Propulsion   *_prop;   ///< propulsion model
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // TEST_AIRCRAFT_H
