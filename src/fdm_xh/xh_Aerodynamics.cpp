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

#include <fdm_xh/xh_Aerodynamics.h>
#include <fdm_xh/xh_Aircraft.h>

#include <fdm/fdm_Test.h>

#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

XH_Aerodynamics::XH_Aerodynamics( const XH_Aircraft *aircraft, Input *input ) :
    Aerodynamics( aircraft, input ),
    _aircraft ( aircraft ),

    _mainRotor ( FDM_NULLPTR ),
    _tailRotor ( FDM_NULLPTR ),
    _fuselage  ( FDM_NULLPTR ),
    _stabHor   ( FDM_NULLPTR ),
    _stabVer   ( FDM_NULLPTR )
{
    _mainRotor = new XH_MainRotor();
    _tailRotor = new XH_TailRotor();
    _fuselage  = new XH_Fuselage();
    _stabHor   = new XH_StabilizerHor();
    _stabVer   = new XH_StabilizerVer();
}

////////////////////////////////////////////////////////////////////////////////

XH_Aerodynamics::~XH_Aerodynamics()
{
    FDM_DELPTR( _mainRotor );
    FDM_DELPTR( _tailRotor );
    FDM_DELPTR( _fuselage );
    FDM_DELPTR( _stabHor );
    FDM_DELPTR( _stabVer );
}

////////////////////////////////////////////////////////////////////////////////

void XH_Aerodynamics::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        XmlNode nodeTailRotor = dataNode.getFirstChildElement( "tail_rotor" );
        XmlNode nodeFuselage  = dataNode.getFirstChildElement( "fuselage" );
        XmlNode nodeStabHor   = dataNode.getFirstChildElement( "stab_hor" );
        XmlNode nodeStabVer   = dataNode.getFirstChildElement( "stab_ver" );

        _tailRotor->readData( nodeTailRotor );
        _fuselage->readData( nodeFuselage );
        _stabHor->readData( nodeStabHor );
        _stabVer->readData( nodeStabVer );

        XmlNode nodeMainRotor = dataNode.getFirstChildElement( "main_rotor_be" );
        _mainRotor->readData( nodeMainRotor );
    }
    else
    {
        XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
}

////////////////////////////////////////////////////////////////////////////////

void XH_Aerodynamics::initialize()
{
    ///////////////////////////
    Aerodynamics::initialize();
    ///////////////////////////
}

////////////////////////////////////////////////////////////////////////////////

void XH_Aerodynamics::computeForceAndMoment()
{
    updateMatrices();

    _mainRotor->computeForceAndMoment( _aircraft->getVel_air_BAS(),
                                       _aircraft->getOmg_air_BAS(),
                                       _aircraft->getOmg_BAS(),
                                       _aircraft->getAcc_BAS(),
                                       _aircraft->getEps_BAS(),
                                       _aircraft->getGrav_BAS(),
                                       _aircraft->getEnvir()->getDensity() );

    Vector3 b = _mainRotor->getR_hub_BAS();
    Vector3 v = _mainRotor->getFor_BAS();
    v.normalize();
    Test::setVector( b, 5.0 * v );

    _tailRotor->computeForceAndMoment( _aircraft->getVel_air_BAS(),// - _mainRotor->getVel_i_BAS(),
                                       _aircraft->getOmg_air_BAS(),
                                       _aircraft->getEnvir()->getDensity() );

    _fuselage->computeForceAndMoment( _aircraft->getVel_air_BAS(),// - _mainRotor->getVel_i_BAS(),
                                      _aircraft->getOmg_air_BAS(),
                                      _aircraft->getEnvir()->getDensity() );

    _stabHor->computeForceAndMoment( _aircraft->getVel_air_BAS(),// - _mainRotor->getVel_i_BAS(),
                                     _aircraft->getOmg_air_BAS(),
                                     _aircraft->getEnvir()->getDensity(),
                                     _aircraft->getCtrl()->getElevator() );

    _stabVer->computeForceAndMoment( _aircraft->getVel_air_BAS(),// - _tailRotor->getVel_i_BAS(),
                                     _aircraft->getOmg_air_BAS(),
                                     _aircraft->getEnvir()->getDensity() );

    _for_bas = _mainRotor->getFor_BAS() + _tailRotor->getFor_BAS()
             + _fuselage->getFor_BAS()
             + _stabHor->getFor_BAS() + _stabVer->getFor_BAS();

    _mom_bas = _mainRotor->getMom_BAS() + _tailRotor->getMom_BAS()
             + _fuselage->getMom_BAS()
             + _stabHor->getMom_BAS() + _stabVer->getMom_BAS();

//            // damping
//            double R_2v = 0.0;

//            if ( _aircraft->getAirspeed() > 0.1 )
//            {
//                R_2v = _mainRotor->getRadius() / ( 2.0 * _aircraft->getAirspeed() );
//            }

//            Vector3 omg_air_stab = _bas2stab * _aircraft->getOmg_air_BAS();

//            double cl = -0.240 * omg_air_stab.x() * R_2v;
//            double cm = -0.120 * omg_air_stab.y() * R_2v;
//            double cn = -0.120 * omg_air_stab.z() * R_2v;

//            // dynamic pressure
//            double dynPress = _aircraft->getDynPress();

//            double aR = _mainRotor->getDiskArea() * _mainRotor->getRadius();

//            Vector3 mom_aero( dynPress * aR * cl,
//                              dynPress * aR * cm,
//                              dynPress * aR * cn );

//            _mom_bas += _stab2bas * mom_aero;

    // computing forces expressed in Aerodynamic Axes System
    // computing moments expressed in Stability Axes System
    _for_aero = _bas2aero * _for_bas;
    _mom_stab = _bas2stab * _mom_bas;

    if ( !_for_bas.isValid() || !_mom_bas.isValid() )
    {
        Exception e;

        e.setType( Exception::UnexpectedNaN );
        e.setInfo( "NaN detected in the aerodynamics model." );

        FDM_THROW( e );
    }
}

////////////////////////////////////////////////////////////////////////////////

void XH_Aerodynamics::update()
{
    ///////////////////////
    Aerodynamics::update();
    ///////////////////////

    _mainRotor->update( _aircraft->getTimeStep(),
                        _aircraft->getVel_air_BAS(),
                        _aircraft->getOmg_air_BAS(),
                        _aircraft->getOmg_BAS(),
                        _aircraft->getAcc_BAS(),
                        _aircraft->getEps_BAS(),
                        _aircraft->getGrav_BAS(),
                        _aircraft->getProp()->getMainRotorOmega(),
                        _aircraft->getProp()->getMainRotorPsi(),
                        _aircraft->getEnvir()->getDensity(),
                        _aircraft->getCtrl()->getCollective(),
                        _aircraft->getCtrl()->getCyclicLat(),
                        _aircraft->getCtrl()->getCyclicLon() );

    _tailRotor->update( _aircraft->getProp()->getTailRotorOmega(),
                        _aircraft->getCtrl()->getTailPitch() );
}
