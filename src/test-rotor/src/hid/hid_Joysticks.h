/****************************************************************************//*
 * Copyright (C) 2020 Marek M. Cel
 *
 * Creative Commons Legal Code
 *
 * CC0 1.0 Universal
 *
 *     CREATIVE COMMONS CORPORATION IS NOT A LAW FIRM AND DOES NOT PROVIDE
 *     LEGAL SERVICES. DISTRIBUTION OF THIS DOCUMENT DOES NOT CREATE AN
 *     ATTORNEY-CLIENT RELATIONSHIP. CREATIVE COMMONS PROVIDES THIS
 *     INFORMATION ON AN "AS-IS" BASIS. CREATIVE COMMONS MAKES NO WARRANTIES
 *     REGARDING THE USE OF THIS DOCUMENT OR THE INFORMATION OR WORKS
 *     PROVIDED HEREUNDER, AND DISCLAIMS LIABILITY FOR DAMAGES RESULTING FROM
 *     THE USE OF THIS DOCUMENT OR THE INFORMATION OR WORKS PROVIDED
 *     HEREUNDER.
 *
 * Statement of Purpose
 *
 * The laws of most jurisdictions throughout the world automatically confer
 * exclusive Copyright and Related Rights (defined below) upon the creator
 * and subsequent owner(s) (each and all, an "owner") of an original work of
 * authorship and/or a database (each, a "Work").
 *
 * Certain owners wish to permanently relinquish those rights to a Work for
 * the purpose of contributing to a commons of creative, cultural and
 * scientific works ("Commons") that the public can reliably and without fear
 * of later claims of infringement build upon, modify, incorporate in other
 * works, reuse and redistribute as freely as possible in any form whatsoever
 * and for any purposes, including without limitation commercial purposes.
 * These owners may contribute to the Commons to promote the ideal of a free
 * culture and the further production of creative, cultural and scientific
 * works, or to gain reputation or greater distribution for their Work in
 * part through the use and efforts of others.
 *
 * For these and/or other purposes and motivations, and without any
 * expectation of additional consideration or compensation, the person
 * associating CC0 with a Work (the "Affirmer"), to the extent that he or she
 * is an owner of Copyright and Related Rights in the Work, voluntarily
 * elects to apply CC0 to the Work and publicly distribute the Work under its
 * terms, with knowledge of his or her Copyright and Related Rights in the
 * Work and the meaning and intended legal effect of CC0 on those rights.
 *
 * 1. Copyright and Related Rights. A Work made available under CC0 may be
 * protected by copyright and related or neighboring rights ("Copyright and
 * Related Rights"). Copyright and Related Rights include, but are not
 * limited to, the following:
 *
 *   i. the right to reproduce, adapt, distribute, perform, display,
 *      communicate, and translate a Work;
 *  ii. moral rights retained by the original author(s) and/or performer(s);
 * iii. publicity and privacy rights pertaining to a person's image or
 *      likeness depicted in a Work;
 *  iv. rights protecting against unfair competition in regards to a Work,
 *      subject to the limitations in paragraph 4(a), below;
 *   v. rights protecting the extraction, dissemination, use and reuse of data
 *      in a Work;
 *  vi. database rights (such as those arising under Directive 96/9/EC of the
 *      European Parliament and of the Council of 11 March 1996 on the legal
 *      protection of databases, and under any national implementation
 *      thereof, including any amended or successor version of such
 *      directive); and
 * vii. other similar, equivalent or corresponding rights throughout the
 *      world based on applicable law or treaty, and any national
 *      implementations thereof.
 *
 * 2. Waiver. To the greatest extent permitted by, but not in contravention
 * of, applicable law, Affirmer hereby overtly, fully, permanently,
 * irrevocably and unconditionally waives, abandons, and surrenders all of
 * Affirmer's Copyright and Related Rights and associated claims and causes
 * of action, whether now known or unknown (including existing as well as
 * future claims and causes of action), in the Work (i) in all territories
 * worldwide, (ii) for the maximum duration provided by applicable law or
 * treaty (including future time extensions), (iii) in any current or future
 * medium and for any number of copies, and (iv) for any purpose whatsoever,
 * including without limitation commercial, advertising or promotional
 * purposes (the "Waiver"). Affirmer makes the Waiver for the benefit of each
 * member of the public at large and to the detriment of Affirmer's heirs and
 * successors, fully intending that such Waiver shall not be subject to
 * revocation, rescission, cancellation, termination, or any other legal or
 * equitable action to disrupt the quiet enjoyment of the Work by the public
 * as contemplated by Affirmer's express Statement of Purpose.
 *
 * 3. Public License Fallback. Should any part of the Waiver for any reason
 * be judged legally invalid or ineffective under applicable law, then the
 * Waiver shall be preserved to the maximum extent permitted taking into
 * account Affirmer's express Statement of Purpose. In addition, to the
 * extent the Waiver is so judged Affirmer hereby grants to each affected
 * person a royalty-free, non transferable, non sublicensable, non exclusive,
 * irrevocable and unconditional license to exercise Affirmer's Copyright and
 * Related Rights in the Work (i) in all territories worldwide, (ii) for the
 * maximum duration provided by applicable law or treaty (including future
 * time extensions), (iii) in any current or future medium and for any number
 * of copies, and (iv) for any purpose whatsoever, including without
 * limitation commercial, advertising or promotional purposes (the
 * "License"). The License shall be deemed effective as of the date CC0 was
 * applied by Affirmer to the Work. Should any part of the License for any
 * reason be judged legally invalid or ineffective under applicable law, such
 * partial invalidity or ineffectiveness shall not invalidate the remainder
 * of the License, and in such case Affirmer hereby affirms that he or she
 * will not (i) exercise any of his or her remaining Copyright and Related
 * Rights in the Work or (ii) assert any associated claims and causes of
 * action with respect to the Work, in either case contrary to Affirmer's
 * express Statement of Purpose.
 *
 * 4. Limitations and Disclaimers.
 *
 *  a. No trademark or patent rights held by Affirmer are waived, abandoned,
 *     surrendered, licensed or otherwise affected by this document.
 *  b. Affirmer offers the Work as-is and makes no representations or
 *     warranties of any kind concerning the Work, express, implied,
 *     statutory or otherwise, including without limitation warranties of
 *     title, merchantability, fitness for a particular purpose, non
 *     infringement, or the absence of latent or other defects, accuracy, or
 *     the present or absence of errors, whether or not discoverable, all to
 *     the greatest extent permissible under applicable law.
 *  c. Affirmer disclaims responsibility for clearing rights of other persons
 *     that may apply to the Work or any use thereof, including without
 *     limitation any person's Copyright and Related Rights in the Work.
 *     Further, Affirmer disclaims responsibility for obtaining any necessary
 *     consents, permissions or other rights required for any use of the
 *     Work.
 *  d. Affirmer understands and acknowledges that Creative Commons is not a
 *     party to this document and has no duty or obligation with respect to
 *     this CC0 or use of the Work.
 *
 ******************************************************************************/

#ifndef HID_JOYSTICKS_H
#define HID_JOYSTICKS_H

////////////////////////////////////////////////////////////////////////////////

#ifdef WIN32
#       define HID_WINMM_JOYSTICK
#else
#   ifdef _LINUX_
#       define HID_LINUX_JOYSTICK
#   endif
#endif

////////////////////////////////////////////////////////////////////////////////

#include <string>

#ifdef HID_LINUX_JOYSTICK
#   include <fcntl.h>
#   include <unistd.h>
#   include <linux/joystick.h>
#endif

#ifdef HID_WINMM_JOYSTICK
#   include <Windows.h>
#endif

#include <hid/hid_Singleton.h>

////////////////////////////////////////////////////////////////////////////////

#ifdef HID_LINUX_JOYSTICK
#   define HID_MAX_JOYS 32
#   define HID_MAX_POVS 4
#   define HID_MAX_AXES 24
#   define HID_MAX_BUTT 32
#   define HID_AXIS_RANGE 32767.0f
#endif

#ifdef HID_WINMM_JOYSTICK
#   define HID_MAX_JOYS 2
#   define HID_MAX_POVS 1
#   define HID_MAX_AXES 6
#   define HID_MAX_BUTT 32
#   define HID_AXIS_RANGE 65535.0f
#endif

#ifndef HID_AXIS_RANGE
#   error 'HID_AXIS_RANGE' not defined!
#endif

////////////////////////////////////////////////////////////////////////////////

namespace hid
{

/** This class handles joystick like input devices. */
class Joysticks : public Singleton< Joysticks >
{
    friend class Singleton< Joysticks >;

public:

    /** */
    enum Axes
    {
        AxisX = 0,                  ///<  0: X-axis deflection
        AxisY,                      ///<  1: Y-axis deflection

#       ifdef HID_LINUX_JOYSTICK
        AxisZ,                      ///<  2: Z-axis deflection
        AxisRX,                     ///<  3: Rotation about X-axis
        AxisRY,                     ///<  4: Rotation about Y-axis
        AxisRZ,                     ///<  5: Rotation about Z-axis
        Throttle,                   ///<  6: Throttle
        Rudder,                     ///<  7: Rudder
        Gas,                        ///<  8: Gas
        Wheel,                      ///<  9: Wheel
        Brake,                      ///< 10: Brake
        Hat0X,                      ///< 11: Hat 0 X-axis
        Hat0Y,                      ///< 12: Hat 0 Y-axis
        Hat1X,                      ///< 13: Hat 1 X-axis
        Hat1Y,                      ///< 14: Hat 1 Y-axis
        Hat2X,                      ///< 15: Hat 2 X-axis
        Hat2Y,                      ///< 16: Hat 2 Y-axis
        Hat3X,                      ///< 17: Hat 3 X-axis
        Hat3Y,                      ///< 18: Hat 3 Y-axis
        Pressure,                   ///< 19:
        Distance,                   ///< 20:
        TiltX,                      ///< 21:
        TiltY,                      ///< 22:
        ToolWidth                   ///< 23:
#       endif

#       ifdef HID_WINMM_JOYSTICK
        AxisZ,                      ///<  2: Z-axis deflection
        AxisR,                      ///<  3:
        AxisU,                      ///<  4:
        AxisV                       ///<  5:
#       endif
    };

    /** */
    struct Data
    {
        std::string name;               ///< device name
        
        float axis[ HID_MAX_AXES ];     ///< -1.0 ... 1.0 normalized axis position
        bool  butt[ HID_MAX_BUTT ];     ///< false: released, true: pressed
        short povs[ HID_MAX_POVS ];     ///< [deg] POVs

        short axisCount;                ///< number of device axes
        short buttCount;                ///< number of devise buttons
        short povsCount;                ///< number of devices POVs

        bool active;                    ///< specifies active device
        bool feedback;                  ///< specifies FF capable device

        bool hasAxis[ HID_MAX_AXES ];   ///<

#       ifdef HID_LINUX_JOYSTICK
        bool hasPOV[ HID_MAX_POVS ];    ///<

        Axes axesMap[ HID_MAX_AXES + 2*HID_MAX_POVS ];  ///<
#       endif

#       ifdef HID_WINMM_JOYSTICK
        bool hasPOV;                    ///<
#       endif
    };

private:

    /**
     * You should use static function getInstance() due to get refernce
     * to Joytsicks class instance.
     */
    Joysticks();

    /** Using this constructor is forbidden. */
    Joysticks( const Joysticks & ) : Singleton< Joysticks >() {}

public:

    static const std::string _axisNames[ HID_MAX_AXES ];

    /** Destructor. */
    virtual ~Joysticks();

    /** This function initiate Joystick object. */
    void init();
    
    /** This function updates Joystick object. */
    void update();

    /** @return number of active joysticks */
    short getJoysCount() const;

    /**
     * @param joystick index number
     * @return joystick data
     */
    Joysticks::Data getJoyData( short joyNum ) const;

private:

    short _joysCount;   ///< number of active joysticks

    Joysticks::Data _joyData[ HID_MAX_JOYS ];

#   ifdef HID_LINUX_JOYSTICK
    int _joysFD[ HID_MAX_JOYS ];
    js_event _joyEvent;
#   endif

#   ifdef HID_WINMM_JOYSTICK
    DWORD _buttons[ HID_MAX_BUTT ];
#   endif
};

} // end of hid namepsace

////////////////////////////////////////////////////////////////////////////////

#endif // HID_JOYSTICKS_H
