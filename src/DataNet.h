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
#ifndef DATANET_H
#define DATANET_H

////////////////////////////////////////////////////////////////////////////////

struct DataNet
{
    /** Flight data. */
    struct FlightData
    {
        double latitude;                ///< [rad] geodetic latitude (positive north)
        double longitude;               ///< [rad] geodetic longitude (positive east)

        double altitude_asl;            ///< [m] altitude above mean sea level
        double altitude_agl;            ///< [m] altitude above ground level

        double roll;                    ///< [rad] roll angle
        double pitch;                   ///< [rad] pitch angle
        double heading;                 ///< [rad] heading

        double angleOfAttack;           ///< [rad] angle of attack
        double sideslipAngle;           ///< [rad] angle of sideslip

        double climbAngle;              ///< [rad] climb angle
        double trackAngle;              ///< [rad] track angle

        double slipSkidAngle;           ///< [rad] slip/skid angle

        double airspeed;                ///< [m/s] airspeed
        double machNumber;              ///< [-]   Mach number
        double climbRate;               ///< [m/s] climb rate

        double rollRate;                ///< [rad/s] roll rate  (angular velcoity p component expressed in BAS)
        double pitchRate;               ///< [rad/s] pitch rate (angular velcoity q component expressed in BAS)
        double yawRate;                 ///< [rad/s] yaw rate   (angular velcoity r component expressed in BAS)
        double turnRate;                ///< [rad/s] turn rate  (heading change rate)
    };

    /** Navigation data. */
    struct Navigation
    {
        double course;                  ///< [rad]
    };

    FlightData flightData;              ///< flight data
    Navigation navigation;              ///< navigation data
};

////////////////////////////////////////////////////////////////////////////////

#endif // DATANET_H
