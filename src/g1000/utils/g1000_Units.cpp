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

#include <g1000/utils/g1000_Units.h>

#include <g1000/g1000_Defines.h>
#include <g1000/utils/g1000_String.h>

////////////////////////////////////////////////////////////////////////////////

using namespace g1000;

////////////////////////////////////////////////////////////////////////////////

Units::fptr Units::getConverter( const char *name )
{
    if ( 0 == String::icompare( name, "deg" ) )
        return &Units::deg2rad;

    else if ( 0 == String::icompare( name, "ft" ) )
        return &Units::ft2m;
    else if ( 0 == String::icompare( name, "km" ) )
        return &Units::km2m;
    else if ( 0 == String::icompare( name, "nmi" ) )
        return &Units::nmi2m;

    else if ( 0 == String::icompare( name, "fpm" ) )
        return &Units::fpm2mps;
    else if ( 0 == String::icompare( name, "fps" ) )
        return &Units::fps2mps;
    else if ( 0 == String::icompare( name, "kmh" ) )
        return &Units::kmh2mps;
    else if ( 0 == String::icompare( name, "kts" ) )
        return &Units::kts2mps;

    else if ( 0 == String::icompare( name, "lb" ) )
        return &Units::lb2kg;

    else if ( 0 == String::icompare( name, "psi" ) )
        return &Units::psi2pa;
    else if ( 0 == String::icompare( name, "inHg" ) )
        return &Units::inhg2pa;
    else if ( 0 == String::icompare( name, "mb" ) )
        return &Units::mb2pa;

    else if ( 0 == String::icompare( name, "degC" ) )
        return &Units::c2k;
    else if ( 0 == String::icompare( name, "degF" ) )
        return &Units::f2k;

    return NULLPTR;
}

////////////////////////////////////////////////////////////////////////////////

Units::fptr Units::getConverterInv( const char *name )
{
    if ( 0 == String::icompare( name, "deg" ) )
        return &Units::rad2deg;

    else if ( 0 == String::icompare( name, "ft" ) )
        return &Units::m2ft;
    else if ( 0 == String::icompare( name, "km" ) )
        return &Units::m2km;
    else if ( 0 == String::icompare( name, "nmi" ) )
        return &Units::m2nmi;

    else if ( 0 == String::icompare( name, "fpm" ) )
        return &Units::mps2fpm;
    else if ( 0 == String::icompare( name, "fps" ) )
        return &Units::mps2fps;
    else if ( 0 == String::icompare( name, "kmh" ) )
        return &Units::mps2kmh;
    else if ( 0 == String::icompare( name, "kts" ) )
        return &Units::mps2kts;

    else if ( 0 == String::icompare( name, "lb" ) )
        return &Units::kg2lb;

    else if ( 0 == String::icompare( name, "psi" ) )
        return &Units::pa2psi;
    else if ( 0 == String::icompare( name, "inHg" ) )
        return &Units::pa2inhg;
    else if ( 0 == String::icompare( name, "mb" ) )
        return &Units::pa2mb;

    else if ( 0 == String::icompare( name, "degC" ) )
        return &Units::k2c;
    else if ( 0 == String::icompare( name, "degF" ) )
        return &Units::k2f;

    return NULLPTR;
}
