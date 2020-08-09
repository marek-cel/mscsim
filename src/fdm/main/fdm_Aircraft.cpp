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

#include <fdm/main/fdm_Aircraft.h>

#include <cstring>

#include <fdm/utils/fdm_GaussJordan.h>

#include <fdm/xml/fdm_XmlDoc.h>
#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Aircraft::Aircraft( const DataInp *dataInp, DataOut *dataOut ) :
    _dataInp ( dataInp ),
    _dataOut ( dataOut ),

    _envir ( FDM_NULLPTR ),
    _isect ( FDM_NULLPTR ),

    _aero ( FDM_NULLPTR ),
    _ctrl ( FDM_NULLPTR ),
    _gear ( FDM_NULLPTR ),
    _mass ( FDM_NULLPTR ),
    _prop ( FDM_NULLPTR ),

    _airspeed_max  ( 0.0 ),
    _load_aero_min ( 0.0 ),
    _load_aero_max ( 0.0 ),
    _load_gear_max ( 0.0 ),

    _stateVect ( FDM_STATE_DIMENSION ),
    _statePrev ( FDM_STATE_DIMENSION ),
    _derivVect ( FDM_STATE_DIMENSION ),

    _integrator ( FDM_NULLPTR ),

    _timeStep ( 0.0 ),

    _crash ( DataOut::NoCrash ),

    _initPropState ( Stopped ),

    _cp_index ( 0 ),

    _elevation     ( 0.0 ),
    _altitude_asl  ( 0.0 ),
    _altitude_agl  ( 0.0 ),
    _roll          ( 0.0 ),
    _pitch         ( 0.0 ),
    _heading       ( 0.0 ),
    _angleOfAttack ( 0.0 ),
    _sideslipAngle ( 0.0 ),
    _climbAngle    ( 0.0 ),
    _trackAngle    ( 0.0 ),
    _slipSkidAngle ( 0.0 ),
    _airspeed      ( 0.0 ),
    _dynPress      ( 0.0 ),
    _ias           ( 0.0 ),
    _tas           ( 0.0 ),
    _groundSpeed   ( 0.0 ),
    _machNumber    ( 0.0 ),
    _climbRate     ( 0.0 ),
    _turnRate      ( 0.0 ),
    _headingPrev   ( 0.0 )
{
    memset( _dataOut, 0, sizeof(DataOut) );

    _envir = new Environment();
    _isect = new Intersections();

    _integrator = new Aircraft::Integrator( this, &Aircraft::computeStateDeriv );
}

////////////////////////////////////////////////////////////////////////////////

Aircraft::~Aircraft()
{
    FDM_DELPTR( _envir );
    FDM_DELPTR( _isect );

    FDM_DELPTR( _integrator );
}

////////////////////////////////////////////////////////////////////////////////

void Aircraft::initialize( bool engineOn )
{
    _initPropState = engineOn ? Running : Stopped;

    _aero->initialize();
    _ctrl->initialize();
    _gear->initialize();
    _mass->initialize();
    _prop->initialize();

    updateVariables( _stateVect, _derivVect );
}

////////////////////////////////////////////////////////////////////////////////

void Aircraft::update( double timeStep )
{
    _timeStep = timeStep;

    try
    {
        anteIntegration();

        /////////////////////////////////////////////////
        _integrator->integrate( _timeStep, &_stateVect );
        /////////////////////////////////////////////////

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

    updateOutputData();
}

////////////////////////////////////////////////////////////////////////////////

void Aircraft::updateFrozen( double timeStep )
{
    _timeStep = timeStep;

    try
    {
        anteIntegration();

        _aero->computeForceAndMoment();
        _gear->computeForceAndMoment();
        _mass->computeForceAndMoment();
        _prop->computeForceAndMoment();

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

    updateOutputData();
}

////////////////////////////////////////////////////////////////////////////////

void Aircraft::setStateVector( const StateVector &stateVector )
{
    _stateVect = stateVector;
    _statePrev = _stateVect;

    anteIntegration();
    computeStateDeriv( _stateVect, &_derivVect );

    updateOutputData();
}

////////////////////////////////////////////////////////////////////////////////

void Aircraft::readFile( const char *dataFile )
{
    XmlDoc doc( dataFile );

    if ( doc.isOpen() )
    {
        XmlNode rootNode = doc.getRootNode();

        if ( rootNode.isValid() )
        {
            readData( rootNode );
        }
        else
        {
            Exception e;

            e.setType( Exception::FileReadingError );
            e.setInfo( "Reading file \"" + std::string( dataFile ) + "\" failed. Invalid root node." );

            FDM_THROW( e );
        }
    }
    else
    {
        Exception e;

        e.setType( Exception::FileReadingError );
        e.setInfo( "Reading file \"" + std::string( dataFile ) + "\" failed." );

        FDM_THROW( e );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Aircraft::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        // collision points
        XmlNode nodeCollisionPoints = dataNode.getFirstChildElement( "collision_points"  );

        if ( nodeCollisionPoints.isValid() )
        {
            XmlNode nodeCollisionPoint = nodeCollisionPoints.getFirstChildElement( "collision_point" );

            while ( result == FDM_SUCCESS && nodeCollisionPoint.isValid() )
            {
                Vector3 collision_point;

                result = XmlUtils::read( nodeCollisionPoint, collision_point );

                if ( result == FDM_SUCCESS )
                {
                    _cp.push_back( collision_point );
                    nodeCollisionPoint = nodeCollisionPoint.getNextSiblingElement( "collision_point" );
                }
            }
        }

        // limitations
        XmlNode nodeLimitations = dataNode.getFirstChildElement( "limitations" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( nodeLimitations, _airspeed_max  , "airspeed_max"  );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( nodeLimitations, _load_aero_min , "load_aero_min" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( nodeLimitations, _load_aero_max , "load_aero_max" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( nodeLimitations, _load_gear_max , "load_gear_max" );

        // pilot position
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _pos_pilot_bas, "pilot_position" );

        if ( result != FDM_SUCCESS )
        {
            XmlUtils::throwError( __FILE__, __LINE__, dataNode );
        }
        else
        {
            XmlNode nodeAero = dataNode.getFirstChildElement( "aerodynamics" );
            XmlNode nodeCtrl = dataNode.getFirstChildElement( "controls"     );
            XmlNode nodeGear = dataNode.getFirstChildElement( "landing_gear" );
            XmlNode nodeMass = dataNode.getFirstChildElement( "mass"         );
            XmlNode nodeProp = dataNode.getFirstChildElement( "propulsion"   );

            _aero->readData( nodeAero );
            _ctrl->readData( nodeCtrl );
            _gear->readData( nodeGear );
            _mass->readData( nodeMass );
            _prop->readData( nodeProp );
        }
    }
    else
    {
        XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Aircraft::anteIntegration()
{
    updateVariables( _stateVect, _derivVect );

    _aero->update();
    _ctrl->update();
    _gear->update();
    _mass->update();
    _prop->update();
}

////////////////////////////////////////////////////////////////////////////////

void Aircraft::postIntegration()
{
    _att_wgs.normalize();

    _stateVect( _is_e0 ) = _att_wgs.e0();
    _stateVect( _is_ex ) = _att_wgs.ex();
    _stateVect( _is_ey ) = _att_wgs.ey();
    _stateVect( _is_ez ) = _att_wgs.ez();

    if ( _stateVect.isValid() )
    {
        if ( _timeStep > 1.0e-9 )
        {
            _derivVect = ( _stateVect - _statePrev   ) / _timeStep;
            _turnRate  = ( _heading   - _headingPrev ) / _timeStep;
        }

        _statePrev = _stateVect;
        _headingPrev = _heading;

        updateVariables( _stateVect, _derivVect );
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
    if ( _crash == DataOut::NoCrash )
    {
        if ( _isect->isIntersection( _pos_wgs, _pos_wgs + _bas2wgs * _cp.at( _cp_index ), true ) )
        {
            _crash = DataOut::Collision;
        }

        _cp_index++;

        if ( !( _cp_index < _cp.size() ) ) _cp_index = 0;
    }

    // detect overspeed
    if ( _crash == DataOut::NoCrash )
    {
        if ( _ias > _airspeed_max )
        {
            _crash = DataOut::Overspeed;
        }
    }

    // detect overload
    if ( _crash == DataOut::NoCrash )
    {
        double weight_inv = 1.0 / ( _mass->getMass() * WGS84::_g );

        double load_factor_aero = _aero->getFor_aero().z() * weight_inv;
        double load_factor_gear = _gear->getFor_BAS().getLength() * weight_inv;

        if ( load_factor_aero > _load_aero_max
          || load_factor_aero < _load_aero_min
          || load_factor_gear > _load_gear_max )
        {
            _crash = DataOut::Overstress;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Aircraft::updateOutputData()
{
    // flight data
    _dataOut->flight.latitude  = _wgs.getPos_Geo().lat;
    _dataOut->flight.longitude = _wgs.getPos_Geo().lon;

    _dataOut->flight.altitude_asl = _altitude_asl;
    _dataOut->flight.altitude_agl = _altitude_agl;

    _dataOut->flight.roll    = _roll;
    _dataOut->flight.pitch   = _pitch;
    _dataOut->flight.heading = _heading;

    _dataOut->flight.angleOfAttack = _angleOfAttack;
    _dataOut->flight.sideslipAngle = _sideslipAngle;

    _dataOut->flight.climbAngle = _climbAngle;
    _dataOut->flight.trackAngle = _trackAngle;

    _dataOut->flight.slipSkidAngle = _slipSkidAngle;

    _dataOut->flight.airspeed    = _airspeed;
    _dataOut->flight.ias         = _ias;
    _dataOut->flight.tas         = _tas;
    _dataOut->flight.groundSpeed = _groundSpeed;
    _dataOut->flight.machNumber  = _machNumber;
    _dataOut->flight.climbRate   = _climbRate;

    _dataOut->flight.rollRate  = _omg_bas.p();
    _dataOut->flight.pitchRate = _omg_bas.q();
    _dataOut->flight.yawRate   = _omg_bas.r();
    _dataOut->flight.turnRate  = _turnRate;

    _dataOut->flight.pos_x_wgs = _pos_wgs.x();
    _dataOut->flight.pos_y_wgs = _pos_wgs.y();
    _dataOut->flight.pos_z_wgs = _pos_wgs.z();

    _dataOut->flight.att_e0_wgs = _att_wgs.e0();
    _dataOut->flight.att_ex_wgs = _att_wgs.ex();
    _dataOut->flight.att_ey_wgs = _att_wgs.ey();
    _dataOut->flight.att_ez_wgs = _att_wgs.ez();

    _dataOut->flight.vel_u_bas = _vel_bas.u();
    _dataOut->flight.vel_v_bas = _vel_bas.v();
    _dataOut->flight.vel_w_bas = _vel_bas.w();

    _dataOut->flight.omg_p_bas = _omg_bas.p();
    _dataOut->flight.omg_q_bas = _omg_bas.q();
    _dataOut->flight.omg_r_bas = _omg_bas.r();

    _dataOut->flight.phi_wgs = _angles_wgs.phi();
    _dataOut->flight.tht_wgs = _angles_wgs.tht();
    _dataOut->flight.psi_wgs = _angles_wgs.psi();

    _dataOut->flight.airspeed_u_bas = _vel_air_bas.u();
    _dataOut->flight.airspeed_v_bas = _vel_air_bas.v();
    _dataOut->flight.airspeed_w_bas = _vel_air_bas.w();

    _dataOut->flight.vel_north = _vel_ned.x();
    _dataOut->flight.vel_east  = _vel_ned.y();

    _dataOut->flight.acc_x_bas = _acc_bas.x();
    _dataOut->flight.acc_y_bas = _acc_bas.y();
    _dataOut->flight.acc_z_bas = _acc_bas.z();

    _dataOut->flight.g_force_x = _g_force.x();
    _dataOut->flight.g_force_y = _g_force.y();
    _dataOut->flight.g_force_z = _g_force.z();

    _dataOut->flight.g_pilot_x = _g_pilot.x();
    _dataOut->flight.g_pilot_y = _g_pilot.y();
    _dataOut->flight.g_pilot_z = _g_pilot.z();

    _dataOut->flight.onGround = _gear->getOnGround();
    _dataOut->flight.stall    = _aero->getStall();

    // environment data
    _dataOut->environment.air_pressure    = _envir->getPressure();
    _dataOut->environment.air_density     = _envir->getDensity();
    _dataOut->environment.air_temperature = _envir->getTemperature();

    // crash
    _dataOut->crash = _crash;
}

////////////////////////////////////////////////////////////////////////////////

void Aircraft::computeStateDeriv( const StateVector &stateVect,
                                  StateVector *derivVect )
{
    updateVariables( stateVect, *derivVect );

    // computing forces and moments
    _aero->computeForceAndMoment();
    _gear->computeForceAndMoment();
    _mass->computeForceAndMoment();
    _prop->computeForceAndMoment();

    Vector3 for_bas = _aero->getFor_BAS()
                    + _mass->getFor_BAS()
                    + _gear->getFor_BAS()
                    + _prop->getFor_BAS();

    Vector3 mom_bas = _aero->getMom_BAS()
                    + _mass->getMom_BAS()
                    + _gear->getMom_BAS()
                    + _prop->getMom_BAS();

    // computing position derivatives
    Vector3 pos_dot_wgs = _bas2wgs * _vel_bas;

    (*derivVect)( _is_x ) = pos_dot_wgs.x();
    (*derivVect)( _is_y ) = pos_dot_wgs.y();
    (*derivVect)( _is_z ) = pos_dot_wgs.z();

    // computing attitude derivatives
    Quaternion att_dot_wgs = _att_wgs.getDerivative( _omg_bas );

    (*derivVect)( _is_e0 ) = att_dot_wgs.e0();
    (*derivVect)( _is_ex ) = att_dot_wgs.ex();
    (*derivVect)( _is_ey ) = att_dot_wgs.ey();
    (*derivVect)( _is_ez ) = att_dot_wgs.ez();

    // computing linear and angular velocities derivatives
    double mass = _mass->getMass();
    Matrix3x3 it_bas = _mass->getInertiaTensor();
    Vector3 st_bas = _mass->getFirstMomentOfMass();

    // momentum and angular momentum
    Vector3 p_bas = mass * _vel_bas + ( _omg_bas % st_bas );
    Vector3 h_bas = it_bas * _omg_bas + ( st_bas % _vel_bas );

    // right-hand-sideforce vector
    Vector3 for_rhs = for_bas - ( _omg_bas % p_bas );

    // right-hand-side moment vector
    Vector3 mom_rhs = mom_bas - ( _vel_bas % p_bas ) - ( _omg_bas % h_bas );

    // right-hand-side combined vector
    Vector6 vec_rhs;

    vec_rhs( 0 ) = for_rhs( _ix );
    vec_rhs( 1 ) = for_rhs( _iy );
    vec_rhs( 2 ) = for_rhs( _iz );
    vec_rhs( 3 ) = mom_rhs( _ix );
    vec_rhs( 4 ) = mom_rhs( _iy );
    vec_rhs( 5 ) = mom_rhs( _iz );

    // inertia matrix
    Matrix6x6 mi_bas = _mass->getInertiaMatrix();

    // state derivatives (results)
    Vector6 acc_bas;

    GaussJordan< 6 >::solve( mi_bas, vec_rhs, &acc_bas );

    // Coriolis effect due to Earth rotation
    Vector3 acc_coriolis_bas = -2.0 * ( _wgs2bas * ( WGS84::getOmega_WGS() % _vel_bas ) );

    acc_bas( 0 ) += acc_coriolis_bas.x();
    acc_bas( 1 ) += acc_coriolis_bas.y();
    acc_bas( 2 ) += acc_coriolis_bas.z();

    // rewriting acceleration into state derivatives vector
    for ( int i = _is_u; i < FDM_STATE_DIMENSION; i++ )
    {
        (*derivVect)( i ) = acc_bas( i - _is_u );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Aircraft::updateVariables( const StateVector &stateVect,
                                const StateVector &derivVect )
{
    _pos_wgs.set( stateVect( _is_x ),
                  stateVect( _is_y ),
                  stateVect( _is_z ) );

    _att_wgs.set( stateVect( _is_e0 ),
                  stateVect( _is_ex ),
                  stateVect( _is_ey ),
                  stateVect( _is_ez ) );

    _vel_bas.set( stateVect( _is_u ),
                  stateVect( _is_v ),
                  stateVect( _is_w ) );

    _omg_bas.set( stateVect( _is_p ),
                  stateVect( _is_q ),
                  stateVect( _is_r ) );

    _wgs.setPos_WGS( _pos_wgs );

    _envir->update( _wgs.getPos_Geo().alt );
    _isect->update( _wgs.getPos_Geo().lat, _wgs.getPos_Geo().lon );

    _att_wgs.normalize();

    _wgs2bas = Matrix3x3( _att_wgs );
    _wgs2ned = Matrix3x3( _wgs.getWGS2NED() );
    _ned2bas = Matrix3x3( _wgs.getNED2BAS( _att_wgs ) );
    _bas2wgs = _wgs2bas.getTransposed();
    _ned2wgs = _wgs2ned.getTransposed();
    _bas2ned = _ned2bas.getTransposed();

    _angles_wgs = _wgs2bas.getAngles();
    _angles_ned = _ned2bas.getAngles();

    _vel_ned = _bas2ned * _vel_bas;

    _vel_air_bas = _vel_bas - _envir->getWind_BAS();
    _omg_air_bas = _omg_bas;

    _acc_bas.u() = derivVect( _is_u );
    _acc_bas.v() = derivVect( _is_v );
    _acc_bas.w() = derivVect( _is_w );

    _eps_bas.p() = derivVect( _is_p );
    _eps_bas.q() = derivVect( _is_q );
    _eps_bas.r() = derivVect( _is_r );

    _grav_wgs = _wgs.getGrav_WGS();
    _grav_bas = _wgs2bas * _grav_wgs;

    Vector3 acc_gforce_bas = _acc_bas + ( _omg_bas % _vel_bas );
    Vector3 acc_gpilot_bas = acc_gforce_bas
                           + ( _omg_bas % ( _omg_bas % _pos_pilot_bas ) )
                           + ( _eps_bas % _pos_pilot_bas );

    _g_force = -( acc_gforce_bas - _grav_bas ) / WGS84::_g;
    _g_pilot = -( acc_gpilot_bas - _grav_bas ) / WGS84::_g;

    WGS84::Geo e_isect_geo = _wgs.getPos_Geo();
    e_isect_geo.alt = -1000.0;

    Vector3 ground_wgs;
    Vector3 normal_wgs;

    if ( FDM_SUCCESS == _isect->getIntersection( _pos_wgs, WGS84::geo2wgs( e_isect_geo ),
                                                 &ground_wgs, &normal_wgs ) )
    {
        _ground_wgs = ground_wgs;
        _normal_wgs = normal_wgs;
    }

    _ground_bas = _wgs2bas * ( _ground_wgs - _pos_wgs );
    _normal_bas = _wgs2bas * _normal_wgs;

    _elevation = WGS84( _ground_wgs ).getPos_Geo().alt;

    _altitude_asl = _wgs.getPos_Geo().alt;
    _altitude_agl = _altitude_asl - _elevation;

    _roll    = _angles_ned.phi();
    _pitch   = _angles_ned.tht();
    _heading = _angles_ned.psi();

    _angleOfAttack = Aerodynamics::getAngleOfAttack( _vel_air_bas );
    _sideslipAngle = Aerodynamics::getSideslipAngle( _vel_air_bas );

    double vel_ne = _vel_ned.getLengthXY();

    _trackAngle = atan2( _vel_ned.v(), _vel_ned.u() );
    _climbAngle = atan2( -_vel_ned.w(), vel_ne );

    while ( _trackAngle < 0.0      ) _trackAngle += 2.0*M_PI;
    while ( _trackAngle > 2.0*M_PI ) _trackAngle -= 2.0*M_PI;

    _slipSkidAngle = atan2( -_g_pilot.y(), _g_pilot.z() );

    double dynPressLon = 0.5 * _envir->getDensity() * Misc::pow2( _vel_air_bas.u() );

    _airspeed    = _vel_air_bas.getLength();
    _dynPress    = 0.5 * _envir->getDensity() * Misc::pow2( _airspeed );
    _ias         = sqrt( 2.0 * dynPressLon / fdm::Atmosphere::_std_sl_rho );
    _tas         = _ias * sqrt( Atmosphere::_std_sl_rho / _envir->getDensity() );
    _groundSpeed = _vel_ned.getLengthXY();
    _machNumber  = _envir->getSpeedOfSound() > 0.0 ? ( _airspeed / _envir->getSpeedOfSound() ) : 0.0;
    _climbRate   = -_vel_ned.z();
}
