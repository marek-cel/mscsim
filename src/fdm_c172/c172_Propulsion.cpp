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

#include <fdm_c172/c172_Propulsion.h>
#include <fdm_c172/c172_Aircraft.h>

#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

C172_Propulsion::C172_Propulsion( const C172_Aircraft *aircraft ) :
    Propulsion( aircraft ),
    m_aircraft ( aircraft ),

    m_engine ( 0 ),
    m_propeller ( 0 )
{
    m_engine    = new PistonEngine();
    m_propeller = new Propeller();
}

////////////////////////////////////////////////////////////////////////////////

C172_Propulsion::~C172_Propulsion()
{
    if ( m_engine ) delete m_engine;
    m_engine = 0;

    if ( m_propeller ) delete m_propeller;
    m_propeller = 0;
}

////////////////////////////////////////////////////////////////////////////////

void C172_Propulsion::init( bool engineOn )
{
    /////////////////////////////
    Propulsion::init( engineOn );
    /////////////////////////////

    m_propeller->setRPM( engineOn ? 2700.0 : 0.0 );
}

////////////////////////////////////////////////////////////////////////////////

void C172_Propulsion::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        XmlNode nodeEngine    = dataNode.getFirstChildElement( "piston_engine" );
        XmlNode nodePropeller = dataNode.getFirstChildElement( "propeller"     );

        m_engine->readData( nodeEngine );
        m_propeller->readData( nodePropeller );
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

void C172_Propulsion::computeForceAndMoment()
{
    m_propeller->computeThrust( m_aircraft->getAirspeed(),
                                m_aircraft->getEnvir()->getDensity() );

    // thrust and moment due to thrust
    Vector3 for_bas( m_propeller->getThrust(), 0.0, 0.0 );
    Vector3 mom_bas = m_propeller->getPos_BAS() ^ for_bas;

    // gyro effect
    Vector3 omega_bas;

    if ( m_propeller->getDirection() == Propeller::CW )
    {
        omega_bas.x() =  m_propeller->getOmega();
    }
    else
    {
        omega_bas.x() = -m_propeller->getOmega();
    }

    mom_bas += ( m_propeller->getInertia() + m_engine->getInertia() ) * ( omega_bas ^ m_aircraft->getOmg_BAS() );

    m_for_bas = for_bas;
    m_mom_bas = mom_bas;

    if ( !m_for_bas.isValid() || !m_mom_bas.isValid() )
    {
        Exception e;

        e.setType( Exception::UnexpectedNaN );
        e.setInfo( "ERROR! NaN detected in the propulsion model." );

        FDM_THROW( e );
    }
}

////////////////////////////////////////////////////////////////////////////////

void C172_Propulsion::update()
{
    m_propeller->integrate( m_aircraft->getTimeStep(), m_engine->getInertia() );

    m_engine->update( m_aircraft->getDataInp()->engine[ 0 ].throttle,
                      m_aircraft->getDataInp()->engine[ 0 ].mixture,
                      m_propeller->getEngineRPM(),
                      m_aircraft->getEnvir()->getPressure(),
                      m_aircraft->getEnvir()->getDensity(),
                      m_aircraft->getDataInp()->engine[ 0 ].fuel,
                      m_aircraft->getDataInp()->engine[ 0 ].starter,
                      m_aircraft->getDataInp()->engine[ 0 ].ignition,
                      m_aircraft->getDataInp()->engine[ 0 ].ignition );

    m_propeller->update( m_aircraft->getDataInp()->engine[ 0 ].propeller,
                         m_engine->getTorque(),
                         m_aircraft->getAirspeed(),
                         m_aircraft->getEnvir()->getDensity() );
}
