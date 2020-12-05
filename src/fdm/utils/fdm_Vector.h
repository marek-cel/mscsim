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
#ifndef FDM_VECTOR_H
#define FDM_VECTOR_H

////////////////////////////////////////////////////////////////////////////////

#include <cmath>
#include <limits>
#include <sstream>
#include <string>

#include <fdm/fdm_Exception.h>
#include <fdm/utils/fdm_Misc.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Column vector base class template.
 *
 * Column vector base class template which is using integer template parameters
 * to specify vector size. Such an approach does not allow to perform
 * mathematical operation between vectors which sizes do not match each other
 * as they are of different types.
 *
 * @tparam SIZE vector size
 */
template < unsigned int SIZE >
class Vector
{
public:

    /** @brief Constructor. */
    Vector() :
        _size ( SIZE )
    {
        zeroize();
    }

    /** @brief Copy constructor. */
    Vector( const Vector< SIZE > &vect ) :
        _size ( SIZE )
    {
        setArray( vect._items );
    }

    /** @brief Constructor. */
    Vector( const double items[] ) :
        _size ( SIZE )
    {
        setArray( items );
    }

    /** @return TRUE if all items are valid */
    virtual bool isValid() const
    {
        return Misc::isValid( _items, _size );
    }

    /** @return vector length squared */
    virtual double getLength2() const
    {
        double length2 = 0.0;

        for ( unsigned int i = 0; i < _size; i++ )
        {
            length2 += ( _items[ i ] * _items[ i ] );
        }

        return length2;
    }

    /** @return vector length */
    virtual double getLength() const
    {
        return sqrt( getLength2() );
    }

    /** @brief Normalizes vector. */
    virtual void normalize()
    {
        double length = getLength();

        if ( length > 0.0 )
        {
            double length_inv = 1.0 / length;

            for ( unsigned int i = 0; i < _size; i++ )
            {
                _items[ i ] *= length_inv;
            }
        }
    }

    /** @brief Puts vector items into given array. */
    virtual void getArray( double items[] ) const
    {
        for ( unsigned int i = 0; i < _size; i++ )
        {
            items[i] = _items[i];
        }
    }

    /**
     * @brief Gets vector item of given indicies.
     * This function is bound-checked which may affect performance.
     * Throws an exception when index is out of range.
     * @return vector item of given indicies.
     */
    virtual double getItem( unsigned int index ) const
    {
        if ( index < _size )
        {
            return _items[ index ];
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

    /** @brief Sets vector items from given array. */
    virtual void setArray( const double items[] )
    {
        for ( unsigned int i = 0; i < _size; i++ )
        {
            _items[i] = items[i];
        }
    }

    /**
     * @brief Sets vector item of given indicies.
     * This function is bound-checked which may affect performance.
     * Throws an exception when index is out of range.
     */
    virtual void setItem( unsigned int index, double val )
    {
        if ( index < _size )
        {
            _items[ index ] = val;
        }
        else
        {
            Exception e;

            e.setType( Exception::ArrayIndexOverLimit );
            e.setInfo( "Index over limit." );

            FDM_THROW( e );
        }
    }

    /** @brief Swaps vector rows. */
    virtual void swapRows( unsigned int row1, unsigned int row2 )
    {
        if ( row1 < _size && row2 < _size )
        {
            double temp = _items[ row1 ];
            _items[ row1 ] = _items[ row2 ];
            _items[ row2 ] = temp;
        }
        else
        {
            Exception e;

            e.setType( Exception::ArrayIndexOverLimit );
            e.setInfo( "Index over limit." );

            FDM_THROW( e );
        }
    }

    /** @brief Returns string representation of the vector. */
    virtual std::string toString() const
    {
        std::stringstream ss;

        for ( unsigned int i = 0; i < _size; i++ )
        {
            if ( i != 0 ) ss << ",";

            ss << _items[ i ];
        }

        return ss.str();
    }

    /** @brief Sets all vector items to zero. */
    virtual void zeroize()
    {
        for ( unsigned int i = 0; i < _size; i++ )
        {
            _items[ i ] = 0.0;
        }
    }

    /**
     * @brief Items accessor.
     * Please notice that this operator is NOT bound-checked.
     * If you want bound-checked item accessor use getItem(int) or
     * setItem(int,double) functions.
     */
    inline double operator() ( unsigned int index ) const
    {
#       ifdef _DEBUG
        if ( index >= _size )
        {
            Exception e;

            e.setType( Exception::ArrayIndexOverLimit );
            e.setInfo( "Index over limit." );

            FDM_THROW( e );
        }
#       endif

        return _items[ index ];
    }

    /**
     * @brief Items accessor.
     * Please notice that this operator is NOT bound-checked.
     * If you want bound-checked item accessor use getItem(int) or
     * setItem(int,double) functions.
     */
    inline double& operator() ( unsigned int index )
    {
#       ifdef _DEBUG
        if ( index >= _size )
        {
            Exception e;

            e.setType( Exception::ArrayIndexOverLimit );
            e.setInfo( "Index over limit." );

            FDM_THROW( e );
        }
#       endif

        return _items[ index ];
    }

    /** @brief Assignment operator. */
    const Vector< SIZE >& operator= ( const Vector< SIZE > &vect )
    {
        setArray( vect._items );

        return (*this);
    }

    /** @brief Addition operator. */
    Vector< SIZE > operator+ ( const Vector< SIZE > &vect ) const
    {
        Vector< SIZE > result;

        for ( unsigned int i = 0; i < _size; i++ )
        {
            result._items[ i ] = _items[ i ] + vect._items[ i ];
        }

        return result;
    }

    /** @brief Negation operator. */
    Vector< SIZE > operator- () const
    {
        Vector< SIZE > result;

        for ( unsigned int i = 0; i < _size; i++ )
        {
            result._items[ i ] = - _items[ i ];
        }

        return result;
    }

    /** @brief Subtraction operator. */
    Vector< SIZE > operator- ( const Vector< SIZE > &vect ) const
    {
        Vector< SIZE > result;

        for ( unsigned int i = 0; i < _size; i++ )
        {
            result._items[ i ] = _items[ i ] - vect._items[ i ];
        }

        return result;
    }

    /** @brief Multiplication operator (by scalar). */
    Vector< SIZE > operator* ( double val ) const
    {
        Vector< SIZE > result;

        for ( unsigned int i = 0; i < _size; i++ )
        {
            result._items[ i ] = _items[ i ] * val;
        }

        return result;
    }

    /** @brief Dot product operator. */
    double operator* ( const Vector< SIZE > &vect ) const
    {
        double result = 0.0;

        for ( unsigned int i = 0; i < _size; i++ )
        {
            result += _items[ i ] * vect._items[ i ];
        }

        return result;
    }

    /** @brief Division operator (by scalar). */
    Vector< SIZE > operator/ ( double val ) const
    {
        Vector< SIZE > result;

        for ( unsigned int i = 0; i < _size; i++ )
        {
            result._items[ i ] = _items[ i ] / val;
        }

        return result;
    }

    /** @brief Unary addition operator. */
    Vector< SIZE >& operator+= ( const Vector< SIZE > &vect )
    {
        for ( unsigned int i = 0; i < _size; i++ )
        {
            _items[ i ] += vect._items[ i ];
        }

        return (*this);
    }

    /** @brief Unary subtraction operator. */
    Vector< SIZE >& operator-= ( const Vector< SIZE > &vect )
    {
        for ( unsigned int i = 0; i < _size; i++ )
        {
            _items[ i ] -= vect._items[ i ];
        }

        return (*this);
    }

    /** @brief Unary multiplication operator (by scalar). */
    Vector< SIZE >& operator*= ( double val )
    {
        for ( unsigned int i = 0; i < _size; i++ )
        {
            _items[ i ] *= val;
        }

        return (*this);
    }

    /** @brief Unary division operator (by scalar). */
    Vector< SIZE >& operator/= ( double val )
    {
        for ( unsigned int i = 0; i < _size; i++ )
        {
            _items[ i ] /= val;
        }

        return (*this);
    }

    /** @brief Equality operator. */
    bool operator== ( const Vector< SIZE > &vect ) const
    {
        bool result = true;

        for ( unsigned int i = 0; i < _size; i++ )
        {
            result = result && ( _items[i] == vect._items[i] );
        }

        return result;
    }

    /** @brief Inequality operator. */
    bool operator!= ( const Vector< SIZE > &vect ) const
    {
        return !( (*this) == vect );
    }

protected:

    double _items[ SIZE ];      ///< vector items

    const unsigned int _size;   ///< vector size
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_VECTOR_H
