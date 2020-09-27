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

#include <fdm_f35a/f35a_TailOff.h>

#include <fdm/main/fdm_Aerodynamics.h>
#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

F35A_TailOff::F35A_TailOff() :
    _ailerons ( 0.0 ),
    _airbrake ( 0.0 ),
    _flaps_le ( 0.0 ),
    _flaps_te ( 0.0 ),

    _dcl_dailerons ( 0.0 ),

    _dcx_dairbrake ( 0.0 ),
    _dcz_dairbrake ( 0.0 )
{
    _dcx_dflaps_te = Table1::createOneRecordTable( 0.0 );
    _dcz_dflaps_te = Table1::createOneRecordTable( 0.0 );
    _dcm_dflaps_te = Table1::createOneRecordTable( 0.0 );
}

////////////////////////////////////////////////////////////////////////////////

F35A_TailOff::~F35A_TailOff() {}

////////////////////////////////////////////////////////////////////////////////

void F35A_TailOff::readData( XmlNode &dataNode )
{
    //////////////////////////////
    TailOff::readData( dataNode );
    //////////////////////////////

    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _dcl_dailerons, "dcl_dailerons" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _dcx_dairbrake, "dcx_dairbrake" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _dcz_dairbrake, "dcz_dairbrake" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _dcx_dflaps_te, "dcx_dflaps_te"  );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _dcz_dflaps_te, "dcz_dflaps_te"  );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _dcm_dflaps_te, "dcm_dflaps_te" );

        if ( result != FDM_SUCCESS ) XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
    else
    {
        XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
}

////////////////////////////////////////////////////////////////////////////////

void F35A_TailOff::computeForceAndMoment( const Vector3 &vel_air_bas,
                                          const Vector3 &omg_air_bas,
                                          double airDensity,
                                          double ailerons,
                                          double airbrake,
                                          double flaps_le,
                                          double flaps_te )
{
    _ailerons = ailerons;
    _airbrake = airbrake;
    _flaps_le = flaps_le;
    _flaps_te = flaps_te;

    ///////////////////////////////////////////////////////////////////////
    TailOff::computeForceAndMoment( vel_air_bas, omg_air_bas, airDensity );
    ///////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////

void F35A_TailOff::update( const Vector3 &vel_air_bas, const Vector3 &omg_air_bas )
{
    ////////////////////////////////////////////
    TailOff::update( vel_air_bas, omg_air_bas );
    ////////////////////////////////////////////

    Table1 cz_total = _cz + _flaps_te * _dcz_dflaps_te;

    _aoa_critical_neg = cz_total.getKeyOfValueMin();
    _aoa_critical_pos = cz_total.getKeyOfValueMax();
}

////////////////////////////////////////////////////////////////////////////////

double F35A_TailOff::getCx( double angleOfAttack ) const
{
    return TailOff::getCx( angleOfAttack )
            + _airbrake * _dcx_dairbrake
            + _flaps_te * _dcx_dflaps_te.getValue( angleOfAttack );
}

////////////////////////////////////////////////////////////////////////////////

double F35A_TailOff::getCy( double sideslipAngle ) const
{
    return TailOff::getCy( sideslipAngle );
}

////////////////////////////////////////////////////////////////////////////////

double F35A_TailOff::getCz( double angleOfAttack ) const
{
    return TailOff::getCz( angleOfAttack )
            + _airbrake * _dcz_dairbrake
            + _flaps_te * _dcz_dflaps_te.getValue( angleOfAttack );
}

////////////////////////////////////////////////////////////////////////////////

double F35A_TailOff::getCl( double sideslipAngle ) const
{
    return TailOff::getCl( sideslipAngle )
            + _ailerons * _dcl_dailerons;
}

////////////////////////////////////////////////////////////////////////////////

double F35A_TailOff::getCm( double angleOfAttack ) const
{
    return TailOff::getCm( angleOfAttack )
            + _flaps_te * _dcm_dflaps_te.getValue( angleOfAttack );
}

////////////////////////////////////////////////////////////////////////////////

double F35A_TailOff::getCn( double sideslipAngle ) const
{
    return TailOff::getCn( sideslipAngle );
}
