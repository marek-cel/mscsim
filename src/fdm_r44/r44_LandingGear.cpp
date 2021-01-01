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

#include <fdm_r44/r44_LandingGear.h>
#include <fdm_r44/r44_Aircraft.h>

#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

R44_LandingGear::R44_LandingGear( const R44_Aircraft *aircraft, Input *input ) :
    LandingGear( aircraft, input ),
    _aircraft ( aircraft )
{}

////////////////////////////////////////////////////////////////////////////////

R44_LandingGear::~R44_LandingGear() {}

////////////////////////////////////////////////////////////////////////////////

void R44_LandingGear::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        if ( FDM_SUCCESS != readWheelsData( dataNode, _wheels ) )
        {
            XmlUtils::throwError( __FILE__, __LINE__, dataNode );
        }
    }
    else
    {
        XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
}

////////////////////////////////////////////////////////////////////////////////

void R44_LandingGear::computeForceAndMoment()
{
    _for_bas.zeroize();
    _mom_bas.zeroize();

    for ( Wheels::iterator it = _wheels.begin(); it != _wheels.end(); ++it )
    {
        Wheel &wheel = (*it).second.wheel;

        if ( wheel.getPosition() >= 1.0 )
        {
            Vector3 r_c_bas;
            Vector3 n_c_bas;

            getIsect( wheel.getRa_BAS(), wheel.getRu_BAS(), &r_c_bas, &n_c_bas );

            wheel.computeForceAndMoment( _aircraft->getVel_BAS(),
                                         _aircraft->getOmg_BAS(),
                                         r_c_bas,
                                         n_c_bas,
                                         _steering, _antiskid );

            _for_bas += wheel.getFor_BAS();
            _mom_bas += wheel.getMom_BAS();
        }
    }

    if ( !_for_bas.isValid() || !_mom_bas.isValid() )
    {
        Exception e;

        e.setType( Exception::UnexpectedNaN );
        e.setInfo( "NaN detected in the landing gear model." );

        FDM_THROW( e );
    }
}

////////////////////////////////////////////////////////////////////////////////

void R44_LandingGear::update()
{
    //////////////////////
    LandingGear::update();
    //////////////////////

    _brake_l = 0.0;
    _brake_r = 0.0;

    _ctrlAngle = 0.0;

    for ( Wheels::iterator it = _wheels.begin(); it != _wheels.end(); ++it )
    {
        Wheel &wheel = (*it).second.wheel;

        Vector3 r_c_bas;
        Vector3 n_c_bas;

        getIsect( wheel.getRa_BAS(), wheel.getRu_BAS(), &r_c_bas, &n_c_bas );

        wheel.integrate( _aircraft->getTimeStep(),
                         _aircraft->getVel_BAS(),
                         _aircraft->getOmg_BAS(),
                         r_c_bas,
                         n_c_bas,
                         _steering );
    }
}
