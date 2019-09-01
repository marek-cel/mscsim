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
    _widgetCGI ( widgetCGI )
{
    for ( int i = 0; i < HID_MAX_KEYS; i++ ) _keysState[ i ] = false;
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
    hid::Manager::instance()->setKeysState( _keysState );
}

////////////////////////////////////////////////////////////////////////////////

void KeyHandler::keyDn( hid::Assignment::Key key )
{
    _keysState[ key ] = true;
}

////////////////////////////////////////////////////////////////////////////////

void KeyHandler::keyUp( hid::Assignment::Key key )
{
    _keysState[ key ] = false;
}

////////////////////////////////////////////////////////////////////////////////

bool KeyHandler::handleKeyDn( const osgGA::GUIEventAdapter &ea )
{
    switch ( ea.getKey() )
    {
    case osgGA::GUIEventAdapter::KEY_0:
    case osgGA::GUIEventAdapter::KEY_KP_0:
        _keysState[ hid::Assignment::Key0 ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_1:
    case osgGA::GUIEventAdapter::KEY_KP_1:
        _keysState[ hid::Assignment::Key1 ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_2:
    case osgGA::GUIEventAdapter::KEY_KP_2:
        _keysState[ hid::Assignment::Key2 ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_3:
    case osgGA::GUIEventAdapter::KEY_KP_3:
        _keysState[ hid::Assignment::Key3 ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_4:
    case osgGA::GUIEventAdapter::KEY_KP_4:
        _keysState[ hid::Assignment::Key4 ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_5:
    case osgGA::GUIEventAdapter::KEY_KP_5:
        _keysState[ hid::Assignment::Key5 ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_6:
    case osgGA::GUIEventAdapter::KEY_KP_6:
        _keysState[ hid::Assignment::Key6 ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_7:
    case osgGA::GUIEventAdapter::KEY_KP_7:
        _keysState[ hid::Assignment::Key7 ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_8:
    case osgGA::GUIEventAdapter::KEY_KP_8:
        _keysState[ hid::Assignment::Key8 ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_9:
    case osgGA::GUIEventAdapter::KEY_KP_9:
        _keysState[ hid::Assignment::Key9 ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_A:
    case 'A':
        _keysState[ hid::Assignment::KeyA ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_B:
    case 'B':
        _keysState[ hid::Assignment::KeyB ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_C:
    case 'C':
        _keysState[ hid::Assignment::KeyC ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_D:
    case 'D':
        _keysState[ hid::Assignment::KeyD ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_E:
    case 'E':
        _keysState[ hid::Assignment::KeyE ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_F:
    case 'F':
        _keysState[ hid::Assignment::KeyF ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_G:
    case 'G':
        _keysState[ hid::Assignment::KeyG ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_H:
    case 'H':
        _keysState[ hid::Assignment::KeyH ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_I:
    case 'I':
        _keysState[ hid::Assignment::KeyI ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_J:
    case 'J':
        _keysState[ hid::Assignment::KeyJ ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_K:
    case 'K':
        _keysState[ hid::Assignment::KeyK ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_L:
    case 'L':
        _keysState[ hid::Assignment::KeyL ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_M:
    case 'M':
        _keysState[ hid::Assignment::KeyM ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_N:
    case 'N':
        _keysState[ hid::Assignment::KeyN ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_O:
    case 'O':
        _keysState[ hid::Assignment::KeyO ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_P:
    case 'P':
        _keysState[ hid::Assignment::KeyP ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Q:
    case 'Q':
        _keysState[ hid::Assignment::KeyQ ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_R:
    case 'R':
        _keysState[ hid::Assignment::KeyR ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_S:
    case 'S':
        _keysState[ hid::Assignment::KeyS ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_T:
    case 'T':
        _keysState[ hid::Assignment::KeyT ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_U:
    case 'U':
        _keysState[ hid::Assignment::KeyU ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_V:
    case 'V':
        _keysState[ hid::Assignment::KeyV ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_W:
    case 'W':
        _keysState[ hid::Assignment::KeyW ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_X:
    case 'X':
        _keysState[ hid::Assignment::KeyX ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Y:
    case 'Y':
        _keysState[ hid::Assignment::KeyY ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Z:
    case 'Z':
        _keysState[ hid::Assignment::KeyZ ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Backquote:
        _keysState[ hid::Assignment::KeyBackquote ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Minus:
        _keysState[ hid::Assignment::KeyMinus ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Equals:
        _keysState[ hid::Assignment::KeyEquals ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Leftbracket:
        _keysState[ hid::Assignment::KeyLeftBracket ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Rightbracket:
        _keysState[ hid::Assignment::KeyRightBracket ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Semicolon:
        _keysState[ hid::Assignment::KeySemicolon ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Quote:
        _keysState[ hid::Assignment::KeyQuote ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Comma:
        _keysState[ hid::Assignment::KeyComma ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Period:
        _keysState[ hid::Assignment::KeyPeriod ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Slash:
        _keysState[ hid::Assignment::KeySlash ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Space:
        _keysState[ hid::Assignment::KeySpace ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Left:
    case osgGA::GUIEventAdapter::KEY_KP_Left:
        _keysState[ hid::Assignment::KeyLeft ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Right:
    case osgGA::GUIEventAdapter::KEY_KP_Right:
        _keysState[ hid::Assignment::KeyRight ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Up:
    case osgGA::GUIEventAdapter::KEY_KP_Up:
        _keysState[ hid::Assignment::KeyUp ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Down:
    case osgGA::GUIEventAdapter::KEY_KP_Down:
        _keysState[ hid::Assignment::KeyDown ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Insert:
    case osgGA::GUIEventAdapter::KEY_KP_Insert:
        _keysState[ hid::Assignment::KeyInsert ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Delete:
    case osgGA::GUIEventAdapter::KEY_KP_Delete:
        _keysState[ hid::Assignment::KeyDelete ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Home:
    case osgGA::GUIEventAdapter::KEY_KP_Home:
        _keysState[ hid::Assignment::KeyHome ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_End:
    case osgGA::GUIEventAdapter::KEY_KP_End:
        _keysState[ hid::Assignment::KeyEnd ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Page_Up:
    case osgGA::GUIEventAdapter::KEY_KP_Page_Up:
        _keysState[ hid::Assignment::KeyPageUp ] = true;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Page_Down:
    case osgGA::GUIEventAdapter::KEY_KP_Page_Down:
        _keysState[ hid::Assignment::KeyPageDown ] = true;
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
        _keysState[ hid::Assignment::Key0 ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_1:
    case osgGA::GUIEventAdapter::KEY_KP_1:
        _keysState[ hid::Assignment::Key1 ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_2:
    case osgGA::GUIEventAdapter::KEY_KP_2:
        _keysState[ hid::Assignment::Key2 ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_3:
    case osgGA::GUIEventAdapter::KEY_KP_3:
        _keysState[ hid::Assignment::Key3 ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_4:
    case osgGA::GUIEventAdapter::KEY_KP_4:
        _keysState[ hid::Assignment::Key4 ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_5:
    case osgGA::GUIEventAdapter::KEY_KP_5:
        _keysState[ hid::Assignment::Key5 ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_6:
    case osgGA::GUIEventAdapter::KEY_KP_6:
        _keysState[ hid::Assignment::Key6 ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_7:
    case osgGA::GUIEventAdapter::KEY_KP_7:
        _keysState[ hid::Assignment::Key7 ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_8:
    case osgGA::GUIEventAdapter::KEY_KP_8:
        _keysState[ hid::Assignment::Key8 ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_9:
    case osgGA::GUIEventAdapter::KEY_KP_9:
        _keysState[ hid::Assignment::Key9 ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_A:
    case 'A':
        _keysState[ hid::Assignment::KeyA ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_B:
    case 'B':
        _keysState[ hid::Assignment::KeyB ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_C:
    case 'C':
        _keysState[ hid::Assignment::KeyC ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_D:
    case 'D':
        _keysState[ hid::Assignment::KeyD ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_E:
    case 'E':
        _keysState[ hid::Assignment::KeyE ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_F:
    case 'F':
        _keysState[ hid::Assignment::KeyF ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_G:
    case 'G':
        _keysState[ hid::Assignment::KeyG ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_H:
    case 'H':
        _keysState[ hid::Assignment::KeyH ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_I:
    case 'I':
        _keysState[ hid::Assignment::KeyI ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_J:
    case 'J':
        _keysState[ hid::Assignment::KeyJ ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_K:
    case 'K':
        _keysState[ hid::Assignment::KeyK ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_L:
    case 'L':
        _keysState[ hid::Assignment::KeyL ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_M:
    case 'M':
        _keysState[ hid::Assignment::KeyM ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_N:
    case 'N':
        _keysState[ hid::Assignment::KeyN ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_O:
    case 'O':
        _keysState[ hid::Assignment::KeyO ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_P:
    case 'P':
        _keysState[ hid::Assignment::KeyP ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Q:
    case 'Q':
        _keysState[ hid::Assignment::KeyQ ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_R:
    case 'R':
        _keysState[ hid::Assignment::KeyR ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_S:
    case 'S':
        _keysState[ hid::Assignment::KeyS ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_T:
    case 'T':
        _keysState[ hid::Assignment::KeyT ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_U:
    case 'U':
        _keysState[ hid::Assignment::KeyU ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_V:
    case 'V':
        _keysState[ hid::Assignment::KeyV ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_W:
    case 'W':
        _keysState[ hid::Assignment::KeyW ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_X:
    case 'X':
        _keysState[ hid::Assignment::KeyX ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Y:
    case 'Y':
        _keysState[ hid::Assignment::KeyY ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Z:
    case 'Z':
        _keysState[ hid::Assignment::KeyZ ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Backquote:
        _keysState[ hid::Assignment::KeyBackquote ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Minus:
        _keysState[ hid::Assignment::KeyMinus ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Equals:
        _keysState[ hid::Assignment::KeyEquals ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Leftbracket:
        _keysState[ hid::Assignment::KeyLeftBracket ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Rightbracket:
        _keysState[ hid::Assignment::KeyRightBracket ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Semicolon:
        _keysState[ hid::Assignment::KeySemicolon ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Quote:
        _keysState[ hid::Assignment::KeyQuote ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Comma:
        _keysState[ hid::Assignment::KeyComma ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Period:
        _keysState[ hid::Assignment::KeyPeriod ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Slash:
        _keysState[ hid::Assignment::KeySlash ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Space:
        _keysState[ hid::Assignment::KeySpace ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Left:
    case osgGA::GUIEventAdapter::KEY_KP_Left:
        _keysState[ hid::Assignment::KeyLeft ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Right:
    case osgGA::GUIEventAdapter::KEY_KP_Right:
        _keysState[ hid::Assignment::KeyRight ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Up:
    case osgGA::GUIEventAdapter::KEY_KP_Up:
        _keysState[ hid::Assignment::KeyUp ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Down:
    case osgGA::GUIEventAdapter::KEY_KP_Down:
        _keysState[ hid::Assignment::KeyDown ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Insert:
    case osgGA::GUIEventAdapter::KEY_KP_Insert:
        _keysState[ hid::Assignment::KeyInsert ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Delete:
    case osgGA::GUIEventAdapter::KEY_KP_Delete:
        _keysState[ hid::Assignment::KeyDelete ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Home:
    case osgGA::GUIEventAdapter::KEY_KP_Home:
        _keysState[ hid::Assignment::KeyHome ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_End:
    case osgGA::GUIEventAdapter::KEY_KP_End:
        _keysState[ hid::Assignment::KeyEnd ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Page_Up:
    case osgGA::GUIEventAdapter::KEY_KP_Page_Up:
        _keysState[ hid::Assignment::KeyPageUp ] = false;
        return true;
        break;

    case osgGA::GUIEventAdapter::KEY_Page_Down:
    case osgGA::GUIEventAdapter::KEY_KP_Page_Down:
        _keysState[ hid::Assignment::KeyPageDown ] = false;
        return true;
        break;
    }

    return false;
}
