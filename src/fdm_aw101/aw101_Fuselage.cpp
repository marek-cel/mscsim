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

#include <fdm_aw101/aw101_Fuselage.h>

#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

AW101_Fuselage::AW101_Fuselage() :
    _landing_gear ( 0.0 ),

    _dcx_dgear ( 0.0 ),
    _dcz_dgear ( 0.0 ),
    _dcm_dgear ( 0.0 )
{}

////////////////////////////////////////////////////////////////////////////////

AW101_Fuselage::~AW101_Fuselage() {}

////////////////////////////////////////////////////////////////////////////////

void AW101_Fuselage::readData( XmlNode &dataNode )
{
    ///////////////////////////////
    Fuselage::readData( dataNode );
    ///////////////////////////////

    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_dcx_dgear, "dcx_dgear" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_dcz_dgear, "dcz_dgear" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_dcm_dgear, "dcm_dgear" );

        if ( result != FDM_SUCCESS ) XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
    else
    {
        XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
}

////////////////////////////////////////////////////////////////////////////////

void AW101_Fuselage::computeForceAndMoment( const fdm::Vector3 &vel_air_bas,
                                            const fdm::Vector3 &omg_air_bas,
                                            double airDensity,
                                            double inducedVelocity,
                                            double wakeSkewAngle,
                                            double landing_gear )
{
    _landing_gear = landing_gear;

    //////////////////////////////////////////////////////////////////////
    Fuselage::computeForceAndMoment( vel_air_bas, omg_air_bas, airDensity,
                                     inducedVelocity, wakeSkewAngle );
    //////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////

double AW101_Fuselage::getCx( double angleOfAttack ) const
{
    return Fuselage::getCx( angleOfAttack ) + _landing_gear * _dcx_dgear;
}

////////////////////////////////////////////////////////////////////////////////

double AW101_Fuselage::getCz( double angleOfAttack ) const
{
    return Fuselage::getCz( angleOfAttack ) + _landing_gear * _dcz_dgear;
}

////////////////////////////////////////////////////////////////////////////////

double AW101_Fuselage::getCm( double angleOfAttack ) const
{
    return Fuselage::getCm( angleOfAttack ) + _landing_gear * _dcm_dgear;
}

