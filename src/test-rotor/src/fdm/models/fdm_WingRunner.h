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
#ifndef FDM_WINGRUNNER_H
#define FDM_WINGRUNNER_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/utils/fdm_Vector3.h>

#include <fdm/xml/fdm_XmlNode.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Wing runner class.
 *
 * XML configuration file format:
 * @code
 * <wing_runner>
 *   <wing> { [m] x-coordinate } { [m] y-coordinate } { [m] z-coordinate } </wing>
 *   <feet> { [m] x-coordinate } { [m] y-coordinate } { [m] z-coordinate } </feet>
 *   <stiffness> { [N/m] stiffness (linear spring) coefficient } </stiffness>
 *   <damping> { [N/(m/s)] damping coefficient  } </damping>
 * </wing_runner>
 * @endcode
 */
class FDMEXPORT WingRunner
{
public:

    /** @brief Constructor. */
    WingRunner();

    /** @brief Destructor. */
    virtual ~WingRunner();

    /**
     * @brief Reads data.
     * @param dataNode XML node
     */
    virtual void readData( XmlNode &dataNode );

    /**
     * @brief Computes force and moment.
     * @param vel_bas [m/s] aircraft linear velocity expressed in BAS
     * @param omg_bas [rad/s] aircraft angular velocity expressed in BAS
     * @param r_c_bas [m] contact point coordinates expressed in BAS
     * @param n_c_bas [-] contact point normal vector expressed in BAS
     */
    virtual void computeForceAndMoment( const Vector3 &vel_bas,
                                        const Vector3 &omg_bas,
                                        const Vector3 &r_c_bas,
                                        const Vector3 &n_c_bas );

    /**
     * @brief Update wing runner model.
     * @param timeStep [s] time step
     */
    virtual void update( double timeStep, const Vector3 &vel_bas, bool onGround );

    inline const Vector3& getFor_BAS() const { return _for_bas; }
    inline const Vector3& getMom_BAS() const { return _mom_bas; }

    inline Vector3 getRw_BAS() const { return _r_w_bas; }
    inline Vector3 getRf_BAS() const { return _r_f_bas; }

protected:

    Vector3 _for_bas;       ///< [N] total force vector expressed in BAS
    Vector3 _mom_bas;       ///< [N*m] total moment vector expressed in BAS

    Vector3 _r_w_bas;       ///< [m] wing coordinates expressed in BAS
    Vector3 _r_f_bas;       ///< [m] feet coordinates expressed in BAS

    double _k;              ///< [N/m] stiffness (linear spring) coefficient
    double _c;              ///< [N/(m/s)] damping coefficient

    bool _active;           ///< specify if wing runner is active
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_WINGRUNNER_H
