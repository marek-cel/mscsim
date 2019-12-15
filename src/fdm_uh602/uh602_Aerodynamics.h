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
#ifndef UH602_AERODYNAMICS_H
#define UH602_AERODYNAMICS_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/main/fdm_Aerodynamics.h>


#include <fdm/models/fdm_Stabilizer.h>

#include <fdm_uh602/uh602_MainRotor.h>
#include <fdm_uh602/uh602_TailRotor.h>
#include <fdm_uh602/uh602_Fuselage.h>
#include <fdm_uh602/uh602_StabilizerHor.h>
#include <fdm_uh602/uh602_StabilizerVer.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

class UH602_Aircraft;    ///< aircraft class forward declaration

/**
 * @brief UH-60 aerodynamics class.
 *
 * @see Howlett J.: UH-60A Black Hawk Engineering Simulation Program. NASA, CR-166309, 1981
 * @see Hilbert K.: A Mathematical Model of the UH-60 Helicopter. NASA, TM-85890, 1984
 */
class UH602_Aerodynamics : public Aerodynamics
{
public:

    /** Constructor. */
    UH602_Aerodynamics( const UH602_Aircraft *aircraft );

    /** Destructor. */
    ~UH602_Aerodynamics();

    /** Initializes aerodynamics. */
    void init();

    /**
     * Reads data.
     * @param dataNode XML node
     */
    void readData( XmlNode &dataNode );

    /** Computes force and moment. */
    void computeForceAndMoment();

    /** Updates model. */
    void update();

    inline const MainRotor2* getMainRotor() const { return _mainRotor; }

private:

    const UH602_Aircraft *_aircraft;     ///< aircraft model main object

    UH602_MainRotor     *_mainRotor;     ///<
    UH602_TailRotor     *_tailRotor;     ///<
    UH602_Fuselage      *_fuselage;      ///<
    UH602_StabilizerHor *_stabHor;       ///<
    UH602_StabilizerVer *_stabVer;       ///<
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // UH602_AERODYNAMICS_H
