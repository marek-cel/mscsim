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
#ifndef C172_PROPULSION_H
#define C172_PROPULSION_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_Propulsion.h>

#include <fdm_c172/c172_Engine.h>
#include <fdm_c172/c172_Propeller.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

class C172_Aircraft;    // aircraft class forward declaration

/**
 * @brief Cessna 172 propulsion class.
 */
class C172_Propulsion : public Propulsion
{
public:

    /** Constructor. */
    C172_Propulsion( const C172_Aircraft *aircraft, Input *input );

    /** Destructor. */
    virtual ~C172_Propulsion();

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

    inline const C172_Engine* getEngine() const { return _engine; }
    inline const C172_Propeller* getPropeller() const { return _propeller; }

private:

    const C172_Aircraft *_aircraft; ///< aircraft model main object

    C172_Engine    *_engine;        ///< engine model
    C172_Propeller *_propeller;     ///< propeller model

    DataRef _inputThrottle;         ///< throttle input data reference
    DataRef _inputMixture;          ///< mixture input data reference
    DataRef _inputPropeller;        ///< propeller input data reference
    DataRef _inputFuel;             ///< fuel input data reference
    DataRef _inputIgnition;         ///< ignition input data reference
    DataRef _inputStarter;          ///< starter input data reference
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // C172_PROPULSION_H
