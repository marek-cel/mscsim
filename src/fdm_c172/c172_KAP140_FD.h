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
#ifndef C172_KAP140_FD_H
#define C172_KAP140_FD_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/auto/fdm_FlightDirector.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

class C172_KAP140_AP;

/**
 * @brief Flight director class.
 *
 * @see Bociek S., Gruszecki J.: Uklady sterowania automatycznego samolotem, 1999, p.213. [in Polish]
 */
class C172_KAP140_FD : public FlightDirector
{
public:

    /** Lateral modes. */
    enum LatMode
    {
        LM_ROL = 0,     ///< roll hold
        LM_HDG,         ///< heading select
        LM_NAV,         ///< navigation
        LM_APR,         ///< approach
        LM_REV          ///< back course
    };

    /** Vertical modes. */
    enum VerMode
    {
        VM_PIT = 0,     ///< pitch hold
        VM_ALT,         ///< altitude hold
        VM_VS           ///< vertical speed
    };

    /** Constructor. */
    C172_KAP140_FD( const C172_KAP140_AP *ap );

    /** Destructor. */
    virtual ~C172_KAP140_FD();

    /**
     * Reads data.
     * @param dataNode XML node
     */
    void readData( XmlNode &dataNode );

    /**
     * Updates flight director.
     */
    void update( double timeStep,
                 double heading,
                 double altitude, double airspeed,
                 double turn_rate, double climb_rate,
                 double dme_distance,
                 double nav_deviation, bool nav_active,
                 double loc_deviation, bool loc_active,
                 double gs_deviation,  bool gs_active );

    inline LatMode getLatMode() const { return _lat_mode; }
    inline VerMode getVerMode() const { return _ver_mode; }

    inline void setLatMode( LatMode lat_mode ) { _lat_mode = lat_mode; }
    inline void setVerMode( VerMode ver_mode ) { _ver_mode = ver_mode; }

    void toggleLatMode( LatMode lat_mode );
    void toggleVerMode( VerMode ver_mode );

protected:

    const C172_KAP140_AP *_ap;  ///< autopilot

    LatMode _lat_mode;          ///< lateral mode
    VerMode _ver_mode;          ///< vertical mode
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // C172_KAP140_FD_H
