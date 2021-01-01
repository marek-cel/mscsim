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
#ifndef FDM_WINCHLAUNCHER_H
#define FDM_WINCHLAUNCHER_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/utils/fdm_Matrix3x3.h>

#include <fdm/xml/fdm_XmlNode.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Sailplane winch launcher class.
 *
 * XML configuration file format:
 * @code
 * <winch_launcher>
 *   <attachment_point> { [m] x-coordinate } { [m] y-coordinate } { [m] z-coordinate } </attachment_point>
 *   <for_max> { [N] maximum cable force } </for_max>
 *   <len_max> { [m] maximum cable length } </len_max>
 *   <ang_max> { [rad] maximum cable angle } </ang_max>
 *   <vel_max> { [m/s] maximum cable velocity } </vel_max>
 *   <tc_for> { [s] force time constant } </tc_for>
 *   <tc_for> { [s] velocity time constant } </tc_for>
 *   <stiffnes> { [N/m^2] cable stiffness } </stiffnes>
 * </winch_launcher>
 * @endcode
 */
class FDMEXPORT WinchLauncher
{
public:

    /** @brief Constructor. */
    WinchLauncher();

    /** @brief Destructor. */
    virtual ~WinchLauncher();

    /**
     * @brief Reads data.
     * @param dataNode XML node
     */
    virtual void readData( XmlNode &dataNode );

    /**
     * @brief Computes force and moment.
     * @param wgs2bas matrix of rotation from WGS to BAS
     * @param pos_wgs [m] aircraft position expressed in WGS
     */
    virtual void computeForceAndMoment( const fdm::Matrix3x3 &wgs2bas,
                                        const Vector3 &pos_wgs );

    /**
     * @brief Update winch model.
     * @param timeStep [s] time step
     * @param bas2wgs matrix of rotation from BAS to WGS
     * @param wgs2ned matrix of rotation from WGS to NED
     * @param pos_wgs [m] aircraft position expressed in WGS
     * @param altitude_agl [m] altitude above ground level
     */
    virtual void update( double timeStep,
                         const fdm::Matrix3x3 &bas2wgs,
                         const fdm::Matrix3x3 &wgs2ned,
                         const Vector3 &pos_wgs,
                         double altitude_agl );

    inline const Vector3& getFor_BAS() const { return _for_bas; }
    inline const Vector3& getMom_BAS() const { return _mom_bas; }

protected:

    Vector3 _for_bas;       ///< [N] total force vector expressed in BAS
    Vector3 _mom_bas;       ///< [N*m] total moment vector expressed in BAS

    Vector3 _r_a_bas;       ///< [m] winch cable attachment point expressed in BAS

    Vector3 _pos_wgs;       ///< [m] winch position expressed in WGS

    double _for_max;        ///< [N]   maximum cable force
    double _len_max;        ///< [m]   maximum cable length
    double _ang_max;        ///< [rad] maximum elevation
    double _vel_max;        ///< [m/s] maximum cable velocity

    double _tc_for;         ///< [s] force time constant
    double _tc_vel;         ///< [s] velocity time constant

    double _stiffness;      ///< [N/m^2] cable stiffness

    double _for;            ///< [N]   current cable force
    double _vel;            ///< [m/s] current cable velocity
    double _len;            ///< [m]   current cable length

    bool _active;           ///< specify if winch is active
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_WINCHLAUNCHER_H
