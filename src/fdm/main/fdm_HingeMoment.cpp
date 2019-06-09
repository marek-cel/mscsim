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

#include <fdm/main/fdm_HingeMoment.h>

#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

HingeMoment::HingeMoment() :
    m_area  ( 0.0 ),
    m_chord ( 0.0 ),
    m_dch_dalpha   ( 0.0 ),
    m_dch_ddelta   ( 0.0 ),
    m_dch_ddelta_t ( 0.0 )
{}

////////////////////////////////////////////////////////////////////////////////

HingeMoment::~HingeMoment() {}

////////////////////////////////////////////////////////////////////////////////

void HingeMoment::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_area  , "area"  );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_chord , "chord" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_dch_dalpha   , "dch_dalpha"   );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_dch_ddelta   , "dch_ddelta"   );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_dch_ddelta_t , "dch_ddelta_t" );

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

double HingeMoment::getHingeMoment( double dynamicPressure,
                                    double alpha,
                                    double delta,
                                    double delta_t ) const
{
    // control surface hinge moment coefficient
    double ch = m_dch_dalpha   * alpha
              + m_dch_ddelta   * delta
              + m_dch_ddelta_t * delta_t;

    return dynamicPressure * m_area * m_chord * ch;
}
