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
#ifndef FDM_STABILIZERVER_H
#define FDM_STABILIZERVER_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/utils/fdm_Vector3.h>
#include <fdm/xml/fdm_XmlNode.h>

#include <fdm/utils/fdm_Table1.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Vertical stabilizer base class.
 *
 * XML configuration file format:
 * @code
 * <stab_ver>
 *   <aerodynamic_center> { [m] x-coordinate } { [m] y-coordinate } { [m] z-coordinate } </aerodynamic_center>
 *   <area> { [m^2] area } </area>
 *   <cx>
 *     { [rad] angle } { [-] drag coefficient }
 *     ... { more entries }
 *   </cx>
 *   <cy>
 *     { [rad] angle } { [-] sideforce coefficient }
 *     ... { more entries }
 *   </cy>
 * </stab_ver>
 * @endcode
 */
class FDMEXPORT StabilizerVer
{
public:

    /**
     * @brief Constructor.
     * @param type stabilizer type
     */
    StabilizerVer();

    /** @brief Destructor. */
    virtual ~StabilizerVer();

    /**
     * @brief Reads data.
     * @param dataNode XML node
     */
    virtual void readData( XmlNode &dataNode );

    /**
     * @brief Computes force and moment.
     * @param vel_air_bas [m/s] aircraft linear velocity relative to the air expressed in BAS
     * @param omg_air_bas [rad/s] aircraft angular velocity relative to the air expressed in BAS
     * @param airDensity [kg/m^3] air density
     */
    virtual void computeForceAndMoment( const Vector3 &vel_air_bas,
                                        const Vector3 &omg_air_bas,
                                        double airDensity );

    inline const Vector3& getFor_BAS() const { return _for_bas; }
    inline const Vector3& getMom_BAS() const { return _mom_bas; }

protected:

    Vector3 _for_bas;           ///< [N] total force vector expressed in BAS
    Vector3 _mom_bas;           ///< [N*m] total moment vector expressed in BAS

    Vector3 _r_ac_bas;          ///< [m] stabilizer aerodynamic center expressed in BAS

    Table1 _cx;                 ///< [-] drag coefficient vs sideslip angle
    Table1 _cy;                 ///< [-] sideforce coefficient vs sideslip angle

    double _area;               ///< [m^2] stabilizer reference area

    /**
     * @brief Computes drag coefficient.
     * @param angle [rad] "angle of attack"
     * @return [-] drag coefficient
     */
    virtual double getCx( double angle ) const;

    /**
     * @brief Computes sideforce coefficient.
     * @param angle [rad] "angle of attack"
     * @return [-] sideforce coefficient
     */
    virtual double getCy( double angle ) const;
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_STABILIZERVER_H
