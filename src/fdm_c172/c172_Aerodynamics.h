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
#ifndef C172_AERODYNAMICS_H
#define C172_AERODYNAMICS_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_Aerodynamics.h>

#include <fdm_c172/c172_TailOff.h>
#include <fdm_c172/c172_StabilizerHor.h>
#include <fdm_c172/c172_StabilizerVer.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

class C172_Aircraft;    // aircraft class forward declaration

/**
 * @brief Cessna 172 aerodynamics class.
 */
class C172_Aerodynamics : public Aerodynamics
{
public:

    /** Constructor. */
    C172_Aerodynamics( const C172_Aircraft *aircraft, Input *input );

    /** Destructor. */
    virtual ~C172_Aerodynamics();

    /**
     * Reads data.
     * @param dataNode XML node
     */
    void readData( XmlNode &dataNode );

    /** Computes force and moment. */
    void computeForceAndMoment();

    /** Updates aerodynamics. */
    void update();

    /**
     * Returns true if aircraft is stalling, otherwise returns false.
     * @return true if aircraft is stalling, false otherwise
     */
    inline bool getStall() const { return _tailOff->getStall(); }

private:

    const C172_Aircraft *_aircraft;     ///< aircraft model main object

    C172_TailOff       *_tailOff;       ///< wing model
    C172_StabilizerHor *_stabHor;       ///< horizontal stabilizer model
    C172_StabilizerVer *_stabVer;       ///< vertical stabilizer model

    Table1 _drag_ground_effect;         ///< [-] drag factor due to ground effect vs [m] altitude AGL
    Table1 _lift_ground_effect;         ///< [-] lift factor due to ground effect vs [m] altitude AGL

    double _dl_dtorque;                 ///< [-] rolling moment due propeller torque
    double _dn_dtorque;                 ///< [-] yawing moment due propeller torque
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // C172_AERODYNAMICS_H
