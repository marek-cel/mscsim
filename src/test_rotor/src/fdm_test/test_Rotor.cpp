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

#include <fdm_test/test_Rotor.h>

#include <fdm/fdm_Log.h>
#include <fdm/utils/fdm_WGS84.h>
#include <fdm/xml/fdm_XmlDoc.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

test_Rotor::test_Rotor() :
    _rotor ( FDM_NULLPTR )
{
    _rotor = new MainRotorBE();

    readData( "../data/test.xml" );

    _ned2enu(0,0) =  0.0;
    _ned2enu(0,1) =  1.0;
    _ned2enu(0,2) =  0.0;

    _ned2enu(1,0) =  1.0;
    _ned2enu(1,1) =  0.0;
    _ned2enu(1,2) =  0.0;

    _ned2enu(2,0) =  0.0;
    _ned2enu(2,1) =  0.0;
    _ned2enu(2,2) = -1.0;

    _enu2ned = _ned2enu.getTransposed();

    _bas2ned = Matrix3x3::createIdentityMatrix();
    _ned2bas = Matrix3x3::createIdentityMatrix();
}

////////////////////////////////////////////////////////////////////////////////

test_Rotor::~test_Rotor()
{
    FDM_DELPTR( _rotor );
}

////////////////////////////////////////////////////////////////////////////////

void test_Rotor::readData( const std::string &dataFile )
{
    XmlDoc doc( dataFile );

    if ( doc.isOpen() )
    {
        XmlNode rootNode = doc.getRootNode();

        if ( rootNode.isValid() )
        {
            XmlNode nodeRotor = rootNode.getFirstChildElement( "main_rotor" );

            _rotor->readData( nodeRotor );
        }
        else
        {
            Log::e() << "Reading file \"" << dataFile << "\" failed. Invalid root node." << std::endl;
        }
    }
    else
    {
        Log::e() << "Reading file \"" << dataFile << "\" failed." << std::endl;
    }
}

////////////////////////////////////////////////////////////////////////////////

void test_Rotor::update( double timeStep )
{
    _ned2bas = Matrix3x3( Angles( Data::get()->state.ned_phi,
                                  Data::get()->state.ned_tht,
                                  Data::get()->state.ned_psi ) );
    _bas2ned = _ned2bas.getTransposed();

    Vector3 vel_wind_ned = Matrix3x3( Angles( 0.0, 0.0, -Data::get()->state.wind_dir ) )
             * Vector3( -Data::get()->state.wind_vel, 0.0, 0.0 );
    Vector3 vel_wind_bas = _ned2bas * vel_wind_ned;

    Vector3 vel_bas = Vector3( Data::get()->state.bas_u,
                               Data::get()->state.bas_v,
                               Data::get()->state.bas_w );

    Vector3 omg_bas = Vector3( Data::get()->state.bas_p,
                               Data::get()->state.bas_q,
                               Data::get()->state.bas_r );

    Vector3 vel_air_bas = vel_bas - vel_wind_bas;
    Vector3 omg_air_bas = omg_bas;

    Vector3 acc_bas( 0.0, 0.0, 0.0 );
    Vector3 eps_bas( 0.0, 0.0, 0.0 );

    Vector3 grav_ned( 0.0, 0.0, WGS84::_g );
    Vector3 grav_bas = _ned2bas * grav_ned;

    //std::cout << grav_ras.toString() << std::endl;

    double omega   = Data::get()->rotor.omega;
    double azimuth = Data::get()->rotor.azimuth;
    double airDensity = 1.225;

    _rotor->update( timeStep,
                    vel_air_bas,
                    omg_air_bas,
                    omg_bas,
                    acc_bas,
                    eps_bas,
                    grav_bas,
                    omega,
                    azimuth,
                    airDensity,
                    Data::get()->rotor.collective,
                    Data::get()->rotor.cyclicLat,
                    Data::get()->rotor.cyclicLon );
}

////////////////////////////////////////////////////////////////////////////////

void test_Rotor::updateData()
{
    for ( int i = 0; i < _rotor->getNumberOfBlades(); i++ )
    {
        Data::get()->blade[ i ].beta  = _rotor->getBlade( i )->getBeta();
        Data::get()->blade[ i ].theta = _rotor->getBlade( i )->getTheta();
    }
}
