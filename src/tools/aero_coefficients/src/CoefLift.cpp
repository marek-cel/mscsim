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
#include "CoefLift.h"

#include <iostream>
#include <math.h>

////////////////////////////////////////////////////////////////////////////////

CoefLift::CoefLift( double cl_s, double cl_1, double cl_2,
                    double al_s, double al_1, double al_2 )
{
    m_cl_0 = 0.0;
    m_cl_s = cl_s;
    m_cl_1 = cl_1;
    m_cl_2 = cl_2;

    m_al_s = al_s;
    m_al_1 = al_1;
    m_al_2 = al_2;

    m_dcl_da_s = m_cl_s / m_al_s;
    m_dcl_da_1 = ( m_cl_1 - m_cl_s ) / ( m_al_1 - m_al_s );

    m_div_l1_1 = ( m_al_1 - m_al_2 ) * ( m_al_1 - M_PI_2 );
    m_div_l1_2 = ( m_al_2 - m_al_1 ) * ( m_al_2 - M_PI_2 );
}

////////////////////////////////////////////////////////////////////////////////

double CoefLift::get( double aoa )
{
    double coef = ( aoa < -M_PI_2 || aoa > M_PI_2 ) ? -1.0 : 1.0;

    if ( aoa < -M_PI_2 ) aoa = -M_PI - aoa;
    if ( aoa >  M_PI_2 ) aoa =  M_PI - aoa;

    double cl = m_cl_0;

    if ( aoa < -m_al_1 )
    {
        cl =  - m_cl_1 * ( aoa + m_al_2 ) * ( aoa + M_PI_2 ) / m_div_l1_1
              - m_cl_2 * ( aoa + m_al_1 ) * ( aoa + M_PI_2 ) / m_div_l1_2
              + m_cl_0;
    }
    else if ( aoa < -m_al_s )
    {
        cl =  m_dcl_da_1 * ( aoa + m_al_s ) - m_cl_s + m_cl_0;
    }
    else if ( aoa < m_al_s )
    {
        cl =  m_dcl_da_s * aoa + m_cl_0;
    }
    else if ( aoa < m_al_1 )
    {
        cl =  m_dcl_da_1 * ( aoa - m_al_s ) + m_cl_s + m_cl_0;
    }
    else
    {
        cl =  m_cl_1 * ( aoa - m_al_2 ) * ( aoa - M_PI_2 ) / m_div_l1_1
            + m_cl_2 * ( aoa - m_al_1 ) * ( aoa - M_PI_2 ) / m_div_l1_2
            + m_cl_0;
    }

    return coef * cl;
}
