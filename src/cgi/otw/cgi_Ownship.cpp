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

#include <cgi/otw/cgi_Ownship.h>

#include <sstream>

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/LineWidth>

#include <fdm/fdm_Path.h>
#include <fdm/utils/fdm_Units.h>
#include <fdm/xml/fdm_XmlDoc.h>
#include <fdm/xml/fdm_XmlUtils.h>

#include <Data.h>

#include <cgi/cgi_Colors.h>
#include <cgi/cgi_Defines.h>
#include <cgi/cgi_FindNode.h>
#include <cgi/cgi_Models.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

Ownship::Ownship( const Module *parent ) :
    Module( parent )
{
    _pat = new osg::PositionAttitudeTransform();
    _root->addChild( _pat.get() );

    _switch = new osg::Switch();
    _switch->setName( "Ownship" );
    _pat->addChild( _switch.get() );

    _patRibbons = new osg::PositionAttitudeTransform();
    _root->addChild( _patRibbons.get() );

    _switchRibbons = new osg::Switch();
    _patRibbons->addChild( _switchRibbons.get() );

    _switchRotor = new osg::Switch();
    _pat->addChild( _switchRotor.get() );

    _trace_1 = new osg::Vec3Array();
    _trace_2 = new osg::Vec3Array();
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

    if ( Data::get()->cgi.viewType == Data::CGI::ViewPilot )
    {
        _switch->setAllChildrenOff();
    }
    else
    {
        _switch->setAllChildrenOn();
    }

    _att_wgs = osg::Quat( Data::get()->ownship.att_ex_wgs,
                          Data::get()->ownship.att_ey_wgs,
                          Data::get()->ownship.att_ez_wgs,
                          Data::get()->ownship.att_e0_wgs );

    _pos_wgs = osg::Vec3d( Data::get()->ownship.pos_x_wgs,
                           Data::get()->ownship.pos_y_wgs,
                           Data::get()->ownship.pos_z_wgs );

    _pat->setAttitude( _att_wgs );
    _pat->setPosition( _pos_wgs );

    updateModel();
    updateRotor();
    updateTraces();
}

////////////////////////////////////////////////////////////////////////////////

void Ownship::updateAxis( double input, ElementData::AxisData *axisData )
{
    axisData->angle = axisData->angle_max;
    axisData->shift = axisData->shift_max;

    if (  axisData->input_min <= input && input < axisData->input_max )
    {
        double coef = ( input - axisData->input_min ) / ( axisData->input_max - axisData->input_min );

        axisData->angle = axisData->angle_min + coef * ( axisData->angle_max - axisData->angle_min );
        axisData->shift = axisData->shift_min + coef * ( axisData->shift_max - axisData->shift_min );
    }
    else if ( input < axisData->input_min  )
    {
        axisData->angle = axisData->angle_min;
        axisData->shift = axisData->shift_min;
    }
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

            osg::ref_ptr<osg::PositionAttitudeTransform> gearElement =
                    dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, ss.str() ) );

            if ( gearElement.valid() )
            {
                _gearElements.push_back( gearElement.get() );
            }
            else
            {
                break;
            }
        }

        // flaps
        for ( unsigned int i = 0; i < CGI_MAX_FLAPS_ELEMENTS; i++ )
        {
            std::stringstream ss;
            ss << "Flap" << ( i + 1 );

            osg::ref_ptr<osg::PositionAttitudeTransform> flapElement =
                    dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, ss.str() ) );

            if ( flapElement.valid() )
            {
                _flapElements.push_back( flapElement.get() );
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
                _rotorBlades.push_back( blade.get() );
            }
            else
            {
                break;
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Ownship::readElementsData( const fdm::XmlNode &rootNode, ElementsData *elementsData )
{
    for ( unsigned int i = 0; i < CGI_MAX_LANDING_GEAR_ELEMENTS; i++ )
    {
        std::stringstream ss;
        ss << "element_" << ( i + 1 );

        fdm::XmlNode landingGearNode = rootNode.getFirstChildElement( ss.str() );

        if ( landingGearNode.isValid() )
        {
            ElementData data;

            memset( &data, 0, sizeof(ElementData) );

            fdm::XmlNode xAxixNode = landingGearNode.getFirstChildElement( "x" );
            fdm::XmlNode yAxixNode = landingGearNode.getFirstChildElement( "y" );
            fdm::XmlNode zAxixNode = landingGearNode.getFirstChildElement( "z" );

            if ( xAxixNode.isValid() ) readElementAxisData( xAxixNode, &(data.x) );
            if ( yAxixNode.isValid() ) readElementAxisData( yAxixNode, &(data.y) );
            if ( zAxixNode.isValid() ) readElementAxisData( zAxixNode, &(data.z) );

            elementsData->push_back( data );
        }
        else
        {
            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Ownship::readElementAxisData( const fdm::XmlNode &node, ElementData::AxisData *axisData )
{
    int result = FDM_SUCCESS;

    double input_min = 0.0;
    double input_max = 0.0;
    double angle_min = 0.0;
    double angle_max = 0.0;
    double shift_min = 0.0;
    double shift_max = 0.0;

    if ( result == FDM_SUCCESS ) result = fdm::XmlUtils::read( node, input_min, "input_min" );
    if ( result == FDM_SUCCESS ) result = fdm::XmlUtils::read( node, input_max, "input_max" );
    if ( result == FDM_SUCCESS ) result = fdm::XmlUtils::read( node, angle_min, "angle_min", true );
    if ( result == FDM_SUCCESS ) result = fdm::XmlUtils::read( node, angle_max, "angle_max", true );
    if ( result == FDM_SUCCESS ) result = fdm::XmlUtils::read( node, shift_min, "shift_min", true );
    if ( result == FDM_SUCCESS ) result = fdm::XmlUtils::read( node, shift_max, "shift_max", true );

    if ( result == FDM_SUCCESS )
    {
        axisData->input_min = input_min;
        axisData->input_max = input_max;

        axisData->angle = 0.0;
        axisData->angle_min = fdm::Units::deg2rad( angle_min );
        axisData->angle_max = fdm::Units::deg2rad( angle_max );

        axisData->shift = 0.0;
        axisData->shift_min = shift_min;
        axisData->shift_max = shift_max;
    }
}

////////////////////////////////////////////////////////////////////////////////

void Ownship::readVec3( const fdm::XmlNode &node, osg::Vec3 *vec )
{
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    int result = FDM_SUCCESS;

    if ( result == FDM_SUCCESS ) result = fdm::XmlUtils::read( node, x, "x" );
    if ( result == FDM_SUCCESS ) result = fdm::XmlUtils::read( node, y, "y" );
    if ( result == FDM_SUCCESS ) result = fdm::XmlUtils::read( node, z, "z" );

    if ( result == FDM_SUCCESS )
    {
        vec->set( x, y, z );
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

            fdm::XmlNode wingTipNodeL = rootNode.getFirstChildElement( "wing_tip_l" );
            fdm::XmlNode wingTipNodeR = rootNode.getFirstChildElement( "wing_tip_r" );

            if ( wingTipNodeL.isValid() ) readVec3( wingTipNodeL, &_wing_tip_l );
            if ( wingTipNodeR.isValid() ) readVec3( wingTipNodeR, &_wing_tip_r );

            _double_trace = ( _wing_tip_l.length() + _wing_tip_r.length() ) > 1.0e-9;

            fdm::XmlNode rotorNode = rootNode.getFirstChildElement( "rotor" );

            if ( rotorNode.isValid() )
            {
                _helicopter = true;

                fdm::XmlNode rotorCenterNode = rotorNode.getFirstChildElement( "rotor_center" );

                readVec3( rotorCenterNode, &_rotor_center );

                fdm::XmlUtils::read( rotorNode, _hinge_offset, "hinge_offset" );
                fdm::XmlUtils::read( rotorNode, _rotor_radius, "rotor_radius" );

                double inclination_deg = 0.0;
                fdm::XmlUtils::read( rotorNode, inclination_deg, "inclination" );
                _inclination = fdm::Units::deg2rad( inclination_deg );
            }

            fdm::XmlNode nodeFlaps = rootNode.getFirstChildElement( "flaps" );
            if ( nodeFlaps.isValid() ) readElementsData( nodeFlaps, &_flapData );

            fdm::XmlNode nodeLandingGear = rootNode.getFirstChildElement( "landing_gear" );
            if ( nodeLandingGear.isValid() ) readElementsData( nodeLandingGear, &_gearData );

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

    if ( _switchRibbons->getNumChildren() > 0 )
    {
        _switchRibbons->removeChildren( 0, _switchRibbons->getNumChildren() );
    }

    _aileronL  = NULLPTR;
    _aileronR  = NULLPTR;
    _elevatorL = NULLPTR;
    _elevatorR = NULLPTR;
    _elevonL   = NULLPTR;
    _elevonR   = NULLPTR;
    _flaperonL = NULLPTR;
    _flaperonR = NULLPTR;
    _flapL     = NULLPTR;
    _flapR     = NULLPTR;
    _lefL      = NULLPTR;
    _lefR      = NULLPTR;
    _rudderL   = NULLPTR;
    _rudderR   = NULLPTR;

    _propeller1 = NULLPTR;
    _propeller2 = NULLPTR;
    _propeller3 = NULLPTR;
    _propeller4 = NULLPTR;

    _mainRotor = NULLPTR;
    _tailRotor = NULLPTR;

    _gearElements.clear();
    _flapElements.clear();
    _rotorBlades.clear();

    _gearData.clear();
    _flapData.clear();

    _pos_0_wgs = osg::Vec3d( 0.0, 0.0, 0.0 );

    _wing_tip_l = osg::Vec3( 0.0, 0.0, 0.0 );
    _wing_tip_r = osg::Vec3( 0.0, 0.0, 0.0 );

    _rotor_center = osg::Vec3( 0.0, 0.0, 0.0 );

    _trace_1->clear();
    _trace_2->clear();

    _hinge_offset = 0.0;
    _rotor_radius = 0.0;
    _inclination  = 0.0;

    _double_trace = false;
    _helicopter = false;
}

////////////////////////////////////////////////////////////////////////////////

void Ownship::updateModel()
{
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

    // flaps
    for ( unsigned int i = 0; i < _flapElements.size() && i < _flapData.size(); i++ )
    {
        updateAxis( Data::get()->ownship.norm_flaps, &(_flapData[ i ].x) );
        updateAxis( Data::get()->ownship.norm_flaps, &(_flapData[ i ].y) );
        updateAxis( Data::get()->ownship.norm_flaps, &(_flapData[ i ].z) );

        _flapElements[ i ]->setAttitude( osg::Quat( _flapData[ i ].x.angle, osg::X_AXIS,
                                                    _flapData[ i ].y.angle, osg::Y_AXIS,
                                                    _flapData[ i ].z.angle, osg::Z_AXIS ) );

        _flapElements[ i ]->setPosition( osg::Vec3( _flapData[ i ].x.shift,
                                                    _flapData[ i ].y.shift,
                                                    _flapData[ i ].z.shift ) );
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
        if ( Data::get()->ownship.norm_landingGear > 0.0 )
        {
            _landingGear->setValue( 0, false );
            _landingGear->setValue( 1, true  );

            for ( unsigned int i = 0; i < _gearElements.size() && i < _gearData.size(); i++ )
            {
                updateAxis( Data::get()->ownship.norm_landingGear, &(_gearData[ i ].x) );
                updateAxis( Data::get()->ownship.norm_landingGear, &(_gearData[ i ].y) );
                updateAxis( Data::get()->ownship.norm_landingGear, &(_gearData[ i ].z) );

                _gearElements[ i ]->setAttitude( osg::Quat( _gearData[ i ].x.angle, osg::X_AXIS,
                                                            _gearData[ i ].y.angle, osg::Y_AXIS,
                                                            _gearData[ i ].z.angle, osg::Z_AXIS ) );

                _gearElements[ i ]->setPosition( osg::Vec3( _gearData[ i ].x.shift,
                                                            _gearData[ i ].y.shift,
                                                            _gearData[ i ].z.shift ) );
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
    for ( unsigned int i = 0; i < _rotorBlades.size() && i < FDM_MAX_BLADES; i++ )
    {
        double pitching = Data::get()->ownship.blade[ i ].theta;
        double flapping = Data::get()->ownship.blade[ i ].beta;

        _rotorBlades[ i ]->setAttitude( osg::Quat( pitching, osg::X_AXIS,
                                                   flapping, osg::Y_AXIS,
                                                        0.0, osg::Z_AXIS ) );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Ownship::updateRotor()
{
    if ( _switchRotor->getNumChildren() > 0 )
    {
        _switchRotor->removeChildren( 0, _switchRotor->getNumChildren() );
    }

    if ( _helicopter )
    {
        osg::ref_ptr<osg::PositionAttitudeTransform> patRotor = new osg::PositionAttitudeTransform();
        _switchRotor->addChild( patRotor.get() );

        patRotor->setPosition( _rotor_center );
        patRotor->setAttitude( osg::Quat( -_inclination, osg::Y_AXIS ) );

        osg::ref_ptr<osg::Geode> geode = new osg::Geode();
        patRotor->addChild( geode.get() );

        osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
        geode->addDrawable( geometry.get() );

        osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
        osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();
        osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();

        const double span = _rotor_radius - _hinge_offset;

        for ( int i = 0; i <= 36; i++ )
        {
            double psi = fdm::Units::deg2rad( 10.0 * i );

            double cosPsi = cos( psi );
            double sinPsi = sin( psi );

            double beta = Data::get()->ownship.mainRotor_coningAngle
                        + cosPsi * Data::get()->ownship.mainRotor_diskPitch
                        - sinPsi * Data::get()->ownship.mainRotor_diskRoll;

            double cosBeta = cos( beta );
            double sinBeta = sin( beta );

            double span_real = cosBeta * span;

            double xi = cosPsi * _hinge_offset;
            double xo = cosPsi * ( _hinge_offset + span_real );

            double yi = sinPsi * _hinge_offset;
            double yo = sinPsi * ( _hinge_offset + span_real );

            double zo = sinBeta * span;

            v->push_back( osg::Vec3( xi, yi, 0.0 ) );
            v->push_back( osg::Vec3( xo, yo, -zo ) );
        }

        n->push_back( osg::Vec3( 0.0f, 0.0f, -1.0f ) );
        c->push_back( osg::Vec4( 0.0f, 0.0f, 0.0f, 0.2f ) );

        geometry->setVertexArray( v );
        geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::QUAD_STRIP, 0, v->size() ) );
        geometry->setNormalArray( n.get() );
        geometry->setNormalBinding( osg::Geometry::BIND_OVERALL );
        geometry->setColorArray( c.get() );
        geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

        osg::ref_ptr<osg::StateSet> stateSet = geode->getOrCreateStateSet();
        stateSet->setMode( GL_BLEND, osg::StateAttribute::ON );
        stateSet->setMode( GL_DEPTH_TEST, osg::StateAttribute::OFF|osg::StateAttribute::OVERRIDE );
        stateSet->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
        stateSet->setRenderBinDetails( CGI_DEPTH_SORTED_BIN_WORLD + 11, "RenderBin" );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Ownship::updateTraces()
{
    if ( _switchRibbons->getNumChildren() > 0 )
    {
        _switchRibbons->removeChildren( 0, _switchRibbons->getNumChildren() );
    }

    if ( Data::get()->stateOut == fdm::DataOut::Idle )
    {
        _trace_1->clear();
        _trace_2->clear();

        if ( _switchRibbons->getNumChildren() > 0 )
        {
            _switchRibbons->removeChildren( 0, _switchRibbons->getNumChildren() );
        }
    }
    else if ( Data::get()->stateOut == fdm::DataOut::Ready )
    {
        _pos_0_wgs = _pos_wgs;
        _patRibbons->setPosition( _pos_wgs );
    }
    else if ( Data::get()->stateOut == fdm::DataOut::Working )
    {
        if ( _double_trace )
        {
            _trace_1->push_back( _pos_wgs + _att_wgs * _wing_tip_l - _pos_0_wgs );
            _trace_2->push_back( _pos_wgs + _att_wgs * _wing_tip_r - _pos_0_wgs );
        }
        else
        {
            _trace_1->push_back( _pos_wgs - _pos_0_wgs );
        }
    }

    if ( Data::get()->cgi.traces )
    {
        _switchRibbons->setAllChildrenOn();

        if ( _double_trace )
        {
            updateTrace( _switchRibbons.get(), _trace_1.get(), Colors::_red  );
            updateTrace( _switchRibbons.get(), _trace_2.get(), Colors::_lime );
        }
        else
        {
            updateTrace( _switchRibbons.get(), _trace_1.get(), Colors::_black );
        }
    }
    else
    {
        _switchRibbons->setAllChildrenOff();
    }
}

////////////////////////////////////////////////////////////////////////////////

void Ownship::updateTrace( osg::Group *parent, osg::Vec3Array *positions,
                           const osg::Vec3 &color )
{
    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    parent->addChild( geode.get() );

    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();  // normals
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();  // colors

    n->push_back( osg::Vec3( 0.0f, 0.0f, 1.0f ) );
    c->push_back( osg::Vec4( color, 1.0f ) );

    geometry->setVertexArray( positions );
    geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINE_STRIP, 0, positions->size() ) );
    geometry->setNormalArray( n.get() );
    geometry->setNormalBinding( osg::Geometry::BIND_OVERALL );
    geometry->setColorArray( c.get() );
    geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

    geode->addDrawable( geometry.get() );

    osg::ref_ptr<osg::LineWidth> lineWidth = new osg::LineWidth();
    lineWidth->setWidth( 2.0f );

    geode->getOrCreateStateSet()->setAttributeAndModes( lineWidth, osg::StateAttribute::ON );
}
