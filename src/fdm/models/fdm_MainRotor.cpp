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

#include <fdm/models/fdm_MainRotor.h>

#include <fdm/utils/fdm_String.h>
#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

MainRotor::MainRotor() :
    _direction ( CW ),

    _nb ( 0 ),

    _delta_psi ( 0.0 ),

    _omega   ( 0.0 ),
    _azimuth ( 0.0 ),

    _beta_0  ( 0.0 ),
    _beta_1c ( 0.0 ),
    _beta_1s ( 0.0 ),

    _theta_0  ( 0.0 ),
    _theta_1c ( 0.0 ),
    _theta_1s ( 0.0 ),

    _coningAngle ( 0.0 ),
    _diskRoll    ( 0.0 ),
    _diskPitch   ( 0.0 ),

    _thrust ( 0.0 ),
    _torque ( 0.0 )
{
    _bas2ras = Matrix3x3::createIdentityMatrix();
    _ras2bas = Matrix3x3::createIdentityMatrix();
}

////////////////////////////////////////////////////////////////////////////////

MainRotor::~MainRotor() {}

////////////////////////////////////////////////////////////////////////////////

void MainRotor::readData( XmlNode &dataNode )
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

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _r_hub_bas, "hub_center" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, inclination, "inclination" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _nb, "number_of_blades" );

        if ( result == FDM_SUCCESS )
        {
            _bas2ras = Matrix3x3( Angles( 0.0, -inclination, 0.0 ) );
            _ras2bas = _bas2ras.getTransposed();

            _delta_psi = ( 2.0 * M_PI ) / (double)( _nb );
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

void MainRotor::update( double omega,
                        double azimuth,
                        double collective,
                        double cyclicLat,
                        double cyclicLon )
{
    _omega   = omega;
    _azimuth = azimuth;

    _theta_0  = collective;
    _theta_1c = _direction == CW ? cyclicLat : -cyclicLat;
    _theta_1s = cyclicLon;
}
