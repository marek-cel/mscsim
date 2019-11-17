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

#include <fdm/utils/fdm_Table2D.h>

#include <cmath>
#include <limits>
#include <sstream>

#include <fdm/fdm_Exception.h>
#include <fdm/utils/fdm_Misc.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Table2D::Table2D() :
    _rows ( 0 ),
    _cols ( 0 ),
    _size ( 0 ),
    _rowValues ( FDM_NULLPTR ),
    _colValues ( FDM_NULLPTR ),
    _tableData ( FDM_NULLPTR ),
    _interpolData ( FDM_NULLPTR )
{}

////////////////////////////////////////////////////////////////////////////////

Table2D::Table2D( const std::vector< double > &rowValues,
                  const std::vector< double > &colValues,
                  const std::vector< double > &tableData ) :
    _rows ( 0 ),
    _cols ( 0 ),
    _size ( 0 ),
    _rowValues ( FDM_NULLPTR ),
    _colValues ( FDM_NULLPTR ),
    _tableData ( FDM_NULLPTR ),
    _interpolData ( FDM_NULLPTR )
{
    if ( rowValues.size() * colValues.size() == tableData.size() )
    {
        _size = tableData.size();

        if ( _size > 0 )
        {
            _rows = rowValues.size();
            _cols = colValues.size();

            _rowValues = new double [ _rows ];
            _colValues = new double [ _cols ];
            _tableData = new double [ _size ];

            _interpolData = new double [ _size ];

            for ( unsigned int i = 0; i < _rows; i++ ) _rowValues[ i ] = rowValues[ i ];
            for ( unsigned int i = 0; i < _cols; i++ ) _colValues[ i ] = colValues[ i ];

            for ( unsigned int i = 0; i < _size; i++ )
            {
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

Table2D::Table2D( const Table2D &table ) :
    _rows ( table._rows ),
    _cols ( table._cols ),
    _size ( table._size ),
    _rowValues ( FDM_NULLPTR ),
    _colValues ( FDM_NULLPTR ),
    _tableData ( FDM_NULLPTR ),
    _interpolData ( FDM_NULLPTR )
{
    if ( _size > 0 )
    {
        _rowValues = new double [ _rows ];
        _colValues = new double [ _cols ];
        _tableData = new double [ _size ];

        _interpolData = new double [ _size ];

        for ( unsigned int i = 0; i < _rows; i++ ) _rowValues[ i ] = table._rowValues[ i ];
        for ( unsigned int i = 0; i < _cols; i++ ) _colValues[ i ] = table._colValues[ i ];

        for ( unsigned int i = 0; i < _size; i++ )
        {
            _tableData[ i ] = table._tableData[ i ];
            _interpolData[ i ] = table._interpolData[ i ] ;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

Table2D::~Table2D()
{
    FDM_DELTAB( _rowValues );
    FDM_DELTAB( _colValues );
    FDM_DELTAB( _tableData );
    FDM_DELTAB( _interpolData );
}

////////////////////////////////////////////////////////////////////////////////

double Table2D::getValue( double rowValue, double colValue ) const
{
    if ( _size > 0 )
    {
        if ( rowValue < _rowValues[ 0 ] ) return getValue( _rowValues[ 0 ], colValue );
        if ( colValue < _colValues[ 0 ] ) return getValue( rowValue, _colValues[ 0 ] );

        if ( rowValue > _rowValues[ _rows - 1 ] ) return getValue( _rowValues[ _rows - 1 ], colValue );
        if ( colValue > _colValues[ _cols - 1 ] ) return getValue( rowValue, _colValues[ _cols - 1 ] );

        unsigned int row_1 = 0;
        unsigned int row_2 = 0;

        for ( unsigned int r = 1; r < _rows; r++ )
        {
            row_1 = r - 1;
            row_2 = r;

            if ( rowValue >= _rowValues[ row_1 ] && rowValue < _rowValues[ row_2 ] ) break;
        }

        unsigned int col_1 = 0;
        unsigned int col_2 = 0;

        for ( unsigned int c = 1; c < _cols; c++ )
        {
            col_1 = c - 1;
            col_2 = c;

            if ( colValue >= _colValues[ col_1 ] && colValue < _colValues[ col_2 ] ) break;
        }

        double result_1 = ( colValue - _colValues[ col_1 ] ) * _interpolData[ row_1 * _cols + col_1 ]
                        + _tableData[ row_1 * _cols + col_1 ];

        double result_2 = ( colValue - _colValues[ col_1 ] ) * _interpolData[ row_2 * _cols + col_1 ]
                        + _tableData[ row_2 * _cols + col_1 ];

        double rowDelta  = _rowValues[ row_2 ] - _rowValues[ row_1 ];
        double rowFactor = 0.0;

        if ( fabs( rowDelta ) > 1.0e-16 )
        {
            rowFactor = ( rowValue - _rowValues[ row_1 ] ) / rowDelta;
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

double Table2D::getValueByIndex( unsigned int rowIndex, unsigned int colIndex ) const
{
    if ( _rows > 0 && rowIndex < _rows
      && _cols > 0 && colIndex < _cols )
    {
        return _tableData[ rowIndex * _cols + colIndex ];
    }

    return std::numeric_limits< double >::quiet_NaN();
}

////////////////////////////////////////////////////////////////////////////////

bool Table2D::isValid() const
{
    bool result = ( _size > 0 ) ? true : false;

    if ( result )
    {
        for ( unsigned int c = 0; c < _cols; c++ )
        {
            if ( result ) result = Misc::isValid( _colValues[ c ] );

            if ( c > 0 )
            {
                if ( result ) result = _colValues[ c - 1 ] < _colValues[ c ];
            }

            if ( !result ) break;
        }

        for ( unsigned int r = 0; r < _rows; r++ )
        {
            if ( result ) result = Misc::isValid( _rowValues[ r ] );

            if ( r > 0 )
            {
                if ( result ) result = _rowValues[ r - 1 ] < _rowValues[ r ];
            }

            if ( !result ) break;
        }

        for ( unsigned int i = 0; i < _size; i++ )
        {
            if ( result ) result = Misc::isValid( _tableData[ i ] );
            if ( result ) result = Misc::isValid( _interpolData[ i ] );

            if ( !result ) break;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

std::string Table2D::toString()
{
    std::stringstream ss;

    ss << "\t";

    for ( unsigned int c = 0; c < _cols; c++ )
    {
        ss << _colValues[ c ] << ",\t";
    }

    ss << std::endl;

    for ( unsigned int r = 0; r < _rows; r++ )
    {
        ss << _rowValues[ r ] << "\t";

        for ( unsigned int c = 0; c < _cols; c++ )
        {
            ss << _tableData[ r * _cols + c ] << ",\t";
        }

        ss << std::endl;
    }

    return ss.str();
}

////////////////////////////////////////////////////////////////////////////////

const Table2D& Table2D::operator= ( const Table2D &table )
{
    FDM_DELTAB( _rowValues );
    FDM_DELTAB( _colValues );
    FDM_DELTAB( _tableData );
    FDM_DELTAB( _interpolData );

    _rows = table._rows;
    _cols = table._cols;
    _size = table._size;

    if ( _size > 0 )
    {
        _rowValues = new double [ _rows ];
        _colValues = new double [ _cols ];
        _tableData = new double [ _size ];

        _interpolData = new double [ _size ];

        for ( unsigned int i = 0; i < _rows; i++ ) _rowValues[ i ] = table._rowValues[ i ];
        for ( unsigned int i = 0; i < _cols; i++ ) _colValues[ i ] = table._colValues[ i ];

        for ( unsigned int i = 0; i < _size; i++ )
        {
            _tableData[ i ] = table._tableData[ i ];
            _interpolData[ i ] = table._interpolData[ i ] ;
        }
    }

    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

void Table2D::updateInterpolationData()
{
    for ( unsigned int r = 0; r < _rows; r++ )
    {
        for ( unsigned int c = 0; c < _cols - 1; c++ )
        {
            _interpolData[ r * _cols + c ] =
                ( _tableData[ r * _cols + c + 1 ] - _tableData[ r * _cols + c ] )
              / ( _colValues[ c + 1 ] - _colValues[ c ] );
        }
    }
}
