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

#include <fdm_c130/c130_Propulsion.h>
#include <fdm_c130/c130_Aircraft.h>

#include <fdm/utils/fdm_String.h>

#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

C130_Propulsion::C130_Propulsion( const C130_Aircraft *aircraft, Input *input ) :
    Propulsion( aircraft, input ),
    _aircraft ( aircraft ),

    _enginesCount ( 4 )
{
    for ( int i = 0; i < _enginesCount; i++ )
    {
        _engine    [ i ] = new C130_Engine();
        _propeller [ i ] = new C130_Propeller();
    }
}

////////////////////////////////////////////////////////////////////////////////

C130_Propulsion::~C130_Propulsion()
{
    for ( int i = 0; i < _enginesCount; i++ )
    {
        FDM_DELPTR( _engine    [ i ] );
        FDM_DELPTR( _propeller [ i ] );
    }
}

////////////////////////////////////////////////////////////////////////////////

void C130_Propulsion::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        XmlNode nodeEngine    = dataNode.getFirstChildElement( "engine"    );
        XmlNode nodePropeller = dataNode.getFirstChildElement( "propeller" );

        for ( int i = 0; i < _enginesCount; i++ )
        {
            _engine[ i ]->readData( nodeEngine );
            _propeller[ i ]->readData( nodePropeller );
        }
    }
    else
    {
        XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
}

////////////////////////////////////////////////////////////////////////////////

void C130_Propulsion::initialize()
{
    /////////////////////////
    Propulsion::initialize();
    /////////////////////////

    bool engineOn = _aircraft->getInitPropState() == Aircraft::Running;

    for ( int i = 0; i < _enginesCount; i++ )
    {
        _propeller[ i ]->setRPM( engineOn ? 600.0 : 0.0 );
        _engine[ i ]->setRPM( _propeller[ i ]->getEngineRPM() );

        std::string number = String::toString( i + 1 );

        std::string idstr_throttle  = "input.engine_" + number + ".throttle";
        std::string idstr_mixture   = "input.engine_" + number + ".mixture";
        std::string idstr_propeller = "input.engine_" + number + ".propeller";
        std::string idstr_fuel      = "input.engine_" + number + ".fuel";
        std::string idstr_ignition  = "input.engine_" + number + ".ignition";
        std::string idstr_starter   = "input.engine_" + number + ".starter";

        _inputThrottle  [ i ] = getDataRef( idstr_throttle  );
        _inputMixture   [ i ] = getDataRef( idstr_mixture   );
        _inputPropeller [ i ] = getDataRef( idstr_propeller );
        _inputFuel      [ i ] = getDataRef( idstr_fuel      );
        _inputIgnition  [ i ] = getDataRef( idstr_ignition  );
        _inputStarter   [ i ] = getDataRef( idstr_starter   );

        if ( !_inputThrottle  [ i ].isValid()
          || !_inputMixture   [ i ].isValid()
          || !_inputPropeller [ i ].isValid()
          || !_inputFuel      [ i ].isValid()
          || !_inputIgnition  [ i ].isValid()
          || !_inputStarter   [ i ].isValid() )
        {
            Exception e;

            e.setType( Exception::UnknownException );
            e.setInfo( "Obtaining input data refs in the propulsion module failed." );

            FDM_THROW( e );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void C130_Propulsion::computeForceAndMoment()
{
    _for_bas.zeroize();
    _mom_bas.zeroize();

    for ( int i = 0; i < _enginesCount; i++ )
    {
        _propeller[ i ]->computeThrust( _aircraft->getAirspeed(),
                                        _aircraft->getEnvir()->getDensity() );

        // thrust and moment due to thrust
        Vector3 for_bas( _propeller[ i ]->getThrust(), 0.0, 0.0 );
        Vector3 mom_bas = _propeller[ i ]->getPos_BAS() % for_bas;

        // gyro effect
        Vector3 omega_bas;

        if ( _propeller[ i ]->getDirection() == Propeller::CW )
        {
            omega_bas.x() =  _propeller[ i ]->getOmega();
        }
        else
        {
            omega_bas.x() = -_propeller[ i ]->getOmega();
        }

        mom_bas += ( _propeller[ i ]->getInertia() + _engine[ i ]->getInertia() )
                 * ( omega_bas % _aircraft->getOmg_BAS() );

        _for_bas += for_bas;
        _mom_bas += mom_bas;
    }

    if ( !_for_bas.isValid() || !_mom_bas.isValid() )
    {
        Exception e;

        e.setType( Exception::UnexpectedNaN );
        e.setInfo( "NaN detected in the propulsion model." );

        FDM_THROW( e );
    }
}

////////////////////////////////////////////////////////////////////////////////

void C130_Propulsion::update()
{
    for ( int i = 0; i < _enginesCount; i++ )
    {
        _propeller[ i ]->integrate( _aircraft->getTimeStep(), _engine[ i ]->getInertia() );

        double throttle  = _inputThrottle  [ i ].getDatad();
        double mixture   = _inputMixture   [ i ].getDatad();
        double propeller = _inputPropeller [ i ].getDatad();

        bool fuel     = _inputFuel     [ i ].getDatab();
        bool ignition = _inputIgnition [ i ].getDatab();
        bool starter  = _inputStarter  [ i ].getDatab();

        _engine[ i ]->update( throttle, mixture, _propeller[ i ]->getEngineRPM(),
                              _aircraft->getEnvir()->getPressure(),
                              _aircraft->getEnvir()->getDensity(),
                              _aircraft->getEnvir()->getDensityAltitude(),
                              fuel, starter, ignition, ignition );

        _propeller[ i ]->update( propeller, _engine[ i ]->getTorque(),
                                 _aircraft->getAirspeed(),
                                 _aircraft->getEnvir()->getDensity() );
    }
}
