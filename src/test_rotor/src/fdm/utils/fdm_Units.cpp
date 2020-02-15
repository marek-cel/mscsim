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

#include <fdm/utils/fdm_Units.h>

#include <fdm/utils/fdm_String.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

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

    else if ( 0 == String::icompare( name, "cuin" ) )
        return &Units::cuin2m3;
    else if ( 0 == String::icompare( name, "l" ) )
        return &Units::l2m3;

    else if ( 0 == String::icompare( name, "fpm" ) )
        return &Units::fpm2mps;
    else if ( 0 == String::icompare( name, "fps" ) )
        return &Units::fps2mps;
    else if ( 0 == String::icompare( name, "kmh" ) )
        return &Units::kmh2mps;
    else if ( 0 == String::icompare( name, "kts" ) )
        return &Units::kts2mps;

    else if ( 0 == String::icompare( name, "deg_s" ) )
        return &Units::deg2rad;
    else if ( 0 == String::icompare( name, "rpm" ) )
        return &Units::rpm2rad_s;

    else if ( 0 == String::icompare( name, "lb" ) )
        return &Units::lb2kg;

    else if ( 0 == String::icompare( name, "lbf" ) )
        return &Units::lbf2n;

    else if ( 0 == String::icompare( name, "psi" ) )
        return &Units::psi2pa;
    else if ( 0 == String::icompare( name, "inHg" ) )
        return &Units::inhg2pa;
    else if ( 0 == String::icompare( name, "mb" ) )
        return &Units::mb2pa;

    else if ( 0 == String::icompare( name, "PS" ) )
        return &Units::ps2w;
    else if ( 0 == String::icompare( name, "hp" ) )
        return &Units::hp2w;
    else if ( 0 == String::icompare( name, "kW" ) )
        return &Units::kw2w;

    else if ( 0 == String::icompare( name, "degC" ) )
        return &Units::c2k;
    else if ( 0 == String::icompare( name, "degF" ) )
        return &Units::f2k;

    else if ( 0 == String::icompare( name, "g_kWh" ) )
        return &Units::g_kWh_2_kg_Ws;

    else if ( 0 == String::icompare( name, "g_kNs" ) )
        return &Units::g_kNs_2_kg_Ns;

    return FDM_NULLPTR;
}
