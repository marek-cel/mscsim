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
#ifndef UH60_AIRCRAFT_H
#define UH60_AIRCRAFT_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/main/fdm_Aircraft.h>

#include <fdm_uh60/uh60_Aerodynamics.h>
#include <fdm_uh60/uh60_Controls.h>
#include <fdm_uh60/uh60_LandingGear.h>
#include <fdm_uh60/uh60_Mass.h>
#include <fdm_uh60/uh60_Propulsion.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief UH-60 aircraft class.
 */
class UH60_Aircraft : public Aircraft
{
public:

    /** Constructor. */
    UH60_Aircraft( const DataInp *dataInp, DataOut *dataOut );

    /** Destructor. */
    ~UH60_Aircraft();

    /** Initializes aircraft. */
    void init( bool engineOn = false );

    /** Updates output data. */
    void updateOutputData();

    inline UH60_Aerodynamics* getAero() { return m_aero; }
    inline UH60_Controls*     getCtrl() { return m_ctrl; }
    inline UH60_LandingGear*  getGear() { return m_gear; }
    inline UH60_Mass*         getMass() { return m_mass; }
    inline UH60_Propulsion*   getProp() { return m_prop; }

    inline const UH60_Aerodynamics* getAero() const { return m_aero; }
    inline const UH60_Controls*     getCtrl() const { return m_ctrl; }
    inline const UH60_LandingGear*  getGear() const { return m_gear; }
    inline const UH60_Mass*         getMass() const { return m_mass; }
    inline const UH60_Propulsion*   getProp() const { return m_prop; }

private:

    UH60_Aerodynamics *m_aero;  ///< aerodynamics model
    UH60_Controls     *m_ctrl;  ///< controls model
    UH60_LandingGear  *m_gear;  ///< landing gear model
    UH60_Mass         *m_mass;  ///< mass and inertia model
    UH60_Propulsion   *m_prop;  ///< propulsion model
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // UH60_AIRCRAFT_H
