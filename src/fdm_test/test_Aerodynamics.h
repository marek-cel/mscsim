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
#ifndef TEST_AERODYNAMICS_H
#define TEST_AERODYNAMICS_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/main/fdm_Aerodynamics.h>


#include <fdm/models/fdm_Stabilizer.h>

#include <fdm_test/test_MainRotorAD.h>
#include <fdm_test/test_MainRotorBE.h>
#include <fdm_test/test_TailRotor.h>
#include <fdm_test/test_Fuselage.h>
#include <fdm_test/test_StabilizerHor.h>
#include <fdm_test/test_StabilizerVer.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

class TEST_Aircraft;    ///< aircraft class forward declaration

/** */
class TEST_Aerodynamics : public Aerodynamics
{
public:

    /** Constructor. */
    TEST_Aerodynamics( const TEST_Aircraft *aircraft );

    /** Destructor. */
    ~TEST_Aerodynamics();

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

    inline const MainRotor* getMainRotor() const { return _mainRotor; }
    inline const TEST_MainRotorAD* getMainRotorAD() const { return _mainRotorAD; }
    inline const TEST_MainRotorBE* getMainRotorBE() const { return _mainRotorBE; }

private:

    const TEST_Aircraft *_aircraft;     ///< aircraft model main object

    MainRotor          *_mainRotor;
    TEST_MainRotorAD   *_mainRotorAD;   ///<
    TEST_MainRotorBE   *_mainRotorBE;   ///<

    TEST_TailRotor     *_tailRotor;     ///<
    TEST_Fuselage      *_fuselage;      ///<
    TEST_StabilizerHor *_stabHor;       ///<
    TEST_StabilizerVer *_stabVer;       ///<
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // TEST_AERODYNAMICS_H
