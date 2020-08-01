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
#ifndef FDM_RUNGEKUTTA4_H
#define FDM_RUNGEKUTTA4_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/utils/fdm_Integrator.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Runge-Kutta 4th order numerical integration template class.
 *
 * @see Press W., et al.: Numerical Recipes: The Art of Scientific Computing, 2007, p.907
 * @see Krupowicz A.: Metody numeryczne zagadnien poczatkowych rownan rozniczkowych zwyczajnych, 1986, p.185. [in Polish]
 * @see Baron B., Piatek L.: Metody numeryczne w C++ Builder, 2004, p.331. [in Polish]
 * @see https://en.wikipedia.org/wiki/Runge%E2%80%93Kutta_methods
 */
template < class TYPE >
class RungeKutta4 : public Integrator< TYPE >
{
public:

    /** Constructor. */
    RungeKutta4( TYPE *obj = 0, void (TYPE::*fun)(const VectorN &, VectorN *) = 0 ) :
        Integrator< TYPE > ( obj, fun )
    {}

    /** Destructor. */
    virtual ~RungeKutta4() {}

    /**
     * Integrates given vector using
     * Runge-Kutta 4th order integration algorithm.
     * @param step integration time step [s]
     * @param vect integrating vector
     */
    void integrate( double step, VectorN *vect )
    {
        _xt = (*vect);

        _k1.resize( vect->getSize() );
        _k2.resize( vect->getSize() );
        _k3.resize( vect->getSize() );
        _k4.resize( vect->getSize() );

        _k1.zeroize();
        _k2.zeroize();
        _k3.zeroize();
        _k4.zeroize();

        // k1 - derivatives calculation
        this->fun( _xt, &_k1 );

        // k2 - derivatives calculation
        _xt = (*vect) + _k1 * ( step / 2.0 );
        this->fun( _xt, &_k2 );

        // k3 - derivatives calculation
        _xt = (*vect) + _k2 * ( step / 2.0 );
        this->fun( _xt, &_k3 );

        // k4 - derivatives calculation
        _xt = (*vect) + _k3 * step;
        this->fun( _xt, &_k4 );

        // integration
        (*vect) = (*vect) + ( _k1 + _k2 * 2.0 + _k3 * 2.0 + _k4 ) * ( step / 6.0 );
    }

    /**
     * Sets vectot size.
     * @param size
     */
    void setSize( unsigned int size )
    {

    }

private:

    VectorN _k1;        ///< auxiliary vector
    VectorN _k2;        ///< auxiliary vector
    VectorN _k3;        ///< auxiliary vector
    VectorN _k4;        ///< auxiliary vector

    VectorN _xt;        ///< auxiliary vector

    /** Using this constructor is forbidden. */
    RungeKutta4( const RungeKutta4 & ) {}
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_RUNGEKUTTA4_H
