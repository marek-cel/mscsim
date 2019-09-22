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

#include <fdm/models/fdm_Compressor.h>
#include <fdm/models/fdm_Atmosphere.h>

#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

const double Compressor::_gamma = Atmosphere::_gamma;

////////////////////////////////////////////////////////////////////////////////

Compressor::Compressor() :
    _temperature ( Atmosphere::_std_sl_t ),
    _pressure    ( Atmosphere::_std_sl_p ),
    _density     ( Atmosphere::_std_sl_rho )
{}

////////////////////////////////////////////////////////////////////////////////

Compressor::~Compressor() {}

////////////////////////////////////////////////////////////////////////////////

void Compressor::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _performance_map, "performance_map" );

        if ( result != FDM_SUCCESS )
        {
            Exception e;

            e.setType( Exception::FileReadingError );
            e.setInfo( "Reading XML file failed. " + XmlUtils::getErrorInfo( dataNode ) );

            FDM_THROW( e );
        }
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

void Compressor::update( double airPressure, double airDensity, double airTemperature,
                         double airFlow, double rpm )
{
    // pressure ratio
    double pressureRatio = _performance_map.getValue( airFlow, rpm );

    double pressure    = airPressure * pressureRatio;
    double temperature = airTemperature * pow( pressureRatio, ( _gamma - 1.0 ) / _gamma );
    double density     = airDensity * ( airTemperature / _temperature ) * pressureRatio;

    _pressure    = Misc::inertia( pressure    , _pressure    , FDM_TIME_STEP, 0.05 );
    _temperature = Misc::inertia( temperature , _temperature , FDM_TIME_STEP, 0.05 );
    _density     = Misc::inertia( density     , _density     , FDM_TIME_STEP, 0.05 );
}
