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
#ifndef FDM_INTEGRATOR_H
#define FDM_INTEGRATOR_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/utils/fdm_VectorN.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Abstract numerical integration template class.
 */
template < class TYPE >
class Integrator
{
public:

    /**
     * Constructor.
     * @param size of vector
     * @param object pointer
     * @param pointer to function which calculates vector derivative and takes current vector as first argument and resulting vector derivative as second
     */
    Integrator( TYPE *obj = FDM_NULLPTR, void (TYPE::*fun)(const VectorN &, VectorN *) = FDM_NULLPTR ) :
        _obj ( obj ),
        _fun ( fun )
    {
        if ( _obj == FDM_NULLPTR )
        {
            Exception e;

            e.setType( Exception::NullPointer );
            e.setInfo( "Object pointer NULL." );

            FDM_THROW( e );
        }

        if ( _fun == FDM_NULLPTR )
        {
            Exception e;

            e.setType( Exception::NullPointer );
            e.setInfo( "Function pointer NULL." );

            FDM_THROW( e );
        }
    }

    /**
     * Integrates given vector.
     * This is interface abstract method.
     * @param step integration time step [s]
     * @param vect integrating vector
     */
    virtual void integrate( double step, VectorN *vect ) = 0;

protected:

    /** Calls function calculating derivative of the given vector. */
    inline void fun( const VectorN &x_0, VectorN *x_dot )
    {
        (_obj->*_fun)( x_0, x_dot );
    }

private:

    TYPE *const _obj;           ///< object pointer

    void (TYPE::*_fun)(const VectorN &, VectorN *); ///< function pointer

    /** Using this constructor is forbidden. */
    Integrator( const Integrator & ) {}
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_INTEGRATOR_H
