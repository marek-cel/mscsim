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
#ifndef C172_KAP140_AP_H
#define C172_KAP140_AP_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/auto/fdm_Autopilot.h>

#include <fdm_c172/c172_KAP140_FD.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Bendix/King KAP 140 autopilot class.
 *
 * @see Bendix/King KAP 140 Autopilot System Pilot's Guide
 */
class C172_KAP140_AP : public Autopilot
{
public:

    /** Constructor. */
    C172_KAP140_AP();

    /** Destructor. */
    virtual ~C172_KAP140_AP();

    /**
     * Reads data.
     * @param dataNode XML node
     */
    void readData( XmlNode &dataNode );

    /** Initializes autopilot. */
    void initialize();

    /**
     * @brief Updates autopilot.
     * @param timeStep [s]
     * @param button_dn
     * @param button_up
     */
    void update( double timeStep, bool button_dn, bool button_up );

    void onPressedAP();
    void onPressedFD();

    void onPressedALT();

    void onPressedHDG();
    void onPressedNAV();
    void onPressedAPR();

private:

    C172_KAP140_FD *_fd;        ///< flight director
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // C172_KAP140_AP_H
