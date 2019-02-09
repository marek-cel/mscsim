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

#include <fdm_f16c/f16c_Aircraft.h>

#include <fdmUtils/fdm_Units.h>
#include <fdmXml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

F16C_Aerodynamics::F16C_Aerodynamics( const F16C_Aircraft *aircraft ) :
    Aerodynamics( aircraft ),
    m_aircraft ( aircraft ),

    m_span ( 0.0 ),
    m_mac  ( 0.0 ),
    m_area ( 0.0 ),

    m_mac_s ( 0.0 ),

    m_b_2v ( 0.0 ),
    m_c_2v ( 0.0 )
{}

////////////////////////////////////////////////////////////////////////////////

F16C_Aerodynamics::~F16C_Aerodynamics() {}

////////////////////////////////////////////////////////////////////////////////

void F16C_Aerodynamics::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_span , "span" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_mac  , "mac"  );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_area , "area" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_cx, "cx" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_cy, "cy" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_cz, "cz" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_cl, "cl" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_cl_p, "cl_p" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_cl_delta_a, "cl_delta_a" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_cm, "cm" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_cm_q, "cm_q" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_cm_delta_h, "cm_delta_h" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_cn, "cn" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_cn_r, "cn_r" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_cn_delta_r, "cn_delta_r" );

        if ( result == FDM_SUCCESS )
        {
            m_mac_s = m_area * m_mac;
        }
        else
        {
            Exception e;

            e.setType( Exception::FileReadingError );
            e.setInfo( "ERROR! Reading XML file failed. " + XmlUtils::getErrorInfo( dataNode ) );

            FDM_THROW( e );
        }
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

void F16C_Aerodynamics::computeForceAndMoment()
{
    updateMatrices();

    // dynamic pressure
    double dynPress = m_aircraft->getDynPress();

    m_b_2v = 0.0;
    m_c_2v = 0.0;

    if ( m_aircraft->getAirspeed() > 0.1 )
    {
        m_b_2v = m_span / ( 2.0 * m_aircraft->getAirspeed() );
        m_c_2v = m_mac  / ( 2.0 * m_aircraft->getAirspeed() );
    }

    m_for_bas = m_stab2bas * Vector3( dynPress * getCx() * m_area,
                                      dynPress * getCy() * m_area,
                                      dynPress * getCz() * m_area );

    m_mom_bas = Vector3( dynPress * getCl() * m_mac_s,
                         dynPress * getCm() * m_mac_s,
                         dynPress * getCn() * m_mac_s );

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

void F16C_Aerodynamics::update()
{
    ///////////////////////
    Aerodynamics::update();
    ///////////////////////
}

////////////////////////////////////////////////////////////////////////////////

double F16C_Aerodynamics::getCx() const
{
    return m_cx.getValue( m_aircraft->getAngleOfAttack() );
}

////////////////////////////////////////////////////////////////////////////////

double F16C_Aerodynamics::getCy() const
{
    return m_cy.getValue( m_aircraft->getSideslipAngle() );
}

////////////////////////////////////////////////////////////////////////////////

double F16C_Aerodynamics::getCz() const
{
    return m_cz.getValue( m_aircraft->getAngleOfAttack() );
}

////////////////////////////////////////////////////////////////////////////////

double F16C_Aerodynamics::getCl() const
{
    return m_cl.getValue( m_aircraft->getSideslipAngle() )
        + m_aircraft->getOmg_air_BAS()( i_p ) * m_b_2v * m_cl_p.getValue( m_aircraft->getAngleOfAttack() ) // sic!
        + m_aircraft->getCtrl()->getAilerons() * m_cl_delta_a.getValue( m_aircraft->getAngleOfAttack() ); // sic!
}

////////////////////////////////////////////////////////////////////////////////

double F16C_Aerodynamics::getCm() const
{
    return m_cm.getValue( m_aircraft->getAngleOfAttack() )
        + m_aircraft->getOmg_air_BAS()( i_q ) * m_c_2v * m_cm_q.getValue( m_aircraft->getAngleOfAttack() )
        + m_aircraft->getCtrl()->getElevator() * m_cm_delta_h.getValue( m_aircraft->getAngleOfAttack() );
}

////////////////////////////////////////////////////////////////////////////////

double F16C_Aerodynamics::getCn() const
{
    return m_cn.getValue( m_aircraft->getSideslipAngle() )
        + m_aircraft->getOmg_air_BAS()( i_r ) * m_b_2v * m_cn_r.getValue( m_aircraft->getAngleOfAttack() ) // sic!
        + m_aircraft->getCtrl()->getRudder() * m_cn_delta_r.getValue( m_aircraft->getSideslipAngle() );
}
