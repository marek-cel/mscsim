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

#include <gui/ComboUnitsVelocity.h>

#include <fdm/utils/fdm_Units.h>

////////////////////////////////////////////////////////////////////////////////

ComboUnitsVelocity::ComboUnitsVelocity( QWidget *parent ) :
    ComboUnits ( parent )
{
    m_coefs.push_back( 1.0f );
    m_names.push_back( QString( "m/s" ) );

    m_coefs.push_back( (float)fdm::Units::mps2fpm() );
    m_names.push_back( QString( "ft/min" ) );

    m_coefs.push_back( (float)fdm::Units::mps2fps() );
    m_names.push_back( QString( "ft/s" ) );

    m_coefs.push_back( (float)fdm::Units::mps2kmh() );
    m_names.push_back( QString( "km/h" ) );

    m_coefs.push_back( (float)fdm::Units::mps2kts() );
    m_names.push_back( QString( "kts" ) );

    m_coefs.push_back( (float)fdm::Units::mps2mph() );
    m_names.push_back( QString( "mph" ) );

    for ( size_t i = 0; i < m_names.size(); i++ ) addItem( m_names[ i ] );
}

////////////////////////////////////////////////////////////////////////////////
    
ComboUnitsVelocity::~ComboUnitsVelocity() {}
