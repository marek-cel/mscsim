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
#ifndef FDM_WHEEL_H
#define FDM_WHEEL_H

////////////////////////////////////////////////////////////////////////////////

#include <vector>

#include <fdm/main/fdm_Module.h>

#include <fdm/utils/fdm_Vector3.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Landing gear wheel class.
 *
 * XML configuration file format:
 * @code
 * <wheel [steerable="{ 0|1 }"] [caster="{ 0|1 }"] [brake_group="{ 0|1|2 }]">
 *   <attachment_point> { [m] x-coordinate } { [m] y-coordinate } { [m] z-coordinate } </attachment_point>
 *   <unloaded_wheel> { [m] x-coordinate } { [m] y-coordinate } { [m] z-coordinate } </unloaded_wheel>
 *   <stiffness> { [N/m] strut stiffness (linear spring) coefficient } </stiffness>
 *   <damping> { [N/(m/s)] strut damping coefficient  } </damping>
 *   <friction_static> { [-] static friction coefficient } </friction_static>
 *   <friction_kinetic> { [-] kinetic friction coefficient } </friction_kinetic>
 *   <friction_rolling> { [-] rolling friction coefficient } </friction_rolling>
 *   [<max_angle> { [rad] max steering angle } </max_angle>]
 *   [<vel_break> { [m/s] static friction breakaway velocity } </vel_break>]
 * </wheel>
 * @endcode
 *
 * Optional elements: "max_angle", "vel_break"
 */
class FDMEXPORT Wheel
{
public:

    /** Brake group. */
    enum BrakeGroup
    {
        None = 0,               ///< none
        Left,                   ///< left brake group
        Right                   ///< right brake group
    };

    /** Constructor. */
    Wheel();

    /** Destructor. */
    virtual ~Wheel();

    /**
     * Reads data.
     * @param dataNode XML node
     */
    virtual void readData( XmlNode &dataNode );

    virtual void computeForce( const Vector3 &vel_bas,
                               const Vector3 &omg_bas,
                               const Vector3 &r_c_bas,
                               const Vector3 &n_c_bas,
                               bool steering, bool antiskid,
                               double surf_coef = 1.0 );

    virtual void update( double delta, double brake );

    inline Vector3 getRa_BAS() const { return _r_a_bas; }
    inline Vector3 getRu_BAS() const { return _r_u_bas; }

    inline BrakeGroup getBrakeGroup() const { return _brakeGroup; }

    inline void setInput( double *input ) { _input = input; }

protected:

    const double *_input;   ///< wheel input (0.0 - retracted, 1.0 - extended)

    Vector3 _for_bas;       ///< [N] total force vector expressed in BAS

    Vector3 _r_a_bas;       ///< [m] strut attachment point coordinates expressed in BAS
    Vector3 _r_u_bas;       ///< [m] unloaded wheel coordinates expressed in BAS

    double _k;              ///< [N/m] strut stiffness (linear spring) coefficient
    double _c;              ///< [N/(m/s)] strut damping coefficient

    double _mu_s;           ///< [-] coefficient of static friction
    double _mu_k;           ///< [-] coefficient of kinetic friction
    double _mu_r;           ///< [-] coefficient of rolling friction

    double _delta_max;      ///< [rad] max turn angle

    double _vel_break;      ///< [m/s] static friction breakaway velocity

    bool _steerable;        ///< specifies if wheel is steerable
    bool _caster;           ///< specifies if wheel is caster

    BrakeGroup _brakeGroup; ///< brake group

    double _delta;          ///< [rad] wheel turn angle
    double _brake;          ///< [0.0,1.0] normalized brake force
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_WHEEL_H
