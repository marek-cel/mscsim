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

#include <gui/ComboUnits.h>

#include <limits>

////////////////////////////////////////////////////////////////////////////////

ComboUnits::ComboUnits( QWidget *parent ) :
    QComboBox ( parent ),

    _factor ( 1.0f ),

    _index      ( currentIndex() ),
    _index_prev ( _index )
{
    _coefs.clear();
    _names.clear();

    connect( this, SIGNAL(currentIndexChanged(int)), this, SLOT(on_currentIndexChanged(int)) );
}

////////////////////////////////////////////////////////////////////////////////

ComboUnits::~ComboUnits() {}

////////////////////////////////////////////////////////////////////////////////

float ComboUnits::convert( float value ) const
{
    return value * _factor;
}

////////////////////////////////////////////////////////////////////////////////

float ComboUnits::convertPrev( float value ) const
{
    return value * _factor_prev;
}

////////////////////////////////////////////////////////////////////////////////

float ComboUnits::invert( float value ) const
{
    return value / _factor;
}

////////////////////////////////////////////////////////////////////////////////

float ComboUnits::invertPrev( float value ) const
{
    return value / _factor_prev;
}

////////////////////////////////////////////////////////////////////////////////

float ComboUnits::getCoef( int index ) const
{
    if ( index >= 0 && index < (int)_coefs.size() )
    {
        return _coefs[ index ];
    }

    return std::numeric_limits< float >::quiet_NaN();
}

////////////////////////////////////////////////////////////////////////////////

void ComboUnits::on_currentIndexChanged( int index )
{
    _factor_prev = _factor;

    _index_prev = _index;
    _index = index;

    if ( index >= 0 && index < (int)_coefs.size() )
    {
        _factor = _coefs[ index ];
    }
}
