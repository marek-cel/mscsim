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

#include <fdm_p51/p51_Propeller.h>

#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

P51_Propeller::P51_Propeller() :
    Propeller(),

    _governor ( 0 )
{
    _governor = new P51_Governor();
}

////////////////////////////////////////////////////////////////////////////////

P51_Propeller::~P51_Propeller()
{
    FDM_DELPTR( _governor );
}

////////////////////////////////////////////////////////////////////////////////

void P51_Propeller::readData( XmlNode &dataNode )
{
    ////////////////////////////////
    Propeller::readData( dataNode );
    ////////////////////////////////

    if ( dataNode.isValid() )
    {
        XmlNode nodeGovernor = dataNode.getFirstChildElement( "governor" );
        _governor->readData( nodeGovernor );
    }
    else
    {
        XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
}

////////////////////////////////////////////////////////////////////////////////

void P51_Propeller::update( double propellerLever, double engineTorque,
                            double airspeed, double airDensity )
{
    _governor->update( propellerLever, _speed_rpm );

    ///////////////////////////////////////////////////////////////////////////////
    Propeller::update( _governor->getPitch(), engineTorque, airspeed, airDensity );
    ///////////////////////////////////////////////////////////////////////////////
}
