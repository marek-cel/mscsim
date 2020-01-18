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
 * <h3>Coordinate Systems Used for Rotor Calculations</h3>
 *
 * <h4>Rotor Axes System</h4>
 * <p>Abbreviated as RAS.</p>
 * <p>Origin of the Rotor Axes System is coincident with the rotor hub center,
 * the x-axis is positive forwards, the y-axis is positive right and z-axis
 * is positive downwards and coincident with the rotor shaft axis.</p>
 *
 * <h4>Individual Blade Coordinates</h4>
 * <p>Abbreviated as IBC.</p>
 *
 * <h3>XML configuration file format:</h3>
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
     * Integrates model.
     * @param timeStep [s] time step
     * @param vel_bas [m/s] aircraft linear velocity vector expressed in BAS
     * @param omg_bas [rad/s] aircraft angular velocity expressed in BAS
     * @param acc_bas [m/s^2] aircraft linear acceleration vector expressed in BAS
     * @param eps_bas [rad/s^2] aircraft angular acceleration vector expressed in BAS
     * @param grav_bas [m/s^2] gravity acceleration vector expressed in BAS
     * @param vel_air_bas [m/s] aircraft linear velocity relative to the air expressed in BAS
     * @param omg_air_bas [rad/s] aircraft angular velocity relative to the air expressed in BAS
     * @param airDensity [kg/m^3] air density
     */
    virtual void integrate( double timeStep,
                            const Vector3 &vel_bas,
                            const Vector3 &omg_bas,
                            const Vector3 &acc_bas,
                            const Vector3 &eps_bas,
                            const Vector3 &grav_bas,
                            const Vector3 &vel_air_bas,
                            const Vector3 &omg_air_bas,
                            double airDensity );

    /**
     * @brief Updates main rotor model.
     * @param omega [rad/s] rotor revolution speed
     * * @param azimuth [rad]
     * @param collective [rad] collective pitch angle
     * @param cyclicLat [rad] cyclic lateral pitch angle
     * @param cyclicLon [rad] cyclic longitudinal pitch angle
     */
    virtual void update( double omega,
                         double azimuth,
                         double collective,
                         double cyclicLat,
                         double cyclicLon );

    /**
     * Returns rotor total inartia about shaft axis.
     * @return [kg*m^2] rotor total inartia about shaft axis
     */
    inline double getInertia() const { return _ir; }

protected:

    Blades _blades;             ///< main rotor blades

    double _ir;                 ///< [kg*m^2] rotor inartia about shaft axis

    double _delta_psi;          ///< [rad]

    double _theta_0;            ///< [rad] collective feathering angle
    double _theta_1c;           ///< [rad]
    double _theta_1s;           ///< [rad]
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_MAINROTORBE_H
