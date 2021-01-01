/****************************************************************************//*
 * Copyright (C) 2021 Marek M. Cel
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

C172_Propulsion::C172_Propulsion( const C172_Aircraft *aircraft, Input *input ) :
    Propulsion( aircraft, input ),
    _aircraft ( aircraft ),

    _engine    ( FDM_NULLPTR ),
    _propeller ( FDM_NULLPTR )
{
    _engine    = new C172_Engine();
    _propeller = new C172_Propeller();
}

////////////////////////////////////////////////////////////////////////////////

C172_Propulsion::~C172_Propulsion()
{
    FDM_DELPTR( _engine    );
    FDM_DELPTR( _propeller );
}

////////////////////////////////////////////////////////////////////////////////

void C172_Propulsion::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        XmlNode nodeEngine    = dataNode.getFirstChildElement( "engine"    );
        XmlNode nodePropeller = dataNode.getFirstChildElement( "propeller" );

        _engine->readData( nodeEngine );
        _propeller->readData( nodePropeller );
    }
    else
    {
        XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
}

////////////////////////////////////////////////////////////////////////////////

void C172_Propulsion::initialize()
{
    /////////////////////////
    Propulsion::initialize();
    /////////////////////////

    bool engineOn = _aircraft->getInitPropState() == Aircraft::Running;

    _propeller->setRPM( engineOn ? 600.0 : 0.0 );
    _engine->setRPM( _propeller->getEngineRPM() );

    _inputThrottle  = getDataRef( "input.engine_1.throttle"  );
    _inputMixture   = getDataRef( "input.engine_1.mixture"   );
    _inputPropeller = getDataRef( "input.engine_1.propeller" );
    _inputFuel      = getDataRef( "input.engine_1.fuel"      );
    _inputIgnition  = getDataRef( "input.engine_1.ignition"  );
    _inputStarter   = getDataRef( "input.engine_1.starter"   );

    if ( !_inputThrottle  .isValid()
      || !_inputMixture   .isValid()
      || !_inputPropeller .isValid()
      || !_inputFuel      .isValid()
      || !_inputIgnition  .isValid()
      || !_inputStarter   .isValid() )
    {
        Exception e;

        e.setType( Exception::UnknownException );
        e.setInfo( "Obtaining input data refs in the propulsion module failed." );

        FDM_THROW( e );
    }
}

////////////////////////////////////////////////////////////////////////////////

void C172_Propulsion::computeForceAndMoment()
{
    _propeller->computeThrust( _aircraft->getAirspeed(),
                               _aircraft->getEnvir()->getDensity() );

    // thrust and moment due to thrust
    Vector3 for_bas( _propeller->getThrust(), 0.0, 0.0 );
    Vector3 mom_bas = _propeller->getPos_BAS() % for_bas;

    // gyro effect
    Vector3 omega_bas;

    if ( _propeller->getDirection() == Propeller::CW )
    {
        omega_bas.x() =  _propeller->getOmega();
    }
    else
    {
        omega_bas.x() = -_propeller->getOmega();
    }

    mom_bas += ( _propeller->getInertia() + _engine->getInertia() ) * ( omega_bas % _aircraft->getOmg_BAS() );

    _for_bas = for_bas;
    _mom_bas = mom_bas;

    if ( !_for_bas.isValid() || !_mom_bas.isValid() )
    {
        Exception e;

        e.setType( Exception::UnexpectedNaN );
        e.setInfo( "NaN detected in the propulsion model." );

        FDM_THROW( e );
    }
}

////////////////////////////////////////////////////////////////////////////////

void C172_Propulsion::update()
{
    _propeller->integrate( _aircraft->getTimeStep(), _engine->getInertia() );

    double throttle  = _inputThrottle  .getDatad();
    double mixture   = _inputMixture   .getDatad();
    double propeller = _inputPropeller .getDatad();

    bool fuel     = _inputFuel     .getDatab();
    bool ignition = _inputIgnition .getDatab();
    bool starter  = _inputStarter  .getDatab();

    _engine->update( throttle, mixture, _propeller->getEngineRPM(),
                     _aircraft->getEnvir()->getPressure(),
                     _aircraft->getEnvir()->getDensity(),
                     _aircraft->getEnvir()->getDensityAltitude(),
                     fuel, starter, ignition, ignition );

    _propeller->update( propeller, _engine->getTorque(),
                        _aircraft->getAirspeed(),
                        _aircraft->getEnvir()->getDensity() );
}
