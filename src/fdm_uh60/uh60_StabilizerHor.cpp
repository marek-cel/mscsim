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

#include <fdm_uh60/uh60_StabilizerHor.h>

#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

UH60_StabilizerHor::UH60_StabilizerHor() :
    _elevator ( 0.0 )
{}

////////////////////////////////////////////////////////////////////////////////

UH60_StabilizerHor::~UH60_StabilizerHor() {}

////////////////////////////////////////////////////////////////////////////////

void UH60_StabilizerHor::computeForceAndMoment( const Vector3 &vel_air_bas,
                                                const Vector3 &omg_air_bas,
                                                double airDensity,
                                                double elevator )
{
    _elevator = elevator;

    StabilizerHor::computeForceAndMoment( vel_air_bas, omg_air_bas, airDensity );
}

////////////////////////////////////////////////////////////////////////////////

double UH60_StabilizerHor::getAngleOfAttack( const Vector3 &vel_air_bas,
                                             double wingAngleOfAttack )
{
    return StabilizerHor::getAngleOfAttack( vel_air_bas, wingAngleOfAttack )
            + _elevator;
}
