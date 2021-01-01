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
#ifndef UH60_FUSELAGE_H
#define UH60_FUSELAGE_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/utils/fdm_Table1.h>
#include <fdm/utils/fdm_Table2.h>
#include <fdm/utils/fdm_Vector3.h>

#include <fdm/xml/fdm_XmlNode.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief UH-60 fuselage class.
 *
 * Force coefficient are given as force devided by dynamic pressure.
 * drag       : C_D = D / q
 * side force : C_Y = Y / q
 * lift       : C_L = L / q
 *
 * Moment coefficient are given as moment devided by dynamic pressure.
 * rolling moment  : C_R = R / q
 * pitching moment : C_M = M / q
 * yawing moment   : C_N = N / q
 *
 * XML configuration file format:
 * @code
 * <fuselage>
 *   <aero_center> { [m] x-coordinate } { [m] y-coordinate } { [m] z-coordinate } </aero_center>
 *   <ekxwf>
 *     { [rad] longitudinal flapping angle } ... { more values }
 *     { [rad] rotor wake skew angle } { [-] rotor wash interference factor (inplane) } ... { more values }
 *     ... { more entries }
 *   </ekxwf>
 *   <ekzwf>
 *     { [rad] longitudinal flapping angle } ... { more values }
 *     { [rad] rotor wake skew angle } { [-] rotor wash interference factor (downwash) } ... { more values }
 *     ... { more entries }
 *   </ekzwf>
 *   <dqfmp>
 *     { [rad] angle of attack } { [m^2] drag coefficient }
 *     ... { more entries }
 *   </dqfmp>
 *   <lqfmp>
 *     { [rad] angle of attack } { [m^2] lift coefficient }
 *     ... { more entries }
 *   </lqfmp>
 *   <mqfmp>
 *     { [rad] angle of attack } { [m^3] pitching moment coefficient }
 *     ... { more entries }
 *   </mqfmp>
 *   <yqfmp>
 *     { [rad] angle of sideslip } { [m^2] sideforce coefficient }
 *     ... { more entries }
 *   </yqfmp>
 *   <rqfmp>
 *     { [rad] angle of sideslip } { [m^3] rolling moment coefficient }
 *     ... { more entries }
 *   </rqfmp>
 *   <nqfmp>
 *     { [rad] angle of sideslip } { [m^3] yawing moment coefficient }
 *     ... { more entries }
 *   </nqfmp>
 *   <ddqfmp>
 *     { [rad] angle of sideslip } { [m^2] incremental drag coefficient }
 *     ... { more entries }
 *   </ddqfmp>
 *   <dlqfmp>
 *     { [rad] angle of sideslip } { [m^2] incremental lift coefficient }
 *     ... { more entries }
 *   </dlqfmp>
 *   <dmqfmp>
 *     { [rad] angle of sideslip } { [m^3] incremental pitching moment coefficient }
 *     ... { more entries }
 *   </dmqfmp>
 * </fuselage>
 * @endcode
 *
 * @see Howlett J.: UH-60A Black Hawk Engineering Simulation Program. NASA, CR-166309, 1981
 */
class UH60_Fuselage
{
public:

    /** Constructor. */
    UH60_Fuselage();

    /** Destructor. */
    virtual ~UH60_Fuselage();

    /**
     * Reads data.
     * @param dataNode XML node
     */
    void readData( XmlNode &dataNode );

    /**
     * Computes force and moment.
     * @param vel_air_bas [m/s] aircraft linear velocity relative to the air expressed in BAS
     * @param omg_air_bas [rad/s] aircraft angular velocity relative to the air expressed in BAS
     * @param airDensity [kg/m^3] air density
     * @param a1fmr [rad] longitudinal flapping angle
     * @param chipmr [rad] rotor wake skew angle
     * @param dwshmr [-] main rotor uniform downwash
     * @param omgtmr [rad/s] rotor speed
     * @param rmr [m] rotor radius
     */
    virtual void computeForceAndMoment( const Vector3 &vel_air_bas,
                                        const Vector3 &omg_air_bas,
                                        double airDensity,
                                        double a1fmr,
                                        double chipmr,
                                        double dwshmr,
                                        double omgtmr,
                                        double rmr );

    inline const Vector3& getFor_BAS() const { return _for_bas; }
    inline const Vector3& getMom_BAS() const { return _mom_bas; }

private:

    Vector3 _for_bas;           ///< [N] total force vector expressed in BAS
    Vector3 _mom_bas;           ///< [N*m] total moment vector expressed in BAS

    Vector3 _r_ac_bas;          ///< [m] fuselage aerodynamic center expressed in BAS

    Table2 _ekxwf;              ///< [-] rotor wash interference factor (inplane)
    Table2 _ekzwf;              ///< [-] rotor wash interference factor (downwash)

    Table1 _dqfmp;              ///< [m^2] drag coefficient due to angle of attack
    Table1 _lqfmp;              ///< [m^2] lift coefficient vs [rad] angle of attack
    Table1 _mqfmp;              ///< [m^3] pitching moment coefficient vs [rad] angle of attack

    Table1 _yqfmp;              ///< [m^2] sideforce coefficient vs [rad] angle of sideslip
    Table1 _rqfmp;              ///< [m^3] rolling moment coefficient vs [rad] angle of sideslip
    Table1 _nqfmp;              ///< [m^3] yawing moment coefficient vs [rad] angle of sideslip

    Table1 _ddqfmp;             ///< [m^2] incremental drag coefficient vs [rad] angle of sideslip
    Table1 _dlqfmp;             ///< [m^2] incremental lift coefficient vs [rad] angle of sideslip
    Table1 _dmqfmp;             ///< [m^3] incremental pitching moment coefficient vs [rad] angle of sideslip

    /**
     * Computes drag coefficient.
     * @param alfwf [rad] body axes angle of attack
     * @param psiwf [rad] W/T model yaw angle
     * @return [-] drag coefficient
     */
    virtual double getDQFTOT( double alfwf, double psiwf ) const;

    /**
     * Computes sideforce coefficient.
     * @param psiwf [rad] W/T model yaw angle
     * @return [-] sideforce coefficient
     */
    virtual double getYQFTOT( double psiwf ) const;

    /**
     * Computes lift coefficient.
     * @param alfwf [rad] body axes angle of attack
     * @param psiwf [rad] W/T model yaw angle
     * @return [-] lift coefficient
     */
    virtual double getLQFTOT( double alfwf, double psiwf ) const;

    /**
     * Computes rolling moment coefficient.
     * @param psiwf [rad] W/T model yaw angle
     * @return [-] rolling moment coefficient
     */
    virtual double getRQFTOT( double psiwf ) const;

    /**
     * Computes pitching moment coefficient.
     * @param alfwf [rad] body axes angle of attack
     * @param psiwf [rad] W/T model yaw angle
     * @return [-] pitching moment coefficient
     */
    virtual double getMQFTOT( double alfwf, double psiwf ) const;

    /**
     * Computes yawing moment coefficient.
     * @param psiwf [rad] W/T model yaw angle
     * @return [-] yawing moment coefficient
     */
    virtual double getNQFTOT( double psiwf ) const;
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // UH60_FUSELAGE_H
