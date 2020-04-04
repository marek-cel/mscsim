/****************************************************************************//*
 * Copyright (C) 2020 Marek M. Cel
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
#ifndef G1000_DATA_H
#define G1000_DATA_H

////////////////////////////////////////////////////////////////////////////////

namespace g1000
{

/** */
struct Data
{
    /** PFD data. */
    struct PFD
    {
        // adi
        double roll;                    ///< [rad] roll angle
        double pitch;                   ///< [rad] pitch angle
        double heading;                 ///< [rad] heading

        bool fd_visible;                ///< specify if flight director is visible
        double fd_pitch;                ///< [rad] flight director pitch
        double fd_roll;                 ///< [rad] flight director roll

        double altitude;                ///< [m] altitude
        double ias;                     ///< [m/s] indicated airspeed
        double tas;                     ///< [m/s] true airspeed
        double climbRate;               ///< [m/s] climb rate
        double slipSkid;                ///< [rad] slip skid

        double ref_pressure;            ///< [Pa] reference pressure

        double sel_course;              ///< [rad] selected course
        double sel_heading;             ///< [rad] selected heading
        double sel_airspeed;            ///< [m/s] selected airspeed
        double sel_altitude;            ///< [m]   selected altitude
        double sel_climbRate;           ///< [m/s] selected climb rate

        double com_1_act;               ///< [MHz]
        double com_1_sby;               ///< [MHz]
        double com_2_act;               ///< [MHz]
        double com_2_sby;               ///< [MHz]

        double nav_1_act;               ///< [MHz]
        double nav_1_sby;               ///< [MHz]
        double nav_2_act;               ///< [MHz]
        double nav_2_sby;               ///< [MHz]
    };

    PFD pfd;                            ///< PFD data
};

} // end of g1000 namespace

////////////////////////////////////////////////////////////////////////////////

#endif // G1000_DATA_H
