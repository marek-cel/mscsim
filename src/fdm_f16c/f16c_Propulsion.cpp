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

#include <fdm_f16c/f16c_Aircraft.h>

#include <fdmUtils/fdm_Units.h>
#include <fdmXml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

F16C_Propulsion::F16C_Propulsion( const F16C_Aircraft *aircraft ) :
    Propulsion( aircraft ),
    m_aircraft ( aircraft ),

    m_engine ( 0 )
{
    m_engine = new Turbojet();
}

////////////////////////////////////////////////////////////////////////////////

F16C_Propulsion::~F16C_Propulsion()
{
    if ( m_engine ) delete m_engine;
    m_engine = 0;
}

////////////////////////////////////////////////////////////////////////////////

void F16C_Propulsion::init( bool engineOn )
{
    /////////////////////////////
    Propulsion::init( engineOn );
    /////////////////////////////

    int result = FDM_SUCCESS;

    // inputs
    if ( result == FDM_SUCCESS ) result = addDataRef( "input/propulsion/throttle" , DataNode::Double );
    if ( result == FDM_SUCCESS ) result = addDataRef( "input/propulsion/fuel"     , DataNode::Bool   );
    if ( result == FDM_SUCCESS ) result = addDataRef( "input/propulsion/ignition" , DataNode::Bool   );
    if ( result == FDM_SUCCESS ) result = addDataRef( "input/propulsion/starter"  , DataNode::Bool   );

    // outputs
    if ( result == FDM_SUCCESS ) result = addDataRef( "output/propulsion/state" , DataNode::Bool   );
    if ( result == FDM_SUCCESS ) result = addDataRef( "output/propulsion/ab"    , DataNode::Bool   );
    if ( result == FDM_SUCCESS ) result = addDataRef( "output/propulsion/n2"    , DataNode::Double );
    if ( result == FDM_SUCCESS ) result = addDataRef( "output/propulsion/tit"   , DataNode::Double );
    if ( result == FDM_SUCCESS ) result = addDataRef( "output/propulsion/ff"    , DataNode::Double );

    if ( result == FDM_SUCCESS )
    {
        m_drThrottle  = getDataRef( "input/propulsion/throttle" );
        m_drFuel      = getDataRef( "input/propulsion/fuel"     );
        m_drIgnition  = getDataRef( "input/propulsion/ignition" );
        m_drStarter   = getDataRef( "input/propulsion/starter"  );

        m_drEngineOn  = getDataRef( "output/propulsion/state" );
        m_drEngineAB  = getDataRef( "output/propulsion/ab"    );
        m_drEngineN2  = getDataRef( "output/propulsion/n2"    );
        m_drEngineTIT = getDataRef( "output/propulsion/tit"   );
        m_drEngineFF  = getDataRef( "output/propulsion/ff"    );
    }
    else
    {
        Exception e;

        e.setType( Exception::UnknownException );
        e.setInfo( "ERROR! Initializing data references failed." );

        FDM_THROW( e );
    }

    m_engine->initialize( engineOn );
}

////////////////////////////////////////////////////////////////////////////////

void F16C_Propulsion::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        XmlNode nodeEngine = dataNode.getFirstChildElement( "turbojet" );

        m_engine->readData( nodeEngine );
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

void F16C_Propulsion::computeForceAndMoment()
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
        e.setInfo( "ERROR! NaN detected in the propulsion model." );

        FDM_THROW( e );
    }
}

////////////////////////////////////////////////////////////////////////////////

void F16C_Propulsion::update()
{
    m_engine->integrate( m_aircraft->getTimeStep() );
    m_engine->update( m_drThrottle.getDatad(), Units::k2c( m_aircraft->getEnvir()->getTemperature() ),
                      m_drFuel.getDatab(), m_drStarter.getDatab() );

    m_drEngineOn  .setDatab( m_engine->getState() == Engine::Running );
    m_drEngineAB  .setDatab( m_engine->getAfterburner() );
    m_drEngineN2  .setDatad( m_engine->getN2() );
    m_drEngineTIT .setDatad( m_engine->getTIT() );
    m_drEngineFF  .setDatad( m_engine->getFuelFlow() );
}
