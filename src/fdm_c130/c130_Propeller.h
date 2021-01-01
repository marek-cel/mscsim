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
#ifndef C130_PROPELLER_H
#define C130_PROPELLER_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/models/fdm_Propeller.h>

#include <fdm_c130/c130_Governor.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief C-130 propeller class.
 */
class C130_Propeller : public Propeller
{
public:

    /** Constructor. */
    C130_Propeller();

    /** Destructor. */
    virtual ~C130_Propeller();

    /**
     * Reads data.
     * @param dataNode XML node
     */
    void readData( XmlNode &dataNode );

    /**
     * Updates propeller.
     * @param normPitch <0.0;1.0> normalized propeller lever position
     * @param engineTorque [N] engine torque
     * @param airspeed [m/s] airspeed
     * @param airDensity [kg/m^3] air density
     */
    void update( double propellerLever, double engineTorque,
                 double airspeed, double airDensity );

private:

    C130_Governor *_governor;       ///< propeller governor model
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // C130_PROPELLER_H
