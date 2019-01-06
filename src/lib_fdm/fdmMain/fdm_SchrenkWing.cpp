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

#include <fdmMain/fdm_SchrenkWing.h>

#include <fdmMain/fdm_Aerodynamics.h>
#include <fdmUtils/fdm_Units.h>
#include <fdmXml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

double SchrenkWing::getMeanAerodynamicChord( double cr, double ct )
{
    double tr = ct / cr; // taper ratio
    return ( 2.0 / 3.0 ) * cr * ( 1.0 + tr + tr*tr ) / ( 1.0 + tr );
}

////////////////////////////////////////////////////////////////////////////////

double SchrenkWing::getMeanAerodynamicChord( const Table &chord )
{
    double sum_mac = 0.0;
    double sum_area = 0.0;

    for ( unsigned int i = 1; i < chord.getSize(); i++ )
    {
        double cr = chord.getValueByIndex( i - 1 );
        double ct = chord.getValueByIndex( i );
        double dy = chord.getIndexValue( i ) - chord.getIndexValue( i -1 );

        double area = 0.5 * dy * ( cr + ct );

        sum_area += area;
        sum_mac += area * getMeanAerodynamicChord( cr, ct );
    }

    return sum_mac / sum_area;
}

////////////////////////////////////////////////////////////////////////////////

SchrenkWing::SchrenkWing() :
    m_span ( 0.0 ),
    m_4S_bpi ( 0.0 ),
    m_2_b ( 0.0 )
{}

////////////////////////////////////////////////////////////////////////////////

SchrenkWing::~SchrenkWing() {}

////////////////////////////////////////////////////////////////////////////////

void SchrenkWing::readData( XmlNode &dataNode )
{
    ///////////////////////////
    Wing::readData( dataNode );
    ///////////////////////////

    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_span, "span" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_chord, "chord" );

        m_4S_bpi = ( 4.0 * m_area ) / ( m_span * M_PI );
        m_2_b = 2.0 / m_span;

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

double SchrenkWing::getDragCoefDist( double y ) const
{
    //
    return ( y < 0.4 * m_span ) ? 0.95 : 1.2;
}

////////////////////////////////////////////////////////////////////////////////

double SchrenkWing::getLiftCoefDist( double y ) const
{
    // equivalent elliptical wing chord
    double chord_e = m_4S_bpi * sqrt( 1.0 - Misc::pow2( y * m_2_b ) );
    return 0.5 * ( 1.0 + chord_e / m_chord.getValue( y ) );
}
