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

#include <cgi/otw/cgi_Ownship.h>

#include <sstream>
#include <stdio.h>

#include <osg/PositionAttitudeTransform>
#include <osg/Switch>

#include <fdm/fdm_Path.h>
#include <fdm/utils/fdm_Units.h>
#include <fdm/xml/fdm_XmlDoc.h>
#include <fdm/xml/fdm_XmlUtils.h>

#include <Data.h>

#include <cgi/cgi_Defines.h>
#include <cgi/cgi_FindNode.h>
#include <cgi/cgi_Models.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

Ownship::Ownship( Module *parent ) :
    Module( parent )
{
    _pat = new osg::PositionAttitudeTransform();
    _root->addChild( _pat.get() );

    _switch = new osg::Switch();
    _pat->addChild( _switch.get() );

    _switch->setName( "Ownship" );
}

////////////////////////////////////////////////////////////////////////////////

Ownship::~Ownship() {}

////////////////////////////////////////////////////////////////////////////////

void Ownship::update()
{
    if ( 0 != strcmp( _aircraftFile.c_str(), Data::get()->ownship.aircraftFile ) )
    {
        reload();
    }

    if ( Data::get()->camera.viewType == Data::Camera::ViewPilot )
    {
        _switch->setAllChildrenOff();
    }
    else
    {
        _switch->setAllChildrenOn();
    }

    osg::Quat q_wgs( Data::get()->ownship.att_ex_wgs,
                     Data::get()->ownship.att_ey_wgs,
                     Data::get()->ownship.att_ez_wgs,
                     Data::get()->ownship.att_e0_wgs );

    osg::Vec3d r_wgs( Data::get()->ownship.pos_x_wgs,
                      Data::get()->ownship.pos_y_wgs,
                      Data::get()->ownship.pos_z_wgs );

    _pat->setAttitude( q_wgs );
    _pat->setPosition( r_wgs );

    // ailerons
    if ( _aileronL.valid() && _aileronR.valid() )
    {
        _aileronL->setAttitude( osg::Quat( -Data::get()->ownship.ailerons, osg::Y_AXIS ) );
        _aileronR->setAttitude( osg::Quat(  Data::get()->ownship.ailerons, osg::Y_AXIS ) );
    }

    // elevator
    if ( _elevatorL.valid() )
    {
        _elevatorL->setAttitude( osg::Quat( Data::get()->ownship.elevator, osg::Y_AXIS ) );
    }

    if ( _elevatorR.valid() )
    {
        _elevatorR->setAttitude( osg::Quat( Data::get()->ownship.elevator, osg::Y_AXIS ) );
    }

    // rudder
    if ( _rudderL.valid() )
    {
        _rudderL->setAttitude( osg::Quat( Data::get()->ownship.rudder, osg::Z_AXIS ) );
    }

    if ( _rudderR.valid() )
    {
        _rudderR->setAttitude( osg::Quat( Data::get()->ownship.rudder, osg::Z_AXIS ) );
    }

    // elevons
    if ( _elevonL.valid() && _elevonR.valid() )
    {
        float angle_l = Data::get()->ownship.elevator - Data::get()->ownship.elevons;
        float angle_r = Data::get()->ownship.elevator + Data::get()->ownship.elevons;

        _elevonL->setAttitude( osg::Quat( angle_l, osg::Y_AXIS ) );
        _elevonR->setAttitude( osg::Quat( angle_r, osg::Y_AXIS ) );
    }

    // flaperons
    if ( _flaperonL.valid() && _flaperonR.valid() )
    {
        float angle_l = Data::get()->ownship.flaps - Data::get()->ownship.flaperons;
        float angle_r = Data::get()->ownship.flaps + Data::get()->ownship.flaperons;

        _flaperonL->setAttitude( osg::Quat( angle_l, osg::Y_AXIS ) );
        _flaperonR->setAttitude( osg::Quat( angle_r, osg::Y_AXIS ) );
    }

    // flaps
    if ( _flapL.valid() && _flapR.valid() )
    {
        _flapL->setAttitude( osg::Quat( Data::get()->ownship.flaps, osg::Y_AXIS ) );
        _flapR->setAttitude( osg::Quat( Data::get()->ownship.flaps, osg::Y_AXIS ) );
    }

    // lefs
    if ( _lefL.valid() && _lefR.valid() )
    {
        _lefL->setAttitude( osg::Quat( -Data::get()->ownship.lef, osg::Y_AXIS ) );
        _lefR->setAttitude( osg::Quat( -Data::get()->ownship.lef, osg::Y_AXIS ) );
    }

    // airbrake
    if ( _airbrakeP.valid() )
    {
        _airbrakeP->setAttitude( osg::Quat(  Data::get()->ownship.airbrake, osg::Y_AXIS ) );
    }

    if ( _airbrakeN.valid() )
    {
        _airbrakeN->setAttitude( osg::Quat( -Data::get()->ownship.airbrake, osg::Y_AXIS ) );
    }

    // landing gear
    if ( _landingGear.valid() )
    {
        if ( Data::get()->ownship.landingGear > 0.0 )
        {
            _landingGear->setValue( 0, false );
            _landingGear->setValue( 1, true  );

            for ( unsigned int i = 0; i < _landingGearElements.size() && i < _landingGearElementsData.size(); i++ )
            {
                double angle_x = getAngle( Data::get()->ownship.landingGear, &(_landingGearElementsData[ i ].x) );
                double angle_y = getAngle( Data::get()->ownship.landingGear, &(_landingGearElementsData[ i ].y) );
                double angle_z = getAngle( Data::get()->ownship.landingGear, &(_landingGearElementsData[ i ].z) );

                _landingGearElements[ i ]->setAttitude( osg::Quat( angle_x, osg::X_AXIS,
                                                                   angle_y, osg::Y_AXIS,
                                                                   angle_z, osg::Z_AXIS ) );
            }
        }
        else
        {
            _landingGear->setValue( 0, true  );
            _landingGear->setValue( 1, false );
        }
    }

    // propeller
    if ( _propeller1.valid() )
    {
        _propeller1->setAttitude( osg::Quat( -Data::get()->ownship.propeller[ 0 ], osg::X_AXIS ) );
    }

    if ( _propeller2.valid() )
    {
        _propeller2->setAttitude( osg::Quat( -Data::get()->ownship.propeller[ 1 ], osg::X_AXIS ) );
    }

    if ( _propeller3.valid() )
    {
        _propeller3->setAttitude( osg::Quat( -Data::get()->ownship.propeller[ 2 ], osg::X_AXIS ) );
    }

    if ( _propeller4.valid() )
    {
        _propeller4->setAttitude( osg::Quat( -Data::get()->ownship.propeller[ 3 ], osg::X_AXIS ) );
    }

    // main rotor
    if ( _mainRotor.valid() )
    {
        double psi = Data::get()->ownship.mainRotor_coef * Data::get()->ownship.mainRotor_azimuth;
        _mainRotor->setAttitude( osg::Quat( psi, osg::Z_AXIS ) );
    }

    // tail rotor
    if ( _tailRotor.valid() )
    {
        double psi = Data::get()->ownship.tailRotor_coef * Data::get()->ownship.tailRotor_azimuth;
        _tailRotor->setAttitude( osg::Quat( psi, osg::Y_AXIS ) );
    }

    // main rotor blades
    unsigned int bladesCount = _mainRotorBlades.size();
    double psiStep = 2.0*M_PI / (float)bladesCount;

    for ( unsigned int i = 0; i < bladesCount; i++ )
    {
        double psi = Data::get()->ownship.mainRotor_azimuth
                   + (double)(i*psiStep) * Data::get()->ownship.mainRotor_coef;

        double sinPsi = sin( psi );
        double cosPsi = cos( psi );

        // pitching
        double pitching = Data::get()->ownship.mainRotor_collective
                        - Data::get()->ownship.mainRotor_cyclicLon * cosPsi * Data::get()->ownship.mainRotor_coef
                        + Data::get()->ownship.mainRotor_cyclicLat * sinPsi;

        double flapping = Data::get()->ownship.mainRotor_coningAngle
                        + Data::get()->ownship.mainRotor_diskPitch * cosPsi
                        - Data::get()->ownship.mainRotor_diskRoll  * sinPsi * Data::get()->ownship.mainRotor_coef;

        _mainRotorBlades[ i ]->setAttitude( osg::Quat( pitching, osg::X_AXIS,
                                                       flapping, osg::Y_AXIS,
                                                            0.0, osg::Z_AXIS ) );
    }
}

////////////////////////////////////////////////////////////////////////////////

double Ownship::getAngle( double input, LandingGearElementData::AxisData *axisData )
{
    double angle = axisData->angle_max;

    if (  axisData->input_min < input && input < axisData->input_max )
    {
        double coef = ( input - axisData->input_min ) / ( axisData->input_max - axisData->input_min );

        angle = axisData->angle_min + coef * ( axisData->angle_max - axisData->angle_min );
    }
    else if ( input < axisData->input_min  )
    {
        angle = axisData->angle_min;
    }

    return angle;
}

////////////////////////////////////////////////////////////////////////////////

void Ownship::loadModel( const std::string &modelFile )
{
    osg::ref_ptr<osg::Node> model = Models::get( modelFile );

    if ( model.valid() )
    {
        _switch->addChild( model.get() );

        _aileronL = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "AileronL" ) );
        _aileronR = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "AileronR" ) );

        _elevatorL = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "ElevatorL" ) );
        _elevatorR = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "ElevatorR" ) );

        if ( !_elevatorL.valid() )
        {
            _elevatorL = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "Elevator" ) );
        }

        _rudderL = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "RudderL" ) );
        _rudderR = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "RudderR" ) );

        if ( !_rudderL.valid() )
        {
            _rudderL = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "Rudder" ) );
        }

        _elevonL = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "ElevonL" ) );
        _elevonR = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "ElevonR" ) );

        _flaperonL = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "FlaperonL" ) );
        _flaperonR = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "FlaperonR" ) );

        _flapL = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "FlapTEL" ) );
        _flapR = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "FlapTER" ) );

        if ( !_flapL.valid() && !_flapR.valid() )
        {
            _flapL = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "FlapL" ) );
            _flapR = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "FlapR" ) );
        }

        _lefL = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "FlapLEL" ) );
        _lefR = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "FlapLER" ) );

        if ( !_lefL.valid() && !_lefR.valid() )
        {
            _lefL = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "LEFL" ) );
            _lefR = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "LEFR" ) );
        }

        _airbrakeP = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "AirbrakeP" ) );
        _airbrakeN = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "AirbrakeN" ) );

        if ( !_airbrakeP.valid() )
        {
            _airbrakeP = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "Airbrake" ) );
        }

        // landing gear
        _landingGear = dynamic_cast<osg::Switch*>( FindNode::findFirst( model, "LandingGear" ) );

        for ( unsigned int i = 0; i < CGI_MAX_LANDING_GEAR_ELEMENTS; i++ )
        {
            std::stringstream ss;
            ss << "LandingGear" << ( i + 1 );

            osg::ref_ptr<osg::PositionAttitudeTransform> landingGearElement =
                    dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, ss.str() ) );

            if ( landingGearElement.valid() )
            {
                _landingGearElements.push_back( landingGearElement.get() );
            }
            else
            {
                break;
            }
        }

        _propeller1 = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "Propeller1" ) );
        _propeller2 = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "Propeller2" ) );
        _propeller3 = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "Propeller3" ) );
        _propeller4 = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "Propeller4" ) );

        if ( !_propeller1.valid() || !_propeller2.valid() )
        {
            _propeller1 = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "PropellerL" ) );
            _propeller2 = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "PropellerR" ) );
        }

        if ( !_propeller1.valid() )
        {
            _propeller1 = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "Propeller" ) );
        }

        // main and tail rotor
        _mainRotor = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "MainRotor" ) );
        _tailRotor = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "TailRotor" ) );

        // main rotor blades
        FindNode::Nodes bladeNodes = FindNode::findNodes( model, "BladeHinge" );

        for ( unsigned int i = 0; i < bladeNodes.size(); i++ )
        {
            osg::ref_ptr<osg::PositionAttitudeTransform> blade =
                    dynamic_cast<osg::PositionAttitudeTransform*>( bladeNodes.at( i ).get() );

            if ( blade.valid() )
            {
                _mainRotorBlades.push_back( blade.get() );
            }
            else
            {
                break;
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Ownship::readLandingGearElementsData( const fdm::XmlNode &rootNode,
                                           LandingGearElementsData *landingGearElementsData )
{
    for ( unsigned int i = 0; i < CGI_MAX_LANDING_GEAR_ELEMENTS; i++ )
    {
        std::stringstream ss;
        ss << "landing_gear_" << ( i + 1 );

        fdm::XmlNode landingGearNode = rootNode.getFirstChildElement( ss.str() );

        if ( landingGearNode.isValid() )
        {
            LandingGearElementData data;

            memset( &data, 0, sizeof(LandingGearElementData) );

            fdm::XmlNode xAxixNode = landingGearNode.getFirstChildElement( "x" );
            fdm::XmlNode yAxixNode = landingGearNode.getFirstChildElement( "y" );
            fdm::XmlNode zAxixNode = landingGearNode.getFirstChildElement( "z" );

            if ( xAxixNode.isValid() ) readLandingGearElementAxisData( xAxixNode, &(data.x) );
            if ( yAxixNode.isValid() ) readLandingGearElementAxisData( yAxixNode, &(data.y) );
            if ( zAxixNode.isValid() ) readLandingGearElementAxisData( zAxixNode, &(data.z) );

            landingGearElementsData->push_back( data );
        }
        else
        {
            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Ownship::readLandingGearElementAxisData( const fdm::XmlNode &node,
                                              LandingGearElementData::AxisData *axisData )
{
    int result = FDM_SUCCESS;

    double input_min = 0.0;
    double input_max = 0.0;
    double angle_min = 0.0;
    double angle_max = 0.0;

    if ( result == FDM_SUCCESS ) result = fdm::XmlUtils::read( node, input_min, "input_min" );
    if ( result == FDM_SUCCESS ) result = fdm::XmlUtils::read( node, input_max, "input_max" );
    if ( result == FDM_SUCCESS ) result = fdm::XmlUtils::read( node, angle_min, "angle_min" );
    if ( result == FDM_SUCCESS ) result = fdm::XmlUtils::read( node, angle_max, "angle_max" );

    if ( result == FDM_SUCCESS )
    {
        axisData->input_min = input_min;
        axisData->input_max = input_max;

        axisData->angle_min = fdm::Units::deg2rad( angle_min );
        axisData->angle_max = fdm::Units::deg2rad( angle_max );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Ownship::reload()
{
    reset();

    _aircraftFile = Data::get()->ownship.aircraftFile;

    fdm::XmlDoc doc( fdm::Path::get( _aircraftFile ) );

    if ( doc.isOpen() )
    {
        fdm::XmlNode rootNode = doc.getRootNode();

        if ( rootNode.isValid() )
        {
            int result = FDM_SUCCESS;

            std::string modelFile;

            if ( result == FDM_SUCCESS ) result = fdm::XmlUtils::read( rootNode, modelFile, "model" );

            readLandingGearElementsData( rootNode, &_landingGearElementsData );

            if ( result == FDM_SUCCESS )
            {
                loadModel( modelFile );
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Ownship::reset()
{
    if ( _switch->getNumChildren() > 0 )
    {
        _switch->removeChildren( 0, _switch->getNumChildren() );
    }

    _aileronL  = 0;
    _aileronR  = 0;
    _elevatorL = 0;
    _elevatorR = 0;
    _elevonL   = 0;
    _elevonR   = 0;
    _flaperonL = 0;
    _flaperonR = 0;
    _flapL     = 0;
    _flapR     = 0;
    _lefL      = 0;
    _lefR      = 0;
    _rudderL   = 0;
    _rudderR   = 0;

    _propeller1 = 0;
    _propeller2 = 0;
    _propeller3 = 0;
    _propeller4 = 0;

    _mainRotor = 0;
    _tailRotor = 0;

    _landingGearElements.clear();
    _mainRotorBlades.clear();

    _landingGearElementsData.clear();
}
