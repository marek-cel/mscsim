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
#ifndef FDM_MAINROTORBE_H
#define FDM_MAINROTORBE_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/models/fdm_RotorBlade.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Helicopter main rotor model class.
 *
 * Articulated blades dynamic model based on blade element theory.
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
 *   <rotor_radius> { [m] rotor radius } </rotor_radius>
 *   <blade>
 *     { blade data }
 *   </blade>
 * </main_rotor>
 * @endcode
 *
 * @see fdm::Blade
 */
class FDMEXPORT MainRotorBE
{
public:

    typedef std::vector< RotorBlade* > Blades;

    /** Rotor direction. */
    enum Direction
    {
        CW  = 0,    ///< clockwise (looking from above)
        CCW = 1     ///< counter-clockwise (looking from above)
    };

    static const double _timeStepMax;       ///< [s] maximum integration time step

    /** @brief Constructor. */
    MainRotorBE();

    /** @brief Destructor. */
    virtual ~MainRotorBE();

    /**
     * @brief Reads data.
     * @param dataNode XML node
     */
    virtual void readData( XmlNode &dataNode );

    /**
     * @brief Compute forces and moments.
     * @param vel_air_bas [m/s]     rotor hub linear velocity relative to airflow expressed in BAS
     * @param omg_air_bas [rad/s]   rotor hub angular velocity relative to airflow expressed in BAS
     * @param omg_bas     [rad/s]   angular velocity expressed in BAS
     * @param acc_bas     [m/s^2]   rotor hub linear acceleration expressed in BAS
     * @param eps_bas     [rad/s^2] angular acceleration expressed in BAS
     * @param grav_bas    [m/s^2]   gravity acceleration vector expressed in BAS
     * @param airDensity  [kg/m^3]  air density
     */
    virtual void computeForceAndMoment( const Vector3 &vel_air_bas,
                                        const Vector3 &omg_air_bas,
                                        const Vector3 &omg_bas,
                                        const Vector3 &acc_bas,
                                        const Vector3 &eps_bas,
                                        const Vector3 &grav_bas,
                                        double airDensity );

    /**
     * @brief Updates main rotor model.
     * @param timeStep    [s]       time step
     * @param vel_air_bas [m/s]     rotor hub linear velocity relative to airflow expressed in BAS
     * @param omg_air_bas [rad/s]   rotor hub angular velocity relative to airflow expressed in BAS
     * @param omg_bas     [rad/s]   angular velocity expressed in BAS
     * @param acc_bas     [m/s^2]   rotor hub linear acceleration expressed in BAS
     * @param eps_bas     [rad/s^2] angular acceleration expressed in BAS
     * @param grav_bas    [m/s^2]   gravity acceleration vector expressed in BAS
     * @param omega       [rad/s]   rotor speed
     * @param azimuth     [rad]     blade azimuth
     * @param airDensity  [kg/m^3]  air density
     * @param collective  [rad]     collective pitch angle
     * @param cyclicLat   [rad]     cyclic lateral pitch angle
     * @param cyclicLon   [rad]     cyclic longitudinal pitch angle
     */
    virtual void update( double timeStep,
                         const Vector3 &vel_air_bas,
                         const Vector3 &omg_air_bas,
                         const Vector3 &omg_bas,
                         const Vector3 &acc_bas,
                         const Vector3 &eps_bas,
                         const Vector3 &grav_bas,
                         double omega,
                         double azimuth,
                         double airDensity,
                         double collective,
                         double cyclicLat,
                         double cyclicLon );

    inline Direction getDirection() const { return _direction; }

    inline const RotorBlade* getBlade( int index ) const { return _blades[ index ]; }

    inline const Vector3& getFor_BAS() const { return _for_bas; }
    inline const Vector3& getMom_BAS() const { return _mom_bas; }

    /**
     * @brief Returns rotor total inartia about shaft axis.
     * @return [kg*m^2] rotor total inartia about shaft axis
     */
    inline double getInertia() const { return _i_tot; }

    inline const Vector3& getR_hub_BAS() const { return _r_hub_bas; }

    inline int getNumberOfBlades() const { return _blades_no; }

    inline double getRadius() const { return _radius; }

    inline double getBeta0()  const { return _beta_0;  }
    inline double getBeta1c() const { return _beta_1c; }
    inline double getBeta1s() const { return _beta_1s; }

    inline double getTheta0()  const { return _theta_0;  }
    inline double getTheta1c() const { return _theta_1c; }
    inline double getTheta1s() const { return _theta_1s; }

    inline double getConingAngle() const { return _coningAngle; }
    inline double getDiskRoll()    const { return _diskRoll;    }
    inline double getDiskPitch()   const { return _diskPitch;   }

    inline double getThrust() const { return _thrust; }
    inline double getTorque() const { return _torque; }

    inline double getVel_i() const { return _vel_i; }

    inline double getWakeSkew() const { return _wakeSkew; }

protected:

    Direction _direction;       ///< rotor rotation direction (clockwise or counter-clockwise)

    Blades _blades;             ///< main rotor blades

    Vector3 _for_bas;           ///< [N] total force vector expressed in BAS
    Vector3 _mom_bas;           ///< [N*m] total moment vector expressed in BAS

    Vector3 _r_hub_bas;         ///< [m] rotor hub coordinates expressed in BAS

    Matrix3x3 _bas2ras;         ///< matrix of rotation from BAS to RAS
    Matrix3x3 _ras2bas;         ///< matrix of rotation from RAS to BAS

    Vector3 _vel_air_ras;       ///< [m/s]     rotor hub linear velocity relative to airflow expressed in RAS
    Vector3 _omg_air_ras;       ///< [rad/s]   rotor hub angular velocity relative to airflow expressed in RAS
    Vector3 _omg_ras;           ///< [rad/s]   angular velocity expressed in RAS
    Vector3 _acc_ras;           ///< [m/s^2]   rotor hub linear acceleration expressed in RAS
    Vector3 _eps_ras;           ///< [rad/s^2] angular acceleration expressed in RAS
    Vector3 _grav_ras;          ///< [m/s^2]   gravity acceleration vector expressed in RAS

    Vector3 _prev_vel_air_ras;  ///< [m/s]     rotor hub linear velocity relative to airflow expressed in RAS (previous value)
    Vector3 _prev_omg_air_ras;  ///< [rad/s]   rotor hub angular velocity relative to airflow expressed in RAS (previous value)
    Vector3 _prev_omg_ras;      ///< [rad/s]   angular velocity expressed in RAS (previous value)
    Vector3 _prev_acc_ras;      ///< [m/s^2]   rotor hub linear acceleration expressed in RAS (previous value)
    Vector3 _prev_eps_ras;      ///< [rad/s^2] angular acceleration expressed in RAS (previous value)
    Vector3 _prev_grav_ras;     ///< [m/s^2]   gravity acceleration vector expressed in RAS (previous value)

    int _blades_no;             ///< number of rotor blades

    double _radius;             ///< [m] rotor radius

    double _omega;              ///< [rad/s] rotor revolution speed
    double _azimuth;            ///< [rad] rotor azimuth position

    double _i_tot;              ///< [kg*m^2] rotor total inartia about shaft axis
    double _d_psi;              ///< [rad] azimuth difference between adjacent blades

    double _beta_0;             ///< [rad] rotor coning angle
    double _beta_1c;            ///< [rad] longitudinal flapping angle
    double _beta_1s;            ///< [rad] lateral flapping angle

    double _theta_0;            ///< [rad] collective feathering angle
    double _theta_1c;           ///< [rad]
    double _theta_1s;           ///< [rad]

    double _coningAngle;        ///< [rad] rotor coning angle
    double _diskRoll;           ///< [rad] rotor disk roll angle
    double _diskPitch;          ///< [rad] rotor disk pitch angle

    double _ct;                 ///< [-] thrust coefficient
    double _cq;                 ///< [-] torque coefficient

    double _thrust;             ///< [N] rotor thrust
    double _torque;             ///< [N*m] rotor torque

    double _vel_i;              ///< [m/s] rotor induced velocity

    double _wakeSkew;           ///< [rad] rotor wake skew angle

    double _prev_azimuth;       ///< [rad] rotor azimuth position (previous value)

    double _prev_theta_0;       ///< [rad] collective feathering angle (previous value)
    double _prev_theta_1c;      ///< [rad] (previous value)
    double _prev_theta_1s;      ///< [rad] (previous value)

    /** */
    virtual void inducedVelcoity();
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_MAINROTORBE_H
