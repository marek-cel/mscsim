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
#ifndef FDM_ROTORBLADE_H
#define FDM_ROTORBLADE_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_Defines.h>

#include <fdm/utils/fdm_Matrix3x3.h>
#include <fdm/utils/fdm_Table1.h>
#include <fdm/utils/fdm_Vector3.h>

#include <fdm/xml/fdm_XmlNode.h>

#ifdef SIM_ROTOR_TEST
#   include <Data.h>
#endif

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Rotor blade model class.
 *
 * This is helicopter articulated rotor blade model class based on the blade
 * element theory. Blade is assumed to be rigid and uniform along its span.
 * Blade lead/lag motion is neglected.
 *
 * Flapping angle is positive upwards.
 *
 * Shaft-Rotating Axis (SRA)
 * Origin of the Shaft-Rotating Axis system is coincident with the rotor hub
 * center and rotates together with the rotor shaft. The z-axis is coincident
 * with the rotor shaft axis and points upwards, the x-axis points towards blade
 * trailing edge and the y-axis completes a right handed coordinate system.
 * Please notice that y-axis is positive toward blade tip for counter-clockwise
 * rotors while it is negative toward blade tip for clockwise rotors.
 *
 * Blade-Span Axis (BSA)
 * Origin of the Blade Axis System is coincident with the point of intersection
 * of flapping and feathering hinge axes. The x-axis lies on XY plane of the
 * Rotor Axis System and it is positive toward blade's trailing edge, z-axis is
 * positive in upward direction and the y-axis completes a right-handed
 * coordinate system.
 * Please notice that y-axis is positive toward blade tip for counter-clockwise
 * rotors while it is negative toward blade tip for clockwise rotors.
 *
 * XML configuration file format:
 * @code
 * <blade>
 *   <blade_mass> { [kg] blade mass } </blade_mass>
 *   <blade_length> { [m] blade length from flapping hinge to the tip } </blade_length>
 *   <blade_chord> { [m] blade chord } </blade_chord>
 *   <hinge_offset> { [m] flapping hinge offset from shaft axis } </hinge_offset>
 *   <beta_min> { [rad] minimum flapping angle } </beta_min>
 *   <beta_max> { [rad] maximum flapping angle } </beta_max>
 *   <twist>
*     { [m] spanwise coordinate } { [rad] twist angle }
 *     ... { more entries }
 *   </twist>
 *   <cd>
*     { [rad] angle of attack } { [-] drag coefficient }
 *     ... { more entries }
 *   </cd>
 *   <cl>
*     { [rad] angle of attack } { [-] lift coefficient }
 *     ... { more entries }
 *   </cl>
 * </blade>
 * @endcode
 */
class FDMEXPORT RotorBlade
{
public:

    typedef Vector< 2 > StateVector;

#   ifdef SIM_ROTOR_TEST
    struct Vect
    {
        bool visible;

        Vector3 b_sra;
        Vector3 v_sra;
    };

    Vect span[ VECT_SPAN ];
#   endif

    static Matrix3x3 getRAS2SRA( double psi, bool ccw = false );

    static Matrix3x3 getSRA2BSA( double beta, bool ccw = false );

    /** @brief Constructor. */
    RotorBlade( bool ccw = false );

    /** @brief Destructor. */
    virtual ~RotorBlade();

    /**
     * Reads data.
     * @param dataNode XML node
     */
    virtual void readData( XmlNode &dataNode );

#   ifdef SIM_ROTOR_TEST
    void TEST_INIT();
#   endif

    /**
     * @brief Compute forces and moments.
     * @param vel_air_ras [m/s]     rotor hub linear velocity relative to airflow expressed in RAS
     * @param omg_air_ras [rad/s]   rotor hub angular velocity relative to airflow expressed in RAS
     * @param omg_ras     [rad/s]   angular velocity expressed in RAS
     * @param acc_ras     [m/s^2]   rotor hub linear acceleration expressed in RAS
     * @param eps_ras     [rad/s^2] angular acceleration expressed in RAS
     * @param grav_ras    [m/s^2]   gravity acceleration vector expressed in RAS
     * @param omega       [rad/s]   rotor speed
     * @param azimuth     [rad]     blade azimuth
     * @param airDensity  [kg/m^3]  air density
     * @param theta_0     [rad]     collective feathering angle
     * @param theta_1c    [rad]     logitudinal feathering angle
     * @param theta_1s    [rad]     lateral feathering angle
     */
    virtual void computeForceAndMoment( const Vector3 &vel_air_ras,
                                        const Vector3 &omg_air_ras,
                                        const Vector3 &omg_ras,
                                        const Vector3 &acc_ras,
                                        const Vector3 &eps_ras,
                                        const Vector3 &grav_ras,
                                        double omega,
                                        double azimuth,
                                        double airDensity,
                                        double theta_0,
                                        double theta_1c,
                                        double theta_1s );

    /**
     * @brief Integrates blade model.
     * @param timeStep    [s]       time step
     * @param vel_air_ras [m/s]     rotor hub linear velocity relative to airflow expressed in RAS
     * @param omg_air_ras [rad/s]   rotor hub angular velocity relative to airflow expressed in RAS
     * @param omg_ras     [rad/s]   angular velocity expressed in RAS
     * @param acc_ras     [m/s^2]   rotor hub linear acceleration expressed in RAS
     * @param eps_ras     [rad/s^2] angular acceleration expressed in RAS
     * @param grav_ras    [m/s^2]   gravity acceleration vector expressed in RAS
     * @param omega       [rad/s]   rotor speed
     * @param azimuth     [rad]     blade azimuth
     * @param airDensity  [kg/m^3]  air density
     * @param theta_0     [rad]     collective feathering angle
     * @param theta_1c    [rad]     logitudinal feathering angle
     * @param theta_1s    [rad]     lateral feathering angle
     */
    virtual void integrate( double timeStep,
                            const Vector3 &vel_air_ras,
                            const Vector3 &omg_air_ras,
                            const Vector3 &omg_ras,
                            const Vector3 &acc_ras,
                            const Vector3 &eps_ras,
                            const Vector3 &grav_ras,
                            double omega,
                            double azimuth,
                            double airDensity,
                            double theta_0,
                            double theta_1c,
                            double theta_1s );

    inline const Vector3& getFor_RAS() const { return _for_ras; }
    inline const Vector3& getMom_RAS() const { return _mom_ras; }

    inline double getInertia() const { return _ib; }

    inline double getTorque() const { return _torque; }

    inline double getBeta()  const { return _beta; }
    inline double getTheta() const { return _theta; }

protected:

    const bool _ccw;            ///< specifies if rotor direction is counter clockwise

    const double _dirFactor;    ///< factor due to direction

    StateVector _stateVect;     ///< blade state vector
    StateVector _derivVect;     ///< blade state vector derivative

    Vector3 _for_ras;           ///< [N] total force vector expressed in RAS
    Vector3 _mom_ras;           ///< [N*m] total moment vector expressed in RAS

    Matrix3x3 _ras2sra;         ///< matrix of rotation from RAS to SRA
    Matrix3x3 _sra2ras;         ///< matrix of rotation from SRA to RAS

    //Matrix3x3 _sra2bsa;         ///< matrix of rotation from SRA to BSA
    //Matrix3x3 _bsa2sra;         ///< matrix of rotation from BSA to SRA

    Vector3 _pos_fh_sra;        ///< [m] flapping hinge coordinates expressed SRA

    Table1 _twist;              ///< [rad] spanwise blade twist vs spanwise coordinate

    Table1 _cd;                 ///< [-] blade section drag coefficient vs angle of attack
    Table1 _cl;                 ///< [-] blade section lift coefficient vs angle of attack

    double _m;                  ///< [kg] blage mass
    double _b;                  ///< [m] blade length
    double _c;                  ///< [m] blade chord
    double _e;                  ///< [m] flapping hinge offset

    double _beta_min;           ///< [rad] minimum flapping angle
    double _beta_max;           ///< [rad] maximum flapping angle

    double _sb;                 ///< [kg*m] blade first moment of mass about flapping hinge
    double _ib;                 ///< [kg*m^2] blade inertia moment about flapping hinge

    double _xforce;             ///< [N] force x component expressed in SRA
    double _yforce;             ///< [N] force y component expressed in SRA
    double _zforce;             ///< [N] force z component expressed in SRA
    double _torque;             ///< [N*m] torque
    double _moment;             ///< [N*m] total moment about flapping hinge (including aerodynamic, gravity and inertia moments)

    double &_beta;              ///< [rad] flapping angle
    double &_beta_dot;          ///< [rad/s] flapping angle derivative

    double _theta;              ///< [rad] feathering angle

    virtual void computeStateDeriv( const StateVector &stateVect,
                                    StateVector *derivVect );

    virtual double getTheta( double azimuth,
                             double theta_0,
                             double theta_1c,
                             double theta_1s );

    virtual void integrateEulerRect( double timeStep,
                                     const Vector3 &vel_air_ras,
                                     const Vector3 &omg_air_ras,
                                     const Vector3 &omg_ras,
                                     const Vector3 &acc_ras,
                                     const Vector3 &eps_ras,
                                     const Vector3 &grav_ras,
                                     double omega,
                                     double airDensity );

    virtual void integrateRungeKutta4( double timeStep,
                                       const Vector3 &vel_air_ras,
                                       const Vector3 &omg_air_ras,
                                       const Vector3 &omg_ras,
                                       const Vector3 &acc_ras,
                                       const Vector3 &eps_ras,
                                       const Vector3 &grav_ras,
                                       double omega,
                                       double airDensity );

    /**
     * @brief Integrates blade spanwise.
     * @param vel_air_ras [m/s]     rotor hub linear velocity relative to airflow expressed in RAS
     * @param omg_air_ras [rad/s]   rotor hub angular velocity relative to airflow expressed in RAS
     * @param omg_ras     [rad/s]   angular velocity expressed in RAS
     * @param acc_ras     [m/s^2]   rotor hub linear acceleration expressed in RAS
     * @param eps_ras     [rad/s^2] angular acceleration expressed in RAS
     * @param grav_ras    [m/s^2]   gravity acceleration vector expressed in RAS
     * @param omega       [rad/s]   rotor speed
     * @param airDensity  [kg/m^3]  air density
     */
    virtual void integrateSpanwise( const Vector3 &vel_air_ras,
                                    const Vector3 &omg_air_ras,
                                    const Vector3 &omg_ras,
                                    const Vector3 &acc_ras,
                                    const Vector3 &eps_ras,
                                    const Vector3 &grav_ras,
                                    double omega,
                                    double airDensity,
                                    double beta,
                                    double beta_dot );
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_ROTORBLADE_H
