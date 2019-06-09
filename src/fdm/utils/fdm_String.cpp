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

#include <fdm/utils/fdm_String.h>

#include <stdio.h>
#include <sstream>
#include <string.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

std::string String::getFirstLine( const std::string &str )
{
    std::istringstream iss( str );
    std::string firstLine;
    std::getline( iss, firstLine );

    return firstLine;
}

////////////////////////////////////////////////////////////////////////////////

int String::icompare( const std::string &str_1, const std::string &str_2 )
{
    std::string strTemp_1 = toLower( str_1 );
    std::string strTemp_2 = toLower( str_2 );

    return strTemp_1.compare( strTemp_2 );
}

////////////////////////////////////////////////////////////////////////////////

std::string String::stripLeadingSpaces( const std::string &str )
{
    char *temp = new char [ str.length() + 1 ];
    strcpy( temp, str.c_str() );

    unsigned int offset = 0;

    for ( unsigned int i = 0; i < str.size(); i++ )
    {
        if ( !isspace( temp[ i ] ) )
        {
            break;
        }

        offset++;
    }

    std::string result = &( temp[ offset ] );

    if ( temp ) delete [] temp;
    temp = 0;

    return result;
}

////////////////////////////////////////////////////////////////////////////////

bool String::toBool( const std::string &str, bool def )
{
    bool result = def;
    int temp;

    if ( 1 == sscanf( str.c_str(), "%d", &temp ) )
    {
        result = temp != 0;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

int String::toInt( const std::string &str, int def )
{
    int result = def;
    int temp;

    if ( 1 == sscanf( str.c_str(), "%d", &temp ) )
    {
        result = temp;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

double String::toDouble( const std::string &str, double def )
{
    double result = def;
    double temp;

    if ( 1 == sscanf( str.c_str(), "%lf", &temp ) )
    {
        result = temp;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

std::string String::toString( int val )
{
    char valueStr[ 255 ];

    sprintf( valueStr, "%d", val );

    return std::string( valueStr );
}

////////////////////////////////////////////////////////////////////////////////

std::string String::toString( float val )
{
    char valueStr[ 255 ];

    sprintf( valueStr, "%f", val );

    return std::string( valueStr );
}

////////////////////////////////////////////////////////////////////////////////

std::string String::toString( double val )
{
    char valueStr[ 255 ];

    sprintf( valueStr, "%lf", val );

    return std::string( valueStr );
}

////////////////////////////////////////////////////////////////////////////////

std::string String::toLower( const std::string &str )
{
    std::string result;
    result.clear();

    unsigned int length = str.length();

    for ( unsigned int i = 0; i < length; i++ )
    {
        result += tolower( str.c_str()[ i ] );
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

std::string String::toUpper( const std::string &str )
{
    std::string result;
    result.clear();

    unsigned int length = str.length();

    for ( unsigned int i = 0; i < length; i++ )
    {
        result += toupper( str.c_str()[ i ] );
    }

    return result;
}
