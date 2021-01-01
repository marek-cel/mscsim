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
#ifndef AUTOPILOT_H
#define AUTOPILOT_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/auto/fdm_Autopilot.h>

#include <fdm_c172/c172_GFC700_AP.h>
#include <fdm_c172/c172_KAP140_AP.h>
#include <fdm_c172/c172_KFC325_AP.h>

////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Autopilot class.
 */
class Autopilot
{
public:

    Autopilot();

    /** @brief Autopilot class destructor. */
    virtual ~Autopilot();

    void init();
    void stop();

    void update( double timeStep );
    void update( double timeStep, bool btn_dn, bool btn_up );

    void onPressedAP();
    void onPressedFD();

    void onPressedALT();
    void onPressedIAS();
    void onPressedENG();
    void onPressedARM();
    void onPressedVNV();
    void onPressedFLC();
    void onPressedVS();

    void onPressedHDG( double hdg );
    void onPressedNAV( double crs );
    void onPressedAPR();
    void onPressedBC();
    void onPressedREV();

    void onPressedYD();

    void onPressedSoftRide();
    void onPressedHalfBank();

    void onPressedTest();
    void onReleasedTest();

    double getAirspeed() const;
    double getAltitude() const;
    double getClimbRate() const;
    double getHeading() const;

    double getCmdRoll() const;
    double getCmdPitch() const;

    double getCtrlRoll() const;
    double getCtrlPitch() const;
    double getCtrlYaw() const;

    bool getLampAP() const;
    bool getLampFD() const;
    bool getLampYD() const;
    bool getLampALT() const;
    bool getLampIAS() const;
    bool getLampGS()  const;
    bool getLampHDG() const;
    bool getLampNAV() const;
    bool getLampAPR() const;
    bool getLampBC()  const;
    bool getLampNAV_ARM() const;
    bool getLampAPR_ARM() const;
    bool getLampSR() const;
    bool getLampHB() const;
    bool getLampTRIM() const;
    bool getLampVS()  const;
    bool getLampARM() const;

    double getMinAltitude()  const;
    double getMaxAltitude()  const;
    double getMinClimbRate() const;
    double getMaxClimbRate() const;

    bool isActiveAP() const;
    bool isActiveFD() const;
    bool isActiveYD() const;

    bool isActiveALT() const;
    bool isActiveIAS() const;
    bool isActiveVS()  const;
    bool isActiveARM() const;
    bool isActiveGS()  const;
    bool isActiveHDG() const;
    bool isActiveNAV() const;
    bool isActiveAPR() const;
    bool isActiveBC()  const;

    bool isInited() const;
    bool isWorking() const;

    void setAltitude( double altitude );
    void setClimbRate( double climbRate );
    void setCourse( double course );
    void setHeading( double heading );

private:

    fdm::Autopilot *_autopilot;             ///< autopilot

    fdm::C172_GFC700_AP *_c172_gfc700_ap;   ///< Cessna 172 Garmin GFC 700 autopilot
    fdm::C172_KAP140_AP *_c172_kap140_ap;   ///< Cessna 172 Bendix/King KAP 140 autopilot
    fdm::C172_KFC325_AP *_c172_kfc325_ap;   ///< Cessna 172 Bendix/King KFC 325 autopilot

    double _altitude;                       ///< [m]   desired altitude
    double _climbRate;                      ///< [m/s] desired climb rate
};

////////////////////////////////////////////////////////////////////////////////

#endif // AUTOPILOT_H
