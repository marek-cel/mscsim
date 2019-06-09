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
#ifndef FDM_LEADLAG_H
#define FDM_LEADLAG_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_Defines.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Lead-lag compensator class.
 * Transfer function: G(s) = ( c1*s + c2 ) / ( c3*s + c4 )
 */
class FDMEXPORT LeadLag
{
public:

    LeadLag();

    LeadLag( double c1, double c2, double c3, double c4, double y = 0.0 );

    inline double getValue() const { return m_y;  }

    inline double getC1() const { return m_c1; }
    inline double getC2() const { return m_c2; }
    inline double getC3() const { return m_c3; }
    inline double getC4() const { return m_c4; }

    void setValue( double y );

    void setC1( double c1 );
    void setC2( double c2 );
    void setC3( double c3 );
    void setC4( double c4 );

    void update( double u, double dt );

private:

    double m_c1;        ///< c1 coefficient
    double m_c2;        ///< c2 coefficient
    double m_c3;        ///< c3 coefficient
    double m_c4;        ///< c4 coefficient

    double m_u_prev;    ///<
    double m_y_prev;    ///<

    double m_y;         ///< current value
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_LEADLAG_H
