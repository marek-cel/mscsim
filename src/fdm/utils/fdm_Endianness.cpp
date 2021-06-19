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

#include <fdm/utils/fdm_Endianness.h>

#ifdef WIN32
#   include <windows.h>
#endif

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

unsigned int Endianness::hostToNet( unsigned int val )
{
    return htonl( val );
}

////////////////////////////////////////////////////////////////////////////////

unsigned short Endianness::hostToNet( unsigned short val )
{
    return htons( val );
}

////////////////////////////////////////////////////////////////////////////////

double Endianness::hostToNet( double val )
{
    unsigned int *val_ptr = (unsigned int*)( &val );
    unsigned int temp = val_ptr[ 0 ];

    val_ptr[ 0 ] = htonl( val_ptr[ 1 ] );
    val_ptr[ 1 ] = htonl( temp );

    return val;
}

////////////////////////////////////////////////////////////////////////////////

float Endianness::hostToNet( float val )
{
    unsigned int *val_ptr = (unsigned int*)( &val );
    unsigned int temp = val_ptr[ 0 ];

    val_ptr[ 0 ] = htonl( temp );

    return val;
}
