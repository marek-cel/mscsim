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

#include <fdm/utils/fdm_Geom.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

bool Geom::isIsect( const Vector3 &b, const Vector3 &e,
                    const Vector3 &r, const Vector3 &n )
{
    double num = n * ( r - b );
    double den = n * ( e - b );

    double u = 0.0;

    if ( fabs( den ) > 10e-14 ) u = num / den;

    if ( 0.0 < u && u < 1.0 )
    {
        return true;
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////

Vector3 Geom::getIsect( const Vector3 &b, const Vector3 &e,
                        const Vector3 &r, const Vector3 &n )
{
    Vector3 r_i = e;

    double num = n * ( r - b );
    double den = n * ( e - b );

    double u = 0.0;

    if ( fabs( den ) < 10e-15 )
    {
        // segment is parallel to the plane
        if ( fabs( num ) < 10e-15 )
        {
            // segment beginning is on the plane
            r_i = b;
        }
    }
    else
    {
        u = num / den;

        if ( 0.0 <= u && u <= 1.0 )
        {
            r_i = b + u * ( e - b );
        }
    }

    return r_i;
}
