/***************************************************************************//**
 *
 * @author Marek M. Cel <marekcel@marekcel.pl>
 *
 * @mainpage
 *
 * <h1>Conventions and Units</h1>
 *
 * <h2>Units</h2>
 * <p>Flight Dynamics Model uses International System of Units (SI) for all
 * internal computations.</p>
 * <p>Other units can be used in XML data files.</p>
 * <p><strong>Make sure to use "unit", "keys_unit", "cols_unit" or "rows_unit"
 * attribute when using non SI units in XML data file.</strong></p>
 * <p>
 * @see fdm::Units::getFactorSI()
 * @see fdm::XmlUtils
 * </p>
 *
 * <h2>Rotations</h2>
 * <p>Rotations angles are expressed as Bryant angles (Euler angles in z-y-z
 * convention).</p>
 * <p>All rotations and rotation related operations are considered to be
 * a passive (alias) rotations.</p>
 * <p>
 * @see https://en.wikipedia.org/wiki/Active_and_passive_transformation
 * </p>
 *
 * <h1>Coordinate Systems</h1>
 *
 * <h2>Body Axes System</h2>
 * <p>Abbreviated as BAS.</p>
 * <p>Body Axes System is the body-fixed coordinate system, with the x-axis
 * positive forwards, the y-axis positive right and z-axis positive
 * downwards.</p>
 *
 * <h2>North-East-Down</h2>
 * <p>Abbreviated as NED.</p>
 * <p>Local ground axes system with x-axis positive North, y-axis positive East
 * and z-axis positive Down.</p>
 *
 * <h2>World Geodetic System 1984</h2>
 * <p>Abbreviated as WGS.</p>
 * <p>World Geodetic System as described in [Department of Defense World
 * Geodetic System 1984. NIMA, Technical Report No. 8350.2, 2000].</p>
 *
 * @section LICENSE
 *
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
 *
 ******************************************************************************/
#ifndef FDM_AIRCRAFT_H
#define FDM_AIRCRAFT_H

////////////////////////////////////////////////////////////////////////////////

#include <fdmMain/fdm_DataManager.h>

#include <fdmMain/fdm_Environment.h>
#include <fdmMain/fdm_Intersections.h>

#include <fdmMain/fdm_Aerodynamics.h>
#include <fdmMain/fdm_Controls.h>
#include <fdmMain/fdm_LandingGear.h>
#include <fdmMain/fdm_Mass.h>
#include <fdmMain/fdm_Propulsion.h>

#include <fdmUtils/fdm_RungeKutta4.h>
#include <fdmUtils/fdm_WGS84.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Aircraft model base class.
 *
 * <h5>XML configuration file format:</h5>
 * @code
 * <fdm>
 *   <collision_points>
 *     <collision_point> { [m] x-coordinate } { [m] y-coordinate } { [m] z-coordinate } </collision_point>
 *     ... { more entries }
 *   </collision_points>
 *   <limitations>
 *     <airspeed_max> { [m/s] maximum airspeed (exceeding this value causes crash) } </airspeed_max>
 *     <load_aero_min> { [-] minimum (maximum negative) load factor due to aerodynamics (exceeding this value causes crash) } </load_aero_min>
 *     <load_aero_max> { [-] maximum (maximum positive) load factor due to aerodynamics (exceeding this value causes crash) } </load_aero_max>
 *     <load_gear_max> { [-] maximum absolute load factor due to landing gear (exceeding this value causes crash) } </load_gear_max>
 *   </limitations>
 *   <pilot_position> { [m] x-coordinate } { [m] y-coordinate } { [m] z-coordinate } </pilot_position>
 *   <aerodynamics>
 *     { aerodynamics data }
 *   </aerodynamics>
 *   <controls>
 *     { controls data }
 *   </controls>
 *   <landing_gear>
 *     { landing gear data }
 *   </landing_gear>
 *   <mass>
 *     { mass data }
 *   </mass>
 *   <propulsion>
 *     { propulsion data }
 *   </propulsion>
 * </fdm>
 * @endcode
 *
 * @see Sibilski K.: Modelowanie i symulacja dynamiki ruchu obiektow latajacych, 2004, p.35. [in Polish]
 * @see Narkiewicz J.: Tiltrotor Modelling for Simulation in Various Flight Conditions, 2006
 */
class FDMEXPORT Aircraft : public DataManager
{
public:

    typedef std::vector< Vector3 > CollisionPoints; ///< collision points

    /** Aircraft crash condition. */
    enum Crash
    {
        NoCrash = 0,    ///< no crash
        Collision,      ///< collision with terrain or obstacle
        Overspeed,      ///< airspeed too high
        Overstressed    ///< load factor too high
    };

    /**
     * Constructor.
     * @param envir environment interface object pointer
     * @param isect intersection interface object pointer
     */
    Aircraft();

    /** Destructor. */
    virtual ~Aircraft();

    /**
     * Updates aircraft due to simulation time step.
     * @param timeStep simulation time step [s]
     */
    virtual void step( double timeStep );

    inline Environment*   getEnvir() { return m_envir; }
    inline Intersections* getIsect() { return m_isect; }

    inline const Environment*   getEnvir() const { return m_envir; }
    inline const Intersections* getIsect() const { return m_isect; }

    inline Aerodynamics* getAero() { return m_aero; }
    inline Controls*     getCtrl() { return m_ctrl; }
    inline LandingGear*  getGear() { return m_gear; }
    inline Mass*         getMass() { return m_mass; }
    inline Propulsion*   getProp() { return m_prop; }

    inline const Aerodynamics* getAero() const { return m_aero; }
    inline const Controls*     getCtrl() const { return m_ctrl; }
    inline const LandingGear*  getGear() const { return m_gear; }
    inline const Mass*         getMass() const { return m_mass; }
    inline const Propulsion*   getProp() const { return m_prop; }

    inline const Vector3& getPosPilotBAS() const { return m_pos_pilot_bas; }

    inline const StateVector& getStateVect() const { return m_stateVect; }
    inline const StateVector& getDerivVect() const { return m_derivVect; }

    inline const Vector3&    getPos_WGS() const { return m_pos_wgs; }
    inline const Quaternion& getAtt_WGS() const { return m_att_wgs; }
    inline const Vector3&    getVel_BAS() const { return m_vel_bas; }
    inline const Vector3&    getOmg_BAS() const { return m_omg_bas; }

    inline const WGS84& getWGS() const { return m_wgs; }

    inline const Matrix3x3& getWGS2BAS() const { return m_wgs2bas; }
    inline const Matrix3x3& getBAS2WGS() const { return m_bas2wgs; }
    inline const Matrix3x3& getWGS2NED() const { return m_wgs2ned; }
    inline const Matrix3x3& getNED2WGS() const { return m_ned2wgs; }
    inline const Matrix3x3& getNED2BAS() const { return m_ned2bas; }
    inline const Matrix3x3& getBAS2NED() const { return m_bas2ned; }

    inline const Angles& getAngles_WGS() const { return m_angles_wgs; }
    inline const Angles& getAngles_NED() const { return m_angles_ned; }

    inline const Vector3& getVel_NED() const { return m_vel_ned; }

    inline const Vector3& getVel_air_BAS() const { return m_vel_air_bas; }
    inline const Vector3& getOmg_air_BAS() const { return m_omg_air_bas; }

    inline const Vector3& getAcc_BAS() const { return m_acc_bas; }
    inline const Vector3& getEps_BAS() const { return m_eps_bas; }

    inline const Vector3& getGrav_WGS() const { return m_grav_wgs; }
    inline const Vector3& getGrav_BAS() const { return m_grav_bas; }

    inline const Vector3& getGForce() const { return m_g_force; }
    inline const Vector3& getGPilot() const { return m_g_pilot; }

    inline const Vector3& getGround_WGS() const { return m_ground_wgs; }
    inline const Vector3& getGround_BAS() const { return m_ground_bas; }

    inline const Vector3& getNormal_WGS() const { return m_normal_wgs; }
    inline const Vector3& getNormal_BAS() const { return m_normal_bas; }

    inline Crash getCrash() const { return m_crash; }

    inline double getElevation()     const { return m_elevation;     }
    inline double getAltitude_ASL()  const { return m_altitude_asl;  }
    inline double getAltitude_AGL()  const { return m_altitude_agl;  }
    inline double getRoll()          const { return m_roll;          }
    inline double getPitch ()        const { return m_pitch;         }
    inline double getHeading()       const { return m_heading;       }
    inline double getAngleOfAttack() const { return m_angleOfAttack; }
    inline double getSideslipAngle() const { return m_sideslipAngle; }
    inline double getCourse()        const { return m_course;        }
    inline double getPathAngle()     const { return m_pathAngle;     }
    inline double getSlipSkidAngle() const { return m_slipSkidAngle; }
    inline double getAirspeed()      const { return m_airspeed;      }
    inline double getMachNumber()    const { return m_machNumber;    }
    inline double getClimbRate()     const { return m_climbRate;     }
    inline double getTurnRate()      const { return m_turnRate;      }

    /**
     * Sets aircraft state vector.
     * <p>This function is meant to set initial conditions at the beginning,
     * as well as to reposition aircraft during flight.</p>
     * @param state state vector
     */
    virtual void setStateVector( const StateVector &stateVector );

protected:

    /**
     * @brief Integrator wrapping class.
     * <p>fdm::Aircraft::computeStateDeriv(const StateVector &,StateVector &)
     * is right-hand-side function for integration procedure.</p>
     * <p>Because fdm::Aircraft::computeStateDeriv(const StateVector &,StateVector &)
     * is private fdm::Aircraft::Integrator is declared friend class for the
     * fdm::Aircraft class.</p>
     * <p>fdm::Aircraft::Integrator is declared private due to friendship with
     * the fdm::Aircraft to avoid possible access issues.</p>
     * <p>Because it is used entirely inside fdm::Aircraft, there is no need
     * to make it public.</p>
     */
    class Integrator : public RungeKutta4< FDM_STATE_DIMENSION, Aircraft >
    {
    public:

        Integrator( Aircraft *obj, void (Aircraft::*fun)(const StateVector &, StateVector &) ) :
            RungeKutta4( obj, fun ) {}
    };

    friend class Aircraft::Integrator;

    Environment   *m_envir;     ///< environment interface
    Intersections *m_isect;     ///< intersections interface

    DataNode *m_data;           ///< data tree root node

    Aerodynamics *m_aero;       ///< aerodynamics model
    Controls     *m_ctrl;       ///< controls model
    LandingGear  *m_gear;       ///< landing gear model
    Mass         *m_mass;       ///< mass and inertia model
    Propulsion   *m_prop;       ///< propulsion model

    CollisionPoints m_cp;       ///< [m] collision points expressed in BAS

    double m_airspeed_max;      ///< [m/s] maximum airspeed (exceeding this value causes crash)
    double m_load_aero_min;     ///< [-] minimum (maximum negative) load factor due to aerodynamics (exceeding this value causes crash)
    double m_load_aero_max;     ///< [-] maximum (maximum positive) load factor due to aerodynamics (exceeding this value causes crash)
    double m_load_gear_max;     ///< [-] maximum absolute load factor due to landing gear (exceeding this value causes crash)

    Vector3 m_pos_pilot_bas;    ///< [m] pilot's head position expressed in BAS

    StateVector m_stateVect;    ///< aircraft state vector
    StateVector m_statePrev;    ///< aircraft state vector (previous)
    StateVector m_derivVect;    ///< aircraft state vector derivative (for output purposes only)

    Integrator *m_integrator;   ///< integration procedure object

    double m_timeStep;          ///< [s] simulation time step

    Vector3    m_pos_wgs;       ///< [m] aircraft position expressed in WGS
    Quaternion m_att_wgs;       ///< aircraft attitude expressed as quaternion of rotation from WGS to BAS
    Vector3    m_vel_bas;       ///< [m/s] aircraft linear velocity vector expressed in BAS
    Vector3    m_omg_bas;       ///< [rad/s] aircraft angular velocity expressed in BAS

    WGS84 m_wgs;                ///< aircraft WGS position wrapper

    Matrix3x3 m_wgs2bas;        ///< matrix of rotation from WGS to BAS
    Matrix3x3 m_bas2wgs;        ///< matrix of rotation from BAS to WGS
    Matrix3x3 m_wgs2ned;        ///< matrix of rotation from WGS to NED
    Matrix3x3 m_ned2wgs;        ///< matrix of rotation from NED to WGS
    Matrix3x3 m_ned2bas;        ///< matrix of rotation from NED to BAS
    Matrix3x3 m_bas2ned;        ///< matrix of rotation from BAS to NED

    Angles m_angles_wgs;        ///< [rad] aircraft attitude expressed as rotation from WGS to BAS
    Angles m_angles_ned;        ///< [rad] aircraft attitude expressed as rotation from NED to BAS

    Vector3 m_vel_ned;          ///< [m/s] aircraft linear velocity vector expressed in NED

    Vector3 m_vel_air_bas;      ///< [m/s] aircraft linear velocity vector relative to the air expressed in BAS
    Vector3 m_omg_air_bas;      ///< [rad/s] aircraft angular velocity relative to the air expressed in BAS

    Vector3 m_acc_bas;          ///< [m/s^2] aircraft linear acceleration vector expressed in BAS
    Vector3 m_eps_bas;          ///< [rad/s^2] aircraft angular acceleration vector expressed in BAS

    Vector3 m_grav_wgs;         ///< [m/s^2] gravity acceleration vector expressed in WGS
    Vector3 m_grav_bas;         ///< [m/s^2] gravity acceleration vector expressed in BAS

    Vector3 m_g_force;          ///< [-] vector of G-Force factor (aircraft)
    Vector3 m_g_pilot;          ///< [-] vector of G-Force factor (pilot's head)

    Vector3 m_ground_wgs;       ///< [m] ground intersection coordinates expressed in WGS
    Vector3 m_ground_bas;       ///< [m] ground intersection coordinates expressed in BAS

    Vector3 m_normal_wgs;       ///< [-] normal to ground vector expressed in WGS
    Vector3 m_normal_bas;       ///< [-] normal to ground vector expressed in BAS

    Crash m_crash;              ///< crash cause

    double m_elevation;         ///< [m] ground elevation above mean sea level
    double m_altitude_asl;      ///< [m] altitude above sea level
    double m_altitude_agl;      ///< [m] altitude above ground level
    double m_roll;              ///< [rad] roll angle
    double m_pitch;             ///< [rad] pitch angle
    double m_heading;           ///< [rad] true heading
    double m_angleOfAttack;     ///< [rad] angle of attack
    double m_sideslipAngle;     ///< [rad] sideslip angle
    double m_course;            ///< [rad] velocity course
    double m_pathAngle;         ///< [rad] path angle
    double m_slipSkidAngle;     ///< [rad] slip/skid angle
    double m_airspeed;          ///< [m/s] true airspeed
    double m_machNumber;        ///< [-] Mach number
    double m_climbRate;         ///< [m/s] climb rate
    double m_turnRate;          ///< [rad/s] turn rate

    /**
     * Reads data.
     * @param dataFilePath XML data file path
     */
    virtual void readData( const std::string &dataFilePath );

    /**
     * This function is called just before time integration step.
     */
    virtual void anteIntegration();

    /**
     * This function integrates aircraft flight dynamics model.
     */
    virtual void integrate();

    /**
     * This function is called just after time integration step.
     */
    virtual void postIntegration();

    /**
     * This function checks collisions.
     */
    virtual void detectCrash();

    /**
     * Computes state vector derivatives due to given state vector.
     * @param stateVect state vector
     * @param derivVect resulting state vector derivative
     */
    virtual void computeStateDeriv( const StateVector &stateVect,
                                    StateVector &derivVect );

    /**
     * Updates aircraft state variables.
     * @param stateVect state vector
     * @param derivVect state vector derivative
     */
    virtual void updateVariables( const StateVector &stateVect,
                                  const StateVector &derivVect );

private:

    /** Using this constructor is forbidden. */
    Aircraft( const Aircraft & ) : DataManager() {}
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_AIRCRAFT_H
