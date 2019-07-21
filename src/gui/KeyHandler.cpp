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

#include <gui/KeyHandler.h>

#include <gui/WidgetCGI.h>

#include <hid/hid_Manager.h>

////////////////////////////////////////////////////////////////////////////////

KeyHandler::KeyHandler( WidgetCGI *widgetCGI ) :
    m_widgetCGI ( widgetCGI )
{
    for ( int i = 0; i < HID_MAX_KEYS; i++ ) m_keysState[ i ] = false;
}

////////////////////////////////////////////////////////////////////////////////

bool KeyHandler::handle( const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter & )
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

void KeyHandler::update()
{
    hid::Manager::instance()->setKeysState( m_keysState );
}

////////////////////////////////////////////////////////////////////////////////

void KeyHandler::keyDn( hid::Assignment::Key key )
{
    m_keysState[ key ] = true;
}

////////////////////////////////////////////////////////////////////////////////

void KeyHandler::keyUp( hid::Assignment::Key key )
{
    m_keysState[ key ] = false;
}

////////////////////////////////////////////////////////////////////////////////

bool KeyHandler::handleKeyDn( const osgGA::GUIEventAdapter &ea )
{
    switch ( ea.getKey() )
    {
    case osgGA::GUIEventAdapter::KEY_0:
    case osgGA::GUIEventAdapter::KEY_KP_0:
        m_keysState[ hid::Assignment::Key0 ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_1:
    case osgGA::GUIEventAdapter::KEY_KP_1:
        m_keysState[ hid::Assignment::Key1 ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_2:
    case osgGA::GUIEventAdapter::KEY_KP_2:
        m_keysState[ hid::Assignment::Key2 ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_3:
    case osgGA::GUIEventAdapter::KEY_KP_3:
        m_keysState[ hid::Assignment::Key3 ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_4:
    case osgGA::GUIEventAdapter::KEY_KP_4:
        m_keysState[ hid::Assignment::Key4 ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_5:
    case osgGA::GUIEventAdapter::KEY_KP_5:
        m_keysState[ hid::Assignment::Key5 ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_6:
    case osgGA::GUIEventAdapter::KEY_KP_6:
        m_keysState[ hid::Assignment::Key6 ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_7:
    case osgGA::GUIEventAdapter::KEY_KP_7:
        m_keysState[ hid::Assignment::Key7 ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_8:
    case osgGA::GUIEventAdapter::KEY_KP_8:
        m_keysState[ hid::Assignment::Key8 ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_9:
    case osgGA::GUIEventAdapter::KEY_KP_9:
        m_keysState[ hid::Assignment::Key9 ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_A:
    case 'A':
        m_keysState[ hid::Assignment::KeyA ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_B:
    case 'B':
        m_keysState[ hid::Assignment::KeyB ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_C:
    case 'C':
        m_keysState[ hid::Assignment::KeyC ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_D:
    case 'D':
        m_keysState[ hid::Assignment::KeyD ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_E:
    case 'E':
        m_keysState[ hid::Assignment::KeyE ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_F:
    case 'F':
        m_keysState[ hid::Assignment::KeyF ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_G:
    case 'G':
        m_keysState[ hid::Assignment::KeyG ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_H:
    case 'H':
        m_keysState[ hid::Assignment::KeyH ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_I:
    case 'I':
        m_keysState[ hid::Assignment::KeyI ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_J:
    case 'J':
        m_keysState[ hid::Assignment::KeyJ ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_K:
    case 'K':
        m_keysState[ hid::Assignment::KeyK ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_L:
    case 'L':
        m_keysState[ hid::Assignment::KeyL ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_M:
    case 'M':
        m_keysState[ hid::Assignment::KeyM ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_N:
    case 'N':
        m_keysState[ hid::Assignment::KeyN ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_O:
    case 'O':
        m_keysState[ hid::Assignment::KeyO ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_P:
    case 'P':
        m_keysState[ hid::Assignment::KeyP ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Q:
    case 'Q':
        m_keysState[ hid::Assignment::KeyQ ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_R:
    case 'R':
        m_keysState[ hid::Assignment::KeyR ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_S:
    case 'S':
        m_keysState[ hid::Assignment::KeyS ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_T:
    case 'T':
        m_keysState[ hid::Assignment::KeyT ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_U:
    case 'U':
        m_keysState[ hid::Assignment::KeyU ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_V:
    case 'V':
        m_keysState[ hid::Assignment::KeyV ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_W:
    case 'W':
        m_keysState[ hid::Assignment::KeyW ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_X:
    case 'X':
        m_keysState[ hid::Assignment::KeyX ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Y:
    case 'Y':
        m_keysState[ hid::Assignment::KeyY ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Z:
    case 'Z':
        m_keysState[ hid::Assignment::KeyZ ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Backquote:
        m_keysState[ hid::Assignment::KeyBackquote ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Minus:
        m_keysState[ hid::Assignment::KeyMinus ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Equals:
        m_keysState[ hid::Assignment::KeyEquals ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Leftbracket:
        m_keysState[ hid::Assignment::KeyLeftBracket ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Rightbracket:
        m_keysState[ hid::Assignment::KeyRightBracket ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Semicolon:
        m_keysState[ hid::Assignment::KeySemicolon ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Quote:
        m_keysState[ hid::Assignment::KeyQuote ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Comma:
        m_keysState[ hid::Assignment::KeyComma ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Period:
        m_keysState[ hid::Assignment::KeyPeriod ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Slash:
        m_keysState[ hid::Assignment::KeySlash ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Space:
        m_keysState[ hid::Assignment::KeySpace ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Left:
    case osgGA::GUIEventAdapter::KEY_KP_Left:
        m_keysState[ hid::Assignment::KeyLeft ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Right:
    case osgGA::GUIEventAdapter::KEY_KP_Right:
        m_keysState[ hid::Assignment::KeyRight ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Up:
    case osgGA::GUIEventAdapter::KEY_KP_Up:
        m_keysState[ hid::Assignment::KeyUp ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Down:
    case osgGA::GUIEventAdapter::KEY_KP_Down:
        m_keysState[ hid::Assignment::KeyDown ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Insert:
    case osgGA::GUIEventAdapter::KEY_KP_Insert:
        m_keysState[ hid::Assignment::KeyInsert ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Delete:
    case osgGA::GUIEventAdapter::KEY_KP_Delete:
        m_keysState[ hid::Assignment::KeyDelete ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Home:
    case osgGA::GUIEventAdapter::KEY_KP_Home:
        m_keysState[ hid::Assignment::KeyHome ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_End:
    case osgGA::GUIEventAdapter::KEY_KP_End:
        m_keysState[ hid::Assignment::KeyEnd ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Page_Up:
    case osgGA::GUIEventAdapter::KEY_KP_Page_Up:
        m_keysState[ hid::Assignment::KeyPageUp ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Page_Down:
    case osgGA::GUIEventAdapter::KEY_KP_Page_Down:
        m_keysState[ hid::Assignment::KeyPageDown ] = true;
        return true;
        break;
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool KeyHandler::handleKeyUp( const osgGA::GUIEventAdapter &ea )
{
    switch ( ea.getKey() )
    {
    case osgGA::GUIEventAdapter::KEY_0:
    case osgGA::GUIEventAdapter::KEY_KP_0:
        m_keysState[ hid::Assignment::Key0 ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_1:
    case osgGA::GUIEventAdapter::KEY_KP_1:
        m_keysState[ hid::Assignment::Key1 ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_2:
    case osgGA::GUIEventAdapter::KEY_KP_2:
        m_keysState[ hid::Assignment::Key2 ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_3:
    case osgGA::GUIEventAdapter::KEY_KP_3:
        m_keysState[ hid::Assignment::Key3 ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_4:
    case osgGA::GUIEventAdapter::KEY_KP_4:
        m_keysState[ hid::Assignment::Key4 ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_5:
    case osgGA::GUIEventAdapter::KEY_KP_5:
        m_keysState[ hid::Assignment::Key5 ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_6:
    case osgGA::GUIEventAdapter::KEY_KP_6:
        m_keysState[ hid::Assignment::Key6 ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_7:
    case osgGA::GUIEventAdapter::KEY_KP_7:
        m_keysState[ hid::Assignment::Key7 ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_8:
    case osgGA::GUIEventAdapter::KEY_KP_8:
        m_keysState[ hid::Assignment::Key8 ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_9:
    case osgGA::GUIEventAdapter::KEY_KP_9:
        m_keysState[ hid::Assignment::Key9 ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_A:
    case 'A':
        m_keysState[ hid::Assignment::KeyA ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_B:
    case 'B':
        m_keysState[ hid::Assignment::KeyB ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_C:
    case 'C':
        m_keysState[ hid::Assignment::KeyC ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_D:
    case 'D':
        m_keysState[ hid::Assignment::KeyD ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_E:
    case 'E':
        m_keysState[ hid::Assignment::KeyE ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_F:
    case 'F':
        m_keysState[ hid::Assignment::KeyF ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_G:
    case 'G':
        m_keysState[ hid::Assignment::KeyG ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_H:
    case 'H':
        m_keysState[ hid::Assignment::KeyH ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_I:
    case 'I':
        m_keysState[ hid::Assignment::KeyI ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_J:
    case 'J':
        m_keysState[ hid::Assignment::KeyJ ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_K:
    case 'K':
        m_keysState[ hid::Assignment::KeyK ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_L:
    case 'L':
        m_keysState[ hid::Assignment::KeyL ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_M:
    case 'M':
        m_keysState[ hid::Assignment::KeyM ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_N:
    case 'N':
        m_keysState[ hid::Assignment::KeyN ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_O:
    case 'O':
        m_keysState[ hid::Assignment::KeyO ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_P:
    case 'P':
        m_keysState[ hid::Assignment::KeyP ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Q:
    case 'Q':
        m_keysState[ hid::Assignment::KeyQ ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_R:
    case 'R':
        m_keysState[ hid::Assignment::KeyR ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_S:
    case 'S':
        m_keysState[ hid::Assignment::KeyS ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_T:
    case 'T':
        m_keysState[ hid::Assignment::KeyT ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_U:
    case 'U':
        m_keysState[ hid::Assignment::KeyU ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_V:
    case 'V':
        m_keysState[ hid::Assignment::KeyV ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_W:
    case 'W':
        m_keysState[ hid::Assignment::KeyW ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_X:
    case 'X':
        m_keysState[ hid::Assignment::KeyX ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Y:
    case 'Y':
        m_keysState[ hid::Assignment::KeyY ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Z:
    case 'Z':
        m_keysState[ hid::Assignment::KeyZ ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Backquote:
        m_keysState[ hid::Assignment::KeyBackquote ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Minus:
        m_keysState[ hid::Assignment::KeyMinus ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Equals:
        m_keysState[ hid::Assignment::KeyEquals ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Leftbracket:
        m_keysState[ hid::Assignment::KeyLeftBracket ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Rightbracket:
        m_keysState[ hid::Assignment::KeyRightBracket ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Semicolon:
        m_keysState[ hid::Assignment::KeySemicolon ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Quote:
        m_keysState[ hid::Assignment::KeyQuote ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Comma:
        m_keysState[ hid::Assignment::KeyComma ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Period:
        m_keysState[ hid::Assignment::KeyPeriod ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Slash:
        m_keysState[ hid::Assignment::KeySlash ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Space:
        m_keysState[ hid::Assignment::KeySpace ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Left:
    case osgGA::GUIEventAdapter::KEY_KP_Left:
        m_keysState[ hid::Assignment::KeyLeft ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Right:
    case osgGA::GUIEventAdapter::KEY_KP_Right:
        m_keysState[ hid::Assignment::KeyRight ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Up:
    case osgGA::GUIEventAdapter::KEY_KP_Up:
        m_keysState[ hid::Assignment::KeyUp ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Down:
    case osgGA::GUIEventAdapter::KEY_KP_Down:
        m_keysState[ hid::Assignment::KeyDown ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Insert:
    case osgGA::GUIEventAdapter::KEY_KP_Insert:
        m_keysState[ hid::Assignment::KeyInsert ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Delete:
    case osgGA::GUIEventAdapter::KEY_KP_Delete:
        m_keysState[ hid::Assignment::KeyDelete ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Home:
    case osgGA::GUIEventAdapter::KEY_KP_Home:
        m_keysState[ hid::Assignment::KeyHome ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_End:
    case osgGA::GUIEventAdapter::KEY_KP_End:
        m_keysState[ hid::Assignment::KeyEnd ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Page_Up:
    case osgGA::GUIEventAdapter::KEY_KP_Page_Up:
        m_keysState[ hid::Assignment::KeyPageUp ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Page_Down:
    case osgGA::GUIEventAdapter::KEY_KP_Page_Down:
        m_keysState[ hid::Assignment::KeyPageDown ] = false;
        return true;
        break;
    }

    return false;
}
