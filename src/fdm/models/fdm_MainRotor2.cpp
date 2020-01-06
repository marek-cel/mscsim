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

#include <fdm/models/fdm_MainRotor2.h>

#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

MainRotor2::MainRotor2() {}

////////////////////////////////////////////////////////////////////////////////

MainRotor2::~MainRotor2()
{
    Blades::iterator it = _blades.begin();

    while ( it != _blades.end() )
    {
        FDM_DELPTR( (*it) );
        it = _blades.erase( it );
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainRotor2::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        double inclination = 0.0;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _r_hub_bas, "hub_center" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, inclination, "inclination" );

        if ( result == FDM_SUCCESS )
        {
            _bas2ras = Matrix3x3( Angles( 0.0, -inclination, 0.0 ) );
            _ras2bas = _bas2ras.getTransposed();
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

void MainRotor2::update( double omega,
                         const Vector3 &vel_air_bas,
                         const Vector3 &omg_air_bas,
                         double collective,
                         double cyclicLat,
                         double cyclicLon )
{
    // velocity transformations
    Vector3 vel_air_ras = _bas2ras * ( vel_air_bas + ( omg_air_bas ^ _r_hub_bas ) );

    for ( Blades::iterator it = _blades.begin(); it != _blades.end(); it++ )
    {
        (*it)->update( omega );
    }
}
