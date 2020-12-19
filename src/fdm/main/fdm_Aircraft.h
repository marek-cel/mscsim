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
#ifndef FDM_AIRCRAFT_H
#define FDM_AIRCRAFT_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_DataInp.h>
#include <fdm/fdm_DataOut.h>

#include <fdm/fdm_Base.h>

#include <fdm/main/fdm_Environment.h>
#include <fdm/main/fdm_Intersections.h>

#include <fdm/main/fdm_Input.h>

#include <fdm/main/fdm_Aerodynamics.h>
#include <fdm/main/fdm_Controls.h>
#include <fdm/main/fdm_LandingGear.h>
#include <fdm/main/fdm_Mass.h>
#include <fdm/main/fdm_Propulsion.h>

#include <fdm/utils/fdm_RungeKutta4.h>
#include <fdm/utils/fdm_WGS84.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Aircraft model base class.
 *
 * Aircraft flight dynamics model base class.
 *
 * @section Conventions and Units
 *
 * Units:
 * Flight Dynamics Model uses International System of Units (SI) for all
 * internal computations.
 * Other units can be used in XML data files.
 * Make sure to use "unit", "keys_unit", "cols_unit" or "rows_unit"
 * attribute when using non SI units in XML data file.
 *
 * @see fdm::Units::getConverter()
 * @see fdm::XmlUtils
 *
 * @subsection Rotations
 *
 * Rotations angles are expressed as Bryant angles (Euler angles in z-y-x
 * convention).
 * All rotations and rotation related operations are considered to be
 * passive (alias) rotations.
 *
 * @see https://en.wikipedia.org/wiki/Active_and_passive_transformation
 *
 * @subsection Coordinate Systems
 *
 * Body Axis System (BAS)
 * Body Axis System is the body-fixed coordinate system, with the x-axis
 * positive forwards, the y-axis positive right and z-axis positive
 * downwards.
 *
 * North-East-Down (NED)
 * Local ground axis system with x-axis positive North, y-axis positive East
 * and z-axis positive Down.
 *
 * World Geodetic System 1984 (WGS)
 * World Geodetic System as described in [Department of Defense World
 * Geodetic System 1984. NIMA, Technical Report No. 8350.2, 2000].
 *
 * @subsection XML configuration file format
 *
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
 * @see Taylor J.: Classical Mechanics, 2005
 * @see Osinski Z.: Mechanika ogolna, 1997, [in Polish]
 * @see Allerton D.: Principles of Flight Simulation, 2009
 * @see Stevens B., Lewis F.: Aircraft Control and Simulation, 1992
 * @see Maryniak J.: Ogolny model matematyczny sterowanego samolotu, 1992 [in Polish]
 * @see Sibilski K.: Modelowanie i symulacja dynamiki ruchu obiektow latajacych, 2004 [in Polish]
 * @see Narkiewicz J.: Tiltrotor Modelling for Simulation in Various Flight Conditions, 2006
 * @see Zlocka M.: Wyklady z dynamiki lotu, 2008 [in Polish]
 * @see https://en.wikipedia.org/wiki/Rotating_reference_frame
 * @see https://en.wikipedia.org/wiki/Centrifugal_force#Derivation
 */
class FDMEXPORT Aircraft : public Base
{
public:

    typedef std::vector< Vector3 > CollisionPoints; ///< collision points
    typedef VectorN StateVector;                    ///< state vector

    /** Propuslion state enum. */
    enum PropState
    {
        Stopped  = 0,   ///< stopped
        Running  = 1    ///< running
    };

    static const UInt8 _i_x;    ///< index of aircraft location x-coordinate (origin of BAS axis system) expressed in WGS axis system
    static const UInt8 _i_y;    ///< index of aircraft location y-coordinate (origin of BAS axis system) expressed in WGS axis system
    static const UInt8 _i_z;    ///< index of aircraft location z-coordinate (origin of BAS axis system) expressed in WGS axis system
    static const UInt8 _i_e0;   ///< index of aircraft attitude quaternion e0-coordinate
    static const UInt8 _i_ex;   ///< index of aircraft attitude quaternion ex-coordinate
    static const UInt8 _i_ey;   ///< index of aircraft attitude quaternion ey-coordinate
    static const UInt8 _i_ez;   ///< index of aircraft attitude quaternion ez-coordinate
    static const UInt8 _i_u;    ///< index of aircraft linear velocity x-coordinate expressed in BAS axis system
    static const UInt8 _i_v;    ///< index of aircraft linear velocity y-coordinate expressed in BAS axis system
    static const UInt8 _i_w;    ///< index of aircraft linear velocity z-coordinate expressed in BAS axis system
    static const UInt8 _i_p;    ///< index of aircraft angular velocity x-coordinate expressed in BAS axis system
    static const UInt8 _i_q;    ///< index of aircraft angular velocity y-coordinate expressed in BAS axis system
    static const UInt8 _i_r;    ///< index of aircraft angular velocity z-coordinate expressed in BAS axis system

    /**
     * @brief Class destructor.
     * @param input
     */
    Aircraft( Input *input );

    /** @brief Aircraft class destructor. */
    virtual ~Aircraft();

    /**
     * @brief Initializes aircraft.
     * @param engineOn specifies if engine is running on startup
     */
    virtual void initialize( bool engineOn = false );

    /**
     * @brief Updates aircraft due to simulation time step.
     * @param timeStep simulation time step [s]
     * @param integrate specifies integration is enabled
     */
    virtual void update( double timeStep, bool integrate = true );

    inline Environment*   getEnvir() { return _envir; }
    inline Intersections* getIsect() { return _isect; }

    inline const Environment*   getEnvir() const { return _envir; }
    inline const Intersections* getIsect() const { return _isect; }

    virtual inline const Aerodynamics* getAero() const { return _aero; }
    virtual inline const Controls*     getCtrl() const { return _ctrl; }
    virtual inline const LandingGear*  getGear() const { return _gear; }
    virtual inline const Mass*         getMass() const { return _mass; }
    virtual inline const Propulsion*   getProp() const { return _prop; }

    inline const Vector3& getPosPilotBAS() const { return _pos_pilot_bas; }

    inline StateVector& getStateVect() { return _stateVect; }

    inline const StateVector& getStateVect() const { return _stateVect; }
    inline const StateVector& getDerivVect() const { return _derivVect; }

    inline double getTimeStep() const { return _timeStep; }

    inline const Vector3&    getPos_WGS() const { return _pos_wgs; }
    inline const Quaternion& getAtt_WGS() const { return _att_wgs; }
    inline const Vector3&    getVel_BAS() const { return _vel_bas; }
    inline const Vector3&    getOmg_BAS() const { return _omg_bas; }

    inline const WGS84& getWGS() const { return _wgs; }

    inline const Matrix3x3& getWGS2BAS() const { return _wgs2bas; }
    inline const Matrix3x3& getBAS2WGS() const { return _bas2wgs; }
    inline const Matrix3x3& getWGS2NED() const { return _wgs2ned; }
    inline const Matrix3x3& getNED2WGS() const { return _ned2wgs; }
    inline const Matrix3x3& getNED2BAS() const { return _ned2bas; }
    inline const Matrix3x3& getBAS2NED() const { return _bas2ned; }

    inline const Angles& getAngles_WGS() const { return _angles_wgs; }
    inline const Angles& getAngles_NED() const { return _angles_ned; }

    inline const Vector3& getVel_NED() const { return _vel_ned; }

    inline const Vector3& getVel_air_BAS() const { return _vel_air_bas; }
    inline const Vector3& getOmg_air_BAS() const { return _omg_air_bas; }

    inline const Vector3& getAcc_BAS() const { return _acc_bas; }
    inline const Vector3& getEps_BAS() const { return _eps_bas; }

    inline const Vector3& getGrav_WGS() const { return _grav_wgs; }
    inline const Vector3& getGrav_BAS() const { return _grav_bas; }

    inline const Vector3& getGForce() const { return _g_force; }
    inline const Vector3& getGPilot() const { return _g_pilot; }

    inline const Vector3& getGround_WGS() const { return _ground_wgs; }
    inline const Vector3& getGround_BAS() const { return _ground_bas; }

    inline const Vector3& getNormal_WGS() const { return _normal_wgs; }
    inline const Vector3& getNormal_BAS() const { return _normal_bas; }

    inline DataOut::Crash getCrash() const { return _crash; }

    inline double getElevation()     const { return _elevation;     }
    inline double getAltitude_ASL()  const { return _altitude_asl;  }
    inline double getAltitude_AGL()  const { return _altitude_agl;  }
    inline double getRoll()          const { return _roll;          }
    inline double getPitch ()        const { return _pitch;         }
    inline double getHeading()       const { return _heading;       }
    inline double getAngleOfAttack() const { return _angleOfAttack; }
    inline double getSideslipAngle() const { return _sideslipAngle; }
    inline double getClimbAngle()    const { return _climbAngle;    }
    inline double getTrackAngle()    const { return _trackAngle;    }
    inline double getSlipSkidAngle() const { return _slipSkidAngle; }
    inline double getAirspeed()      const { return _airspeed;      }
    inline double getIAS()           const { return _ias;           }
    inline double getTAS()           const { return _tas;           }
    inline double getGroundSpeed()   const { return _groundSpeed;   }
    inline double getDynPress()      const { return _dynPress;      }
    inline double getMachNumber()    const { return _machNumber;    }
    inline double getClimbRate()     const { return _climbRate;     }
    inline double getTurnRate()      const { return _turnRate;      }

    inline PropState getInitPropState() const { return _initPropState; }

    /**
     * @brief Sets aircraft state vector.
     * This function is meant to set initial conditions at the beginning,
     * as well as to reposition aircraft during flight.
     * @param state state vector
     */
    virtual void setStateVector( const StateVector &stateVector );

    inline void setFreezePosition( bool freeze_position ) { _freeze_position = freeze_position; }
    inline void setFreezeAttitude( bool freeze_attitude ) { _freeze_attitude = freeze_attitude; }
    inline void setFreezeVelocity( bool freeze_velocity ) { _freeze_velocity = freeze_velocity; }

protected:

    /**
     * @brief Integrator wrapping class.
     * fdm::Aircraft::computeStateDeriv(const StateVector &,StateVector &)
     * is right-hand-side function for integration procedure.
     * Because fdm::Aircraft::computeStateDeriv(const StateVector &,StateVector &)
     * is private fdm::Aircraft::Integrator is declared friend class for the
     * fdm::Aircraft class.
     * fdm::Aircraft::Integrator is declared private due to friendship with
     * the fdm::Aircraft to avoid possible access issues.
     * Because it is used entirely inside fdm::Aircraft, there is no need
     * to make it public.
     */
    class Integrator : public RungeKutta4< Aircraft >
    {
    public:

        Integrator( Aircraft *obj, void (Aircraft::*fun)(const StateVector &, StateVector *) ) :
            RungeKutta4( obj, fun ) {}
    };

    friend class Aircraft::Integrator;

    Input *_input;              ///< input data tree root node

    Environment   *_envir;      ///< environment interface
    Intersections *_isect;      ///< intersections interface

    Aerodynamics *_aero;        ///< aerodynamics model
    Controls     *_ctrl;        ///< controls model
    LandingGear  *_gear;        ///< landing gear model
    Mass         *_mass;        ///< mass and inertia model
    Propulsion   *_prop;        ///< propulsion model

    CollisionPoints _cp;        ///< [m] collision points expressed in BAS

    double _airspeed_max;       ///< [m/s] maximum indicated airspeed (exceeding this value causes crash)
    double _load_aero_min;      ///< [-] minimum (maximum negative) load factor due to aerodynamics (exceeding this value causes crash)
    double _load_aero_max;      ///< [-] maximum (maximum positive) load factor due to aerodynamics (exceeding this value causes crash)
    double _load_gear_max;      ///< [-] maximum absolute load factor due to landing gear (exceeding this value causes crash)

    Vector3 _pos_pilot_bas;     ///< [m] pilot's head position expressed in BAS

    StateVector _stateVect;     ///< aircraft state vector
    StateVector _statePrev;     ///< aircraft state vector (previous)
    StateVector _derivVect;     ///< aircraft state vector derivative (for output purposes only)

    Integrator *_integrator;    ///< integration procedure object

    double _timeStep;           ///< [s] simulation time step

    Vector3    _pos_wgs;        ///< [m] aircraft position expressed in WGS
    Quaternion _att_wgs;        ///< aircraft attitude expressed as quaternion of rotation from WGS to BAS
    Vector3    _vel_bas;        ///< [m/s] aircraft linear velocity vector expressed in BAS
    Vector3    _omg_bas;        ///< [rad/s] aircraft angular velocity expressed in BAS

    WGS84 _wgs;                 ///< aircraft WGS position wrapper

    Matrix3x3 _wgs2bas;         ///< matrix of rotation from WGS to BAS
    Matrix3x3 _bas2wgs;         ///< matrix of rotation from BAS to WGS
    Matrix3x3 _wgs2ned;         ///< matrix of rotation from WGS to NED
    Matrix3x3 _ned2wgs;         ///< matrix of rotation from NED to WGS
    Matrix3x3 _ned2bas;         ///< matrix of rotation from NED to BAS
    Matrix3x3 _bas2ned;         ///< matrix of rotation from BAS to NED

    Angles _angles_wgs;         ///< [rad] aircraft attitude expressed as rotation from WGS to BAS
    Angles _angles_ned;         ///< [rad] aircraft attitude expressed as rotation from NED to BAS

    Vector3 _vel_ned;           ///< [m/s] aircraft linear velocity vector expressed in NED

    Vector3 _vel_air_bas;       ///< [m/s] aircraft linear velocity vector relative to the air expressed in BAS
    Vector3 _omg_air_bas;       ///< [rad/s] aircraft angular velocity relative to the air expressed in BAS

    Vector3 _acc_bas;           ///< [m/s^2] aircraft linear acceleration vector expressed in BAS
    Vector3 _eps_bas;           ///< [rad/s^2] aircraft angular acceleration vector expressed in BAS

    Vector3 _grav_wgs;          ///< [m/s^2] gravity acceleration vector expressed in WGS
    Vector3 _grav_bas;          ///< [m/s^2] gravity acceleration vector expressed in BAS

    Vector3 _g_force;           ///< [-] vector of G-Force factor (aircraft)
    Vector3 _g_pilot;           ///< [-] vector of G-Force factor (pilot's head)

    Vector3 _ground_wgs;        ///< [m] ground intersection coordinates expressed in WGS
    Vector3 _ground_bas;        ///< [m] ground intersection coordinates expressed in BAS

    Vector3 _normal_wgs;        ///< [-] normal to ground vector expressed in WGS
    Vector3 _normal_bas;        ///< [-] normal to ground vector expressed in BAS

    DataOut::Crash _crash;      ///< crash cause

    PropState _initPropState;   ///< initial propulsion state

    UInt32 _cp_index;           ///< current collision point index

    double _elevation;          ///< [m] ground elevation above mean sea level
    double _altitude_asl;       ///< [m] altitude above sea level
    double _altitude_agl;       ///< [m] altitude above ground level
    double _roll;               ///< [rad] roll angle
    double _pitch;              ///< [rad] pitch angle
    double _heading;            ///< [rad] true heading
    double _angleOfAttack;      ///< [rad] angle of attack
    double _sideslipAngle;      ///< [rad] sideslip angle
    double _climbAngle;         ///< [rad] climb angle
    double _trackAngle;         ///< [rad] track angle
    double _slipSkidAngle;      ///< [rad] slip/skid angle
    double _airspeed;           ///< [m/s] real airspeed
    double _dynPress;           ///< [Pa] dynamic pressure
    double _ias;                ///< [m/s] indicated airspeed
    double _tas;                ///< [m/s] true airspeed
    double _groundSpeed;        ///< [m/s] ground speed (horizontal velocity)
    double _machNumber;         ///< [-] Mach number
    double _climbRate;          ///< [m/s] climb rate
    double _turnRate;           ///< [rad/s] turn rate
    double _headingPrev;        ///< [rad] previous heading

    bool _freeze_position;      ///< specifies if position is to be frozen (is not integrating)
    bool _freeze_attitude;      ///< specifies if attitude is to be frozen (is not integrating)
    bool _freeze_velocity;      ///< specifies if velocity is to be frozen (is not integrating)

    /**
     * @brief Reads data.
     * @param dataFile XML data file path
     */
    virtual void readFile( const char *dataFile );

    /**
     * @brief Reads data.
     * @param dataNode XML node
     */
    virtual void readData( XmlNode &dataNode );

    /** @brief This function is called just before time integration step. */
    virtual void anteIntegration();

    /** @brief This function is called just after time integration step. */
    virtual void postIntegration();

    /** @brief This function checks collisions. */
    virtual void detectCrash();

    /**
     * @brief Computes state vector derivatives due to given state vector.
     * @param stateVect state vector
     * @param derivVect resulting state vector derivative
     */
    virtual void computeStateDeriv( const StateVector &stateVect,
                                    StateVector *derivVect );

    /**
     * @brief Updates aircraft state variables.
     * @param stateVect state vector
     * @param derivVect state vector derivative
     */
    virtual void updateVariables( const StateVector &stateVect,
                                  const StateVector &derivVect );

private:

    /** Using this constructor is forbidden. */
    Aircraft( const Aircraft & ) : Base() {}
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_AIRCRAFT_H
