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
#ifndef G1000_GDC_H
#define G1000_GDC_H

////////////////////////////////////////////////////////////////////////////////

#include <g1000/g1000_Input.h>

////////////////////////////////////////////////////////////////////////////////

namespace g1000
{

class IFD; // IFD class forward declaration

/**
 * @brief Garmin GDC 74A Air Data Computer class.
 */
class GDC
{
public:

    /** Constructor. */
    GDC( IFD *ifd );

    /** Destructor. */
    virtual ~GDC();

    /** Updates Air Data Computer. */
    void update();

    inline double getAltitude()  const { return _altitude;  }
    inline double getClimbRate() const { return _climbRate; }

    inline double getIAS() const { return _ias; }
    inline double getTAS() const { return _tas; }

private:

    const IFD *_ifd;        ///< IFD object

    double _altitude;       ///< [m] barometric altitude
    double _climbRate;      ///< [m/s] climb rate

    double _ias;            ///< [m/s] indicated airspeed
    double _tas;            ///< [m/s] true airspeed
};

} // end of g1000 namespace

////////////////////////////////////////////////////////////////////////////////

#endif // G1000_GDC_H
