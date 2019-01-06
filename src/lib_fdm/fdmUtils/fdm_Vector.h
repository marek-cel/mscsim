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
#ifndef FDM_VECTOR_H
#define FDM_VECTOR_H

////////////////////////////////////////////////////////////////////////////////

#include <limits>
#include <math.h>
#include <sstream>
#include <string>

#include <fdmMain/fdm_Exception.h>
#include <fdmUtils/fdm_Misc.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Column vector base class template.
 */
template < unsigned int SIZE >
class Vector
{
public:

    /** Constructor. */
    Vector() :
        m_size ( SIZE )
    {
        zeroize();
    }

    /** Copy constructor. */
    Vector( const Vector< SIZE > &vect ) :
        m_size ( SIZE )
    {
        setArray( vect.m_items );
    }

    /** Constructor. */
    Vector( const double items[] ) :
        m_size ( SIZE )
    {
        setArray( items );
    }

    /** @return TRUE if all items are valid */
    virtual bool isValid() const
    {
        return Misc::isValid( m_items, m_size );
    }

    /** @return vector length squared */
    virtual double getLength2() const
    {
        double length2 = 0.0;

        for ( unsigned int i = 0; i < m_size; i++ )
        {
            length2 += ( m_items[ i ] * m_items[ i ] );
        }

        return length2;
    }

    /** @return vector length */
    virtual double getLength() const
    {
        return sqrt( getLength2() );
    }

    /** This function normalizes vector. */
    void normalize()
    {
        double length = getLength();

        if ( length > 0.0 )
        {
            for ( unsigned int i = 0; i < m_size; i++ )
            {
                m_items[ i ] = m_items[ i ] / length;
            }
        }
    }

    /** Puts vector items into given array. */
    virtual void getArray( double items[] ) const
    {
        for ( unsigned int i = 0; i < m_size; i++ )
        {
            items[i] = m_items[i];
        }
    }

    /**
     * This function is bound-checked which may affect performance.
     * Throws FDM_Exception when index is out of range.
     * @return vector item of given indicies.
     */
    virtual double getItem( unsigned int index ) const
    {
        if ( index < m_size )
        {
            return m_items[ index ];
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

    /** Sets vector items from given array. */
    virtual void setArray( const double items[] )
    {
        for ( unsigned int i = 0; i < m_size; i++ )
        {
            m_items[i] = items[i];
        }
    }

    /**
     * This function sets vector item of given indicies.
     * This function is bound-checked which may affect performance.
     * Throws FDM_Exception when index is out of range.
     */
    virtual void setItem( unsigned int index, double val )
    {
        if ( index < m_size )
        {
            m_items[ index ] = val;
        }
        else
        {
            Exception e;

            e.setType( Exception::ArrayIndexOverLimit );
            e.setInfo( "Index over limit." );

            FDM_THROW( e );
        }
    }

    /** Swaps vector rows. */
    virtual void swapRows( unsigned int row1, unsigned int row2 )
    {
        if ( row1 < m_size && row2 < m_size )
        {
            double temp = m_items[ row1 ];
            m_items[ row1 ] = m_items[ row2 ];
            m_items[ row2 ] = temp;
        }
        else
        {
            Exception e;

            e.setType( Exception::ArrayIndexOverLimit );
            e.setInfo( "Index over limit." );

            FDM_THROW( e );
        }
    }

    /** Returns string representation of the vector. */
    virtual std::string toString() const
    {
        std::stringstream ss;

        for ( unsigned int i = 0; i < m_size; i++ )
        {
            if ( i != 0 ) ss << ",";

            ss << m_items[ i ];
        }

        return ss.str();
    }

    /**
     * Sets all vector items to zero.
     */
    virtual void zeroize()
    {
        for ( unsigned int i = 0; i < m_size; i++ )
        {
            m_items[ i ] = 0.0;
        }
    }

    /**
     * Items accessor. Please notice that this operator is NOT bound-checked.
     * If you want bound-checked item accessor use getItem(int) or
     * setItem(int,double) functions.
     */
    inline double operator() ( unsigned int index ) const
    {
#       ifdef _DEBUG
        if ( index >= m_size )
        {
            Exception e;

            e.setType( Exception::ArrayIndexOverLimit );
            e.setInfo( "Index over limit." );

            FDM_THROW( e );
        }
#       endif

        return m_items[ index ];
    }

    /**
     * Items accessor. Please notice that this operator is NOT bound-checked.
     * If you want bound-checked item accessor use getItem(int) or
     * setItem(int,double) functions.
     */
    inline double& operator() ( unsigned int index )
    {
#       ifdef _DEBUG
        if ( index >= m_size )
        {
            Exception e;

            e.setType( Exception::ArrayIndexOverLimit );
            e.setInfo( "Index over limit." );

            FDM_THROW( e );
        }
#       endif

        return m_items[ index ];
    }

    /** Assignment operator. */
    const Vector< SIZE >& operator= ( const Vector< SIZE > &vect )
    {
        setArray( vect.m_items );

        return (*this);
    }

    /** Addition operator. */
    Vector< SIZE > operator+ ( const Vector< SIZE > &vect ) const
    {
        Vector< SIZE > result;

        for ( unsigned int i = 0; i < m_size; i++ )
        {
            result.m_items[ i ] = m_items[ i ] + vect.m_items[ i ];
        }

        return result;
    }

    /** Negation operator. */
    Vector< SIZE > operator- () const
    {
        Vector< SIZE > result;

        for ( unsigned int i = 0; i < m_size; i++ )
        {
            result.m_items[ i ] = - m_items[ i ];
        }

        return result;
    }

    /** Subtraction operator. */
    Vector< SIZE > operator- ( const Vector< SIZE > &vect ) const
    {
        Vector< SIZE > result;

        for ( unsigned int i = 0; i < m_size; i++ )
        {
            result.m_items[ i ] = m_items[ i ] - vect.m_items[ i ];
        }

        return result;
    }

    /** Multiplication operator (by scalar). */
    Vector< SIZE > operator* ( double val ) const
    {
        Vector< SIZE > result;

        for ( unsigned int i = 0; i < m_size; i++ )
        {
            result.m_items[ i ] = m_items[ i ] * val;
        }

        return result;
    }

    /** Dot product operator. */
    double operator* ( const Vector< SIZE > &vect ) const
    {
        double result = 0.0;

        for ( unsigned int i = 0; i < m_size; i++ )
        {
            result += m_items[ i ] * vect.m_items[ i ];
        }

        return result;
    }

    /** Division operator (by scalar). */
    Vector< SIZE > operator/ ( double val ) const
    {
        Vector< SIZE > result;

        for ( unsigned int i = 0; i < m_size; i++ )
        {
            result.m_items[ i ] = m_items[ i ] / val;
        }

        return result;
    }

    /** Unary addition operator. */
    Vector< SIZE >& operator+= ( const Vector< SIZE > &vect )
    {
        for ( unsigned int i = 0; i < m_size; i++ )
        {
            m_items[ i ] += vect.m_items[ i ];
        }

        return (*this);
    }

    /** Unary subtraction operator. */
    Vector< SIZE >& operator-= ( const Vector< SIZE > &vect )
    {
        for ( unsigned int i = 0; i < m_size; i++ )
        {
            m_items[ i ] -= vect.m_items[ i ];
        }

        return (*this);
    }

    /** Unary multiplication operator (by scalar). */
    Vector< SIZE >& operator*= ( double val )
    {
        for ( unsigned int i = 0; i < m_size; i++ )
        {
            m_items[ i ] *= val;
        }

        return (*this);
    }

    /** Unary division operator (by scalar). */
    Vector< SIZE >& operator/= ( double val )
    {
        for ( unsigned int i = 0; i < m_size; i++ )
        {
            m_items[ i ] /= val;
        }

        return (*this);
    }

    /** Equality operator. */
    bool operator== ( const Vector< SIZE > &vect ) const
    {
        bool result = true;

        for ( unsigned int i = 0; i < m_size; i++ )
        {
            result = result && ( m_items[i] == vect.m_items[i] );
        }

        return result;
    }

    /** Inequality operator. */
    bool operator!= ( const Vector< SIZE > &vect ) const
    {
        return !( (*this) == vect );
    }

protected:

    double m_items[ SIZE ];     ///< vector items

    const unsigned int m_size;  ///< vector size
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_VECTOR_H
