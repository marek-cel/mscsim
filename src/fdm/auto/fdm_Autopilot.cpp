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

#include <fdm/auto/fdm_Autopilot.h>

#include <fdm/utils/fdm_Misc.h>
#include <fdm/utils/fdm_Units.h>

#include <fdm/xml/fdm_XmlUtils.h>

#include <iostream>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Autopilot::Autopilot( FlightDirector *fd ) :
    _fd ( fd ),

    _pid_r ( 0.0, 0.0, 0.0, -1.0, 1.0 ),
    _pid_p ( 0.0, 0.0, 0.0, -1.0, 1.0 ),
    _pid_y ( 0.0, 0.0, 0.0, -1.0, 1.0 ),

    _max_rate_roll  ( DBL_MAX ),
    _max_rate_pitch ( DBL_MAX ),
    _max_rate_yaw   ( DBL_MAX ),

    _min_roll  ( -M_PI_2  ),
    _max_roll  (  M_PI_2  ),
    _min_pitch ( -M_PI_2  ),
    _max_pitch (  M_PI_2  ),
    _min_alt   (  0.0     ),
    _max_alt   (  DBL_MAX ),
    _min_ias   (  0.0     ),
    _max_ias   (  DBL_MAX ),
    _min_vs    ( -DBL_MAX ),
    _max_vs    (  DBL_MAX ),

    _ctrl_roll  ( 0.0 ),
    _ctrl_pitch ( 0.0 ),
    _ctrl_yaw   ( 0.0 ),

    _yaw_damper ( false ),

    _testing ( false ),
    _engaged ( false )
{
    _pid_r.setAntiWindup( PID::Calculation );
    _pid_p.setAntiWindup( PID::Calculation );
    _pid_y.setAntiWindup( PID::Calculation );

    _gain_ias_r = Table1::oneRecordTable( 1.0 );
    _gain_ias_p = Table1::oneRecordTable( 1.0 );
    _gain_ias_y = Table1::oneRecordTable( 1.0 );
}

////////////////////////////////////////////////////////////////////////////////

Autopilot::~Autopilot() {}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::readData( XmlNode &data_node )
{
    if ( data_node.isValid() )
    {
        int result = FDM_SUCCESS;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( data_node, _min_roll  , "min_roll"  , true );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( data_node, _max_roll  , "max_roll"  , true );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( data_node, _min_pitch , "min_pitch" , true );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( data_node, _max_pitch , "max_pitch" , true );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( data_node, _min_alt   , "min_alt"   , true );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( data_node, _max_alt   , "max_alt"   , true );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( data_node, _min_ias   , "min_ias"   , true );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( data_node, _max_ias   , "max_ias"   , true );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( data_node, _min_vs    , "min_vs"    , true );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( data_node, _max_vs    , "max_vs"    , true );

        if ( result == FDM_SUCCESS )
        {
            XmlNode nodeRoll  = data_node.getFirstChildElement( "ctrl_roll"  );
            XmlNode nodePitch = data_node.getFirstChildElement( "ctrl_pitch" );
            XmlNode nodeYaw   = data_node.getFirstChildElement( "ctrl_yaw"   );

            readChannel( nodeRoll  , _max_rate_roll  , _pid_r, _gain_ias_r );
            readChannel( nodePitch , _max_rate_pitch , _pid_p, _gain_ias_p );
            readChannel( nodeYaw   , _max_rate_yaw   , _pid_y, _gain_ias_y );
        }
        else
        {
            XmlUtils::throwError( __FILE__, __LINE__, data_node );
        }

        XmlNode nodeFlightDirector = data_node.getFirstChildElement( "flight_director" );
        _fd->readData( nodeFlightDirector );
    }
    else
    {
        XmlUtils::throwError( __FILE__, __LINE__, data_node );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::initialize() {}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::update( double time_step,
                        double roll, double pitch, double heading,
                        double altitude, double airspeed,
                        double turn_rate, double yaw_rate, double climb_rate,
                        double dme_distance,
                        double nav_deviation, bool nav_active,
                        double loc_deviation, bool loc_active,
                        double gs_deviation,  bool gs_active )
{
    _fd->update( time_step,
                 heading,
                 altitude, airspeed,
                 turn_rate, climb_rate,
                 dme_distance,
                 nav_deviation, nav_active,
                 loc_deviation, loc_active,
                 gs_deviation, gs_active );

    if ( _engaged && _fd->isEngaged() )
    {
        _pid_r.update( time_step, _fd->getCmdRoll()  - roll  );
        _pid_p.update( time_step, _fd->getCmdPitch() - pitch );
    }
    else
    {
        _pid_r.reset();
        _pid_p.reset();

        _ctrl_roll  = 0.0;
        _ctrl_pitch = 0.0;
    }

    if ( _yaw_damper )
    {
        _pid_y.update( time_step, -yaw_rate );
    }
    else
    {
        _pid_y.reset();
        _ctrl_yaw = 0.0;
    }

    double ctrl_roll  = _gain_ias_r.getValue( airspeed ) * _pid_r.getValue();
    double ctrl_pitch = _gain_ias_p.getValue( airspeed ) * _pid_p.getValue();
    double ctrl_yaw   = _gain_ias_y.getValue( airspeed ) * _pid_y.getValue();

//    if ( _softRide )
//    {
//        ctrl_roll  *= _softRideCoef;
//        ctrl_pitch *= _softRideCoef;
//        ctrl_yaw   *= _softRideCoef;
//    }

    _ctrl_roll  = Misc::rate( time_step, _max_rate_roll  , _ctrl_roll  , ctrl_roll  );
    _ctrl_pitch = Misc::rate( time_step, _max_rate_pitch , _ctrl_pitch , ctrl_pitch );
    _ctrl_yaw   = Misc::rate( time_step, _max_rate_yaw   , _ctrl_yaw   , ctrl_yaw   );

    _ctrl_roll  = Misc::satur( -1.0, 1.0, _ctrl_roll  );
    _ctrl_pitch = Misc::satur( -1.0, 1.0, _ctrl_pitch );
    _ctrl_yaw   = Misc::satur( -1.0, 1.0, _ctrl_yaw   );
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::setAltitude( double altitude )
{
    _fd->setAltitude( fdm::Misc::satur( _min_alt, _max_alt, altitude ) );
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::setAirspeed( double airspeed )
{
    _fd->setAirspeed( fdm::Misc::satur( _min_ias, _max_ias, airspeed ) );
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::setClimbRate( double climb_rate )
{
    _fd->setClimbRate( fdm::Misc::satur( _min_vs, _max_vs, climb_rate ) );
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::setHeading( double heading )
{
    _fd->setHeading( heading );
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::setCourse( double course )
{
    _fd->setCourse( course );
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::setRoll( double roll )
{
    _fd->setRoll( Misc::satur( _min_roll, _max_roll, roll ) );
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::setPitch( double pitch )
{
    _fd->setPitch( Misc::satur( _min_pitch, _max_pitch, pitch ) );
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::readChannel( const XmlNode &data_node, double &max_rate,
                             PID &pid, Table1 &gain_ias )
{
    if ( data_node.isValid() )
    {
        int result = XmlUtils::read( data_node, max_rate, "max_rate" );

        if ( result != FDM_SUCCESS ) XmlUtils::throwError( __FILE__, __LINE__, data_node );

        XmlNode nodePID = data_node.getFirstChildElement( "pid" );

        if ( FDM_SUCCESS != XmlUtils::read( nodePID, pid, -1.0, 1.0 ) )
        {
            XmlUtils::throwError( __FILE__, __LINE__, data_node );
        }

        result = XmlUtils::read( data_node, gain_ias, "gain_ias", true );

        if ( result != FDM_SUCCESS ) XmlUtils::throwError( __FILE__, __LINE__, data_node );
    }
    else
    {
        XmlUtils::throwError( __FILE__, __LINE__, data_node );
    }
}
