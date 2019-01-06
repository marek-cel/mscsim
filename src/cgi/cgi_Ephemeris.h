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
#ifndef CGI_EPHEMERIS_H
#define CGI_EPHEMERIS_H

////////////////////////////////////////////////////////////////////////////////

namespace cgi
{

/**
 * @brief Ephemeris model.
 */
class Ephemeris
{
public:

    struct DateTime
    {
        unsigned short year;
        unsigned char  month;
        unsigned char  day;
        unsigned char  hour;
        unsigned char  minute;
        unsigned char  second;
    };

    static double dayOfYear( DateTime dateTime );

    static double daysInYear( unsigned short year );

    static double daysSinceJ2000( DateTime dateTime );

    static double julianDay( DateTime dateTime );

    /**
     * Computes celestial object local elevation and azimuth.
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

    /** Constructor. */
    Ephemeris();

    void update( DateTime dateTime, double lat, double lon );

    inline double getJD() const { return m_jd; }
    inline double getJC() const { return m_jc; }
    inline double getUT() const { return m_ut; }

    inline double getGST() const { return m_gst; }
    inline double getLST() const { return m_lst; }

    inline double getSunAlpha() const { return m_sunAlpha; }
    inline double getSunDelta() const { return m_sunDelta; }
    inline double getSunElev()  const { return m_sunElev;  }
    inline double getSunAzim()  const { return m_sunAzim;  }

    inline double getMoonAlpha() const { return m_moonAlpha; }
    inline double getMoonDelta() const { return m_moonDelta; }
    inline double getMoonElev()  const { return m_moonElev;  }
    inline double getMoonAzim()  const { return m_moonAzim;  }

private:

    double m_jd;            ///< [-] Julian day
    double m_jc;            ///< [-] Julian century
    double m_ut;            ///< [h] universal time

    double m_gst;           ///< [rad] Greenwhich Siderial Time
    double m_lst;           ///< [rad] Local Siderial Time

    double m_sunAlpha;      ///< [rad] Sun right ascension
    double m_sunDelta;      ///< [rad] Sun declination
    double m_sunElev;       ///< [rad] Sun elevation
    double m_sunAzim;       ///< [rad] Sun azimuth

    double m_moonAlpha;     ///< [rad] Moon right ascension
    double m_moonDelta;     ///< [rad] Moon declination
    double m_moonElev;      ///< [rad] Moon elevation
    double m_moonAzim;      ///< [rad] Moon azimuth
};

} // end of cgi namespace

////////////////////////////////////////////////////////////////////////////////

#endif // CGI_EPHEMERIS_H
