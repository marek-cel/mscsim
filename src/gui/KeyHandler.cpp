/****************************************************************************//*
 * Copyright (C) 2020 Marek M. Cel
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

int KeyHandler::getIndex( int key )
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

bool KeyHandler::handleKeyDn( const osgGA::GUIEventAdapter &ea )
{
    int index = getIndex( ea.getKey() );

    if ( index >= 0 )
    {
        _keysState[ index ] = true;
        return true;
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool KeyHandler::handleKeyUp( const osgGA::GUIEventAdapter &ea )
{
    int index = getIndex( ea.getKey() );

    if ( index >= 0 )
    {
        _keysState[ index ] = false;
        return true;
    }

    return false;
}
