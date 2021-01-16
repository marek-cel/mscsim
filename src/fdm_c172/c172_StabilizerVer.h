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
#ifndef C172_STABILIZERVER_H
#define C172_STABILIZERVER_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/models/fdm_StabilizerVer.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Cessna 172 vertical stabilizer class.
 */
class C172_StabilizerVer : public StabilizerVer
{
public:

    /** Constructor. */
    C172_StabilizerVer();

    /** Destructor. */
    virtual ~C172_StabilizerVer();

    /**
     * Reads data.
     * @param dataNode XML node
     */
    virtual void readData( XmlNode &dataNode );

    /**
     * Computes force and moment.
     * @param vel_air_bas [m/s] aircraft linear velocity relative to the air expressed in BAS
     * @param omg_air_bas [rad/s] aircraft angular velocity relative to the air expressed in BAS
     * @param airDensity [kg/m^3] air density
     * @param rudder [rad] rudder deflection
     */
    void computeForceAndMoment( const Vector3 &vel_air_bas,
                                const Vector3 &omg_air_bas,
                                double airDensity,
                                double rudder );

private:

    double _dcx_drudder;            ///< [1/rad] drag coefficient due to rudder deflection
    double _dcy_drudder;            ///< [1/rad] sideforce coefficient due to rudder deflection

    double _rudder;                 ///< [rad] rudder deflection

    /**
     * Computes drag coefficient.
     * @param angle [rad] "angle of attack"
     * @return [-] drag coefficient
     */
    virtual double getCx( double angle ) const;

    /**
     * Computes sideforce coefficient.
     * @param angle [rad] "angle of attack"
     * @return [-] sideforce coefficient
     */
    virtual double getCy( double angle ) const;
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // C172_STABILIZERVER_H
