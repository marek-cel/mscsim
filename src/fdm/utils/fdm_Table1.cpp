/****************************************************************************//*
 * Copyright (C) 2020 Marek M. Cel
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

#include <fdm/utils/fdm_Table1.h>

#include <cmath>
#include <limits>
#include <sstream>

#include <fdm/fdm_Exception.h>
#include <fdm/utils/fdm_Misc.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Table1 Table1::oneRecordTable( double val )
{
    std::vector< double > keyValues;
    std::vector< double > tableData;

    keyValues.push_back( 0.0 );
    tableData.push_back( val );

    return Table1( keyValues, tableData );
}

////////////////////////////////////////////////////////////////////////////////

Table1::Table1() :
    _size ( 0 ),
    _key_values ( FDM_NULLPTR ),
    _table_data ( FDM_NULLPTR ),
    _inter_data ( FDM_NULLPTR )
{}

////////////////////////////////////////////////////////////////////////////////

Table1::Table1( const std::vector< double > &key_values,
                const std::vector< double > &table_data ) :
    _size ( 0 ),
    _key_values ( FDM_NULLPTR ),
    _table_data ( FDM_NULLPTR ),
    _inter_data ( FDM_NULLPTR )
{
    if ( key_values.size() == table_data.size() )
    {
        _size = key_values.size();

        if ( _size > 0 )
        {
            _key_values = new double [ _size ];
            _table_data = new double [ _size ];
            _inter_data = new double [ _size ];

            for ( unsigned int i = 0; i < _size; i++ )
            {
                _key_values[ i ] = key_values[ i ];
                _table_data[ i ] = table_data[ i ];
                _inter_data[ i ] = 0.0;
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

Table1::Table1( const Table1 &table ) :
    _size ( table._size ),
    _key_values ( FDM_NULLPTR ),
    _table_data ( FDM_NULLPTR ),
    _inter_data ( FDM_NULLPTR )
{
    if ( _size > 0 )
    {
        _key_values = new double [ _size ];
        _table_data = new double [ _size ];
        _inter_data = new double [ _size ];

        for ( unsigned int i = 0; i < _size; i++ )
        {
            _key_values[ i ] = table._key_values[ i ];
            _table_data[ i ] = table._table_data[ i ];
            _inter_data[ i ] = table._inter_data[ i ];
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

Table1::~Table1()
{
    FDM_DELTAB( _key_values );
    FDM_DELTAB( _table_data );
    FDM_DELTAB( _inter_data );
}

////////////////////////////////////////////////////////////////////////////////

double Table1::getIndexValue( unsigned int key_index ) const
{
    if ( _size > 0 && key_index < _size )
    {
        return _key_values[ key_index ];
    }

    return std::numeric_limits< double >::quiet_NaN();
}

////////////////////////////////////////////////////////////////////////////////

double Table1::getKeyOfValueMin() const
{
    double result = std::numeric_limits< double >::quiet_NaN();
    double min_value = std::numeric_limits< double >::max();

    for ( unsigned int i = 1; i < _size; i++ )
    {
        if ( _table_data[ i ] < min_value )
        {
            result = _key_values[ i ];
            min_value = _table_data[ i ];
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

double Table1::getKeyOfValueMin( double key_min, double key_max ) const
{
    double result = std::numeric_limits< double >::quiet_NaN();
    double min_value = std::numeric_limits< double >::max();

    for ( unsigned int i = 1; i < _size; i++ )
    {
        if ( _table_data[ i ] < min_value )
        {
            if ( _key_values[ i ] <= key_max )
            {
                if ( _key_values[ i ] >= key_min )
                {
                    result = _key_values[ i ];
                    min_value = _table_data[ i ];
                }
            }
            else
            {
                break;
            }
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

double Table1::getKeyOfValueMax() const
{
    double result = std::numeric_limits< double >::quiet_NaN();
    double max_value = std::numeric_limits< double >::min();

    for ( unsigned int i = 1; i < _size; i++ )
    {
        if ( _table_data[ i ] > max_value )
        {
            result = _key_values[ i ];
            max_value = _table_data[ i ];
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

double Table1::getKeyOfValueMax( double key_min, double key_max ) const
{
    double result = std::numeric_limits< double >::quiet_NaN();
    double min_value = std::numeric_limits< double >::min();

    for ( unsigned int i = 1; i < _size; i++ )
    {
        if ( _table_data[ i ] > min_value )
        {
            if ( _key_values[ i ] <= key_max )
            {
                if ( _key_values[ i ] >= key_min )
                {
                    result = _key_values[ i ];
                    min_value = _table_data[ i ];
                }
            }
            else
            {
                break;
            }
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

double Table1::getValue( double key_value ) const
{
    if ( _size > 0 )
    {
        if ( key_value < _key_values[ 0 ] )
            return getFirstValue();

        if ( key_value > _key_values[ _size - 1 ] )
            return getLastValue();

        unsigned int key_1 = 0;
        unsigned int key_2 = 0;

        for ( unsigned int i = 1; i < _size; i++ )
        {
            key_1 = i - 1;
            key_2 = i;

            if ( key_value >= _key_values[ key_1 ]
              && key_value <  _key_values[ key_2 ] )
            {
                break;
            }
        }

        return ( key_value - _key_values[ key_1 ] ) * _inter_data[ key_1 ]
                + _table_data[ key_1 ];
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

double Table1::getValueByIndex( unsigned int key_index ) const
{
    if ( _size > 0 && key_index < _size )
    {
        return _table_data[ key_index ];
    }

    return std::numeric_limits< double >::quiet_NaN();
}

////////////////////////////////////////////////////////////////////////////////

double Table1::getFirstValue() const
{
    return getValueByIndex( 0 );
}

////////////////////////////////////////////////////////////////////////////////

double Table1::getLastValue() const
{
    return getValueByIndex( _size - 1 );
}

////////////////////////////////////////////////////////////////////////////////

double Table1::getValueMin() const
{
    double result = std::numeric_limits< double >::max();

    for ( unsigned int i = 0; i < _size; i++ )
    {
        if ( _table_data[ i ] < result )
        {
            result = _table_data[ i ];
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

double Table1::getValueMax() const
{
    double result = std::numeric_limits< double >::min();

    for ( unsigned int i = 0; i < _size; i++ )
    {
        if ( _table_data[ i ] > result )
        {
            result = _table_data[ i ];
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

bool Table1::isValid() const
{
    bool result = ( _size > 0 ) ? true : false;

    for ( unsigned int i = 0; i < _size; i++ )
    {
        if ( result ) result = Misc::isValid( _key_values[ i ] );
        if ( result ) result = Misc::isValid( _table_data[ i ] );
        if ( result ) result = Misc::isValid( _inter_data[ i ] );

        if ( i > 0 )
        {
            if ( result ) result = _key_values[ i - 1 ] < _key_values[ i ];
        }

        if ( !result ) break;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

void Table1::multiplyKeys( double factor )
{
    for ( unsigned int i = 0; i < _size; i++ )
    {
        _key_values[ i ] *= factor;
    }

    updateInterpolationData();
}

////////////////////////////////////////////////////////////////////////////////

void Table1::multiplyValues( double factor )
{
    for ( unsigned int i = 0; i < _size; i++ )
    {
        _table_data[ i ] *= factor;
    }

    updateInterpolationData();
}

////////////////////////////////////////////////////////////////////////////////

std::string Table1::toString()
{
    std::stringstream ss;

    for ( unsigned int i = 0; i < _size; i++ )
    {
        ss << _key_values[ i ] << "\t" << _table_data[ i ] << std::endl;
    }

    return ss.str();
}

////////////////////////////////////////////////////////////////////////////////

const Table1& Table1::operator= ( const Table1 &table )
{
    FDM_DELTAB( _key_values );
    FDM_DELTAB( _table_data );
    FDM_DELTAB( _inter_data );

    _size = table._size;

    if ( _size > 0 )
    {
        _key_values = new double [ _size ];
        _table_data = new double [ _size ];

        _inter_data = new double [ _size ];

        for ( unsigned int i = 0; i < _size; i++ )
        {
            _key_values[ i ] = table._key_values [ i ];
            _table_data[ i ] = table._table_data [ i ];
            _inter_data[ i ] = table._inter_data [ i ];
        }
    }

    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

Table1 Table1::operator+ ( const Table1 &table ) const
{
    std::vector< double > keyValues;
    std::vector< double > tableData;

    for ( unsigned int i = 0; i < _size; i++ )
    {
        double keyValue = _key_values[ i ];

        keyValues.push_back( keyValue );
        tableData.push_back( getValue( keyValue ) + table.getValue( keyValue ) );
    }

    return Table1( keyValues, tableData );
}

////////////////////////////////////////////////////////////////////////////////

Table1 Table1::operator* ( double val ) const
{
    std::vector< double > keyValues;
    std::vector< double > tableData;

    for ( unsigned int i = 0; i < _size; i++ )
    {
        keyValues.push_back( _key_values[ i ] );
        tableData.push_back( _table_data[ i ] * val );
    }

    return Table1( keyValues, tableData );
}

////////////////////////////////////////////////////////////////////////////////

void Table1::updateInterpolationData()
{
    for ( unsigned int i = 0; i < _size - 1; i++ )
    {
        _inter_data[ i ] = ( _table_data[ i + 1 ] - _table_data[ i ] )
                         / ( _key_values[ i + 1 ] - _key_values[ i ] );
    }
}
