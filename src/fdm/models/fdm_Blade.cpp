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

#include <fdm/models/fdm_Blade.h>

#include <fdm/fdm_Log.h>
#include <fdm/main/fdm_Aerodynamics.h>
#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Matrix3x3 Blade::getRAS2SRA( double psi )
{
    double tempPsi = -psi + M_PI / 2.0;

    Matrix3x3 result;

//    result(0,0) = -1.0;
//    result(0,1) =  0.0;
//    result(0,2) =  0.0;

//    result(1,0) =  0.0;
//    result(1,1) =  1.0;
//    result(1,2) =  0.0;

//    result(2,0) =  0.0;
//    result(2,1) =  0.0;
//    result(2,2) = -1.0;

    double cosPsi = cos( tempPsi );
    double sinPsi = sin( tempPsi );

    result(0,0) = -cosPsi;
    result(0,1) = -sinPsi;
    result(0,2) =  0.0;

    result(1,0) = -sinPsi;
    result(1,1) =  cosPsi;
    result(1,2) =  0.0;

    result(2,0) =  0.0;
    result(2,1) =  0.0;
    result(2,2) = -1.0;

    return result;
}

////////////////////////////////////////////////////////////////////////////////

Blade::Blade() :
    _r ( 0.0 ),
    _c ( 0.0 ),
    _e ( 0.0 ),
    _m ( 0.0 ),

    _beta_min ( 0.0 ),
    _beta_max ( 0.0 ),

    _b ( 0.0 ),

    _sb ( 0.0 ),
    _ib ( 0.0 ),

    _thrust ( 0.0 ),
    _hforce ( 0.0 ),
    _yforce ( 0.0 ),
    _torque ( 0.0 ),
    _moment ( 0.0 ),

    _beta_0 ( 0.0 ),
    _beta_1 ( 0.0 ),
    _beta_2 ( 0.0 ),

    _beta ( _beta_0 ),

    _theta ( 0.0 )
{
    _cd = Table::createOneRecordTable( 0.0 );
    _cl = Table::createOneRecordTable( 0.0 );

    _twist = Table::createOneRecordTable( 0.0 );

    _ras2sra = Matrix3x3::createIdentityMatrix();
    _sra2ras = Matrix3x3::createIdentityMatrix();
}

////////////////////////////////////////////////////////////////////////////////

Blade::~Blade() {}

////////////////////////////////////////////////////////////////////////////////

void Blade::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _m , "blade_mass"   );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _r , "rotor_radius" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _c , "blade_chord"  );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _e , "hinge_offset" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _beta_min, "beta_min" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _beta_max, "beta_max" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _twist, "twist" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _cd, "cd" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _cl, "cl" );

        if ( result == FDM_SUCCESS )
        {
            _b = _r - _e;

            _sb = _m * _b / 2.0;
            _ib = _m * _b * _b / 3.0;
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

void Blade::TEST_INIT()
{
    Log::i() << "REMOVE ME" << std::endl;
}
