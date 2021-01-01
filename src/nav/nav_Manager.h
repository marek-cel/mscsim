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
#ifndef NAV_MANAGER_H
#define NAV_MANAGER_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/utils/fdm_WGS84.h>

#include <Data.h>

#include <nav/nav_DataBase.h>

////////////////////////////////////////////////////////////////////////////////

namespace nav
{

/**
 * @brief Navigation class.
 *
 * @see https://en.wikipedia.org/wiki/Great-circle_distance
 */
class Manager
{
public:

    typedef Data::Navigation::NAV_CDI NAV_CDI;

    static const double _range_ils;     ///< [m] ILS range
    static const double _range_ndb;     ///< [m] NDB range
    static const double _range_vor;     ///< [m] VOR range

    static const double _ils_gs_max;    ///< [rad] ILS GS full deflection
    static const double _ils_lc_max;    ///< [rad] ILS LC full deflection

    static const double _vor_max;       ///< [rad] VOR full deflection

    /** @brief Constructor. */
    Manager();

    /** @brief Destructor. */
    virtual ~Manager();

    /** @brief Updates navigation. */
    void update();

    inline void setCourse( double course )   { _course  = course;  }

    inline void setFreqADF( int freq ) { _adf_freq = freq; }
    inline void setFreqNAV( int freq ) { _nav_freq = freq; }

private:

    const DataBase::ILS *_ils_loc;  ///<
    const DataBase::ILS *_ils_dme;  ///<
    const DataBase::ILS *_ils_gs;   ///<

    const DataBase::NAV *_vor;      ///<
    const DataBase::NAV *_ndb;      ///<

    fdm::WGS84 _aircraft_wgs;       ///<

    double _course;                 ///< [rad]

    int _adf_freq;                  ///< [kHz]
    int _nav_freq;                  ///< [kHz]

    bool _adf_visible;              ///<
    double _adf_bearing;            ///< [rad]

    bool _dme_visible;              ///<
    double _dme_distance;           ///< [m]

    bool _ils_gs_visible;           ///<
    bool _ils_lc_visible;           ///<
    double _ils_heading;            ///< [rad]
    double _ils_gs_deviation;       ///< [rad]
    double _ils_lc_deviation;       ///< [rad]
    double _ils_gs_norm;            ///< [-1.0;1.0]
    double _ils_lc_norm;            ///< [-1.0;1.0]

    NAV_CDI _nav_cdi;               ///<
    bool _nav_visible;              ///<
    double _nav_bearing;            ///< [rad]
    double _nav_deviation;          ///< [rad]

    double _nav_norm;               ///< [-1.0;1.0]

    double getAzimuth( const fdm::Vector3 &pos_wgs );
    double getDistance( const fdm::Vector3 &pos_wgs );

    void updateNavaids();
    void updateNavaidsActive();

    void updateADF();
    void updateILS();
    void updateNAV();
    void updateNAV( double azim, double course, double max );
};

} // end of nav namepsace

////////////////////////////////////////////////////////////////////////////////

#endif // NAV_MANAGER_H
