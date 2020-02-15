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

#include <fdm_test/fdm_Test.h>

#include <cstring>
#include <iostream>

#include <Data.h>

#include <fdm/fdm_Log.h>
#include <fdm/xml/fdm_XmlDoc.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Test::Test( MainRotor::Direction direction ) :
    _direction ( direction ),
    _blade ( FDM_NULLPTR )
{
    for ( int i = 0; i < 4; i++ )
    {
        _blades.push_back( new Blade( _direction ) );
    }

    readData( "../data/blade.xml" );

    for ( Blades::iterator it = _blades.begin(); it != _blades.end(); it++ )
    {
        (*it)->TEST_INIT();
    }

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

    _bas2ras = Matrix3x3::createIdentityMatrix();
    _ras2bas = Matrix3x3::createIdentityMatrix();
}

////////////////////////////////////////////////////////////////////////////////

Test::~Test()
{
    FDM_DELPTR( _blade );
}

////////////////////////////////////////////////////////////////////////////////

void Test::readData( const std::string &dataFile )
{
    XmlDoc doc( dataFile );

    if ( doc.isOpen() )
    {
        XmlNode rootNode = doc.getRootNode();

        if ( rootNode.isValid() )
        {
            XmlNode nodeBlade = rootNode.getFirstChildElement( "blade" );

            for ( Blades::iterator it = _blades.begin(); it != _blades.end(); it++ )
            {
                (*it)->readData( nodeBlade );
            }
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

void Test::update( double timeStep )
{
    _ned2bas = Matrix3x3( Angles( Data::get()->state.ned_phi,
                                  Data::get()->state.ned_tht,
                                  Data::get()->state.ned_psi ) );
    _bas2ned = _ned2bas.getTransposed();

    Vector3 vel_ras = Vector3( Data::get()->state.bas_u,
                               Data::get()->state.bas_v,
                               Data::get()->state.bas_w );

    Vector3 omg_ras = Vector3( Data::get()->state.bas_p,
                               Data::get()->state.bas_q,
                               Data::get()->state.bas_r );

    Vector3 vel_air_ras = vel_ras;
    Vector3 omg_air_ras = omg_ras;

    Vector3 grav_ned( 0.0, 0.0, WGS84::_g );
    Vector3 grav_bas = _ned2bas * grav_ned;
    Vector3 grav_ras = _bas2ras * grav_bas;

    //std::cout << grav_ras.toString() << std::endl;

    double omega   = Data::get()->rotor.omega;
    double azimuth = Data::get()->rotor.azimuth;
    double airDensity = 1.225;

    double delta_psi = 2.0 * M_PI / 4.0;

    double theta_0  = Data::get()->rotor.collective;
    double theta_1c = ( _direction == MainRotor::CCW ? -1.0 : 1.0 ) * Data::get()->rotor.cyclicLat;
    double theta_1s = Data::get()->rotor.cyclicLon;

    int index = 0;

    for ( Blades::iterator it = _blades.begin(); it != _blades.end(); it++ )
    {
        (*it)->update( timeStep,
                       vel_air_ras,
                       omg_air_ras,
                       omg_ras,
                       grav_ras,
                       omega,
                       azimuth + index * delta_psi,
                       airDensity,
                       theta_0,
                       theta_1c,
                       theta_1s
                       );

        index++;
    }

    //////////////

    if ( 1 )
    {
        Data::get()->other.vect_0_x_enu = 0.0;
        Data::get()->other.vect_0_y_enu = 0.0;
        Data::get()->other.vect_0_z_enu = 0.0;

        Data::get()->other.vect_0_x_enu = 0.0;
        Data::get()->other.vect_0_y_enu = 0.0;
        Data::get()->other.vect_0_z_enu = 0.0;

        //updateDataVect( 0, azimuth, _blade->vec_test_1, "vec_test_1" );
        //updateDataVect( 1, azimuth, _blade->vec_test_2, "xxx" );
        //updateDataVect( 2, azimuth, _blade->vec_test_3, "xxx" );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Test::updateData()
{
    int index = 0;

    for ( Blades::iterator it = _blades.begin(); it != _blades.end(); it++ )
    {
        Data::get()->blade[ index ].beta  = (*it)->getBeta();
        Data::get()->blade[ index ].theta = (*it)->getTheta();

        index++;
    }
}

////////////////////////////////////////////////////////////////////////////////

void Test::updateDataVect( int index,
                           double azimuth,
                           const Vector3 &vec_sra,
                           const std::string &label )
{
    MainRotor::Direction direction = MainRotor::CW;

    if ( Data::get()->rotor.direction == Data::Rotor::CCW )
    {
        direction = MainRotor::CCW;
    }

    Matrix3x3 ras2sra = _blade->getRAS2SRA( azimuth, direction );
    Matrix3x3 sra2ras = ras2sra.getTransposed();

    Vector3 vec_ras =  sra2ras * vec_sra;
    Vector3 vec_bas = _ras2bas * vec_ras;
    Vector3 vec_ned = _bas2ned * vec_bas;
    Vector3 vec_enu = _ned2enu * vec_ned;

    Data::get()->other.vect[ index ] = true;
    Data::get()->other.vect_x_enu[ index ] = vec_enu.x();
    Data::get()->other.vect_y_enu[ index ] = vec_enu.y();
    Data::get()->other.vect_z_enu[ index ] = vec_enu.z();

    strcpy( Data::get()->other.vect_label[ index ], label.c_str() );
}
