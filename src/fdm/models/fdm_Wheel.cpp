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

Wheel::Wheel() :
    _input ( FDM_NULLPTR ),

    _k ( 0.0 ),
    _c ( 0.0 ),

    _mu_s ( 0.0 ),
    _mu_k ( 0.0 ),
    _mu_r ( 0.0 ),

    _delta_max ( 0.0 ),

    _vel_break ( 1.0 ),

    _steerable ( false ),
    _caster    ( false ),

    _brakeGroup ( None ),

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

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _vel_break, "vel_break", true );

        if ( result != FDM_SUCCESS ) XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
    else
    {
        XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Wheel::computeForce( const Vector3 &vel_bas,
                          const Vector3 &omg_bas,
                          const Vector3 &r_c_bas,
                          const Vector3 &n_c_bas,
                          bool steering, bool antiskid,
                          double surf_coef )
{
    _for_bas.zeroize();

    double deflection_norm = n_c_bas * ( r_c_bas - _r_u_bas );

    if ( deflection_norm > 1.0e-6 )
    {
        // intersection velocities components
        Vector3 v_c_bas = vel_bas + ( omg_bas % r_c_bas );
        double v_norm = n_c_bas * v_c_bas;
        Vector3 v_norm_bas = v_norm * n_c_bas;
        Vector3 v_tang_bas = v_c_bas - v_norm_bas;
        double v_tang = v_tang_bas.getLength();

        // normal force
        double for_norm = _k * deflection_norm - _c * v_norm;

        // longitudal and lateral directions
        Vector3 dir_lon_bas = ( n_c_bas % Vector3::ey() ).getNormalized();
        Vector3 dir_lat_bas = ( Vector3::ex() % n_c_bas ).getNormalized();

        // longitudal and lateral velocity components
        double vel_lon = v_tang_bas * dir_lon_bas;
        double vel_lat = v_tang_bas * dir_lat_bas;

        // steering
        double cosTurn = 1.0;
        double sinTurn = 0.0;

        if ( _steerable && steering )
        {
            double angle = Misc::satur( -_delta_max, _delta_max, _delta );

            cosTurn = cos( angle );
            sinTurn = sin( angle );
        }
        else if ( _caster && v_tang > _vel_break )
        {
            cosTurn =  vel_lon / v_tang;
            sinTurn = -vel_lat / v_tang;
        }

        // tire velocities component
        double vel_roll = vel_lon * cosTurn - vel_lat * sinTurn;
        double vel_slip = vel_lat * cosTurn - vel_lon * sinTurn;

        // friction coefs
        double mu_surf_s = _mu_s * surf_coef;
        double mu_surf_k = _mu_k * surf_coef;

        double mu_roll_t = _mu_r;

        double coef_roll = Misc::satur( 0.0, 1.0, fabs( vel_roll ) / _vel_break ) * Misc::sign( vel_roll );
        double coef_slip = Misc::satur( 0.0, 1.0, fabs( vel_slip ) / _vel_break ) * Misc::sign( vel_slip );

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
        _for_bas = for_tan_bas + for_norm * n_c_bas;
    }
}

////////////////////////////////////////////////////////////////////////////////

void Wheel::update( double delta, double brake )
{
    _delta = delta;
    _brake = brake;
}
