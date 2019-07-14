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

#include <fdm_uh60/uh60_Aircraft.h>

#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

UH60_Propulsion::UH60_Propulsion( const UH60_Aircraft *aircraft ) :
    Propulsion( aircraft ),
    m_aircraft ( aircraft ),

    m_mainRotorPsi ( 0.0 ),
    m_tailRotorPsi ( 0.0 ),

    m_mainRotorOmega ( 0.0 ),
    m_tailRotorOmega ( 0.0 )
{}

////////////////////////////////////////////////////////////////////////////////

UH60_Propulsion::~UH60_Propulsion() {}

////////////////////////////////////////////////////////////////////////////////

void UH60_Propulsion::init( bool engineOn )
{
    /////////////////////////////
    Propulsion::init( engineOn );
    /////////////////////////////

    // TODO

    if ( engineOn )
    {
        m_mainRotorOmega = 2 * M_PI *  258.0 / 60.0;
        m_tailRotorOmega = 2 * M_PI * 1190.0 / 60.0;
    }
    else
    {
        m_mainRotorOmega = 0.0;
        m_tailRotorOmega = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////

void UH60_Propulsion::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        // TODO
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

void UH60_Propulsion::computeForceAndMoment()
{
    if ( !m_for_bas.isValid() || !m_mom_bas.isValid() )
    {
        Exception e;

        e.setType( Exception::UnexpectedNaN );
        e.setInfo( "NaN detected in the propulsion model." );

        FDM_THROW( e );
    }
}

////////////////////////////////////////////////////////////////////////////////

void UH60_Propulsion::update()
{
    // TODO
    m_mainRotorPsi += m_aircraft->getTimeStep() * m_mainRotorOmega;
    m_tailRotorPsi += m_aircraft->getTimeStep() * m_tailRotorOmega;

    while ( m_mainRotorPsi > 2.0 * M_PI ) m_mainRotorPsi -= 2.0 * M_PI;
    while ( m_tailRotorPsi > 2.0 * M_PI ) m_tailRotorPsi -= 2.0 * M_PI;

    // TODO
    m_mainRotorOmega = 2 * M_PI *  258.0 / 60.0;
    m_tailRotorOmega = 2 * M_PI * 1190.0 / 60.0;
}
