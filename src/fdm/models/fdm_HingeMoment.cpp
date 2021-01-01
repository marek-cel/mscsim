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

#include <fdm/models/fdm_HingeMoment.h>

#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

HingeMoment::HingeMoment() :
    _area  ( 0.0 ),
    _chord ( 0.0 ),
    _dch_dalpha   ( 0.0 ),
    _dch_ddelta   ( 0.0 ),
    _dch_ddelta_t ( 0.0 )
{}

////////////////////////////////////////////////////////////////////////////////

HingeMoment::~HingeMoment() {}

////////////////////////////////////////////////////////////////////////////////

void HingeMoment::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_area  , "area"  );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_chord , "chord" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_dch_dalpha   , "dch_dalpha"   );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_dch_ddelta   , "dch_ddelta"   );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_dch_ddelta_t , "dch_ddelta_t" );

        if ( result != FDM_SUCCESS ) XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
    else
    {
        XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
}

////////////////////////////////////////////////////////////////////////////////

double HingeMoment::getHingeMoment( double dynamicPress,
                                    double alpha,
                                    double delta,
                                    double delta_t ) const
{
    // control surface hinge moment coefficient
    double ch = _dch_dalpha   * alpha
              + _dch_ddelta   * delta
              + _dch_ddelta_t * delta_t;

    return dynamicPress * _area * _chord * ch;
}
