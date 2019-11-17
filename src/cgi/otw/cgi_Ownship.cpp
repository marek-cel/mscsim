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
    updateTraces();
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

void Ownship::readWingTipData( const fdm::XmlNode &node, osg::Vec3 *wing_tip )
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
        wing_tip->set( x, y, z );
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

            if ( wingTipNodeL.isValid() ) readWingTipData( wingTipNodeL, &_wing_tip_l );
            if ( wingTipNodeR.isValid() ) readWingTipData( wingTipNodeR, &_wing_tip_r );

            _double_trace = ( _wing_tip_l.length() + _wing_tip_r.length() ) > 0.0;

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

    _landingGearElements.clear();
    _mainRotorBlades.clear();

    _landingGearElementsData.clear();

    _pos_0_wgs = osg::Vec3d( 0.0, 0.0, 0.0 );

    _wing_tip_l = osg::Vec3( 0.0, 0.0, 0.0 );
    _wing_tip_r = osg::Vec3( 0.0, 0.0, 0.0 );

    _trace_1->clear();
    _trace_2->clear();

    _double_trace = false;
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
