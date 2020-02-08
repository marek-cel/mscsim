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
 * Flapping angle is positive upwards.
 *
 * Shaft-Rotating Axis (SRA)
 * Each rotor blade has its own Shaft-Rotating Axis system. Origin of the
 * Shaft-Rotating Axis system is coincident with the rotor hub center and
 * rotates together with the rotor. The x-axis lies on XY plane of the Rotor
 * Axis System and points towards blade trailing edge, the y-axis lies on XY
 * plane of the Rotor Axis System and points outwards, the z-axis it is
 * coincident with the rotor shaft axis and points upwards.
 *
 * Blade-Span Axis (BSA)
 *
 * XML configuration file format:
 * @code
 * @endcode
 */
class FDMEXPORT Blade
{
public:

    static Matrix3x3 getRAS2SRA( double psi );

    /** Constructor. */
    Blade();

    /** Destructor. */
    virtual ~Blade();

    /**
     * Reads data.
     * @param dataNode XML node
     */
    virtual void readData( XmlNode &dataNode );

    void TEST_INIT();

    /**
     * @brief Updates blade model.
     * @param timeStep [s] time step
     */
    virtual void update( double timeStep );

    inline double getInertia() const { return _ib; }

    inline double getThrust() const { return  _thrust; }
    inline double getHForce() const { return  _hforce; }
    inline double getYForce() const { return  _yforce; }
    inline double getTorque() const { return  _torque; }
    inline double getMoment() const { return  _moment; }

    inline double getBeta()  const { return _beta; }
    inline double getTheta() const { return _theta; }

protected:

    Matrix3x3 _ras2sra;         ///< matrix of rotation from RAS to SRA
    Matrix3x3 _sra2ras;         ///< matrix of rotation from SRA to RAS

    Table _twist;               ///< [rad] spanwise blade twist

    Table _cd;                  ///< [-] blade section drag coefficient vs angle of attack
    Table _cl;                  ///< [-] blade section lift coefficient vs angle of attack

    double _r;                  ///< [m] main rotor radius
    double _c;                  ///< [m] blade chord
    double _e;                  ///< [m] flapping hinge offset
    double _m;                  ///< [kg] blage mass

    double _beta_min;           ///< [rad] minimum flapping angle
    double _beta_max;           ///< [rad] maximum flapping angle

    double _b;                  ///< [m] blade length

    double _sb;                 ///< [kg*m] blade first moment of mass about flapping hinge
    double _ib;                 ///< [kg*m^2] blade inertia moment about flapping hinge

    double _thrust;             ///< [N] thrust
    double _hforce;             ///< [N] H force
    double _yforce;             ///< [N] Y force
    double _torque;             ///< [N*m] torque
    double _moment;             ///< [N*m] total moment about flapping hinge (including aerodynamic, gravity and inertia moments)

    double _beta_0;             ///< [rad] current flapping angle (positive upwards)
    double _beta_1;             ///< [rad/s] flapping angle time derivative
    double _beta_2;             ///< [rad/s^2] flapping angle second time derivative

    double &_beta;              ///< _beta_0 alias

    double _theta;              ///< [rad] feathering angle
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_BLADE_H
