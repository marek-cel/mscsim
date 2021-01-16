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
#ifndef XF_TAILOFF_H
#define XF_TAILOFF_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/models/fdm_TailOff.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief X/F tail-off aerodynamics class.
 */
class XF_TailOff : public TailOff
{
public:

    /** Constructor. */
    XF_TailOff();

    /** Destructor. */
    virtual ~XF_TailOff();

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
     * @param airbrake [-] airbrake normalized deflection
     * @param flaps_le [rad] leading flaps deflection
     * @param flaps_te [rad] trailing edge flaps deflection
     */
    void computeForceAndMoment( const fdm::Vector3 &vel_air_bas,
                                const fdm::Vector3 &omg_air_bas,
                                double airDensity ,
                                double ailerons,
                                double airbrake,
                                double flaps_le,
                                double flaps_te );

    /**
     * Updates model.
     * @param vel_air_bas [m/s] aircraft linear velocity relative to the air expressed in BAS
     * @param omg_air_bas [rad/s] aircraft angular velocity relative to the air expressed in BAS
     */
    void update( const fdm::Vector3 &vel_air_bas, const fdm::Vector3 &omg_air_bas );

private:

    double _ailerons;               ///< [rad] ailerons deflection
    double _airbrake;               ///< [-] airbrake normalized deflection
    double _flaps_le;               ///< [rad] leading flaps deflection
    double _flaps_te;               ///< [rad] trailing edge flaps deflection

    double _dcl_dailerons;          ///< [1/rad]

    double _dcx_dairbrake;          ///< [1/-]
    double _dcz_dairbrake;          ///< [1/-]

    Table1 _dcx_dflaps_te;          ///< [1/rad]
    Table1 _dcz_dflaps_te;          ///< [1/rad]
    Table1 _dcm_dflaps_te;          ///< [1/rad]

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

#endif // XF_TAILOFF_H
