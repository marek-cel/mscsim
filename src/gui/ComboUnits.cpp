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

////////////////////////////////////////////////////////////////////////////////

ComboUnits::ComboUnits( QWidget *parent ) :
    QComboBox ( parent ),

    m_factor ( 1.0f ),

    m_index      ( currentIndex() ),
    m_index_prev ( m_index )
{
    m_coefs.clear();
    m_names.clear();

    connect( this, SIGNAL(currentIndexChanged(int)), this, SLOT(on_currentIndexChanged(int)) );
}

////////////////////////////////////////////////////////////////////////////////

ComboUnits::~ComboUnits() {}

////////////////////////////////////////////////////////////////////////////////

float ComboUnits::convert( float value )
{
    return value * m_factor;
}

////////////////////////////////////////////////////////////////////////////////

float ComboUnits::convertPrev( float value )
{
    return value * m_factor_prev;
}

////////////////////////////////////////////////////////////////////////////////

float ComboUnits::invert( float value )
{
    return value / m_factor;
}

////////////////////////////////////////////////////////////////////////////////

float ComboUnits::invertPrev( float value )
{
    return value / m_factor_prev;
}

////////////////////////////////////////////////////////////////////////////////

void ComboUnits::on_currentIndexChanged( int index )
{
    m_factor_prev = m_factor;

    m_index_prev = m_index;
    m_index = index;

    if ( index >= 0 && index < (int)m_coefs.size() )
    {
        m_factor = m_coefs[ index ];
    }
}
