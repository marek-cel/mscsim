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

#include <fdm/models/fdm_StabilizerVer.h>

#include <fdm/main/fdm_Aerodynamics.h>
#include <fdm/utils/fdm_String.h>
#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

StabilizerVer::StabilizerVer() :
    _area ( 0.0 )
{
    _cx = Table1::oneRecordTable( 0.0 );
    _cy = Table1::oneRecordTable( 0.0 );
}

////////////////////////////////////////////////////////////////////////////////

StabilizerVer::~StabilizerVer() {}

////////////////////////////////////////////////////////////////////////////////

void StabilizerVer::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _r_ac_bas, "aero_center" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _area, "area" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _cx, "cx" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _cy, "cy" );

        if ( result != FDM_SUCCESS ) XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
    else
    {
        XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
}

////////////////////////////////////////////////////////////////////////////////

void StabilizerVer::computeForceAndMoment( const Vector3 &vel_air_bas,
                                           const Vector3 &omg_air_bas,
                                           double airDensity )
{
    // stabilizer velocity
    Vector3 vel_stab_bas = vel_air_bas + ( omg_air_bas % _r_ac_bas );

    // stabilizer angle of attack and sideslip angle
    double angleOfAttack = Aerodynamics::getAngleOfAttack( vel_stab_bas );
    double sideslipAngle = Aerodynamics::getSideslipAngle( vel_stab_bas );

    // dynamic pressure
    double dynPress = 0.5 * airDensity * vel_stab_bas.getLength2();

    Vector3 for_aero( dynPress * getCx( sideslipAngle ) * _area,
                      dynPress * getCy( sideslipAngle ) * _area,
                      0.0 );

    _for_bas = Aerodynamics::getAero2BAS( angleOfAttack, sideslipAngle ) * for_aero;
    _mom_bas = _r_ac_bas % _for_bas;

    if ( !_for_bas.isValid() || !_mom_bas.isValid() )
    {
        Exception e;

        e.setType( Exception::UnexpectedNaN );
        e.setInfo( "NaN detected in the stabilizer model." );

        FDM_THROW( e );
    }
}

////////////////////////////////////////////////////////////////////////////////

double StabilizerVer::getCx( double angle ) const
{
    return _cx.getValue( angle );
}

////////////////////////////////////////////////////////////////////////////////

double StabilizerVer::getCy( double angle ) const
{
    return _cy.getValue( angle );
}
