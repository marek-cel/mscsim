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

#include <fdm_f16/f16_Propulsion.h>
#include <fdm_f16/f16_Aircraft.h>

#include <fdm/utils/fdm_Units.h>
#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

F16_Propulsion::F16_Propulsion( const F16_Aircraft *aircraft ) :
    Propulsion( aircraft ),
    m_aircraft ( aircraft ),

    m_engine ( 0 )
{
    m_engine = new TurbofanAB();
}

////////////////////////////////////////////////////////////////////////////////

F16_Propulsion::~F16_Propulsion()
{
    if ( m_engine ) delete m_engine;
    m_engine = 0;
}

////////////////////////////////////////////////////////////////////////////////

void F16_Propulsion::init( bool engineOn )
{
    /////////////////////////////
    Propulsion::init( engineOn );
    /////////////////////////////

    m_engine->initialize( engineOn );
}

////////////////////////////////////////////////////////////////////////////////

void F16_Propulsion::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        XmlNode nodeEngine = dataNode.getFirstChildElement( "turbofan_ab" );

        m_engine->readData( nodeEngine );
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

void F16_Propulsion::computeForceAndMoment()
{
    m_engine->computeThrust( m_aircraft->getMachNumber(),
                             m_aircraft->getAltitude_ASL() );

    // thrust and moment due to thrust
    Vector3 for_bas( m_engine->getThrust(), 0.0, 0.0 );
    Vector3 mom_bas = m_engine->getPos_BAS() ^ for_bas;

    m_for_bas = for_bas;
    m_mom_bas = mom_bas;

    if ( !m_for_bas.isValid() || !m_mom_bas.isValid() )
    {
        Exception e;

        e.setType( Exception::UnexpectedNaN );
        e.setInfo( "NaN detected in the propulsion model." );

        FDM_THROW( e );
    }
}

////////////////////////////////////////////////////////////////////////////////

void F16_Propulsion::update()
{
    m_engine->integrate( m_aircraft->getTimeStep() );
    m_engine->update( m_aircraft->getDataInp()->engine[ 0 ].throttle,
                      Units::k2c( m_aircraft->getEnvir()->getTemperature() ),
                      m_aircraft->getMachNumber(),
                      m_aircraft->getAltitude_ASL(),
                      m_aircraft->getDataInp()->engine[ 0 ].fuel,
                      m_aircraft->getDataInp()->engine[ 0 ].starter );
}
