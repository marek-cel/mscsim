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

#include <fdm_uh60/uh60_MainRotor.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

UH60_MainRotor::UH60_MainRotor() :
    _downwashLag ( FDM_NULLPTR )
{
    _downwashLag = new Lag();
}

////////////////////////////////////////////////////////////////////////////////

UH60_MainRotor::~UH60_MainRotor()
{
    FDM_DELPTR( _downwashLag );
}

////////////////////////////////////////////////////////////////////////////////

void UH60_MainRotor::update( double omega,
                             double azimuth,
                             double collective,
                             double cyclicLat,
                             double cyclicLon )
{
    //////////////////////////////////////////////////////////////////////
    MainRotor::update( omega, azimuth, collective, cyclicLat, cyclicLon );
    //////////////////////////////////////////////////////////////////////

    double timeStep = 0.0; // TODO

    // NASA-CR-166309, p.5.1-49 (PDF p.69)
    const double k_ct  = 1.0;
    const double t_dwo = 0.01039;

    double omegaR = omega * _r;

    // s - shaft axis
    double mu_xs = 0.0 / omegaR; // TODO
    double mu_ys = 0.0 / omegaR; // TODO

    double lambda_0t = _vel_i / omegaR;

    // total velocity component at the rotor (UTOTMR)
    // NASA-CR-166309, p.5.1-20 (PDF p.40)
    double mu_tot = sqrt( mu_xs*mu_xs + mu_ys*mu_ys + lambda_0t*lambda_0t );

    // NASA-CR-166309, p.5.1-21 (PDF p.41)
    _downwashLag->setTimeConst( t_dwo / mu_tot );
    _downwashLag->update( timeStep, k_ct * _ct / ( 2.0 * mu_tot ) );
}
