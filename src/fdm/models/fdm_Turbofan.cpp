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

#include <fdm/models/fdm_Turbofan.h>

#include <fdm/utils/fdm_Misc.h>
#include <fdm/utils/fdm_Units.h>
#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Turbofan::Turbofan() :
    _n1 ( 0.0 ),
    _n2 ( 0.0 ),
    _tit ( 0.0 ),
    _fuelFlow ( 0.0 ),
    _thrust ( 0.0 )
{}

////////////////////////////////////////////////////////////////////////////////

Turbofan::~Turbofan() {}

////////////////////////////////////////////////////////////////////////////////

void Turbofan::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        // TODO
    }
    else
    {
        Exception e;

        e.setType( Exception::FileReadingError );
        e.setInfo( "Reading XML file failed. " + XmlUtils::getErrorInfo( dataNode ) );

        FDM_THROW( e );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Turbofan::initialize( bool engineOn )
{
    if ( engineOn )
    {
        // TODO
    }
    else
    {
        // TODO
    }
}

////////////////////////////////////////////////////////////////////////////////

void Turbofan::computeThrust( double machNumber, double densityAltitude )
{
    // TODO
    _thrust = 0.0;
}

////////////////////////////////////////////////////////////////////////////////

void Turbofan::integrate( double timeStep )
{
    // TODO
}

////////////////////////////////////////////////////////////////////////////////

void Turbofan::update( double throttle, double temperature,
                       double machNumber, double densityAltitude,
                       bool fuel, bool starter )
{
    computeThrust( machNumber, densityAltitude );
    // TODO
}
