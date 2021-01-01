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

#include <fdm_uh60/uh60_Propulsion.h>
#include <fdm_uh60/uh60_Aircraft.h>

#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

UH60_Propulsion::UH60_Propulsion( const UH60_Aircraft *aircraft, Input *input ) :
    Propulsion( aircraft, input ),
    _aircraft ( aircraft ),

    _mainRotorPsi ( 0.0 ),
    _tailRotorPsi ( 0.0 ),

    _mainRotorOmega ( 0.0 ),
    _tailRotorOmega ( 0.0 )
{}

////////////////////////////////////////////////////////////////////////////////

UH60_Propulsion::~UH60_Propulsion() {}

////////////////////////////////////////////////////////////////////////////////

void UH60_Propulsion::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        // TODO
    }
    else
    {
        XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
}

////////////////////////////////////////////////////////////////////////////////

void UH60_Propulsion::initialize()
{
    /////////////////////////
    Propulsion::initialize();
    /////////////////////////

    bool engineOn = _aircraft->getInitPropState() == Aircraft::Running;

    // TODO

    if ( engineOn )
    {
        _mainRotorOmega = 2 * M_PI *  258.0 / 60.0;
        _tailRotorOmega = 2 * M_PI * 1190.0 / 60.0;
    }
    else
    {
        _mainRotorOmega = 0.0;
        _tailRotorOmega = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////

void UH60_Propulsion::computeForceAndMoment()
{
    if ( !_for_bas.isValid() || !_mom_bas.isValid() )
    {
        Exception e;

        e.setType( Exception::UnexpectedNaN );
        e.setInfo( "NaN detected in the propulsion model." );

        FDM_THROW( e );
    }
}

////////////////////////////////////////////////////////////////////////////////

void UH60_Propulsion::update()
{
    // TODO
    _mainRotorPsi += _aircraft->getTimeStep() * _mainRotorOmega;
    _tailRotorPsi += _aircraft->getTimeStep() * _tailRotorOmega;

    _mainRotorPsi = Angles::normalize( _mainRotorPsi );
    _tailRotorPsi = Angles::normalize( _tailRotorPsi );

    // TODO
    _mainRotorOmega = 2 * M_PI *  258.0 / 60.0;
    _tailRotorOmega = 2 * M_PI * 1190.0 / 60.0;
}
