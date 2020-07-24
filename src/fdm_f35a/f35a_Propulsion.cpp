/****************************************************************************//*
 * Copyright (C) 2020 Marek M. Cel
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

#include <fdm_f35a/f35a_Propulsion.h>
#include <fdm_f35a/f35a_Aircraft.h>

#include <fdm/utils/fdm_Units.h>
#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

F35A_Propulsion::F35A_Propulsion( const F35A_Aircraft *aircraft ) :
    Propulsion( aircraft ),
    _aircraft ( aircraft ),

    _engine ( 0 )
{
    _engine = new F35A_Engine();
}

////////////////////////////////////////////////////////////////////////////////

F35A_Propulsion::~F35A_Propulsion()
{
    FDM_DELPTR( _engine );
}

////////////////////////////////////////////////////////////////////////////////

void F35A_Propulsion::readData( XmlNode &dataNode )
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

void F35A_Propulsion::initialize()
{
    /////////////////////////
    Propulsion::initialize();
    /////////////////////////

    bool engineOn = _aircraft->getInitPropState() == Aircraft::Running;

    _engine->initialize( engineOn );
}

////////////////////////////////////////////////////////////////////////////////

void F35A_Propulsion::computeForceAndMoment()
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

void F35A_Propulsion::update()
{
    _engine->integrate( _aircraft->getTimeStep() );
    _engine->update( _aircraft->getDataInp()->engine[ 0 ].throttle,
                      Units::k2c( _aircraft->getEnvir()->getTemperature() ),
                      _aircraft->getMachNumber(),
                      _aircraft->getEnvir()->getDensityAltitude(),
                      _aircraft->getDataInp()->engine[ 0 ].fuel,
                      _aircraft->getDataInp()->engine[ 0 ].starter );
}
