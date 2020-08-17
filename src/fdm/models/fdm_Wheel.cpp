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

#include <fdm/models/fdm_Wheel.h>

#include <fdm/utils/fdm_String.h>
#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Wheel::Wheels::Wheels() {}

////////////////////////////////////////////////////////////////////////////////

Wheel::Wheels::~Wheels() {}

////////////////////////////////////////////////////////////////////////////////

int Wheel::Wheels::addWheel( const char *name, Wheel wheel )
{
    std::pair< std::map< std::string, Wheel >::iterator, bool > temp =
            _wheels.insert( std::pair< std::string, Wheel >( name, wheel ) );

    if ( temp.second == true )
    {
        return FDM_SUCCESS;
    }

    return FDM_FAILURE;
}

////////////////////////////////////////////////////////////////////////////////

Wheel* Wheel::Wheels::getWheelByName( const char *name )
{
    std::map< std::string, Wheel >::iterator it = _wheels.find( name );

    if ( it != _wheels.end() )
    {
        return &(it->second);
    }

    return FDM_NULLPTR;
}

////////////////////////////////////////////////////////////////////////////////

Vector3 Wheel::getIntersection( const Vector3 &b, const Vector3 &e,
                                const Vector3 &r, const Vector3 &n )
{
    Vector3 r_i = e;

    double num = n * ( r - b );
    double den = n * ( e - b );

    double u = 0.0;

    if ( fabs( den ) < 10e-15 )
    {
        // segment is parallel to the plane
        if ( fabs( num ) < 10e-15 )
        {
            // segment beginning is on the plane
            r_i = b;
        }
    }
    else
    {
        u = num / den;

        if ( 0.0 <= u && u <= 1.0 )
        {
            r_i = b + u * ( e - b );
        }
    }

    return r_i;
}

////////////////////////////////////////////////////////////////////////////////

double Wheel::getPacejkaCoef( double kappa,
                              double b, double c, double d, double e )
{
    return d * sin( c * atan( b*( 1.0 - e )*kappa + e*atan( b*kappa ) ) );
}

////////////////////////////////////////////////////////////////////////////////

Wheel::Wheel() :
    _input ( FDM_NULLPTR ),

    _k ( 0.0 ),
    _c ( 0.0 ),

    _mu_s ( 0.0 ),
    _mu_k ( 0.0 ),
    _mu_r ( 0.0 ),

    _delta_max ( 0.0 ),

    _d_max ( 0.0 ),
    _v_max ( 1.0 ),

    _steerable ( false ),
    _caster    ( false ),

    _brakeGroup ( None ),

    _d_roll ( 0.0 ),
    _d_slip ( 0.0 ),

    _delta ( 0.0 ),
    _brake ( 0.0 )
{}

////////////////////////////////////////////////////////////////////////////////

Wheel::~Wheel() {}

////////////////////////////////////////////////////////////////////////////////

void Wheel::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        _steerable = String::toBool( dataNode.getAttribute( "steerable" ), false );
        _caster    = String::toBool( dataNode.getAttribute( "caster"    ), false );

        switch ( String::toInt( dataNode.getAttribute( "brake_group" ), 0 ) )
        {
            case 1:  _brakeGroup = Left;  break;
            case 2:  _brakeGroup = Right; break;
            default: _brakeGroup = None;  break;
        }

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _r_a_bas , "attachment_point" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _r_u_bas , "unloaded_wheel"   );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _k       , "stiffness"        );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _c       , "damping"          );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _mu_s    , "friction_static"  );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _mu_k    , "friction_kinetic" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _mu_r    , "friction_rolling" );

        if ( result == FDM_SUCCESS && _steerable )
        {
            result = XmlUtils::read( dataNode, _delta_max, "max_angle", true );
        }

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _d_max, "d_max"  , true );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _v_max, "v_max" , true );

        if ( result != FDM_SUCCESS ) XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
    else
    {
        XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Wheel::computeForceAndMoment( const Vector3 &vel_bas,
                                   const Vector3 &omg_bas,
                                   const Vector3 &r_c_bas,
                                   const Vector3 &n_c_bas,
                                   bool steering, bool antiskid,
                                   double surf_coef )
{
    _for_bas.zeroize();
    _mom_bas.zeroize();

    double deflection_norm = n_c_bas * ( r_c_bas - _r_u_bas );

    if ( deflection_norm > 1.0e-6 )
    {
        Vector3 dir_lon_bas;
        Vector3 dir_lat_bas;

        double cosDelta = 1.0;
        double sinDelta = 0.0;

        double v_norm = 0.0;
        double v_roll = 0.0;
        double v_slip = 0.0;

        calculateVariables( vel_bas, omg_bas, r_c_bas, n_c_bas, steering,
                            &dir_lon_bas, &dir_lat_bas,
                            &cosDelta, &sinDelta, &v_norm, &v_roll, &v_slip );

        // normal force
        double for_norm = _k * deflection_norm - _c * v_norm;

        // friction coefs
        double mu_surf_s = _mu_s * surf_coef;
        double mu_surf_k = _mu_k * surf_coef;

        double mu_roll_t = _mu_r;

        double coef_roll = 0.0;
        double coef_slip = 0.0;

        if ( _v_max > 0.0 )
        {
            coef_roll = Misc::satur( 0.0, 1.0, fabs( v_roll ) / _v_max ) * Misc::sign( v_roll );
            coef_slip = Misc::satur( 0.0, 1.0, fabs( v_slip ) / _v_max ) * Misc::sign( v_slip );
        }
        else
        {
            coef_roll = Misc::sign( v_roll );
            coef_slip = Misc::sign( v_slip );
        }

        if ( fabs( _d_roll ) < _d_max && fabs( _d_slip ) < _d_max )
        {
            // spring-like model of static friction
            coef_roll += ( 2.0 / ( 1.0 + exp( -3.0 * Misc::satur( 0.0, 1.0, fabs( _d_roll ) / _d_max ) ) ) - 1.0 ) * Misc::sign( _d_roll );
            coef_slip += ( 2.0 / ( 1.0 + exp( -3.0 * Misc::satur( 0.0, 1.0, fabs( _d_slip ) / _d_max ) ) ) - 1.0 ) * Misc::sign( _d_slip );
        }

        coef_roll = Misc::satur( -1.0, 1.0, coef_roll );
        coef_slip = Misc::satur( -1.0, 1.0, coef_slip );

        // braking friction
        mu_roll_t += mu_surf_s * _brake;

        double mu_roll_max = mu_surf_s;

        if ( antiskid )
        {
            mu_roll_max = mu_surf_k + ( mu_surf_s - mu_surf_k ) * ( 1.0 - coef_slip );
        }
        else
        {
            mu_roll_max = mu_surf_k;
        }

        if ( mu_roll_t > mu_roll_max ) mu_roll_t = mu_roll_max;

        // tire forces
        double for_norm_pos = ( for_norm < 0.0 ) ? 0.0 : for_norm;
        double for_roll_max = for_norm_pos * mu_roll_t;
        double for_slip_max = for_norm_pos * mu_surf_k;

        // continuous friction model
        double for_roll = for_roll_max * coef_roll;
        double for_slip = for_slip_max * coef_slip;

        // tangent forces
        double for_lon = -for_roll * cosDelta + for_slip * sinDelta;
        double for_lat = -for_slip * cosDelta + for_roll * sinDelta;

        // preliminary tangent force
        Vector3 for_tan_bas = for_lon * dir_lon_bas + for_lat * dir_lat_bas;

        // max friction check
        double max_fric = mu_surf_s * for_norm;
        double max_coef = max_fric / for_tan_bas.getLength();

        if ( max_coef < 1.0 && Misc::isValid( max_coef ) )
        {
            for_tan_bas = max_coef * for_tan_bas;
        }

        // resulting forces
        _for_bas = for_tan_bas + for_norm * n_c_bas;
        _mom_bas = r_c_bas % _for_bas;
    }
}

////////////////////////////////////////////////////////////////////////////////

void Wheel::integrate( double timeStep,
                       const Vector3 &vel_bas,
                       const Vector3 &omg_bas,
                       const Vector3 &r_c_bas,
                       const Vector3 &n_c_bas,
                       bool steering )
{
    double deflection_norm = n_c_bas * ( r_c_bas - _r_u_bas );

    if ( deflection_norm > 1.0e-6 )
    {
        Vector3 dir_lon_bas;
        Vector3 dir_lat_bas;

        double cosDelta = 1.0;
        double sinDelta = 0.0;

        double v_norm = 0.0;
        double v_roll = 0.0;
        double v_slip = 0.0;

        calculateVariables( vel_bas, omg_bas, r_c_bas, n_c_bas, steering,
                            &dir_lon_bas, &dir_lat_bas,
                            &cosDelta, &sinDelta, &v_norm, &v_roll, &v_slip );

        _d_roll += v_roll * timeStep;
        _d_slip += v_slip * timeStep;

        if ( fabs( _d_roll ) > _d_max || fabs( v_roll ) > _v_max ) _d_roll = 0.0;
        if ( fabs( _d_slip ) > _d_max || fabs( v_slip ) > _v_max ) _d_slip = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////

void Wheel::update( double delta, double brake )
{
    _delta = delta;
    _brake = brake;
}

////////////////////////////////////////////////////////////////////////////////

void Wheel::calculateVariables( const Vector3 &vel_bas,
                                const Vector3 &omg_bas,
                                const Vector3 &r_c_bas,
                                const Vector3 &n_c_bas,
                                bool steering,
                                Vector3 *dir_lon_bas,
                                Vector3 *dir_lat_bas,
                                double *cosDelta,
                                double *sinDelta,
                                double *v_norm,
                                double *v_roll,
                                double *v_slip )
{
    // contact point velocities components
    Vector3 v_c_bas = vel_bas + ( omg_bas % r_c_bas );

    (*v_norm) = n_c_bas * v_c_bas;

    Vector3 v_norm_bas = (*v_norm) * n_c_bas;
    Vector3 v_tang_bas = v_c_bas - v_norm_bas;

    double v_tang = v_tang_bas.getLength();

    // longitudal and lateral directions
    (*dir_lon_bas) = ( n_c_bas % Vector3::ey() ).getNormalized();
    (*dir_lat_bas) = ( Vector3::ex() % n_c_bas ).getNormalized();

    // longitudal and lateral velocity components
    double vel_lon = v_tang_bas * (*dir_lon_bas);
    double vel_lat = v_tang_bas * (*dir_lat_bas);

    // steering
    double delta = 0.0;

    (*cosDelta) = 1.0;
    (*sinDelta) = 0.0;

    if ( _steerable && steering )
    {
        delta = Misc::satur( -_delta_max, _delta_max, _delta );

        (*cosDelta) = cos( delta );
        (*sinDelta) = sin( delta );
    }
    else if ( _caster && v_tang > _v_max )
    {
        (*cosDelta) =  vel_lon / v_tang;
        (*sinDelta) = -vel_lat / v_tang;

        delta = asin( (*sinDelta) );
    }

    // tire velocities component
    (*v_roll) = vel_lon * (*cosDelta) - vel_lat * (*sinDelta);
    (*v_slip) = vel_lat * (*cosDelta) - vel_lon * (*sinDelta);
}