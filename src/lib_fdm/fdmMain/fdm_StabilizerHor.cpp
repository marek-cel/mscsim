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

#include <fdmMain/fdm_StabilizerHor.h>

#include <fdmMain/fdm_Aerodynamics.h>
#include <fdmUtils/fdm_Units.h>
#include <fdmXml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

StabilizerHor::StabilizerHor() :
    m_incidence ( 0.0 ),
    m_downwash ( 0.0 )
{}

////////////////////////////////////////////////////////////////////////////////

StabilizerHor::~StabilizerHor() {}

////////////////////////////////////////////////////////////////////////////////

void StabilizerHor::readData( XmlNode &dataNode )
{
    /////////////////////////////////
    Stabilizer::readData( dataNode );
    /////////////////////////////////

    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_incidence , "incidence" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_downwash  , "downwash"  );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_cx, "cx" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_cz, "cz" );

        if ( result != FDM_SUCCESS )
        {
            Exception e;

            e.setType( Exception::FileReadingError );
            e.setInfo( "Error reading XML file. " + XmlUtils::getErrorInfo( dataNode ) );

            FDM_THROW( e );
        }
    }
    else
    {
        Exception e;

        e.setType( Exception::FileReadingError );
        e.setInfo( "Error reading XML file. " + XmlUtils::getErrorInfo( dataNode ) );

        FDM_THROW( e );
    }
}

////////////////////////////////////////////////////////////////////////////////

double StabilizerHor::getAngleOfAttack( const Vector3 &vel_air_bas,
                                        double angleOfAttackWing )
{
    return Aerodynamics::getAngleOfAttack( vel_air_bas )
         + m_incidence
         - m_downwash * angleOfAttackWing;
}
