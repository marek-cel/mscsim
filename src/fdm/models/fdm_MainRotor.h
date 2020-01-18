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
 * @brief Helicopter main rotormodel base  class.
 *
 * <h3>XML configuration file format:</h3>
 * @code
 * <main_rotor>
 *   <hub_center> { [m] x-coordinate } { [m] y-coordinate } { [m] z-coordinate } </hub_center>
 *   <inclination> { [rad] rotor inclination angle (positive if forward) } </inclination>
 *   <number_of_blades> { number of blades } </number_of_blades>
 * </main_rotor>
 * @endcode
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
     * @brief Updates main rotor model.
     * @param omega [rad/s] rotor revolution speed
     * @param azimuth [rad] rotor azimuth
     */
    virtual void update( double omega, double azimuth );

    /**
     * Returns rotor total inartia about shaft axis.
     * @return [kg*m^2] rotor total inartia about shaft axis
     */
    virtual double getInertia() const = 0;

    inline const Vector3& getFor_BAS() const { return _for_bas; }
    inline const Vector3& getMom_BAS() const { return _mom_bas; }

    inline double getConingAngle() const { return _coningAngle; }
    inline double getDiskRoll()    const { return _diskRoll;    }
    inline double getDiskPitch()   const { return _diskPitch;   }

    inline const Vector3& getVel_i_BAS() const { return _vel_i_bas; }

    inline double getThrust() const { return _thrust; }
    inline double getTorque() const { return _torque; }

protected:

    Vector3 _for_bas;           ///< [N] total force vector expressed in BAS
    Vector3 _mom_bas;           ///< [N*m] total moment vector expressed in BAS

    Direction _direction;       ///< rotor rotation direction (clockwise or counter-clockwise)

    Vector3 _r_hub_bas;         ///< [m] rotor hub coordinates expressed in BAS

    Matrix3x3 _bas2ras;         ///< matrix of rotation from BAS to RAS
    Matrix3x3 _ras2bas;         ///< matrix of rotation from RAS to BAS

    Vector3 _vel_i_bas;         ///< [m/s] induced velocity expressed in BAS

    int _nb;                    ///< number of rotor blades

    double _omega;              ///< [rad/s] rotor revolution speed
    double _azimuth;            ///< [rad] rotor azimuth

    double _coningAngle;        ///< [rad] rotor coning angle
    double _diskRoll;           ///< [rad] rotor disk roll angle
    double _diskPitch;          ///< [rad] rotor disk pitch angle

    double _thrust;             ///< [N] rotor thrust
    double _torque;             ///< [N*m] rotor torque
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_MAINROTOR_H
