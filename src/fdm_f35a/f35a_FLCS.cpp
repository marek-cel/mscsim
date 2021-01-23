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

#include <fdm_f35a/f35a_FLCS.h>

#include <fdm/utils/fdm_Units.h>

#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

F35A_FLCS::F35A_FLCS() :
    _ctrl_input_roll  ( Table1::oneRecordTable( 0.0 ) ),
    _ctrl_input_pitch ( Table1::oneRecordTable( 0.0 ) ),
    _pitch_rate_gain  ( Table1::oneRecordTable( 0.0 ) ),
    _lef_aoa_flaps    ( Table1::oneRecordTable( 0.0 ) ),

    _pid_roll    ( 0.0, 0.0, 0.0, -1.0, 1.0 ),
    _pid_pitch_1 ( 0.0, 0.0, 0.0, -1.0, 1.0 ),
    _pid_pitch_2 ( 0.0, 0.0, 0.0, -1.0, 1.0 ),
    _pid_yaw     ( 0.0, 0.0, 0.0, -1.0, 1.0 ),

    _timeStep ( 0.0 ),

    _max_rate_ailerons ( 0.0 ),
    _max_rate_elevator ( 0.0 ),
    _max_rate_rudder   ( 0.0 ),
    _max_rate_flaps_le ( 0.0 ),
    _max_rate_flaps_te ( 0.0 ),

    _roll_rate_gain ( 0.0 ),
    _gforce_gain    ( 0.0 ),
    _aoa_gain       ( 0.0 ),
    _yaw_rate_gain  ( 0.0 ),

    _lef_time_const ( 0.0 ),

    _norm_ailerons ( 0.0 ),
    _norm_elevator ( 0.0 ),
    _norm_rudder   ( 0.0 ),
    _norm_flaps_le ( 0.0 ),
    _norm_flaps_te ( 0.0 )
{
    _pid_roll    .setAntiWindup( PID::Calculation );
    _pid_pitch_1 .setAntiWindup( PID::Calculation );
    _pid_pitch_2 .setAntiWindup( PID::Calculation );
    _pid_yaw     .setAntiWindup( PID::Calculation );
}

////////////////////////////////////////////////////////////////////////////////

F35A_FLCS::~F35A_FLCS() {}

////////////////////////////////////////////////////////////////////////////////

void F35A_FLCS::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        // maximum controls rates
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_max_rate_ailerons , "max_rate_ailerons" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_max_rate_elevator , "max_rate_elevator" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_max_rate_rudder   , "max_rate_rudder"   );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_max_rate_flaps_le , "max_rate_flaps_le" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_max_rate_flaps_te , "max_rate_flaps_te" );

        XmlNode nodeRoll  = dataNode.getFirstChildElement( "ctrl_roll"  );
        XmlNode nodePitch = dataNode.getFirstChildElement( "ctrl_pitch" );
        XmlNode nodeYaw   = dataNode.getFirstChildElement( "ctrl_yaw"   );

        // roll
        if ( nodeRoll.isValid() )
        {
            double ctrl_tc = 0.0;
            double rate_tc = 0.0;

            if ( result == FDM_SUCCESS ) result = XmlUtils::read( nodeRoll, &ctrl_tc, "ctrl_tc" );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( nodeRoll, &rate_tc, "rate_tc" );

            _lag_ctrl_roll.setTimeConst( ctrl_tc );
            _lag_rate_roll.setTimeConst( rate_tc );

            if ( result == FDM_SUCCESS ) result = XmlUtils::read( nodeRoll, &_ctrl_input_roll, "ctrl_input" );

            if ( result == FDM_SUCCESS ) result = XmlUtils::read( nodeRoll, &_roll_rate_gain, "roll_rate_gain" );

            XmlNode nodePID = nodeRoll.getFirstChildElement( "pid" );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( nodePID, &_pid_roll, -1.0, 1.0 );
        }
        else
        {
            result = FDM_FAILURE;
        }

        // pitch
        if ( nodePitch.isValid() )
        {
            double ctrl_tc = 0.0;
            double rate_tc = 0.0;

            if ( result == FDM_SUCCESS ) result = XmlUtils::read( nodePitch, &ctrl_tc, "ctrl_tc" );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( nodePitch, &rate_tc, "rate_tc" );

            _lag_ctrl_pitch.setTimeConst( ctrl_tc );
            _lag_rate_pitch.setTimeConst( rate_tc );

            if ( result == FDM_SUCCESS ) result = XmlUtils::read( nodePitch, &_ctrl_input_pitch, "ctrl_input" );

            if ( result == FDM_SUCCESS ) result = XmlUtils::read( nodePitch, &_gforce_gain , "gforce_gain" );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( nodePitch, &_aoa_gain    , "aoa_gain"    );

            if ( result == FDM_SUCCESS ) result = XmlUtils::read( nodePitch, &_pitch_rate_gain, "pitch_rate_gain" );
            if ( result == FDM_SUCCESS ) _pitch_rate_gain.multiplyKeys( Units::mb2pa() );

            XmlNode nodePID_1 = nodePitch.getFirstChildElement( "pid_1" );
            XmlNode nodePID_2 = nodePitch.getFirstChildElement( "pid_2" );

            if ( result == FDM_SUCCESS ) result = XmlUtils::read( nodePID_1, &_pid_pitch_1, -1.0, 1.0 );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( nodePID_2, &_pid_pitch_2, -1.0, 1.0 );
        }
        else
        {
            result = FDM_FAILURE;
        }

        // yaw
        if ( nodeYaw.isValid() )
        {
            double ctrl_tc = 0.0;
            double rate_tc = 0.0;

            if ( result == FDM_SUCCESS ) result = XmlUtils::read( nodeYaw, &ctrl_tc, "ctrl_tc" );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( nodeYaw, &rate_tc, "rate_tc" );

            _lag_ctrl_yaw.setTimeConst( ctrl_tc );
            _lag_rate_yaw.setTimeConst( rate_tc );

            if ( result == FDM_SUCCESS ) result = XmlUtils::read( nodeYaw, &_yaw_rate_gain, "yaw_rate_gain" );

            XmlNode nodePID = nodeYaw.getFirstChildElement( "pid" );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( nodePID, &_pid_yaw, -1.0, 1.0 );
        }
        else
        {
            result = FDM_FAILURE;
        }

        // leading edge flaps
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_lef_time_const, "lef_time_const" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_lef_aoa_flaps, "lef_aoa_flaps" );
        if ( result == FDM_SUCCESS ) _lef_aoa_flaps.multiplyKeys( Units::deg2rad() );

        if ( result != FDM_SUCCESS ) XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
    else
    {
        XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
}

////////////////////////////////////////////////////////////////////////////////

void F35A_FLCS::update( double timeStep,
                        double ctrlLat, double trimLat,
                        double ctrlLon, double trimLon,
                        double ctrlYaw, double trimYaw,
                        const Vector3 &omg_bas,
                        const Vector3 &gforce_bas,
                        const Vector3 &grav_bas,
                        double angleOfAttack,
                        double statPress, double dynPress,
                        bool lg_handle_dn )
{
    if ( timeStep > 0.0 )
    {
        _timeStep = timeStep;

        updateCtrlRoll  ( timeStep, ctrlLat, trimLat, omg_bas );
        updateCtrlPitch ( timeStep, ctrlLon, trimLon, omg_bas, gforce_bas, grav_bas, statPress, dynPress, angleOfAttack );
        updateCtrlYaw   ( timeStep, ctrlYaw, trimYaw, omg_bas );

        updateFlapsLE( timeStep, angleOfAttack );
        updateFlapsTE( timeStep, lg_handle_dn  );
    }
}

////////////////////////////////////////////////////////////////////////////////

void F35A_FLCS::updateCtrlRoll( double timeStep,
                                double ctrlLat, double trimLat,
                                const Vector3 &omg_bas )
{
    _lag_ctrl_roll.update( timeStep, ctrlLat );
    _lag_rate_roll.update( timeStep, omg_bas.p() );

    double ctrl_filtered = _lag_ctrl_roll.getValue();
    double rate_filtered = _lag_rate_roll.getValue();

    double command = _ctrl_input_roll.getValue( ctrl_filtered );

    double error = command - _roll_rate_gain * rate_filtered;

    _pid_roll.update( timeStep, error );

    double setpoint = -_pid_roll.getValue();

    _norm_ailerons = Misc::rate( timeStep, _max_rate_ailerons, _norm_ailerons, setpoint );
    _norm_ailerons = Misc::satur( -1.0, 1.0, _norm_ailerons );
}

////////////////////////////////////////////////////////////////////////////////

void F35A_FLCS::updateCtrlPitch( double timeStep,
                                 double ctrlLon, double trimLon,
                                 const Vector3 &omg_bas,
                                 const Vector3 &gforce_bas,
                                 const Vector3 &grav_bas,
                                 double statPress, double dynPress,
                                 double angleOfAttack )
{
    // NASA-TP-1689, p.49

    _lag_ctrl_pitch.update( timeStep, ctrlLon );
    _lag_rate_pitch.update( timeStep, omg_bas.q() );

    double ctrl_filtered = _lag_ctrl_pitch.getValue();
    double rate_filtered = _lag_rate_pitch.getValue();

    double command = _ctrl_input_pitch.getValue( ctrl_filtered );

    double acc_grav = grav_bas.getLength();
    double gz_wo_grav = gforce_bas.z() - ( grav_bas.z() / acc_grav );

    double pitch_rate_gain = _pitch_rate_gain.getValue( dynPress );

    double error = command - ( _gforce_gain * gz_wo_grav
                             + pitch_rate_gain * rate_filtered
                             + _aoa_gain * angleOfAttack );

    _pid_pitch_1.update( timeStep, error );
    _pid_pitch_2.update( timeStep, error );

    double setpoint = -( _pid_pitch_1.getValue() + _pid_pitch_2.getValue() );

    _norm_elevator = Misc::rate( timeStep, _max_rate_elevator , _norm_elevator , setpoint );
    _norm_elevator = Misc::satur( -1.0, 1.0, _norm_elevator );
}

////////////////////////////////////////////////////////////////////////////////

void F35A_FLCS::updateCtrlYaw( double timeStep,
                               double ctrlYaw, double trimYaw,
                               const Vector3 &omg_bas )
{
    _lag_ctrl_yaw.update( timeStep, ctrlYaw );
    _lag_rate_yaw.update( timeStep, omg_bas.r() );

    double ctrl_filtered = _lag_ctrl_yaw.getValue();
    double rate_filtered = _lag_rate_yaw.getValue();

    double error = -_yaw_rate_gain * rate_filtered;

    _pid_yaw.update( timeStep, error );

    double setpoint = -_pid_yaw.getValue() * ( 1.0 - fabs( ctrl_filtered ) ) + ctrl_filtered;

    _norm_rudder = Misc::rate( timeStep, _max_rate_rudder, _norm_rudder, setpoint );
    _norm_rudder = Misc::satur( -1.0, 1.0, _norm_rudder );
}

////////////////////////////////////////////////////////////////////////////////

void F35A_FLCS::updateFlapsLE( double timeStep, double angleOfAttack )
{
    double setpoint = Misc::inertia( _lef_aoa_flaps.getValue( angleOfAttack ),
                                     _norm_flaps_le, timeStep, _lef_time_const );

    _norm_flaps_le = Misc::rate( timeStep, _max_rate_flaps_le, _norm_flaps_le, setpoint );
    _norm_flaps_le = Misc::satur(  0.0, 1.0, _norm_flaps_le );
}

////////////////////////////////////////////////////////////////////////////////

void F35A_FLCS::updateFlapsTE( double timeStep, bool lg_handle_dn )
{
    double setpoint = lg_handle_dn ? 1.0 : 0.0;

    _norm_flaps_te = Misc::rate( timeStep, _max_rate_flaps_te, _norm_flaps_te, setpoint );
    _norm_flaps_te = Misc::satur(  0.0, 1.0, _norm_flaps_te );
}
