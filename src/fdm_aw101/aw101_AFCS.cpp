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

#include <fdm_aw101/aw101_AFCS.h>

#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

AW101_AFCS::AW101_AFCS() :
    _pid_sas_roll   ( 0.0, 0.0, 0.0, -1.0, 1.0 ),
    _pid_sas_pitch  ( 0.0, 0.0, 0.0, -1.0, 1.0 ),
    _pid_sas_yaw    ( 0.0, 0.0, 0.0, -1.0, 1.0 ),
    _pid_collective ( 0.0, 0.0, 0.0, -1.0, 1.0 ),

    _cyclic_lat ( 0.0 ),
    _cyclic_lon ( 0.0 ),
    _tail_pitch ( 0.0 ),
    _collective ( 0.0 )
{
    _pid_sas_roll   .setAntiWindup( PID::Calculation );
    _pid_sas_pitch  .setAntiWindup( PID::Calculation );
    _pid_sas_yaw    .setAntiWindup( PID::Calculation );
    _pid_collective .setAntiWindup( PID::Calculation );
}

////////////////////////////////////////////////////////////////////////////////

AW101_AFCS::~AW101_AFCS() {}

////////////////////////////////////////////////////////////////////////////////

void AW101_AFCS::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        XmlNode nodeRoll  = dataNode.getFirstChildElement( "sas_roll"  );
        XmlNode nodePitch = dataNode.getFirstChildElement( "sas_pitch" );
        XmlNode nodeYaw   = dataNode.getFirstChildElement( "sas_yaw"   );

        readSAS( nodeRoll  , &_pid_sas_roll   );
        readSAS( nodePitch , &_pid_sas_pitch  );
        readSAS( nodeYaw   , &_pid_sas_yaw    );

        XmlNode nodeCollective = dataNode.getFirstChildElement( "collective"   );
        readSAS( nodeCollective , &_pid_collective );

        if ( result != FDM_SUCCESS ) XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
    else
    {
        XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
}

////////////////////////////////////////////////////////////////////////////////

void AW101_AFCS::update( double timeStep,
                         double ctrlLat, double trimLat,
                         double ctrlLon, double trimLon,
                         double ctrlYaw, double trimYaw,
                         double climbRate,
                         const Angles &angles_ned,
                         const Vector3 &omg_bas )
{
    if ( timeStep > 0.0 )
    {
        _pid_sas_roll  .update( timeStep, omg_bas.p() );
        _pid_sas_pitch .update( timeStep, omg_bas.q() );
        _pid_sas_yaw   .update( timeStep, omg_bas.r() );

        _pid_collective.update( timeStep, -climbRate );

        _cyclic_lat = _pid_sas_roll  .getValue() * ( 1.0 - fabs( ctrlLat ) );
        _cyclic_lon = _pid_sas_pitch .getValue() * ( 1.0 - fabs( ctrlLon ) );
        _tail_pitch = _pid_sas_yaw   .getValue() * ( 1.0 - fabs( ctrlYaw ) );

        _collective = _pid_collective.getValue();

        _pid_sas_roll  .setValue( timeStep, angles_ned.phi() , _cyclic_lat );
        _pid_sas_pitch .setValue( timeStep, angles_ned.tht() , _cyclic_lon );
        _pid_sas_yaw   .setValue( timeStep, omg_bas.r()      , _tail_pitch );
    }
}

////////////////////////////////////////////////////////////////////////////////

void AW101_AFCS::readSAS( const XmlNode &dataNode, PID *pid )
{
    if ( dataNode.isValid() )
    {
        XmlNode nodePID = dataNode.getFirstChildElement( "pid" );

        if ( FDM_SUCCESS != XmlUtils::read( nodePID, pid, -1.0, 1.0 ) )
        {
            XmlUtils::throwError( __FILE__, __LINE__, dataNode );
        }
    }
    else
    {
        XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
}
