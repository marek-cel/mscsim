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
#ifndef FDM_EULERRECT_H
#define FDM_EULERRECT_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/utils/fdm_Integrator.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Euler's rectangular numerical integration template class.
 *
 * @see Press W., et al.: Numerical Recipes: The Art of Scientific Computing, 2007, p.907
 * @see Allerton D.: Principles of Flight Simulation, 2009, p.58
 * @see DeLoura M.: Game Programming Gems Vol. 1, 2000, p.176
 * @see Krupowicz A.: Metody numeryczne zagadnien poczatkowych rownan rozniczkowych zwyczajnych, 1986, p.29. [in Polish]
 * @see Matulewski J., et. al.: Grafika fizyka metody numeryczne, 2010, p.309. [in Polish]
 * @see https://en.wikipedia.org/wiki/Euler_method
 */
template < unsigned int SIZE, class TYPE >
class EulerRect : public Integrator< SIZE, TYPE >
{
public:

    /** Constructor. */
    EulerRect( TYPE *obj = 0, void (TYPE::*fun)(const Vector< SIZE > &, Vector< SIZE > &) = 0 ) :
        Integrator< SIZE, TYPE > ( obj, fun )
    {}

    /** Destructor. */
    virtual ~EulerRect() {}

    /**
     * Integrates given vector using
     * Euler's rectangular integration algorithm.
     * @param step integration time step [s]
     * @param vect integrating vector
     */
    void integrate( double step, Vector< SIZE > &vect )
    {
        _xt = vect;
        _k0 = Vector< SIZE >();

        // derivatives calculation
        this->fun( _xt, _k0 );

        // integration
        vect = vect + _k0 * step;
    }

private:

    Vector< SIZE > _k0;     ///< auxiliary vector
    Vector< SIZE > _xt;     ///< auxiliary vector

    /** Using this constructor is forbidden. */
    EulerRect( const EulerRect & ) {}
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_EULERRECT_H
