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

#include <fdmMain/fdm_Aircraft.h>

#include <fdmUtils/fdm_GaussJordan.h>

#include <fdmXml/fdm_XmlDoc.h>
#include <fdmXml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Aircraft::Aircraft() :
    DataManager( m_data = new DataNode() ),

    m_envir ( 0 ),
    m_isect ( 0 ),

    m_aero ( 0 ),
    m_ctrl ( 0 ),
    m_gear ( 0 ),
    m_mass ( 0 ),
    m_prop ( 0 ),

    m_airspeed_max  ( 0.0 ),
    m_load_aero_min ( 0.0 ),
    m_load_aero_max ( 0.0 ),
    m_load_gear_max ( 0.0 ),

    m_integrator ( 0 ),

    m_timeStep ( 0.0 ),

    m_crash ( NoCrash ),

    m_elevation     ( 0.0 ),
    m_altitude_asl  ( 0.0 ),
    m_altitude_agl  ( 0.0 ),
    m_roll          ( 0.0 ),
    m_pitch         ( 0.0 ),
    m_heading       ( 0.0 ),
    m_angleOfAttack ( 0.0 ),
    m_sideslipAngle ( 0.0 ),
    m_course        ( 0.0 ),
    m_pathAngle     ( 0.0 ),
    m_slipSkidAngle ( 0.0 ),
    m_airspeed      ( 0.0 ),
    m_machNumber    ( 0.0 ),
    m_climbRate     ( 0.0 ),
    m_turnRate      ( 0.0 )
{
    m_envir = new Environment();
    m_isect = new Intersections();

    m_integrator = new Aircraft::Integrator( this, &Aircraft::computeStateDeriv );
}

////////////////////////////////////////////////////////////////////////////////

Aircraft::~Aircraft()
{
    if ( m_integrator ) delete m_integrator;
    m_integrator = 0;

    if ( m_envir ) delete m_envir;
    m_envir = 0;

    if ( m_isect ) delete m_isect;
    m_isect = 0;

    if ( m_data ) delete m_data;
    m_data = 0;
}

////////////////////////////////////////////////////////////////////////////////

void Aircraft::step( double timeStep )
{
    m_timeStep = timeStep;

    try
    {
        anteIntegration();
        integrate();
        postIntegration();
    }
    catch ( Exception &catched )
    {
        Exception e;

        e.setType( Exception::ExceptionCatched );
        e.setInfo( "Exception catched." );
        e.setCause( catched );

        FDM_THROW( e );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Aircraft::setStateVector( const StateVector &stateVector )
{
    m_stateVect = stateVector;
    m_statePrev = m_stateVect;

    anteIntegration();
    computeStateDeriv( m_stateVect, m_derivVect );
}

////////////////////////////////////////////////////////////////////////////////

void Aircraft::readData( const std::string &dataFilePath )
{
    XmlDoc doc( dataFilePath );

    if ( doc.isOpen() )
    {
        XmlNode rootNode = doc.getRootNode();

        if ( rootNode.isValid() )
        {
            int result = FDM_SUCCESS;

            // collision points
            XmlNode nodeCollisionPoints = rootNode.getFirstChildElement( "collision_points"  );

            if ( nodeCollisionPoints.isValid() )
            {
                XmlNode nodeCollisionPoint = nodeCollisionPoints.getFirstChildElement( "collision_point" );

                while ( result == FDM_SUCCESS && nodeCollisionPoint.isValid() )
                {
                    Vector3 collision_point;

                    result = XmlUtils::read( nodeCollisionPoint, collision_point );

                    if ( result == FDM_SUCCESS )
                    {
                        m_cp.push_back( collision_point );
                        nodeCollisionPoint = nodeCollisionPoint.getNextSiblingElement( "collision_point" );
                    }
                }
            }

            // limitations
            XmlNode nodeLimitations = rootNode.getFirstChildElement( "limitations" );

            if ( result == FDM_SUCCESS ) result = XmlUtils::read( nodeLimitations, m_airspeed_max  , "airspeed_max"  );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( nodeLimitations, m_load_aero_min , "load_aero_min" );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( nodeLimitations, m_load_aero_max , "load_aero_max" );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( nodeLimitations, m_load_gear_max , "load_gear_max" );

            // pilot position
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( rootNode, m_pos_pilot_bas, "pilot_position" );

            if ( result != FDM_SUCCESS )
            {
                Exception e;

                e.setType( Exception::FileReadingError );
                e.setInfo( "Error reading XML file. " + XmlUtils::getErrorInfo( rootNode ) );

                FDM_THROW( e );
            }

            XmlNode nodeAero = rootNode.getFirstChildElement( "aerodynamics" );
            XmlNode nodeCtrl = rootNode.getFirstChildElement( "controls"     );
            XmlNode nodeGear = rootNode.getFirstChildElement( "landing_gear" );
            XmlNode nodeMass = rootNode.getFirstChildElement( "mass"         );
            XmlNode nodeProp = rootNode.getFirstChildElement( "propulsion"   );

            m_aero->readData( nodeAero );
            m_ctrl->readData( nodeCtrl );
            m_gear->readData( nodeGear );
            m_mass->readData( nodeMass );
            m_prop->readData( nodeProp );
        }
        else
        {
            Exception e;

            e.setType( Exception::FileReadingError );
            e.setInfo( "Error reading file \"" + dataFilePath + "\". Invalid root node." );

            FDM_THROW( e );
        }
    }
    else
    {
        Exception e;

        e.setType( Exception::FileReadingError );
        e.setInfo( "Error reading file \"" + dataFilePath + "\"." );

        FDM_THROW( e );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Aircraft::anteIntegration()
{
    updateVariables( m_stateVect, m_derivVect );

    m_envir->update( m_altitude_asl );
    m_isect->update( m_wgs.getPos_Geo().lat, m_wgs.getPos_Geo().lon );

    m_aero->update();
    m_ctrl->update();
    m_gear->update();
    m_mass->update();
    m_prop->update();
}

////////////////////////////////////////////////////////////////////////////////

void Aircraft::integrate()
{
    m_prop->integrate( m_timeStep );

    ///////////////////////////////////////////////////
    m_integrator->integrate( m_timeStep, m_stateVect );
    ///////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////

void Aircraft::postIntegration()
{
    m_att_wgs.normalize();

    m_stateVect( is_e0 ) = m_att_wgs.e0();
    m_stateVect( is_ex ) = m_att_wgs.ex();
    m_stateVect( is_ey ) = m_att_wgs.ey();
    m_stateVect( is_ez ) = m_att_wgs.ez();

    if ( m_stateVect.isValid() )
    {
        m_derivVect = ( m_stateVect - m_statePrev ) / m_timeStep;
        m_statePrev = m_stateVect;

        updateVariables( m_stateVect, m_derivVect );
    }
    else
    {
        Exception e;

        e.setType( Exception::UnexpectedNaN );
        e.setInfo( "NaN detected in the state vector." );

        FDM_THROW( e );
    }

    detectCrash();
}

////////////////////////////////////////////////////////////////////////////////

void Aircraft::detectCrash()
{
    // detect collisions
    if ( m_crash == NoCrash )
    {
        for ( CollisionPoints::iterator it = m_cp.begin(); it != m_cp.end(); it++ )
        {
            if ( m_isect->isIntersection( m_pos_wgs, m_pos_wgs + m_bas2wgs * (*it) ) )
            {
                m_crash = Collision;
                break;
            }
        }
    }

    // detect overspeed
    if ( m_crash == NoCrash )
    {
        if ( m_airspeed > m_airspeed_max )
        {
            m_crash = Overspeed;
        }
    }

    // detect overload
    if ( m_crash == NoCrash )
    {
        double weight_inv = 1.0 / ( m_mass->getMass() * WGS84::m_g );

        double load_factor_aero = m_aero->getFor_aero().z() * weight_inv;
        double load_factor_gear = m_gear->getFor_BAS().getLength() * weight_inv;

        if ( load_factor_aero > m_load_aero_max
          || load_factor_aero < m_load_aero_min
          || load_factor_gear > m_load_gear_max )
        {
            m_crash = Overstressed;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Aircraft::computeStateDeriv( const StateVector &stateVect,
                                  StateVector &derivVect )
{
    updateVariables( stateVect, derivVect );

    // computing forces and moments
    m_aero->computeForceAndMoment();
    m_gear->computeForceAndMoment();
    m_mass->computeForceAndMoment();
    m_prop->computeForceAndMoment();

    Vector3 for_bas = m_aero->getFor_BAS()
                    + m_mass->getFor_BAS()
                    + m_gear->getFor_BAS()
                    + m_prop->getFor_BAS();

    Vector3 mom_bas = m_aero->getMom_BAS()
                    + m_mass->getMom_BAS()
                    + m_gear->getMom_BAS()
                    + m_prop->getMom_BAS();

    // computing position derivatives
    Vector3 pos_dot_wgs = m_bas2wgs * m_vel_bas;

    derivVect( is_x ) = pos_dot_wgs.x();
    derivVect( is_y ) = pos_dot_wgs.y();
    derivVect( is_z ) = pos_dot_wgs.z();

    // computing attitude derivatives
    Quaternion att_dot_wgs = m_att_wgs.getDerivative( m_omg_bas );

    derivVect( is_e0 ) = att_dot_wgs.e0();
    derivVect( is_ex ) = att_dot_wgs.ex();
    derivVect( is_ey ) = att_dot_wgs.ey();
    derivVect( is_ez ) = att_dot_wgs.ez();

    // computing linear and angular velocities derivatives
    double mass = m_mass->getMass();
    Matrix3x3 it_bas = m_mass->getInertiaTensor();
    Vector3 st_bas = m_mass->getFirstMomentOfMass();

    // momentum and angular momentum
    Vector3 p_bas = mass * m_vel_bas + ( m_omg_bas ^ st_bas );
    Vector3 h_bas = it_bas * m_omg_bas + ( st_bas ^ m_vel_bas );

    // right-hand-side force vector
    Vector3 for_rhs = for_bas - ( m_omg_bas ^ p_bas );

    // right-hand-side moment vector
    Vector3 mom_rhs = mom_bas - ( m_vel_bas ^ p_bas ) - ( m_omg_bas ^ h_bas );

    // right-hand-side combined vector
    Vector6 vec_rhs;

    vec_rhs( 0 ) = for_rhs( i_x );
    vec_rhs( 1 ) = for_rhs( i_y );
    vec_rhs( 2 ) = for_rhs( i_z );
    vec_rhs( 3 ) = mom_rhs( i_x );
    vec_rhs( 4 ) = mom_rhs( i_y );
    vec_rhs( 5 ) = mom_rhs( i_z );

    // inertia matrix
    Matrix6x6 mi_bas = m_mass->getInertiaMatrix();

    // state derivatives (results)
    Vector6 acc_bas;

    GaussJordan< 6 >::solve( mi_bas, vec_rhs, acc_bas );

    // Coriolis effect due to Earth rotation
    Vector3 acc_coriolis_bas = -2.0 * ( m_wgs2bas * ( WGS84::getOmega_WGS() ^ m_vel_bas ) );

    acc_bas( 0 ) += acc_coriolis_bas.x();
    acc_bas( 1 ) += acc_coriolis_bas.y();
    acc_bas( 2 ) += acc_coriolis_bas.z();

    // rewriting acceleration into state derivatives vector
    for ( int i = is_u; i < FDM_STATE_DIMENSION; i++ )
    {
        derivVect( i ) = acc_bas( i - is_u );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Aircraft::updateVariables( const StateVector &stateVect,
                                const StateVector &derivVect )
{
    m_pos_wgs.set( stateVect( is_x ),
                   stateVect( is_y ),
                   stateVect( is_z ) );

    m_att_wgs.set( stateVect( is_e0 ),
                   stateVect( is_ex ),
                   stateVect( is_ey ),
                   stateVect( is_ez ) );

    m_vel_bas.set( stateVect( is_u ),
                   stateVect( is_v ),
                   stateVect( is_w ) );

    m_omg_bas.set( stateVect( is_p ),
                   stateVect( is_q ),
                   stateVect( is_r ) );

    m_wgs.setPos_WGS( m_pos_wgs );

    m_att_wgs.normalize();

    m_wgs2bas = m_att_wgs;
    m_bas2wgs = m_wgs2bas.getInverted();
    m_wgs2ned = m_wgs.getWGS2NED();
    m_ned2wgs = m_wgs2ned.getInverted();
    m_ned2bas = m_wgs.getNED2BAS( m_wgs2bas );
    m_bas2ned = m_ned2bas.getInverted();

    m_angles_wgs = m_wgs2bas.getAngles();
    m_angles_ned = m_ned2bas.getAngles();

    m_vel_ned = m_bas2ned * m_vel_bas;

    m_vel_air_bas = m_vel_bas - m_envir->getWind_BAS();
    m_omg_air_bas = m_omg_bas;

    m_acc_bas.x() = derivVect( is_u );
    m_acc_bas.y() = derivVect( is_v );
    m_acc_bas.z() = derivVect( is_w );

    m_eps_bas.x() = derivVect( is_p );
    m_eps_bas.y() = derivVect( is_q );
    m_eps_bas.z() = derivVect( is_r );

    m_grav_wgs = m_wgs.getGrav_WGS();
    m_grav_bas = m_wgs2bas * m_grav_wgs;

    Vector3 acc_gforce_bas = m_acc_bas + ( m_omg_bas ^ m_vel_bas );
    Vector3 acc_gpilot_bas = acc_gforce_bas
                           + ( m_omg_bas ^ ( m_omg_bas ^ m_pos_pilot_bas ) )
                           + ( m_eps_bas ^ m_pos_pilot_bas );

    m_g_force = -( acc_gforce_bas - m_grav_bas ) / WGS84::m_g;
    m_g_pilot = -( acc_gpilot_bas - m_grav_bas ) / WGS84::m_g;

    WGS84::Geo e_isect_geo = m_wgs.getPos_Geo();
    e_isect_geo.alt = -1000.0;

    Vector3 ground_wgs;
    Vector3 normal_wgs;

    if ( FDM_SUCCESS == m_isect->getIntersection( m_pos_wgs, WGS84::geo2wgs( e_isect_geo ),
                                                  ground_wgs, normal_wgs ) )
    {
        m_ground_wgs = ground_wgs;
        m_normal_wgs = normal_wgs;
    }

    m_ground_bas = m_wgs2bas * ( m_ground_wgs - m_pos_wgs );
    m_normal_bas = m_wgs2bas * m_normal_wgs;

    m_elevation = WGS84( m_ground_wgs ).getPos_Geo().alt;

    m_altitude_asl = m_wgs.getPos_Geo().alt;
    m_altitude_agl = m_altitude_asl - m_elevation;

    m_roll    = m_angles_ned.phi();
    m_pitch   = m_angles_ned.tht();
    m_heading = m_angles_ned.psi();

    m_angleOfAttack = Aerodynamics::getAngleOfAttack( m_vel_air_bas );
    m_sideslipAngle = Aerodynamics::getSideslipAngle( m_vel_air_bas );

    double vel_ne = sqrt( m_vel_ned.x()*m_vel_ned.x() + m_vel_ned.y()*m_vel_ned.y() );

    m_course    = atan2( m_vel_ned.y(), m_vel_ned.x() );
    m_pathAngle = atan2( -m_vel_ned.z(), vel_ne );

    while ( m_course < 0.0      ) m_course += 2.0*M_PI;
    while ( m_course > 2.0*M_PI ) m_course -= 2.0*M_PI;

    m_slipSkidAngle = atan2( -m_g_pilot.y(), m_g_pilot.z() );

    m_airspeed   = m_vel_air_bas.getLength();
    m_machNumber = m_airspeed / m_envir->getSpeedOfSound();
    m_climbRate  = -m_vel_ned.z();

    m_turnRate = 0.0;                           // TODO
}
