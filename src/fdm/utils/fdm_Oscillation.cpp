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

#include <fdm/utils/fdm_Oscillation.h>

#include <algorithm>
#include <cfloat>
#include <cmath>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Oscillation::Oscillation() :
    _x_last ( -DBL_MAX ),

    _y_max ( 0.0 ),
    _y_min ( 0.0 ),

    _p_max ( 0.0 ),
    _p_min ( 0.0 ),
    _p_avg ( 0.0 ),

    _a_max ( 0.0 ),
    _a_min ( 0.0 ),
    _a_avg ( 0.0 )
{}

////////////////////////////////////////////////////////////////////////////////

Oscillation::~Oscillation() {}

////////////////////////////////////////////////////////////////////////////////

void Oscillation::add( double x, double y )
{
    if ( x > _x_last || _points.size() == 0 )
    {
        _x_last = x;

        Point pt;
        pt.x = x;
        pt.y = y;

        _points.push_back( pt );

        update();
    }
}

////////////////////////////////////////////////////////////////////////////////

void Oscillation::add( double *x, double *y, unsigned int n )
{
    size_t size_prev = _points.size();

    for ( unsigned int i = 0; i < n; i++ )
    {
        if ( x[ i ] > _x_last || _points.size() == 0 )
        {
            _x_last = x[ i ];

            Point pt;
            pt.x = x[ i ];
            pt.y = y[ i ];

            _points.push_back( pt );
        }
        else
        {
            break;
        }
    }

    if ( size_prev != _points.size() ) update();
}

////////////////////////////////////////////////////////////////////////////////

void Oscillation::update()
{
    if ( _points.size() > 0 )
    {
        const double y_0 = _points[ 0 ].y;

        double y = y_0;

        bool inc = false;
        bool dec = false;

        _y_max = y_0;
        _y_min = y_0;

        _p_max = -DBL_MAX;
        _p_min =  DBL_MAX;

        _a_max = -DBL_MAX;
        _a_min =  DBL_MAX;

        int ne = 0; // number of extremum
        int np = 0; // number of periods

        double xe0;

        double ye_prev;

        double as = 0.0;
        double ps = 0.0;

        for ( Points::iterator it = _points.begin(); it != _points.end(); it++ )
        {
            Point &pt = *(it);

            double y_prev = y;

            y = pt.y;

            bool inc_prev = inc;
            bool dec_prev = dec;

            if ( pt.y > _y_max ) _y_max = pt.y;
            if ( pt.y < _y_min ) _y_min = pt.y;

            inc = false;
            dec = false;

            if ( pt.y - y_prev > 0.0 ) inc = true;
            if ( pt.y - y_prev < 0.0 ) dec = true;

            if ( ( inc && dec_prev ) || ( dec && inc_prev ) )
            {
                if ( ne % 2 == 0 )
                {
                    if ( ne > 0 )
                    {
                        double p = pt.x - xe0;

                        if ( p > _p_max ) _p_max = p;
                        if ( p < _p_min ) _p_min = p;

                        ps += p;
                        np++;
                    }

                    xe0 = pt.x;
                }

                if ( ne > 0 )
                {
                    double a = fabs( pt.y - ye_prev );

                    if ( a > _a_max ) _a_max = a;
                    if ( a < _a_min ) _a_min = a;

                    as += a;
                }

                ye_prev = pt.y;

                ne++;
            }
        }

        if ( np > 0 )
        {
            _p_avg = ps / ( (double)(np) );
            _a_avg = as / ( (double)(ne - 1) );
        }
        else
        {
            _p_max = 0.0;
            _p_min = 0.0;
            _p_avg = 0.0;

            _a_max = 0.0;
            _a_min = 0.0;
            _a_avg = 0.0;
        }
    }
}
