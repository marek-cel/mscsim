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
#ifndef F35A_AERODYNAMICS_H
#define F35A_AERODYNAMICS_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_Aerodynamics.h>

#include <fdm_f35a/f35a_TailOff.h>
#include <fdm_f35a/f35a_StabilizerHor.h>
#include <fdm_f35a/f35a_StabilizerVer.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

class F35A_Aircraft;    // aircraft class forward declaration

/**
 * @brief F-35A aerodynamics class.
 */
class F35A_Aerodynamics : public Aerodynamics
{
public:

    /** Constructor. */
    F35A_Aerodynamics( const F35A_Aircraft *aircraft, Input *input );

    /** Destructor. */
    virtual ~F35A_Aerodynamics();

    /**
     * Reads data.
     * @param dataNode XML node
     */
    void readData( XmlNode &dataNode );

    /** Computes force and moment. */
    void computeForceAndMoment();

    /** Updates model. */
    void update();

    /**
     * Returns true if aircraft is stalling, otherwise returns false.
     * @return true if aircraft is stalling, false otherwise
     */
    inline bool getStall() const { return _tailOff->getStall(); }

private:

    const F35A_Aircraft *_aircraft; ///< aircraft model main object

    F35A_TailOff       *_tailOff;       ///< wing model
    F35A_StabilizerHor *_stabHor;       ///< horizontal stabilizer model
    F35A_StabilizerVer *_stabVer;       ///< vertical stabilizer model

    Table1 _drag_ground_effect;         ///< [-] drag factor due to ground effect vs [m] altitude AGL
    Table1 _lift_ground_effect;         ///< [-] lift factor due to ground effect vs [m] altitude AGL

    Table1 _wave_drag;              ///< wave drag coefficient
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // F35A_AERODYNAMICS_H
