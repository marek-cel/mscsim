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
#ifndef C130_PROPULSION_H
#define C130_PROPULSION_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/main/fdm_Propulsion.h>

#include <fdm_c130/c130_Engine.h>
#include <fdm_c130/c130_Propeller.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

class C130_Aircraft;    ///< aircraft class forward declaration

/**
 * @brief C-130 propulsion class.
 */
class C130_Propulsion : public Propulsion
{
public:

    /** Constructor. */
    C130_Propulsion( const C130_Aircraft *aircraft );

    /** Destructor. */
    ~C130_Propulsion();

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

    /** Updates propulsion. */
    void update();

    inline int getEnginesCount() const { return _enginesCount; }

    inline const C130_Engine* getEngine( int i ) const { return _engine[ i ]; }
    inline const C130_Propeller* getPropeller( int i ) const { return _propeller[ i ]; }

private:

    const C130_Aircraft *_aircraft;     ///< aircraft model main object

    const int _enginesCount;            ///< engines count

    C130_Engine    *_engine    [ 4 ];   ///< engine model
    C130_Propeller *_propeller [ 4 ];   ///< propeller model
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // C130_PROPULSION_H
