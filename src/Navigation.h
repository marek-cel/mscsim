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

#include <fdmUtils/fdm_WGS84.h>

////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Navigation class.
 */
class Navigation
{
public:

    struct ILS
    {
        fdm::Vector3 pos_gs_wgs;        ///< [m] ILS GS position expressed in WGS
        fdm::Vector3 pos_lc_wgs;        ///< [m] ILS LC position expressed in WGS

        double glide_slope;             ///< [rad] glide slope
        double heading;                 ///< [rad] runway heading
    };

    struct NDB
    {
        fdm::Vector3 pos_wgs;           ///< [m] NDB position expressed in WGS
    };

    struct VOR
    {
        fdm::Vector3 pos_wgs;           ///< [m] VOR position expressed in WGS
    };

    static const double m_range_ils;    ///< [m] ILS range
    static const double m_range_ndb;    ///< [m] NDB range
    static const double m_range_vor;    ///< [m] VOR range

    static const double m_ils_gs_max;   ///< [rad] ILS GS full deflection
    static const double m_ils_lc_max;   ///< [rad] ILS LC full deflection

    static const double m_vor_max;      ///< [rad] VOR full deflection

    /** Constructor. */
    Navigation();

    /** Destructor. */
    virtual ~Navigation();

    /** Updates navigation. */
    void update();

private:

    ILS m_ils;                  ///<
    NDB m_ndb;                  ///<
    VOR m_vor;                  ///<

    fdm::WGS84 m_aircraft_wgs;  ///<

    bool  m_adf_visible;        ///<
    float m_adf_bearing;        ///< [rad]

    bool  m_ils_gs_visible;     ///<
    bool  m_ils_lc_visible;     ///<
    float m_ils_gs_deviation;   ///< [-1.0,1.0]
    float m_ils_lc_deviation;   ///< [-1.0,1.0]

    bool  m_nav_visible;        ///<
    float m_nav_deviation;      ///< [-1.0,1.0]
    float m_nav_distance;       ///< [-1.0,1.0]

    double getAzimuth( const fdm::Vector3 &pos_wgs );
    double getDistance( const fdm::Vector3 &pos_wgs );

    fdm::Vector3 readPosition( const QDomElement &node );

    void updateADF();
    void updateILS();
    void updateNav();
};

////////////////////////////////////////////////////////////////////////////////

#endif // NAVIGATION_H
