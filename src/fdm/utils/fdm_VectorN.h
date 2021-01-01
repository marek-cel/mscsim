/****************************************************************************//*
 * Copyright (C) 2021 Marek M. Cel
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
#ifndef FDM_VECTORN_H
#define FDM_VECTORN_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_Defines.h>
#include <fdm/fdm_Exception.h>
#include <fdm/fdm_Types.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief N elements dynamic allocated vector class.
 */
class FDMEXPORT VectorN
{
public:

    /** @brief Constructor. */
    VectorN();

    /**
     * @brief Constructor.
     * @param size vector size
     */
    VectorN( unsigned int size );

    /** @brief Copy constructor. */
    VectorN( const VectorN &vect );

    /** @brief Destructor. */
    virtual ~VectorN();

    /** @return TRUE if all items are valid */
    virtual bool isValid() const;

    /** @return vector length */
    virtual double getLength() const;

    /** @brief This function normalizes vector. */
    virtual void normalize();

    /** @brief uts vector items into given array. */
    virtual void getArray( double items[] ) const;

    /**
     * @brief Gets vector item of given indicies.
     * This function is bound-checked which may affect performance.
     * Throws an exception when index is out of range.
     * @return vector item of given indicies.
     */
    virtual double getItem( unsigned int index ) const;

    /** @brief Sets vector items from given array. */
    virtual void setArray( const double items[] );

    /**
     * @brief Sets vector item of given indicies.
     * This function is bound-checked which may affect performance.
     * Throws an exception when index is out of range.
     */
    virtual void setItem( unsigned int index, double val );

    virtual void setValue( double val );

    /**
     * @brief Returns vector size (number of elements)
     * @return vector size (number of elements)
     */
    inline unsigned int getSize() const { return _size; }

    /** @brief Returns string representation of the vector. */
    virtual std::string toString() const;

    /** @brief Resizes vector if needed. */
    virtual void resize( unsigned int size );

    /** @brief Sets all vector items to zero. */
    virtual void zeroize();

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
    const VectorN& operator= ( const VectorN &vect );

    /** @brief Addition operator. */
    VectorN operator+ ( const VectorN &vect ) const;

    /** @brief Negation operator. */
    VectorN operator- () const;

    /** @brief Subtraction operator. */
    VectorN operator- ( const VectorN &vect ) const;

    /** @brief Multiplication operator (by scalar). */
    VectorN operator* ( double val ) const;

    /** @brief Division operator (by scalar). */
    VectorN operator/ ( double val ) const;

    /** @brief Unary addition operator. */
    VectorN& operator+= ( const VectorN &vect );

    /** @brief Unary subtraction operator. */
    VectorN& operator-= ( const VectorN &vect );

    /** @brief Unary multiplication operator (by scalar). */
    VectorN& operator*= ( double val );

    /** @brief Unary division operator (by scalar). */
    VectorN& operator/= ( double val );

protected:

    unsigned int _size;     ///< vector size

    double *_items;         ///< vector items
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

/** @brief Multiplication operator (by scalar). */
inline fdm::VectorN operator* ( double val, const fdm::VectorN & vect )
{
    return ( vect * val );
}

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_VECTORN_H
