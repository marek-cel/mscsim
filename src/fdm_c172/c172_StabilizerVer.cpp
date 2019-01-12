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

#include <fdm_c172/c172_StabilizerVer.h>

#include <fdmXml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

C172_StabilizerVer::C172_StabilizerVer() :
    m_dcx_drudder ( 0.0 ),
    m_dcy_drudder ( 0.0 ),
    m_rudder ( 0.0 )
{}

////////////////////////////////////////////////////////////////////////////////

C172_StabilizerVer::~C172_StabilizerVer() {}

////////////////////////////////////////////////////////////////////////////////

void C172_StabilizerVer::readData( XmlNode &dataNode )
{
    /////////////////////////////////
    Stabilizer::readData( dataNode );
    /////////////////////////////////

    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_dcx_drudder, "dcx_drudder" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_dcy_drudder, "dcy_drudder" );

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

void C172_StabilizerVer::computeForceAndMoment( const Vector3 &vel_air_bas,
                                                const Vector3 &omg_air_bas,
                                                double airDensity,
                                                double rudder )
{
    m_rudder = rudder;

    Stabilizer::computeForceAndMoment( vel_air_bas, omg_air_bas,
                                       airDensity );
}

////////////////////////////////////////////////////////////////////////////////

double C172_StabilizerVer::getCx( double angle ) const
{
    return Stabilizer::getCx( angle )
            + m_dcx_drudder * m_rudder;
}

////////////////////////////////////////////////////////////////////////////////

double C172_StabilizerVer::getCy( double angle ) const
{
    return Stabilizer::getCy( angle )
            + m_dcy_drudder * m_rudder;
}
