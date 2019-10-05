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

#include <fdm/models/fdm_Propeller.h>

#include <fdm/utils/fdm_Misc.h>
#include <fdm/utils/fdm_String.h>
#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Propeller::Propeller() :
    _direction ( CW ),
    _gearRatio ( 1.0 ),
    _diameter  ( 0.0 ),
    _inertia   ( 0.0 ),

    _area ( 0.0 ),

    _pitch     ( 0.0 ),
    _omega     ( 0.0 ),
    _speed_rps ( 0.0 ),
    _speed_rpm ( 0.0 ),
    _thrust    ( 0.0 ),

    _torqueAvailable ( 0.0 ),
    _torqueRequired  ( 0.0 )
{}

////////////////////////////////////////////////////////////////////////////////

Propeller::~Propeller() {}

////////////////////////////////////////////////////////////////////////////////

void Propeller::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        if ( String::toBool( dataNode.getAttribute( "counter-clockwise" ), false ) )
        {
            _direction = CCW;
        }
        else
        {
            _direction = CW;
        }

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _pos_bas   , "position"   );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _gearRatio , "gear_ratio" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _diameter  , "diameter"   );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _inertia   , "inertia"    );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _propPitch, "pitch" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _coefThrust , "thrust_coef" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _coefPower  , "power_coef"  );

        if ( result == FDM_SUCCESS )
            _area = M_PI * pow( 0.5 * _diameter, 2.0 );
        else
            XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
    else
    {
        XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Propeller::computeThrust( double airspeed, double airDensity )
{
    if ( _speed_rps > 0.0 )
    {
        double advance = airspeed / ( _diameter * _speed_rps );
        double coefThrust = _coefThrust.getValue( advance, _pitch );

        _thrust = coefThrust * airDensity * Misc::pow2( _speed_rps ) * Misc::pow4( _diameter );
    }
    else
    {
        _thrust = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////

void Propeller::integrate( double timeStep, double engineInertia )
{
    // integrating propeller omega
    _omega += ( ( _torqueAvailable - _torqueRequired ) / ( _inertia + engineInertia ) ) * timeStep;

    _speed_rps = Misc::max( 0.0, _omega / ( 2.0 * M_PI ) );

    // engine friction stops propeller
    if ( _torqueAvailable < _torqueRequired && _speed_rps < 1.0 )
    {
        _speed_rps = _speed_rps < 0.1 ? 0.0 : Misc::inertia( 0.0, _speed_rps, timeStep, 0.1 );
        _omega = 2.0 * M_PI * _speed_rps;
    }

    _speed_rpm = 60.0 * _speed_rps;
}

////////////////////////////////////////////////////////////////////////////////

void Propeller::update( double propellerLever, double engineTorque,
                        double airspeed, double airDensity )
{
    _pitch = getPropellerPitch( propellerLever );

    double advance = airspeed / ( _diameter * ( _speed_rps > 0.1 ? _speed_rps : 0.1 ) );
    double coefPower = _coefPower.getValue( advance, _pitch );
    double powerRequired = coefPower * airDensity * Misc::pow3( _speed_rps ) * Misc::pow5( _diameter );

    _inducedVelocity = getInducedVelocity( airspeed, airDensity );

    _torqueRequired  = powerRequired / ( _omega > 1.0 ? _omega : 1.0 );
    _torqueAvailable = engineTorque / _gearRatio;
}

////////////////////////////////////////////////////////////////////////////////

void Propeller::setRPM( double rpm )
{
    _speed_rpm = Misc::max( 0.0, rpm );
    _speed_rps = _speed_rpm / 60.0;
    _omega = 2.0 * M_PI * _speed_rps;
}

////////////////////////////////////////////////////////////////////////////////

double Propeller::getInducedVelocity( double airspeed, double airDensity )
{
    double vi = 0.0;

    // 0.5*rho*A*vi^2 + rho*A*V*vi - T = 0
    // a = 0.5*rho*A
    // b = rho*A*V
    // c = -T
    double a = 0.5 * airDensity * _area;
    double b = airDensity * _area * airspeed;
    double c = -_thrust;

    double delta = b*b - 4.0*a*c;

    if ( delta >= 0.0 )
    {
        // the 2nd result has no physical meaning
        vi = ( -b + sqrt( delta ) ) / ( 2.0 * a );
    }

    return vi;
}

////////////////////////////////////////////////////////////////////////////////

double Propeller::getPropellerPitch( double propellerLever )
{
    return _propPitch.getValue( propellerLever );
}
