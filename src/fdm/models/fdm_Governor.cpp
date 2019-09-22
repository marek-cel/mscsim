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

#include <fdm/models/fdm_Governor.h>

#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Governor::Governor() :
    _pid  ( 0 ),
    _pitch ( 0.0 )
{
    _pid = new PID( 0.0, 0.0, 0.0, 0.0, 1.0 );
}

////////////////////////////////////////////////////////////////////////////////

Governor::~Governor() {}

////////////////////////////////////////////////////////////////////////////////

void Governor::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        double kp = 0.0;
        double ki = 0.0;
        double kd = 0.0;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, kp, "gain_p", true );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, ki, "gain_i", true );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, kd, "gain_d", true );

        if ( result == FDM_SUCCESS )
        {
            _pid->setKp( kp );
            _pid->setKi( ki );
            _pid->setKd( kd );
        }

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _prop_rpm, "prop_rpm" );

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

void Governor::update( double timeStep, double propellerLever, double rpm )
{
    double error = _prop_rpm.getValue( propellerLever ) - rpm;
    _pid->update( timeStep, error );
    _pitch = _pid->getValue();
    //std::cout << _prop_rpm.getValue( propellerLever ) << "  " << error << "  " << _pitch << std::endl;
}
