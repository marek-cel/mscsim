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
#ifndef G1000_GRS_H
#define G1000_GRS_H

////////////////////////////////////////////////////////////////////////////////

#include <g1000/g1000_Input.h>

////////////////////////////////////////////////////////////////////////////////

namespace g1000
{

class IFD; // IFD class forward declaration

/**
 * @brief Garmin GRS 77 AHRS class.
 */
class GRS
{
public:

    /** Constructor. */
    GRS( IFD *ifd );

    /** Destructor. */
    virtual ~GRS();

    /** Updates AHRS. */
    void update();

    inline double getRoll()    const { return _roll;    }
    inline double getPitch()   const { return _pitch;   }
    inline double getHeading() const { return _heading; }

    inline double getSlipSkid() const { return _slipSkid; }

private:

    const IFD *_ifd;        ///< IFD object

    double _roll;           ///< [rad]
    double _pitch;          ///< [rad]
    double _heading;        ///< [rad]

    double _slipSkid;       ///< [rad]
};

} // end of g1000 namespace

////////////////////////////////////////////////////////////////////////////////

#endif // G1000_GRS_H
