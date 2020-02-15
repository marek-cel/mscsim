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
    _blade = new Blade( _direction );

    readData( "../data/blade.xml" );

    _blade->TEST_INIT();

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
            _blade->readData( nodeBlade );
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

    Vector3 grav_ned( 0.0, 0.0, WGS84::_g );
    Vector3 grav_bas = _ned2bas * grav_ned;
    Vector3 grav_ras = _bas2ras * grav_bas;

    //std::cout << grav_ras.toString() << std::endl;

    double omega   = Data::get()->rotor.omega;
    double azimuth = Data::get()->rotor.azimuth;

    _blade->update( timeStep, grav_ras, omega, azimuth );

    //////////////

    if ( 1 )
    {
        updateDataVect( 0, azimuth, _blade->vec_test_1_sra, "pos_i_sra" );
        updateDataVect( 1, azimuth, _blade->vec_test_2_sra, "omega_r_sra" );
        updateDataVect( 2, azimuth, _blade->vec_test_3_sra, "for_cf_bsa" );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Test::updateData()
{
    Data::get()->blade[ 0 ].beta  = _blade->getBeta();
    Data::get()->blade[ 0 ].theta = _blade->getTheta();
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
