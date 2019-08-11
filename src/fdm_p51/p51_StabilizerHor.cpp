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

#include <fdm_p51/p51_StabilizerHor.h>

#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

P51_StabilizerHor::P51_StabilizerHor() :
    m_dcx_delevator ( 0.0 ),
    m_dcz_delevator ( 0.0 ),
    m_dcz_delevator_trim ( 0.0 ),
    m_elevator ( 0.0 ),
    m_elevatorTrim ( 0.0 )
{}

////////////////////////////////////////////////////////////////////////////////

P51_StabilizerHor::~P51_StabilizerHor() {}

////////////////////////////////////////////////////////////////////////////////

void P51_StabilizerHor::readData( XmlNode &dataNode )
{
    /////////////////////////////////
    Stabilizer::readData( dataNode );
    /////////////////////////////////

    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_dcx_delevator, "dcx_delevator" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_dcz_delevator, "dcz_delevator" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_dcz_delevator_trim, "dcz_delevator_trim" );

        if ( result != FDM_SUCCESS )
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

void P51_StabilizerHor::computeForceAndMoment( const Vector3 &vel_air_bas,
                                                const Vector3 &omg_air_bas,
                                                double airDensity,
                                                double wingAngleOfAttack,
                                                double elevator,
                                                double elevatorTrim )
{
    m_elevator     = elevator;
    m_elevatorTrim = elevatorTrim;

    Stabilizer::computeForceAndMoment( vel_air_bas, omg_air_bas,
                                       airDensity, wingAngleOfAttack );
}

////////////////////////////////////////////////////////////////////////////////

double P51_StabilizerHor::getCx( double angle ) const
{
    return Stabilizer::getCx( angle )
            + m_dcx_delevator * m_elevator;
}

////////////////////////////////////////////////////////////////////////////////

double P51_StabilizerHor::getCz( double angle ) const
{
    return Stabilizer::getCz( angle )
            + m_dcz_delevator      * m_elevator
            + m_dcz_delevator_trim * m_elevatorTrim;
}
