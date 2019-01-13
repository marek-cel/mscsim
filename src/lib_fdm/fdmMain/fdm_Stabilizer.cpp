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

#include <fdmMain/fdm_Stabilizer.h>

#include <fdmMain/fdm_Aerodynamics.h>
#include <fdmUtils/fdm_String.h>
#include <fdmXml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Stabilizer::Stabilizer() :
    m_type ( Horizontal ),
    m_area ( 0.0 ),
    m_incidence ( 0.0 ),
    m_downwash ( 0.0 )
{
    m_cx = Table::createOneRecordTable( 0.0 );
    m_cy = Table::createOneRecordTable( 0.0 );
    m_cz = Table::createOneRecordTable( 0.0 );
}

////////////////////////////////////////////////////////////////////////////////

Stabilizer::~Stabilizer() {}

////////////////////////////////////////////////////////////////////////////////

void Stabilizer::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        std::string type = dataNode.getAttribute( "type" );

        if      ( 0 == String::icompare( type, "horizontal" ) ) m_type = Horizontal;
        else if ( 0 == String::icompare( type, "vertical"   ) ) m_type = Vertical;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_r_ac_bas, "aero_center" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_area, "area" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_incidence , "incidence" , true );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_downwash  , "downwash"  , true );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_cx, "cx" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_cy, "cy", m_type == Horizontal );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_cz, "cz", m_type == Vertical   );

        if ( result != FDM_SUCCESS )
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

void Stabilizer::computeForceAndMoment( const Vector3 &vel_air_bas,
                                        const Vector3 &omg_air_bas,
                                        double airDensity,
                                        double wingAngleOfAttack )
{
    // stabilizer velocity
    Vector3 vel_stab_bas = vel_air_bas + ( omg_air_bas ^ m_r_ac_bas );

    // stabilizer angle of attack and sideslip angle
    double angleOfAttack = getAngleOfAttack( vel_stab_bas, wingAngleOfAttack );
    double sideslipAngle = Aerodynamics::getSideslipAngle( vel_stab_bas );

    double angle = ( m_type == Horizontal ) ? angleOfAttack : sideslipAngle;

    // dynamic pressure
    double dynPress = 0.5 * airDensity * vel_stab_bas.getLength2();

    Vector3 for_aero( dynPress * getCx( angle ) * m_area,
                      dynPress * getCy( angle ) * m_area,
                      dynPress * getCz( angle ) * m_area );

    m_for_bas = Aerodynamics::getAero2BAS( angleOfAttack, sideslipAngle ) * for_aero;
    m_mom_bas = m_r_ac_bas ^ m_for_bas;

    if ( !m_for_bas.isValid() || !m_mom_bas.isValid() )
    {
        Exception e;

        e.setType( Exception::UnexpectedNaN );
        e.setInfo( "ERROR! NaN detected in the stabilizer model." );

        FDM_THROW( e );
    }
}

////////////////////////////////////////////////////////////////////////////////

double Stabilizer::getAngleOfAttack( const Vector3 &vel_air_bas,
                                     double wingAngleOfAttack )
{
    return Aerodynamics::getAngleOfAttack( vel_air_bas )
         + m_incidence - m_downwash * wingAngleOfAttack;
}

////////////////////////////////////////////////////////////////////////////////

double Stabilizer::getCx( double angle ) const
{
    return m_cx.getValue( angle );
}

////////////////////////////////////////////////////////////////////////////////

double Stabilizer::getCy( double angle ) const
{
    return m_cy.getValue( angle );
}

////////////////////////////////////////////////////////////////////////////////

double Stabilizer::getCz( double angle ) const
{
    return m_cz.getValue( angle );
}
