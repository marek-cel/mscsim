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
#ifndef C172_AIRCRAFT_H
#define C172_AIRCRAFT_H

////////////////////////////////////////////////////////////////////////////////

#include <fdmMain/fdm_Aircraft.h>

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
    C172_Aircraft();

    /** Destructor. */
    ~C172_Aircraft();

    inline C172_Aerodynamics* getAero() { return m_aero; }
    inline C172_Controls*     getCtrl() { return m_ctrl; }
    inline C172_LandingGear*  getGear() { return m_gear; }
    inline C172_Mass*         getMass() { return m_mass; }
    inline C172_Propulsion*   getProp() { return m_prop; }

    inline const C172_Aerodynamics* getAero() const { return m_aero; }
    inline const C172_Controls*     getCtrl() const { return m_ctrl; }
    inline const C172_LandingGear*  getGear() const { return m_gear; }
    inline const C172_Mass*         getMass() const { return m_mass; }
    inline const C172_Propulsion*   getProp() const { return m_prop; }

private:

    C172_Aerodynamics *m_aero;  ///< aerodynamics model
    C172_Controls     *m_ctrl;  ///< controls model
    C172_LandingGear  *m_gear;  ///< landing gear model
    C172_Mass         *m_mass;  ///< mass and inertia model
    C172_Propulsion   *m_prop;  ///< propulsion model
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // C172_AIRCRAFT_H
