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

#include <fdm/models/fdm_WinchLauncher.h>

#include <fdm/utils/fdm_String.h>
#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

WinchLauncher::WinchLauncher() :
    _for_max ( 0.0 ),
    _len_max ( 0.0 ),
    _ang_max ( 0.0 ),
    _vel_max ( 0.0 ),

    _tc_for ( 0.0 ),
    _tc_vel ( 0.0 ),

    _for ( 0.0 ),
    _vel ( 0.0 ),
    _len ( 0.0 ),

    _stiffness ( 0.0 ),

    _active ( true )
{}

////////////////////////////////////////////////////////////////////////////////

WinchLauncher::~WinchLauncher() {}

////////////////////////////////////////////////////////////////////////////////

void WinchLauncher::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_r_a_bas, "attachment_point" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_for_max, "for_max" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_len_max, "len_max" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_ang_max, "ang_max" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_vel_max, "vel_max" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_tc_for, "tc_for" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_tc_vel, "tc_vel" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_stiffness, "stiffness" );

        _len = _len_max;

        if ( result != FDM_SUCCESS ) XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
    else
    {
        XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
}

////////////////////////////////////////////////////////////////////////////////

void WinchLauncher::computeForceAndMoment( const fdm::Matrix3x3 &wgs2bas,
                                           const Vector3 &pos_wgs )
{
    _for_bas.zeroize();
    _mom_bas.zeroize();

    if ( _active )
    {
        Vector3 pos_bas = wgs2bas * ( _pos_wgs - pos_wgs );

        double x = Misc::max( 0.0, pos_bas.getLength() - _len );

        _for_bas = Misc::max( _for, fabs( x ) * x * _stiffness ) * pos_bas.getNormalized();
        _mom_bas = _r_a_bas % _for_bas;
    }
}

////////////////////////////////////////////////////////////////////////////////

void WinchLauncher::update( double timeStep,
                            const fdm::Matrix3x3 &bas2wgs,
                            const fdm::Matrix3x3 &wgs2ned,
                            const Vector3 &pos_wgs,
                            double altitude_agl )
{
    if ( _active )
    {
        if ( timeStep > FDM_TIME_STEP_MIN )
        {
            _for = Misc::inertia( _for_max, _for, timeStep, _tc_for );
            _vel = Misc::inertia( _vel_max, _vel, timeStep, _tc_vel );
            _len = _len - timeStep * _vel;

            Vector3 pos_ned = wgs2ned * ( pos_wgs - _pos_wgs );

            if ( atan2( -pos_ned.z(), pos_ned.getLengthXY() ) > _ang_max )
            {
                _active = false;
            }
        }
        else
        {
            if ( altitude_agl < FDM_MIN_INIT_ALTITUDE )
            {
                _pos_wgs = pos_wgs + bas2wgs * Vector3( _len_max, 0.0, 0.0 );
                _len = _len_max;
            }
        }
    }
}
