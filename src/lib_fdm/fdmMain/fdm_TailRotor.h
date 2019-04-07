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
#ifndef FDM_TAILROTOR_H
#define FDM_TAILROTOR_H

////////////////////////////////////////////////////////////////////////////////

#include <fdmMain/fdm_Base.h>

#include <fdmUtils/fdm_Matrix3x3.h>

#include <fdmXml/fdm_XmlNode.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Helicopter tail rotor base class.
 *
 * <h3>XML configuration file format:</h3>
 * @code
 * <tail_rotor>
 *   <hub_center> { [m] x-coordinate } { [m] y-coordinate } { [m] z-coordinate } </hub_center>
 *   <cant_angle> { [rad] cant angle } </cant_angle>
 *   <number_of_blades> { number of blades } </number_of_blades>
 *   <blade_mass> { [kg] single blade mass } </blade_mass>
 *   <rotor_radius> { [m] rotor radius } </rotor_radius>
 *   <blades_chord> { [m] blades chord } </blades_chord>
 *   <lift_slope> { [1/rad] blade section lift curve slope } </lift_slope>
 *   <tip_losses> { [-] tip losses coefficient } </tip_losses>
 *   <delta_0> { [-] drag coefficient constant component } </delta_0>
 *   <delta_2> { [-] drag coefficient quadratic component } </delta_2>
 *   <ct_max> { [-] maximum thrust coefficient } </ct_max>
 *   <cq_max> { [-] maximum thrust coefficient } </cq_max>
 *   <thrust_factor> { [-] thrust scaling factor } </thrust_factor>
 *   <torque_factor> { [-] torque scaling factor } </torque_factor>
 *   <vel_i_factor> { [-] induced velocity scaling factor } </vel_i_factor>
 * </tail_rotor>
 * @endcode
 *
 * <p>Optional elements: "thrust_factor", "torque_factor", "vel_i_factor"</p>
 *
 * @see Gessow A., Myers G.: Aerodynamics of the Helicopter, 1985
 * @see Bramwell A.: Bramwells Helicopter Dynamics, 2001
 * @see Padfield G.: Helicopter Flight Dynamics, 2007
 * @see Johnson W.: Helicopter Theory, 1980
 */
class FDMEXPORT TailRotor : public Base
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

    virtual void computeForceAndMoment( const Vector3 &vel_air_bas,
                                        const Vector3 &omg_air_bas,
                                        double airDensity,
                                        double collective );

    /**
     * @brief update
     * @param omega [rad/s]
     */
    virtual void update( double omega );

    inline const Vector3& getFor_BAS() const { return m_for_bas; }
    inline const Vector3& getMom_BAS() const { return m_mom_bas; }

    inline const Vector3& getVel_i_BAS() const { return m_vel_i_bas; }

    inline double getMomentOfInertia() const { return m_nb * m_i_b; }

    inline double getThrust() const { return m_thrust; }
    inline double getTorque() const { return m_torque; }

protected:

    Vector3 m_for_bas;          ///< [N] total force vector expressed in BAS
    Vector3 m_mom_bas;          ///< [N*m] total moment vector expressed in BAS

    Vector3 m_r_hub_bas;        ///< [m] rotor hub coordinates expressed in BAS

    Matrix3x3 m_bas2ras;        ///< matrix of rotation from BAS to RAS
    Matrix3x3 m_ras2bas;        ///< matrix of rotation from RAS to BAS

    Vector3 m_vel_i_bas;        ///< [m/s] induced velocity

    int m_nb;                   ///< number of rotor blades

    double m_r;                 ///< [m] rotor radius
    double m_c;                 ///< [m] blades chord

    double m_a;                 ///< [1/rad] blade section lift curve slope
    double m_b;                 ///< [-] tip losses coefficient

    double m_delta_0;           ///< [-] drag coefficient constant component
    double m_delta_2;           ///< [-] drag coefficient quadratic component

    double m_ct_max;            ///< [-] maximum thrust coefficient
    double m_cq_max;            ///< [-] maximum torque coefficient

    double m_thrust_factor;     ///< [-] thrust scaling factor
    double m_torque_factor;     ///< [-] torque scaling factor
    double m_vel_i_factor;      ///< [-] induced velocity scaling factor

    double m_r2;                ///< [m^2] rotor radius squared
    double m_r3;                ///< [m^3] rotor radius cubed
    double m_ad;                ///< [m^2] rotor disk area
    double m_s;                 ///< [-] rotor solidity

    double m_i_b;               ///< [kg*m^2] single rotor blade inertia moment about flapping hinge

    double m_omega;             ///< [rad/s] rotor revolution speed

    double m_thrust;            ///< [N] rotor thrust
    double m_torque;            ///< [N*m] rotor torque
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_TAILROTOR_H
