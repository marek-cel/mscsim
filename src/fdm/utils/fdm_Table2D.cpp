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

#include <math.h>
#include <limits>
#include <sstream>

#include <fdm/fdm_Exception.h>
#include <fdm/utils/fdm_Misc.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Table2D::Table2D() :
    m_rows ( 0 ),
    m_cols ( 0 ),
    m_size ( 0 ),
    m_rowValues ( 0 ),
    m_colValues ( 0 ),
    m_tableData ( 0 ),
    m_interpolData ( 0 )
{}

////////////////////////////////////////////////////////////////////////////////

Table2D::Table2D( const std::vector< double > &rowValues,
                  const std::vector< double > &colValues,
                  const std::vector< double > &tableData ) :
    m_rows ( 0 ),
    m_cols ( 0 ),
    m_size ( 0 ),
    m_rowValues ( 0 ),
    m_colValues ( 0 ),
    m_tableData ( 0 ),
    m_interpolData ( 0 )
{
    if ( rowValues.size() * colValues.size() == tableData.size() )
    {
        m_size = tableData.size();

        if ( m_size > 0 )
        {
            m_rows = rowValues.size();
            m_cols = colValues.size();

            m_rowValues = new double [ m_rows ];
            m_colValues = new double [ m_cols ];
            m_tableData = new double [ m_size ];

            m_interpolData = new double [ m_size ];

            for ( unsigned int i = 0; i < m_rows; i++ ) m_rowValues[ i ] = rowValues[ i ];
            for ( unsigned int i = 0; i < m_cols; i++ ) m_colValues[ i ] = colValues[ i ];

            for ( unsigned int i = 0; i < m_size; i++ )
            {
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

Table2D::Table2D( const Table2D &table ) :
    m_rows ( table.m_rows ),
    m_cols ( table.m_cols ),
    m_size ( table.m_size ),
    m_rowValues ( 0 ),
    m_colValues ( 0 ),
    m_tableData ( 0 ),
    m_interpolData ( 0 )
{
    if ( m_size > 0 )
    {
        m_rowValues = new double [ m_rows ];
        m_colValues = new double [ m_cols ];
        m_tableData = new double [ m_size ];

        m_interpolData = new double [ m_size ];

        for ( unsigned int i = 0; i < m_rows; i++ ) m_rowValues[ i ] = table.m_rowValues[ i ];
        for ( unsigned int i = 0; i < m_cols; i++ ) m_colValues[ i ] = table.m_colValues[ i ];

        for ( unsigned int i = 0; i < m_size; i++ )
        {
            m_tableData[ i ] = table.m_tableData[ i ];
            m_interpolData[ i ] = table.m_interpolData[ i ] ;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

Table2D::~Table2D()
{
    if ( m_rowValues ) delete m_rowValues;
    m_rowValues = 0;

    if ( m_colValues ) delete m_colValues;
    m_colValues = 0;

    if ( m_tableData ) delete m_tableData;
    m_tableData = 0;

    if ( m_interpolData ) delete m_interpolData;
    m_interpolData = 0;
}

////////////////////////////////////////////////////////////////////////////////

double Table2D::getValue( double rowValue, double colValue ) const
{
    if ( m_size > 0 )
    {
        if ( rowValue < m_rowValues[ 0 ] ) return getValue( m_rowValues[ 0 ], colValue );
        if ( colValue < m_colValues[ 0 ] ) return getValue( rowValue, m_colValues[ 0 ] );

        if ( rowValue > m_rowValues[ m_rows - 1 ] ) return getValue( m_rowValues[ m_rows - 1 ], colValue );
        if ( colValue > m_colValues[ m_cols - 1 ] ) return getValue( rowValue, m_colValues[ m_cols - 1 ] );

        unsigned int row_1 = 0;
        unsigned int row_2 = 0;

        for ( unsigned int r = 1; r < m_rows; r++ )
        {
            row_1 = r - 1;
            row_2 = r;

            if ( rowValue >= m_rowValues[ row_1 ] && rowValue < m_rowValues[ row_2 ] ) break;
        }

        unsigned int col_1 = 0;
        unsigned int col_2 = 0;

        for ( unsigned int c = 1; c < m_cols; c++ )
        {
            col_1 = c - 1;
            col_2 = c;

            if ( colValue >= m_colValues[ col_1 ] && colValue < m_colValues[ col_2 ] ) break;
        }

        double result_1 = ( colValue - m_colValues[ col_1 ] ) * m_interpolData[ row_1 * m_cols + col_1 ]
                        + m_tableData[ row_1 * m_cols + col_1 ];

        double result_2 = ( colValue - m_colValues[ col_1 ] ) * m_interpolData[ row_2 * m_cols + col_1 ]
                        + m_tableData[ row_2 * m_cols + col_1 ];

        double rowDelta  = m_rowValues[ row_2 ] - m_rowValues[ row_1 ];
        double rowFactor = 0.0;

        if ( fabs( rowDelta ) > 1.0e-16 )
        {
            rowFactor = ( rowValue - m_rowValues[ row_1 ] ) / rowDelta;
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
    if ( m_rows > 0 && rowIndex < m_rows
      && m_cols > 0 && colIndex < m_cols )
    {
        return m_tableData[ rowIndex * m_cols + colIndex ];
    }

    return std::numeric_limits< double >::quiet_NaN();
}

////////////////////////////////////////////////////////////////////////////////

bool Table2D::isValid() const
{
    bool result = ( m_size > 0 ) ? true : false;

    if ( result )
    {
        for ( unsigned int c = 0; c < m_cols; c++ )
        {
            if ( result ) result = Misc::isValid( m_colValues[ c ] );

            if ( c > 0 )
            {
                if ( result ) result = m_colValues[ c - 1 ] < m_colValues[ c ];
            }

            if ( !result ) break;
        }

        for ( unsigned int r = 0; r < m_rows; r++ )
        {
            if ( result ) result = Misc::isValid( m_rowValues[ r ] );

            if ( r > 0 )
            {
                if ( result ) result = m_rowValues[ r - 1 ] < m_rowValues[ r ];
            }

            if ( !result ) break;
        }

        for ( unsigned int i = 0; i < m_size; i++ )
        {
            if ( result ) result = Misc::isValid( m_tableData[ i ] );
            if ( result ) result = Misc::isValid( m_interpolData[ i ] );

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

    for ( unsigned int c = 0; c < m_cols; c++ )
    {
        ss << m_colValues[ c ] << ",\t";
    }

    ss << std::endl;

    for ( unsigned int r = 0; r < m_rows; r++ )
    {
        ss << m_rowValues[ r ] << "\t";

        for ( unsigned int c = 0; c < m_cols; c++ )
        {
            ss << m_tableData[ r * m_cols + c ] << ",\t";
        }

        ss << std::endl;
    }

    return ss.str();
}

////////////////////////////////////////////////////////////////////////////////

const Table2D& Table2D::operator= ( const Table2D &table )
{
    if ( m_rowValues ) delete m_rowValues;
    m_rowValues = 0;

    if ( m_colValues ) delete m_colValues;
    m_colValues = 0;

    if ( m_tableData ) delete m_tableData;
    m_tableData = 0;

    if ( m_interpolData ) delete m_interpolData;
    m_interpolData = 0;

    m_rows = table.m_rows;
    m_cols = table.m_cols;
    m_size = table.m_size;

    if ( m_size > 0 )
    {
        m_rowValues = new double [ m_rows ];
        m_colValues = new double [ m_cols ];
        m_tableData = new double [ m_size ];

        m_interpolData = new double [ m_size ];

        for ( unsigned int i = 0; i < m_rows; i++ ) m_rowValues[ i ] = table.m_rowValues[ i ];
        for ( unsigned int i = 0; i < m_cols; i++ ) m_colValues[ i ] = table.m_colValues[ i ];

        for ( unsigned int i = 0; i < m_size; i++ )
        {
            m_tableData[ i ] = table.m_tableData[ i ];
            m_interpolData[ i ] = table.m_interpolData[ i ] ;
        }
    }

    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

void Table2D::updateInterpolationData()
{
    for ( unsigned int r = 0; r < m_rows; r++ )
    {
        for ( unsigned int c = 0; c < m_cols - 1; c++ )
        {
            m_interpolData[ r * m_cols + c ] =
                ( m_tableData[ r * m_cols + c + 1 ] - m_tableData[ r * m_cols + c ] )
              / ( m_colValues[ c + 1 ] - m_colValues[ c ] );
        }
    }
}
