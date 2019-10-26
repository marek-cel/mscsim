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
#ifndef NAVIGATION_H
#define NAVIGATION_H

////////////////////////////////////////////////////////////////////////////////

#include <QDomElement>
#include <QList>

#include <fdm/utils/fdm_WGS84.h>

#include <Data.h>

////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Navigation class.
 */
class Navigation
{
public:

    typedef Data::Navigation::NAV_CDI NAV_CDI;

    struct ILS
    {
        int freq;                       ///< [kHz] ILS frequency

        fdm::Vector3 pos_wgs_gs;        ///< [m] ILS GS position expressed in WGS
        fdm::Vector3 pos_wgs_loc;       ///< [m] ILS LOC position expressed in WGS
        fdm::Vector3 pos_wgs_dme;       ///< [m] ILS DME position expressed in WGS

        double glide_slope;             ///< [rad] glide slope
        double heading;                 ///< [rad] runway heading
    };

    struct NDB
    {
        int freq;                       ///< [kHz] NDB frequency
        fdm::Vector3 pos_wgs;           ///< [m] NDB position expressed in WGS
    };

    struct VOR
    {
        int freq;                       ///< [kHz] VOR frequency
        fdm::Vector3 pos_wgs;           ///< [m] VOR position expressed in WGS
    };

    typedef QList< ILS > ListILS;
    typedef QList< NDB > ListNDB;
    typedef QList< VOR > ListVOR;

    static const double _range_ils;     ///< [m] ILS range
    static const double _range_ndb;     ///< [m] NDB range
    static const double _range_vor;     ///< [m] VOR range

    static const double _ils_gs_max;    ///< [rad] ILS GS full deflection
    static const double _ils_lc_max;    ///< [rad] ILS LC full deflection

    static const double _vor_max;       ///< [rad] VOR full deflection

    /** Constructor. */
    Navigation();

    /** Destructor. */
    virtual ~Navigation();

    /** Updates navigation. */
    void update();

    inline void setCourse( double course )   { _course  = course;  }

    inline void setFreqADF( int freq ) { _adf_freq = freq; }
    inline void setFreqNAV( int freq ) { _nav_freq = freq; }

private:

    ListILS _listILS;           ///<
    ListNDB _listNDB;           ///<
    ListVOR _listVOR;           ///<

    ILS *_ils;                  ///<
    NDB *_ndb;                  ///<
    VOR *_vor;                  ///<

    fdm::WGS84 _aircraft_wgs;   ///<

    double _course;             ///< [rad]

    int _adf_freq;              ///< [kHz]
    int _nav_freq;              ///< [kHz]

    bool _adf_visible;          ///<
    double _adf_bearing;        ///< [rad]

    bool _ils_gs_visible;       ///<
    bool _ils_lc_visible;       ///<
    double _ils_gs_deviation;   ///< [rad]
    double _ils_lc_deviation;   ///< [rad]
    double _ils_gs_norm;        ///< [-1.0;1.0]
    double _ils_lc_norm;        ///< [-1.0;1.0]

    NAV_CDI _nav_cdi;           ///<
    double _nav_bearing;        ///< [rad]
    double _nav_deviation;      ///< [rad]
    double _nav_distance;       ///< [m]
    double _nav_norm;           ///< [-1.0;1.0]

    double getAzimuth( const fdm::Vector3 &pos_wgs );
    double getDistance( const fdm::Vector3 &pos_wgs );

    void parseNavaid( const QDomElement &node );

    void parseILS( const QDomElement &node );
    void parseNDB( const QDomElement &node );
    void parseVOR( const QDomElement &node );

    fdm::Vector3 readPosition( const QDomElement &node );

    void updateNavaids();
    void updateNavaidsActive();

    void updateADF();
    void updateILS();
    void updateNAV();
    void updateNAV( double azim, double course, double max );
};

////////////////////////////////////////////////////////////////////////////////

#endif // NAVIGATION_H
