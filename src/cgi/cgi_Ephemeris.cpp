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

#include <cgi/cgi_Ephemeris.h>

#include <cmath>

////////////////////////////////////////////////////////////////////////////////

using namespace cgi;

////////////////////////////////////////////////////////////////////////////////

double Ephemeris::dayOfYear( DateTime dateTime )
{
    bool leapYear = false;

    if ( dateTime.year % 4 == 0 )
    {
        if ( dateTime.year % 100 != 0 || dateTime.year % 400 == 0 )
        {
            leapYear = true;
        }
    }

    double doy = 0.0;

    if ( dateTime.month  <  1 ) dateTime.month  = 1;
    if ( dateTime.month  > 12 ) dateTime.month  = 12;
    if ( dateTime.day    > 31 ) dateTime.day    = 31;
    if ( dateTime.hour   > 23 ) dateTime.hour   = 23;
    if ( dateTime.minute > 59 ) dateTime.minute = 59;
    if ( dateTime.second > 59 ) dateTime.second = 59;

    switch ( dateTime.month )
    {
        case 1:  doy =  0.0; break;
        case 2:  doy = 31.0; break;
        case 3:  doy = ( leapYear ) ?  60.0 :  59.0; break;
        case 4:  doy = ( leapYear ) ?  91.0 :  90.0; break;
        case 5:  doy = ( leapYear ) ? 121.0 : 120.0; break;
        case 6:  doy = ( leapYear ) ? 152.0 : 151.0; break;
        case 7:  doy = ( leapYear ) ? 182.0 : 181.0; break;
        case 8:  doy = ( leapYear ) ? 213.0 : 212.0; break;
        case 9:  doy = ( leapYear ) ? 244.0 : 243.0; break;
        case 10: doy = ( leapYear ) ? 274.0 : 273.0; break;
        case 11: doy = ( leapYear ) ? 305.0 : 304.0; break;
        case 12: doy = ( leapYear ) ? 335.0 : 334.0; break;
    }

    doy += (double)( dateTime.day - 1.0 );
    doy += (double)dateTime.hour   / 24.0;
    doy += (double)dateTime.minute / 24.0 / 60.0;
    doy += (double)dateTime.second / 24.0 / 60.0 / 60.0;

    return doy;
}

////////////////////////////////////////////////////////////////////////////////

double Ephemeris::daysInYear( unsigned short year )
{
    if ( year % 4 == 0 )
    {
        if ( year % 100 != 0 || year % 400 == 0 )
        {
            return 366.0;
        }
    }

    return 365.0;
}

////////////////////////////////////////////////////////////////////////////////

double Ephemeris::daysSinceJ2000( DateTime dateTime )
{
    double days = 0.0;

    double y = dateTime.year;
    double m = dateTime.month;
    double d = dateTime.day;

    days = 367.0 * y - 7.0 * ( y + ( m + 9.0 ) / 12.0 ) / 4.0 + 275.0 * m / 9.0 + d - 730530.0;

    days += dateTime.hour   / 24.0;
    days += dateTime.minute / 24.0 / 60.0;
    days += dateTime.second / 24.0 / 60.0 / 60.0;

    return days;
}

////////////////////////////////////////////////////////////////////////////////

double Ephemeris::julianDay( DateTime dateTime )
{
    double jd = 0.0;

    // Meeus J.: Astronomical Algorithms, p.61
    double y = dateTime.year;
    double m = dateTime.month;
    double d = dateTime.day;

    if ( m == 1 || m == 2 )
    {
        y = y - 1;
        m = 12 + m;
    }

    double a = (int)( y / 100.0 );
    double b = 2.0 - a + (int)( a / 4.0 );

    jd = (int)( 365.25 * ( y + 4716.0 ) ) + (int)( 30.6001 * ( m + 1.0 ) )
       + d + b - 1524.5;

    jd += dateTime.hour   / 24.0;
    jd += dateTime.minute / 24.0 / 60.0;
    jd += dateTime.second / 24.0 / 60.0 / 60.0;

    return jd;
}

////////////////////////////////////////////////////////////////////////////////

void Ephemeris::computeElevAndAzim( double alpha, double delta,
                                    double &elev, double &azim,
                                    double sinLat, double cosLat,
                                    double lst )
{
    double lha = lst - alpha;
    while ( lha < -M_PI ) lha += 2.0 * M_PI;
    while ( lha >  M_PI ) lha -= 2.0 * M_PI;

    double cosLha = cos( lha );

    double sinDelta = sin( delta );
    double cosDelta = cos( delta );

    double sinElev = sinDelta*sinLat + cosDelta*cosLha*cosLat;

    if ( sinElev >  1.0 ) sinElev =  1.0;
    if ( sinElev < -1.0 ) sinElev = -1.0;

    elev = asin( sinElev );

    double cosElev = cos( elev );

    double cosAzim = ( sinDelta*cosLat - cosLha*cosDelta*sinLat ) / cosElev;
    cosAzim = fabs( cosAzim );

    if ( cosAzim >  1.0 ) cosAzim =  1.0;
    if ( cosAzim < -1.0 ) cosAzim = -1.0;

    if ( lha < 0.0 )
        azim = M_PI - acos( cosAzim );
    else
        azim = M_PI + acos( cosAzim );
}

////////////////////////////////////////////////////////////////////////////////

Ephemeris::Ephemeris() :
    _jd ( 0.0 ),
    _jc ( 0.0 ),

    _sunAlpha ( 0.0 ),
    _sunDelta ( 0.0 ),
    _sunElev ( 0.0 ),
    _sunAzim ( 0.0 ),

    _moonAlpha ( 0.0 ),
    _moonDelta ( 0.0 ),
    _moonElev ( 0.0 ),
    _moonAzim ( 0.0 )
{}

////////////////////////////////////////////////////////////////////////////////

void Ephemeris::update( DateTime dateTime, double lat, double lon )
{
    // Meeus J.: Astronomical Algorithms, p.87
    _jd = julianDay( dateTime );
    _jc = ( _jd - 2451545.0 ) / 36525.0;

    _ut = dateTime.hour
        + dateTime.minute / 60.0f
        + dateTime.second / 3600.0f;

    double T0 = 6.69737455833333
              + 2400.05133690722 * _jc
              + 2.58622222e-5 * _jc*_jc
              - 1.72222222e-9 * _jc*_jc*_jc
              + 1.00273790935 * _ut;
    while ( T0 > 24.0 ) T0 -= 24.0;
    while ( T0 <  0.0 ) T0 += 24.0;

    // Greenwich sidereal time
    _gst = M_PI * T0 / 12.0;

    // local sidereal time angle
    _lst = _gst + lon;

    const double sinLat = sin( lat );
    const double cosLat = cos( lat );

    // obliquity of the ecliptic
    double epsilon = 0.409093 - 0.000227 * _jc;

    double cosEpsilon = cos( epsilon );
    double sinEpsilon = sin( epsilon );

    // mean anomaly
    double M = 6.240041 + 628.302 * _jc;

    while ( M > 2.0*M_PI ) M -= 2.0 * M_PI;
    while ( M <      0.0 ) M += 2.0 * M_PI;

    // Sun ecliptic longitude
    double sunLambda = 4.894968 + 628.331951 * _jc
                     + ( 0.033417 - 0.000084 * _jc ) * sin( M )
                     + 0.000351 * sin( 2.0*M );

    while ( sunLambda > 2.0*M_PI ) sunLambda -= 2.0 * M_PI;
    while ( sunLambda <      0.0 ) sunLambda += 2.0 * M_PI;

    double cosSunLambda = cos( sunLambda );
    double sinSunLambda = sin( sunLambda );

    // Sun right ascension
    _sunAlpha = atan2( (double)(sinSunLambda * cosEpsilon), (double)cosSunLambda );
    while ( _sunAlpha > 2.0*M_PI ) _sunAlpha -= 2.0 * M_PI;
    while ( _sunAlpha <      0.0 ) _sunAlpha += 2.0 * M_PI;

    // Sun declination
    _sunDelta = asin( sinSunLambda * sinEpsilon );

    // Moon
    double l_p = 3.8104 + 8399.7091 * _jc;
    double m   = 6.2300 +  628.3019 * _jc;
    double f   = 1.6280 + 8433.4663 * _jc;
    double m_p = 2.3554 + 8328.6911 * _jc;
    double d   = 5.1985 + 7771.3772 * _jc;

    // Moon ecliptic longitude
    double moonLambda
            = l_p
            + 0.1098 * sin( m_p )
            + 0.0222 * sin( 2.0*d - m_p )
            + 0.0115 * sin( 2.0*d )
            + 0.0037 * sin( 2.0*m_p )
            - 0.0032 * sin( m )
            - 0.0020 * sin( 2.0*f )
            + 0.0010 * sin( 2.0*d - 2*m_p )
            + 0.0010 * sin( 2.0*d - m - m_p )
            + 0.0009 * sin( 2.0*d + m_p )
            + 0.0008 * sin( 2.0*d - m )
            + 0.0007 * sin( m_p - m)
            - 0.0006 * sin( d )
            - 0.0005 * sin( m + m_p );

    double sinMoonLambda = sin( moonLambda );
    double cosMoonLambda = cos( moonLambda );

    // Moon ecliptic latitude
    double moonBeta
            = 0.0895 * sin( f )
            + 0.0049 * sin( m_p + f )
            + 0.0048 * sin( m_p - f )
            + 0.0030 * sin( 2.0*d - f )
            + 0.0010 * sin( 2.0*d + f - m_p )
            + 0.0008 * sin( 2.0*d - f - m_p )
            + 0.0006 * sin( 2.0*d + f );

    double sinMoonBeta = sin( moonBeta );
    double cosMoonBeta = cos( moonBeta );
    double tanMoonBeta = tan( moonBeta );

    // Moon right ascension
    _moonAlpha = atan2( sinMoonLambda*cosEpsilon - tanMoonBeta*sinEpsilon, cosMoonLambda );

    while ( _moonAlpha > 2.0*M_PI ) _moonAlpha -= 2.0 * M_PI;
    while ( _moonAlpha <      0.0 ) _moonAlpha += 2.0 * M_PI;

    // Moon declination
    _moonDelta = asin( sinMoonBeta*cosEpsilon + cosMoonBeta*sinEpsilon*sinMoonLambda );

    // Sun elevation and azimuth
    computeElevAndAzim( _sunAlpha, _sunDelta, _sunElev, _sunAzim,
                        sinLat, cosLat, _lst );

    // Moon elevation and azimuth
    computeElevAndAzim( _moonAlpha, _moonDelta, _moonElev, _moonAzim,
                        sinLat, cosLat, _lst );
}
