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

#include <fdm/models/fdm_TailOff.h>

#include <fdm/main/fdm_Aerodynamics.h>
#include <fdm/utils/fdm_Units.h>
#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

TailOff::TailOff() :
    _mac ( 0.0 ),
    _area ( 0.0 ),
    _area_2 ( 0.0 ),
    _mac_s_2 ( 0.0 ),
    _aoa_critical_neg ( 0.0 ),
    _aoa_critical_pos ( 0.0 ),
    _aoa_l ( 0.0 ),
    _aoa_r ( 0.0 ),
    _stall ( false )
{
    _cx = Table1::oneRecordTable( 0.0 );
    _cy = Table1::oneRecordTable( 0.0 );
    _cz = Table1::oneRecordTable( 0.0 );
    _cl = Table1::oneRecordTable( 0.0 );
    _cm = Table1::oneRecordTable( 0.0 );
    _cn = Table1::oneRecordTable( 0.0 );
}

////////////////////////////////////////////////////////////////////////////////

TailOff::~TailOff() {}

////////////////////////////////////////////////////////////////////////////////

void TailOff::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_r_ac_l_bas, "aero_center_l" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_r_ac_r_bas, "aero_center_r" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_mac  , "mac"  );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_area , "area" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_cx, "cx" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_cy, "cy", true );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_cz, "cz" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_cl, "cl", true );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_cm, "cm" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_cn, "cn", true );

        if ( result == FDM_SUCCESS )
        {
            _area_2 = 0.5 * _area;
            _mac_s_2 = _mac * _area_2;

            _aoa_critical_neg = _cz.getKeyOfValueMin();
            _aoa_critical_pos = _cz.getKeyOfValueMax();

            _cx.multiplyKeys( Units::deg2rad() );
            _cy.multiplyKeys( Units::deg2rad() );
            _cz.multiplyKeys( Units::deg2rad() );
            _cl.multiplyKeys( Units::deg2rad() );
            _cm.multiplyKeys( Units::deg2rad() );
            _cn.multiplyKeys( Units::deg2rad() );
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

void TailOff::computeForceAndMoment( const Vector3 &vel_air_bas,
                                     const Vector3 &omg_air_bas,
                                     double airDensity )
{
    _for_bas.zeroize();
    _mom_bas.zeroize();

    addForceAndMoment( _r_ac_l_bas, vel_air_bas, omg_air_bas, airDensity );
    addForceAndMoment( _r_ac_r_bas, vel_air_bas, omg_air_bas, airDensity );

    if ( !_for_bas.isValid() || !_mom_bas.isValid() )
    {
        Exception e;

        e.setType( Exception::UnexpectedNaN );
        e.setInfo( "NaN detected in the wing model." );

        FDM_THROW( e );
    }
}

////////////////////////////////////////////////////////////////////////////////

void TailOff::update( const Vector3 &vel_air_bas, const Vector3 &omg_air_bas )
{
    Vector3 vel_l_bas = vel_air_bas + ( omg_air_bas % _r_ac_l_bas );
    Vector3 vel_r_bas = vel_air_bas + ( omg_air_bas % _r_ac_r_bas );

    _aoa_l = Aerodynamics::getAngleOfAttack( vel_l_bas );
    _aoa_r = Aerodynamics::getAngleOfAttack( vel_r_bas );

    bool stall_l = ( _aoa_l < _aoa_critical_neg ) || ( _aoa_l > _aoa_critical_pos );
    bool stall_r = ( _aoa_r < _aoa_critical_neg ) || ( _aoa_r > _aoa_critical_pos );

    _stall = stall_l || stall_r;
}

////////////////////////////////////////////////////////////////////////////////

void TailOff::addForceAndMoment( const Vector3 &r_ac_bas,
                                 const Vector3 &vel_air_bas,
                                 const Vector3 &omg_air_bas,
                                 double airDensity )
{
    // wing velocity
    Vector3 vel_wing_bas = vel_air_bas + ( omg_air_bas % r_ac_bas );

    // stabilizer angle of attack and sideslip angle
    double angleOfAttack = Aerodynamics::getAngleOfAttack( vel_wing_bas );
    double sideslipAngle = Aerodynamics::getSideslipAngle( vel_wing_bas );

    // dynamic pressure
    double dynPress = 0.5 * airDensity * vel_wing_bas.getLength2();

    Vector3 for_aero( dynPress * getCx( angleOfAttack ) * _area_2,
                      dynPress * getCy( sideslipAngle ) * _area_2,
                      dynPress * getCz( angleOfAttack ) * _area_2 );

    Vector3 mom_stab( dynPress * getCl( sideslipAngle ) * _mac_s_2,
                      dynPress * getCm( angleOfAttack ) * _mac_s_2,
                      dynPress * getCn( sideslipAngle ) * _mac_s_2 );


    double sinAlpha = sin( angleOfAttack );
    double cosAlpha = cos( angleOfAttack );
    double sinBeta  = sin( sideslipAngle );
    double cosBeta  = cos( sideslipAngle );

    Vector3 for_bas = Aerodynamics::getAero2BAS( sinAlpha, cosAlpha, sinBeta, cosBeta ) * for_aero;
    Vector3 mom_bas = Aerodynamics::getStab2BAS( sinAlpha, cosAlpha ) * mom_stab
            + ( r_ac_bas % for_bas );

    _for_bas += for_bas;
    _mom_bas += mom_bas;
}

////////////////////////////////////////////////////////////////////////////////

double TailOff::getCx( double angleOfAttack ) const
{
    return _cx.getValue( angleOfAttack );
}

////////////////////////////////////////////////////////////////////////////////

double TailOff::getCy( double sideslipAngle ) const
{
    return _cy.getValue( sideslipAngle );
}

////////////////////////////////////////////////////////////////////////////////

double TailOff::getCz( double angleOfAttack ) const
{
    return _cz.getValue( angleOfAttack );
}

////////////////////////////////////////////////////////////////////////////////

double TailOff::getCl( double sideslipAngle ) const
{
    return _cl.getValue( sideslipAngle );
}

////////////////////////////////////////////////////////////////////////////////

double TailOff::getCm( double angleOfAttack ) const
{
    return _cm.getValue( angleOfAttack );
}

////////////////////////////////////////////////////////////////////////////////

double TailOff::getCn( double sideslipAngle ) const
{
    return _cn.getValue( sideslipAngle );
}
