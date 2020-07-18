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

#include <fdm/main/fdm_LandingGear.h>
#include <fdm/main/fdm_Aircraft.h>

#include <fdm/utils/fdm_String.h>
#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Vector3 LandingGear::getIntersection( const Vector3 &b, const Vector3 &e,
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

double LandingGear::getPacejkaCoef( double kappa,
                                    double b, double c, double d, double e )
{
    return d * sin( c * atan( b*( 1.0 - e )*kappa + e*atan( b*kappa ) ) );
}

////////////////////////////////////////////////////////////////////////////////

LandingGear::LandingGear( const Aircraft* aircraft ) :
    Module ( aircraft ),

    _ctrlAngle ( 0.0 ),

    _brake_l ( 0.0 ),
    _brake_r ( 0.0 ),

    _antiskid ( false ),
    _steering ( false ),

    _onGround ( false )
{}

////////////////////////////////////////////////////////////////////////////////

LandingGear::~LandingGear() {}

////////////////////////////////////////////////////////////////////////////////

void LandingGear::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        XmlNode wheelNode = dataNode.getFirstChildElement( "wheel" );

        while ( result == FDM_SUCCESS && wheelNode.isValid() )
        {
            Wheel wheel;

            wheel.input = FDM_NULLPTR;

            std::string name = wheelNode.getAttribute( "name" );

            wheel.steerable = String::toBool( wheelNode.getAttribute( "steerable" ), false );
            wheel.caster    = String::toBool( wheelNode.getAttribute( "caster"    ), false );

            switch ( String::toInt( wheelNode.getAttribute( "brake_group" ), 0 ) )
            {
                case 1:  wheel.group = Left;  break;
                case 2:  wheel.group = Right; break;
                default: wheel.group = None;  break;
            }

            if ( result == FDM_SUCCESS ) result = XmlUtils::read( wheelNode, wheel.r_a_bas , "attachment_point" );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( wheelNode, wheel.r_u_bas , "unloaded_wheel"   );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( wheelNode, wheel.k       , "stiffness"        );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( wheelNode, wheel.c       , "damping"          );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( wheelNode, wheel.mu_s    , "friction_static"  );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( wheelNode, wheel.mu_k    , "friction_kinetic" );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( wheelNode, wheel.mu_r    , "friction_rolling" );

            if ( result == FDM_SUCCESS && wheel.steerable )
            {
                result = XmlUtils::read( wheelNode, wheel.angle_max, "max_angle" );
            }

            if ( result == FDM_SUCCESS )
            {
                std::pair<Wheels::iterator,bool> temp =
                        _wheels.insert( std::pair<std::string,Wheel>( name, wheel ) );

                if ( temp.second != true )
                {
                    result = FDM_FAILURE;
                }
            }

            if ( result != FDM_SUCCESS ) XmlUtils::throwError( __FILE__, __LINE__, wheelNode );

            wheelNode = wheelNode.getNextSiblingElement( "wheel" );
        }

        if ( result != FDM_SUCCESS ) XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
    else
    {
        XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
}

////////////////////////////////////////////////////////////////////////////////

void LandingGear::initialize() {}

////////////////////////////////////////////////////////////////////////////////

void LandingGear::computeForceAndMoment()
{
    _for_bas.set( 0.0, 0.0, 0.0 );
    _mom_bas.set( 0.0, 0.0, 0.0 );

    for ( Wheels::iterator it = _wheels.begin(); it != _wheels.end(); ++it )
    {
        Wheel &wheel = (*it).second;

        double position = 1.0;
        if ( wheel.input )
        {
            position = *wheel.input;
        }

        if ( position >= 1.0 )
        {
            Vector3 r_i_bas = getWheelIsect( wheel );
            Vector3 for_bas = getWheelForce( wheel, r_i_bas );

            _for_bas += for_bas;
            _mom_bas += r_i_bas % for_bas;
        }
    }

    if ( !_for_bas.isValid() || !_mom_bas.isValid() )
    {
        Exception e;

        e.setType( Exception::UnexpectedNaN );
        e.setInfo( "NaN detected in the landing gear model." );

        FDM_THROW( e );
    }
}

////////////////////////////////////////////////////////////////////////////////

void LandingGear::update()
{
    _onGround = _for_bas.getLength2() > 0.0;
}

////////////////////////////////////////////////////////////////////////////////

LandingGear::Wheel* LandingGear::getWheelByName( const char *name )
{
    Wheels::iterator it = _wheels.find( name );

    if ( it != _wheels.end() )
    {
        return &(it->second);
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////

Vector3 LandingGear::getWheelForce( const Wheel &wheel, const Vector3 &r_i_bas,
                                    double surf_coef, double vel_break )
{
    double deflection_norm = _aircraft->getNormal_BAS() * ( r_i_bas - wheel.r_u_bas );

    if ( deflection_norm > 1.0e-6 )
    {
        // intersection velocities components
        Vector3 v_i_bas = _aircraft->getVel_BAS() + ( _aircraft->getOmg_BAS() % r_i_bas );
        double v_norm = _aircraft->getNormal_BAS() * v_i_bas;
        Vector3 v_norm_bas = v_norm * _aircraft->getNormal_BAS();
        Vector3 v_tang_bas = v_i_bas - v_norm_bas;
        double v_tang = v_tang_bas.getLength();

        // normal force
        double for_norm = wheel.k * deflection_norm - wheel.c * v_norm;

        // longitudal and lateral directions
        Vector3 dir_lon_bas = ( _aircraft->getNormal_BAS() % Vector3::ey() ).getNormalized();
        Vector3 dir_lat_bas = ( Vector3::ex() % _aircraft->getNormal_BAS() ).getNormalized();

        // longitudal and lateral velocity components
        double vel_lon = v_tang_bas * dir_lon_bas;
        double vel_lat = v_tang_bas * dir_lat_bas;

        // steering
        double cosTurn = 1.0;
        double sinTurn = 0.0;

        if ( wheel.steerable && _steering )
        {
            double angle = Misc::satur( -wheel.angle_max, wheel.angle_max, _ctrlAngle );

            cosTurn = cos( angle );
            sinTurn = sin( angle );
        }
        else if ( wheel.caster && v_tang > vel_break )
        {
            cosTurn =  vel_lon / v_tang;
            sinTurn = -vel_lat / v_tang;
        }

        // tire velocities component
        double vel_roll = vel_lon * cosTurn - vel_lat * sinTurn;
        double vel_slip = vel_lat * cosTurn - vel_lon * sinTurn;

        // friction coefs
        double mu_surf_s = wheel.mu_s * surf_coef;
        double mu_surf_k = wheel.mu_k * surf_coef;

        double mu_roll_t = wheel.mu_r;

        double coef_roll = Misc::satur( 0.0, 1.0, fabs( vel_roll ) / vel_break ) * Misc::sign( vel_roll );
        double coef_slip = Misc::satur( 0.0, 1.0, fabs( vel_slip ) / vel_break ) * Misc::sign( vel_slip );

        // braking friction
        if ( wheel.group == Left || wheel.group == Right )
        {
            double brake = ( wheel.group == Left ) ? _brake_l : _brake_r;

            mu_roll_t += mu_surf_s * brake;

            double mu_roll_max = mu_surf_s;

            if ( _antiskid )
            {
                mu_roll_max = mu_surf_k + ( mu_surf_s - mu_surf_k ) * ( 1.0 - coef_slip );
            }
            else
            {
                mu_roll_max = mu_surf_k;
            }

            if ( mu_roll_t > mu_roll_max ) mu_roll_t = mu_roll_max;
        }

        // tire forces
        double for_norm_pos = ( for_norm < 0.0 ) ? 0.0 : for_norm;
        double for_roll_max = for_norm_pos * mu_roll_t;
        double for_slip_max = for_norm_pos * mu_surf_k;

        // continuous friction model
        double for_roll = for_roll_max * coef_roll;
        double for_slip = for_slip_max * coef_slip;

        // tangent forces
        double for_lon = -for_roll * cosTurn + for_slip * sinTurn;
        double for_lat = -for_slip * cosTurn + for_roll * sinTurn;

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
        return for_tan_bas + for_norm * _aircraft->getNormal_BAS();
    }

    return Vector3();
}

////////////////////////////////////////////////////////////////////////////////

Vector3 LandingGear::getWheelIsect( const Wheel &wheel )
{
    Vector3 r_i_bas = wheel.r_u_bas;

    Vector3 b_wgs = _aircraft->getBAS2WGS() * wheel.r_a_bas + _aircraft->getPos_WGS();
    Vector3 e_wgs = _aircraft->getBAS2WGS() * wheel.r_u_bas + _aircraft->getPos_WGS();
    Vector3 r_wgs;
    Vector3 n_wgs;

    if ( FDM_SUCCESS == _aircraft->getIsect()->getIntersection( b_wgs, e_wgs,
                                                                r_wgs, n_wgs ) )
    {
        r_i_bas = _aircraft->getWGS2BAS() * ( r_wgs - _aircraft->getPos_WGS() );
    }

    return r_i_bas;
}
