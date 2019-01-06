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
#ifndef C172_PROPULSION_H
#define C172_PROPULSION_H

////////////////////////////////////////////////////////////////////////////////

#include <fdmMain/fdm_Propulsion.h>
#include <fdmMain/fdm_PistonEngine.h>
#include <fdmMain/fdm_Propeller.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

class C172_Aircraft; ///< aircraft class forward declaration

/**
 * @brief Cessna 172 propulsion class.
 */
class C172_Propulsion : public Propulsion
{
public:

    /** Constructor. */
    C172_Propulsion( const C172_Aircraft *aircraft );

    /** Destructor. */
    ~C172_Propulsion();

    /**
     * Initializes engine due to initial engine state.
     * @param engineOn specifies if engine is working at start
     */
    void initialize( bool engineOn );

    /**
     * Reads data.
     * @param dataNode XML node
     */
    void readData( XmlNode &dataNode );

    /**
     * Computes force and moment.
     */
    void computeForceAndMoment();

    /**
     * Integrates model.
     * @param timeStep [s] time step
     */
    void integrate( double timeStep );

    /**
     * Updates model.
     */
    void update();

    /** */
    inline double getTorque() const
    {
        return m_propeller->getTorque();
    }

private:

    const C172_Aircraft *m_aircraft;    ///< aircraft model main object

    PistonEngine *m_engine;             ///< engine model
    Propeller *m_propeller;             ///< propeller model

    DataRef m_drThrottle;               ///< engine throttle data reference
    DataRef m_drMixture;                ///< engine mixture lever data reference
    DataRef m_drFuel;                   ///< engine fuel data reference
    DataRef m_drIgnition;               ///< engine ignition data reference
    DataRef m_drStarter;                ///< engine starter data reference

    DataRef m_drEngineOn;               ///< engine state data refenrence
    DataRef m_drEngineRPM;              ///< engine rpm data reference
    DataRef m_drEngineMAP;              ///< engine manifold absolute pressure data reference
    DataRef m_drEngineFF;               ///< engine fuel flow data reference
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // C172_PROPULSION_H
