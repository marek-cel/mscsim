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

#include <cgi/cgi_Ownship.h>

#include <stdio.h>

#include <osg/PositionAttitudeTransform>
#include <osg/Switch>

#include <fdmXml/fdm_XmlDoc.h>
#include <fdmXml/fdm_XmlUtils.h>

#include <Data.h>

#include <cgi/cgi_FindNode.h>
#include <cgi/cgi_Models.h>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

Ownship::Ownship( Module *parent ) :
    Module( parent )
{
    m_pat = new osg::PositionAttitudeTransform();
    m_root->addChild( m_pat.get() );

    m_switch = new osg::Switch();
    m_pat->addChild( m_switch.get() );

    m_switch->setName( "Ownship" );
}

////////////////////////////////////////////////////////////////////////////////

Ownship::~Ownship() {}

////////////////////////////////////////////////////////////////////////////////

void Ownship::update()
{
    if ( 0 != strcmp( m_aircraftFile.c_str(), Data::get()->ownship.aircraftFile ) )
    {
        reload();
    }

    if ( Data::get()->camera.viewType == Data::Camera::ViewPilot )
    {
        m_switch->setAllChildrenOff();
    }
    else
    {
        m_switch->setAllChildrenOn();
    }

    osg::Quat q_wgs( Data::get()->ownship.att_ex_wgs,
                     Data::get()->ownship.att_ey_wgs,
                     Data::get()->ownship.att_ez_wgs,
                     Data::get()->ownship.att_e0_wgs );

    osg::Vec3d r_wgs( Data::get()->ownship.pos_x_wgs,
                      Data::get()->ownship.pos_y_wgs,
                      Data::get()->ownship.pos_z_wgs );

    m_pat->setAttitude( q_wgs );
    m_pat->setPosition( r_wgs );

    // ailerons
    if ( m_aileronL.valid() && m_aileronR.valid() )
    {
        m_aileronL->setAttitude( osg::Quat( -Data::get()->ownship.ailerons, osg::Y_AXIS ) );
        m_aileronR->setAttitude( osg::Quat(  Data::get()->ownship.ailerons, osg::Y_AXIS ) );
    }

    // elevator
    if ( m_elevatorL.valid() )
    {
        m_elevatorL->setAttitude( osg::Quat( Data::get()->ownship.elevator, osg::Y_AXIS ) );
    }

    if ( m_elevatorR.valid() )
    {
        m_elevatorR->setAttitude( osg::Quat( Data::get()->ownship.elevator, osg::Y_AXIS ) );
    }

    // rudder
    if ( m_rudderL.valid() )
    {
        m_rudderL->setAttitude( osg::Quat( Data::get()->ownship.rudder, osg::Z_AXIS ) );
    }

    if ( m_rudderR.valid() )
    {
        m_rudderR->setAttitude( osg::Quat( Data::get()->ownship.rudder, osg::Z_AXIS ) );
    }

    // elevons
    if ( m_elevonL.valid() && m_elevonR.valid() )
    {
        float angle_l = Data::get()->ownship.elevator - Data::get()->ownship.elevons;
        float angle_r = Data::get()->ownship.elevator + Data::get()->ownship.elevons;

        m_elevonL->setAttitude( osg::Quat( angle_l, osg::Y_AXIS ) );
        m_elevonR->setAttitude( osg::Quat( angle_r, osg::Y_AXIS ) );
    }

    // flaperons
    if ( m_flaperonL.valid() && m_flaperonR.valid() )
    {
        float angle_l = Data::get()->ownship.flaps - Data::get()->ownship.flaperons;
        float angle_r = Data::get()->ownship.flaps + Data::get()->ownship.flaperons;

        m_flaperonL->setAttitude( osg::Quat( angle_l, osg::Y_AXIS ) );
        m_flaperonR->setAttitude( osg::Quat( angle_r, osg::Y_AXIS ) );
    }

    // flaps
    if ( m_flapL.valid() && m_flapR.valid() )
    {
        m_flapL->setAttitude( osg::Quat( Data::get()->ownship.flaps, osg::Y_AXIS ) );
        m_flapR->setAttitude( osg::Quat( Data::get()->ownship.flaps, osg::Y_AXIS ) );
    }

    // lefs
    if ( m_lefL.valid() && m_lefR.valid() )
    {
        m_lefL->setAttitude( osg::Quat( -Data::get()->ownship.lef, osg::Y_AXIS ) );
        m_lefR->setAttitude( osg::Quat( -Data::get()->ownship.lef, osg::Y_AXIS ) );
    }

    // airbrake
    if ( m_airbrakeP.valid() )
    {
        m_airbrakeP->setAttitude( osg::Quat(  Data::get()->ownship.airbrake, osg::Y_AXIS ) );
    }

    if ( m_airbrakeN.valid() )
    {
        m_airbrakeN->setAttitude( osg::Quat( -Data::get()->ownship.airbrake, osg::Y_AXIS ) );
    }

    // landing gear
    if ( m_landingGear.valid() )
    {
        if ( Data::get()->ownship.landingGear > 0.99 )
        {
            m_landingGear->setValue( 0, false );
            m_landingGear->setValue( 1, true  );
        }
        else
        {
            m_landingGear->setValue( 0, true  );
            m_landingGear->setValue( 1, false );
        }
    }

    // propeller
    if ( m_propeller1.valid() )
    {
        m_propeller1->setAttitude( osg::Quat( -Data::get()->ownship.propeller[ 0 ], osg::X_AXIS ) );
    }

    if ( m_propeller2.valid() )
    {
        m_propeller2->setAttitude( osg::Quat( -Data::get()->ownship.propeller[ 1 ], osg::X_AXIS ) );
    }

    if ( m_propeller3.valid() )
    {
        m_propeller3->setAttitude( osg::Quat( -Data::get()->ownship.propeller[ 2 ], osg::X_AXIS ) );
    }

    if ( m_propeller4.valid() )
    {
        m_propeller4->setAttitude( osg::Quat( -Data::get()->ownship.propeller[ 3 ], osg::X_AXIS ) );
    }

    // main rotor
    if ( m_mainRotor.valid() )
    {
        double psi = Data::get()->ownship.mainRotor_coef * Data::get()->ownship.mainRotor_azimuth;
        m_mainRotor->setAttitude( osg::Quat( psi, osg::Z_AXIS ) );
    }

    // tail rotor
    if ( m_tailRotor.valid() )
    {
        double psi = Data::get()->ownship.tailRotor_coef * Data::get()->ownship.tailRotor_azimuth;
        m_tailRotor->setAttitude( osg::Quat( psi, osg::Y_AXIS ) );
    }

    // main rotor blades
    unsigned int bladesCount = m_mainRotorBlades.size();
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

        m_mainRotorBlades[ i ]->setAttitude( osg::Quat( pitching, osg::X_AXIS,
                                                        flapping, osg::Y_AXIS,
                                                             0.0, osg::Z_AXIS ) );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Ownship::loadModel( const std::string &modelFile )
{
    osg::ref_ptr<osg::Node> model = Models::get( modelFile );

    if ( model.valid() )
    {
        m_switch->addChild( model.get() );

        m_aileronL = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "AileronL" ) );
        m_aileronR = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "AileronR" ) );

        m_elevatorL = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "ElevatorL" ) );
        m_elevatorR = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "ElevatorR" ) );

        if ( !m_elevatorL.valid() )
        {
            m_elevatorL = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "Elevator" ) );
        }

        m_rudderL = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "RudderL" ) );
        m_rudderR = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "RudderR" ) );

        if ( !m_rudderL.valid() )
        {
            m_rudderL = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "Rudder" ) );
        }

        m_elevonL = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "ElevonL" ) );
        m_elevonR = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "ElevonR" ) );

        m_flaperonL = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "FlaperonL" ) );
        m_flaperonR = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "FlaperonR" ) );

        m_flapL = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "FlapTEL" ) );
        m_flapR = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "FlapTER" ) );

        if ( !m_flapL.valid() && !m_flapR.valid() )
        {
            m_flapL = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "FlapL" ) );
            m_flapR = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "FlapR" ) );
        }

        m_lefL = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "FlapLEL" ) );
        m_lefR = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "FlapLER" ) );

        if ( !m_lefL.valid() && !m_lefR.valid() )
        {
            m_lefL = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "LEFL" ) );
            m_lefR = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "LEFR" ) );
        }

        m_airbrakeP = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "AirbrakeP" ) );
        m_airbrakeN = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "AirbrakeN" ) );

        if ( !m_airbrakeP.valid() )
        {
            m_airbrakeP = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "Airbrake" ) );
        }

        m_landingGear = dynamic_cast<osg::Switch*>( FindNode::findFirst( model, "LandingGear" ) );

        m_propeller1 = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "Propeller1" ) );
        m_propeller2 = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "Propeller2" ) );
        m_propeller3 = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "Propeller3" ) );
        m_propeller4 = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "Propeller4" ) );

        if ( !m_propeller1.valid() || !m_propeller2.valid() )
        {
            m_propeller1 = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "PropellerL" ) );
            m_propeller2 = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "PropellerR" ) );
        }

        if ( !m_propeller1.valid() )
        {
            m_propeller1 = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "Propeller" ) );
        }

        // main and tail rotor
        m_mainRotor = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "MainRotor" ) );
        m_tailRotor = dynamic_cast<osg::PositionAttitudeTransform*>( FindNode::findFirst( model, "TailRotor" ) );

        // main rotor blades
        FindNode::Nodes bladeNodes = FindNode::findNodes( model, "BladeHinge" );

        for ( unsigned int i = 0; i < bladeNodes.size(); i++ )
        {
            osg::ref_ptr<osg::PositionAttitudeTransform> blade =
                    dynamic_cast<osg::PositionAttitudeTransform*>( bladeNodes.at( i ).get() );

            if ( blade.valid() )
            {
                m_mainRotorBlades.push_back( blade.get() );
            }
            else
            {
                break;
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Ownship::reload()
{
    reset();

    m_aircraftFile = Data::get()->ownship.aircraftFile;

    fdm::XmlDoc doc( m_aircraftFile );

    if ( doc.isOpen() )
    {
        fdm::XmlNode rootNode = doc.getRootNode();

        if ( rootNode.isValid() )
        {
            int result = FDM_SUCCESS;

            std::string modelFile;

            if ( result == FDM_SUCCESS ) result = fdm::XmlUtils::read( rootNode, modelFile, "model" );

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
    if ( m_switch->getNumChildren() > 0 )
    {
        m_switch->removeChildren( 0, m_switch->getNumChildren() );
    }

    m_aileronL  = 0;
    m_aileronR  = 0;
    m_elevatorL = 0;
    m_elevatorR = 0;
    m_elevonL   = 0;
    m_elevonR   = 0;
    m_flaperonL = 0;
    m_flaperonR = 0;
    m_flapL     = 0;
    m_flapR     = 0;
    m_lefL      = 0;
    m_lefR      = 0;
    m_rudderL   = 0;
    m_rudderR   = 0;

    m_propeller1 = 0;
    m_propeller2 = 0;
    m_propeller3 = 0;
    m_propeller4 = 0;

    m_mainRotor = 0;
    m_tailRotor = 0;

    m_mainRotorBlades.clear();
}
