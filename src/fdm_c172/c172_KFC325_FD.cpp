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

#include <fdm_c172/c172_KFC325_FD.h>

#include <fdm/fdm_Log.h>
#include <fdm/utils/fdm_Misc.h>
#include <fdm/utils/fdm_Units.h>
#include <fdm/utils/fdm_WGS84.h>
#include <fdm/xml/fdm_XmlUtils.h>

#include <fdm_c172/c172_KFC325_AP.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

C172_KFC325_FD::C172_KFC325_FD( const C172_KFC325_AP *ap ) :
    FlightDirector( ap ),

    _ap ( ap ),

    _lat_mode ( LM_FD ),
    _ver_mode ( VM_FD ),

    _arm_mode ( ARM_NONE ),

    _lat_mode_arm ( LM_FD ),

    _pid_alt     ( 0.0, 0.0, 0.0, -M_PI_2  , M_PI_2  ),
    _pid_ias     ( 0.0, 0.0, 0.0, -M_PI_2  , M_PI_2  ),
    _pid_vs      ( 0.0, 0.0, 0.0, -M_PI_2  , M_PI_2  ),
    _pid_arm     ( 0.0, 0.0, 0.0, -DBL_MAX , DBL_MAX ),
    _pid_gs      ( 0.0, 0.0, 0.0, -M_PI_2  , M_PI_2  ),
    _pid_nav_ang ( 0.0, 0.0, 0.0, -M_PI_2  , M_PI_2  ),
    _pid_nav_lin ( 0.0, 0.0, 0.0, -M_PI_2  , M_PI_2  ),
    _pid_apr_ang ( 0.0, 0.0, 0.0, -M_PI_2  , M_PI_2  ),
    _pid_apr_lin ( 0.0, 0.0, 0.0, -M_PI_2  , M_PI_2  ),
    _pid_hdg     ( 0.0, 0.0, 0.0, -M_PI_2  , M_PI_2  ),
    _pid_turn    ( 0.0, 0.0, 0.0, -DBL_MAX , DBL_MAX ),

    _max_roll  ( 0.0 ),
    _min_pitch ( 0.0 ),
    _max_pitch ( 0.0 ),
    _max_yaw   ( 0.0 ),

    _max_rate_roll  ( 0.0 ),
    _max_rate_pitch ( 0.0 ),
    _max_rate_tr    ( 0.0 ),

    _heading_act ( 0.0 ),
    _heading_ils ( 0.0 ),

    _climb_rate_act ( 0.0 ),
    _climb_rate_tc  ( 0.0 ),

    _turn_rate ( 0.0 ),

    _min_dh_arm  ( 0.0 ),

    _nav_dev_max ( 0.0 ),
    _apr_dev_max ( 0.0 ),

    _gs_dev_prev ( 0.0 ),

    _turn_rate_mode ( false ),

    _half_bank ( false )
{
    _pid_alt     .setAntiWindup( PID::Calculation );
    _pid_ias     .setAntiWindup( PID::Calculation );
    _pid_vs      .setAntiWindup( PID::Calculation );
    _pid_arm     .setAntiWindup( PID::Calculation );
    _pid_gs      .setAntiWindup( PID::Calculation );
    _pid_nav_ang .setAntiWindup( PID::Calculation );
    _pid_nav_lin .setAntiWindup( PID::Calculation );
    _pid_apr_ang .setAntiWindup( PID::Calculation );
    _pid_apr_lin .setAntiWindup( PID::Calculation );
    _pid_hdg     .setAntiWindup( PID::Calculation );
    _pid_turn    .setAntiWindup( PID::Calculation );
}

////////////////////////////////////////////////////////////////////////////////

C172_KFC325_FD::~C172_KFC325_FD() {}

////////////////////////////////////////////////////////////////////////////////

void C172_KFC325_FD::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_max_roll  , "max_roll"  );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_min_pitch , "min_pitch" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_max_pitch , "max_pitch" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_max_yaw   , "max_yaw"   );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_max_rate_roll  , "max_rate_roll"  );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_max_rate_pitch , "max_rate_pitch" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_max_rate_tr    , "max_rate_tr"    );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_climb_rate_tc, "climb_rate_tc" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_min_dh_arm, "min_dh_arm" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_nav_dev_max, "nav_dev_max" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_apr_dev_max, "apr_dev_max" );

        if ( result != FDM_SUCCESS ) XmlUtils::throwError( __FILE__, __LINE__, dataNode );

        XmlNode nodeALT = dataNode.getFirstChildElement( "mode_alt" );
        XmlNode nodeIAS = dataNode.getFirstChildElement( "mode_ias" );
        XmlNode nodeVS  = dataNode.getFirstChildElement( "mode_vs"  );
        XmlNode nodeARM = dataNode.getFirstChildElement( "mode_arm" );
        XmlNode nodeGS  = dataNode.getFirstChildElement( "mode_gs"  );

        XmlNode nodeNAV_ANG = dataNode.getFirstChildElement( "mode_nav_ang" );
        XmlNode nodeNAV_LIN = dataNode.getFirstChildElement( "mode_nav_lin" );
        XmlNode nodeAPR_ANG = dataNode.getFirstChildElement( "mode_apr_ang" );
        XmlNode nodeAPR_LIN = dataNode.getFirstChildElement( "mode_apr_lin" );
        XmlNode nodeHDG  = dataNode.getFirstChildElement( "mode_hdg" );
        XmlNode nodeTurn = dataNode.getFirstChildElement( "mode_turn" );

        readMode( nodeALT , &_pid_alt , _min_pitch , _max_pitch );
        readMode( nodeIAS , &_pid_ias , _min_pitch , _max_pitch );
        readMode( nodeVS  , &_pid_vs  , _min_pitch , _max_pitch );
        readMode( nodeARM , &_pid_arm , -DBL_MAX   , DBL_MAX    );
        readMode( nodeGS  , &_pid_gs  , _min_pitch , _max_pitch );

        readMode( nodeNAV_ANG, &_pid_nav_ang, -_max_yaw, _max_yaw );
        readMode( nodeNAV_LIN, &_pid_nav_lin, -_max_yaw, _max_yaw );
        readMode( nodeAPR_ANG, &_pid_apr_ang, -_max_yaw, _max_yaw );
        readMode( nodeAPR_LIN, &_pid_apr_lin, -_max_yaw, _max_yaw );
        readMode( nodeHDG  , &_pid_hdg  , -DBL_MAX   , DBL_MAX   );
        readMode( nodeTurn , &_pid_turn , -_max_roll , _max_roll );
    }
    else
    {
        XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
}

////////////////////////////////////////////////////////////////////////////////

void C172_KFC325_FD::update( double timeStep,
                             double heading,
                             double altitude, double airspeed,
                             double turn_rate, double climb_rate,
                             double dme_distance,
                             double nav_deviation, bool nav_active,
                             double loc_deviation, bool loc_active,
                             double gs_deviation,  bool gs_active )
{
    _turn_rate_mode = false;

    updateArmMode( dme_distance,
                   nav_deviation, nav_active,
                   loc_deviation, loc_active );

    if ( _engaged )
    {
        if ( _lat_mode != LM_NAV && _lat_mode != LM_APR && _lat_mode != LM_BC )
        {
            _heading_act = _heading;
        }

        updateLatFD  ( timeStep );
        updateLatNAV ( timeStep, dme_distance, nav_deviation );
        updateLatAPR ( timeStep, dme_distance, loc_deviation );
        updateLatBC  ( timeStep, dme_distance, loc_deviation );

        updateLatHDG  ( timeStep, heading, turn_rate );
        updateLatTurn ( timeStep, turn_rate, airspeed );

        updateVerFD  ( timeStep );
        updateVerALT ( timeStep, altitude );
        updateVerIAS ( timeStep, airspeed );
        updateVerVS  ( timeStep, altitude, climb_rate );
        updateVerARM ( timeStep, altitude, climb_rate );
        updateVerGS  ( timeStep, gs_deviation, gs_active );

        _cmd_roll  = Misc::satur( -_max_roll  , _max_roll  , _cmd_roll  );
        _cmd_pitch = Misc::satur(  _min_pitch , _max_pitch , _cmd_pitch );

        _gs_dev_prev = gs_deviation;
    }
    else
    {
        _pid_alt .reset();
        _pid_ias .reset();
        _pid_vs  .reset();
        _pid_arm .reset();
        _pid_hdg .reset();

        _cmd_roll  = 0.0;
        _cmd_pitch = 0.0;

        _climb_rate_act = climb_rate;
        _turn_rate = turn_rate;

        _gs_dev_prev = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////

void C172_KFC325_FD::onPressedFD()
{
    if ( _engaged )
    {
        disengage();

        if ( !_ap->isActiveAP() )
        {
            setLatMode( LM_FD );
            setVerMode( VM_FD );
        }

    }
    else
    {
        engage();
    }
}

////////////////////////////////////////////////////////////////////////////////

void C172_KFC325_FD::onPressedALT()
{
    if ( _ver_mode != VM_GS )
    {
        toggleVerMode( VM_ALT );
    }
}

////////////////////////////////////////////////////////////////////////////////

void C172_KFC325_FD::onPressedIAS()
{
    if ( _ver_mode != VM_GS )
    {
        toggleVerMode( VM_IAS );
    }
}

////////////////////////////////////////////////////////////////////////////////

void C172_KFC325_FD::onPressedENG()
{
    if ( _ver_mode != VM_GS )
    {
        setVerMode( VM_VS );    // or toggle ??
    }
}

////////////////////////////////////////////////////////////////////////////////

void C172_KFC325_FD::onPressedARM()
{
    if ( _ver_mode != VM_GS )
    {
        setVerMode( VM_ARM );   // or toggle ??
    }
}

////////////////////////////////////////////////////////////////////////////////

void C172_KFC325_FD::onPressedHDG()
{
    engage();
    toggleLatMode( LM_HDG );
}

////////////////////////////////////////////////////////////////////////////////

void C172_KFC325_FD::onPressedNAV()
{
    engage();

    if ( _lat_mode == LM_NAV && _arm_mode != ARM_NAV )
        toggleLatMode( LM_NAV );
    else
        setArmMode( ARM_NAV );
}

////////////////////////////////////////////////////////////////////////////////

void C172_KFC325_FD::onPressedAPR()
{
    engage();

    if ( _lat_mode == LM_APR && _arm_mode != ARM_APR )
    {
        setLatMode( LM_FD );

        if ( _ver_mode == VM_GS )
            setVerMode( VM_FD );
    }
    else
        setArmMode( ARM_APR );
}

////////////////////////////////////////////////////////////////////////////////

void C172_KFC325_FD::onPressedBC()
{
    engage();

    if ( _lat_mode == LM_BC && _arm_mode != ARM_BC )
    {
        setLatMode( LM_FD );

        if ( _ver_mode == VM_GS )
            setVerMode( VM_FD );
    }
    else
        setArmMode( ARM_BC );
}

////////////////////////////////////////////////////////////////////////////////

void C172_KFC325_FD::onPressedHalfBank()
{
    _half_bank = !_half_bank;

    if ( _half_bank )
    {
        double min_roll = -0.5 * _max_roll;
        double max_roll =  0.5 * _max_roll;

        _pid_turn .setMin( min_roll );
        _pid_turn .setMax( max_roll );
    }
    else
    {
        _pid_turn .setMin( -_max_roll );
        _pid_turn .setMax(  _max_roll );
    }
}

////////////////////////////////////////////////////////////////////////////////

void C172_KFC325_FD::setHeadingILS( double heading_ils )
{
    _heading_ils = heading_ils;
}

////////////////////////////////////////////////////////////////////////////////

void C172_KFC325_FD::toggleLatMode( LatMode lat_mode )
{
    if ( _lat_mode == lat_mode )
        _lat_mode = LM_FD;
    else
        _lat_mode = lat_mode;

    _arm_mode = ARM_NONE;
}

////////////////////////////////////////////////////////////////////////////////

void C172_KFC325_FD::toggleVerMode( VerMode ver_mode )
{
    if ( _ver_mode == ver_mode )
        _ver_mode = VM_FD;
    else
        _ver_mode = ver_mode;
}

////////////////////////////////////////////////////////////////////////////////

void C172_KFC325_FD::readMode( const XmlNode &dataNode, PID *pid, double min, double max )
{
    if ( dataNode.isValid() )
    {
        XmlNode nodePID = dataNode.getFirstChildElement( "pid" );

        if ( FDM_SUCCESS != XmlUtils::read( nodePID, pid, min, max ) )
        {
            XmlUtils::throwError( __FILE__, __LINE__, dataNode );
        }
    }
    else
    {
        XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
}

////////////////////////////////////////////////////////////////////////////////

void C172_KFC325_FD::updateArmMode( double dme_distance,
                                    double nav_deviation, bool nav_active,
                                    double loc_deviation, bool loc_active )
{
    // NAV
    if ( _arm_mode == ARM_NAV )
    {
        if ( nav_active && fabs( nav_deviation ) < _nav_dev_max )
        {
            _arm_mode = ARM_NONE;
            _lat_mode_arm = _lat_mode;
            _lat_mode = LM_NAV;
        }
    }
    else if (  _arm_mode == ARM_NONE && _lat_mode == LM_NAV && !nav_active )
    {
        _arm_mode = ARM_NAV;
        _lat_mode = _lat_mode_arm;
    }
    else if ( _arm_mode == ARM_NONE && _lat_mode == LM_NAV && fabs( nav_deviation ) > _nav_dev_max )
    {
        if ( dme_distance > 1.0e-9 )
        {
            double lin_dev_max = _pid_nav_lin.getMax() / _pid_nav_lin.getKp();
            double lin_deviation = dme_distance * sin( nav_deviation );

            if ( fabs( lin_deviation ) > lin_dev_max )
                _arm_mode = ARM_NAV;
        }
        else
        {
            double ang_dev_max = _pid_nav_ang.getMax() / _pid_nav_ang.getKp();

            if ( fabs( nav_deviation ) > ang_dev_max )
                _arm_mode = ARM_NAV;
        }
    }

    // APR
    if ( _arm_mode == ARM_APR )
    {
        if ( loc_active && fabs( loc_deviation ) < _apr_dev_max )
        {
            _arm_mode = ARM_NONE;
            _lat_mode_arm = _lat_mode;
            _lat_mode = LM_APR;
        }
    }
    else if (  _arm_mode == ARM_NONE && _lat_mode == LM_APR && !loc_active )
    {
        _arm_mode = ARM_APR;
        _lat_mode = _lat_mode_arm;
    }
    else if ( _arm_mode == ARM_NONE && _lat_mode == LM_APR && fabs( loc_deviation ) > _apr_dev_max )
    {
        _arm_mode = ARM_APR;
    }

    // BC
    if ( _arm_mode == ARM_BC )
    {
        if ( loc_active && fabs( loc_deviation ) < _apr_dev_max )
        {
            _arm_mode = ARM_NONE;
            _lat_mode_arm = _lat_mode;
            _lat_mode = LM_BC;
        }
    }
    else if ( _arm_mode == ARM_NONE && _lat_mode == LM_BC && !loc_active )
    {
        _arm_mode = ARM_BC;
        _lat_mode = _lat_mode_arm;
    }
    else if ( _arm_mode == ARM_NONE && _lat_mode == LM_BC && fabs( loc_deviation ) > _apr_dev_max )
    {
        _arm_mode = ARM_BC;
    }
}

////////////////////////////////////////////////////////////////////////////////

void C172_KFC325_FD::updateLatFD( double timeStep )
{
    if ( _lat_mode == LM_FD )
    {
        _cmd_roll = Misc::rate( timeStep, _max_rate_roll, _cmd_roll, 0.0 );
    }
}

////////////////////////////////////////////////////////////////////////////////

void C172_KFC325_FD::updateLatNAV( double timeStep, double dme_distance, double nav_deviation )
{
    if ( _lat_mode == LM_NAV )
    {
        if ( _arm_mode == ARM_NAV )
        {
            _heading_act = nav_deviation > 0.0 ? _course + _max_yaw : _course - _max_yaw;
        }
        else
        {
            if ( dme_distance > 1.0e-9 )
            {
                double lin_deviation = dme_distance * sin( nav_deviation );
                _pid_nav_lin.update( timeStep, lin_deviation );
                _heading_act = _course + _pid_nav_lin.getValue();
            }
            else
            {
                _pid_nav_ang.update( timeStep, nav_deviation );
                _heading_act = _course + _pid_nav_ang.getValue();
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void C172_KFC325_FD::updateLatAPR( double timeStep, double dme_distance, double loc_deviation )
{
    if ( _lat_mode == LM_APR )
    {
        updateLatABC( timeStep, dme_distance,  loc_deviation, _heading_ils );
    }
}

////////////////////////////////////////////////////////////////////////////////

void C172_KFC325_FD::updateLatBC( double timeStep, double dme_distance, double loc_deviation )
{
    if ( _lat_mode == LM_BC )
    {
        updateLatABC( timeStep, dme_distance, -loc_deviation, _heading_ils - M_PI );
    }
}

////////////////////////////////////////////////////////////////////////////////

void C172_KFC325_FD::updateLatABC( double timeStep, double dme_distance, double loc_deviation, double heading )
{
    if ( ( _lat_mode == LM_APR && _arm_mode == ARM_APR )
      || ( _lat_mode == LM_BC  && _arm_mode == ARM_BC ) )
    {
        _heading_act = loc_deviation > 0.0 ? heading + _max_yaw : heading - _max_yaw;
    }
    else
    {
        if ( dme_distance > 1.0e-9 )
        {
            double lin_deviation = dme_distance * sin( loc_deviation );
            _pid_apr_lin.update( timeStep, lin_deviation );
            _heading_act = heading + _pid_apr_lin.getValue();
        }
        else
        {
            _pid_apr_ang.update( timeStep, loc_deviation );
            _heading_act = heading + _pid_apr_ang.getValue();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void C172_KFC325_FD::updateLatHDG( double timeStep, double heading, double turnRate )
{
    double delta_hdg = _heading_act - heading;

    while ( delta_hdg < -M_PI ) delta_hdg += 2.0 * M_PI;
    while ( delta_hdg >  M_PI ) delta_hdg -= 2.0 * M_PI;

    if ( _lat_mode == LM_HDG || _lat_mode == LM_NAV || _lat_mode == LM_APR || _lat_mode == LM_BC )
    {
        _turn_rate_mode = true;

        _pid_hdg.update( timeStep, delta_hdg );
        _turn_rate = Misc::rate( timeStep, _max_rate_tr, _turn_rate, _pid_hdg.getValue() );
    }
    else
    {
        _pid_hdg.setValue( turnRate );
    }
}

////////////////////////////////////////////////////////////////////////////////

void C172_KFC325_FD::updateLatTurn( double timeStep, double turnRate, double airspeed )
{
    if ( _turn_rate_mode )
    {
        double roll = atan2( airspeed * _turn_rate, WGS84::_g );

        _pid_turn.update( timeStep, _turn_rate - turnRate );
        _cmd_roll = Misc::rate( timeStep, _max_rate_roll, _cmd_roll, roll + _pid_turn.getValue() );
    }
    else
    {
        _pid_turn.setValue( _cmd_roll );
        _turn_rate = turnRate;
    }
}

////////////////////////////////////////////////////////////////////////////////

void C172_KFC325_FD::updateVerFD( double timeStep )
{
    if ( _ver_mode == VM_FD )
        _cmd_pitch = Misc::rate( timeStep, _max_rate_pitch, _cmd_pitch, _pitch );
    else
        _pitch = _cmd_pitch;
}

////////////////////////////////////////////////////////////////////////////////

void C172_KFC325_FD::updateVerALT( double timeStep, double altitude )
{
    if ( _ver_mode == VM_ALT )
    {
        _pid_alt.update( timeStep, _altitude - altitude );
        _cmd_pitch = Misc::rate( timeStep, _max_rate_pitch, _cmd_pitch, _pid_alt.getValue() );
    }
    else
    {
        _pid_alt.setValue( _cmd_pitch );
    }
}

////////////////////////////////////////////////////////////////////////////////

void C172_KFC325_FD::updateVerIAS( double timeStep, double airspeed )
{
    if ( _ver_mode == VM_IAS )
    {
        _pid_ias.update( timeStep, _airspeed - airspeed );
        _cmd_pitch = Misc::rate( timeStep, _max_rate_pitch, _cmd_pitch, _pid_ias.getValue() );
    }
    else
    {
        _pid_ias.setValue( _cmd_pitch );
    }
}

////////////////////////////////////////////////////////////////////////////////

void C172_KFC325_FD::updateVerVS( double timeStep, double altitude, double climbRate )
{
    if ( _ver_mode == VM_VS || _ver_mode == VM_ARM )
    {
        _climb_rate_act = Misc::inertia( _climbRate, _climb_rate_act, timeStep, _climb_rate_tc );
        _pid_vs.update( timeStep, _climb_rate_act - climbRate );
        _cmd_pitch = Misc::rate( timeStep, _max_rate_pitch, _cmd_pitch, _pid_vs.getValue() );

        //if ( _ver_mode == VM_ARM )
        //{
        //    _pid_alt.setValue( 0.0 );
        //    _pid_alt.update( timeStep, _altitude - altitude );
        //
        //    double delta_tht = _pid_vs.getValue() - _pid_alt.getValue();
        //    double delta_alt = _altitude - altitude;
        //
        //    _pid_alt.setValue( _cmd_pitch );
        //
        //    if ( ( delta_alt < 0.0 && delta_tht > 0.0 )
        //      || ( delta_alt > 0.0 && delta_tht < 0.0 ) )
        //    {
        //        _ver_mode = VM_ALT;
        //    }
        //}
    }
    else
    {
        _pid_vs.setValue( _cmd_pitch );
        _climb_rate_act = climbRate;
    }
}

////////////////////////////////////////////////////////////////////////////////

void C172_KFC325_FD::updateVerARM( double timeStep, double altitude, double climbRate )
{
    double delta_h = _altitude - altitude;

    if ( _ver_mode == VM_ARM )
    {
        if ( fabs( delta_h ) < _min_dh_arm )
        {
            _ver_mode = VM_ALT;
        }
        else
        {
            _pid_arm.update( timeStep, delta_h );
            _climbRate = _pid_arm.getValue();
        }
    }
    else
    {
        _pid_arm.setValue( climbRate );
    }
}

////////////////////////////////////////////////////////////////////////////////

void C172_KFC325_FD::updateVerGS( double timeStep, double gs_deviation, bool gs_active )
{
    if ( gs_active && _lat_mode == LM_APR && _ver_mode != VM_GS
      && ( Misc::sign( _gs_dev_prev ) != Misc::sign( gs_deviation ) ) )
    {
        _ver_mode = VM_GS;
    }

    if ( _ver_mode == VM_GS )
    {
        _pid_gs.update( timeStep, gs_deviation );
        _cmd_pitch = Misc::rate( timeStep, _max_rate_pitch, _cmd_pitch, _pid_gs.getValue() );
    }
    else
    {
        _pid_gs.setValue( _cmd_pitch );
    }
}
