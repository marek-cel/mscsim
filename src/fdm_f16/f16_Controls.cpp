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

#include <fdm_f16/f16_Controls.h>
#include <fdm_f16/f16_Aircraft.h>

#include <fdm/utils/fdm_Units.h>
#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

F16_Controls::F16_Controls( const F16_Aircraft *aircraft ) :
    Controls( aircraft ),
    _aircraft ( aircraft ),

    _channelRoll      ( 0 ),
    _channelPitch     ( 0 ),
    _channelYaw       ( 0 ),
    _channelRollTrim  ( 0 ),
    _channelPitchTrim ( 0 ),
    _channelYawTrim   ( 0 ),
    _channelAirbrake  ( 0 ),
    _channelBrakeL    ( 0 ),
    _channelBrakeR    ( 0 ),
    _channelNoseWheel ( 0 ),

    _flcs ( 0 ),

    _airbrake_max ( 0.0 ),

    _airbrake      ( 0.0 ),
    _airbrake_norm ( 0.0 ),
    _brake_l       ( 0.0 ),
    _brake_r       ( 0.0 ),
    _nose_wheel    ( 0.0 ),

    _angleOfAttack ( 0.0 ),
    _g_y ( 0.0 ),
    _g_z ( 0.0 ),
    _rollRate ( 0.0 ),
    _pitchRate ( 0.0 ),
    _yawRate ( 0.0 ),
    _ctrlLat ( 0.0 ),
    _trimLat ( 0.0 ),
    _ctrlLon ( 0.0 ),
    _trimLon ( 0.0 ),
    _ctrlYaw ( 0.0 ),
    _trimYaw ( 0.0 ),
    _statPress ( 0.0 ),
    _dynPress ( 0.0 )
{
    _flcs = new F16_FLCS();
}

////////////////////////////////////////////////////////////////////////////////

F16_Controls::~F16_Controls()
{
    FDM_DELPTR( _flcs );
}

////////////////////////////////////////////////////////////////////////////////

void F16_Controls::readData( XmlNode &dataNode )
{
    ///////////////////////////////
    Controls::readData( dataNode );
    ///////////////////////////////

    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        double ailerons_max = 0.0;
        double elevator_max = 0.0;
        double rudder_max   = 0.0;
        double flaps_le_max = 0.0;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, ailerons_max , "ailerons_max" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, elevator_max , "elevator_max" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, rudder_max   , "rudder_max"   );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, flaps_le_max , "flaps_le_max" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _airbrake_max, "airbrake_max" );

        if ( result == FDM_SUCCESS )
        {
            _flcs->setAilerons_max( ailerons_max );
            _flcs->setElevator_max( elevator_max );
            _flcs->setRudder_max( rudder_max );
            _flcs->setFlaps_le_max( flaps_le_max );
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

void F16_Controls::init()
{
    _channelRoll      = getChannelByName( "roll"       );
    _channelPitch     = getChannelByName( "pitch"      );
    _channelYaw       = getChannelByName( "yaw"        );
    _channelRollTrim  = getChannelByName( "roll_trim"  );
    _channelPitchTrim = getChannelByName( "pitch_trim" );
    _channelYawTrim   = getChannelByName( "yaw_trim"   );
    _channelAirbrake  = getChannelByName( "airbrake"   );
    _channelBrakeL    = getChannelByName( "brake_l"    );
    _channelBrakeR    = getChannelByName( "brake_r"    );
    _channelNoseWheel = getChannelByName( "nose_wheel" );

    if ( 0 != _channelRoll
      && 0 != _channelPitch
      && 0 != _channelYaw
      && 0 != _channelRollTrim
      && 0 != _channelPitchTrim
      && 0 != _channelYawTrim
      && 0 != _channelAirbrake
      && 0 != _channelBrakeL
      && 0 != _channelBrakeR
      && 0 != _channelNoseWheel )
    {
        _channelRoll      ->input = &_aircraft->getDataInp()->controls.roll;
        _channelPitch     ->input = &_aircraft->getDataInp()->controls.pitch;
        _channelYaw       ->input = &_aircraft->getDataInp()->controls.yaw;
        _channelRollTrim  ->input = &_aircraft->getDataInp()->controls.trim_roll;
        _channelPitchTrim ->input = &_aircraft->getDataInp()->controls.trim_pitch;
        _channelYawTrim   ->input = &_aircraft->getDataInp()->controls.trim_yaw;
        _channelAirbrake  ->input = &_aircraft->getDataInp()->controls.airbrake;
        _channelBrakeL    ->input = &_aircraft->getDataInp()->controls.brake_l;
        _channelBrakeR    ->input = &_aircraft->getDataInp()->controls.brake_r;
        _channelNoseWheel ->input = &_aircraft->getDataInp()->controls.nose_wheel;
    }
    else
    {
        Exception e;

        e.setType( Exception::UnknownException );
        e.setInfo( "Obtaining control channels failed." );

        FDM_THROW( e );
    }

    /////////////////
    Controls::init();
    /////////////////
}

////////////////////////////////////////////////////////////////////////////////

void F16_Controls::update()
{
    ///////////////////
    Controls::update();
    ///////////////////

    _airbrake_norm = _channelAirbrake->output;
    _airbrake = _airbrake_norm * _airbrake_max;

    _brake_l = _channelBrakeL->output;
    _brake_r = _channelBrakeR->output;

    _nose_wheel = _channelNoseWheel->output;

    // 1000 Hz
    const unsigned int steps = ceil( _aircraft->getTimeStep() / 0.001 );

    const double timeStep = _aircraft->getTimeStep() / ( (double)steps );
    const double delta_angleOfAttack = _aircraft->getAngleOfAttack() - _angleOfAttack;
    const double delta_g_y = _aircraft->getGForce().y() - _g_y;
    const double delta_g_z = _aircraft->getGForce().z() - _g_z;
    const double delta_rollRate  = _aircraft->getOmg_BAS()( _ip ) - _rollRate;
    const double delta_pitchRate = _aircraft->getOmg_BAS()( _iq ) - _pitchRate;
    const double delta_yawRate   = _aircraft->getOmg_BAS()( _ir ) - _yawRate;
    const double delta_ctrlLat = _channelRoll->output      - _ctrlLat;
    const double delta_trimLat = _channelRollTrim->output  - _trimLat;
    const double delta_ctrlLon = _channelPitch->output     - _ctrlLon;
    const double delta_trimLon = _channelPitchTrim->output - _trimLon;
    const double delta_ctrlYaw = _channelYaw->output       - _ctrlYaw;
    const double delta_trimYaw = _channelYawTrim->output   - _trimYaw;
    const double delta_statPress = _aircraft->getEnvir()->getPressure() - _statPress;
    const double delta_dynPress  = _aircraft->getDynPress()             - _dynPress;

    for ( unsigned int i = 0; i < steps; i++ )
    {
        const double coef = ( (double)( i + 1 ) ) / ( (double)steps );

        double angleOfAttack = _angleOfAttack + coef * delta_angleOfAttack;
        double g_y = _g_y + coef * delta_g_y;
        double g_z = _g_z + coef * delta_g_z;
        double rollRate  = _rollRate  + coef * delta_rollRate;
        double pitchRate = _pitchRate + coef * delta_pitchRate;
        double yawRate   = _yawRate   + coef * delta_yawRate;
        double ctrlLat   = _ctrlLat   + coef * delta_ctrlLat;
        double trimLat   = _trimLat   + coef * delta_trimLat;
        double ctrlLon   = _ctrlLon   + coef * delta_ctrlLon;
        double trimLon   = _trimLon   + coef * delta_trimLon;
        double ctrlYaw   = _ctrlYaw   + coef * delta_ctrlYaw;
        double trimYaw   = _trimYaw   + coef * delta_trimYaw;
        double statPress = _statPress + coef * delta_statPress;
        double dynPress  = _dynPress  + coef * delta_dynPress;

        _flcs->update( timeStep, angleOfAttack,
                       g_y, g_z,
                       rollRate, pitchRate, yawRate,
                       ctrlLat, trimLat,
                       ctrlLon, trimLon,
                       ctrlYaw, trimYaw,
                       statPress, dynPress,
                       false, false,
                       _aircraft->getDataInp()->controls.lgh,
                       _aircraft->getGear()->getOnGround() );

//        _flcs->update( _aircraft->getTimeStep(),
//                       _aircraft->getAngleOfAttack(),
//                       _aircraft->getGForce().y(), _aircraft->getGForce().z(),
//                       _aircraft->getOmg_BAS()( _i_p ), _aircraft->getOmg_BAS()( _i_q ), _aircraft->getOmg_BAS()( _i_r ),
//                       _channelRoll->output  , _channelRollTrim->output,
//                       _channelPitch->output , _channelPitchTrim->output,
//                       _channelYaw->output   , _channelYawTrim->output,
//                       _aircraft->getEnvir()->getPressure(), _aircraft->getDynPress(),
//                       false, false, _aircraft->getGear()->isDown() );
    }

    _angleOfAttack = _aircraft->getAngleOfAttack();
    _g_y = _aircraft->getGForce().y();
    _g_z = _aircraft->getGForce().z();
    _rollRate  = _aircraft->getOmg_BAS()( _ip );
    _pitchRate = _aircraft->getOmg_BAS()( _iq );
    _yawRate   = _aircraft->getOmg_BAS()( _ir );
    _ctrlLat = _channelRoll->output;
    _trimLat = _channelRollTrim->output;
    _ctrlLon = _channelPitch->output;
    _trimLon = _channelPitchTrim->output;
    _ctrlYaw = _channelYaw->output;
    _trimYaw = _channelYawTrim->output;
    _statPress = _aircraft->getEnvir()->getPressure();
    _dynPress  = _aircraft->getDynPress();
}
