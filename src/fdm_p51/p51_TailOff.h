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
#ifndef P51_TAILOFF_H
#define P51_TAILOFF_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/models/fdm_TailOff.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief P-51 tail-off aerodynamics class.
 */
class P51_TailOff : public TailOff
{
public:

    /** Constructor. */
    P51_TailOff();

    /** Destructor. */
    virtual ~P51_TailOff();

    /**
     * Reads data.
     * @param dataNode XML node
     */
    void readData( fdm::XmlNode &dataNode );

    /**
     * Computes force and moment.
     * @param vel_air_bas [m/s] aircraft linear velocity relative to the air expressed in BAS
     * @param omg_air_bas [rad/s] aircraft angular velocity relative to the air expressed in BAS
     * @param airDensity [kg/m^3] air density
     * @param ailerons [rad] ailerons deflection
     * @param flaps [rad] flaps deflection
     * @param gear [rad] landing gear
     */
    void computeForceAndMoment( const fdm::Vector3 &vel_air_bas,
                                const fdm::Vector3 &omg_air_bas,
                                double airDensity ,
                                double ailerons,
                                double flaps,
                                double gear );

    /**
     * Updates model.
     * @param vel_air_bas [m/s] aircraft linear velocity relative to the air expressed in BAS
     * @param omg_air_bas [rad/s] aircraft angular velocity relative to the air expressed in BAS
     */
    void update( const fdm::Vector3 &vel_air_bas, const fdm::Vector3 &omg_air_bas );

private:

    double _ailerons;               ///< [rad] ailerons deflection
    double _flaps;                  ///< [rad] flaps deflection
    double _landing_gear;           ///< [-] landing gear

    double _dcl_dailerons;          ///< [1/rad]

    double _dcx_dgear;              ///< [-/-]
    double _dcz_dgear;              ///< [-/-]
    double _dcm_dgear;              ///< [-/-]

    Table1 _dcx_dflaps;             ///< [1/rad]
    Table1 _dcz_dflaps;             ///< [1/rad]
    Table1 _dcm_dflaps;             ///< [1/rad]

    /**
     * Computes drag coefficient.
     * @param angleOfAttack [rad] angle of attack
     * @return [-] drag coefficient
     */
    double getCx( double angleOfAttack ) const;

    /**
     * Computes sideforce coefficient.
     * @param sideslipAngle [rad] angle of sideslip
     * @return [-] sideforce coefficient
     */
    double getCy( double sideslipAngle ) const;

    /**
     * Computes lift coefficient.
     * @param angleOfAttack [rad] angle of attack
     * @return [-] lift coefficient
     */
    double getCz( double angleOfAttack ) const;

    /**
     * Computes rolling moment coefficient.
     * @param sideslipAngle [rad] angle of sideslip
     * @return [-] rolling moment coefficient
     */
    double getCl( double sideslipAngle ) const;

    /**
     * Computes pitching moment coefficient.
     * @param angleOfAttack [rad] angle of attack
     * @return [-] pitching moment coefficient
     */
    double getCm( double angleOfAttack ) const;

    /**
     * Computes yawing moment coefficient.
     * @param sideslipAngle [rad] angle of sideslip
     * @return [-] yawing moment coefficient
     */
    double getCn( double sideslipAngle ) const;
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // P51_TAILOFF_H
