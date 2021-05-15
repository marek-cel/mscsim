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
#ifndef XH_AERODYNAMICS_H
#define XH_AERODYNAMICS_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_Aerodynamics.h>

#include <fdm_xh/xh_MainRotorBE.h>
#include <fdm_xh/xh_TailRotor.h>
#include <fdm_xh/xh_Fuselage.h>
#include <fdm_xh/xh_StabilizerHor.h>
#include <fdm_xh/xh_StabilizerVer.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

class XH_Aircraft;      // aircraft class forward declaration

/**
 * @brief X/H aerodynamics class.
 */
class XH_Aerodynamics : public Aerodynamics
{
public:

    /** Constructor. */
    XH_Aerodynamics( const XH_Aircraft *aircraft, Input *input );

    /** Destructor. */
    virtual ~XH_Aerodynamics();

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

    inline const XH_MainRotor* getMainRotor() const { return _mainRotor; }

private:

    const XH_Aircraft *_aircraft;       ///< aircraft model main object

    XH_MainRotor     *_mainRotor;
    XH_TailRotor     *_tailRotor;     ///<
    XH_Fuselage      *_fuselage;      ///<
    XH_StabilizerHor *_stabHor;       ///<
    XH_StabilizerVer *_stabVer;       ///<
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // XH_AERODYNAMICS_H
