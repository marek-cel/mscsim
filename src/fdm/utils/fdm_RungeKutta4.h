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
template < unsigned int SIZE, class TYPE >
class RungeKutta4 : public Integrator< SIZE, TYPE >
{
public:

    /** Constructor. */
    RungeKutta4( TYPE *obj = 0, void (TYPE::*fun)(const Vector< SIZE > &, Vector< SIZE > &) = 0 ) :
        Integrator< SIZE, TYPE > ( obj, fun )
    {}

    /** Destructor. */
    virtual ~RungeKutta4() {}

    /**
     * Integrates given vector using
     * Runge-Kutta 4th order integration algorithm.
     * @param step integration time step [s]
     * @param vect integrating vector
     */
    void integrate( double step, Vector< SIZE > &vect )
    {
        m_x_t = vect;
        m_k_1 = m_k_2 = m_k_3 = m_k_4 = Vector< SIZE >();

        // k1 - derivatives calculation
        this->fun( m_x_t, m_k_1 );

        // k2 - derivatives calculation
        m_x_t = vect + m_k_1 * ( step / 2.0 );
        this->fun( m_x_t, m_k_2 );

        // k3 - derivatives calculation
        m_x_t = vect + m_k_2 * ( step / 2.0 );
        this->fun( m_x_t, m_k_3 );

        // k4 - derivatives calculation
        m_x_t = vect + m_k_3 * step;
        this->fun( m_x_t, m_k_4 );

        // integration
        vect = vect + ( m_k_1 + m_k_2 * 2.0 + m_k_3 * 2.0 + m_k_4 ) * ( step / 6.0 );
    }

private:

    Vector< SIZE > m_k_1;   ///< auxiliary vector
    Vector< SIZE > m_k_2;   ///< auxiliary vector
    Vector< SIZE > m_k_3;   ///< auxiliary vector
    Vector< SIZE > m_k_4;   ///< auxiliary vector

    Vector< SIZE > m_x_t;   ///< auxiliary vector

    /** Using this constructor is forbidden. */
    RungeKutta4( const RungeKutta4 & ) {}
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_RUNGEKUTTA4_H
