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
#ifndef F16_PROPULSION_H
#define F16_PROPULSION_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/main/fdm_Propulsion.h>
#include <fdm/main/fdm_Turbojet.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

class F16_Aircraft;     ///< aircraft class forward declaration

/**
 * @brief F-16 propulsion class.
 */
class F16_Propulsion : public Propulsion
{
public:

    /** Constructor. */
    F16_Propulsion( const F16_Aircraft *aircraft );

    /** Destructor. */
    ~F16_Propulsion();

    /**
     * Initializes propulsion.
     * @param engineOn specifies if engine is working at start
     */
    void init( bool engineOn );

    /**
     * Reads data.
     * @param dataNode XML node
     */
    void readData( XmlNode &dataNode );

    /** Computes force and moment. */
    void computeForceAndMoment();

    /** Updates model. */
    void update();

    inline const Turbojet* getEngine() const { return m_engine; }

private:

    const F16_Aircraft *m_aircraft; ///< aircraft model main object

    Turbojet *m_engine;             ///< engine model
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // F16_PROPULSION_H
