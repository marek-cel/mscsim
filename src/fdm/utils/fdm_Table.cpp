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

#include <cmath>
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
    _size ( 0 ),
    _keyValues ( FDM_NULLPTR ),
    _tableData ( FDM_NULLPTR ),
    _interpolData ( FDM_NULLPTR )
{}

////////////////////////////////////////////////////////////////////////////////

Table::Table( const std::vector< double > &keyValues,
              const std::vector< double > &tableData ) :
    _size ( 0 ),
    _keyValues ( FDM_NULLPTR ),
    _tableData ( FDM_NULLPTR ),
    _interpolData ( FDM_NULLPTR )
{
    if ( keyValues.size() == tableData.size() )
    {
        _size = keyValues.size();

        if ( _size > 0 )
        {
            _keyValues = new double [ _size ];
            _tableData = new double [ _size ];

            _interpolData = new double [ _size ];

            for ( unsigned int i = 0; i < _size; i++ )
            {
                _keyValues[ i ] = keyValues[ i ];
                _tableData[ i ] = tableData[ i ];

                _interpolData[ i ] = 0.0;
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
    _size ( table._size ),
    _keyValues ( FDM_NULLPTR ),
    _tableData ( FDM_NULLPTR ),
    _interpolData ( FDM_NULLPTR )
{
    if ( _size > 0 )
    {
        _keyValues = new double [ _size ];
        _tableData = new double [ _size ];
        _interpolData = new double [ _size ];

        for ( unsigned int i = 0; i < _size; i++ )
        {
            _keyValues    [ i ] = table._keyValues    [ i ];
            _tableData    [ i ] = table._tableData    [ i ];
            _interpolData [ i ] = table._interpolData [ i ];
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

Table::~Table()
{
    FDM_DELTAB( _keyValues );
    FDM_DELTAB( _tableData );
    FDM_DELTAB( _interpolData );
}

////////////////////////////////////////////////////////////////////////////////

double Table::getIndexValue( unsigned int keyIndex ) const
{
    if ( _size > 0 && keyIndex < _size )
    {
        return _keyValues[ keyIndex ];
    }

    return std::numeric_limits< double >::quiet_NaN();
}

////////////////////////////////////////////////////////////////////////////////

double Table::getKeyOfValueMin() const
{
    double result = std::numeric_limits< double >::quiet_NaN();
    double min_value = std::numeric_limits< double >::max();

    for ( unsigned int i = 1; i < _size; i++ )
    {
        if ( _tableData[ i ] < min_value )
        {
            result = _keyValues[ i ];
            min_value = _tableData[ i ];
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

double Table::getKeyOfValueMax() const
{
    double result = std::numeric_limits< double >::quiet_NaN();
    double max_value = std::numeric_limits< double >::min();

    for ( unsigned int i = 1; i < _size; i++ )
    {
        if ( _tableData[ i ] > max_value )
        {
            result = _keyValues[ i ];
            max_value = _tableData[ i ];
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

double Table::getValue( double keyValue ) const
{
    if ( _size > 0 )
    {
        if ( keyValue < _keyValues[ 0 ] )
            return getValueFirst();

        if ( keyValue > _keyValues[ _size - 1 ] )
            return getValueLast();

        unsigned int key_1 = 0;
        unsigned int key_2 = 0;

        for ( unsigned int i = 1; i < _size; i++ )
        {
            key_1 = i - 1;
            key_2 = i;

            if ( keyValue >= _keyValues[ key_1 ]
              && keyValue <  _keyValues[ key_2 ] )
            {
                break;
            }
        }

        return ( keyValue - _keyValues[ key_1 ] ) * _interpolData[ key_1 ]
                + _tableData[ key_1 ];
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
    if ( _size > 0 && keyIndex < _size )
    {
        return _tableData[ keyIndex ];
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
    return getValueByIndex( _size - 1 );
}

////////////////////////////////////////////////////////////////////////////////

double Table::getValueFirst() const
{
    if ( _size > 0 )
    {
        return _tableData[ 0 ];
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
    if ( _size > 0 )
    {
        return _tableData[ _size - 1 ];
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

    for ( unsigned int i = 0; i < _size; i++ )
    {
        if ( _tableData[ i ] < result )
        {
            result = _tableData[ i ];
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

double Table::getValueMax() const
{
    double result = std::numeric_limits< double >::min();

    for ( unsigned int i = 0; i < _size; i++ )
    {
        if ( _tableData[ i ] > result )
        {
            result = _tableData[ i ];
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

bool Table::isValid() const
{
    bool result = ( _size > 0 ) ? true : false;

    for ( unsigned int i = 0; i < _size; i++ )
    {
        if ( result ) result = Misc::isValid( _keyValues[ i ] );
        if ( result ) result = Misc::isValid( _tableData[ i ] );
        if ( result ) result = Misc::isValid( _interpolData[ i ] );

        if ( i > 0 )
        {
            if ( result ) result = _keyValues[ i - 1 ] < _keyValues[ i ];
        }

        if ( !result ) break;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

std::string Table::toString()
{
    std::stringstream ss;

    for ( unsigned int i = 0; i < _size; i++ )
    {
        ss << _keyValues[ i ] << "\t" << _tableData[ i ] << std::endl;
    }

    return ss.str();
}

////////////////////////////////////////////////////////////////////////////////

const Table& Table::operator= ( const Table &table )
{
    FDM_DELTAB( _keyValues );
    FDM_DELTAB( _tableData );
    FDM_DELTAB( _interpolData );

    _size = table._size;

    if ( _size > 0 )
    {
        _keyValues = new double [ _size ];
        _tableData = new double [ _size ];

        _interpolData = new double [ _size ];

        for ( unsigned int i = 0; i < _size; i++ )
        {
            _keyValues    [ i ] = table._keyValues    [ i ];
            _tableData    [ i ] = table._tableData    [ i ];
            _interpolData [ i ] = table._interpolData [ i ];
        }
    }

    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

Table Table::operator+ ( const Table &table ) const
{
    std::vector< double > keyValues;
    std::vector< double > tableData;

    for ( unsigned int i = 0; i < _size; i++ )
    {
        double keyValue = _keyValues[ i ];

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

    for ( unsigned int i = 0; i < _size; i++ )
    {
        keyValues.push_back( _keyValues[ i ] );
        tableData.push_back( _tableData[ i ] * val );
    }

    return Table( keyValues, tableData );
}

////////////////////////////////////////////////////////////////////////////////

void Table::updateInterpolationData()
{
    for ( unsigned int i = 0; i < _size - 1; i++ )
    {
        _interpolData[ i ] = ( _tableData[ i + 1 ] - _tableData[ i ] )
                           / ( _keyValues[ i + 1 ] - _keyValues[ i ] );
    }
}
