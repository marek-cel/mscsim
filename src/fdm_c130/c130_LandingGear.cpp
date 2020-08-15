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

#include <fdm_c130/c130_LandingGear.h>
#include <fdm_c130/c130_Aircraft.h>

#include <fdm/utils/fdm_String.h>
#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

C130_LandingGear::C130_LandingGear( const C130_Aircraft *aircraft, DataNode *rootNode ) :
    LandingGear( aircraft, rootNode ),
    _aircraft ( aircraft )
{}

////////////////////////////////////////////////////////////////////////////////

C130_LandingGear::~C130_LandingGear() {}

////////////////////////////////////////////////////////////////////////////////

void C130_LandingGear::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        XmlNode wheelNode = dataNode.getFirstChildElement( "wheel" );

        while ( result == FDM_SUCCESS && wheelNode.isValid() )
        {
            Wheel wheel;

            std::string name = wheelNode.getAttribute( "name" );

            wheel.readData( wheelNode );

            result = _wheels.addWheel( name.c_str(), wheel );

            wheelNode = wheelNode.getNextSiblingElement( "wheel" );
        }

        if ( result != FDM_SUCCESS ) XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
    else
    {
        XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
}

////////////////////////////////////////////////////////////////////////////////

void C130_LandingGear::initialize()
{
    Wheel *wheel_n  = _wheels.getWheelByName( "wheel_n"  );
    Wheel *wheel_lf = _wheels.getWheelByName( "wheel_lf" );
    Wheel *wheel_lr = _wheels.getWheelByName( "wheel_lr" );
    Wheel *wheel_rf = _wheels.getWheelByName( "wheel_rf" );
    Wheel *wheel_rr = _wheels.getWheelByName( "wheel_rr" );

    if ( wheel_n && wheel_lf && wheel_lr && wheel_rf && wheel_rr )
    {
        wheel_n  ->setInput( &_aircraft->getDataInp()->controls.landing_gear );
        wheel_lf ->setInput( &_aircraft->getDataInp()->controls.landing_gear );
        wheel_lr ->setInput( &_aircraft->getDataInp()->controls.landing_gear );
        wheel_rf ->setInput( &_aircraft->getDataInp()->controls.landing_gear );
        wheel_rr ->setInput( &_aircraft->getDataInp()->controls.landing_gear );
    }
    else
    {
        Exception e;

        e.setType( Exception::UnknownException );
        e.setInfo( "Obtaining wheels failed." );

        FDM_THROW( e );
    }

    //////////////////////////
    LandingGear::initialize();
    //////////////////////////
}

////////////////////////////////////////////////////////////////////////////////

void C130_LandingGear::computeForceAndMoment()
{
    _for_bas.zeroize();
    _mom_bas.zeroize();

    for ( Wheels::iterator it = _wheels.begin(); it != _wheels.end(); ++it )
    {
        Wheel &wheel = (*it).second;

        double position = 1.0;
        if ( wheel.isInputValid() )
        {
            position = wheel.getInputValue();
        }

        if ( position >= 1.0 )
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

void C130_LandingGear::update()
{
    //////////////////////
    LandingGear::update();
    //////////////////////

    _brake_l = _aircraft->getCtrl()->getBrakeL();
    _brake_r = _aircraft->getCtrl()->getBrakeR();

    _ctrlAngle = 0.0;

    _antiskid = _aircraft->getDataInp()->controls.abs;
    _steering = _aircraft->getDataInp()->controls.nws;

    for ( Wheels::iterator it = _wheels.begin(); it != _wheels.end(); ++it )
    {
        Wheel &wheel = (*it).second;

        Vector3 r_c_bas;
        Vector3 n_c_bas;

        getIsect( wheel.getRa_BAS(), wheel.getRu_BAS(), &r_c_bas, &n_c_bas );

        wheel.integrate( _aircraft->getTimeStep(),
                         _aircraft->getVel_BAS(),
                         _aircraft->getOmg_BAS(),
                         r_c_bas,
                         n_c_bas,
                         _steering );

        double brake = 0.0;
        if      ( wheel.getBrakeGroup() == Wheel::Left  ) brake = _brake_l;
        else if ( wheel.getBrakeGroup() == Wheel::Right ) brake = _brake_r;

        wheel.update( _ctrlAngle, brake );
    }
}
