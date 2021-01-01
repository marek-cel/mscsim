/****************************************************************************//*
 * Copyright (C) 2021 Marek M. Cel
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

#include <fdm_c130/c130_StabilizerVer.h>

#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

C130_StabilizerVer::C130_StabilizerVer() :
    _dcx_drudder ( 0.0 ),
    _dcy_drudder ( 0.0 ),
    _rudder ( 0.0 )
{}

////////////////////////////////////////////////////////////////////////////////

C130_StabilizerVer::~C130_StabilizerVer() {}

////////////////////////////////////////////////////////////////////////////////

void C130_StabilizerVer::readData( XmlNode &dataNode )
{
    ////////////////////////////////////
    StabilizerVer::readData( dataNode );
    ////////////////////////////////////

    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_dcx_drudder, "dcx_drudder" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_dcy_drudder, "dcy_drudder" );

        if ( result != FDM_SUCCESS ) XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
    else
    {
        XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
}

////////////////////////////////////////////////////////////////////////////////

void C130_StabilizerVer::computeForceAndMoment( const Vector3 &vel_air_bas,
                                                const Vector3 &omg_air_bas,
                                                double airDensity,
                                                double rudder )
{
    _rudder = rudder;

    StabilizerVer::computeForceAndMoment( vel_air_bas, omg_air_bas,
                                          airDensity );
}

////////////////////////////////////////////////////////////////////////////////

double C130_StabilizerVer::getCx( double angle ) const
{
    return StabilizerVer::getCx( angle )
            + _dcx_drudder * _rudder;
}

////////////////////////////////////////////////////////////////////////////////

double C130_StabilizerVer::getCy( double angle ) const
{
    return StabilizerVer::getCy( angle )
            + _dcy_drudder * _rudder;
}
