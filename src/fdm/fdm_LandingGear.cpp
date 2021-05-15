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

#include <fdm/fdm_LandingGear.h>
#include <fdm/fdm_Aircraft.h>

#include <fdm/utils/fdm_String.h>
#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

LandingGear::LandingGear( const Aircraft *aircraft, Input *input ) :
    Module ( aircraft, input ),

    _ctrlAngle ( 0.0 ),

    _brake_l ( 0.0 ),
    _brake_r ( 0.0 ),

    _position ( 0.0 ),

    _antiskid ( false ),
    _steering ( false ),

    _onGround ( false )
{}

////////////////////////////////////////////////////////////////////////////////

LandingGear::~LandingGear() {}

////////////////////////////////////////////////////////////////////////////////

void LandingGear::initialize()
{
    _inputABS = getDataRef( "input.controls.abs" );
    _inputNWS = getDataRef( "input.controls.nws" );

    if ( !_inputABS.isValid() || !_inputNWS.isValid() )
    {
        Exception e;

        e.setType( Exception::UnknownException );
        e.setInfo( "Obtaining input data refs in the landing gear module failed." );

        FDM_THROW( e );
    }
}

////////////////////////////////////////////////////////////////////////////////

void LandingGear::update()
{
    _onGround = _for_bas.getLength2() > 0.0;

    _antiskid = _inputABS.getDatab();
    _steering = _inputNWS.getDatab();
}

////////////////////////////////////////////////////////////////////////////////

bool LandingGear::getIsect( const Vector3 &r_a_bas, const Vector3 &r_u_bas,
                            Vector3 *r_c_bas, Vector3 *n_c_bas )
{
    (*r_c_bas) = r_u_bas;
    (*n_c_bas).zeroize();

    Vector3 b_wgs = _aircraft->getBAS2WGS() * r_a_bas + _aircraft->getPos_WGS();
    Vector3 e_wgs = _aircraft->getBAS2WGS() * r_u_bas + _aircraft->getPos_WGS();
    Vector3 r_wgs;
    Vector3 n_wgs;

    if ( FDM_SUCCESS == _aircraft->getIsect()->getIntersection( b_wgs, e_wgs,
                                                                &r_wgs, &n_wgs,
                                                                true ) )
    {
        (*r_c_bas) = _aircraft->getWGS2BAS() * ( r_wgs - _aircraft->getPos_WGS() );
        (*n_c_bas) = _aircraft->getWGS2BAS() * n_wgs;

        return true;
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////

int LandingGear::readWheelsData( XmlNode &dataNode, Wheels &wheels )
{
    int result = FDM_SUCCESS;

    XmlNode wheelNode = dataNode.getFirstChildElement( "wheel" );

    while ( result == FDM_SUCCESS && wheelNode.isValid() )
    {
        WheelAndInput wheelAndInput;

        std::string name  = wheelNode.getAttribute( "name"  );
        std::string input = wheelNode.getAttribute( "input" );

        wheelAndInput.input = getDataRef( input );
        wheelAndInput.wheel.readData( wheelNode );

        result = wheels.addItem( name, wheelAndInput );

        wheelNode = wheelNode.getNextSiblingElement( "wheel" );
    }

    return result;
}
