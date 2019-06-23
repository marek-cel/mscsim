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

#include <fdm_uh60/uh60_Aerodynamics.h>
#include <fdm_uh60/uh60_Aircraft.h>

#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

UH60_Aerodynamics::UH60_Aerodynamics( const UH60_Aircraft *aircraft ) :
    Aerodynamics( aircraft ),
    m_aircraft ( aircraft ),

    m_mainRotor ( 0 ),
    m_tailRotor ( 0 ),
    m_fuselage  ( 0 ),
    m_stabHor   ( 0 ),
    m_stabVer   ( 0 )
{
    m_mainRotor = new MainRotor();
    m_tailRotor = new TailRotor();
    m_fuselage  = new UH60_Fuselage();
    m_stabHor   = new UH60_StabilizerHor();
    m_stabVer   = new Stabilizer();
}

////////////////////////////////////////////////////////////////////////////////

UH60_Aerodynamics::~UH60_Aerodynamics()
{
    if ( m_mainRotor ) delete m_mainRotor;
    m_mainRotor = 0;

    if ( m_tailRotor ) delete m_tailRotor;
    m_tailRotor = 0;

    if ( m_fuselage ) delete m_fuselage;
    m_fuselage = 0;

    if ( m_stabHor ) delete m_stabHor;
    m_stabHor = 0;

    if ( m_stabVer ) delete m_stabVer;
    m_stabVer = 0;
}

////////////////////////////////////////////////////////////////////////////////

void UH60_Aerodynamics::init()
{
    /////////////////////
    Aerodynamics::init();
    /////////////////////
}

////////////////////////////////////////////////////////////////////////////////

void UH60_Aerodynamics::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        XmlNode nodeMainRotor = dataNode.getFirstChildElement( "main_rotor" );
        XmlNode nodeTailRotor = dataNode.getFirstChildElement( "tail_rotor" );
        XmlNode nodeFuselage  = dataNode.getFirstChildElement( "fuselage" );
        XmlNode nodeStabHor   = dataNode.getFirstChildElement( "stab_hor" );
        XmlNode nodeStabVer   = dataNode.getFirstChildElement( "stab_ver" );

        m_mainRotor->readData( nodeMainRotor );
        m_tailRotor->readData( nodeTailRotor );
        m_fuselage->readData( nodeFuselage );
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

void UH60_Aerodynamics::computeForceAndMoment()
{
    updateMatrices();

    m_mainRotor->computeForceAndMoment( m_aircraft->getVel_BAS(),
                                        m_aircraft->getOmg_BAS(),
                                        m_aircraft->getAcc_BAS(),
                                        m_aircraft->getEps_BAS(),
                                        m_aircraft->getGrav_BAS(),
                                        m_aircraft->getVel_air_BAS(),
                                        m_aircraft->getOmg_air_BAS(),
                                        m_aircraft->getEnvir()->getDensity(),
                                        m_aircraft->getCtrl()->getCollective(),
                                        m_aircraft->getCtrl()->getCyclicLat(),
                                        m_aircraft->getCtrl()->getCyclicLon() );

    m_tailRotor->computeForceAndMoment( m_aircraft->getVel_air_BAS() - m_mainRotor->getVel_i_BAS(),
                                        m_aircraft->getOmg_air_BAS(),
                                        m_aircraft->getEnvir()->getDensity(),
                                        m_aircraft->getCtrl()->getTailPitch() );

    m_fuselage->computeForceAndMoment( m_aircraft->getVel_air_BAS() - m_mainRotor->getVel_i_BAS(),
                                       m_aircraft->getOmg_air_BAS(),
                                       m_aircraft->getEnvir()->getDensity() );

    m_stabHor->computeForceAndMoment( m_aircraft->getVel_air_BAS() - m_mainRotor->getVel_i_BAS(),
                                      m_aircraft->getOmg_air_BAS(),
                                      m_aircraft->getEnvir()->getDensity(),
                                      m_aircraft->getCtrl()->getElevator() );

    m_stabVer->computeForceAndMoment( m_aircraft->getVel_air_BAS() - m_tailRotor->getVel_i_BAS(),
                                      m_aircraft->getOmg_air_BAS(),
                                      m_aircraft->getEnvir()->getDensity() );

    m_for_bas = m_mainRotor->getFor_BAS() + m_tailRotor->getFor_BAS()
              + m_fuselage->getFor_BAS()
              + m_stabHor->getFor_BAS() + m_stabVer->getFor_BAS();

    m_mom_bas = m_mainRotor->getMom_BAS() + m_tailRotor->getMom_BAS()
              + m_fuselage->getMom_BAS()
              + m_stabHor->getMom_BAS() + m_stabVer->getMom_BAS();

//            // damping
//            double R_2v = 0.0;

//            if ( m_aircraft->getAirspeed() > 0.1 )
//            {
//                R_2v = m_mainRotor->getRadius() / ( 2.0 * m_aircraft->getAirspeed() );
//            }

//            Vector3 omg_air_stab = m_bas2stab * m_aircraft->getOmg_air_BAS();

//            double cl = -0.240 * omg_air_stab.x() * R_2v;
//            double cm = -0.120 * omg_air_stab.y() * R_2v;
//            double cn = -0.120 * omg_air_stab.z() * R_2v;

//            // dynamic pressure
//            double dynPress = m_aircraft->getDynPress();

//            double aR = m_mainRotor->getDiskArea() * m_mainRotor->getRadius();

//            Vector3 mom_aero( dynPress * aR * cl,
//                              dynPress * aR * cm,
//                              dynPress * aR * cn );

//            m_mom_bas += m_stab2bas * mom_aero;

    // computing forces expressed in Aerodynamic Axes System
    // computing moments expressed in Stability Axes System
    m_for_aero = m_bas2aero * m_for_bas;
    m_mom_stab = m_bas2stab * m_mom_bas;

    if ( !m_for_bas.isValid() || !m_mom_bas.isValid() )
    {
        Exception e;

        e.setType( Exception::UnexpectedNaN );
        e.setInfo( "ERROR! NaN detected in the aerodynamics model." );

        FDM_THROW( e );
    }
}

////////////////////////////////////////////////////////////////////////////////

void UH60_Aerodynamics::update()
{
    ///////////////////////
    Aerodynamics::update();
    ///////////////////////

    m_mainRotor->update( m_aircraft->getProp()->getMainRotorOmega() );
    m_tailRotor->update( m_aircraft->getProp()->getTailRotorOmega() );
}
