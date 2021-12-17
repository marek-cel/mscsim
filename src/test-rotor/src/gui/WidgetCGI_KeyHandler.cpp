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

#include <gui/WidgetCGI.h>

////////////////////////////////////////////////////////////////////////////////

WidgetCGI::KeyHandler::KeyHandler( WidgetCGI *widgetCGI ) :
    _widgetCGI ( widgetCGI )
{
    for ( int i = 0; i < HID_MAX_KEYS; i++ ) _keysState[ i ] = false;
}

////////////////////////////////////////////////////////////////////////////////

bool WidgetCGI::KeyHandler::handle( const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter & )
{
#   ifndef SIM_NOKEYINPUT
    switch( ea.getEventType() )
    {
    case osgGA::GUIEventAdapter::KEYDOWN:
        return handleKeyDn( ea );
        break;

    case osgGA::GUIEventAdapter::KEYUP:
        return handleKeyUp( ea );
        break;

    default:
        return false;
    }
#   endif

    return false;
}

////////////////////////////////////////////////////////////////////////////////

void WidgetCGI::KeyHandler::keyDn( hid::Assignment::Key key )
{
    _keysState[ key ] = true;
}

////////////////////////////////////////////////////////////////////////////////

void WidgetCGI::KeyHandler::keyUp( hid::Assignment::Key key )
{
    _keysState[ key ] = false;
}

////////////////////////////////////////////////////////////////////////////////

int WidgetCGI::KeyHandler::getKeyIndex( int key )
{
    switch ( key )
    {
    case osgGA::GUIEventAdapter::KEY_0:
    case osgGA::GUIEventAdapter::KEY_KP_0:
        return hid::Assignment::Key0;
        break;

    case osgGA::GUIEventAdapter::KEY_1:
    case osgGA::GUIEventAdapter::KEY_KP_1:
        return hid::Assignment::Key1;
        break;

    case osgGA::GUIEventAdapter::KEY_2:
    case osgGA::GUIEventAdapter::KEY_KP_2:
        return hid::Assignment::Key2;
        break;

    case osgGA::GUIEventAdapter::KEY_3:
    case osgGA::GUIEventAdapter::KEY_KP_3:
        return hid::Assignment::Key3;
        break;

    case osgGA::GUIEventAdapter::KEY_4:
    case osgGA::GUIEventAdapter::KEY_KP_4:
        return hid::Assignment::Key4;
        break;

    case osgGA::GUIEventAdapter::KEY_5:
    case osgGA::GUIEventAdapter::KEY_KP_5:
        return hid::Assignment::Key5;
        break;

    case osgGA::GUIEventAdapter::KEY_6:
    case osgGA::GUIEventAdapter::KEY_KP_6:
        return hid::Assignment::Key6;
        break;

    case osgGA::GUIEventAdapter::KEY_7:
    case osgGA::GUIEventAdapter::KEY_KP_7:
        return hid::Assignment::Key7;
        break;

    case osgGA::GUIEventAdapter::KEY_8:
    case osgGA::GUIEventAdapter::KEY_KP_8:
        return hid::Assignment::Key8;
        break;

    case osgGA::GUIEventAdapter::KEY_9:
    case osgGA::GUIEventAdapter::KEY_KP_9:
        return hid::Assignment::Key9;
        break;

    case osgGA::GUIEventAdapter::KEY_A:
    case 'A':
        return hid::Assignment::KeyA;
        break;

    case osgGA::GUIEventAdapter::KEY_B:
    case 'B':
        return hid::Assignment::KeyB;
        break;

    case osgGA::GUIEventAdapter::KEY_C:
    case 'C':
        return hid::Assignment::KeyC;
        break;

    case osgGA::GUIEventAdapter::KEY_D:
    case 'D':
        return hid::Assignment::KeyD;
        break;

    case osgGA::GUIEventAdapter::KEY_E:
    case 'E':
        return hid::Assignment::KeyE;
        break;

    case osgGA::GUIEventAdapter::KEY_F:
    case 'F':
        return hid::Assignment::KeyF;
        break;

    case osgGA::GUIEventAdapter::KEY_G:
    case 'G':
        return hid::Assignment::KeyG;
        break;

    case osgGA::GUIEventAdapter::KEY_H:
    case 'H':
        return hid::Assignment::KeyH;
        break;

    case osgGA::GUIEventAdapter::KEY_I:
    case 'I':
        return hid::Assignment::KeyI;
        break;

    case osgGA::GUIEventAdapter::KEY_J:
    case 'J':
        return hid::Assignment::KeyJ;
        break;

    case osgGA::GUIEventAdapter::KEY_K:
    case 'K':
        return hid::Assignment::KeyK;
        break;

    case osgGA::GUIEventAdapter::KEY_L:
    case 'L':
        return hid::Assignment::KeyL;
        break;

    case osgGA::GUIEventAdapter::KEY_M:
    case 'M':
        return hid::Assignment::KeyM;
        break;

    case osgGA::GUIEventAdapter::KEY_N:
    case 'N':
        return hid::Assignment::KeyN;
        break;

    case osgGA::GUIEventAdapter::KEY_O:
    case 'O':
        return hid::Assignment::KeyO;
        break;

    case osgGA::GUIEventAdapter::KEY_P:
    case 'P':
        return hid::Assignment::KeyP;
        break;

    case osgGA::GUIEventAdapter::KEY_Q:
    case 'Q':
        return hid::Assignment::KeyQ;
        break;

    case osgGA::GUIEventAdapter::KEY_R:
    case 'R':
        return hid::Assignment::KeyR;
        break;

    case osgGA::GUIEventAdapter::KEY_S:
    case 'S':
        return hid::Assignment::KeyS;
        break;

    case osgGA::GUIEventAdapter::KEY_T:
    case 'T':
        return hid::Assignment::KeyT;
        break;

    case osgGA::GUIEventAdapter::KEY_U:
    case 'U':
        return hid::Assignment::KeyU;
        break;

    case osgGA::GUIEventAdapter::KEY_V:
    case 'V':
        return hid::Assignment::KeyV;
        break;

    case osgGA::GUIEventAdapter::KEY_W:
    case 'W':
        return hid::Assignment::KeyW;
        break;

    case osgGA::GUIEventAdapter::KEY_X:
    case 'X':
        return hid::Assignment::KeyX;
        break;

    case osgGA::GUIEventAdapter::KEY_Y:
    case 'Y':
        return hid::Assignment::KeyY;
        break;

    case osgGA::GUIEventAdapter::KEY_Z:
    case 'Z':
        return hid::Assignment::KeyZ;
        break;

    case osgGA::GUIEventAdapter::KEY_Backquote:
        return hid::Assignment::KeyBackquote;
        break;

    case osgGA::GUIEventAdapter::KEY_Minus:
        return hid::Assignment::KeyMinus;
        break;

    case osgGA::GUIEventAdapter::KEY_Equals:
        return hid::Assignment::KeyEquals;
        break;

    case osgGA::GUIEventAdapter::KEY_Leftbracket:
        return hid::Assignment::KeyLeftBracket;
        break;

    case osgGA::GUIEventAdapter::KEY_Rightbracket:
        return hid::Assignment::KeyRightBracket;
        break;

    case osgGA::GUIEventAdapter::KEY_Semicolon:
        return hid::Assignment::KeySemicolon;
        break;

    case osgGA::GUIEventAdapter::KEY_Quote:
        return hid::Assignment::KeyQuote;
        break;

    case osgGA::GUIEventAdapter::KEY_Comma:
        return hid::Assignment::KeyComma;
        break;

    case osgGA::GUIEventAdapter::KEY_Period:
        return hid::Assignment::KeyPeriod;
        break;

    case osgGA::GUIEventAdapter::KEY_Slash:
        return hid::Assignment::KeySlash;
        break;

    case osgGA::GUIEventAdapter::KEY_Space:
        return hid::Assignment::KeySpace;
        break;

    case osgGA::GUIEventAdapter::KEY_Left:
    case osgGA::GUIEventAdapter::KEY_KP_Left:
        return hid::Assignment::KeyLeft;
        break;

    case osgGA::GUIEventAdapter::KEY_Right:
    case osgGA::GUIEventAdapter::KEY_KP_Right:
        return hid::Assignment::KeyRight;
        break;

    case osgGA::GUIEventAdapter::KEY_Up:
    case osgGA::GUIEventAdapter::KEY_KP_Up:
        return hid::Assignment::KeyUp;
        break;

    case osgGA::GUIEventAdapter::KEY_Down:
    case osgGA::GUIEventAdapter::KEY_KP_Down:
        return hid::Assignment::KeyDown;
        break;

    case osgGA::GUIEventAdapter::KEY_Insert:
    case osgGA::GUIEventAdapter::KEY_KP_Insert:
        return hid::Assignment::KeyInsert;
        break;

    case osgGA::GUIEventAdapter::KEY_Delete:
    case osgGA::GUIEventAdapter::KEY_KP_Delete:
        return hid::Assignment::KeyDelete;
        break;

    case osgGA::GUIEventAdapter::KEY_Home:
    case osgGA::GUIEventAdapter::KEY_KP_Home:
        return hid::Assignment::KeyHome;
        break;

    case osgGA::GUIEventAdapter::KEY_End:
    case osgGA::GUIEventAdapter::KEY_KP_End:
        return hid::Assignment::KeyEnd;
        break;

    case osgGA::GUIEventAdapter::KEY_Page_Up:
    case osgGA::GUIEventAdapter::KEY_KP_Page_Up:
        return hid::Assignment::KeyPageUp;
        break;

    case osgGA::GUIEventAdapter::KEY_Page_Down:
    case osgGA::GUIEventAdapter::KEY_KP_Page_Down:
        return hid::Assignment::KeyPageDown;
        break;
    }

    return -1;
}

////////////////////////////////////////////////////////////////////////////////

bool WidgetCGI::KeyHandler::handleKeyDn( const osgGA::GUIEventAdapter &ea )
{
    int index = getKeyIndex( ea.getKey() );

    if ( index >= 0 )
    {
        _keysState[ index ] = true;
        return true;
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool WidgetCGI::KeyHandler::handleKeyUp( const osgGA::GUIEventAdapter &ea )
{
    int index = getKeyIndex( ea.getKey() );

    if ( index >= 0 )
    {
        _keysState[ index ] = false;
        return true;
    }

    return false;
}
