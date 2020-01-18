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
#ifndef FDM_BLADE_H
#define FDM_BLADE_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_Defines.h>

#include <fdm/utils/fdm_Matrix3x3.h>
#include <fdm/utils/fdm_Table.h>
#include <fdm/utils/fdm_Vector3.h>

#include <fdm/xml/fdm_XmlNode.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Rotor blade model class.
 *
 * This model is based on blade element theory.
 *
 * <h4>Individual Blade Coordinates</h4>
 * <p>Abbreviated as IBC.</p>
 * <p>Origin of the Individual Blade Coordinates is coincident with the point of
 * intersection of flapping and feathering hinge axes, x-axis is coincident with
 * the blade chord and points towards trailing edge, y-axis is coincident with
 * the feathering hinge axis and it is positive towards blade tip, while the
 * z-axis completes a right-handed coordinate system.</p>
 *
 * <h3>XML configuration file format:</h3>
 * @code
 * @endcode
 */
class FDMEXPORT Blade
{
public:

    static Matrix3x3 getRAS2Psi( double psi );
    static Matrix3x3 getPsi2IBC( double beta );

    static Matrix3x3 getRAS2IBC( double psi, double beta );

    /** Constructor. */
    Blade();

    /** Destructor. */
    virtual ~Blade();

    /**
     * Reads data.
     * @param dataNode XML node
     */
    virtual void readData( XmlNode &dataNode );

    /**
     * Computes force and moment acting on the blade.
     * @param vel_air_ras [m/s] aircraft linear velocity relative to the air expressed in RAS
     * @param omg_air_ras [rad/s] aircraft angular velocity relative to the air expressed in RAS
     * @param omega [rad/s] rotor angular velocity vector due to rotation
     * @param azimuth [rad]
     * @param airDensity [kg/m^3] air density
     * @param theta_0 [rad]
     * @param theta_1c [rad]
     * @param theta_1s [rad]
     */
    virtual void computeForceAndMoment( const Vector3 &vel_bas,
                                        const Vector3 &omg_bas,
                                        const Vector3 &acc_bas,
                                        const Vector3 &eps_bas,
                                        const Vector3 &grav_bas,
                                        const Vector3 &vel_air_ras,
                                        const Vector3 &omg_air_ras,
                                        double omega,
                                        double azimuth,
                                        double airDensity,
                                        double theta_0,
                                        double theta_1c,
                                        double theta_1s );

    /**
     * Integrates blade.
     * @param timeStep [s]
     * @param vel_air_ras [m/s] aircraft linear velocity relative to the air expressed in RAS
     * @param omg_air_ras [rad/s] aircraft angular velocity relative to the air expressed in RAS
     * @param omega [rad/s] rotor angular velocity vector due to rotation
     * @param azimuth [rad]
     * @param airDensity [kg/m^3] air density
     * @param theta_0 [rad]
     * @param theta_1c [rad]
     * @param theta_1s [rad]
     */
    virtual void integrate( double timeStep,
                            const Vector3 &vel_bas,
                            const Vector3 &omg_bas,
                            const Vector3 &acc_bas,
                            const Vector3 &eps_bas,
                            const Vector3 &grav_bas,
                            const Vector3 &vel_air_ras,
                            const Vector3 &omg_air_ras,
                            double omega,
                            double azimuth,
                            double airDensity,
                            double theta_0,
                            double theta_1c,
                            double theta_1s );

    inline double getInertia() const { return _ib; }

    inline double getThrust() const { return  _thrust; }
    inline double getHForce() const { return  _hforce; }
    inline double getTorque() const { return  _torque; }
    inline double getMoment() const { return  _moment; }

protected:

    Matrix3x3 _ras2ibc;         ///< matrix of rotation from RAS to IBC

    Table _cd;                  ///< [-] blade section drag coefficient vs angle of attack
    Table _cl;                  ///< [-] blade section lift coefficient vs angle of attack

    double _b;                  ///< [m] blade span
    double _c;                  ///< [m] blade chord
    double _e;                  ///< [m] flapping hinge offset
    double _m;                  ///< [kg] blage mass

    double _beta_max;           ///< [rad] maximum flapping angle

    double _twist_0;            ///< [rad] blade twist at attachment
    double _twist_rate;         ///< [rad/m] blade twist angle rate

    double _sb;                 ///< [kg*m] blade first moment of mass about flapping hinge
    double _ib;                 ///< [kg*m^2] blade inertia moment about flapping hinge

    double _thrust;             ///< [N] thrust
    double _hforce;             ///< [N] H force
    double _yforce;             ///< [N] Y force
    double _torque;             ///< [N*m] torque
    double _moment;             ///< [N*m] total moment about flapping hinge (including aerodynamic, gravity and inertia moments)

    double _beta_0;             ///< [rad] current flapping angle
    double _beta_1;             ///< [rad/s] flapping angle time derivative
    double _beta_2;             ///< [rad/s^2] flapping angle second time derivative

};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_BLADE_H
