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

#include <fdm/utils/fdm_Random.h>

#include <stdlib.h>
#include <time.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Random* Random::m_instance = 0;

////////////////////////////////////////////////////////////////////////////////

Random::Random()
{
    srand( (unsigned int)time( NULL ) );
}

////////////////////////////////////////////////////////////////////////////////

Random::~Random() {}

////////////////////////////////////////////////////////////////////////////////

int Random::getRandom( int min, int max )
{
    if ( max > 0 && max > min && max <= RAND_MAX )
    {
        return min + rand() % ( max - min + 1 );
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////

float Random::getRandom( float min, float max )
{
    int random = getRandom( 0, RAND_MAX );

    return min + ( max - min ) * ( (float)random / (float)RAND_MAX );
}
