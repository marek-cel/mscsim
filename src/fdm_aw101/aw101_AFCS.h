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
#ifndef AW101_AFCS_H
#define AW101_AFCS_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/ctrl/fdm_PID.h>
#include <fdm/utils/fdm_Vector3.h>
#include <fdm/xml/fdm_XmlNode.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief AW101 Automatic Flight Control System (AFCS) class.
 */
class AW101_AFCS
{
public:

    /** Constructor. */
    AW101_AFCS();

    /** Destructor. */
    virtual ~AW101_AFCS();

    /**
     * Reads data.
     * @param dataNode XML node
     */
    void readData( XmlNode &dataNode );

    void update( double timeStep,
                 double ctrlLat, double trimLat,
                 double ctrlLon, double trimLon,
                 double ctrlYaw, double trimYaw,
                 double climbRate,
                 const Angles &angles_ned,
                 const Vector3 &omg_bas );

    inline double getCyclicLat()  const { return _cyclic_lat; }
    inline double getCyclicLon()  const { return _cyclic_lon; }
    inline double getTailPitch()  const { return _tail_pitch; }

    inline double getCollective() const { return _collective; }

private:

    /** */
    struct Channel
    {
        double _auth;                   ///<
    };

    PID _pid_sas_roll;
    PID _pid_sas_pitch;
    PID _pid_sas_yaw;

    PID _pid_collective;

    double _cyclic_lat;                 ///<
    double _cyclic_lon;                 ///<
    double _tail_pitch;                 ///<

    double _collective;                 ///<

    virtual void readSAS( const XmlNode &dataNode, PID *pid );
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // AW101_AFCS_H
