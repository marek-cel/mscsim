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
#ifndef XH_AFCS_H
#define XH_AFCS_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/ctrl/fdm_PID.h>
#include <fdm/utils/fdm_Vector3.h>
#include <fdm/xml/fdm_XmlNode.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief X/H Automatic Flight Control System class.
 */
class XH_AFCS
{
public:

    /** Constructor. */
    XH_AFCS();

    /** Destructor. */
    virtual ~XH_AFCS();

    /**
     * Reads data.
     * @param dataNode XML node
     */
    void readData( XmlNode &dataNode );

    void update( double timeStep,
                 double ctrlLat, double trimLat,
                 double ctrlLon, double trimLon,
                 double ctrlYaw, double trimYaw,
                 const Angles &angles_ned,
                 const Vector3 &omg_bas );

    inline double getCyclicLat()  const { return _cyclic_lat; }
    inline double getCyclicLon()  const { return _cyclic_lon; }
    inline double getTailPitch()  const { return _tail_pitch; }

private:

    /** */
    struct Channel
    {
        double _auth;                   ///<
    };

    PID _pid_sas_roll;
    PID _pid_sas_pitch;
    PID _pid_sas_yaw;

    double _cyclic_lat;                 ///< [rad]
    double _cyclic_lon;                 ///< [rad]
    double _tail_pitch;                 ///< [rad]

    virtual void readSAS( const XmlNode &dataNode, PID *pid );
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // XH_AFCS_H
