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

#include <fdm_c172/c172_Aerodynamics.h>
#include <fdm_c172/c172_Aircraft.h>

#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

C172_Aerodynamics::C172_Aerodynamics( const C172_Aircraft *aircraft, Input *input ) :
    Aerodynamics( aircraft, input ),
    _aircraft ( aircraft ),

    _tailOff ( FDM_NULLPTR ),
    _stabHor ( FDM_NULLPTR ),
    _stabVer ( FDM_NULLPTR ),

    _dl_dtorque ( 0.0 ),
    _dn_dtorque ( 0.0 )
{
    _tailOff = new C172_TailOff();
    _stabHor = new C172_StabilizerHor();
    _stabVer = new C172_StabilizerVer();
}

////////////////////////////////////////////////////////////////////////////////

C172_Aerodynamics::~C172_Aerodynamics()
{
    FDM_DELPTR( _tailOff );
    FDM_DELPTR( _stabHor );
    FDM_DELPTR( _stabVer );
}

////////////////////////////////////////////////////////////////////////////////

void C172_Aerodynamics::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_drag_ground_effect, "drag_ground_effect" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_lift_ground_effect, "lift_ground_effect" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_dl_dtorque, "dl_dtorque" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_dn_dtorque, "dn_dtorque" );

        if ( result != FDM_SUCCESS ) XmlUtils::throwError( __FILE__, __LINE__, dataNode );

        XmlNode nodeTailOff = dataNode.getFirstChildElement( "tail_off" );
        XmlNode nodeStabHor = dataNode.getFirstChildElement( "stab_hor" );
        XmlNode nodeStabVer = dataNode.getFirstChildElement( "stab_ver" );

        _tailOff->readData( nodeTailOff );
        _stabHor->readData( nodeStabHor );
        _stabVer->readData( nodeStabVer );
    }
    else
    {
        XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
}

////////////////////////////////////////////////////////////////////////////////

void C172_Aerodynamics::computeForceAndMoment()
{
    updateMatrices();

    _tailOff->computeForceAndMoment( _aircraft->getVel_air_BAS(),
                                     _aircraft->getOmg_air_BAS(),
                                     _aircraft->getEnvir()->getDensity(),
                                     _aircraft->getCtrl()->getAilerons(),
                                     _aircraft->getCtrl()->getFlaps() );

    _stabHor->computeForceAndMoment( _aircraft->getVel_air_BAS(),
                                     _aircraft->getOmg_air_BAS(),
                                     _aircraft->getEnvir()->getDensity(),
                                     _aircraft->getAngleOfAttack(),
                                     _aircraft->getCtrl()->getElevator(),
                                     _aircraft->getCtrl()->getElevatorTrim() );

    _stabVer->computeForceAndMoment( _aircraft->getVel_air_BAS(),
                                     _aircraft->getOmg_air_BAS(),
                                     _aircraft->getEnvir()->getDensity(),
                                     _aircraft->getCtrl()->getRudder() );

    _for_bas = _tailOff->getFor_BAS() + _stabHor->getFor_BAS() + _stabVer->getFor_BAS();
    _mom_bas = _tailOff->getMom_BAS() + _stabHor->getMom_BAS() + _stabVer->getMom_BAS();

    // computing forces expressed in Aerodynamic Axes System
    // computing moments expressed in Stability Axes System
    _for_aero = _bas2aero * _for_bas;
    _mom_stab = _bas2stab * _mom_bas;

    // wave drag
    _for_aero.x() *= getPrandtlGlauertCoef( _aircraft->getMachNumber() );

    // propwash effect
    _mom_stab.x() += _aircraft->getProp()->getPropeller()->getTorque() * _dl_dtorque;
    _mom_stab.z() += _aircraft->getProp()->getPropeller()->getTorque() * _dn_dtorque;

    // ground effect
    _for_aero.x() *= _drag_ground_effect.getValue( _aircraft->getAltitude_AGL() );
    _for_aero.z() *= _lift_ground_effect.getValue( _aircraft->getAltitude_AGL() );

    // computing forces and moments expressed in BAS
    _for_bas = _aero2bas * _for_aero;
    _mom_bas = _stab2bas * _mom_stab;

    if ( !_for_bas.isValid() || !_mom_bas.isValid() )
    {
        Exception e;

        e.setType( Exception::UnexpectedNaN );
        e.setInfo( "NaN detected in the aerodynamics model." );

        FDM_THROW( e );
    }
}

////////////////////////////////////////////////////////////////////////////////

void C172_Aerodynamics::update()
{
    ///////////////////////
    Aerodynamics::update();
    ///////////////////////

    _tailOff->update( _aircraft->getVel_air_BAS(),  _aircraft->getOmg_air_BAS() );
}
