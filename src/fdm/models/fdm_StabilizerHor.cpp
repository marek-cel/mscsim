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

#include <fdm/models/fdm_StabilizerHor.h>

#include <fdm/main/fdm_Aerodynamics.h>
#include <fdm/utils/fdm_Units.h>
#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

StabilizerHor::StabilizerHor() :
    _area ( 0.0 ),
    _incidence ( 0.0 )
{
    _cx = Table1::oneRecordTable( 0.0 );
    _cz = Table1::oneRecordTable( 0.0 );

    _downwash = Table1::oneRecordTable( 0.0 );
}

////////////////////////////////////////////////////////////////////////////////

StabilizerHor::~StabilizerHor() {}

////////////////////////////////////////////////////////////////////////////////

void StabilizerHor::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_r_ac_bas, "aero_center" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_area, "area" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_incidence, "incidence", true );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_downwash, "downwash", true );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_cx, "cx" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_cz, "cz" );

        if ( result == FDM_SUCCESS )
        {
            //_downwash.multiplyKeys( Units::deg2rad() );
            //_downwash.multiplyValues( Units::deg2rad() );

            _cx.multiplyKeys( Units::deg2rad() );
            _cz.multiplyKeys( Units::deg2rad() );
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

void StabilizerHor::computeForceAndMoment( const Vector3 &vel_air_bas,
                                           const Vector3 &omg_air_bas,
                                           double airDensity,
                                           double wingAngleOfAttack )
{
    // stabilizer velocity
    Vector3 vel_stab_bas = vel_air_bas + ( omg_air_bas % _r_ac_bas );

    // stabilizer angle of attack and sideslip angle
    double angleOfAttack = getAngleOfAttack( vel_stab_bas, wingAngleOfAttack );
    double sideslipAngle = Aerodynamics::getSideslipAngle( vel_stab_bas );

    // dynamic pressure
    double dynPress = 0.5 * airDensity * vel_stab_bas.getLength2();

    Vector3 for_aero( dynPress * getCx( angleOfAttack ) * _area,
                      0.0,
                      dynPress * getCz( angleOfAttack ) * _area );

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

double StabilizerHor::getAngleOfAttack( const Vector3 &vel_air_bas,
                                        double wingAngleOfAttack )
{
    return Aerodynamics::getAngleOfAttack( vel_air_bas )
         + _incidence - _downwash.getValue( wingAngleOfAttack );
}

////////////////////////////////////////////////////////////////////////////////

double StabilizerHor::getCx( double angle ) const
{
    return _cx.getValue( angle );
}

////////////////////////////////////////////////////////////////////////////////

double StabilizerHor::getCz( double angle ) const
{
    return _cz.getValue( angle );
}
