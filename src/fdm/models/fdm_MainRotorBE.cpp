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

#include <fdm/models/fdm_MainRotorBE.h>

#include <fdm/utils/fdm_String.h>

#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

MainRotorBE::MainRotorBE() :
    _ir ( 0.0 ),

    _delta_psi ( 0.0 ),

    _theta_0  ( 0.0 ),
    _theta_1c ( 0.0 ),
    _theta_1s ( 0.0 )
{}

////////////////////////////////////////////////////////////////////////////////

MainRotorBE::~MainRotorBE()
{
    Blades::iterator it = _blades.begin();

    while ( it != _blades.end() )
    {
        FDM_DELPTR( *it );
        it = _blades.erase( it );
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainRotorBE::readData( XmlNode &dataNode )
{
    ////////////////////////////////
    MainRotor::readData( dataNode );
    ////////////////////////////////

    if ( dataNode.isValid() )
    {
        _delta_psi = ( 2.0 * M_PI ) / (double)( _nb );

        XmlNode nodeBlade = dataNode.getFirstChildElement( "blade" );

        for ( int i = 0; i < _nb; i++ )
        {
            Blade *blade = new Blade();
            blade->readData( nodeBlade );

            _ir += blade->getInertia();

            _blades.push_back( blade );
        }
    }
    else
    {
        XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
}

////////////////////////////////////////////////////////////////////////////////

#include <iostream>
void MainRotorBE::integrate( double timeStep,
                             const Vector3 &vel_bas,
                             const Vector3 &omg_bas,
                             const Vector3 &acc_bas,
                             const Vector3 &eps_bas,
                             const Vector3 &grav_bas,
                             const Vector3 &vel_air_bas,
                             const Vector3 &omg_air_bas,
                             double airDensity )
{
    // velocity transformations
    Vector3 vel_air_ras = _bas2ras * ( vel_air_bas + ( omg_air_bas % _r_hub_bas ) );
    Vector3 omg_air_ras = _bas2ras * omg_air_bas;

    // azimuth integration
    _azimuth += _omega * timeStep;
    _azimuth = Angles::normalize( _azimuth );

    double thrust = 0.0;
    double hforce = 0.0;
    double torque = 0.0;

    // rotor blades integration
    for ( int i = 0; i < _nb; i++ )
    {
        _blades[ i ]->integrate( timeStep,
                                 vel_bas,
                                 omg_bas,
                                 acc_bas,
                                 eps_bas,
                                 grav_bas,
                                 vel_air_ras,
                                 omg_air_ras,
                                 airDensity,
                                 _azimuth + i * _delta_psi,
                                 _omega,
                                 _theta_0,
                                 _theta_1c,
                                 _theta_1s );

        thrust += _blades[ i ]->getThrust();
        hforce += _blades[ i ]->getHForce();
        torque += _blades[ i ]->getTorque();
    }

    //std::cout << thrust << std::endl;

    _for_bas = _ras2bas * Vector3( 0.0, 0.0, -thrust );
    _mom_bas = ( _r_hub_bas % _for_bas );
             //+ _ras2bas * Vector3( 0.0, 0.0, _direction == CW ? -torque : torque );
}

////////////////////////////////////////////////////////////////////////////////

void MainRotorBE::update( double omega,
                          double azimuth,
                          double collective,
                          double cyclicLat,
                          double cyclicLon )
{
    ////////////////////////////////////
    MainRotor::update( omega, azimuth );
    ////////////////////////////////////

    // controls
    _theta_0  = collective;
    _theta_1c = _direction == CW ? cyclicLat : -cyclicLat;
    _theta_1s = cyclicLon;
}
