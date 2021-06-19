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

#include <fdm_f16/f16_Controls.h>
#include <fdm_f16/f16_Aircraft.h>

#include <fdm/utils/fdm_Units.h>
#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

F16_Controls::F16_Controls( const F16_Aircraft *aircraft, Input *input ) :
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

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &ailerons_max , "ailerons_max" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &elevator_max , "elevator_max" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &rudder_max   , "rudder_max"   );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &flaps_le_max , "flaps_le_max" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_airbrake_max, "airbrake_max" );

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

void F16_Controls::initialize()
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

    _inputLGH = getDataRef( "input.controls.lgh" );

    if ( !_inputLGH.isValid() )
    {
        Exception e;

        e.setType( Exception::UnknownException );
        e.setInfo( "Obtaining input data refs in the controls module failed." );

        FDM_THROW( e );
    }

    ///////////////////////
    Controls::initialize();
    ///////////////////////
}

////////////////////////////////////////////////////////////////////////////////

void F16_Controls::update()
{
    ///////////////////
    Controls::update();
    ///////////////////

    _airbrake_norm = _channelAirbrake->output;
    _airbrake = _airbrake_norm * _airbrake_max;

    _brake_l = _channelBrakeLeft  ->output;
    _brake_r = _channelBrakeRight ->output;

    _nose_wheel = _channelNoseWheel->output;

    // 1000 Hz
    const unsigned int steps = static_cast< unsigned int >( ceil( _aircraft->getTimeStep() / 0.001 ) );

    const double timeStep = _aircraft->getTimeStep() / ( (double)steps );
    const double delta_angleOfAttack = _aircraft->getAngleOfAttack() - _angleOfAttack;
    const double delta_g_y = _aircraft->getGForce().y() - _g_y;
    const double delta_g_z = _aircraft->getGForce().z() - _g_z;
    const double delta_rollRate  = _aircraft->getOmg_BAS().p() - _rollRate;
    const double delta_pitchRate = _aircraft->getOmg_BAS().q() - _pitchRate;
    const double delta_yawRate   = _aircraft->getOmg_BAS().r() - _yawRate;
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
                       _inputLGH.getDatab(),
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
    _rollRate  = _aircraft->getOmg_BAS().p();
    _pitchRate = _aircraft->getOmg_BAS().q();
    _yawRate   = _aircraft->getOmg_BAS().r();
    _ctrlLat = _channelRoll->output;
    _trimLat = _channelRollTrim->output;
    _ctrlLon = _channelPitch->output;
    _trimLon = _channelPitchTrim->output;
    _ctrlYaw = _channelYaw->output;
    _trimYaw = _channelYawTrim->output;
    _statPress = _aircraft->getEnvir()->getPressure();
    _dynPress  = _aircraft->getDynPress();
}
