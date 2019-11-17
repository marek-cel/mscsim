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

#include <fdm/models/fdm_WindShear.h>

#include <cmath>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Vector3 WindShear::getWindModel1( double distance )
{
    Vector3 result( 0.0, 0.0, 0.0 );

    if ( distance < 1371.6 )        // 4,500 ft
    {
        // 46 kts = 23.66 m/s
        double grad = 23.66 / 1371.6;
        result.x() = distance * grad;
    }
    else if ( distance < 2286.0 )   // 7,500 ft
    {
        // 46 kts = 23.66 m/s
        result.x() = 23.66;
    }
    else if ( distance < 3657.6 )   // 12,000 ft
    {
        // 46 kts = 23.66 m/s
        double grad = 23.66 / ( 3657.6 - 2286.0 );
        result.x() = 23.66 - ( distance - 2286.0 ) * grad;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

Vector3 WindShear::getWindModel2( double distance )
{
    Vector3 result( 0.0, 0.0, 0.0 );

    if ( distance < 2133.6 )        // 7,000 ft
    {
        // 60 kts = 30.87 m/s
        double grad = 30.87 / 2133.6;
        result.x() = distance * grad;
    }
    else if ( distance < 3413.76 )  // 11,200 ft
    {
        // 60 kts = 30.87 m/s
        result.x() = 30.87;
    }
    else if ( distance < 5120.64 )  // 16,800 ft
    {
        // 60 kts = 30.87 m/s
        double grad = 30.87 / ( 5120.64 - 3413.76 );
        result.x() = 30.87 - ( distance - 3413.76 ) * grad;
    }

    if ( distance < 640.08 )        // 2,100 ft
    {
        // 18 kts = 9.26 m/s
        double grad = 9.26 / 640.08;
        result.z() = distance * grad;
    }
    else if ( distance < 1371.6 )   // 4,500 ft
    {
        // 18 kts = 9.26 m/s
        result.z() = 9.26;
    }
    else if ( distance < 2133.6 )   // 7,000 ft
    {
        // 18 kts = 9.26 m/s
        double grad = 9.26 / ( 2133.6 - 1371.6 );
        result.z() = 9.26 - ( distance - 1371.6 ) * grad;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

Vector3 WindShear::getWindModel3( double distance )
{
    Vector3 result( 0.0, 0.0, 0.0 );

    if ( distance < 1524.0 )        // 5,000 ft
    {
        // 52 kts = 26.75 m/s
        double grad = 26.75 / 1524.0;
        result.x() = distance * grad;
    }
    else if ( distance < 2438.4 )  // 8,000 ft
    {
        // 52 kts = 26.75 m/s
        result.x() = 26.75;
    }
    else if ( distance < 3901.44 )  // 12,800 ft
    {
        // 52 kts = 26.75 m/s
        double grad = 26.75 / ( 3901.44 - 2438.4 );
        result.x() = 26.75 - ( distance - 2438.4 ) * grad;
    }

    // TODO

    return result;
}

////////////////////////////////////////////////////////////////////////////////

Vector3 WindShear::getWindModel4( double distance )
{
    Vector3 result( 0.0, 0.0, 0.0 );
    // TODO
    return result;
}
