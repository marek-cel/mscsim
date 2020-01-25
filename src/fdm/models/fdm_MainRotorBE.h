/****************************************************************************//*
 * Copyright (C) 2020 Marek M. Cel
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
#ifndef FDM_MAINROTORBE_H
#define FDM_MAINROTORBE_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/models/fdm_MainRotor.h>

#include <fdm/models/fdm_Blade.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Helicopter main rotor model class.
 *
 * This model is based on blade element theory.
 *
 * Flapping angle is positive upwards.
 *
 * Coordinate Systems Used for Rotor Calculations:
 *
 * Rotor Axes System (RAS)
 * Origin of the Rotor Axes System is coincident with the rotor hub center,
 * the x-axis is positive forwards, the y-axis is positive right and z-axis
 * is positive downwards and coincident with the rotor shaft axis.
 *
 * XML configuration file format:
 * @code
 * <main_rotor>
 *   <hub_center> { [m] x-coordinate } { [m] y-coordinate } { [m] z-coordinate } </hub_center>
 *   <inclination> { [rad] rotor inclination angle (positive if forward) } </inclination>
 *   <number_of_blades> { number of blades } </number_of_blades>
 *   <blade>
 *     { blade data }
 *   </blade>
 * </main_rotor>
 * @endcode
 */
class FDMEXPORT MainRotorBE : public MainRotor
{
public:

    typedef std::vector< Blade* > Blades;

    /** Constructor. */
    MainRotorBE();

    /** Destructor. */
    virtual ~MainRotorBE();

    /**
     * Reads data.
     * @param dataNode XML node
     */
    virtual void readData( XmlNode &dataNode );

    /**
     * Computes force and moment.
     * @param vel_bas     [m/s]     aircraft linear velocity vector expressed in BAS
     * @param omg_bas     [rad/s]   aircraft angular velocity expressed in BAS
     * @param acc_bas     [m/s^2]   aircraft linear acceleration vector expressed in BAS
     * @param eps_bas     [rad/s^2] aircraft angular acceleration vector expressed in BAS
     * @param vel_air_bas [m/s]     aircraft linear velocity relative to the air expressed in BAS
     * @param omg_air_bas [rad/s]   aircraft angular velocity relative to the air expressed in BAS
     * @param grav_bas    [m/s^2]   gravity acceleration vector expressed in BAS
     * @param airDensity  [kg/m^3]  air density
     */
    virtual void computeForceAndMoment( const Vector3 &vel_bas,
                                        const Vector3 &omg_bas,
                                        const Vector3 &acc_bas,
                                        const Vector3 &eps_bas,
                                        const Vector3 &vel_air_bas,
                                        const Vector3 &omg_air_bas,
                                        const Vector3 &grav_bas,
                                        double airDensity );

    /**
     * Integrates rotor.
     * @param timeStep    [s]       time step
     * @param vel_bas     [m/s]     aircraft linear velocity vector expressed in BAS
     * @param omg_bas     [rad/s]   aircraft angular velocity expressed in BAS
     * @param acc_bas     [m/s^2]   aircraft linear acceleration vector expressed in BAS
     * @param eps_bas     [rad/s^2] aircraft angular acceleration vector expressed in BAS
     * @param vel_air_bas [m/s]     aircraft linear velocity relative to the air expressed in BAS
     * @param omg_air_bas [rad/s]   aircraft angular velocity relative to the air expressed in BAS
     * @param grav_bas    [m/s^2]   gravity acceleration vector expressed in BAS
     * @param airDensity  [kg/m^3]  air density
     */
    virtual void integrate( double timeStep,
                            const Vector3 &vel_bas,
                            const Vector3 &omg_bas,
                            const Vector3 &acc_bas,
                            const Vector3 &eps_bas,
                            const Vector3 &vel_air_bas,
                            const Vector3 &omg_air_bas,
                            const Vector3 &grav_bas,
                            double airDensity );

    /**
     * Returns rotor total inartia about shaft axis.
     * @return [kg*m^2] rotor total inartia about shaft axis
     */
    inline double getInertia() const { return _ir; }

    inline const Blade* getBlade( int index ) const { return _blades[ index ]; }

protected:

    Blades _blades;             ///< main rotor blades

    double _ir;                 ///< [kg*m^2] rotor inartia about shaft axis

    double _azimuth_prev;       ///< [rad] azimuth (previous value)

    /**
     * @brief Converts parameters to Rotor Axis System.
     * @param vel_bas     [m/s]     aircraft linear velocity vector expressed in BAS
     * @param omg_bas     [rad/s]   aircraft angular velocity expressed in BAS
     * @param acc_bas     [m/s^2]   aircraft linear acceleration vector expressed in BAS
     * @param eps_bas     [rad/s^2] aircraft angular acceleration vector expressed in BAS
     * @param vel_air_bas [m/s]     aircraft linear velocity relative to the air expressed in BAS
     * @param omg_air_bas [rad/s]   aircraft angular velocity relative to the air expressed in BAS
     * @param grav_bas    [m/s^2]   gravity acceleration vector expressed in BAS
     * @param vel_ras     [m/s]     rotor hub linear velocity vector expressed in RAS
     * @param omg_ras     [rad/s]   rotor hub angular velocity expressed in RAS
     * @param acc_ras     [m/s^2]   rotor hub linear acceleration vector expressed in RAS
     * @param eps_ras     [rad/s^2] rotor hub angular acceleration vector expressed in RAS
     * @param vel_air_ras [m/s]     rotor hub linear velocity relative to the air expressed in RAS
     * @param omg_air_ras [rad/s]   rotor hub angular velocity relative to the air expressed in RAS
     * @param grav_ras    [m/s^2]   gravity acceleration vector expressed in RAS
     */
    virtual void convertToHubRAS( const Vector3 &vel_bas,
                                  const Vector3 &omg_bas,
                                  const Vector3 &acc_bas,
                                  const Vector3 &eps_bas,
                                  const Vector3 &vel_air_bas,
                                  const Vector3 &omg_air_bas,
                                  const Vector3 &grav_bas,
                                  Vector3 *vel_ras,
                                  Vector3 *omg_ras,
                                  Vector3 *acc_ras,
                                  Vector3 *eps_ras,
                                  Vector3 *vel_air_ras,
                                  Vector3 *omg_air_ras,
                                  Vector3 *grav_ras );
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_MAINROTORBE_H
