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

#include <fdm_c172/c172_Aircraft.h>

#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

C172_Aerodynamics::C172_Aerodynamics( const C172_Aircraft *aircraft ) :
    Aerodynamics( aircraft ),
    m_aircraft ( aircraft ),

    m_tailOff ( 0 ),
    m_stabHor ( 0 ),
    m_stabVer ( 0 ),

    m_dl_dtorque ( 0.0 ),
    m_dn_dtorque ( 0.0 )
{
    m_tailOff = new C172_TailOff();
    m_stabHor = new C172_StabilizerHor();
    m_stabVer = new C172_StabilizerVer();
}

////////////////////////////////////////////////////////////////////////////////

C172_Aerodynamics::~C172_Aerodynamics()
{
    if ( m_tailOff ) delete m_tailOff;
    m_tailOff = 0;

    if ( m_stabHor ) delete m_stabHor;
    m_stabHor = 0;

    if ( m_stabVer ) delete m_stabVer;
    m_stabVer = 0;
}

////////////////////////////////////////////////////////////////////////////////

void C172_Aerodynamics::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_drag_ground_effect, "drag_ground_effect" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_lift_ground_effect, "lift_ground_effect" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_dl_dtorque, "dl_dtorque" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_dn_dtorque, "dn_dtorque" );

        if ( result != FDM_SUCCESS )
        {
            Exception e;

            e.setType( Exception::FileReadingError );
            e.setInfo( "ERROR! Reading XML file failed. " + XmlUtils::getErrorInfo( dataNode ) );

            FDM_THROW( e );
        }

        XmlNode nodeTailOff = dataNode.getFirstChildElement( "tail_off" );
        XmlNode nodeStabHor = dataNode.getFirstChildElement( "stab_hor" );
        XmlNode nodeStabVer = dataNode.getFirstChildElement( "stab_ver" );

        m_tailOff->readData( nodeTailOff );
        m_stabHor->readData( nodeStabHor );
        m_stabVer->readData( nodeStabVer );
    }
    else
    {
        Exception e;

        e.setType( Exception::FileReadingError );
        e.setInfo( "ERROR! Reading XML file failed. " + XmlUtils::getErrorInfo( dataNode ) );

        FDM_THROW( e );
    }
}

////////////////////////////////////////////////////////////////////////////////

void C172_Aerodynamics::computeForceAndMoment()
{
    updateMatrices();

    m_tailOff->computeForceAndMoment( m_aircraft->getVel_air_BAS(),
                                      m_aircraft->getOmg_air_BAS(),
                                      m_aircraft->getEnvir()->getDensity(),
                                      m_aircraft->getCtrl()->getAilerons(),
                                      m_aircraft->getCtrl()->getFlaps() );

    m_stabHor->computeForceAndMoment( m_aircraft->getVel_air_BAS(),
                                      m_aircraft->getOmg_air_BAS(),
                                      m_aircraft->getEnvir()->getDensity(),
                                      m_aircraft->getAngleOfAttack(),
                                      m_aircraft->getCtrl()->getElevator(),
                                      m_aircraft->getCtrl()->getElevatorTrim() );

    m_stabVer->computeForceAndMoment( m_aircraft->getVel_air_BAS(),
                                      m_aircraft->getOmg_air_BAS(),
                                      m_aircraft->getEnvir()->getDensity(),
                                      m_aircraft->getCtrl()->getRudder() );

    m_for_bas = m_tailOff->getFor_BAS() + m_stabHor->getFor_BAS() + m_stabVer->getFor_BAS();
    m_mom_bas = m_tailOff->getMom_BAS() + m_stabHor->getMom_BAS() + m_stabVer->getMom_BAS();

    // computing forces expressed in Aerodynamic Axes System
    // computing moments expressed in Stability Axes System
    m_for_aero = m_bas2aero * m_for_bas;
    m_mom_stab = m_bas2stab * m_mom_bas;

    // wave drag
    m_for_aero.x() *= getPrandtlGlauertCoef( m_aircraft->getMachNumber() );

    // propwash effect
    m_mom_stab.x() += m_aircraft->getProp()->getPropeller()->getTorque() * m_dl_dtorque;
    m_mom_stab.z() += m_aircraft->getProp()->getPropeller()->getTorque() * m_dn_dtorque;

    // ground effect
    m_for_aero.x() *= m_drag_ground_effect.getValue( m_aircraft->getAltitude_AGL() );
    m_for_aero.z() *= m_lift_ground_effect.getValue( m_aircraft->getAltitude_AGL() );

    // computing forces and moments expressed in BAS
    m_for_bas = m_aero2bas * m_for_aero;
    m_mom_bas = m_stab2bas * m_mom_stab;

    if ( !m_for_bas.isValid() || !m_mom_bas.isValid() )
    {
        Exception e;

        e.setType( Exception::UnexpectedNaN );
        e.setInfo( "ERROR! NaN detected in the aerodynamics model." );

        FDM_THROW( e );
    }
}

////////////////////////////////////////////////////////////////////////////////

void C172_Aerodynamics::update()
{
    ///////////////////////
    Aerodynamics::update();
    ///////////////////////

    m_tailOff->update( m_aircraft->getVel_air_BAS(),  m_aircraft->getOmg_air_BAS() );
}
