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
#ifndef F35A_PROPULSION_H
#define F35A_PROPULSION_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_Propulsion.h>

#include <fdm_f35a/f35a_Engine.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

class F35A_Aircraft;    // aircraft class forward declaration

/**
 * @brief F-35A propulsion class.
 */
class F35A_Propulsion : public Propulsion
{
public:

    /** Constructor. */
    F35A_Propulsion( const F35A_Aircraft *aircraft, Input *input );

    /** Destructor. */
    virtual ~F35A_Propulsion();

    /**
     * Reads data.
     * @param dataNode XML node
     */
    void readData( XmlNode &dataNode );

    /** Initializes propulsion. */
    void initialize();

    /** Computes force and moment. */
    void computeForceAndMoment();

    /** Updates model. */
    void update();

    inline const F35A_Engine* getEngine() const { return _engine; }

private:

    const F35A_Aircraft *_aircraft; ///< aircraft model main object

    F35A_Engine *_engine;           ///< engine model

    DataRef _inputThrottle;         ///< throttle input data reference
    DataRef _inputFuel;             ///< fuel input data reference
    DataRef _inputStarter;          ///< starter input data reference
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // F35A_PROPULSION_H
