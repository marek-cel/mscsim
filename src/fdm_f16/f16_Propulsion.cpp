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

#include <fdm_f16/f16_Propulsion.h>
#include <fdm_f16/f16_Aircraft.h>

#include <fdm/utils/fdm_Units.h>
#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

F16_Propulsion::F16_Propulsion( const F16_Aircraft *aircraft, Input *input ) :
    Propulsion( aircraft, input ),
    _aircraft ( aircraft ),

    _engine ( FDM_NULLPTR )
{
    _engine = new F16_Engine();
}

////////////////////////////////////////////////////////////////////////////////

F16_Propulsion::~F16_Propulsion()
{
    FDM_DELPTR( _engine );
}

////////////////////////////////////////////////////////////////////////////////

void F16_Propulsion::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        XmlNode nodeEngine = dataNode.getFirstChildElement( "engine" );

        _engine->readData( nodeEngine );
    }
    else
    {
        XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
}

////////////////////////////////////////////////////////////////////////////////

void F16_Propulsion::initialize()
{
    /////////////////////////
    Propulsion::initialize();
    /////////////////////////

    bool engineOn = _aircraft->getInitPropState() == Aircraft::Running;

    _engine->initialize( engineOn );

    _inputThrottle  = getDataRef( "input.engine_1.throttle" );
    _inputFuel      = getDataRef( "input.engine_1.fuel"     );
    _inputStarter   = getDataRef( "input.engine_1.starter"  );

    if ( !_inputThrottle .isValid()
      || !_inputFuel     .isValid()
      || !_inputStarter  .isValid() )
    {
        Exception e;

        e.setType( Exception::UnknownException );
        e.setInfo( "Obtaining input data refs in the propulsion module failed." );

        FDM_THROW( e );
    }
}

////////////////////////////////////////////////////////////////////////////////

void F16_Propulsion::computeForceAndMoment()
{
    _engine->computeThrust( _aircraft->getMachNumber(),
                            _aircraft->getAltitude_ASL() );

    // thrust and moment due to thrust
    Vector3 for_bas( _engine->getThrust(), 0.0, 0.0 );
    Vector3 mom_bas = _engine->getPos_BAS() % for_bas;

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

void F16_Propulsion::update()
{
    double throttle = _inputThrottle.getDatad();

    bool fuel    = _inputFuel    .getDatab();
    bool starter = _inputStarter .getDatab();

    _engine->integrate( _aircraft->getTimeStep() );
    _engine->update( throttle, Units::k2c( _aircraft->getEnvir()->getTemperature() ),
                     _aircraft->getMachNumber(),
                     _aircraft->getEnvir()->getDensity(),
                     fuel, starter );
}
