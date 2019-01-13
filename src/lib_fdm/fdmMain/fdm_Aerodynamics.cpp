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

#include <fdmMain/fdm_Aircraft.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

double Aerodynamics::getAngleOfAttack( const Vector3 &vel_bas, double vel_min )
{
    double angleOfAttack = 0.0;

    double uv = sqrt( vel_bas( i_u )*vel_bas( i_u ) + vel_bas( i_v )*vel_bas( i_v ) );

    if ( fabs( uv ) > vel_min || fabs( vel_bas( i_w ) ) > vel_min )
    {
        angleOfAttack = atan2( vel_bas( i_w ), uv );
    }

    return angleOfAttack;
}

////////////////////////////////////////////////////////////////////////////////

double Aerodynamics::getSideslipAngle( const Vector3 &vel_bas, double vel_min )
{
    double sideslipAngle = 0.0;

    if ( fabs( vel_bas( i_u ) ) > vel_min || fabs( vel_bas( i_v ) ) > vel_min )
    {
        double vw = vel_bas.getLength();
        double v_vw = ( vw > vel_min ) ? ( vel_bas( i_v ) / vw ) : 0.0;

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

Matrix3x3 Aerodynamics::getAero2BAS( double sinAlpha, double cosAlpha,
                                     double sinBeta, double cosBeta )
{
    // Stevens B., Lewis F.: Aircraft Control and Simulation, p.63
    Matrix3x3 T_aero2bas;

    T_aero2bas(0,0) = -cosAlpha * cosBeta;
    T_aero2bas(0,1) = -cosAlpha * sinBeta;
    T_aero2bas(0,2) =  sinAlpha;

    T_aero2bas(1,0) = -sinBeta;
    T_aero2bas(1,1) =  cosBeta;
    T_aero2bas(1,2) =  0.0;

    T_aero2bas(2,0) = -sinAlpha * cosBeta;
    T_aero2bas(2,1) = -sinAlpha * sinBeta;
    T_aero2bas(2,2) = -cosAlpha;

    return T_aero2bas;
}

////////////////////////////////////////////////////////////////////////////////

Matrix3x3 Aerodynamics::getStab2BAS( double alpha )
{
    return getStab2BAS( sin( alpha ), cos( alpha ) );
}

////////////////////////////////////////////////////////////////////////////////

Matrix3x3 Aerodynamics::getStab2BAS( double sinAlpha, double cosAlpha )
{
    // Stevens B., Lewis F.: Aircraft Control and Simulation, p.63
    Matrix3x3 T_stab2bas;

    T_stab2bas(0,0) = -cosAlpha;
    T_stab2bas(0,1) =  0.0;
    T_stab2bas(0,2) =  sinAlpha;

    T_stab2bas(1,0) = 0.0;
    T_stab2bas(1,1) = 1.0;
    T_stab2bas(1,2) = 0.0;

    T_stab2bas(2,0) = -sinAlpha;
    T_stab2bas(2,1) =  0.0;
    T_stab2bas(2,2) = -cosAlpha;

    return T_stab2bas;
}

////////////////////////////////////////////////////////////////////////////////

Aerodynamics::Aerodynamics( const Aircraft* aircraft ) :
    DataManager( aircraft ),
    m_aircraft ( aircraft )
{}

////////////////////////////////////////////////////////////////////////////////

Aerodynamics::~Aerodynamics() {}

////////////////////////////////////////////////////////////////////////////////

void Aerodynamics::update()
{
    updateMatrices();
}

////////////////////////////////////////////////////////////////////////////////

void Aerodynamics::updateMatrices()
{
    double sinAlpha = sin( m_aircraft->getAngleOfAttack() );
    double cosAlpha = cos( m_aircraft->getAngleOfAttack() );
    double sinBeta  = sin( m_aircraft->getSideslipAngle() );
    double cosBeta  = cos( m_aircraft->getSideslipAngle() );

    m_aero2bas = getAero2BAS( sinAlpha, cosAlpha, sinBeta, cosBeta );
    m_stab2bas = getStab2BAS( sinAlpha, cosAlpha );
    m_bas2aero = m_aero2bas.getTransposed();
    m_bas2stab = m_stab2bas.getTransposed();
}
