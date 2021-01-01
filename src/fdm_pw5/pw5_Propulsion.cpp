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

#include <fdm_pw5/pw5_Propulsion.h>
#include <fdm_pw5/pw5_Aircraft.h>

#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

PW5_Propulsion::PW5_Propulsion( const PW5_Aircraft *aircraft, Input *input ) :
    Propulsion( aircraft, input ),
    _aircraft ( aircraft ),

    _winchLauncher ( FDM_NULLPTR )
{
    _winchLauncher = new WinchLauncher();
}

////////////////////////////////////////////////////////////////////////////////

PW5_Propulsion::~PW5_Propulsion()
{
    FDM_DELPTR( _winchLauncher );
}

////////////////////////////////////////////////////////////////////////////////

void PW5_Propulsion::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        XmlNode winchLauncherNode = dataNode.getFirstChildElement( "winch_launcher" );

        _winchLauncher->readData( winchLauncherNode );
    }
    else
    {
        XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
}


////////////////////////////////////////////////////////////////////////////////

void PW5_Propulsion::computeForceAndMoment()
{
    _for_bas.zeroize();
    _mom_bas.zeroize();

    _winchLauncher->computeForceAndMoment( _aircraft->getWGS2BAS(),
                                           _aircraft->getPos_WGS() );

    _for_bas += _winchLauncher->getFor_BAS();
    _mom_bas += _winchLauncher->getMom_BAS();
}

////////////////////////////////////////////////////////////////////////////////

void PW5_Propulsion::update()
{
    _winchLauncher->update( _aircraft->getTimeStep(),
                            _aircraft->getBAS2WGS(),
                            _aircraft->getWGS2NED(),
                            _aircraft->getPos_WGS(),
                            _aircraft->getAltitude_AGL() );
}
