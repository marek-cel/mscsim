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

#include <gui/ComboUnitsTemperature.h>

#include <fdm/utils/fdm_Units.h>

////////////////////////////////////////////////////////////////////////////////

ComboUnitsTemperature::ComboUnitsTemperature( QWidget *parent ) :
    ComboUnits ( parent )
{
    _coefs.push_back( 1.0 );
    _names.push_back( QString( tr("K") ) );

    _coefs.push_back( 1.0 );
    _names.push_back( QString( tr("deg C") ) );

    _coefs.push_back( 1.0 );
    _names.push_back( QString( tr("deg F") ) );

    for ( size_t i = 0; i < _names.size(); i++ ) addItem( _names[ i ] );
}

////////////////////////////////////////////////////////////////////////////////
    
ComboUnitsTemperature::~ComboUnitsTemperature() {}

////////////////////////////////////////////////////////////////////////////////

double ComboUnitsTemperature::convert( double value ) const
{
    double result = value;

    switch ( _index )
    {
        case 1: result = fdm::Units::k2c( value ); break;
        case 2: result = fdm::Units::k2f( value ); break;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

double ComboUnitsTemperature::convertPrev( double value ) const
{
    double result = value;

    switch ( _index_prev )
    {
        case 1: result = fdm::Units::k2c( value ); break;
        case 2: result = fdm::Units::k2f( value ); break;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

double ComboUnitsTemperature::invert( double value ) const
{
    double result = value;

    switch ( _index )
    {
        case 1: result = fdm::Units::c2k( value ); break;
        case 2: result = fdm::Units::f2k( value ); break;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

double ComboUnitsTemperature::invertPrev( double value ) const
{
    double result = value;

    switch ( _index_prev )
    {
        case 1: result = fdm::Units::c2k( value ); break;
        case 2: result = fdm::Units::f2k( value ); break;
    }

    return result;
}
