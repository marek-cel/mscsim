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
#ifndef CGI_EPHEMERIS_H
#define CGI_EPHEMERIS_H

////////////////////////////////////////////////////////////////////////////////

namespace cgi
{

/**
 * @brief Ephemeris model.
 *
 * @see Meeus J.: Astronomical Algorithms, 1998
 */
class Ephemeris
{
public:

    struct DateTime
    {
        unsigned short year;        ///< year
        unsigned char  month;       ///< month
        unsigned char  day;         ///< day
        unsigned char  hour;        ///< hour
        unsigned char  minute;      ///< minute
        unsigned char  second;      ///< second
    };

    static double dayOfYear( DateTime dateTime );

    static double daysInYear( unsigned short year );

    static double daysSinceJ2000( DateTime dateTime );

    static double julianDay( DateTime dateTime );

    /**
     * @brief Computes celestial object local elevation and azimuth.
     * @param alpha [rad] right ascension
     * @param delta [rad] declination
     * @param elev [rad] elevation (result)
     * @param azim [rad] azimuth (result)
     * @param sinLat [-] latitude sine
     * @param cosLat [-] latitude cosine
     * @param lst [rad] Local Siderial Time
     */
    static void computeElevAndAzim( double alpha, double delta,
                                    double &elev, double &azim,
                                    double sinLat, double cosLat,
                                    double lst );

    /** @brief Constructor. */
    Ephemeris();

    void update( DateTime dateTime, double lat, double lon );

    inline double getJD() const { return _jd; }
    inline double getJC() const { return _jc; }
    inline double getUT() const { return _ut; }

    inline double getGST() const { return _gst; }
    inline double getLST() const { return _lst; }

    inline double getSunAlpha() const { return _sunAlpha; }
    inline double getSunDelta() const { return _sunDelta; }
    inline double getSunElev()  const { return _sunElev;  }
    inline double getSunAzim()  const { return _sunAzim;  }

    inline double getMoonAlpha() const { return _moonAlpha; }
    inline double getMoonDelta() const { return _moonDelta; }
    inline double getMoonElev()  const { return _moonElev;  }
    inline double getMoonAzim()  const { return _moonAzim;  }

private:

    double _jd;             ///< [-] Julian day
    double _jc;             ///< [-] Julian century
    double _ut;             ///< [h] universal time

    double _gst;            ///< [rad] Greenwhich Siderial Time
    double _lst;            ///< [rad] Local Siderial Time

    double _sunAlpha;       ///< [rad] Sun right ascension
    double _sunDelta;       ///< [rad] Sun declination
    double _sunElev;        ///< [rad] Sun elevation
    double _sunAzim;        ///< [rad] Sun azimuth

    double _moonAlpha;      ///< [rad] Moon right ascension
    double _moonDelta;      ///< [rad] Moon declination
    double _moonElev;       ///< [rad] Moon elevation
    double _moonAzim;       ///< [rad] Moon azimuth
};

} // end of cgi namespace

////////////////////////////////////////////////////////////////////////////////

#endif // CGI_EPHEMERIS_H
