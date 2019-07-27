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
#ifndef FDM_MATRIX_H
#define FDM_MATRIX_H

////////////////////////////////////////////////////////////////////////////////

#include <sstream>

#include <fdm/utils/fdm_Vector.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Matrix base class template.
 */
template < unsigned int ROWS, unsigned int COLS >
class Matrix
{
public:

    /** Constructor. */
    Matrix() :
        m_rows ( ROWS ),
        m_cols ( COLS ),
        m_size ( ROWS * COLS )
    {
        for ( unsigned int i = 0; i < m_size; i++ )
        {
            m_items[ i ] = 0.0;
        }
    }

    /** Copy constructor. */
    Matrix( const Matrix< ROWS, COLS > &mtrx ) :
        m_rows ( ROWS ),
        m_cols ( COLS ),
        m_size ( ROWS * COLS )
    {
        setArray( mtrx.m_items );
    }

    /** Constructor. */
    Matrix( const double items[] ) :
        m_rows ( ROWS ),
        m_cols ( COLS ),
        m_size ( ROWS * COLS )
    {
        setArray( items );
    }

    /** @return "true" if all items are valid */
    virtual bool isValid() const
    {
        return Misc::isValid( m_items, m_size );
    }

    /** Puts matrix items into given array. */
    virtual void getArray( double items[] ) const
    {
        for ( unsigned int i = 0; i < m_size; i++ )
        {
            items[ i ] = m_items[ i ];
        }
    }

    /**
     * This function is bound-checked which may affect performance.
     * Throws FDM_Exception when row or column index is out of range.
     * @return matrix item of given indicies.
     */
    virtual double getItem( unsigned int row, unsigned int col ) const
    {
        if ( ( row < m_rows ) && ( col < m_cols ) )
        {
            return m_items[ row * m_cols + col ];
        }
        else
        {
            Exception e;

            e.setType( Exception::ArrayIndexOverLimit );
            e.setInfo( "Index over limit." );

            FDM_THROW( e );
        }

        return std::numeric_limits< double >::quiet_NaN();
    }

    /** Sets matrix items from given array. */
    virtual void setArray( const double items[] )
    {
        for ( unsigned int i = 0; i < m_size; i++ )
        {
            m_items[ i ] = items[ i ];
        }
    }

    /**
     * This function sets matrix item of given indicies.
     * This function is bound-checked which may affect performance.
     * Throws FDM_Exception when row or column index is out of range.
     */
    virtual void setItem( unsigned int row, unsigned int col, double value )
    {
        if ( ( row < m_rows ) && ( col < m_cols ) )
        {
            m_items[ row * m_cols + col ] = value;
        }
        else
        {
            Exception e;

            e.setType( Exception::ArrayIndexOverLimit );
            e.setInfo( "Index over limit." );

            FDM_THROW( e );
        }
    }

    /** Swaps matrix rows. */
    virtual void swapRows( unsigned int row1, unsigned int row2 )
    {
        if ( ( row1 < m_rows ) && ( row2 < m_rows ) )
        {
            double temp[ COLS ];

            for ( unsigned int c = 0; c < COLS; c++ )
            {
                temp[ c ] = m_items[ row1 * m_cols + c ];
                m_items[ row1 * m_cols + c ] = m_items[ row2 * m_cols + c ];
                m_items[ row2 * m_cols + c ] = temp[ c ];
            }
        }
        else
        {
            Exception e;

            e.setType( Exception::ArrayIndexOverLimit );
            e.setInfo( "Index over limit." );

            FDM_THROW( e );
        }
    }

    /** Returns string representation of the matrix. */
    virtual std::string toString() const
    {
        std::stringstream ss;

        for ( unsigned int r = 0; r < m_rows; r++ )
        {
            for ( unsigned int c = 0; c < m_cols; c++ )
            {
                ss << m_items[ r * m_cols + c ] << ",";
            }

            ss << std::endl;
        }

        return ss.str();
    }

    /**
     * Items accessor. Please notice that this operator is NOT bound-checked.
     * If you want bound-checked item accessor use getItem(int,int) or
     * setItem(int,int,double) functions.
     */
    inline double operator() ( unsigned int row, unsigned int col ) const
    {
#       ifdef _DEBUG
        if ( ( row >= m_rows ) || ( col >= m_cols ) )
        {
            Exception e;

            e.setType( Exception::ArrayIndexOverLimit );
            e.setInfo( "Index over limit." );
            e.setFile( __FILE__ );
            e.setLine( __LINE__ );

            throw e;
        }
#       endif

        return m_items[ row * m_cols + col ];
    }

    /**
     * Items accessor. Please notice that this operator is NOT bound-checked.
     * If you want bound-checked item accessor use getItem(int,int) or
     * setItem(int,int,double) functions.
     */
    inline double& operator() ( unsigned int row, unsigned int col )
    {
#       ifdef _DEBUG
        if ( ( row >= m_rows ) || ( col >= m_cols ) )
        {
            Exception e;

            e.setType( Exception::ArrayIndexOverLimit );
            e.setInfo( "Index over limit." );
            e.setFile( __FILE__ );
            e.setLine( __LINE__ );

            throw e;
        }
#       endif

        return m_items[ row * m_cols + col ];
    }

    /** Assignment operator. */
    const Matrix< ROWS, COLS >& operator= ( const Matrix< ROWS, COLS > &mtrx )
    {
        setArray( mtrx.m_items );

        return (*this);
    }

    /** Addition operator. */
    Matrix< ROWS, COLS > operator+ ( const Matrix< ROWS, COLS > &mtrx ) const
    {
        Matrix< ROWS, COLS > result;
        
        for ( unsigned int i = 0; i < m_size; i++ )
        {
            result.m_items[ i ] = m_items[ i ] + mtrx.m_items[ i ];
        }

        return result;
    }

    /** Subtraction operator. */
    Matrix< ROWS, COLS > operator- ( const Matrix< ROWS, COLS > &mtrx ) const
    {
        Matrix< ROWS, COLS > result;

        for ( unsigned int i = 0; i < m_size; i++ )
        {
            result.m_items[ i ] = m_items[ i ] - mtrx.m_items[ i ];
        }

        return result;
    }

    /** Multiplication operator (by scalar). */
    Matrix< ROWS, COLS > operator* ( double value ) const
    {
        Matrix< ROWS, COLS > result;

        for ( unsigned int i = 0; i < m_size; i++ )
        {
            result.m_items[ i ] = m_items[ i ] * value;
        }

        return result;
    }

    /** Multiplication operator (by matrix). */
    Matrix< ROWS, COLS > operator* ( const Matrix< ROWS, COLS > &mtrx ) const
    {
        Matrix< ROWS, COLS > result;

        for ( unsigned int r = 0; r < m_rows; r++ )
        {
            for ( unsigned int c = 0; c < m_cols; c++ )
            {
                result(r,c) = 0.0;

                for ( unsigned int i = 0; i < m_cols; i++ )
                {
                    result(r,c) += ( m_items[ r*m_cols + i ] * mtrx(i,c) );
                }
            }
        }

        return result;
    }

    /** Multiplication operator (by vector). */
    Vector<ROWS> operator* ( const Vector<COLS> &vect ) const
    {
        Vector<ROWS> result;

        for ( unsigned int r = 0; r < m_rows; r++ )
        {
            result(r) = 0.0;

            for ( unsigned int c = 0; c < m_cols; c++ )
            {
                result(r) += ( m_items[ r*m_cols + c ] * vect(c) );
            }
        }

        return result;
    }

    /** Division operator (by scalar). */
    Matrix< ROWS, COLS > operator/ ( double value ) const
    {
        Matrix< ROWS, COLS > result;

        for ( unsigned int i = 0; i < m_size; i++ )
        {
            result.m_items[ i ] = m_items[ i ] / value;
        }

        return result;
    }

    /** Unary addition operator. */
    Matrix< ROWS, COLS >& operator+= ( const Matrix< ROWS, COLS > &mtrx )
    {
        for ( unsigned int i = 0; i < m_size; i++ )
        {
            m_items[ i ] += mtrx.m_items[ i ];
        }

        return (*this);
    }

    /** Unary subtraction operator. */
    Matrix< ROWS, COLS >& operator-= ( const Matrix< ROWS, COLS > &mtrx )
    {
        for ( unsigned int i = 0; i < m_size; i++ )
        {
            m_items[ i ] -= mtrx.m_items[ i ];
        }

        return (*this);
    }

    /** Unary multiplication operator (by scalar). */
    Matrix< ROWS, COLS >& operator*= ( double value )
    {
        for ( unsigned int i = 0; i < m_size; i++ )
        {
            m_items[ i ] *= value;
        }

        return (*this);
    }

    /** Unary division operator (by scalar). */
    Matrix< ROWS, COLS >& operator/= ( double value )
    {
        for ( unsigned int i = 0; i < m_size; i++ )
        {
            m_items[ i ] /= value;
        }

        return (*this);
    }

    /** Equality operator. */
    bool operator== ( const Matrix< ROWS, COLS > &mtrx ) const
    {
        bool result = true;

        for ( unsigned int i = 0; i < m_size; i++ )
        {
            result = result && ( m_items[ i ] == mtrx.m_items[ i ] );
        }

        return result;
    }

    /** Inequality operator. */
    bool operator!= ( const Matrix< ROWS, COLS > &mtrx ) const
    {
        return !( (*this) == mtrx );
    }

protected:

    double m_items[ ROWS * COLS ];  ///< matrix items

    const unsigned int m_rows;      ///< number of rows
    const unsigned int m_cols;      ///< number of columns
    const unsigned int m_size;      ///< matrix size
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_MATRIX_H
