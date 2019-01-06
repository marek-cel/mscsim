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

#include <fdm_c172/c172_StabilizerVer.h>

#include <fdmMain/fdm_Aerodynamics.h>
#include <fdmUtils/fdm_Units.h>
#include <fdmXml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

C172_StabilizerVer::C172_StabilizerVer() :
    m_dcx_drudder ( 0.0 ),
    m_dcy_drudder ( 0.0 )
{}

////////////////////////////////////////////////////////////////////////////////

C172_StabilizerVer::~C172_StabilizerVer() {}

////////////////////////////////////////////////////////////////////////////////

void C172_StabilizerVer::readData( XmlNode &dataNode )
{
    ////////////////////////////////////
    StabilizerVer::readData( dataNode );
    ////////////////////////////////////

    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_dcx_drudder, "dcx_drudder" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_dcy_drudder, "dcy_drudder" );

        if ( result != FDM_SUCCESS )
        {
            Exception e;

            e.setType( Exception::FileReadingError );
            e.setInfo( "Error reading XML file. " + XmlUtils::getErrorInfo( dataNode ) );

            FDM_THROW( e );
        }
    }
    else
    {
        Exception e;

        e.setType( Exception::FileReadingError );
        e.setInfo( "Error reading XML file. " + XmlUtils::getErrorInfo( dataNode ) );

        FDM_THROW( e );
    }
}

////////////////////////////////////////////////////////////////////////////////

void C172_StabilizerVer::computeForceAndMoment( const Vector3 &vel_air_bas,
                                                const Vector3 &omg_air_bas,
                                                double airDensity,
                                                double rudder )
{
    // stabilizer velocity
    Vector3 vel_stab_bas = vel_air_bas + ( omg_air_bas ^ m_pos_bas );

    // stabilizer angle of attack and sideslip angle
    double angleOfAttackStab = Aerodynamics::getAngleOfAttack( vel_stab_bas );
    double sideslipAngleStab = Aerodynamics::getSideslipAngle( vel_stab_bas );

    // dynamic pressure
    double dynPress = 0.5 * airDensity * vel_stab_bas.getLength2();

    double cx = m_cx.getValue( sideslipAngleStab ) + m_dcx_drudder * rudder;
    double cy = m_cy.getValue( sideslipAngleStab ) + m_dcy_drudder * rudder;

    Vector3 for_aero( dynPress * cx * m_area,
                      dynPress * cy * m_area,
                      0.0 );

    m_for_bas = Aerodynamics::getRotMat_aero2BAS( angleOfAttackStab, sideslipAngleStab ) * for_aero;
    m_mom_bas = m_pos_bas ^ m_for_bas;
}
