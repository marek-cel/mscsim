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
#ifndef P51_PROPULSION_H
#define P51_PROPULSION_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_Propulsion.h>

#include <fdm_p51/p51_Engine.h>
#include <fdm_p51/p51_Propeller.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

class P51_Aircraft;    // aircraft class forward declaration

/**
 * @brief P-51 propulsion class.
 */
class P51_Propulsion : public Propulsion
{
public:

    /** Constructor. */
    P51_Propulsion( const P51_Aircraft *aircraft, Input *input );

    /** Destructor. */
    virtual ~P51_Propulsion();

    /**
     * Reads data.
     * @param dataNode XML node
     */
    void readData( XmlNode &dataNode );

    /** Initializes propulsion. */
    void initialize();

    /** Computes force and moment. */
    void computeForceAndMoment();

    /** Updates propulsion. */
    void update();

    inline const P51_Engine* getEngine() const { return _engine; }
    inline const P51_Propeller* getPropeller() const { return _propeller; }

private:

    const P51_Aircraft *_aircraft;  ///< aircraft model main object

    P51_Engine    *_engine;         ///< engine model
    P51_Propeller *_propeller;      ///< propeller model

    DataRef _inputThrottle;         ///< throttle input data reference
    DataRef _inputMixture;          ///< mixture input data reference
    DataRef _inputPropeller;        ///< propeller input data reference
    DataRef _inputFuel;             ///< fuel input data reference
    DataRef _inputIgnition;         ///< ignition input data reference
    DataRef _inputStarter;          ///< starter input data reference
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // P51_PROPULSION_H
