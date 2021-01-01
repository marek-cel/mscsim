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

#include <fdm_xh/xh_Fuselage.h>

#include <fdm/utils/fdm_Units.h>
#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

XH_Fuselage::XH_Fuselage()
{
    _cx_beta = Table1::oneRecordTable( 0.0 );
    _cz_beta = Table1::oneRecordTable( 0.0 );
    _cm_beta = Table1::oneRecordTable( 0.0 );
}

////////////////////////////////////////////////////////////////////////////////

XH_Fuselage::~XH_Fuselage() {}

////////////////////////////////////////////////////////////////////////////////

void XH_Fuselage::readData( XmlNode &dataNode )
{
    ///////////////////////////////
    Fuselage::readData( dataNode );
    ///////////////////////////////

    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_cx_beta, "cx_beta" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_cz_beta, "cz_beta" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_cm_beta, "cm_beta" );

        if ( result == FDM_SUCCESS )
        {
            _cx_beta.multiplyKeys( Units::deg2rad() );
            _cz_beta.multiplyKeys( Units::deg2rad() );
            _cm_beta.multiplyKeys( Units::deg2rad() );
        }
        else
        {
            XmlUtils::throwError( __FILE__, __LINE__, dataNode );
        }
    }
    else
    {
        XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
}

////////////////////////////////////////////////////////////////////////////////

double XH_Fuselage::getCx( double angleOfAttack ) const
{
    return Fuselage::getCx( angleOfAttack ) + _cx_beta.getValue( _sideslipAngle );
}

////////////////////////////////////////////////////////////////////////////////

double XH_Fuselage::getCz( double angleOfAttack ) const
{
    return Fuselage::getCz( angleOfAttack ) + _cz_beta.getValue( _sideslipAngle );
}

////////////////////////////////////////////////////////////////////////////////

double XH_Fuselage::getCm( double angleOfAttack ) const
{
    return Fuselage::getCm( angleOfAttack ) + _cm_beta.getValue( _sideslipAngle );
}
