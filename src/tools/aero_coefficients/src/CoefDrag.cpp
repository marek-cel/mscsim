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
    _cd_0 = cd_0;
    _cd_1 = cd_1;
    _cd_2 = cd_2;
    _cd_3 = cd_3;
    _cd_4 = cd_4;
    _cd_5 = cd_5;

    _ad_1 = ad_1;
    _ad_2 = ad_2;
    _ad_3 = ad_3;
    _ad_4 = ad_4;

    _ad_1_2 = _ad_1 * _ad_1;
    _ad_2_2 = _ad_2 * _ad_2;

    _div_d1_0 = _ad_1_2 * _ad_2_2;
    _div_d1_1 = ( _ad_1_2 - _ad_2_2 ) * _ad_1_2;
    _div_d1_2 = ( _ad_2_2 - _ad_1_2 ) * _ad_2_2;

    _div_d2_2 = ( _ad_2  - _ad_3 )*( _ad_2  - _ad_4 )*( _ad_2  - M_PI_2 );
    _div_d2_3 = ( _ad_3  - _ad_2 )*( _ad_3  - _ad_4 )*( _ad_3  - M_PI_2 );
    _div_d2_4 = ( _ad_4  - _ad_2 )*( _ad_4  - _ad_3 )*( _ad_4  - M_PI_2 );
    _div_d2_5 = ( M_PI_2 - _ad_2 )*( M_PI_2 - _ad_3 )*( M_PI_2 - _ad_4  );
}

////////////////////////////////////////////////////////////////////////////////

double CoefDrag::get( double aoa )
{
    if ( aoa < -M_PI_2 ) aoa = -M_PI - aoa;
    if ( aoa >  M_PI_2 ) aoa =  M_PI - aoa;

    if ( -_ad_2 <= aoa && aoa <= _ad_2 )
    {
        double az_2 = aoa*aoa;

        double az2_sub_ad12 = az_2 - _ad_1_2;
        double az2_sub_ad22 = az_2 - _ad_2_2;

        return _cd_0 * az2_sub_ad22 * az2_sub_ad12 / _div_d1_0
             + _cd_1 * az2_sub_ad22 * az_2 / _div_d1_1
             + _cd_2 * az2_sub_ad12 * az_2 / _div_d1_2;
    }
    else
    {
        double az = fabs( aoa );

        double az_sub_ad2 = az - _ad_2;
        double az_sub_ad3 = az - _ad_3;
        double az_sub_ad4 = az - _ad_4;
        double az_sub_pi2 = az - M_PI_2;

        return _cd_2 * az_sub_ad3 * az_sub_ad4 * az_sub_pi2 / _div_d2_2
             + _cd_3 * az_sub_ad2 * az_sub_ad4 * az_sub_pi2 / _div_d2_3
             + _cd_4 * az_sub_ad2 * az_sub_ad3 * az_sub_pi2 / _div_d2_4
             + _cd_5 * az_sub_ad2 * az_sub_ad3 * az_sub_ad4 / _div_d2_5;
    }

    return _cd_0;
}
