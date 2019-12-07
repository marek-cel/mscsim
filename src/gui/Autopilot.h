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
#ifndef AUTOPILOT_H
#define AUTOPILOT_H

////////////////////////////////////////////////////////////////////////////////

#include <QElapsedTimer>
#include <QObject>

#include <fdm/auto/fdm_Autopilot.h>
#include <fdm_c172/c172_Autopilot.h>

////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Autopilot class.
 */
class Autopilot: public QObject
{
    Q_OBJECT

public:

    Autopilot();

    virtual ~Autopilot();

    void init();
    void stop();

    void update( bool btn_dn, bool btn_up );

    void onPressedAP();
    void onPressedFD();

    void onPressedALT();
    void onPressedIAS();
    void onPressedENG();
    void onPressedARM();

    void onPressedHDG( double hdg );
    void onPressedNAV( double crs );
    void onPressedAPR();
    void onPressedBC();

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

protected:

    /** */
    void timerEvent( QTimerEvent *event );

private:

    typedef fdm::C172_Autopilot C172_Autopilot;

    QElapsedTimer *_timer;              ///<

    fdm::Autopilot *_autopilot;         ///<
    C172_Autopilot *_autopilot_c172;    ///<

    double _altitude;                   ///< [m]   desired altitude
    double _climbRate;                  ///< [m/s] desired climb rate

    int _timerId;
};

////////////////////////////////////////////////////////////////////////////////

#endif // AUTOPILOT_H
