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

#include <fdm/utils/fdm_Table.h>

#include <math.h>
#include <limits>
#include <sstream>

#include <fdm/fdm_Exception.h>
#include <fdm/utils/fdm_Misc.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Table Table::createOneRecordTable( double val )
{
    std::vector< double > keyValues;
    std::vector< double > tableData;

    keyValues.push_back( 0.0 );
    tableData.push_back( val );

    return Table( keyValues, tableData );
}

////////////////////////////////////////////////////////////////////////////////

Table::Table() :
    m_size ( 0 ),
    m_keyValues ( 0 ),
    m_tableData ( 0 ),
    m_interpolData ( 0 )
{}

////////////////////////////////////////////////////////////////////////////////

Table::Table( const std::vector< double > &keyValues,
              const std::vector< double > &tableData ) :
    m_size ( 0 ),
    m_keyValues ( 0 ),
    m_tableData ( 0 ),
    m_interpolData ( 0 )
{
    if ( keyValues.size() == tableData.size() )
    {
        m_size = keyValues.size();

        if ( m_size > 0 )
        {
            m_keyValues = new double [ m_size ];
            m_tableData = new double [ m_size ];

            m_interpolData = new double [ m_size ];

            for ( unsigned int i = 0; i < m_size; i++ )
            {
                m_keyValues[ i ] = keyValues[ i ];
                m_tableData[ i ] = tableData[ i ];

                m_interpolData[ i ] = 0.0;
            }

            updateInterpolationData();
        }
    }
    else
    {
        Exception e;

        e.setType( Exception::UnknownException );
        e.setInfo( "Invalid table size." );

        FDM_THROW( e );
    }
}

////////////////////////////////////////////////////////////////////////////////

Table::Table( const Table &table ) :
    m_size ( table.m_size ),
    m_keyValues ( 0 ),
    m_tableData ( 0 ),
    m_interpolData ( 0 )
{
    if ( m_size > 0 )
    {
        m_keyValues = new double [ m_size ];
        m_tableData = new double [ m_size ];
        m_interpolData = new double [ m_size ];

        for ( unsigned int i = 0; i < m_size; i++ )
        {
            m_keyValues    [ i ] = table.m_keyValues    [ i ];
            m_tableData    [ i ] = table.m_tableData    [ i ];
            m_interpolData [ i ] = table.m_interpolData [ i ];
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

Table::~Table()
{
    if ( m_keyValues ) delete m_keyValues;
    m_keyValues = 0;

    if ( m_tableData ) delete m_tableData;
    m_tableData = 0;

    if ( m_interpolData ) delete m_interpolData;
    m_interpolData = 0;
}

////////////////////////////////////////////////////////////////////////////////

double Table::getIndexValue( unsigned int keyIndex ) const
{
    if ( m_size > 0 && keyIndex < m_size )
    {
        return m_keyValues[ keyIndex ];
    }

    return std::numeric_limits< double >::quiet_NaN();
}

////////////////////////////////////////////////////////////////////////////////

double Table::getKeyOfValueMin() const
{
    double result = std::numeric_limits< double >::quiet_NaN();
    double min_value = std::numeric_limits< double >::max();

    for ( unsigned int i = 1; i < m_size; i++ )
    {
        if ( m_tableData[ i ] < min_value )
        {
            result = m_keyValues[ i ];
            min_value = m_tableData[ i ];
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

double Table::getKeyOfValueMax() const
{
    double result = std::numeric_limits< double >::quiet_NaN();
    double max_value = std::numeric_limits< double >::min();

    for ( unsigned int i = 1; i < m_size; i++ )
    {
        if ( m_tableData[ i ] > max_value )
        {
            result = m_keyValues[ i ];
            max_value = m_tableData[ i ];
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

double Table::getValue( double keyValue ) const
{
    if ( m_size > 0 )
    {
        if ( keyValue < m_keyValues[ 0 ] )
            return getValueFirst();

        if ( keyValue > m_keyValues[ m_size - 1 ] )
            return getValueLast();

        unsigned int key_1 = 0;
        unsigned int key_2 = 0;

        for ( unsigned int i = 1; i < m_size; i++ )
        {
            key_1 = i - 1;
            key_2 = i;

            if ( keyValue >= m_keyValues[ key_1 ]
              && keyValue <  m_keyValues[ key_2 ] )
            {
                break;
            }
        }

        return ( keyValue - m_keyValues[ key_1 ] ) * m_interpolData[ key_1 ]
                + m_tableData[ key_1 ];
    }
    else
    {
        Exception e;

        e.setType( Exception::UnknownException );
        e.setInfo( "Invalid size of table." );

        FDM_THROW( e );
    }

    return std::numeric_limits< double >::quiet_NaN();
}

////////////////////////////////////////////////////////////////////////////////

double Table::getValueByIndex( unsigned int keyIndex ) const
{
    if ( m_size > 0 && keyIndex < m_size )
    {
        return m_tableData[ keyIndex ];
    }

    return std::numeric_limits< double >::quiet_NaN();
}

////////////////////////////////////////////////////////////////////////////////

double Table::getFirstValue() const
{
    return getValueByIndex( 0 );
}

////////////////////////////////////////////////////////////////////////////////

double Table::getLastValue() const
{
    return getValueByIndex( m_size - 1 );
}

////////////////////////////////////////////////////////////////////////////////

double Table::getValueFirst() const
{
    if ( m_size > 0 )
    {
        return m_tableData[ 0 ];
    }
    else
    {
        Exception e;

        e.setType( Exception::UnknownException );
        e.setInfo( "Invalid size of table." );

        FDM_THROW( e );
    }

    return std::numeric_limits< double >::quiet_NaN();
}

////////////////////////////////////////////////////////////////////////////////

double Table::getValueLast() const
{
    if ( m_size > 0 )
    {
        return m_tableData[ m_size - 1 ];
    }
    else
    {
        Exception e;

        e.setType( Exception::UnknownException );
        e.setInfo( "Invalid size of table." );

        FDM_THROW( e );
    }

    return std::numeric_limits< double >::quiet_NaN();
}

////////////////////////////////////////////////////////////////////////////////

double Table::getValueMin() const
{
    double result = std::numeric_limits< double >::max();

    for ( unsigned int i = 0; i < m_size; i++ )
    {
        if ( m_tableData[ i ] < result )
        {
            result = m_tableData[ i ];
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

double Table::getValueMax() const
{
    double result = std::numeric_limits< double >::min();

    for ( unsigned int i = 0; i < m_size; i++ )
    {
        if ( m_tableData[ i ] > result )
        {
            result = m_tableData[ i ];
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

bool Table::isValid() const
{
    bool result = ( m_size > 0 ) ? true : false;

    for ( unsigned int i = 0; i < m_size; i++ )
    {
        if ( result ) result = Misc::isValid( m_keyValues[ i ] );
        if ( result ) result = Misc::isValid( m_tableData[ i ] );
        if ( result ) result = Misc::isValid( m_interpolData[ i ] );

        if ( i > 0 )
        {
            if ( result ) result = m_keyValues[ i - 1 ] < m_keyValues[ i ];
        }

        if ( !result ) break;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

std::string Table::toString()
{
    std::stringstream ss;

    for ( unsigned int i = 0; i < m_size; i++ )
    {
        ss << m_keyValues[ i ] << "\t" << m_tableData[ i ] << std::endl;
    }

    return ss.str();
}

////////////////////////////////////////////////////////////////////////////////

const Table& Table::operator= ( const Table &table )
{
    if ( m_keyValues ) delete m_keyValues;
    m_keyValues = 0;

    if ( m_tableData ) delete m_tableData;
    m_tableData = 0;

    if ( m_interpolData ) delete m_interpolData;
    m_interpolData = 0;

    m_size = table.m_size;

    if ( m_size > 0 )
    {
        m_keyValues = new double [ m_size ];
        m_tableData = new double [ m_size ];

        m_interpolData = new double [ m_size ];

        for ( unsigned int i = 0; i < m_size; i++ )
        {
            m_keyValues    [ i ] = table.m_keyValues    [ i ];
            m_tableData    [ i ] = table.m_tableData    [ i ];
            m_interpolData [ i ] = table.m_interpolData [ i ];
        }
    }

    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

Table Table::operator+ ( const Table &table ) const
{
    std::vector< double > keyValues;
    std::vector< double > tableData;

    for ( unsigned int i = 0; i < m_size; i++ )
    {
        double keyValue = m_keyValues[ i ];

        keyValues.push_back( keyValue );
        tableData.push_back( getValue( keyValue ) + table.getValue( keyValue ) );
    }

    return Table( keyValues, tableData );
}

////////////////////////////////////////////////////////////////////////////////

Table Table::operator* ( double val ) const
{
    std::vector< double > keyValues;
    std::vector< double > tableData;

    for ( unsigned int i = 0; i < m_size; i++ )
    {
        keyValues.push_back( m_keyValues[ i ] );
        tableData.push_back( m_tableData[ i ] * val );
    }

    return Table( keyValues, tableData );
}

////////////////////////////////////////////////////////////////////////////////

void Table::updateInterpolationData()
{
    for ( unsigned int i = 0; i < m_size - 1; i++ )
    {
        m_interpolData[ i ] = ( m_tableData[ i + 1 ] - m_tableData[ i ] )
                         / ( m_keyValues[ i + 1 ] - m_keyValues[ i ] );
    }
}
