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
