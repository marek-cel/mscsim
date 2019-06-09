/***************************************************************************//**
 * @author  Marek M. Cel <marekcel@marekcel.pl>
 *
 * @section LICENSE
 *
 * Copyright (c) 2019 Marek M. Cel
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
#ifndef WRITE_H
#define WRITE_H

////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <stdio.h>
#include <string>

#include "global.h"
#include "utils.h"

////////////////////////////////////////////////////////////////////////////////

void write();

////////////////////////////////////////////////////////////////////////////////

template < class TYPE >
int write( const std::string &inp, const std::string &out, TYPE *obj )
{
    int error = 0;

    double aoa_deg = 0.0;
    double aoa_rad = 0.0;

    FILE *inpFile = fopen( inp.c_str(), "r" );
    FILE *outFile = fopen( out.c_str(), "w" );

    if ( error == 0 && inpFile != 0 && outFile != 0 )
    {
        while ( true )
        {
            if ( 0 < fscanf( inpFile, "%lf", &aoa_deg ) )
            {
                aoa_rad = deg2rad( aoa_deg );

                fprintf( outFile, "%lf,%lf\n", aoa_deg, obj->get( aoa_rad ) );
            }
            else
            {
                break;
            }
        }
    }
    else
    {
        error = 1;

        if ( inpFile == 0 ) std::cerr << "Error! Cannot open file: \"" << inp << "\"" << std::endl;
        if ( outFile == 0 ) std::cerr << "Error! Cannot open file: \"" << out << "\"" << std::endl;
        std::cout << std::endl;
    }

    return error;
}

////////////////////////////////////////////////////////////////////////////////

#endif // WRITE_H
