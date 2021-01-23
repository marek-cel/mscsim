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

#include <fdm_xf/xf_Controls.h>
#include <fdm_xf/xf_Aircraft.h>

#include <fdm/utils/fdm_Units.h>
#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

XF_Controls::XF_Controls( const XF_Aircraft *aircraft, Input *input ) :
    Controls( aircraft, input ),
    _aircraft ( aircraft ),

    _channelRoll       ( FDM_NULLPTR ),
    _channelPitch      ( FDM_NULLPTR ),
    _channelYaw        ( FDM_NULLPTR ),
    _channelRollTrim   ( FDM_NULLPTR ),
    _channelPitchTrim  ( FDM_NULLPTR ),
    _channelYawTrim    ( FDM_NULLPTR ),
    _channelAirbrake   ( FDM_NULLPTR ),
    _channelBrakeLeft  ( FDM_NULLPTR ),
    _channelBrakeRight ( FDM_NULLPTR ),
    _channelNoseWheel  ( FDM_NULLPTR ),

    _flcs ( FDM_NULLPTR ),

    _ailerons_max ( 0.0 ),
    _elevator_max ( 0.0 ),
    _rudder_max   ( 0.0 ),
    _flaps_le_max ( 0.0 ),
    _flaps_te_max ( 0.0 ),

    _ailerons   ( 0.0 ),
    _elevator   ( 0.0 ),
    _rudder     ( 0.0 ),
    _flaps_le   ( 0.0 ),
    _flaps_te   ( 0.0 ),
    _airbrake   ( 0.0 ),
    _brake_l    ( 0.0 ),
    _brake_r    ( 0.0 ),
    _nose_wheel ( 0.0 )
{
    _flcs = new XF_FLCS();
}

////////////////////////////////////////////////////////////////////////////////

XF_Controls::~XF_Controls()
{
    FDM_DELPTR( _flcs );
}

////////////////////////////////////////////////////////////////////////////////

void XF_Controls::readData( XmlNode &dataNode )
{
    ///////////////////////////////
    Controls::readData( dataNode );
    ///////////////////////////////

    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_ailerons_max , "ailerons_max" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_elevator_max , "elevator_max" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_rudder_max   , "rudder_max"   );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_flaps_le_max , "flaps_le_max" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_flaps_te_max , "flaps_te_max" );

        if ( result != FDM_SUCCESS )
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

void XF_Controls::initialize()
{
    _channelRoll       = _channels.getItemByKey( "roll"        );
    _channelPitch      = _channels.getItemByKey( "pitch"       );
    _channelYaw        = _channels.getItemByKey( "yaw"         );
    _channelRollTrim   = _channels.getItemByKey( "roll_trim"   );
    _channelPitchTrim  = _channels.getItemByKey( "pitch_trim"  );
    _channelYawTrim    = _channels.getItemByKey( "yaw_trim"    );
    _channelAirbrake   = _channels.getItemByKey( "airbrake"    );
    _channelBrakeLeft  = _channels.getItemByKey( "brake_left"  );
    _channelBrakeRight = _channels.getItemByKey( "brake_right" );
    _channelNoseWheel  = _channels.getItemByKey( "nose_wheel"  );

    if ( FDM_NULLPTR == _channelRoll
      || FDM_NULLPTR == _channelPitch
      || FDM_NULLPTR == _channelYaw
      || FDM_NULLPTR == _channelRollTrim
      || FDM_NULLPTR == _channelPitchTrim
      || FDM_NULLPTR == _channelYawTrim
      || FDM_NULLPTR == _channelAirbrake
      || FDM_NULLPTR == _channelBrakeLeft
      || FDM_NULLPTR == _channelBrakeRight
      || FDM_NULLPTR == _channelNoseWheel )
    {
        Exception e;

        e.setType( Exception::UnknownException );
        e.setInfo( "Obtaining control channels failed." );

        FDM_THROW( e );
    }

    ///////////////////////
    Controls::initialize();
    ///////////////////////
}

////////////////////////////////////////////////////////////////////////////////

void XF_Controls::update()
{
    ///////////////////
    Controls::update();
    ///////////////////

    _flcs->update( _aircraft->getTimeStep() );

    _ailerons = _ailerons_max * _channelRoll->output;
    _elevator = _elevator_max * _channelPitch->output;
    _rudder   = _rudder_max   * _channelYaw->output;

    _flaps_le = _flaps_le_max * 0.0;
    _flaps_te = _flaps_te_max * 0.0;

    _airbrake = _channelAirbrake->output;

    _brake_l = _channelBrakeLeft  ->output;
    _brake_r = _channelBrakeRight ->output;

    _nose_wheel = _channelNoseWheel->output;
}
