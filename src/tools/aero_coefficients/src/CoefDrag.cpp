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
#include "CoefDrag.h"

#include <math.h>

////////////////////////////////////////////////////////////////////////////////

CoefDrag::CoefDrag( double cd_0, double cd_1, double cd_2, double cd_3, double cd_4, double cd_5,
                    double ad_1, double ad_2, double ad_3, double ad_4 )
{
    m_cd_0 = cd_0;
    m_cd_1 = cd_1;
    m_cd_2 = cd_2;
    m_cd_3 = cd_3;
    m_cd_4 = cd_4;
    m_cd_5 = cd_5;

    m_ad_1 = ad_1;
    m_ad_2 = ad_2;
    m_ad_3 = ad_3;
    m_ad_4 = ad_4;

    m_ad_1_2 = m_ad_1 * m_ad_1;
    m_ad_2_2 = m_ad_2 * m_ad_2;

    m_div_d1_0 = m_ad_1_2 * m_ad_2_2;
    m_div_d1_1 = ( m_ad_1_2 - m_ad_2_2 ) * m_ad_1_2;
    m_div_d1_2 = ( m_ad_2_2 - m_ad_1_2 ) * m_ad_2_2;

    m_div_d2_2 = ( m_ad_2 - m_ad_3 )*( m_ad_2 - m_ad_4 )*( m_ad_2 - M_PI_2 );
    m_div_d2_3 = ( m_ad_3 - m_ad_2 )*( m_ad_3 - m_ad_4 )*( m_ad_3 - M_PI_2 );
    m_div_d2_4 = ( m_ad_4 - m_ad_2 )*( m_ad_4 - m_ad_3 )*( m_ad_4 - M_PI_2 );
    m_div_d2_5 = ( M_PI_2 - m_ad_2 )*( M_PI_2 - m_ad_3 )*( M_PI_2 - m_ad_4 );
}

////////////////////////////////////////////////////////////////////////////////

double CoefDrag::get( double aoa )
{
    if ( aoa < -M_PI_2 ) aoa = -M_PI - aoa;
    if ( aoa >  M_PI_2 ) aoa =  M_PI - aoa;

    if ( -m_ad_2 <= aoa && aoa <= m_ad_2 )
    {
        double az_2 = aoa*aoa;

        double az2_sub_ad12 = az_2 - m_ad_1_2;
        double az2_sub_ad22 = az_2 - m_ad_2_2;

        return m_cd_0 * az2_sub_ad22 * az2_sub_ad12 / m_div_d1_0
             + m_cd_1 * az2_sub_ad22 * az_2 / m_div_d1_1
             + m_cd_2 * az2_sub_ad12 * az_2 / m_div_d1_2;
    }
    else
    {
        double az = fabs( aoa );

        double az_sub_ad2 = az - m_ad_2;
        double az_sub_ad3 = az - m_ad_3;
        double az_sub_ad4 = az - m_ad_4;
        double az_sub_pi2 = az - M_PI_2;

        return m_cd_2 * az_sub_ad3 * az_sub_ad4 * az_sub_pi2 / m_div_d2_2
             + m_cd_3 * az_sub_ad2 * az_sub_ad4 * az_sub_pi2 / m_div_d2_3
             + m_cd_4 * az_sub_ad2 * az_sub_ad3 * az_sub_pi2 / m_div_d2_4
             + m_cd_5 * az_sub_ad2 * az_sub_ad3 * az_sub_ad4 / m_div_d2_5;
    }

    return m_cd_0;
}
