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

#include <fdm_xf/xf_Engine.h>

#ifdef _MSC_VER
#   include <algorithm>
#endif

#include <fdm/utils/fdm_Misc.h>
#include <fdm/utils/fdm_Units.h>
#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

XF_Engine::XF_Engine() :
    _state ( Stopped ),

    _thrust_mil ( 0.0 ),
    _thrust_ab  ( 0.0 ),

    _tc_tit ( 1.0 ),

    _tsfc    ( 0.0 ),
    _tsfc_ab ( 0.0 ),

    _n1_idle ( 0.0 ),
    _n2_idle ( 0.0 ),
    _n1_ab   ( 0.0 ),
    _n2_ab   ( 0.0 ),
    _n1_max  ( 0.0 ),
    _n2_max  ( 0.0 ),

    _n1_setpoint ( 0.0 ),
    _n2_setpoint ( 0.0 ),

    _tit_setpoint ( 0.0 ),

    _pow_command ( 0.0 ),
    _pow ( 0.0 ),

    _thrust_tc_inv ( 1.0 ),
    _tit_tc_actual ( 1.0 ),

    _temperature ( 15.0 ),

    _n1 ( 0.0 ),
    _n2 ( 0.0 ),
    _tit ( 0.0 ),
    _fuelFlow ( 0.0 ),
    _thrust ( 0.0 ),

    _afterburner ( false )
{}

////////////////////////////////////////////////////////////////////////////////

XF_Engine::~XF_Engine() {}

////////////////////////////////////////////////////////////////////////////////

void XF_Engine::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        _n1_idle = 0.0;
        _n2_idle = 0.0;
        _n1_ab   = 0.0;
        _n2_ab   = 0.0;
        _n1_max  = 0.0;
        _n2_max  = 0.0;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_pos_bas, "position" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_thrust_mil , "thrust_mil" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_thrust_ab  , "thrust_ab"  );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_ab_threshold, "ab_threshold" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_tc_n1, "time_constant_n1" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_tc_n2, "time_constant_n2" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_tc_thrust, "time_constant_thrust" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_tc_tit, "time_constant_tit" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_tsfc    , "tsfc"    );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_tsfc_ab , "tsfc_ab" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_n1_throttle, "n1_throttle" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_n2_throttle, "n2_throttle" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_tit_n2, "tit_n2" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_tf_idle , "thrust_factor_idle" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_tf_mil  , "thrust_factor_mil"  );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_tf_ab   , "thrust_factor_ab"   );

        if ( result == FDM_SUCCESS )
        {
            _n1_idle = _n1_throttle.getValueMin();
            _n2_idle = _n2_throttle.getValueMin();

            _n1_ab = _n1_throttle.getValue( _ab_threshold );
            _n2_ab = _n2_throttle.getValue( _ab_threshold );

            _n1_max = _n1_throttle.getValueMax();
            _n2_max = _n2_throttle.getValueMax();
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

void XF_Engine::initialize( bool engineOn )
{
    if ( engineOn )
    {
        _n1 = _n1_setpoint = _n1_idle;
        _n2 = _n2_setpoint = _n2_idle;

        _tit = _tit_n2.getValue( _n2_idle );
    }
    else
    {
        _n1 = _n1_setpoint = 0.0;
        _n2 = _n2_setpoint = 0.0;

        _tit = _temperature;
    }
}

////////////////////////////////////////////////////////////////////////////////

void XF_Engine::computeThrust( double machNumber, double airDensity )
{
    if ( _state == Running )
    {
        // (NASA-TP-1538, p.218)
        // (NASA-TN-D-8176, p.152)
        if ( _afterburner )
        {
            double t_mil = _thrust_mil * _tf_mil .getValue( machNumber, airDensity );
            double t_ab  = _thrust_ab  * _tf_ab  .getValue( machNumber, airDensity );

            _thrust = t_mil + ( t_ab - t_mil ) * ( _pow - 0.5 ) / 0.5;
        }
        else
        {
            double t_idle = _thrust_mil * _tf_idle .getValue( machNumber, airDensity );
            double t_mil  = _thrust_mil * _tf_mil  .getValue( machNumber, airDensity );

            _thrust = t_idle + ( t_mil - t_idle ) * std::min( 0.5, _pow ) / 0.5;
        }
    }
    else
    {
        _thrust = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////

void XF_Engine::integrate( double timeStep )
{
    double pow_deriv = _thrust_tc_inv * ( _pow_command - _pow );
    _pow = _pow + timeStep * pow_deriv;

    double epsilon_n1 = 0.01 * ( _n1 < _n1_idle ? _n1_idle : _n1_max );
    double epsilon_n2 = 0.01 * ( _n2 < _n2_idle ? _n2_idle : _n2_max );

    double n1_tc = _tc_n1.getValue( _n1_setpoint - _n1 );
    double n2_tc = _tc_n1.getValue( _n2_setpoint - _n2 );

    _n1 = fabs( _n1 - _n1_setpoint ) < epsilon_n1 ? _n1_setpoint : Misc::inertia( _n1_setpoint, _n1, timeStep, n1_tc );
    _n2 = fabs( _n2 - _n2_setpoint ) < epsilon_n2 ? _n2_setpoint : Misc::inertia( _n2_setpoint, _n2, timeStep, n2_tc );

    _tit = Misc::inertia( _tit_setpoint, _tit, timeStep, _tit_tc_actual );
}

////////////////////////////////////////////////////////////////////////////////

void XF_Engine::update( double throttle, double temperature,
                         double machNumber, double airDensity,
                         bool fuel, bool starter )
{
    computeThrust( machNumber, airDensity );

    _temperature = temperature;

    if ( _n1 >= _n1_idle && _n2 >= _n2_idle && fuel )
    {
        _state = Running;

        if ( throttle < _ab_threshold )
        {
            // military
            if ( _pow < 0.5 )
            {
                _pow_command = 0.5 * throttle / _ab_threshold;
                _thrust_tc_inv = 1.0 / _tc_thrust.getValue( _pow_command - _pow );

                _n1_setpoint = _n1_throttle.getValue( throttle );
                _n2_setpoint = _n2_throttle.getValue( throttle );
            }
            else
            {
                _pow_command = 0.4;
                _thrust_tc_inv = 5.0;

                _n1_setpoint = _n1_ab;
                _n2_setpoint = _n2_ab;
            }
        }
        else
        {
            // afterburner
            if ( _pow < 0.5 )
            {
                _pow_command = 0.6;
                _thrust_tc_inv = 1.0 / _tc_thrust.getValue( _pow_command - _pow );

                _n1_setpoint = _n1 < _n2_ab ? 1.2 * _n2_ab : _n1_throttle.getValue( throttle );
                _n2_setpoint = _n2 < _n2_ab ? 1.2 * _n2_ab : _n2_throttle.getValue( throttle );
            }
            else
            {
                _pow_command = 0.5 + 0.5 * ( throttle - _ab_threshold ) / ( 1.0 - _ab_threshold );
                _thrust_tc_inv = 5.0;

                _n1_setpoint = _n1_throttle.getValue( throttle );
                _n2_setpoint = _n2_throttle.getValue( throttle );
            }
        }

        _tit_tc_actual = _tc_tit;
        _tit_setpoint = _tit_n2.getValue( _n2 );

        _afterburner = _pow >= 0.5 && _n1 >= _n1_ab && _n2 >= _n2_ab;

        if ( _afterburner )
        {
            _fuelFlow = _thrust * _tsfc_ab;
        }
        else
        {
            _fuelFlow = _thrust * _tsfc;
        }
    }
    else
    {
        if ( starter )
        {
            _state = Starting;

            _pow_command = 0.0; // TODO

            _n1_setpoint = 1.2 * _n1_idle;
            _n2_setpoint = 1.2 * _n2_idle;

            _tit_setpoint = _temperature; // TODO

            _fuelFlow = 0.0; // TODO
        }
        else
        {
            _state = Stopped;

            _pow_command = 0.0;
            _pow = 0.0;

            _n1_setpoint = 0.0;
            _n2_setpoint = 0.0;

            _tit_setpoint = _temperature;

            _fuelFlow = 0.0;
        }

        _tit_tc_actual = 20.0;

        _afterburner = false;
    }
}
