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

#include <fdm_c172/c172_Aircraft.h>

#include <fdmXml/fdm_XmlUtils.h>

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

void C172_Propulsion::initialize( bool engineOn )
{
    m_propeller->setRPM( engineOn ? 2700.0 : 0.0 );
}

////////////////////////////////////////////////////////////////////////////////

void C172_Propulsion::initDataRefs()
{
    ///////////////////////////
    Propulsion::initDataRefs();
    ///////////////////////////

    int result = FDM_SUCCESS;

    // inputs
    if ( result == FDM_SUCCESS ) result = addDataRef( "input/propulsion/throttle" , DataNode::Double );
    if ( result == FDM_SUCCESS ) result = addDataRef( "input/propulsion/mixture"  , DataNode::Double );
    if ( result == FDM_SUCCESS ) result = addDataRef( "input/propulsion/fuel"     , DataNode::Bool   );
    if ( result == FDM_SUCCESS ) result = addDataRef( "input/propulsion/ignition" , DataNode::Bool   );
    if ( result == FDM_SUCCESS ) result = addDataRef( "input/propulsion/starter"  , DataNode::Bool   );

    // outputs
    if ( result == FDM_SUCCESS ) result = addDataRef( "output/propulsion/state" , DataNode::Bool   );
    if ( result == FDM_SUCCESS ) result = addDataRef( "output/propulsion/rpm"   , DataNode::Double );
    if ( result == FDM_SUCCESS ) result = addDataRef( "output/propulsion/map"   , DataNode::Double );
    if ( result == FDM_SUCCESS ) result = addDataRef( "output/propulsion/ff"    , DataNode::Double );

    if ( result == FDM_SUCCESS )
    {
        m_drThrottle  = getDataRef( "input/propulsion/throttle" );
        m_drMixture   = getDataRef( "input/propulsion/mixture"  );
        m_drFuel      = getDataRef( "input/propulsion/fuel"     );
        m_drIgnition  = getDataRef( "input/propulsion/ignition" );
        m_drStarter   = getDataRef( "input/propulsion/starter"  );

        m_drEngineOn  = getDataRef( "output/propulsion/state" );
        m_drEngineRPM = getDataRef( "output/propulsion/rpm"   );
        m_drEngineMAP = getDataRef( "output/propulsion/map"   );
        m_drEngineFF  = getDataRef( "output/propulsion/ff"    );
    }
    else
    {
        Exception e;

        e.setType( Exception::UnknownException );
        e.setInfo( "ERROR! Initializing data references failed." );

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

    m_engine->update( m_drThrottle.getDatad(), m_drMixture.getDatad(),
                      m_propeller->getEngineRPM(),
                      m_aircraft->getEnvir()->getPressure(),
                      m_aircraft->getEnvir()->getDensity(),
                      m_drFuel.getDatab(), m_drStarter.getDatab(),
                      m_drIgnition.getDatab(), m_drIgnition.getDatab() );

    m_propeller->update( 1.0, m_engine->getTorque(),
                         m_aircraft->getAirspeed(),
                         m_aircraft->getEnvir()->getDensity() );

    m_drEngineOn  .setDatab( m_engine->getState() == Engine::Running );
    m_drEngineRPM .setDatad( m_engine->getRPM() );
    m_drEngineMAP .setDatad( m_engine->getMAP() );
    m_drEngineFF  .setDatad( m_engine->getFuelFlow() );
}
