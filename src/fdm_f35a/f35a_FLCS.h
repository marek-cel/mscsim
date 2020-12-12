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
#ifndef F35A_FLCS_H
#define F35A_FLCS_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/ctrl/fdm_Filter2.h>
#include <fdm/ctrl/fdm_Lag.h>
#include <fdm/ctrl/fdm_LeadLag.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief F-35A Flight Control System class.
 */
class F35A_FLCS
{
public:

    /** Constructor. */
    F35A_FLCS();

    /** Destructor. */
    ~F35A_FLCS();

    /** Updates model. */
    void update( double time_step );

private:

    double _time_step;                  ///< [s] time step
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // F35A_FLCS_H
