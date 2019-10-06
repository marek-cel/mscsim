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

#include <fdm/auto/fdm_Autopilot.h>

#include <fdm/utils/fdm_Misc.h>
#include <fdm/utils/fdm_Units.h>

#include <fdm/xml/fdm_XmlDoc.h>
#include <fdm/xml/fdm_XmlUtils.h>

#include <iostream>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Autopilot::Autopilot() :
    _fd ( FDM_NULLPTR ),

    _pid_r ( 0.0, 0.0, 0.0, -1.0, 1.0 ),
    _pid_p ( 0.0, 0.0, 0.0, -1.0, 1.0 ),
    _pid_y ( 0.0, 0.0, 0.0, -1.0, 1.0 ),

    _max_rate_roll  ( 0.0 ),
    _max_rate_pitch ( 0.0 ),
    _max_rate_yaw   ( 0.0 ),

    _min_pitch ( 0.0 ),
    _max_pitch ( 0.0 ),
    _min_alt   ( 0.0 ),
    _max_alt   ( 0.0 ),
    _min_ias   ( 0.0 ),
    _max_ias   ( 0.0 ),
    _min_vs    ( 0.0 ),
    _max_vs    ( 0.0 ),

    _rate_pitch ( 0.0 ),
    _rate_alt   ( 0.0 ),
    _rate_ias   ( 0.0 ),
    _rate_vs    ( 0.0 ),

    _softRideCoef ( 0.0 ),

    _ctrl_roll  ( 0.0 ),
    _ctrl_pitch ( 0.0 ),
    _ctrl_yaw   ( 0.0 ),

    _yawDamper ( false ),

    _softRide ( false ),
    _halfBank ( false ),

    _testing ( false ),
    _engaged ( false )
{
    _fd = new FlightDirector();

    _pid_r.setAntiWindup( PID::Calculation );
    _pid_p.setAntiWindup( PID::Calculation );
    _pid_y.setAntiWindup( PID::Calculation );

    _gain_ias_r = Table::createOneRecordTable( 1.0 );
    _gain_ias_p = Table::createOneRecordTable( 1.0 );
    _gain_ias_y = Table::createOneRecordTable( 1.0 );
}

////////////////////////////////////////////////////////////////////////////////

Autopilot::~Autopilot()
{
    FDM_DELPTR( _fd );
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::init() {}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::readData( const std::string &dataFile )
{
    fdm::XmlDoc doc( dataFile );

    if ( doc.isOpen() )
    {
        fdm::XmlNode rootNode = doc.getRootNode();

        if ( rootNode.isValid() )
        {
            int result = FDM_SUCCESS;

            if ( result == FDM_SUCCESS ) result = XmlUtils::read( rootNode, _min_pitch , "min_pitch" );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( rootNode, _max_pitch , "max_pitch" );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( rootNode, _min_alt   , "min_alt"   );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( rootNode, _max_alt   , "max_alt"   );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( rootNode, _min_ias   , "min_ias"   );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( rootNode, _max_ias   , "max_ias"   );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( rootNode, _min_vs    , "min_vs"    );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( rootNode, _max_vs    , "max_vs"    );

            if ( result == FDM_SUCCESS ) result = XmlUtils::read( rootNode, _rate_pitch , "rate_pitch" );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( rootNode, _rate_alt   , "rate_alt"   );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( rootNode, _rate_ias   , "rate_ias"   );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( rootNode, _rate_vs    , "rate_vs"    );

            if ( result == FDM_SUCCESS ) result = XmlUtils::read( rootNode, _softRideCoef, "soft_ride_coef" );

            if ( result == FDM_SUCCESS )
            {
                XmlNode nodeRoll  = rootNode.getFirstChildElement( "ctrl_roll"  );
                XmlNode nodePitch = rootNode.getFirstChildElement( "ctrl_pitch" );
                XmlNode nodeYaw   = rootNode.getFirstChildElement( "ctrl_yaw"   );

                readChannel( nodeRoll  , _max_rate_roll  , _pid_r, _gain_ias_r );
                readChannel( nodePitch , _max_rate_pitch , _pid_p, _gain_ias_p );
                readChannel( nodeYaw   , _max_rate_yaw   , _pid_y, _gain_ias_y );
            }
            else
            {
                XmlUtils::throwError( __FILE__, __LINE__, rootNode );
            }

            XmlNode nodeFlightDirector = rootNode.getFirstChildElement( "flight_director" );
            _fd->readData( nodeFlightDirector );
        }
        else
        {
            Exception e;

            e.setType( Exception::FileReadingError );
            e.setInfo( "Reading file \"" + dataFile + "\" failed. Invalid root node." );

            FDM_THROW( e );
        }
    }
    else
    {
        Exception e;

        e.setType( Exception::FileReadingError );
        e.setInfo( "Reading file \"" + dataFile + "\" failed." );

        FDM_THROW( e );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::update( double timeStep,
                        double roll, double pitch, double heading, double yawRate,
                        double altitude, double airspeed, double climbRate,
                        double deviation_hor, double deviation_ver,
                        bool buttonDN, bool buttonUP )
{
    if ( buttonDN || buttonUP )
    {
        double coef = buttonUP ? 1.0 : -1.0;

        switch ( _fd->getVerMode() )
        {
        case fdm::FlightDirector::VM_FD:
            setPitch( getPitch() + coef * timeStep * _rate_pitch );
            break;

        case fdm::FlightDirector::VM_ALT:
            setAltitude( getAltitude() + coef * timeStep * _rate_alt );
            break;

        case fdm::FlightDirector::VM_IAS:
            setAirspeed( getAirspeed() + coef * timeStep * _rate_ias );
            break;

        case fdm::FlightDirector::VM_VS:
            setClimbRate( getClimbRate() + coef * timeStep * _rate_vs );
            break;

        case fdm::FlightDirector::VM_ARM:
        default:
            // do nothing
            break;
        }
    }

    _fd->update( timeStep, heading, altitude,
                 airspeed, climbRate,
                 deviation_hor, deviation_ver );

    if ( _engaged && _fd->isEngaged() )
    {
        _pid_r.update( timeStep, _fd->getCmdRoll()  - roll  );
        _pid_p.update( timeStep, _fd->getCmdPitch() - pitch );
    }
    else
    {
        _pid_r.reset();
        _pid_p.reset();

        _ctrl_roll  = 0.0;
        _ctrl_pitch = 0.0;
    }

    if ( _yawDamper )
    {
        _pid_y.update( timeStep, -yawRate );
    }
    else
    {
        _pid_y.reset();
        _ctrl_yaw = 0.0;
    }

    double ctrl_roll  = _gain_ias_r.getValue( airspeed ) * _pid_r.getValue();
    double ctrl_pitch = _gain_ias_p.getValue( airspeed ) * _pid_p.getValue();
    double ctrl_yaw   = _gain_ias_y.getValue( airspeed ) * _pid_y.getValue();

    if ( _softRide )
    {
        ctrl_roll  *= _softRideCoef;
        ctrl_pitch *= _softRideCoef;
        ctrl_yaw   *= _softRideCoef;
    }

    _ctrl_roll  = Misc::rate( timeStep, _max_rate_roll  , _ctrl_roll  , ctrl_roll  );
    _ctrl_pitch = Misc::rate( timeStep, _max_rate_pitch , _ctrl_pitch , ctrl_pitch );
    _ctrl_yaw   = Misc::rate( timeStep, _max_rate_yaw   , _ctrl_yaw   , ctrl_yaw   );

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

void Autopilot::setClimbRate( double climbRate )
{
    _fd->setClimbRate( fdm::Misc::satur( _min_vs, _max_vs, climbRate ) );
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::setHeading( double heading )
{
    _fd->setHeading( heading );
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::setPitch( double pitch )
{
    _fd->setPitch( Misc::satur( _min_pitch, _max_pitch, pitch ) );
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::readChannel( const fdm::XmlNode &dataNode, double &max_rate,
                             PID &pid, Table &gain_ias )
{
    if ( dataNode.isValid() )
    {
        int result = XmlUtils::read( dataNode, max_rate, "max_rate" );

        if ( result != FDM_SUCCESS ) XmlUtils::throwError( __FILE__, __LINE__, dataNode );

        XmlNode nodePID = dataNode.getFirstChildElement( "pid" );
        readPID( nodePID, pid );

        result = XmlUtils::read( dataNode, gain_ias, "gain_ias", true );

        if ( result != FDM_SUCCESS ) XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
    else
    {
        XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Autopilot::readPID( const fdm::XmlNode &dataNode, PID &pid )
{
    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        double kp = 0.0;
        double ki = 0.0;
        double kd = 0.0;

        double min = -1.0;
        double max =  1.0;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, kp, "kp", true );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, ki, "ki", true );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, kd, "kd", true );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, min, "min", true );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, max, "max", true );

        if ( result == FDM_SUCCESS )
        {
            pid.setKp( kp );
            pid.setKi( ki );
            pid.setKd( kd );

            pid.setMin( min );
            pid.setMax( max );
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
