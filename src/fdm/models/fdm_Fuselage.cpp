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
    m_length ( 0.0 ),
    m_area ( 0.0 ),
    m_sl ( 0.0 ),

    m_angleOfAttack ( 0.0 ),
    m_sideslipAngle ( 0.0 )
{
    m_cx = Table::createOneRecordTable( 0.0 );
    m_cy = Table::createOneRecordTable( 0.0 );
    m_cz = Table::createOneRecordTable( 0.0 );
    m_cl = Table::createOneRecordTable( 0.0 );
    m_cm = Table::createOneRecordTable( 0.0 );
    m_cn = Table::createOneRecordTable( 0.0 );
}

////////////////////////////////////////////////////////////////////////////////

Fuselage::~Fuselage() {}

////////////////////////////////////////////////////////////////////////////////

void Fuselage::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        m_cx = Table::createOneRecordTable( 0.0 );
        m_cy = Table::createOneRecordTable( 0.0 );
        m_cz = Table::createOneRecordTable( 0.0 );
        m_cl = Table::createOneRecordTable( 0.0 );
        m_cm = Table::createOneRecordTable( 0.0 );
        m_cn = Table::createOneRecordTable( 0.0 );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_r_ac_bas, "aero_center" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_length , "length" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_area   , "area"   );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_cx, "cx" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_cy, "cy", true );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_cz, "cz", true );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_cl, "cl", true );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_cm, "cm", true );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_cn, "cn", true );

        if ( result == FDM_SUCCESS )
        {
            m_sl = m_area * m_length;
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
    Vector3 vel_f_bas = vel_air_bas + ( omg_air_bas ^ m_r_ac_bas );

    // stabilizer angle of attack and sideslip angle
    m_angleOfAttack = Aerodynamics::getAngleOfAttack( vel_f_bas );
    m_sideslipAngle = Aerodynamics::getSideslipAngle( vel_f_bas );

    // dynamic pressure
    double dynPress = 0.5 * airDensity * vel_f_bas.getLength2();

    Vector3 for_aero( dynPress * getCx( m_angleOfAttack ) * m_area,
                      dynPress * getCy( m_sideslipAngle ) * m_area,
                      dynPress * getCz( m_angleOfAttack ) * m_area );

    Vector3 mom_stab( dynPress * getCl( m_sideslipAngle ) * m_sl,
                      dynPress * getCm( m_angleOfAttack ) * m_sl,
                      dynPress * getCn( m_sideslipAngle ) * m_sl );


    double sinAlpha = sin( m_angleOfAttack );
    double cosAlpha = cos( m_angleOfAttack );
    double sinBeta  = sin( m_sideslipAngle );
    double cosBeta  = cos( m_sideslipAngle );

    Vector3 for_bas = Aerodynamics::getAero2BAS( sinAlpha, cosAlpha, sinBeta, cosBeta ) * for_aero;
    Vector3 mom_bas = Aerodynamics::getStab2BAS( sinAlpha, cosAlpha ) * mom_stab
            + ( m_r_ac_bas ^ for_bas );

    m_for_bas = for_bas;
    m_mom_bas = mom_bas;

    if ( !m_for_bas.isValid() || !m_mom_bas.isValid() )
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
    return m_cx.getValue( angleOfAttack );
}

////////////////////////////////////////////////////////////////////////////////

double Fuselage::getCy( double sideslipAngle ) const
{
    return m_cy.getValue( sideslipAngle );
}

////////////////////////////////////////////////////////////////////////////////

double Fuselage::getCz( double angleOfAttack ) const
{
    return m_cz.getValue( angleOfAttack );
}

////////////////////////////////////////////////////////////////////////////////

double Fuselage::getCl( double sideslipAngle ) const
{
    return m_cl.getValue( sideslipAngle );
}

////////////////////////////////////////////////////////////////////////////////

double Fuselage::getCm( double angleOfAttack ) const
{
    return m_cm.getValue( angleOfAttack );
}

////////////////////////////////////////////////////////////////////////////////

double Fuselage::getCn( double sideslipAngle ) const
{
    return m_cn.getValue( sideslipAngle );
}
