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

const double MainRotorBE::_timeStepMax = 1.0e-2;

////////////////////////////////////////////////////////////////////////////////

MainRotorBE::MainRotorBE() :
    _direction ( CW ),

    _blades_no ( 0 ),

    _radius ( 0.0 ),

    _omega   ( 0.0 ),
    _azimuth ( 0.0 ),

    _i_tot ( 0.0 ),
    _d_psi ( 0.0 ),

    _beta_0  ( 0.0 ),
    _beta_1c ( 0.0 ),
    _beta_1s ( 0.0 ),

    _theta_0  ( 0.0 ),
    _theta_1c ( 0.0 ),
    _theta_1s ( 0.0 ),

    _coningAngle ( 0.0 ),
    _diskRoll    ( 0.0 ),
    _diskPitch   ( 0.0 ),

    _ct ( 0.0 ),
    _cq ( 0.0 ),

    _thrust ( 0.0 ),
    _torque ( 0.0 ),

    _vel_i ( 0.0 ),

    _wakeSkew ( 0.0 ),

    _prev_azimuth  ( 0.0 ),
    _prev_theta_0  ( 0.0 ),
    _prev_theta_1c ( 0.0 ),
    _prev_theta_1s ( 0.0 )
{
    _bas2ras = Matrix3x3::identityMatrix();
    _ras2bas = Matrix3x3::identityMatrix();
}

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

        double inclination = 0.0;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _r_hub_bas  , "hub_center" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, inclination , "inclination" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _blades_no  , "number_of_blades" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _radius     , "rotor_radius" );

        if ( result == FDM_SUCCESS )
        {
            _bas2ras = Matrix3x3( Angles( 0.0, -inclination, 0.0 ) );
            _ras2bas = _bas2ras.getTransposed();

            _d_psi = ( 2.0 * M_PI ) / (double)( _blades_no );
        }
        else
        {
            XmlUtils::throwError( __FILE__, __LINE__, dataNode );
        }

        // blades
        XmlNode nodeBlade = dataNode.getFirstChildElement( "blade" );

        for ( int i = 0; i < _blades_no; i++ )
        {
            RotorBlade *blade = new RotorBlade( _direction );
            blade->readData( nodeBlade );

            _i_tot += blade->getInertia();

            _blades.push_back( blade );
        }
    }
    else
    {
        XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainRotorBE::computeForceAndMoment( const Vector3 &vel_air_bas,
                                         const Vector3 &omg_air_bas,
                                         const Vector3 &omg_bas,
                                         const Vector3 &acc_bas,
                                         const Vector3 &eps_bas,
                                         const Vector3 &grav_bas,
                                         double airDensity )
{
    _for_bas.zeroize();
    _mom_bas.zeroize();

    _thrust = 0.0;
    _torque = 0.0;

    Vector3 vel_air_ras = _bas2ras * ( vel_air_bas + omg_air_bas % _r_hub_bas );
    Vector3 omg_air_ras = _bas2ras * omg_air_bas;
    Vector3 omg_ras     = _bas2ras * omg_bas;
    Vector3 acc_ras     = _bas2ras * ( acc_bas + ( omg_bas % ( omg_bas % _r_hub_bas ) ) + ( eps_bas % _r_hub_bas ) );
    Vector3 eps_ras     = _bas2ras * eps_bas;
    Vector3 grav_ras    = _bas2ras * grav_bas;

    double delta_psi = 0.0;

    for ( Blades::iterator it = _blades.begin(); it != _blades.end(); ++it )
    {
        (*it)->computeForceAndMoment( vel_air_ras,
                                      omg_air_ras,
                                      omg_ras,
                                      acc_ras,
                                      eps_ras,
                                      grav_ras,
                                      _omega,
                                      _azimuth + delta_psi,
                                      airDensity,
                                      _theta_0,
                                      _theta_1c,
                                      _theta_1s
                                    );

        _for_bas += _ras2bas * (*it)->getFor_RAS();
        _mom_bas += _ras2bas * (*it)->getMom_RAS();

        _torque += (*it)->getTorque();

        delta_psi += _d_psi;
    }

    _mom_bas += _r_hub_bas % _for_bas;
}

////////////////////////////////////////////////////////////////////////////////

void MainRotorBE::update( double timeStep,
                          const Vector3 &vel_air_bas,
                          const Vector3 &omg_air_bas,
                          const Vector3 &omg_bas,
                          const Vector3 &acc_bas,
                          const Vector3 &eps_bas,
                          const Vector3 &grav_bas,
                          double omega,
                          double azimuth,
                          double airDensity,
                          double collective,
                          double cyclicLat,
                          double cyclicLon )
{
    _prev_vel_air_ras = _vel_air_ras;
    _prev_omg_air_ras = _omg_air_ras;
    _prev_omg_ras     = _omg_ras;
    _prev_acc_ras     = _acc_ras;
    _prev_eps_ras     = _eps_ras;
    _prev_grav_ras    = _grav_ras;

    _vel_air_ras = _bas2ras * ( vel_air_bas + omg_air_bas % _r_hub_bas );
    _omg_air_ras = _bas2ras * omg_air_bas;
    _omg_ras     = _bas2ras * omg_bas;
    _acc_ras     = _bas2ras * ( acc_bas + ( omg_bas % ( omg_bas % _r_hub_bas ) ) + ( eps_bas % _r_hub_bas ) );
    _eps_ras     = _bas2ras * eps_bas;
    _grav_ras    = _bas2ras * grav_bas;

    _prev_azimuth  = _azimuth;

    _prev_theta_0  = _theta_0;
    _prev_theta_1c = _theta_1c;
    _prev_theta_1s = _theta_1s;

    _omega   = omega;
    _azimuth = azimuth;

    _theta_0  = collective;
    _theta_1c = _direction == CW ? cyclicLat : -cyclicLat;
    _theta_1s = cyclicLon;

    Vector3 d_vel_air_ras = _vel_air_ras - _prev_vel_air_ras;
    Vector3 d_omg_air_ras = _omg_air_ras - _prev_omg_air_ras;
    Vector3 d_omg_ras     = _omg_ras     - _prev_omg_ras;
    Vector3 d_acc_ras     = _acc_ras     - _prev_acc_ras;
    Vector3 d_eps_ras     = _eps_ras     - _prev_eps_ras;
    Vector3 d_grav_ras    = _grav_ras    - _prev_grav_ras;

    double d_azimuth = _azimuth - _prev_azimuth;

    double d_theta_0  = _theta_0  - _prev_theta_0;
    double d_theta_1c = _theta_1c - _prev_theta_1c;
    double d_theta_1s = _theta_1s - _prev_theta_1s;

    const int steps = ceil( timeStep / _timeStepMax );
    double timeStepInt = timeStep / ( (double)steps );

    for ( int i = 0; i < steps; i++ )
    {
        const double coef = ( (double)( i + 1 ) ) / ( (double)steps );

        Vector3 vel_air_ras = _prev_vel_air_ras + coef * d_vel_air_ras;
        Vector3 omg_air_ras = _prev_omg_air_ras + coef * d_omg_air_ras;
        Vector3 omg_ras     = _prev_omg_ras     + coef * d_omg_ras;
        Vector3 acc_ras     = _prev_acc_ras     + coef * d_acc_ras;
        Vector3 eps_ras     = _prev_eps_ras     + coef * d_eps_ras;
        Vector3 grav_ras    = _prev_grav_ras    + coef * d_grav_ras;

        double azimuth = _prev_azimuth + coef * d_azimuth;

        double theta_0  = _prev_theta_0  + coef * d_theta_0;
        double theta_1c = _prev_theta_1c + coef * d_theta_1c;
        double theta_1s = _prev_theta_1s + coef * d_theta_1s;

        double delta_psi = 0.0;

        for ( Blades::iterator it = _blades.begin(); it != _blades.end(); ++it )
        {
            (*it)->integrate( timeStepInt,
                              vel_air_ras,
                              omg_air_ras,
                              omg_ras,
                              acc_ras,
                              eps_ras,
                              grav_ras,
                              omega,
                              azimuth + delta_psi,
                              airDensity,
                              theta_0,
                              theta_1c,
                              theta_1s
                            );

            delta_psi += _d_psi;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainRotorBE::inducedVelcoity()
{
    // iteration loop
//    for ( int i = 0; i < 100; i++ )
//    {
//        // thrust coefficient
//        _ct = 0.5 * _a * _s * _b * ( lambda * _b / 2.0
//                                  + _theta_0 * ( _b2 + 1.5 * mu2 ) / 3.0
//                                  + _b * mu * beta_1c_cwas / 4.0 );
//        if ( _ct > _ct_max ) _ct = _ct_max;

//        // zero function (Padfield p.124)
//        double lambda_d = mu2 + lambda * lambda;
//        double g_0 = lambda_i - _ct / ( 2.0 * sqrt( lambda_d ) );

//        // break condition
//        if ( fabs( g_0 ) < 1.0e-6 ) break;

//        // (Padfield p.124)
//        double h_j = -( 2.0 * lambda_i * sqrt( lambda_d ) - _ct ) * lambda_d
//                / ( 2*pow( lambda_d, 2.0/3.0 ) + _a * _s * lambda_d / 4.0 - _ct * lambda );

//        // (Padfield p.124)
//        double f_j = 1.0;
//        if ( fabs( mu_z ) < 0.05 ) f_j = 0.6;

//        // (Padfield p.124)
//        double lambda_i_new = lambda_i + f_j * h_j;

//        if ( Misc::isValid( lambda_i_new ) ) lambda_i = lambda_i_new;
//    }
}
