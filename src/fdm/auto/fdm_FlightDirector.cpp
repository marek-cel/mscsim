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

#include <fdm/auto/fdm_FlightDirector.h>

#include <fdm/utils/fdm_Misc.h>
#include <fdm/xml/fdm_XmlUtils.h>

#include <iostream>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

FlightDirector::FlightDirector() :
    _hor_mode ( HM_FD ),
    _ver_mode ( VM_FD ),

    _pid_alt ( 0.0, 0.0, 0.0, -M_PI_2  , M_PI_2  ),
    _pid_ias ( 0.0, 0.0, 0.0, -M_PI_2  , M_PI_2  ),
    _pid_vs  ( 0.0, 0.0, 0.0, -M_PI_2  , M_PI_2  ),
    _pid_arm ( 0.0, 0.0, 0.0, -DBL_MAX , DBL_MAX ),
    _pid_hdg ( 0.0, 0.0, 0.0, -M_PI_2  , M_PI_2  ),

    _max_roll  ( 0.0 ),
    _min_pitch ( 0.0 ),
    _max_pitch ( 0.0 ),

    _max_rate_roll  ( 0.0 ),
    _max_rate_pitch ( 0.0 ),

    _cmd_roll  ( 0.0 ),
    _cmd_pitch ( 0.0 ),

    _altitude  ( 0.0 ),
    _airspeed  ( 0.0 ),
    _heading   ( 0.0 ),
    _course    ( 0.0 ),
    _climbRate ( 0.0 ),
    _pitch     ( 0.0 ),

    _climbRate_act ( 0.0 ),
    _climbRate_tc  ( 0.0 ),

    _min_dh_arm ( 0.0 ),

    _engaged ( false )
{
    _pid_alt .setAntiWindup( PID::Calculation );
    _pid_ias .setAntiWindup( PID::Calculation );
    _pid_vs  .setAntiWindup( PID::Calculation );
    _pid_arm .setAntiWindup( PID::Calculation );
    _pid_hdg .setAntiWindup( PID::Calculation );
}

////////////////////////////////////////////////////////////////////////////////

FlightDirector::~FlightDirector() {}

////////////////////////////////////////////////////////////////////////////////

void FlightDirector::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _max_roll  , "max_roll"  );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _min_pitch , "min_pitch" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _max_pitch , "max_pitch" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _max_rate_roll  , "max_rate_roll"  );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _max_rate_pitch , "max_rate_pitch" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _climbRate_tc, "climb_rate_tc" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _min_dh_arm, "min_dh_arm" );

        if ( result != FDM_SUCCESS ) XmlUtils::throwError( __FILE__, __LINE__, dataNode );

        XmlNode nodeALT = dataNode.getFirstChildElement( "mode_alt" );
        XmlNode nodeIAS = dataNode.getFirstChildElement( "mode_ias" );
        XmlNode nodeVS  = dataNode.getFirstChildElement( "mode_vs"  );
        XmlNode nodeARM = dataNode.getFirstChildElement( "mode_arm" );
        XmlNode nodeHDG = dataNode.getFirstChildElement( "mode_hdg" );

        readMode( nodeALT , _pid_alt , _min_pitch , _max_pitch );
        readMode( nodeIAS , _pid_ias , _min_pitch , _max_pitch );
        readMode( nodeVS  , _pid_vs  , _min_pitch , _max_pitch );

        readMode( nodeARM , _pid_arm , -DBL_MAX   , DBL_MAX );

        readMode( nodeHDG , _pid_hdg , -_max_roll , _max_roll );

        disableHalfBank();
    }
    else
    {
        XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
}

////////////////////////////////////////////////////////////////////////////////

void FlightDirector::update( double timeStep, double heading, double altitude,
                             double airspeed, double climbRate,
                             double deviation_hor, double deviation_ver )
{
    if ( _engaged )
    {
        updateHorFD( timeStep );
        updateHorHDG( timeStep, heading );
        updateHorNAV();
        updateHorAPR();
        updateHorBC();

        updateVerFD( timeStep );
        updateVerALT( timeStep, altitude );
        updateVerIAS( timeStep, airspeed );
        updateVerVS( timeStep, climbRate );
        updateVerARM( timeStep, altitude, climbRate );
        updateVerGS();

        _cmd_roll  = Misc::satur( -_max_roll  , _max_roll  , _cmd_roll  );
        _cmd_pitch = Misc::satur(  _min_pitch , _max_pitch , _cmd_pitch );
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

        _climbRate_act = climbRate;
    }
}

////////////////////////////////////////////////////////////////////////////////

void FlightDirector::disableHalfBank()
{
    _pid_hdg .setMin( -_max_roll );
    _pid_hdg .setMax(  _max_roll );
}

////////////////////////////////////////////////////////////////////////////////

void FlightDirector::enableHalfBank()
{
    double min_roll = -0.5 * _max_roll;
    double max_roll =  0.5 * _max_roll;

    _pid_hdg .setMin( min_roll );
    _pid_hdg .setMax( max_roll );
}

////////////////////////////////////////////////////////////////////////////////

void FlightDirector::toggleHorMode( HorMode hor_mode )
{
    if ( _hor_mode == hor_mode )
        _hor_mode = HM_FD;
    else
        _hor_mode = hor_mode;
}

////////////////////////////////////////////////////////////////////////////////

void FlightDirector::toggleVerMode( VerMode ver_mode )
{
    if ( _ver_mode == ver_mode )
        _ver_mode = VM_FD;
    else
        _ver_mode = ver_mode;
}

////////////////////////////////////////////////////////////////////////////////

void FlightDirector::setAltitude( double altitude )
{
    _altitude = fdm::Misc::max( 0.0, altitude );
}

////////////////////////////////////////////////////////////////////////////////

void FlightDirector::setAirspeed( double airspeed )
{
    _airspeed = fdm::Misc::max( 0.0, airspeed );
}

////////////////////////////////////////////////////////////////////////////////

void FlightDirector::setClimbRate( double climbRate )
{
    _climbRate = climbRate;
}

////////////////////////////////////////////////////////////////////////////////

void FlightDirector::setHeading( double heading )
{
    _heading = heading;
}

////////////////////////////////////////////////////////////////////////////////

void FlightDirector::setCourse( double course )
{
    _course = course;
}

////////////////////////////////////////////////////////////////////////////////

void FlightDirector::setPitch( double pitch )
{
    _pitch = Misc::satur( -M_PI_2, M_PI_2, pitch );
    _cmd_pitch = _pitch;
}

////////////////////////////////////////////////////////////////////////////////

void FlightDirector::readMode( const fdm::XmlNode &dataNode, PID &pid, double min, double max )
{
    if ( dataNode.isValid() )
    {
        XmlNode nodePID = dataNode.getFirstChildElement( "pid" );
        readPID( nodePID, pid, min, max );
    }
    else
    {
        XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
}

////////////////////////////////////////////////////////////////////////////////

void FlightDirector::readPID( const fdm::XmlNode &dataNode, PID &pid, double min, double max )
{
    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        double kp = 0.0;
        double ki = 0.0;
        double kd = 0.0;

        double temp_min = pid.getMin();
        double temp_max = pid.getMax();

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, kp, "kp", true );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, ki, "ki", true );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, kd, "kd", true );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, temp_min, "min", true );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, temp_max, "max", true );

        if ( result == FDM_SUCCESS )
        {
            if ( temp_min < min ) temp_min = min;
            if ( temp_max > max ) temp_max = max;

            pid.setKp( kp );
            pid.setKi( ki );
            pid.setKd( kd );

            pid.setMin( temp_min );
            pid.setMax( temp_max );
        }
        else
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

void FlightDirector::updateHorFD( double timeStep )
{
    if ( _hor_mode == HM_FD )
        _cmd_roll = Misc::rate( timeStep, _max_rate_roll, _cmd_roll, 0.0 );
}

////////////////////////////////////////////////////////////////////////////////

void FlightDirector::updateHorHDG( double timeStep, double heading )
{
    if ( _hor_mode == HM_HDG )
    {
        double delta_hdg = _heading - heading;

        while ( delta_hdg < -M_PI ) delta_hdg += 2.0 * M_PI;
        while ( delta_hdg >  M_PI ) delta_hdg -= 2.0 * M_PI;

        _pid_hdg.update( timeStep, delta_hdg );
        _cmd_roll = Misc::rate( timeStep, _max_rate_roll, _cmd_roll, _pid_hdg.getValue() );
    }
    else
    {
        _pid_hdg.setValue( _cmd_roll );
    }
}

////////////////////////////////////////////////////////////////////////////////

void FlightDirector::updateHorNAV()
{
    if ( _hor_mode == HM_NAV )
    {

    }
    else
    {

    }
}

////////////////////////////////////////////////////////////////////////////////

void FlightDirector::updateHorAPR() {}

////////////////////////////////////////////////////////////////////////////////

void FlightDirector::updateHorBC() {}

////////////////////////////////////////////////////////////////////////////////

void FlightDirector::updateVerFD( double timeStep )
{
    if ( _ver_mode == VM_FD )
        _cmd_pitch = Misc::rate( timeStep, _max_rate_pitch, _cmd_pitch, _pitch );
    else
        _pitch = _cmd_pitch;
}

////////////////////////////////////////////////////////////////////////////////

void FlightDirector::updateVerALT( double timeStep, double altitude )
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

void FlightDirector::updateVerIAS( double timeStep, double airspeed )
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

void FlightDirector::updateVerVS( double timeStep, double climbRate )
{
    if ( _ver_mode == VM_VS || _ver_mode == VM_ARM )
    {
        _climbRate_act = Misc::inertia( _climbRate, _climbRate_act, timeStep, _climbRate_tc );
        _pid_vs.update( timeStep, _climbRate_act - climbRate );
        _cmd_pitch = Misc::rate( timeStep, _max_rate_pitch, _cmd_pitch, _pid_vs.getValue() );
    }
    else
    {
        _pid_vs.setValue( _cmd_pitch );
        _climbRate_act = climbRate;
    }
}

////////////////////////////////////////////////////////////////////////////////

void FlightDirector::updateVerARM( double timeStep, double altitude, double climbRate )
{
    if ( _ver_mode == VM_ARM )
    {
        double delta_h = _altitude - altitude;

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

void FlightDirector::updateVerGS()
{

}
