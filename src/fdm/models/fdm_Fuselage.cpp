/****************************************************************************//*
 * Copyright (C) 2019 Marek M. Cel
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

#include <fdm/models/fdm_Fuselage.h>

#include <fdm/main/fdm_Aerodynamics.h>
#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Fuselage::Fuselage() :
    _length ( 0.0 ),
    _area ( 0.0 ),
    _sl ( 0.0 ),

    _angleOfAttack ( 0.0 ),
    _sideslipAngle ( 0.0 )
{
    _cx = Table::createOneRecordTable( 0.0 );
    _cy = Table::createOneRecordTable( 0.0 );
    _cz = Table::createOneRecordTable( 0.0 );
    _cl = Table::createOneRecordTable( 0.0 );
    _cm = Table::createOneRecordTable( 0.0 );
    _cn = Table::createOneRecordTable( 0.0 );
}

////////////////////////////////////////////////////////////////////////////////

Fuselage::~Fuselage() {}

////////////////////////////////////////////////////////////////////////////////

void Fuselage::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        _cx = Table::createOneRecordTable( 0.0 );
        _cy = Table::createOneRecordTable( 0.0 );
        _cz = Table::createOneRecordTable( 0.0 );
        _cl = Table::createOneRecordTable( 0.0 );
        _cm = Table::createOneRecordTable( 0.0 );
        _cn = Table::createOneRecordTable( 0.0 );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _r_ac_bas, "aero_center" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _length , "length" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _area   , "area"   );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _cx, "cx" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _cy, "cy", true );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _cz, "cz", true );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _cl, "cl", true );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _cm, "cm", true );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _cn, "cn", true );

        if ( result == FDM_SUCCESS )
        {
            _sl = _area * _length;
        }
        else
        {
            Exception e;

            e.setType( Exception::FileReadingError );
            e.setInfo( "Reading XML file failed. " + XmlUtils::getErrorInfo( dataNode ) );

            FDM_THROW( e );
        }
    }
    else
    {
        Exception e;

        e.setType( Exception::FileReadingError );
        e.setInfo( "Reading XML file failed. " + XmlUtils::getErrorInfo( dataNode ) );

        FDM_THROW( e );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Fuselage::computeForceAndMoment( const Vector3 &vel_air_bas,
                                      const Vector3 &omg_air_bas,
                                      double airDensity )
{
    // fuselage velocity
    Vector3 vel_f_bas = vel_air_bas + ( omg_air_bas ^ _r_ac_bas );

    // stabilizer angle of attack and sideslip angle
    _angleOfAttack = Aerodynamics::getAngleOfAttack( vel_f_bas );
    _sideslipAngle = Aerodynamics::getSideslipAngle( vel_f_bas );

    // dynamic pressure
    double dynPress = 0.5 * airDensity * vel_f_bas.getLength2();

    Vector3 for_aero( dynPress * getCx( _angleOfAttack ) * _area,
                      dynPress * getCy( _sideslipAngle ) * _area,
                      dynPress * getCz( _angleOfAttack ) * _area );

    Vector3 mom_stab( dynPress * getCl( _sideslipAngle ) * _sl,
                      dynPress * getCm( _angleOfAttack ) * _sl,
                      dynPress * getCn( _sideslipAngle ) * _sl );


    double sinAlpha = sin( _angleOfAttack );
    double cosAlpha = cos( _angleOfAttack );
    double sinBeta  = sin( _sideslipAngle );
    double cosBeta  = cos( _sideslipAngle );

    Vector3 for_bas = Aerodynamics::getAero2BAS( sinAlpha, cosAlpha, sinBeta, cosBeta ) * for_aero;
    Vector3 mom_bas = Aerodynamics::getStab2BAS( sinAlpha, cosAlpha ) * mom_stab
            + ( _r_ac_bas ^ for_bas );

    _for_bas = for_bas;
    _mom_bas = mom_bas;

    if ( !_for_bas.isValid() || !_mom_bas.isValid() )
    {
        Exception e;

        e.setType( Exception::UnexpectedNaN );
        e.setInfo( "NaN detected in the wing model." );

        FDM_THROW( e );
    }
}

////////////////////////////////////////////////////////////////////////////////

double Fuselage::getCx( double angleOfAttack ) const
{
    return _cx.getValue( angleOfAttack );
}

////////////////////////////////////////////////////////////////////////////////

double Fuselage::getCy( double sideslipAngle ) const
{
    return _cy.getValue( sideslipAngle );
}

////////////////////////////////////////////////////////////////////////////////

double Fuselage::getCz( double angleOfAttack ) const
{
    return _cz.getValue( angleOfAttack );
}

////////////////////////////////////////////////////////////////////////////////

double Fuselage::getCl( double sideslipAngle ) const
{
    return _cl.getValue( sideslipAngle );
}

////////////////////////////////////////////////////////////////////////////////

double Fuselage::getCm( double angleOfAttack ) const
{
    return _cm.getValue( angleOfAttack );
}

////////////////////////////////////////////////////////////////////////////////

double Fuselage::getCn( double sideslipAngle ) const
{
    return _cn.getValue( sideslipAngle );
}
