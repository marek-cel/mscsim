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
#ifndef R44_AERODYNAMICS_H
#define R44_AERODYNAMICS_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_Aerodynamics.h>

#include <fdm_r44/r44_MainRotor.h>
#include <fdm_r44/r44_TailRotor.h>
#include <fdm_r44/r44_Fuselage.h>
#include <fdm_r44/r44_StabilizerHor.h>
#include <fdm_r44/r44_StabilizerVer.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

class R44_Aircraft;     // aircraft class forward declaration

/**
 * @brief R44 aerodynamics class.
 */
class R44_Aerodynamics : public Aerodynamics
{
public:

    /** Constructor. */
    R44_Aerodynamics( const R44_Aircraft *aircraft, Input *input );

    /** Destructor. */
    virtual ~R44_Aerodynamics();

    /**
     * Reads data.
     * @param dataNode XML node
     */
    void readData( XmlNode &dataNode );

    /** Initializes aerodynamics. */
    void initialize();

    /** Computes force and moment. */
    void computeForceAndMoment();

    /** Updates model. */
    void update();

    inline const R44_MainRotor* getMainRotor() const { return _mainRotor; }

private:

    const R44_Aircraft *_aircraft;      ///< aircraft model main object

    R44_MainRotor     *_mainRotor;      ///<
    R44_TailRotor     *_tailRotor;      ///<
    R44_Fuselage      *_fuselage;       ///<
    R44_StabilizerHor *_stabHor;        ///<
    R44_StabilizerVer *_stabVer;        ///<
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // R44_AERODYNAMICS_H
