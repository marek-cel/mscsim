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

#include <fdm/utils/fdm_Period.h>

#include <algorithm>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Period::Period() :
    _arg_0 ( 0.0 ),

    _value_avg ( 0.0 ),
    _value_min ( std::numeric_limits< double >::max() ),
    _value_max ( std::numeric_limits< double >::min() ),

    _value_avg_min ( std::numeric_limits< double >::max() ),
    _value_avg_max ( std::numeric_limits< double >::min() ),

    _amplitude     ( 0.0 ),
    _amplitude_avg ( 0.0 ),
    _amplitude_min ( std::numeric_limits< double >::max() ),
    _amplitude_max ( std::numeric_limits< double >::min() ),

    _period     ( 0.0 ),
    _period_avg ( 0.0 ),
    _period_min ( std::numeric_limits< double >::max() ),
    _period_max ( std::numeric_limits< double >::min() ),

    _count ( 0 ),

    _even ( false )
{}

////////////////////////////////////////////////////////////////////////////////

Period::~Period() {}

////////////////////////////////////////////////////////////////////////////////

void Period::update( double arg, double val, double min )
{
    if ( _values.size() > 0 )
    {
        double val_last = *_values.end();

        if ( ( val > _value_avg && val_last < _value_avg )
          || ( val < _value_avg && val_last > _value_avg ) )
        {
            double val_min = *std::min_element( _values.begin(), _values.end() );
            double val_max = *std::max_element( _values.begin(), _values.end() );

            double amplitude = fabs( val_max - val_min );

            if ( amplitude > fabs( min ) )
            {
                if ( _even )
                {
                    _even = false;

                    double coef = 1.0 / ( (double)_count + 1.0 );

                    _value_avg_min = ( _value_avg_min * _count + val_min ) * coef;
                    _value_avg_max = ( _value_avg_max * _count + val_max ) * coef;

                    // amplitude
                    _amplitude = amplitude;

                    if ( _amplitude > _amplitude_max ) _amplitude_max = _amplitude;
                    if ( _amplitude < _amplitude_min ) _amplitude_min = _amplitude;

                    _amplitude_avg = ( _amplitude_avg * _count + _amplitude ) * coef;

                    // period
                    _period = arg - _arg_0;

                    if ( _period > _period_max ) _period_max = _period;
                    if ( _period < _period_min ) _period_min = _period;

                    _period_avg = ( _period_avg * _count + _period ) * coef;

                    // next
                    _arg_0 = arg;
                    _values.clear();
                    _count++;
                }
                else
                {
                    _even = true;
                }
            }
        }
    }

    if ( val > _value_max ) _value_max = val;
    if ( val < _value_min ) _value_min = val;

    _value_avg = 0.5 * ( _value_min + _value_max );

    _values.push_back( val );
}

////////////////////////////////////////////////////////////////////////////////

void Period::reset()
{
    _values.clear();

    _arg_0 = 0.0;

    _value_avg = 0.0;
    _value_min = std::numeric_limits< double >::max();
    _value_max = std::numeric_limits< double >::min();

    _value_avg_min = std::numeric_limits< double >::max();
    _value_avg_max = std::numeric_limits< double >::min();

    _amplitude     = 0.0;
    _amplitude_avg = 0.0;
    _amplitude_min = std::numeric_limits< double >::max();
    _amplitude_max = std::numeric_limits< double >::min();

    _period     = 0.0;
    _period_avg = 0.0;
    _period_min = std::numeric_limits< double >::max();
    _period_max = std::numeric_limits< double >::min();

    _count = 0;
}
