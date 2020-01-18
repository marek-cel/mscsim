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

#include <fdm/main/fdm_Aerodynamics.h>
#include <fdm/main/fdm_Aircraft.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

double Aerodynamics::getAngleOfAttack( const Vector3 &vel_bas, double vel_min )
{
    double uv = sqrt( vel_bas( _iu )*vel_bas( _iu ) + vel_bas( _iv )*vel_bas( _iv ) );

    return getAngleOfAttack( uv, vel_bas( _iw ), vel_min );
}

////////////////////////////////////////////////////////////////////////////////

double Aerodynamics::getAngleOfAttack( double uv, double w, double vel_min )
{
    double angleOfAttack = 0.0;

    if ( fabs( uv ) > vel_min || fabs( w ) > vel_min )
    {
        angleOfAttack = atan2( w, uv );
    }

    return angleOfAttack;
}

////////////////////////////////////////////////////////////////////////////////

double Aerodynamics::getSideslipAngle( const Vector3 &vel_bas, double vel_min )
{
    double sideslipAngle = 0.0;

    if ( fabs( vel_bas( _iu ) ) > vel_min || fabs( vel_bas( _iv ) ) > vel_min )
    {
        double vw = vel_bas.getLength();
        double v_vw = ( vw > vel_min ) ? ( vel_bas( _iv ) / vw ) : 0.0;

        if ( v_vw >  1.0 ) v_vw =  1.0;
        if ( v_vw < -1.0 ) v_vw = -1.0;

        sideslipAngle = asin( v_vw );
    }

    return sideslipAngle;
}

////////////////////////////////////////////////////////////////////////////////

double Aerodynamics::getPrandtlGlauertCoef( double machNumber, double max )
{
    double prandtlGlauertCoef  = 1.0;

    if ( machNumber < 1.0 )
    {
        prandtlGlauertCoef = 1.0 / sqrt( fabs( 1.0 - Misc::pow2( machNumber ) ) );
    }
    else
    {
        prandtlGlauertCoef = 1.0 / sqrt( fabs( 1.0 - Misc::pow2( machNumber ) ) );
    }

    if ( prandtlGlauertCoef > max || !Misc::isValid( prandtlGlauertCoef ) )
    {
        prandtlGlauertCoef = max;
    }

    return prandtlGlauertCoef;
}

////////////////////////////////////////////////////////////////////////////////

Matrix3x3 Aerodynamics::getAero2BAS( double alpha, double beta )
{
    return getAero2BAS( sin( alpha ), cos( alpha ),
                        sin( beta ), cos( beta ) );
}

////////////////////////////////////////////////////////////////////////////////

Matrix3x3 Aerodynamics::getAero2BAS( double sinAlpha , double cosAlpha,
                                     double sinBeta  , double cosBeta )
{
    Matrix3x3 aero2bas;

    aero2bas(0,0) = -cosAlpha * cosBeta;
    aero2bas(0,1) = -cosAlpha * sinBeta;
    aero2bas(0,2) =  sinAlpha;

    aero2bas(1,0) = -sinBeta;
    aero2bas(1,1) =  cosBeta;
    aero2bas(1,2) =  0.0;

    aero2bas(2,0) = -sinAlpha * cosBeta;
    aero2bas(2,1) = -sinAlpha * sinBeta;
    aero2bas(2,2) = -cosAlpha;

    return aero2bas;
}

////////////////////////////////////////////////////////////////////////////////

Matrix3x3 Aerodynamics::getStab2BAS( double alpha )
{
    return getStab2BAS( sin( alpha ), cos( alpha ) );
}

////////////////////////////////////////////////////////////////////////////////

Matrix3x3 Aerodynamics::getStab2BAS( double sinAlpha, double cosAlpha )
{
    Matrix3x3 stab2bas;

    stab2bas(0,0) = -cosAlpha;
    stab2bas(0,1) =  0.0;
    stab2bas(0,2) =  sinAlpha;

    stab2bas(1,0) = 0.0;
    stab2bas(1,1) = 1.0;
    stab2bas(1,2) = 0.0;

    stab2bas(2,0) = -sinAlpha;
    stab2bas(2,1) =  0.0;
    stab2bas(2,2) = -cosAlpha;

    return stab2bas;
}

////////////////////////////////////////////////////////////////////////////////

Aerodynamics::Aerodynamics( const Aircraft* aircraft ) :
    Module ( aircraft )
{}

////////////////////////////////////////////////////////////////////////////////

Aerodynamics::~Aerodynamics() {}

////////////////////////////////////////////////////////////////////////////////

void Aerodynamics::init() {}

////////////////////////////////////////////////////////////////////////////////

void Aerodynamics::update()
{
    updateMatrices();
}

////////////////////////////////////////////////////////////////////////////////

void Aerodynamics::updateMatrices()
{
    double sinAlpha = sin( _aircraft->getAngleOfAttack() );
    double cosAlpha = cos( _aircraft->getAngleOfAttack() );
    double sinBeta  = sin( _aircraft->getSideslipAngle() );
    double cosBeta  = cos( _aircraft->getSideslipAngle() );

    _aero2bas = getAero2BAS( sinAlpha, cosAlpha, sinBeta, cosBeta );
    _stab2bas = getStab2BAS( sinAlpha, cosAlpha );
    _bas2aero = _aero2bas.getTransposed();
    _bas2stab = _stab2bas.getTransposed();
}
