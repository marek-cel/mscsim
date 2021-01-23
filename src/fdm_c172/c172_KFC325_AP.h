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
#ifndef C172_KFC325_AP_H
#define C172_KFC325_AP_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/auto/fdm_Autopilot.h>

#include <fdm_c172/c172_KFC325_FD.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Bendix/King KFC 325 autopilot class.
 *
 * @see Bendix King KFC 325 Pilot's Guide
 */
class C172_KFC325_AP : public Autopilot
{
public:

    /** Constructor. */
    C172_KFC325_AP();

    /** Destructor. */
    virtual ~C172_KFC325_AP();

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
    void onPressedIAS();
    void onPressedENG();
    void onPressedARM();

    void onPressedHDG();
    void onPressedNAV();
    void onPressedAPR();
    void onPressedBC();

    void onPressedYD();

    void onPressedSoftRide();
    void onPressedHalfBank();

    void onPressedTest();
    void onReleasedTest();

    inline bool getLampAP()  const { return _testing || isActiveAP(); }
    inline bool getLampFD()  const { return _testing || isActiveFD(); }
    inline bool getLampYD()  const { return _testing || isActiveYD(); }

    inline bool getLampALT() const { return _testing || isActiveALT(); }
    inline bool getLampIAS() const { return _testing || isActiveIAS(); }
    inline bool getLampGS()  const { return _testing || isActiveGS();  }
    inline bool getLampHDG() const { return _testing || isActiveHDG(); }
    inline bool getLampNAV() const { return _testing || isActiveNAV() || isArmedNAV(); }
    inline bool getLampAPR() const { return _testing || isActiveOrArmedAPR() || isActiveOrArmedBC(); }
    inline bool getLampBC()  const { return _testing || isActiveOrArmedBC(); }

    inline bool getLampNAV_ARM() const { return _testing || isArmedNAV(); }
    inline bool getLampAPR_ARM() const { return _testing || isArmedAPR() || isArmedBC(); }

    inline bool getLampSR() const { return _testing || isActiveSoftRide(); }
    inline bool getLampHB() const { return _testing || isActiveHalfBank(); }

    inline bool getLampTRIM() const { return _testing || false; }

    inline bool getLampVS()  const { return isActiveVS();  }
    inline bool getLampARM() const { return isActiveARM(); }

    inline bool isActiveALT() const { return _fd->getVerMode() == C172_KFC325_FD::VM_ALT; }
    inline bool isActiveIAS() const { return _fd->getVerMode() == C172_KFC325_FD::VM_IAS; }
    inline bool isActiveVS()  const { return _fd->getVerMode() == C172_KFC325_FD::VM_VS;  }
    inline bool isActiveARM() const { return _fd->getVerMode() == C172_KFC325_FD::VM_ARM; }
    inline bool isActiveGS()  const { return _fd->getVerMode() == C172_KFC325_FD::VM_GS;  }
    inline bool isActiveHDG() const { return _fd->getLatMode() == C172_KFC325_FD::LM_HDG; }
    inline bool isActiveNAV() const { return _fd->getLatMode() == C172_KFC325_FD::LM_NAV; }
    inline bool isActiveAPR() const { return _fd->getLatMode() == C172_KFC325_FD::LM_APR; }
    inline bool isActiveBC()  const { return _fd->getLatMode() == C172_KFC325_FD::LM_BC;  }

    inline bool isActiveSoftRide() const { return _softRide; }
    inline bool isActiveHalfBank() const { return _fd->isActiveHalfBank(); }

    inline bool isArmedNAV() const { return _fd->getArmMode() == C172_KFC325_FD::ARM_NAV; }
    inline bool isArmedAPR() const { return _fd->getArmMode() == C172_KFC325_FD::ARM_APR; }
    inline bool isArmedBC()  const { return _fd->getArmMode() == C172_KFC325_FD::ARM_BC;  }

    inline bool isActiveOrArmedAPR() const { return isActiveAPR() || isArmedAPR(); }
    inline bool isActiveOrArmedBC()  const { return isActiveBC()  || isArmedBC();  }

    void setHeadingILS( double heading_ils );

private:

    C172_KFC325_FD *_fd;        ///< flight director

    double _rate_pitch;         ///< [deg/s] pitch rate of change
    double _rate_alt;           ///< [m/s] altitude rate of change
    double _rate_ias;           ///< [(m/s)/s] airspeed rate of change
    double _rate_vs;            ///< [(m/s)/s] vertical speed rate of change

    double _softRideCoef;       ///< [-] soft ride coefficient

    bool _softRide;             ///< specifies if soft ride is engaged
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // C172_KFC325_AP_H
