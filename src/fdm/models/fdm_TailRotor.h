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
#ifndef FDM_TAILROTOR_H
#define FDM_TAILROTOR_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/utils/fdm_Matrix3x3.h>

#include <fdm/xml/fdm_XmlNode.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Helicopter tail rotor model class.
 *
 * This model is based on actuator disc theory.
 *
 * XML configuration file format:
 * @code
 * <tail_rotor>
 *   <hub_center> { [m] x-coordinate } { [m] y-coordinate } { [m] z-coordinate } </hub_center>
 *   <cant_angle> { [rad] cant angle } </cant_angle>
 *   <number_of_blades> { number of blades } </number_of_blades>
 *   <blade_mass> { [kg] single blade mass } </blade_mass>
 *   <rotor_radius> { [m] rotor radius } </rotor_radius>
 *   <blade_chord> { [m] blades chord } </blade_chord>
 *   <lift_slope> { [1/rad] blade section lift curve slope } </lift_slope>
 *   <tip_losses> { [-] tip losses coefficient } </tip_losses>
 *   <delta_0> { [-] drag coefficient constant component } </delta_0>
 *   <delta_2> { [-] drag coefficient quadratic component } </delta_2>
 *   <ct_max> { [-] maximum thrust coefficient } </ct_max>
 *   <cq_max> { [-] maximum thrust coefficient } </cq_max>
 *   [<thrust_factor> { [-] thrust scaling factor } </thrust_factor>]
 *   [<torque_factor> { [-] torque scaling factor } </torque_factor>]
 *   [<vel_i_factor> { [-] induced velocity scaling factor } </vel_i_factor>]
 * </tail_rotor>
 * @endcode
 *
 * Optional elements: "thrust_factor", "torque_factor", "vel_i_factor"
 *
 * @see Gessow A., Myers G.: Aerodynamics of the Helicopter, 1985
 * @see Bramwell A.: Bramwells Helicopter Dynamics, 2001
 * @see Padfield G.: Helicopter Flight Dynamics, 2007
 * @see Johnson W.: Helicopter Theory, 1980
 */
class FDMEXPORT TailRotor
{
public:

    /** Constructor. */
    TailRotor();

    /** Destructor. */
    virtual ~TailRotor();

    /**
     * Reads data.
     * @param dataNode XML node
     */
    virtual void readData( XmlNode &dataNode );

    /**
     * Computes force and moment.
     * @param vel_air_bas [m/s]    aircraft linear velocity relative to the air expressed in BAS
     * @param omg_air_bas [rad/s]  aircraft angular velocity relative to the air expressed in BAS
     * @param airDensity  [kg/m^3] air density
     */
    virtual void computeForceAndMoment( const Vector3 &vel_air_bas,
                                        const Vector3 &omg_air_bas,
                                        double airDensity );

    /**
     * @brief Updates rotor model.
     * @param omega      [rad/s] rotor revolution speed
     * @param collective [rad]   collective pitch angle
     */
    virtual void update( double omega, double collective );

    inline const Vector3& getFor_BAS() const { return _for_bas; }
    inline const Vector3& getMom_BAS() const { return _mom_bas; }

    inline const Vector3& getVel_i_BAS() const { return _vel_i_bas; }

    inline double getMomentOfInertia() const { return _nb * _i_b; }

    inline double getThrust() const { return _thrust; }
    inline double getTorque() const { return _torque; }

protected:

    Vector3 _for_bas;           ///< [N] total force vector expressed in BAS
    Vector3 _mom_bas;           ///< [N*m] total moment vector expressed in BAS

    Vector3 _r_hub_bas;         ///< [m] rotor hub coordinates expressed in BAS

    Matrix3x3 _bas2ras;         ///< matrix of rotation from BAS to RAS
    Matrix3x3 _ras2bas;         ///< matrix of rotation from RAS to BAS

    Vector3 _vel_i_bas;         ///< [m/s] induced velocity

    int _nb;                    ///< number of rotor blades

    double _r;                  ///< [m] rotor radius
    double _c;                  ///< [m] blades chord

    double _a;                  ///< [1/rad] blade section lift curve slope
    double _b;                  ///< [-] tip losses coefficient

    double _delta_0;            ///< [-] drag coefficient constant component
    double _delta_2;            ///< [-] drag coefficient quadratic component

    double _ct_max;             ///< [-] maximum thrust coefficient
    double _cq_max;             ///< [-] maximum torque coefficient

    double _thrust_factor;      ///< [-] thrust scaling factor
    double _torque_factor;      ///< [-] torque scaling factor
    double _vel_i_factor;       ///< [-] induced velocity scaling factor

    double _r2;                 ///< [m^2] rotor radius squared
    double _r3;                 ///< [m^3] rotor radius cubed
    double _ad;                 ///< [m^2] rotor disk area
    double _s;                  ///< [-] rotor solidity

    double _i_b;                ///< [kg*m^2] single rotor blade inertia moment about flapping hinge

    double _omega;              ///< [rad/s] rotor revolution speed

    double _theta;              ///< [rad] feathering angle

    double _thrust;             ///< [N] rotor thrust
    double _torque;             ///< [N*m] rotor torque
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_TAILROTOR_H
