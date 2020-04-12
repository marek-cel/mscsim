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
#ifndef G1000_GRS77_H
#define G1000_GRS77_H

////////////////////////////////////////////////////////////////////////////////

#include <sstream>

////////////////////////////////////////////////////////////////////////////////

namespace g1000
{

/**
 * @brief Garmin GRS 77 AHRS class.
 *
 * @see G1000 Integrated Flight Deck Cockpit Reference Guide
 * @see G1000 Integrated Flight Deck Pilots Guide
 * @see G1000 NXi Pilot's Guise Cessna Nav III
 * @see G1000 Pilot's Guide for Cessna Nav III
 * @see G1000 Guide for Designated Pilot Examiners and Certified Flight Instructors
 */
class GRS77
{
public:

    /** Constructor. */
    GRS77();

    /** Destructor. */
    virtual ~GRS77();
};

} // end of g1000 namespace

////////////////////////////////////////////////////////////////////////////////

#endif // G1000_GRS77_H
