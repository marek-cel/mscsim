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

#include <fdm_uh60/uh60_Fuselage.h>

#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

UH60_Fuselage::UH60_Fuselage()
{
    m_cx_beta = Table::createOneRecordTable( 0.0 );
    m_cz_beta = Table::createOneRecordTable( 0.0 );
    m_cm_beta = Table::createOneRecordTable( 0.0 );
}

////////////////////////////////////////////////////////////////////////////////

UH60_Fuselage::~UH60_Fuselage() {}

////////////////////////////////////////////////////////////////////////////////

void UH60_Fuselage::readData( XmlNode &dataNode )
{
    ///////////////////////////////
    Fuselage::readData( dataNode );
    ///////////////////////////////

    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_cx_beta, "cx_beta" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_cz_beta, "cz_beta" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_cm_beta, "cm_beta" );

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

double UH60_Fuselage::getCx( double angleOfAttack ) const
{
    return Fuselage::getCx( angleOfAttack ) + m_cx_beta.getValue( m_sideslipAngle );
}

////////////////////////////////////////////////////////////////////////////////

double UH60_Fuselage::getCz( double angleOfAttack ) const
{
    return Fuselage::getCz( angleOfAttack ) + m_cz_beta.getValue( m_sideslipAngle );
}

////////////////////////////////////////////////////////////////////////////////

double UH60_Fuselage::getCm( double angleOfAttack ) const
{
    return Fuselage::getCm( angleOfAttack ) + m_cm_beta.getValue( m_sideslipAngle );
}
