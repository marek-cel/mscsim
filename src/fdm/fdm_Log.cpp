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

#include <fdm/fdm_Log.h>

#include <ctime>
#include <iomanip>
#include <iostream>
#include <math.h>

#ifdef _LINUX_
#   include <sys/time.h>
#endif

#ifdef WIN32
#   include <Windows.h>
#endif

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

std::ostream& Log::m_out = std::cout;

////////////////////////////////////////////////////////////////////////////////

std::ostream& Log::timeTag()
{
    int year = 2000;
    int mon  = 1;
    int day  = 1;
    int hour = 0;
    int min  = 0;
    int sec  = 0;
    int msec = 0;

#   ifdef _LINUX_
    struct timeval tp;
    gettimeofday( &tp, NULL );
    std::tm *tm = std::localtime( &tp.tv_sec );

    year = 1900 + tm->tm_year;
    mon  = tm->tm_mon;
    day  = tm->tm_mday;
    hour = tm->tm_hour;
    min  = tm->tm_min;
    sec  = tm->tm_sec;
    msec = floor( tp.tv_usec * 0.001 );
#   endif

#   ifdef WIN32
    SYSTEMTIME st;
    GetLocalTime( &st );

    year = st.wYear;
    mon  = st.wMonth;
    day  = st.wDay;
    hour = st.wHour;
    min  = st.wMinute;
    sec  = st.wSecond;
    msec = st.wMilliseconds;
#   endif

    m_out << "[";
    m_out << year;
    m_out << "-";
    m_out << std::setfill('0') << std::setw( 2 ) << mon;
    m_out << "-";
    m_out << std::setfill('0') << std::setw( 2 ) << day;
    m_out << " ";
    m_out << std::setfill('0') << std::setw( 2 ) << hour;
    m_out << ":";
    m_out << std::setfill('0') << std::setw( 2 ) << min;
    m_out << ":";
    m_out << std::setfill('0') << std::setw( 2 ) << sec;
    m_out << ".";
    m_out << std::setfill('0') << std::setw( 3 ) << msec;
    m_out << "]";

    return m_out;
}
