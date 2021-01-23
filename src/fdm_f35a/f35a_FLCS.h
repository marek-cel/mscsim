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
#ifndef F35A_FLCS_H
#define F35A_FLCS_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/ctrl/fdm_Lag.h>
#include <fdm/ctrl/fdm_PID.h>

#include <fdm/utils/fdm_Table1.h>
#include <fdm/utils/fdm_Vector3.h>

#include <fdm/xml/fdm_XmlNode.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief F-35A Flight Control System class.
 *
 * @see Nguyen, Gilbert, Ogburn: Control-System Techniques for Improved Departure-Spin Resistance for Fighter AIrcraft, NASA-TP-1689, 1980
 */
class F35A_FLCS
{
public:

    /** Constructor. */
    F35A_FLCS();

    /** Destructor. */
    virtual ~F35A_FLCS();

    /**
     * Reads data.
     * @param dataNode XML node
     */
    void readData( XmlNode &dataNode );

    /** Updates model. */
    void update( double timeStep,
                 double ctrlLat, double trimLat,
                 double ctrlLon, double trimLon,
                 double ctrlYaw, double trimYaw,
                 const Vector3 &omg_bas,
                 const Vector3 &gforce_bas,
                 const Vector3 &grav_bas,
                 double angleOfAttack,
                 double statPress, double dynPress,
                 bool lg_handle_dn );

    inline double getNormAilerons () const { return _norm_ailerons; }
    inline double getNormElevator () const { return _norm_elevator; }
    inline double getNormRudder   () const { return _norm_rudder;   }
    inline double getNormFlapsLE  () const { return _norm_flaps_le; }
    inline double getNormFlapsTE  () const { return _norm_flaps_te; }

private:

    Table1 _ctrl_input_roll;            ///<
    Table1 _ctrl_input_pitch;           ///<

    Table1 _pitch_rate_gain;            ///<

    Table1 _lef_aoa_flaps;              ///<

    Lag _lag_ctrl_roll;                 ///<
    Lag _lag_ctrl_pitch;                ///<
    Lag _lag_ctrl_yaw;                  ///<

    Lag _lag_rate_roll;                 ///<
    Lag _lag_rate_pitch;                ///<
    Lag _lag_rate_yaw;                  ///<

    PID _pid_roll;                      ///<
    PID _pid_pitch_1;                   ///<
    PID _pid_pitch_2;                   ///<
    PID _pid_yaw;                       ///<

    double _timeStep;                   ///< [s] time step

    double _max_rate_ailerons;          ///< [1/s]
    double _max_rate_elevator;          ///< [1/s]
    double _max_rate_rudder;            ///< [1/s]
    double _max_rate_flaps_le;          ///< [1/s]
    double _max_rate_flaps_te;          ///< [1/s]

    double _roll_rate_gain;             ///<
    double _gforce_gain;                ///<
    double _aoa_gain;                   ///<
    double _yaw_rate_gain;              ///<

    double _lef_time_const;             ///< [s] leading edge flaps time constant

    double _norm_ailerons;              ///< [-] normalized ailerons
    double _norm_elevator;              ///< [-] normalized elevator
    double _norm_rudder;                ///< [-] normalized rudder
    double _norm_flaps_le;              ///< [-]
    double _norm_flaps_te;              ///< [-]

    void updateCtrlRoll( double timeStep,
                         double ctrlLat, double trimLat,
                         const Vector3 &omg_bas );

    void updateCtrlPitch( double timeStep,
                          double ctrlLon, double trimLon,
                          const Vector3 &omg_bas,
                          const Vector3 &gforce_bas,
                          const Vector3 &grav_bas,
                          double statPress, double dynPress,
                          double angleOfAttack );

    void updateCtrlYaw( double timeStep,
                        double ctrlYaw, double trimYaw,
                        const Vector3 &omg_bas );

    void updateFlapsLE( double timeStep, double angleOfAttack );
    void updateFlapsTE( double timeStep, bool lg_handle_dn );
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // F35A_FLCS_H
