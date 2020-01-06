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
#ifndef FDM_PROPELLER_H
#define FDM_PROPELLER_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_Base.h>

#include <fdm/utils/fdm_Table.h>
#include <fdm/utils/fdm_Table2D.h>
#include <fdm/utils/fdm_Vector3.h>

#include <fdm/xml/fdm_XmlNode.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Propeller class.
 *
 * <h5>XML configuration file format:</h5>
 * @code
 * <propeller>
 *   <position> { [m] x-coordinate } { [m] y-coordinate } { [m] z-coordinate } </position>
 *   <gear_ratio> { [-] gear ratio (propeller rpm / engine rpm) } </gear_ratio>
 *   <diameter> { [m] propeller diameter } </diameter>
 *   <inertia> { [kg*m^2] polar moment of inertia } </inertia>
 *   <pitch>
 *     { [-] normalized pitch } { [deg] propeller pitch at 0.75 radius }
 *     ... { more entries }
 *   </pitch>
 *   <thrust_coef>
 *     { [deg] propeller pitch at 0.75 radius } ... { more values of propeller pitch }
 *     { [-] propeller advance } { [-] thrust coefficients } ... { more values of thrust coefficients }
 *     ... { more entries }
 *   </thrust_coef>
 *   <power_coef>
 *     { [deg] propeller pitch at 0.75 radius } ... { more values of propeller pitch}
 *     { [-] propeller advance } { [-] power coefficients } ... { more values of power coefficients }
 *     ... { more entries }
 *   </power_coef>
 * </propeller>
 * @endcode
 *
 * @see Allerton D.: Principles of Flight Simulation, 2009, p.131
 * @see Raymer D.: Aircraft Design: A Conceptual Approach, 1992, p.327
 * @see Torenbeek E.: Synthesis of Subsonic Airplane Design, 1982, p.191
 * @see Paturski Z.: Przewodnik po projektach z Mechaniki Lotu, Projekt nr 5: Charakterystyki zespolu napedowego. [in Polish]
 */
class FDMEXPORT Propeller : public Base
{
public:

    /** Propeller direction. */
    enum Direction
    {
        CW  = 0,    ///< clockwise (looking from cockpit)
        CCW = 1     ///< counter-clockwise (looking from cockpit)
    };

    /** Constructor. */
    Propeller();

    /** Destructor. */
    virtual ~Propeller();

    /**
     * Reads data.
     * @param dataNode XML node
     */
    virtual void readData( XmlNode &dataNode );

    /**
     * Computes thrust.
     * @param airspeed [m/s] airspeed
     * @param airDensity [kg/m^3] air density
     */
    virtual void computeThrust( double airspeed, double airDensity );

    /**
     * Integrates model.
     * @param timeStep [s] time step
     * @param engineInertia [kg*m^2] engine polar moment of inertia
     */
    virtual void integrate( double timeStep, double engineInertia );

    /**
     * Updates propeller.
     * @param normPitch [0.0,1.0] normalized propeller lever position
     * @param engineTorque [N] engine torque
     * @param airspeed [m/s] airspeed
     * @param airDensity [kg/m^3] air density
     */
    virtual void update( double propellerLever, double engineTorque,
                         double airspeed, double airDensity );

    /**
     * Returns propeller direction.
     * @return propeller direction
     */
    inline Direction getDirection() const
    {
        return _direction;
    }

    /**
     * Returns engine rpm.
     * @return [rpm] engine rpm
     */
    inline double getEngineRPM() const
    {
        return _speed_rpm / _gearRatio;
    }

    /**
     * Returns propeller rpm.
     * @return [rpm] propeller rpm
     */
    inline double getRPM() const
    {
        return _speed_rpm;
    }

    /**
     * Returns propeller polar moment of inertia.
     * @return [kg*m^2] propeller polar moment of inertia
     */
    inline double getInertia() const
    {
        return _inertia;
    }

    /**
     * Returns propeller angular velocity.
     * @return [rad/s] propeller angular velocity
     */
    inline double getOmega() const
    {
        return _omega;
    }

    /**
     * Returns propeller position expressed in BAS.
     * @return [m] propeller position expressed in BAS
     */
    inline const Vector3& getPos_BAS() const
    {
        return _pos_bas;
    }

    /**
     * Returns propeller thrust.
     * @return [N] propeller thrust
     */
    inline double getThrust() const
    {
        return _thrust;
    }

    /**
     * Returns induced velocity.
     * @return [m/s] induced velocity
     */
    inline double getInducedVelocity() const
    {
        return _inducedVelocity;
    }

    /**
     * Returns torque.
     * @return [N*m] torque
     */
    inline double getTorque() const
    {
        return ( _torqueRequired < _torqueAvailable ) ? _torqueRequired : _torqueAvailable;
    }

    void setRPM( double rpm );

protected:

    Vector3 _pos_bas;           ///< [m] propeller position expressed in BAS

    Table _propPitch;           ///< [rad] propeller pitch vs [-] normalized pitch

    Table2D _coefThrust;        ///< [-] thrust coefficient
    Table2D _coefPower;         ///< [-] power coefficient

    Direction _direction;       ///< propeller direction looking from cockpit

    double _gearRatio;          ///< [-] gear ratio (propeller rpm / engine rpm)
    double _diameter;           ///< [m] diameter
    double _inertia;            ///< [kg*m^2] polar moment of inertia

    double _area;               ///< [m^2] propeller disc area

    double _pitch;              ///< [rad] propeller pitch at 0.75 radius
    double _omega;              ///< [rad/s] propeller angular velocity
    double _speed_rps;          ///< [rps] propeller speed
    double _speed_rpm;          ///< [rpm] propeller speed
    double _thrust;             ///< [N] thrust

    double _inducedVelocity;    ///< [m/s] induced velocity

    double _torqueAvailable;    ///< [N*m] available torque
    double _torqueRequired;     ///< [N*m] required torque

    /**
     * Computes induced velocity.
     * @param airspeed [m/s] airspeed
     * @param airDensity [kg/m^3] air density
     */
    virtual double getInducedVelocity( double airspeed, double airDensity );

    /**
     * Computes propeller pitch.
     * @param propellerLever [0.0,1.0] normalized propeller lever position
     */
    virtual double getPropellerPitch( double propellerLever );
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_PROPELLER_H
