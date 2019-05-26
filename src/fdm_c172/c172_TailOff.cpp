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

#include <fdm_c172/c172_TailOff.h>

#include <fdmMain/fdm_Aerodynamics.h>
#include <fdmXml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

C172_TailOff::C172_TailOff() :
    m_ailerons ( 0.0 ),
    m_flaps    ( 0.0 ),

    m_dcl_dailerons ( 0.0 )
{
    m_dcx_dflaps = Table::createOneRecordTable( 0.0 );
    m_dcz_dflaps = Table::createOneRecordTable( 0.0 );
    m_dcm_dflaps = Table::createOneRecordTable( 0.0 );
}

////////////////////////////////////////////////////////////////////////////////

C172_TailOff::~C172_TailOff() {}

////////////////////////////////////////////////////////////////////////////////

void C172_TailOff::readData( XmlNode &dataNode )
{
    //////////////////////////////
    TailOff::readData( dataNode );
    //////////////////////////////

    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_dcl_dailerons, "dcl_dailerons" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_dcx_dflaps, "dcx_dflaps"  );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_dcz_dflaps, "dcz_dflaps"  );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_dcm_dflaps, "dcm_dflaps" );

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

void C172_TailOff::computeForceAndMoment( const Vector3 &vel_air_bas,
                                          const Vector3 &omg_air_bas,
                                          double airDensity,
                                          double ailerons,
                                          double flaps )
{
    m_ailerons = ailerons;
    m_flaps    = flaps;

    ///////////////////////////////////////////////////////////////////////
    TailOff::computeForceAndMoment( vel_air_bas, omg_air_bas, airDensity );
    ///////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////

void C172_TailOff::update( const Vector3 &vel_air_bas, const Vector3 &omg_air_bas )
{
    ////////////////////////////////////////////
    TailOff::update( vel_air_bas, omg_air_bas );
    ////////////////////////////////////////////

    Table cz_total = m_cz + m_flaps * m_dcz_dflaps;

    m_aoa_critical_neg = cz_total.getKeyOfValueMin();
    m_aoa_critical_pos = cz_total.getKeyOfValueMax();

    //m_ailerons->getHingeMoment( dynPress, alpha, m_delta_a );
}

////////////////////////////////////////////////////////////////////////////////

double C172_TailOff::getCx( double angleOfAttack ) const
{
    return TailOff::getCx( angleOfAttack ) + m_flaps * m_dcx_dflaps.getValue( angleOfAttack );
}

////////////////////////////////////////////////////////////////////////////////

double C172_TailOff::getCy( double sideslipAngle ) const
{
    return TailOff::getCy( sideslipAngle );
}

////////////////////////////////////////////////////////////////////////////////

double C172_TailOff::getCz( double angleOfAttack ) const
{
    return TailOff::getCz( angleOfAttack ) + m_flaps * m_dcz_dflaps.getValue( angleOfAttack );
}

////////////////////////////////////////////////////////////////////////////////

double C172_TailOff::getCl( double sideslipAngle ) const
{
    return TailOff::getCl( sideslipAngle ) + m_ailerons * m_dcl_dailerons;
}

////////////////////////////////////////////////////////////////////////////////

double C172_TailOff::getCm( double angleOfAttack ) const
{
    return TailOff::getCm( angleOfAttack ) + m_flaps * m_dcm_dflaps.getValue( angleOfAttack );
}

////////////////////////////////////////////////////////////////////////////////

double C172_TailOff::getCn( double sideslipAngle ) const
{
    return TailOff::getCn( sideslipAngle );
}
