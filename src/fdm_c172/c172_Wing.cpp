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

#include <fdm_c172/c172_Wing.h>

#include <fdmMain/fdm_Aerodynamics.h>
#include <fdmXml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

C172_Wing::C172_Wing() :
    m_ailerons ( 0.0 ),
    m_flaps    ( 0.0 ),

    m_dcl_dailerons ( 0.0 ),
    m_dcx_dflaps ( 0.0 ),
    m_dcz_dflaps ( 0.0 ),
    m_dcm_dflaps ( 0.0 )
{}

////////////////////////////////////////////////////////////////////////////////

C172_Wing::~C172_Wing() {}

////////////////////////////////////////////////////////////////////////////////

void C172_Wing::readData( XmlNode &dataNode )
{
    ///////////////////////////
    Wing::readData( dataNode );
    ///////////////////////////

    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_dcl_dailerons, "dcl_dailerons" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_dcx_dflaps, "dcx_dflaps"  );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_dcz_dflaps, "dcz_dflaps"  );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_dcm_dflaps, "dcm_dflaps" );

        if ( result != FDM_SUCCESS )
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

void C172_Wing::computeForceAndMoment( const Vector3 &vel_air_bas,
                                       const Vector3 &omg_air_bas,
                                       double airDensity,
                                       double ailerons,
                                       double flaps )
{
    m_ailerons = ailerons;
    m_flaps    = flaps;

    ////////////////////////////////////////////////////////////////////
    Wing::computeForceAndMoment( vel_air_bas, omg_air_bas, airDensity );
    ////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////

void C172_Wing::update( const Vector3 &vel_air_bas, const Vector3 &omg_air_bas )
{
    /////////////////////////////////////////
    Wing::update( vel_air_bas, omg_air_bas );
    /////////////////////////////////////////

    //m_ailerons->getHingeMoment( dynPress, alpha, m_delta_a );
}

////////////////////////////////////////////////////////////////////////////////

double C172_Wing::getCx( double angleOfAttack ) const
{
    return Wing::getCx( angleOfAttack ) + m_flaps * m_dcx_dflaps;
}

////////////////////////////////////////////////////////////////////////////////

double C172_Wing::getCy( double sideslipAngle ) const
{
    return Wing::getCy( sideslipAngle );
}

////////////////////////////////////////////////////////////////////////////////

double C172_Wing::getCz( double angleOfAttack ) const
{
    return Wing::getCz( angleOfAttack ) + m_flaps * m_dcz_dflaps;
}

////////////////////////////////////////////////////////////////////////////////

double C172_Wing::getCl( double sideslipAngle ) const
{
    return Wing::getCl( sideslipAngle ) + m_ailerons * m_dcl_dailerons;
}

////////////////////////////////////////////////////////////////////////////////

double C172_Wing::getCm( double angleOfAttack ) const
{
    return Wing::getCm( angleOfAttack ) + m_flaps * m_dcm_dflaps;
}

////////////////////////////////////////////////////////////////////////////////

double C172_Wing::getCn( double sideslipAngle ) const
{
    return Wing::getCn( sideslipAngle );
}
