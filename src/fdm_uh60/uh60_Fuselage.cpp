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

#include <fdm_uh60/uh60_Fuselage.h>

#include <fdm/utils/fdm_Units.h>
#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

UH60_Fuselage::UH60_Fuselage()
{
    _dqfmp = Table1::oneRecordTable( 0.0 );
    _lqfmp = Table1::oneRecordTable( 0.0 );
    _mqfmp = Table1::oneRecordTable( 0.0 );

    _yqfmp = Table1::oneRecordTable( 0.0 );
    _rqfmp = Table1::oneRecordTable( 0.0 );
    _nqfmp = Table1::oneRecordTable( 0.0 );

    _ddqfmp = Table1::oneRecordTable( 0.0 );
    _dlqfmp = Table1::oneRecordTable( 0.0 );
    _dmqfmp = Table1::oneRecordTable( 0.0 );
}

////////////////////////////////////////////////////////////////////////////////

UH60_Fuselage::~UH60_Fuselage() {}

////////////////////////////////////////////////////////////////////////////////

void UH60_Fuselage::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_r_ac_bas, "aero_center" );

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_ekxwf, "ekxwf" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_ekzwf, "ekzwf" );

        //if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_dqfmp, "dqfmp" );
        //if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_lqfmp, "lqfmp" );
        //if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_mqfmp, "mqfmp" );
        //
        //if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_yqfmp, "yqfmp" );
        //if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_rqfmp, "rqfmp" );
        //if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_nqfmp, "nqfmp" );
        //
        //if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_ddqfmp, "ddqfmp" );
        //if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_dlqfmp, "dlqfmp" );
        //if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, &_dmqfmp, "dmqfmp" );

        if ( result == FDM_SUCCESS )
        {
            _ekxwf.multiplyColsAndRows( Units::deg2rad(), Units::deg2rad() );
            _ekzwf.multiplyColsAndRows( Units::deg2rad(), Units::deg2rad() );

            _dqfmp.multiplyKeys( Units::deg2rad() );
            _lqfmp.multiplyKeys( Units::deg2rad() );
            _mqfmp.multiplyKeys( Units::deg2rad() );

            _yqfmp.multiplyKeys( Units::deg2rad() );
            _rqfmp.multiplyKeys( Units::deg2rad() );
            _nqfmp.multiplyKeys( Units::deg2rad() );

            _ddqfmp.multiplyKeys( Units::deg2rad() );
            _dlqfmp.multiplyKeys( Units::deg2rad() );
            _dmqfmp.multiplyKeys( Units::deg2rad() );
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

void UH60_Fuselage::computeForceAndMoment( const Vector3 &vel_air_bas,
                                           const Vector3 &omg_air_bas,
                                           double airDensity,
                                           double a1fmr,
                                           double chipmr,
                                           double dwshmr,
                                           double omgtmr,
                                           double rmr )
{
    // rotor wash on the fuselage, NASA-CR-166309, p.5.2-5 (PDF p.90)
    Vector3 vel_i_bas(  _ekxwf.getValue( chipmr, a1fmr ) * dwshmr * omgtmr * rmr,
                        0.0,
                       -_ekzwf.getValue( chipmr, a1fmr ) * dwshmr * omgtmr * rmr );

    // fuselage velocity
    // NASA-CR-166309, p.5.2-5 (PDF p.90)
    Vector3 vel_f_bas = vel_air_bas
                      + vel_i_bas
    // "Contributions from angular rates due to mounting point offset from CG
    // are ignored." [NASA-CR-166309, p.5.2-5 (PDF p.90)]
                      + ( omg_air_bas % _r_ac_bas )
    ;

    // NASA-CR-166309, p.5.2-6 (PDF p.91)
    double vxabs = fabs( vel_f_bas.x() );
    double alfwf = ( vxabs > 0.1 ) ? atan2( vel_f_bas.z(), vxabs ) : 0.0;
    double afabwf = fabs( alfwf );

    // NASA-CR-166309, p.5.2-6 (PDF p.91)
    double v_xz =  vel_f_bas.getLengthXZ();
    double betawf = ( v_xz > 0.1 ) ? atan2( vel_f_bas.y(), v_xz ) : 0.0;

    // NASA-CR-166309, p.5.2-12 (PDF p.97)
    double psiwf = -betawf;

    // dynamic pressure
    // NASA-CR-166309, p.5.2-6 (PDF p.91)
    double qwf = 0.5 * airDensity * vel_f_bas.getLength2();

    // NASA-CR-166309, p.5.2-9 (PDF p.94)
    Vector3 for_temp( -qwf * getDQFTOT( alfwf, psiwf ),
                      -qwf * getYQFTOT( psiwf ),
                      -qwf * getLQFTOT( alfwf, psiwf ) );

    Vector3 mom_temp(  qwf * getRQFTOT( psiwf ),
                      -qwf * getMQFTOT( alfwf, psiwf ),
                       qwf * getNQFTOT( psiwf ) );

    double sinAlpha = sin( alfwf );
    double cosAlpha = cos( alfwf );
    double sinBeta  = sin( betawf );
    double cosBeta  = cos( betawf );

    Matrix3x3 T_tot2wfp;

    T_tot2wfp(0,0) =  cosAlpha * cosBeta;
    T_tot2wfp(0,1) =  cosAlpha * sinBeta;
    T_tot2wfp(0,2) = -sinAlpha;

    T_tot2wfp(1,0) =  sinBeta;
    T_tot2wfp(1,1) = -cosBeta;
    T_tot2wfp(1,2) =  0.0;

    T_tot2wfp(2,0) =  sinAlpha * cosBeta;
    T_tot2wfp(2,1) =  sinAlpha * sinBeta;
    T_tot2wfp(2,2) =  cosAlpha;

    // NASA-CR-166309, p.5.2-9
    Vector3 for_bas = T_tot2wfp * for_temp;
    Vector3 mom_bas = T_tot2wfp * mom_temp
                    + ( _r_ac_bas % for_bas );

    // low speed filter of fuselage aerodynamics
    // NASA-CR-166309, p.5.2-10 (PDF p.95)
    // 25 ft/s = ca. 7.62 m/s
    if ( vxabs <= 7.62 )
    {
        double coef = vxabs / 7.62;

        double al_ala = ( afabwf > 1.0e-9 ) ? ( alfwf / afabwf ) : 0.0;

        double vy_abs = fabs( vel_f_bas.y() );
        double vy_vya = ( vy_abs > 1.0e-9 ) ? ( vel_f_bas.y() / vy_abs ) : 0.0;

        Vector3 for_ls = for_temp; // ??? not sure !!! NASA-CR-166309, p.5.2-3 (PDF p.88)
        Vector3 mom_ls = mom_temp; // ??? not sure !!! NASA-CR-166309, p.5.2-3 (PDF p.88)

        for_bas.x() = coef * for_bas.x() - al_ala * ( 1.0 - coef ) * for_ls.x();
        for_bas.y() = coef * for_bas.y() - vy_vya * ( 1.0 - coef ) * for_ls.y();
        for_bas.z() = coef * for_bas.z() - al_ala * ( 1.0 - coef ) * for_ls.z();

        mom_bas.x() = coef * mom_bas.x() - vy_vya * ( 1.0 - coef ) * mom_ls.x();
        mom_bas.y() = coef * mom_bas.y() - al_ala * ( 1.0 - coef ) * mom_ls.y();
        mom_bas.z() = coef * mom_bas.z() - vy_vya * ( 1.0 - coef ) * mom_ls.z();
    }

    _for_bas = for_bas;
    _mom_bas = mom_bas;

    if ( !_for_bas.isValid() || !_mom_bas.isValid() )
    {
        Exception e;

        e.setType( Exception::UnexpectedNaN );
        e.setInfo( "NaN detected in the wing model." );

        FDM_THROW( e );
    }
}

////////////////////////////////////////////////////////////////////////////////

double UH60_Fuselage::getDQFTOT( double alfwf, double psiwf ) const
{
    // NASA-CR-166309, p.5.2-7 (PDF p.92)
    return _dqfmp.getValue( alfwf ) + _ddqfmp.getValue( psiwf );
}

////////////////////////////////////////////////////////////////////////////////

double UH60_Fuselage::getYQFTOT( double psiwf ) const
{
    // NASA-CR-166309, p.5.2-7 (PDF p.92)
    return _yqfmp.getValue( psiwf );
}

////////////////////////////////////////////////////////////////////////////////

double UH60_Fuselage::getLQFTOT( double alfwf, double psiwf ) const
{
    // NASA-CR-166309, p.5.2-7 (PDF p.92)
    return _lqfmp.getValue( alfwf ) + _dlqfmp.getValue( psiwf );
    // + DLQD ???
}

////////////////////////////////////////////////////////////////////////////////

double UH60_Fuselage::getRQFTOT( double psiwf ) const
{
    // NASA-CR-166309, p.5.2-7 (PDF p.92)
    return _rqfmp.getValue( psiwf );
}

////////////////////////////////////////////////////////////////////////////////

double UH60_Fuselage::getMQFTOT( double alfwf, double psiwf ) const
{
    // NASA-CR-166309, p.5.2-7 (PDF p.92)
    return _mqfmp.getValue( alfwf ) + _dmqfmp.getValue( psiwf );
    // + DMQD ???
}

////////////////////////////////////////////////////////////////////////////////

double UH60_Fuselage::getNQFTOT( double psiwf ) const
{
    // NASA-CR-166309, p.5.2-7 (PDF p.92)
    return _nqfmp.getValue( psiwf );
    // + DNQD ???
}
