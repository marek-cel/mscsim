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
#ifndef UH602_AIRCRAFT_H
#define UH602_AIRCRAFT_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/main/fdm_Aircraft.h>

#include <fdm_uh602/uh602_Aerodynamics.h>
#include <fdm_uh602/uh602_Controls.h>
#include <fdm_uh602/uh602_LandingGear.h>
#include <fdm_uh602/uh602_Mass.h>
#include <fdm_uh602/uh602_Propulsion.h>

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
class UH602_Aircraft : public Aircraft
{
public:

    /** Constructor. */
    UH602_Aircraft( const DataInp *dataInp, DataOut *dataOut );

    /** Destructor. */
    ~UH602_Aircraft();

    /** Initializes aircraft. */
    void init( bool engineOn = false );

    /** Updates output data. */
    void updateOutputData();

    inline UH602_Aerodynamics* getAero() { return _aero; }
    inline UH602_Controls*     getCtrl() { return _ctrl; }
    inline UH602_LandingGear*  getGear() { return _gear; }
    inline UH602_Mass*         getMass() { return _mass; }
    inline UH602_Propulsion*   getProp() { return _prop; }

    inline const UH602_Aerodynamics* getAero() const { return _aero; }
    inline const UH602_Controls*     getCtrl() const { return _ctrl; }
    inline const UH602_LandingGear*  getGear() const { return _gear; }
    inline const UH602_Mass*         getMass() const { return _mass; }
    inline const UH602_Propulsion*   getProp() const { return _prop; }

private:

    UH602_Aerodynamics *_aero;   ///< aerodynamics model
    UH602_Controls     *_ctrl;   ///< controls model
    UH602_LandingGear  *_gear;   ///< landing gear model
    UH602_Mass         *_mass;   ///< mass and inertia model
    UH602_Propulsion   *_prop;   ///< propulsion model
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // UH602_AIRCRAFT_H
