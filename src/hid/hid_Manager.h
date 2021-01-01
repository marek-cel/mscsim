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
#ifndef HID_MANAGER_H
#define HID_MANAGER_H

////////////////////////////////////////////////////////////////////////////////

#include <string>
#include <vector>

#include <fdm/fdm_Defines.h>

#include <sim/Singleton.h>

#include <hid/hid_Assignment.h>

////////////////////////////////////////////////////////////////////////////////

namespace hid
{

/**
 * @brief HID manager class.
 */
class Manager : public Singleton< Manager >
{
    friend class Singleton< Manager >;

public:

    static const std::string _actionNames[ HID_MAX_ACTIONS ];   ///<
    static const std::string _keysNames[ HID_MAX_KEYS ];        ///<

    static const double _speedCtrl;         ///< [1/s]
    static const double _speedTrim;         ///< [1/s]
    static const double _speedBrakes;       ///< [1/s]
    static const double _speedGear;         ///< [1/s]
    static const double _speedFlaps;        ///< [1/s]
    static const double _speedAirbrake;     ///< [1/s]
    static const double _speedSpoilers;     ///< [1/s]
    static const double _speedCollective;   ///< [1/s]
    static const double _speedThrottle;     ///< [1/s]
    static const double _speedMixture;      ///< [1/s]
    static const double _speedPropeller;    ///< [1/s]

    /** */
    static Assignment::POVs getPOV( short pov_deg );

    static bool isAxis( Assignment::Action action );

private:

    /**
     * You should use static function instance() due to get refernce
     * to Manager class instance.
     */
    Manager();

    /** Using this constructor is forbidden. */
    Manager( const Manager & ) {}

public:

    /** @brief Destructor. */
    virtual ~Manager();

    /** */
    void init();

    /** */
    void reset( bool onGround = true );

    /**
     * @param time step [s]
     * @param data input
     * @param data output
     */
    void update( double timeStep );

    double getCtrlRoll()     const { return _ctrlRoll;     }
    double getCtrlPitch()    const { return _ctrlPitch;    }
    double getCtrlYaw()      const { return _ctrlYaw;      }
    double getTrimRoll()     const { return _trimRoll;     }
    double getTrimPitch()    const { return _trimPitch;    }
    double getTrimYaw()      const { return _trimYaw;      }
    double getBrakeLeft()    const { return _brakeLeft;    }
    double getBrakeRight()   const { return _brakeRight;   }
    double getParkingBrake() const { return _parkingBrake; }
    double getLandingGear()  const { return _landingGear;  }
    double getFlaps()        const { return _flaps;        }
    double getAirbrake()     const { return _airbrake;     }
    double getSpoilers()     const { return _spoilers;     }
    double getCollective()   const { return _collective;   }

    double getThrottle  ( int num ) const { return _throttle  [ num ]; }
    double getMixture   ( int num ) const { return _mixture   [ num ]; }
    double getPropeller ( int num ) const { return _propeller [ num ]; }

    bool getTrigger() const { return _trigger; }
    bool getAP_Disc() const { return _ap_disc; }
    bool getCWS() const { return _cws; }


    bool isLgHandleDown() const { return _stateLandingGear; }

    /** */
    void setAssingment( Assignment::Action action, const Assignment &assignment );

    /** */
    void setKeysState( bool keysState[] );

    /** */
    void setNotches( const std::vector< double > &notches );

private:

    Assignment _assignments[ HID_MAX_ACTIONS ]; ///<

    bool _keysState[ HID_MAX_KEYS ];            ///<

    double _timeStep;                           ///< [s] simulation time step

    bool _trigger;                              ///< trigger
    bool _ap_disc;                              ///< autopilot disconnect
    bool _cws;                                  ///< cws

    double _ctrlRoll;                           ///< [-1.0,1.0]
    double _ctrlPitch;                          ///< [-1.0,1.0]
    double _ctrlYaw;                            ///< [-1.0,1.0]

    double _trimRoll;                           ///< [-1.0,1.0]
    double _trimPitch;                          ///< [-1.0,1.0]
    double _trimYaw;                            ///< [-1.0,1.0]

    double _brakeLeft;                          ///< [0.0,1.0]
    double _brakeRight;                         ///< [0.0,1.0]
    double _parkingBrake;                       ///< [0.0,1.0]

    double _landingGear;                        ///< [0.0,1.0]

    double _flaps;                              ///< [0.0,1.0]
    double _airbrake;                           ///< [0.0,1.0]
    double _spoilers;                           ///< [0.0,1.0]

    double _collective;                         ///< [0.0,1.0]

    double _commonThrottle;                     ///< [0.0,1.0]
    double _commonMixture;                      ///< [0.0,1.0]
    double _commonPropeller;                    ///< [0.0,1.0]

    double _throttle  [ FDM_MAX_ENGINES ];      ///< [0.0,1.0]
    double _mixture   [ FDM_MAX_ENGINES ];      ///< [0.0,1.0]
    double _propeller [ FDM_MAX_ENGINES ];      ///< [0.0,1.0]

    bool _prevFlapsExtend;
    bool _prevFlapsRetract;
    bool _prevLandingGearToggle;                ///<
    bool _prevParkingBrakeToggle;               ///<
    bool _prevSpoilersToggle;                   ///<

    bool _stateLandingGear;                     ///<
    bool _stateParkingBrake;                    ///<
    bool _stateSpoilers;                        ///<

    int _notch;                                 ///< current flaps notch

    std::vector< double > _notches;             ///< flaps notches

    /** */
    void getAxisValue( const Assignment &assignment, double *value, int absolute = 0 );

    /** */
    void getRealValue( Assignment::Action decreaseAction,
                       Assignment::Action increaseAction,
                       double *value,
                       double speed,
                       double min,
                       double max,
                       bool autocenter = false );

    /** */
    void getRealValue( Assignment::Action applyAction,
                       double *value,
                       double speed,
                       double min,
                       double max );

    /** */
    void getRealValue( Assignment::Action toggleAction,
                       bool *togglePrev,
                       bool *state,
                       double *value,
                       double speed,
                       double min,
                       double max );

    /** */
    bool getButtState( const Assignment &assignment );

    /** */
    void updateAxisActions();

    /** */
    void updateMiscActions();
};

} // end of hid namepsace

////////////////////////////////////////////////////////////////////////////////

#endif // HID_MANAGER_H
