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
#ifndef HID_MANAGER_H
#define HID_MANAGER_H

////////////////////////////////////////////////////////////////////////////////

#include <string>

#include <fdm/fdm_Defines.h>

#include <hid/hid_Assignment.h>

////////////////////////////////////////////////////////////////////////////////

namespace hid
{

/**
 * @brief HID manager class.
 */
class Manager
{
public:

    static const std::string m_actionNames[ HID_MAX_ACTIONS ];  ///<
    static const std::string m_keysNames[ HID_MAX_KEYS ];       ///<

    static const double m_speedCtrl;        ///< [1/s]
    static const double m_speedTrim;        ///< [1/s]
    static const double m_speedBrakes;      ///< [1/s]
    static const double m_speedGear;        ///< [1/s]
    static const double m_speedFlaps;       ///< [1/s]
    static const double m_speedAirbrake;    ///< [1/s]
    static const double m_speedSpoilers;    ///< [1/s]
    static const double m_speedCollective;  ///< [1/s]
    static const double m_speedThrottle;    ///< [1/s]
    static const double m_speedMixture;     ///< [1/s]
    static const double m_speedPropeller;   ///< [1/s]

    /** */
    static inline Manager* instance()
    {
        if ( !m_instance )
        {
            m_instance = new Manager();
        }

        return m_instance;
    }

    /** */
    static Assignment::POVs getPOV( short pov_deg );

    static bool isAxis( Assignment::Action action );

    /** Destructor. */
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

    double getCtrlRoll()     const { return m_ctrlRoll;     }
    double getCtrlPitch()    const { return m_ctrlPitch;    }
    double getCtrlYaw()      const { return m_ctrlYaw;      }
    double getTrimRoll()     const { return m_trimRoll;     }
    double getTrimPitch()    const { return m_trimPitch;    }
    double getTrimYaw()      const { return m_trimYaw;      }
    double getBrakeLeft()    const { return m_brakeLeft;    }
    double getBrakeRight()   const { return m_brakeRight;   }
    double getParkingBrake() const { return m_parkingBrake; }
    double getLandingGear()  const { return m_landingGear;  }
    double getFlaps()        const { return m_flaps;        }
    double getAirbrake()     const { return m_airbrake;     }
    double getSpoilers()     const { return m_spoilers;     }
    double getCollective()   const { return m_collective;   }

    double getThrottle  ( int num ) const { return m_throttle  [ num ]; }
    double getMixture   ( int num ) const { return m_mixture   [ num ]; }
    double getPropeller ( int num ) const { return m_propeller [ num ]; }

    bool isLgHandleDown() const { return m_stateLandingGear; }

    /** */
    void setAssingment( Assignment::Action action, const Assignment &assignment );

    /** */
    void setKeysState( bool keysState[] );

private:

    static Manager *m_instance;                     ///<  instance of Manager singleton class

    Assignment m_assignments[ HID_MAX_ACTIONS ];    ///<

    bool m_keysState[ HID_MAX_KEYS ];               ///<

    double m_timeStep;          ///< [s] simulation time step

    short m_trigger;            ///< trigger

    double m_ctrlRoll;          ///< [-1.0,1.0]
    double m_ctrlPitch;         ///< [-1.0,1.0]
    double m_ctrlYaw;           ///< [-1.0,1.0]

    double m_trimRoll;          ///< [-1.0,1.0]
    double m_trimPitch;         ///< [-1.0,1.0]
    double m_trimYaw;           ///< [-1.0,1.0]

    double m_brakeLeft;         ///< [0.0,1.0]
    double m_brakeRight;        ///< [0.0,1.0]
    double m_parkingBrake;      ///< [0.0,1.0]

    double m_landingGear;       ///< [0.0,1.0]

    double m_flaps;             ///< [0.0,1.0]
    double m_airbrake;          ///< [0.0,1.0]
    double m_spoilers;          ///< [0.0,1.0]

    double m_collective;        ///< [0.0,1.0]

    double m_commonThrottle;    ///< [0.0,1.0]
    double m_commonMixture;     ///< [0.0,1.0]
    double m_commonPropeller;   ///< [0.0,1.0]

    double m_throttle  [ FDM_MAX_ENGINES ]; ///< [0.0,1.0]
    double m_mixture   [ FDM_MAX_ENGINES ]; ///< [0.0,1.0]
    double m_propeller [ FDM_MAX_ENGINES ]; ///< [0.0,1.0]

    bool m_prevLandingGearToggle;   ///<
    bool m_prevParkingBrakeToggle;  ///<
    bool m_prevSpoilersToggle;      ///<

    bool m_stateLandingGear;        ///<
    bool m_stateParkingBrake;       ///<
    bool m_stateSpoilers;           ///<

    /**
     * You should use static function instance() due to get refernce
     * to Manager class instance.
     */
    Manager();

    /** Using this constructor is forbidden. */
    Manager( const Manager & ) {}

    /** */
    void getAxisValue( const Assignment &assignment, double &value, int absolute = 0 );

    /** */
    void getRealValue( Assignment::Action decreaseAction,
                       Assignment::Action increaseAction,
                       double &value,
                       double speed,
                       double min,
                       double max,
                       bool autocenter = false );

    /** */
    void getRealValue( Assignment::Action applyAction,
                       double &value,
                       double speed,
                       double min,
                       double max );

    /** */
    void getRealValue( Assignment::Action toggleAction,
                       bool &togglePrev,
                       bool &state,
                       double &value,
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
