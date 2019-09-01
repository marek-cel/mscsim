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
#ifndef FDM_MAINROTOR_H
#define FDM_MAINROTOR_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_Base.h>

#include <fdm/utils/fdm_Matrix3x3.h>

#include <fdm/xml/fdm_XmlNode.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Helicopter main rotor base class.
 *
 * <h3>Coordinate Systems Used for Rotor Calculations</h3>
 *
 * <h4>Rotor Axes System</h4>
 * <p>Abbreviated as RAS.</p>
 * <p>Origin of the Rotor Axes System is coincident with the rotor hub center,
 * the x-axis is positive forwards, the y-axis is positive right and z-axis
 * is positive downwards and coincident with the rotor shaft axis.</p>
 *
 * <h4>Rotor-Wind Axes System</h4>
 * <p>Abbreviated as RWAS.</p>
 * <p>Rotor-Wind Axes System is very much like Rotor Axes System, the only
 * difference is that it is rotated about z-axis in such a manner that x-axis
 * points directly into relative wind, so there is no lateral airspeed
 * component.</p>
 *
 * <h4>Control Axes System</h4>
 * <p>Abbreviated as CAS.</p>
 * <p>For most purposes, using the Rotor Axes System causes unnecessary
 * complications. It is convenient to use no cyclic feathering axes system.
 * Origin of the Control Axes System is coincident with the origin of the Rotor
 * Axes System, but it is rotated by angles of the swashplate roll and pitch so
 * there is no cyclic feathering in this coordinate system.</p>
 *
 * <h4>Disc Axes System</h4>
 * <p>Abbreviated as DAS.</p>
 * <p>Origin of the Disc Axes System is coincident with the origin of the Rotor
 * Axes System, but it is rotated by angles of the rotor cone roll and pitch
 * in such a manner that z?axis is perpendicular to the tip path plane so there
 * is no cyclic flapping in this coordinate system.</p>
 *
 * <h4>Control-Wind Axes System</h4>
 * <p>Abbreviated as CWAS.</p>
 * <p>Control-Wind Axes System is very much like Control Axes System, the only
 * difference is that it is rotated about z-axis in such a manner that x-axis
 * points directly into relative wind, so there is no lateral airspeed
 * component.</p>
 *
 * <h3>XML configuration file format:</h3>
 * @code
 * <main_rotor>
 *   <hub_center> { [m] x-coordinate } { [m] y-coordinate } { [m] z-coordinate } </hub_center>
 *   <inclination> { [rad] rotor inclination angle (positive if forward) } </inclination>
 *   <number_of_blades> { number of blades } </number_of_blades>
 *   <blade_mass> { [kg] single blade mass } </blade_mass>
 *   <rotor_radius> { [m] rotor radius } </rotor_radius>
 *   <blades_chord> { [m] blades chord } </blades_chord>
 *   <hinge_offset> { [m] flapping hinge offset } </hinge_offset>
 *   <lift_slope> { [1/rad] blade section lift curve slope } </lift_slope>
 *   <tip_losses> { [-] tip losses coefficient } </tip_losses>
 *   <delta_0> { [-] drag coefficient constant component } </delta_0>
 *   <delta_2> { [-] drag coefficient quadratic component } </delta_2>
 *   <beta_max> { [rad] maximum flapping angle } </beta_max>
 *   <ct_max> { [-] maximum thrust coefficient } </ct_max>
 *   <cq_max> { [-] maximum thrust coefficient } </cq_max>
 *   <thrust_factor> { [-] thrust scaling factor } </thrust_factor>
 *   <torque_factor> { [-] torque scaling factor } </torque_factor>
 *   <vel_i_factor> { [-] induced velocity scaling factor } </vel_i_factor>
 * </main_rotor>
 * @endcode
 *
 * <p>Optional elements: "thrust_factor", "torque_factor", "vel_i_factor"</p>
 *
 * @see Mil M.: Helicopters: Calculation and Design. Volume 1: Aerodynamics, NASA, TM-X-74335, 1967
 * @see Gessow A., Myers G.: Aerodynamics of the Helicopter, 1985
 * @see Bramwell A.: Bramwells Helicopter Dynamics, 2001
 * @see Padfield G.: Helicopter Flight Dynamics, 2007
 * @see Stepniewski W.: Rotary-Wing Aerodynamics. Volume I: Basic Theories of Rotor Aerodynamics, 1984
 * @see Johnson W.: Helicopter Theory, 1980
 */
class FDMEXPORT MainRotor : public Base
{
public:

    /** Rotor direction. */
    enum Direction
    {
        CW  = 0,    ///< clockwise (looking from above)
        CCW = 1     ///< counter-clockwise (looking from above)
    };

    /** Constructor. */
    MainRotor();

    /** Destructor. */
    virtual ~MainRotor();

    /**
     * Reads data.
     * @param dataNode XML node
     */
    virtual void readData( XmlNode &dataNode );

    /**
     * Computes force and moment.
     * @param vel_bas [m/s] aircraft linear velocity vector expressed in BAS
     * @param omg_bas [rad/s] aircraft angular velocity expressed in BAS
     * @param acc_bas [m/s^2] aircraft linear acceleration vector expressed in BAS
     * @param eps_bas [rad/s^2] aircraft angular acceleration vector expressed in BAS
     * @param grav_bas [m/s^2] gravity acceleration vector expressed in BAS
     * @param vel_air_bas [m/s] aircraft linear velocity relative to the air expressed in BAS
     * @param omg_air_bas [rad/s] aircraft angular velocity relative to the air expressed in BAS
     * @param airDensity [kg/m^3] air density
     * @param collective [rad] collective pitch angle
     * @param cyclicLat [rad] cyclic lateral pitch angle
     * @param cyclicLon [rad] cyclic longitudinal pitch angle
     */
    virtual void computeForceAndMoment( const Vector3 &vel_bas,
                                        const Vector3 &omg_bas,
                                        const Vector3 &acc_bas,
                                        const Vector3 &eps_bas,
                                        const Vector3 &grav_bas,
                                        const Vector3 &vel_air_bas,
                                        const Vector3 &omg_air_bas,
                                        double airDensity,
                                        double collective,
                                        double cyclicLat,
                                        double cyclicLon );

    /**
     * @brief update
     * @param omega [rad/s]
     */
    virtual void update( double omega );

    inline const Vector3& getFor_BAS() const { return _for_bas; }
    inline const Vector3& getMom_BAS() const { return _mom_bas; }

    inline const Vector3& getVel_i_BAS() const { return _vel_i_bas; }

    inline double getRadius()   const { return _r;  }
    inline double getDiskArea() const { return _ad; }
    inline double getInertia()  const { return _nb * _i_b; }

    inline double getConingAngle() const { return _beta_0; }

    inline double getDiskRoll()  const { return _rotorDiskRoll;  }
    inline double getDiskPitch() const { return _rotorDiskPitch; }

    inline double getThrust() const { return _thrust; }
    inline double getTorque() const { return _torque; }

protected:

    Vector3 _for_bas;           ///< [N] total force vector expressed in BAS
    Vector3 _mom_bas;           ///< [N*m] total moment vector expressed in BAS

    Direction _direction;       ///< rotor direction (clockwise or counter-clockwise)

    Vector3 _r_hub_bas;         ///< [m] rotor hub coordinates expressed in BAS

    Matrix3x3 _bas2ras;         ///< matrix of rotation from BAS to RAS
    Matrix3x3 _ras2bas;         ///< matrix of rotation from RAS to BAS
    Matrix3x3 _ras2cas;         ///< matrix of rotation from RAS to CAS

    Matrix3x3 _cas2ras;         ///< matrix of rotation from CAS to RAS

    Matrix3x3 _bas2cas;         ///< matrix of rotation from BAS to CAS
    Matrix3x3 _bas2das;         ///< matrix of rotation from BAS to DAS
    Matrix3x3 _das2bas;         ///< matrix of rotation from DAS to BAS

    Matrix3x3 _ras2rwas;        ///< matrix of rotation from RAS to RWAS
    Matrix3x3 _rwas2ras;

    Matrix3x3 _cas2cwas;        ///< matrix of rotation from CAS to CWAS
    Matrix3x3 _cwas2cas;        ///< matrix of rotation from CWAS to CAS

    Matrix3x3 _bas2cwas;        ///< matrix of rotation from BAS to CWAS

    Vector3 _vel_i_bas;         ///< [m/s] induced velocity

    int _nb;                    ///< number of rotor blades

    double _r;                  ///< [m] rotor radius
    double _c;                  ///< [m] blades chord
    double _e;                  ///< [m] flapping hinge offset

    double _a;                  ///< [1/rad] blade section lift curve slope
    double _b;                  ///< [-] tip losses coefficient

    double _delta_0;            ///< [-] drag coefficient constant component
    double _delta_2;            ///< [-] drag coefficient quadratic component

    double _beta_max;           ///< [rad] maximum flapping angle

    double _ct_max;             ///< [-] maximum thrust coefficient
    double _cq_max;             ///< [-] maximum torque coefficient

    double _thrust_factor;      ///< [-] thrust scaling factor
    double _torque_factor;      ///< [-] torque scaling factor
    double _vel_i_factor;       ///< [-] induced velocity scaling factor

    double _r2;                 ///< [m^2] rotor radius squared
    double _r3;                 ///< [m^3] rotor radius cubed
    double _r4;                 ///< [m^3] rotor radius to the power of 4
    double _b2;                 ///< [-] tip losses coefficient squared
    double _b3;                 ///< [-] tip losses coefficient cubed
    double _b4;                 ///< [-] tip losses coefficient to the power of 4
    double _ad;                 ///< [m^2] rotor disk area
    double _s;                  ///< [-] rotor solidity

    double _s_b;                ///< [kg*m] single rotor blade first moment of mass about flapping hinge
    double _i_b;                ///< [kg*m^2] single rotor blade inertia moment about flapping hinge

    double _omega;              ///< [rad/s] rotor revolution speed

    double _beta_0;             ///< [rad] rotor coning angle
    double _beta_1c_ras;        ///< [rad] longitudinal flapping angle expressed in RAS
    double _beta_1s_ras;        ///< [rad] lateral flapping angle expressed in RAS

    double _rotorDiskRoll;      ///< [rad] rotor disk roll angle
    double _rotorDiskPitch;     ///< [rad] rotor disk pitch angle

    double _thrust;             ///< [N] rotor thrust
    double _torque;             ///< [N*m] rotor torque
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_MAINROTOR_H
