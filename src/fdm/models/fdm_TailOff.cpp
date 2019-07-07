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

#include <fdm/models/fdm_TailOff.h>

#include <fdm/main/fdm_Aerodynamics.h>
#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

TailOff::TailOff() :
    m_mac ( 0.0 ),
    m_area ( 0.0 ),
    m_area_2 ( 0.0 ),
    m_mac_s_2 ( 0.0 ),
    m_aoa_critical_neg ( 0.0 ),
    m_aoa_critical_pos ( 0.0 ),
    m_aoa_l ( 0.0 ),
    m_aoa_r ( 0.0 ),
    m_stall ( false )
{
    m_cx = Table::createOneRecordTable( 0.0 );
    m_cy = Table::createOneRecordTable( 0.0 );
    m_cz = Table::createOneRecordTable( 0.0 );
    m_cl = Table::createOneRecordTable( 0.0 );
    m_cm = Table::createOneRecordTable( 0.0 );
    m_cn = Table::createOneRecordTable( 0.0 );
}

////////////////////////////////////////////////////////////////////////////////

TailOff::~TailOff() {}

////////////////////////////////////////////////////////////////////////////////

void TailOff::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_r_ac_l_bas, "aero_center_l" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_r_ac_r_bas, "aero_center_r" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_mac  , "mac"  );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_area , "area" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_cx, "cx" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_cy, "cy", true );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_cz, "cz" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_cl, "cl", true );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_cm, "cm" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_cn, "cn", true );

        if ( result == FDM_SUCCESS )
        {
            m_area_2 = 0.5 * m_area;
            m_mac_s_2 = m_mac * m_area_2;

            m_aoa_critical_neg = m_cz.getKeyOfValueMin();
            m_aoa_critical_pos = m_cz.getKeyOfValueMax();
        }
        else
        {
            Exception e;

            e.setType( Exception::FileReadingError );
            e.setInfo( "ERROR! Reading XML file failed. " + XmlUtils::getErrorInfo( dataNode ) );

            FDM_THROW( e );
        }
    }
    else
    {
        Exception e;

        e.setType( Exception::FileReadingError );
        e.setInfo( "ERROR! Reading XML file failed. " + XmlUtils::getErrorInfo( dataNode ) );

        FDM_THROW( e );
    }
}

////////////////////////////////////////////////////////////////////////////////

void TailOff::computeForceAndMoment( const Vector3 &vel_air_bas,
                                     const Vector3 &omg_air_bas,
                                     double airDensity )
{
    m_for_bas.zeroize();
    m_mom_bas.zeroize();

    addForceAndMoment( m_r_ac_l_bas, vel_air_bas, omg_air_bas, airDensity );
    addForceAndMoment( m_r_ac_r_bas, vel_air_bas, omg_air_bas, airDensity );

    if ( !m_for_bas.isValid() || !m_mom_bas.isValid() )
    {
        Exception e;

        e.setType( Exception::UnexpectedNaN );
        e.setInfo( "ERROR! NaN detected in the wing model." );

        FDM_THROW( e );
    }
}

////////////////////////////////////////////////////////////////////////////////

void TailOff::update( const Vector3 &vel_air_bas, const Vector3 &omg_air_bas )
{
    Vector3 vel_l_bas = vel_air_bas + ( omg_air_bas ^ m_r_ac_l_bas );
    Vector3 vel_r_bas = vel_air_bas + ( omg_air_bas ^ m_r_ac_r_bas );

    m_aoa_l = Aerodynamics::getAngleOfAttack( vel_l_bas );
    m_aoa_r = Aerodynamics::getAngleOfAttack( vel_r_bas );

    bool stall_l = ( m_aoa_l < m_aoa_critical_neg ) || ( m_aoa_l > m_aoa_critical_pos );
    bool stall_r = ( m_aoa_r < m_aoa_critical_neg ) || ( m_aoa_r > m_aoa_critical_pos );

    m_stall = stall_l || stall_r;
}

////////////////////////////////////////////////////////////////////////////////

void TailOff::addForceAndMoment( const Vector3 &r_ac_bas,
                                 const Vector3 &vel_air_bas,
                                 const Vector3 &omg_air_bas,
                                 double airDensity )
{
    // wing velocity
    Vector3 vel_wing_bas = vel_air_bas + ( omg_air_bas ^ r_ac_bas );

    // stabilizer angle of attack and sideslip angle
    double angleOfAttack = Aerodynamics::getAngleOfAttack( vel_wing_bas );
    double sideslipAngle = Aerodynamics::getSideslipAngle( vel_wing_bas );

    // dynamic pressure
    double dynPress = 0.5 * airDensity * vel_wing_bas.getLength2();

    Vector3 for_aero( dynPress * getCx( angleOfAttack ) * m_area_2,
                      dynPress * getCy( sideslipAngle ) * m_area_2,
                      dynPress * getCz( angleOfAttack ) * m_area_2 );

    Vector3 mom_stab( dynPress * getCl( sideslipAngle ) * m_mac_s_2,
                      dynPress * getCm( angleOfAttack ) * m_mac_s_2,
                      dynPress * getCn( sideslipAngle ) * m_mac_s_2 );


    double sinAlpha = sin( angleOfAttack );
    double cosAlpha = cos( angleOfAttack );
    double sinBeta  = sin( sideslipAngle );
    double cosBeta  = cos( sideslipAngle );

    Vector3 for_bas = Aerodynamics::getAero2BAS( sinAlpha, cosAlpha, sinBeta, cosBeta ) * for_aero;
    Vector3 mom_bas = Aerodynamics::getStab2BAS( sinAlpha, cosAlpha ) * mom_stab
            + ( r_ac_bas ^ for_bas );

    m_for_bas += for_bas;
    m_mom_bas += mom_bas;
}

////////////////////////////////////////////////////////////////////////////////

double TailOff::getCx( double angleOfAttack ) const
{
    return m_cx.getValue( angleOfAttack );
}

////////////////////////////////////////////////////////////////////////////////

double TailOff::getCy( double sideslipAngle ) const
{
    return m_cy.getValue( sideslipAngle );
}

////////////////////////////////////////////////////////////////////////////////

double TailOff::getCz( double angleOfAttack ) const
{
    return m_cz.getValue( angleOfAttack );
}

////////////////////////////////////////////////////////////////////////////////

double TailOff::getCl( double sideslipAngle ) const
{
    return m_cl.getValue( sideslipAngle );
}

////////////////////////////////////////////////////////////////////////////////

double TailOff::getCm( double angleOfAttack ) const
{
    return m_cm.getValue( angleOfAttack );
}

////////////////////////////////////////////////////////////////////////////////

double TailOff::getCn( double sideslipAngle ) const
{
    return m_cn.getValue( sideslipAngle );
}
