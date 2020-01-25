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

#include <fdm/utils/fdm_GaussJordan.h>
#include <fdm/utils/fdm_String.h>

#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

MainRotorBE::MainRotorBE() :
    _ir ( 0.0 ),
    _azimuth_prev ( 0.0 )
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

void MainRotorBE::computeForceAndMoment( const Vector3 &vel_bas,
                                         const Vector3 &omg_bas,
                                         const Vector3 &acc_bas,
                                         const Vector3 &eps_bas,
                                         const Vector3 &vel_air_bas,
                                         const Vector3 &omg_air_bas,
                                         const Vector3 &grav_bas,
                                         double airDensity )
{
    Vector3 vel_ras;
    Vector3 omg_ras;
    Vector3 acc_ras;
    Vector3 eps_ras;
    Vector3 vel_air_ras;
    Vector3 omg_air_ras;
    Vector3 grav_ras;

    convertToHubRAS( vel_bas, omg_bas, acc_bas, eps_bas,
                     vel_air_bas, omg_air_bas, grav_bas,
                     &vel_ras, &omg_ras, &acc_ras, &eps_ras,
                     &vel_air_ras, &omg_air_ras, &grav_ras );

    double thrust = 0.0;
    double hforce = 0.0;
    double yforce = 0.0;
    double torque = 0.0;

    // rotor blades integration
    for ( int i = 0; i < _nb; i++ )
    {
        _blades[ i ]->computeForceAndMoment( vel_air_ras,
                                             omg_air_ras,
                                             omg_ras,
                                             acc_ras,
                                             eps_ras,
                                             grav_ras,
                                             _omega,
                                             _azimuth + i * _delta_psi,
                                             airDensity,
                                             _theta_0,
                                             _theta_1c,
                                             _theta_1s );

        thrust += _blades[ i ]->getThrust();
        hforce += _blades[ i ]->getHForce();
        yforce += _blades[ i ]->getYForce();
        torque += _blades[ i ]->getTorque();
    }

    _thrust = thrust;
    _torque = torque;

    Vector3 for_ras( hforce, yforce, thrust );

    //std::cout << thrust << std::endl;

//    _for_bas = _ras2bas * Vector3( 0.0, 0.0, -thrust );
//    _mom_bas = ( _r_hub_bas % _for_bas );
             //+ _ras2bas * Vector3( 0.0, 0.0, _direction == CW ? -torque : torque );
}

////////////////////////////////////////////////////////////////////////////////

#include <iostream>
void MainRotorBE::integrate( double timeStep,
                             const Vector3 &vel_bas,
                             const Vector3 &omg_bas,
                             const Vector3 &acc_bas,
                             const Vector3 &eps_bas,
                             const Vector3 &vel_air_bas,
                             const Vector3 &omg_air_bas,
                             const Vector3 &grav_bas,
                             double airDensity )
{
    Vector3 vel_ras;
    Vector3 omg_ras;
    Vector3 acc_ras;
    Vector3 eps_ras;
    Vector3 vel_air_ras;
    Vector3 omg_air_ras;
    Vector3 grav_ras;

    convertToHubRAS( vel_bas, omg_bas, acc_bas, eps_bas,
                     vel_air_bas, omg_air_bas, grav_bas,
                     &vel_ras, &omg_ras, &acc_ras, &eps_ras,
                     &vel_air_ras, &omg_air_ras, &grav_ras );

    const unsigned int steps = ceil( timeStep / 0.001 );
    const double timeStepInt = timeStep / ( (double)steps );

    for ( unsigned int i = 0; i < steps; i++ )
    {
        const double coef = ( (double)( i + 1 ) ) / ( (double)steps );
        double azimuth = _azimuth_prev + coef * ( _azimuth - _azimuth_prev );

        // rotor blades integration
        for ( int i = 0; i < _nb; i++ )
        {
            _blades[ i ]->integrate( timeStepInt,
                                     vel_air_ras,
                                     omg_air_ras,
                                     omg_ras,
                                     acc_ras,
                                     eps_ras,
                                     grav_ras,
                                     _omega,
                                     azimuth + i * _delta_psi,
                                     airDensity,
                                     _theta_0,
                                     _theta_1c,
                                     _theta_1s );
        }
    }

    _azimuth_prev = _azimuth;

    // disc coning, roll and pitch
    if ( _nb >= 3 )
    {
        Matrix3x3 mtr;
        Vector3 rhs;
        Vector3 result;

        for ( int i = 0; i < 3; i++ )
        {
            double psi = _azimuth + i * _delta_psi;

            mtr( i, 0 ) = 1.0;
            mtr( i, 1 ) = cos( psi );
            mtr( i, 2 ) = sin( psi );

            rhs( i ) = _blades[ i ]->getBeta();
        }

        GaussJordan< 3 >::solve( mtr, rhs, result );

        _beta_0  = result( 0 );
        _beta_1c = result( 1 );
        _beta_1s = result( 2 );

        _coningAngle =  _beta_0;
        _diskRoll    =  _direction == CW ? _beta_1s : -_beta_1s;
        _diskPitch   = -_beta_1c;
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainRotorBE::convertToHubRAS( const Vector3 &vel_bas,
                                   const Vector3 &omg_bas,
                                   const Vector3 &acc_bas,
                                   const Vector3 &eps_bas,
                                   const Vector3 &vel_air_bas,
                                   const Vector3 &omg_air_bas,
                                   const Vector3 &grav_bas,
                                   Vector3 *vel_ras,
                                   Vector3 *omg_ras,
                                   Vector3 *acc_ras,
                                   Vector3 *eps_ras,
                                   Vector3 *vel_air_ras,
                                   Vector3 *omg_air_ras,
                                   Vector3 *grav_ras )
{
    (*vel_ras) = _bas2ras * ( vel_bas + ( omg_bas % _r_hub_bas ) );
    (*omg_ras) = _bas2ras * omg_bas;

    (*acc_ras) = _bas2ras * (
                    acc_bas
                    + ( omg_bas % ( omg_bas % _r_hub_bas ) )    // centrifugal acceleration
                    + ( eps_bas % _r_hub_bas )                  // Euler acceleration
               );
    (*eps_ras) = _bas2ras * eps_bas;

    (*grav_ras) = _bas2ras * grav_bas;

    (*vel_air_ras) = _bas2ras * ( vel_air_bas + ( omg_air_bas % _r_hub_bas ) );
    (*omg_air_ras) = _bas2ras * omg_air_bas;
}
