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

#include <fdm/utils/fdm_Table2.h>

#include <cmath>
#include <limits>
#include <sstream>

#include <fdm/fdm_Exception.h>
#include <fdm/utils/fdm_Misc.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Table2 Table2::oneRecordTable( double val )
{
    std::vector< double > rowValues;
    std::vector< double > colValues;
    std::vector< double > tableData;

    rowValues.push_back( 0.0 );
    colValues.push_back( 0.0 );
    tableData.push_back( val );

    return Table2( rowValues, colValues, tableData );
}

////////////////////////////////////////////////////////////////////////////////

Table2::Table2() :
    _rows ( 0 ),
    _cols ( 0 ),
    _size ( 0 ),
    _row_values ( FDM_NULLPTR ),
    _col_values ( FDM_NULLPTR ),
    _table_data ( FDM_NULLPTR ),
    _inter_data ( FDM_NULLPTR )
{}

////////////////////////////////////////////////////////////////////////////////

Table2::Table2( const std::vector< double > &row_values,
                const std::vector< double > &col_values,
                const std::vector< double > &table_data ) :
    _rows ( 0 ),
    _cols ( 0 ),
    _size ( 0 ),
    _row_values ( FDM_NULLPTR ),
    _col_values ( FDM_NULLPTR ),
    _table_data ( FDM_NULLPTR ),
    _inter_data ( FDM_NULLPTR )
{
    if ( row_values.size() * col_values.size() == table_data.size() )
    {
        _size = table_data.size();

        if ( _size > 0 )
        {
            _rows = row_values.size();
            _cols = col_values.size();

            _row_values = new double [ _rows ];
            _col_values = new double [ _cols ];
            _table_data = new double [ _size ];

            _inter_data = new double [ _size ];

            for ( unsigned int i = 0; i < _rows; i++ ) _row_values[ i ] = row_values[ i ];
            for ( unsigned int i = 0; i < _cols; i++ ) _col_values[ i ] = col_values[ i ];

            for ( unsigned int i = 0; i < _size; i++ )
            {
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

Table2::Table2( const Table2 &table ) :
    _rows ( table._rows ),
    _cols ( table._cols ),
    _size ( table._size ),
    _row_values ( FDM_NULLPTR ),
    _col_values ( FDM_NULLPTR ),
    _table_data ( FDM_NULLPTR ),
    _inter_data ( FDM_NULLPTR )
{
    if ( _size > 0 )
    {
        _row_values = new double [ _rows ];
        _col_values = new double [ _cols ];
        _table_data = new double [ _size ];

        _inter_data = new double [ _size ];

        for ( unsigned int i = 0; i < _rows; i++ ) _row_values[ i ] = table._row_values[ i ];
        for ( unsigned int i = 0; i < _cols; i++ ) _col_values[ i ] = table._col_values[ i ];

        for ( unsigned int i = 0; i < _size; i++ )
        {
            _table_data[ i ] = table._table_data[ i ];
            _inter_data[ i ] = table._inter_data[ i ] ;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

Table2::~Table2()
{
    FDM_DELTAB( _row_values );
    FDM_DELTAB( _col_values );
    FDM_DELTAB( _table_data );
    FDM_DELTAB( _inter_data );
}

////////////////////////////////////////////////////////////////////////////////

Table1 Table2::getTable( double col_value ) const
{
    std::vector< double > keyValues;
    std::vector< double > tableData;

    for ( unsigned int i = 0; i < _rows; i++ )
    {
        keyValues.push_back( _row_values[ i ] );
        tableData.push_back( getValue( _row_values[ i ], col_value ) );
    }

    return Table1( keyValues, tableData );
}

////////////////////////////////////////////////////////////////////////////////

double Table2::getValue( double row_value, double col_value ) const
{
    if ( _size > 0 )
    {
        if ( row_value < _row_values[ 0 ] ) return getValue( _row_values[ 0 ], col_value );
        if ( col_value < _col_values[ 0 ] ) return getValue( row_value, _col_values[ 0 ] );

        if ( row_value > _row_values[ _rows - 1 ] ) return getValue( _row_values[ _rows - 1 ], col_value );
        if ( col_value > _col_values[ _cols - 1 ] ) return getValue( row_value, _col_values[ _cols - 1 ] );

        unsigned int row_1 = 0;
        unsigned int row_2 = 0;

        for ( unsigned int r = 1; r < _rows; r++ )
        {
            row_1 = r - 1;
            row_2 = r;

            if ( row_value >= _row_values[ row_1 ] && row_value < _row_values[ row_2 ] ) break;
        }

        unsigned int col_1 = 0;
        unsigned int col_2 = 0;

        for ( unsigned int c = 1; c < _cols; c++ )
        {
            col_1 = c - 1;
            col_2 = c;

            if ( col_value >= _col_values[ col_1 ] && col_value < _col_values[ col_2 ] ) break;
        }

        double result_1 = ( col_value - _col_values[ col_1 ] ) * _inter_data[ row_1 * _cols + col_1 ]
                        + _table_data[ row_1 * _cols + col_1 ];

        double result_2 = ( col_value - _col_values[ col_1 ] ) * _inter_data[ row_2 * _cols + col_1 ]
                        + _table_data[ row_2 * _cols + col_1 ];

        double rowDelta  = _row_values[ row_2 ] - _row_values[ row_1 ];
        double rowFactor = 0.0;

        if ( fabs( rowDelta ) > 1.0e-16 )
        {
            rowFactor = ( row_value - _row_values[ row_1 ] ) / rowDelta;
        }

        return rowFactor * ( result_2 - result_1 ) + result_1;
    }
    else
    {
        Exception e;

        e.setType( Exception::UnknownException );
        e.setInfo( "Invalid table size." );

        FDM_THROW( e );
    }

    return std::numeric_limits< double >::quiet_NaN();
}

////////////////////////////////////////////////////////////////////////////////

double Table2::getValueByIndex( unsigned int row_index, unsigned int col_index ) const
{
    if ( _rows > 0 && row_index < _rows
      && _cols > 0 && col_index < _cols )
    {
        return _table_data[ row_index * _cols + col_index ];
    }

    return std::numeric_limits< double >::quiet_NaN();
}

////////////////////////////////////////////////////////////////////////////////

bool Table2::isValid() const
{
    bool result = ( _size > 0 ) ? true : false;

    if ( result )
    {
        for ( unsigned int c = 0; c < _cols; c++ )
        {
            if ( result ) result = Misc::isValid( _col_values[ c ] );

            if ( c > 0 )
            {
                if ( result ) result = _col_values[ c - 1 ] < _col_values[ c ];
            }

            if ( !result ) break;
        }

        for ( unsigned int r = 0; r < _rows; r++ )
        {
            if ( result ) result = Misc::isValid( _row_values[ r ] );

            if ( r > 0 )
            {
                if ( result ) result = _row_values[ r - 1 ] < _row_values[ r ];
            }

            if ( !result ) break;
        }

        for ( unsigned int i = 0; i < _size; i++ )
        {
            if ( result ) result = Misc::isValid( _table_data[ i ] );
            if ( result ) result = Misc::isValid( _inter_data[ i ] );

            if ( !result ) break;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

std::string Table2::toString()
{
    std::stringstream ss;

    ss << "\t";

    for ( unsigned int c = 0; c < _cols; c++ )
    {
        ss << _col_values[ c ] << ",\t";
    }

    ss << std::endl;

    for ( unsigned int r = 0; r < _rows; r++ )
    {
        ss << _row_values[ r ] << "\t";

        for ( unsigned int c = 0; c < _cols; c++ )
        {
            ss << _table_data[ r * _cols + c ] << ",\t";
        }

        ss << std::endl;
    }

    return ss.str();
}

////////////////////////////////////////////////////////////////////////////////

const Table2& Table2::operator= ( const Table2 &table )
{
    FDM_DELTAB( _row_values );
    FDM_DELTAB( _col_values );
    FDM_DELTAB( _table_data );
    FDM_DELTAB( _inter_data );

    _rows = table._rows;
    _cols = table._cols;
    _size = table._size;

    if ( _size > 0 )
    {
        _row_values = new double [ _rows ];
        _col_values = new double [ _cols ];
        _table_data = new double [ _size ];

        _inter_data = new double [ _size ];

        for ( unsigned int i = 0; i < _rows; i++ ) _row_values[ i ] = table._row_values[ i ];
        for ( unsigned int i = 0; i < _cols; i++ ) _col_values[ i ] = table._col_values[ i ];

        for ( unsigned int i = 0; i < _size; i++ )
        {
            _table_data[ i ] = table._table_data[ i ];
            _inter_data[ i ] = table._inter_data[ i ] ;
        }
    }

    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

void Table2::updateInterpolationData()
{
    for ( unsigned int r = 0; r < _rows; r++ )
    {
        for ( unsigned int c = 0; c < _cols - 1; c++ )
        {
            _inter_data[ r * _cols + c ] =
                ( _table_data[ r * _cols + c + 1 ] - _table_data[ r * _cols + c ] )
              / ( _col_values[ c + 1 ] - _col_values[ c ] );
        }
    }
}
