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
#ifndef FDM_AERODYNAMICS_H
#define FDM_AERODYNAMICS_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_Base.h>

#include <fdm/utils/fdm_Matrix3x3.h>
#include <fdm/utils/fdm_Vector3.h>

#include <fdm/xml/fdm_XmlNode.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

class Aircraft; ///< aircraft class forward declaration

/**
 * @brief Aerodynamics model base class.
 *
 * <p>Aerodynamic Axes System (abbreviated as aero).</p>
 * <p>Stability Axes System (abbreviated as stab).</p>
 *
 * @see Stevens B., Lewis F.: Aircraft Control and Simulation, 1992
 */
class FDMEXPORT Aerodynamics : public Base
{
public:

    /**
     * Returns angle of attack.
     * @param vel_bas [m/s] airspeed vector
     * @param vel_min [m/s] minimum airspeed of calculations
     * @return [rad] angle of attack
     */
    static double getAngleOfAttack( const Vector3 &vel_bas, double vel_min = 1.0e-2 );

    /**
     * Returns sideslip angle.
     * It is positive when the aircraft velocity component along the transverse
     * axis is positive. [ISO 1151-1:1988]
     * @param vel_bas [m/s] airspeed vector
     * @param vel_min [m/s] minimum airspeed of calculations
     * @return [rad] sideslip angle
     */
    static double getSideslipAngle( const Vector3 &vel_bas, double vel_min = 1.0e-2 );

    /**
     * Returns Prandtl-Glauert coefficient.
     * @see https://en.wikipedia.org/wiki/Prandtl%E2%80%93Glauert_singularity
     * @param machNumber Mach number
     * @param max maximum value
     * @return Prandtl-Glauert coefficient
     */
    static double getPrandtlGlauertCoef( double machNumber, double max = 5.0 );

    /**
     * Returns rotation matrix from aerodynamic axes system to BAS.
     * @param alpha [rad] angle of attack cosine
     * @param beta [rad] sideslip angle cosine
     * @return rotation matrix from WAS to BAS
     */
    static Matrix3x3 getAero2BAS( double alpha, double beta );

    /**
     * Returns rotation matrix from aerodynamic axes system to BAS.
     * @param sinAlpha [-] sine of angle of attack cosine
     * @param cosAlpha [-] cosine of angle of attack cosine
     * @param sinBeta  [-] sine of sideslip angle cosine
     * @param cosBeta  [-] cosine of sideslip angle cosine
     * @return rotation matrix from WAS to BAS
     */
    static Matrix3x3 getAero2BAS( double sinAlpha , double cosAlpha,
                                  double sinBeta  , double cosBeta );

    /**
     * Returns rotation matrix from stability axes system to BAS.
     * @param alpha [rad] angle of attack cosine
     * @return rotation matrix from WAS to BAS
     */
    static Matrix3x3 getStab2BAS( double alpha );

    /**
     * Returns rotation matrix from stability axes system to BAS.
     * @param sinAlpha [-] sine of angle of attack cosine
     * @param cosAlpha [-] cosine of angle of attack cosine
     * @return rotation matrix from WAS to BAS
     */
    static Matrix3x3 getStab2BAS( double sinAlpha , double cosAlpha );

    /** Constructor. */
    Aerodynamics( const Aircraft* aircraft );

    /** Destructor. */
    virtual ~Aerodynamics();

    /** Initializes aerodynamics. */
    virtual void init();

    /**
     * Reads data.
     * @param dataNode XML node
     */
    virtual void readData( XmlNode &dataNode ) = 0;

    /** Computes force and moment. */
    virtual void computeForceAndMoment() = 0;

    /** Updates aerodynamics. */
    virtual void update();

    inline const Vector3& getFor_BAS() const { return _for_bas; }
    inline const Vector3& getMom_BAS() const { return _mom_bas; }

    inline const Vector3& getFor_aero() const { return _for_aero; }
    inline const Vector3& getMom_stab() const { return _mom_stab; }

    /**
     * Returns true if aircraft is stalling, otherwise returns false.
     * @return true if aircraft is stalling, false otherwise
     */
    virtual inline bool getStall() const { return false; }

protected:

    const Aircraft *_aircraft;  ///< aircraft model main object

    Vector3 _for_bas;           ///< [N] total force vector expressed in BAS
    Vector3 _mom_bas;           ///< [N*m] total moment vector expressed in BAS

    Vector3 _for_aero;          ///< [N] total force vector expressed in Aerodynamic Axes System
    Vector3 _mom_stab;          ///< [N*m] total moment vector expressed in Stability Axes System

    Matrix3x3 _aero2bas;        ///< rotation matrix from Aerodynamic Axes System to BAS
    Matrix3x3 _stab2bas;        ///< rotation matrix from Stability Axes System to BAS
    Matrix3x3 _bas2aero;        ///< rotation matrix from BAS to Aerodynamic Axes System
    Matrix3x3 _bas2stab;        ///< rotation matrix from BAS to Stability Axes System

    /**
     * Updates rotation matrices.
     */
    virtual void updateMatrices();

private:

    /** Using this constructor is forbidden. */
    Aerodynamics( const Aerodynamics & ) {}
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_AERODYNAMICS_H
