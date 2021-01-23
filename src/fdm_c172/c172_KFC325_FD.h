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
#ifndef C172_KFC325_FD_H
#define C172_KFC325_FD_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/auto/fdm_FlightDirector.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

class C172_KFC325_AP;

/**
 * @brief Flight director class.
 *
 * @see Bociek S., Gruszecki J.: Uklady sterowania automatycznego samolotem, 1999, p.213. [in Polish]
 */
class C172_KFC325_FD : public FlightDirector
{
public:

    /** Lateral modes. */
    enum LatMode
    {
        LM_FD = 0,      ///< wing level
        LM_HDG,         ///< heading select
        LM_NAV,         ///< track navigation sensor
        LM_APR,         ///< approach
        LM_BC           ///< back course approach
    };

    /** Vertical modes. */
    enum VerMode
    {
        VM_FD = 0,      ///< pitch attitude hold
        VM_ALT,         ///< altitude hold
        VM_IAS,         ///< indicated airspeed hold
        VM_VS,          ///< vertical speed
        VM_ARM,         ///< altitude select
        VM_GS           ///< approach glide slope
    };

    /** Armed modes. */
    enum ArmMode
    {
        ARM_NONE = 0,   ///< no mode armed
        ARM_NAV,        ///< track navigation senson
        ARM_APR,        ///< approach
        ARM_BC          ///< back course approach
    };

    /** Constructor. */
    C172_KFC325_FD( const C172_KFC325_AP *ap );

    /** Destructor. */
    virtual ~C172_KFC325_FD();

    /**
     * Reads data.
     * @param dataNode XML node
     */
    void readData( XmlNode &dataNode );

    /**
     * Updates flight director.
     */
    void update( double timeStep,
                 double heading,
                 double altitude, double airspeed,
                 double turn_rate, double climb_rate,
                 double dme_distance,
                 double nav_deviation, bool nav_active,
                 double loc_deviation, bool loc_active,
                 double gs_deviation,  bool gs_active );

    void onPressedFD();

    void onPressedALT();
    void onPressedIAS();
    void onPressedENG();
    void onPressedARM();

    void onPressedHDG();
    void onPressedNAV();
    void onPressedAPR();
    void onPressedBC();

    void onPressedHalfBank();

    inline LatMode getLatMode() const { return _lat_mode; }
    inline VerMode getVerMode() const { return _ver_mode; }
    inline ArmMode getArmMode() const { return _arm_mode; }

    inline bool isActiveHalfBank() const { return _half_bank; }

    inline void setLatMode( LatMode lat_mode ) { _lat_mode = lat_mode; }
    inline void setVerMode( VerMode ver_mode ) { _ver_mode = ver_mode; }
    inline void setArmMode( ArmMode arm_mode ) { _arm_mode = arm_mode; }

    void setHeadingILS( double heading_ils );

    void toggleLatMode( LatMode lat_mode );
    void toggleVerMode( VerMode ver_mode );

protected:

    const C172_KFC325_AP *_ap;  ///< autopilot

    LatMode _lat_mode;          ///< lateral mode
    VerMode _ver_mode;          ///< vertical mode
    ArmMode _arm_mode;          ///< horizontal arm mode

    LatMode _lat_mode_arm;      ///< lateral mode (before armed)

    PID _pid_alt;               ///< ALT mode PID controller
    PID _pid_ias;               ///< IAS mode PID controller
    PID _pid_vs;                ///< VS mode PID controller
    PID _pid_arm;               ///< ARM mode PID controller
    PID _pid_gs;                ///< GS quasi mode PID controller
    PID _pid_nav_ang;           ///< NAV mode PID controller (angular)
    PID _pid_nav_lin;           ///< NAV mode PID controller (linear)
    PID _pid_apr_ang;           ///< APR mode PID controller (angular)
    PID _pid_apr_lin;           ///< APR mode PID controller (linear)
    PID _pid_hdg;               ///< HDG mode PID controller
    PID _pid_turn;              ///< turn quasi mode PID controller

    double _max_roll;           ///< [rad] max roll
    double _min_pitch;          ///< [rad] min pitch
    double _max_pitch;          ///< [rad] max pitch
    double _max_yaw;            ///< [rad] max yaw

    double _max_rate_roll;      ///< [rad/s] roll max rate
    double _max_rate_pitch;     ///< [rad/s] pitch max rate
    double _max_rate_tr;        ///< [rad/s^2] turn rate max rate

    double _heading_act;        ///< [rad] acting desired heading
    double _heading_ils;        ///< [rad] ILS heading

    double _climb_rate_act;     ///< [m/s] acting desired climb rate
    double _climb_rate_tc;      ///< [s] acting desired climb rate time constant

    double _turn_rate;          ///< [rad/s] turn rate

    double _min_dh_arm;         ///< [m] ARM mode min altitude difference

    double _nav_dev_max;        ///< [rad]
    double _apr_dev_max;        ///< [rad]

    double _gs_dev_prev;        ///< [rad]

    bool _turn_rate_mode;       ///<

    bool _half_bank;            ///< specifies if half bank is engaged

    virtual void readMode( const XmlNode &dataNode, PID *pid, double min, double max );

    virtual void updateArmMode( double dme_distance,
                                double nav_deviation, bool nav_active,
                                double loc_deviation, bool loc_active );

    virtual void updateLatFD   ( double timeStep );
    virtual void updateLatNAV  ( double timeStep, double dme_distance, double nav_deviation );
    virtual void updateLatAPR  ( double timeStep, double dme_distance, double loc_deviation );
    virtual void updateLatBC   ( double timeStep, double dme_distance, double loc_deviation );
    virtual void updateLatABC  ( double timeStep, double dme_distance, double loc_deviation, double heading );
    virtual void updateLatHDG  ( double timeStep, double heading, double turnRate );
    virtual void updateLatTurn ( double timeStep, double turnRate, double airspeed );

    virtual void updateVerFD  ( double timeStep );
    virtual void updateVerALT ( double timeStep, double altitude );
    virtual void updateVerIAS ( double timeStep, double airspeed );
    virtual void updateVerVS  ( double timeStep, double altitude, double climbRate );
    virtual void updateVerARM ( double timeStep, double altitude, double climbRate );
    virtual void updateVerGS  ( double timeStep, double gs_deviation, bool gs_active );
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // C172_KFC325_FD_H
