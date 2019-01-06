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

#include <fdmUtils/fdm_Units.h>

#include <fdmUtils/fdm_String.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

double Units::factorSI( const char *name )
{
    if ( 0 == String::icompare( name, "deg" ) )
        return deg2rad();

    else if ( 0 == String::icompare( name, "ft" ) )
        return ft2m();
    else if ( 0 == String::icompare( name, "km" ) )
        return km2m();
    else if ( 0 == String::icompare( name, "nmi" ) )
        return nmi2m();

    else if ( 0 == String::icompare( name, "ft_min" ) )
        return fpm2mps();
    else if ( 0 == String::icompare( name, "ft_s" ) )
        return fps2mps();
    else if ( 0 == String::icompare( name, "km_h" ) )
        return kmh2mps();
    else if ( 0 == String::icompare( name, "kts" ) )
        return kts2mps();

    else if ( 0 == String::icompare( name, "lb" ) )
        return lb2kg();

    else if ( 0 == String::icompare( name, "lbf" ) )
        return lbf2n();

    else if ( 0 == String::icompare( name, "psi" ) )
        return psi2pa();
    else if ( 0 == String::icompare( name, "inHg" ) )
        return inhg2pa();

    else if ( 0 == String::icompare( name, "PS" ) )
        return ps2w();
    else if ( 0 == String::icompare( name, "hp" ) )
        return hp2w();
    else if ( 0 == String::icompare( name, "kW" ) )
        return kw2w();

    else if ( 0 == String::icompare( name, "g_kWh" ) )
        return g_kWh_2_kg_Ws();

    return 1.0;
}
