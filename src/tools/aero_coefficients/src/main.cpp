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
#ifndef MAIN_CPP
#   define MAIN_CPP
#endif

////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "defs.h"
#include "global.h"
#include "write.h"

////////////////////////////////////////////////////////////////////////////////

using namespace std;

////////////////////////////////////////////////////////////////////////////////

void printArgs();
void printHelp();

void readArgs( char *argv[] );

////////////////////////////////////////////////////////////////////////////////

int main( int argc, char *argv[] )
{
    cout << APP_NAME << ", ver. " << APP_VER << endl;

    if ( argc == 21 )
    {
        readArgs( argv );
        printArgs();

        al_s = deg2rad( al_s );
        al_1 = deg2rad( al_1 );
        al_2 = deg2rad( al_2 );

        ad_1 = deg2rad( ad_1 );
        ad_2 = deg2rad( ad_2 );
        ad_3 = deg2rad( ad_3 );
        ad_4 = deg2rad( ad_4 );

        lift = new CoefLift( cl_s, cl_1, cl_2,
                             al_s, al_1, al_2 );

        drag = new CoefDrag( cd_0, cd_1, cd_2, cd_3, cd_4, cd_5,
                             ad_1, ad_2, ad_3, ad_4 );

        write();
    }
    else
    {
        if ( argc != 1 )
        {
            cerr << "Error! Wrong number of arguments!" << endl;
        }

        printHelp();
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////

void printArgs()
{
    cout << "   Drag Inp : " << dragInp << endl;
    cout << "   Drag Out : " << dragOut << endl;
    cout << "   Lift Inp : " << liftInp << endl;
    cout << "   Lift Out : " << liftOut << endl;
    cout << endl;
    cout << " al_s, cl_s : " << al_s << " , " << cl_s << endl;
    cout << " al_1, cl_1 : " << al_1 << " , " << cl_1 << endl;
    cout << " al_2, cl_2 : " << al_2 << " , " << cl_2 << endl;
    cout << endl;
    cout << "       cd_0 : " << cd_0 << endl;
    cout << " ad_1, cd_1 : " << ad_1 << " , " << cd_1 << endl;
    cout << " ad_2, cd_2 : " << ad_2 << " , " << cd_2 << endl;
    cout << " ad_3, cd_3 : " << ad_3 << " , " << cd_3 << endl;
    cout << " ad_4, cd_4 : " << ad_4 << " , " << cd_4 << endl;
    cout << "       cd_5 : " << cd_5 << endl;
    cout << endl;
}

////////////////////////////////////////////////////////////////////////////////

void printHelp()
{
    cout << "See:" << endl;
    cout << "  https://ntrs.nasa.gov/archive/nasa/casi.ntrs.nasa.gov/19910009728.pdf" << endl;
    cout << "  Takahashi M.: A Flight-Dynamic Helicopter Mathematical Model with a Single" << endl;
    cout << "  Flap-Lag-Torsion Main Rotor, NASA, TM-102267" << endl;
    cout << "Usage:" << endl;
    cout << "$aero_coefficients" << endl;
    cout << "  DRAG_COEF_AOA_INPUT_FILE DRAG_COEF_OUTPUT_FILE" << endl;
    cout << "  LIFT_COEF_AOA_INPUT_FILE LIFT_COEF_OUTPUT_FILE" << endl;
    cout << "  AL_S, CL_S, AL_1, CL_1, AL_2, CL_2" << endl;
    cout << "  CD_0, AD_1, CD_1, AD_2, CD_2, AD_3, CD_3, AD_4, CD_4, CD_5" << endl;
}

////////////////////////////////////////////////////////////////////////////////

void readArgs( char *argv[] )
{
    dragInp = argv[ 1 ];
    dragOut = argv[ 2 ];
    liftInp = argv[ 3 ];
    liftOut = argv[ 4 ];

    sscanf( argv[  5 ], "%lf", &al_s );
    sscanf( argv[  6 ], "%lf", &cl_s );

    sscanf( argv[  7 ], "%lf", &al_1 );
    sscanf( argv[  8 ], "%lf", &cl_1 );

    sscanf( argv[  9 ], "%lf", &al_2 );
    sscanf( argv[ 10 ], "%lf", &cl_2 );

    sscanf( argv[ 11 ], "%lf", &cd_0 );

    sscanf( argv[ 12 ], "%lf", &ad_1 );
    sscanf( argv[ 13 ], "%lf", &cd_1 );

    sscanf( argv[ 14 ], "%lf", &ad_2 );
    sscanf( argv[ 15 ], "%lf", &cd_2 );

    sscanf( argv[ 16 ], "%lf", &ad_3 );
    sscanf( argv[ 17 ], "%lf", &cd_3 );

    sscanf( argv[ 18 ], "%lf", &ad_4 );
    sscanf( argv[ 19 ], "%lf", &cd_4 );

    sscanf( argv[ 20 ], "%lf", &cd_5 );
}
