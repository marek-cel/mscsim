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

#include <fdm_r44/r44_Aerodynamics.h>
#include <fdm_r44/r44_Aircraft.h>

#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

R44_Aerodynamics::R44_Aerodynamics( const R44_Aircraft *aircraft, Input *input ) :
    Aerodynamics( aircraft, input ),
    _aircraft ( aircraft ),

    _mainRotor ( FDM_NULLPTR ),
    _tailRotor ( FDM_NULLPTR ),
    _fuselage  ( FDM_NULLPTR ),
    _stabHor   ( FDM_NULLPTR ),
    _stabVer   ( FDM_NULLPTR )
{
    _mainRotor = new R44_MainRotor();
    _tailRotor = new R44_TailRotor();
    _fuselage  = new R44_Fuselage();
    _stabHor   = new R44_StabilizerHor();
    _stabVer   = new R44_StabilizerVer();
}

////////////////////////////////////////////////////////////////////////////////

R44_Aerodynamics::~R44_Aerodynamics()
{
    FDM_DELPTR( _mainRotor );
    FDM_DELPTR( _tailRotor );
    FDM_DELPTR( _fuselage );
    FDM_DELPTR( _stabHor );
    FDM_DELPTR( _stabVer );
}

////////////////////////////////////////////////////////////////////////////////

void R44_Aerodynamics::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        XmlNode nodeMainRotor = dataNode.getFirstChildElement( "main_rotor" );
        XmlNode nodeTailRotor = dataNode.getFirstChildElement( "tail_rotor" );
        XmlNode nodeFuselage  = dataNode.getFirstChildElement( "fuselage" );
        XmlNode nodeStabHor   = dataNode.getFirstChildElement( "stab_hor" );
        XmlNode nodeStabVer   = dataNode.getFirstChildElement( "stab_ver" );

        _mainRotor->readData( nodeMainRotor );
        _tailRotor->readData( nodeTailRotor );
        _fuselage->readData( nodeFuselage );
        _stabHor->readData( nodeStabHor );
        _stabVer->readData( nodeStabVer );
    }
    else
    {
        XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
}

////////////////////////////////////////////////////////////////////////////////

void R44_Aerodynamics::initialize()
{
    ///////////////////////////
    Aerodynamics::initialize();
    ///////////////////////////
}

////////////////////////////////////////////////////////////////////////////////

void R44_Aerodynamics::computeForceAndMoment()
{
    updateMatrices();

    _mainRotor->computeForceAndMoment( _aircraft->getVel_BAS(),
                                       _aircraft->getOmg_BAS(),
                                       _aircraft->getAcc_BAS(),
                                       _aircraft->getEps_BAS(),
                                       _aircraft->getVel_air_BAS(),
                                       _aircraft->getOmg_air_BAS(),
                                       _aircraft->getGrav_BAS(),
                                       _aircraft->getEnvir()->getDensity() );

    _tailRotor->computeForceAndMoment( _aircraft->getVel_air_BAS(),
                                       _aircraft->getOmg_air_BAS(),
                                       _aircraft->getEnvir()->getDensity() );

    _fuselage->computeForceAndMoment( _aircraft->getVel_air_BAS(),
                                      _aircraft->getOmg_air_BAS(),
                                      _aircraft->getEnvir()->getDensity(),
                                      _mainRotor->getBeta1c(),
                                      _mainRotor->getWakeSkew() );

    _stabHor->computeForceAndMoment( _aircraft->getVel_air_BAS(),
                                     _aircraft->getOmg_air_BAS(),
                                     _aircraft->getEnvir()->getDensity() );

    _stabVer->computeForceAndMoment( _aircraft->getVel_air_BAS(),
                                     _aircraft->getOmg_air_BAS(),
                                     _aircraft->getEnvir()->getDensity() );

    _for_bas = _mainRotor->getFor_BAS() + _tailRotor->getFor_BAS()
             + _fuselage->getFor_BAS()
             + _stabHor->getFor_BAS() + _stabVer->getFor_BAS();

    _mom_bas = _mainRotor->getMom_BAS() + _tailRotor->getMom_BAS()
             + _fuselage->getMom_BAS()
             + _stabHor->getMom_BAS() + _stabVer->getMom_BAS();

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

void R44_Aerodynamics::update()
{
    ///////////////////////
    Aerodynamics::update();
    ///////////////////////

    _mainRotor->update( _aircraft->getProp()->getMainRotorOmega(),
                        _aircraft->getProp()->getMainRotorPsi(),
                        _aircraft->getCtrl()->getCollective(),
                        _aircraft->getCtrl()->getCyclicLat(),
                        _aircraft->getCtrl()->getCyclicLon() );

    _tailRotor->update( _aircraft->getProp()->getTailRotorOmega(),
                        _aircraft->getCtrl()->getTailPitch() );
}
