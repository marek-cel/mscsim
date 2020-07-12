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

#include <gui/KeyMap.h>

#include <qnamespace.h>

////////////////////////////////////////////////////////////////////////////////

hid::Assignment::Key KeyMap::remapHID( int key_qt )
{
    switch ( key_qt )
    {
    case Qt::Key_0:
        return hid::Assignment::Key0;

    case Qt::Key_1:
        return hid::Assignment::Key1;

    case Qt::Key_2:
        return hid::Assignment::Key2;

    case Qt::Key_3:
        return hid::Assignment::Key3;

    case Qt::Key_4:
        return hid::Assignment::Key4;

    case Qt::Key_5:
        return hid::Assignment::Key5;

    case Qt::Key_6:
        return hid::Assignment::Key6;

    case Qt::Key_7:
        return hid::Assignment::Key7;

    case Qt::Key_8:
        return hid::Assignment::Key8;

    case Qt::Key_9:
        return hid::Assignment::Key9;

    case Qt::Key_A:
        return hid::Assignment::KeyA;

    case Qt::Key_B:
        return hid::Assignment::KeyB;

    case Qt::Key_C:
        return hid::Assignment::KeyC;

    case Qt::Key_D:
        return hid::Assignment::KeyD;

    case Qt::Key_E:
        return hid::Assignment::KeyE;

    case Qt::Key_F:
        return hid::Assignment::KeyF;

    case Qt::Key_G:
        return hid::Assignment::KeyG;

    case Qt::Key_H:
        return hid::Assignment::KeyH;

    case Qt::Key_I:
        return hid::Assignment::KeyI;

    case Qt::Key_J:
        return hid::Assignment::KeyJ;

    case Qt::Key_K:
        return hid::Assignment::KeyK;

    case Qt::Key_L:
        return hid::Assignment::KeyL;

    case Qt::Key_M:
        return hid::Assignment::KeyM;

    case Qt::Key_N:
        return hid::Assignment::KeyN;

    case Qt::Key_O:
        return hid::Assignment::KeyO;

    case Qt::Key_P:
        return hid::Assignment::KeyP;

    case Qt::Key_Q:
        return hid::Assignment::KeyQ;

    case Qt::Key_R:
        return hid::Assignment::KeyR;

    case Qt::Key_S:
        return hid::Assignment::KeyS;

    case Qt::Key_T:
        return hid::Assignment::KeyT;

    case Qt::Key_U:
        return hid::Assignment::KeyU;

    case Qt::Key_V:
        return hid::Assignment::KeyV;

    case Qt::Key_W:
        return hid::Assignment::KeyW;

    case Qt::Key_X:
        return hid::Assignment::KeyX;

    case Qt::Key_Y:
        return hid::Assignment::KeyY;

    case Qt::Key_Z:
        return hid::Assignment::KeyZ;

    case Qt::Key_QuoteLeft:
        return hid::Assignment::KeyBackquote;

    case Qt::Key_Minus:
        return hid::Assignment::KeyMinus;

    case Qt::Key_Equal:
        return hid::Assignment::KeyEquals;

    case Qt::Key_BraceLeft:
        return hid::Assignment::KeyLeftBracket;

    case Qt::Key_BraceRight:
        return hid::Assignment::KeyRightBracket;

    case Qt::Key_Semicolon:
        return hid::Assignment::KeySemicolon;

    case Qt::Key_Apostrophe:
        return hid::Assignment::KeyQuote;

    case Qt::Key_Comma:
        return hid::Assignment::KeyComma;

    case Qt::Key_Period:
        return hid::Assignment::KeyPeriod;

    case Qt::Key_Slash:
        return hid::Assignment::KeySlash;

    case Qt::Key_Space:
        return hid::Assignment::KeySpace;

    case Qt::Key_Left:
        return hid::Assignment::KeyLeft;

    case Qt::Key_Right:
        return hid::Assignment::KeyRight;

    case Qt::Key_Up:
        return hid::Assignment::KeyUp;

    case Qt::Key_Down:
        return hid::Assignment::KeyDown;

    case Qt::Key_Insert:
        return hid::Assignment::KeyInsert;

    case Qt::Key_Delete:
        return hid::Assignment::KeyDelete;

    case Qt::Key_Home:
        return hid::Assignment::KeyHome;

    case Qt::Key_End:
        return hid::Assignment::KeyEnd;

    case Qt::Key_PageUp:
        return hid::Assignment::KeyPageUp;

    case Qt::Key_PageDown:
        return hid::Assignment::KeyPageDown;

    case Qt::Key_F1:
        return hid::Assignment::KeyF1;

    case Qt::Key_F2:
        return hid::Assignment::KeyF2;

    case Qt::Key_F3:
        return hid::Assignment::KeyF3;

    case Qt::Key_F4:
        return hid::Assignment::KeyF4;

    case Qt::Key_F5:
        return hid::Assignment::KeyF5;

    case Qt::Key_F6:
        return hid::Assignment::KeyF6;

    case Qt::Key_F7:
        return hid::Assignment::KeyF7;

    case Qt::Key_F8:
        return hid::Assignment::KeyF8;

    case Qt::Key_F9:
        return hid::Assignment::KeyF9;

    case Qt::Key_F10:
        return hid::Assignment::KeyF10;

    case Qt::Key_F11:
        return hid::Assignment::KeyF11;

    case Qt::Key_F12:
        return hid::Assignment::KeyF12;
    }

    return (hid::Assignment::Key)(-1);
}

////////////////////////////////////////////////////////////////////////////////

osgGA::GUIEventAdapter::KeySymbol KeyMap::remapOSG( int key_qt )
{
    switch ( key_qt )
    {
    case Qt::Key_0:
        return osgGA::GUIEventAdapter::KEY_0;

    case Qt::Key_1:
        return osgGA::GUIEventAdapter::KEY_1;

    case Qt::Key_2:
        return osgGA::GUIEventAdapter::KEY_2;

    case Qt::Key_3:
        return osgGA::GUIEventAdapter::KEY_3;

    case Qt::Key_4:
        return osgGA::GUIEventAdapter::KEY_4;

    case Qt::Key_5:
        return osgGA::GUIEventAdapter::KEY_5;

    case Qt::Key_6:
        return osgGA::GUIEventAdapter::KEY_6;

    case Qt::Key_7:
        return osgGA::GUIEventAdapter::KEY_7;

    case Qt::Key_8:
        return osgGA::GUIEventAdapter::KEY_8;

    case Qt::Key_9:
        return osgGA::GUIEventAdapter::KEY_9;

    case Qt::Key_A:
        return osgGA::GUIEventAdapter::KEY_A;

    case Qt::Key_B:
        return osgGA::GUIEventAdapter::KEY_B;

    case Qt::Key_C:
        return osgGA::GUIEventAdapter::KEY_C;

    case Qt::Key_D:
        return osgGA::GUIEventAdapter::KEY_D;

    case Qt::Key_E:
        return osgGA::GUIEventAdapter::KEY_E;

    case Qt::Key_F:
        return osgGA::GUIEventAdapter::KEY_F;

    case Qt::Key_G:
        return osgGA::GUIEventAdapter::KEY_G;

    case Qt::Key_H:
        return osgGA::GUIEventAdapter::KEY_H;

    case Qt::Key_I:
        return osgGA::GUIEventAdapter::KEY_I;

    case Qt::Key_J:
        return osgGA::GUIEventAdapter::KEY_J;

    case Qt::Key_K:
        return osgGA::GUIEventAdapter::KEY_K;

    case Qt::Key_L:
        return osgGA::GUIEventAdapter::KEY_L;

    case Qt::Key_M:
        return osgGA::GUIEventAdapter::KEY_M;

    case Qt::Key_N:
        return osgGA::GUIEventAdapter::KEY_N;

    case Qt::Key_O:
        return osgGA::GUIEventAdapter::KEY_O;

    case Qt::Key_P:
        return osgGA::GUIEventAdapter::KEY_P;

    case Qt::Key_Q:
        return osgGA::GUIEventAdapter::KEY_Q;

    case Qt::Key_R:
        return osgGA::GUIEventAdapter::KEY_R;

    case Qt::Key_S:
        return osgGA::GUIEventAdapter::KEY_S;

    case Qt::Key_T:
        return osgGA::GUIEventAdapter::KEY_T;

    case Qt::Key_U:
        return osgGA::GUIEventAdapter::KEY_U;

    case Qt::Key_V:
        return osgGA::GUIEventAdapter::KEY_V;

    case Qt::Key_W:
        return osgGA::GUIEventAdapter::KEY_W;

    case Qt::Key_X:
        return osgGA::GUIEventAdapter::KEY_X;

    case Qt::Key_Y:
        return osgGA::GUIEventAdapter::KEY_Y;

    case Qt::Key_Z:
        return osgGA::GUIEventAdapter::KEY_Z;

    case Qt::Key_Escape:
        return osgGA::GUIEventAdapter::KEY_Escape;

    case Qt::Key_Tab:
        return osgGA::GUIEventAdapter::KEY_Tab;

    case Qt::Key_Backspace:
        return osgGA::GUIEventAdapter::KEY_BackSpace;

    case Qt::Key_Return:
        return osgGA::GUIEventAdapter::KEY_Return;

    case Qt::Key_Enter:
        return osgGA::GUIEventAdapter::KEY_KP_Enter;

    case Qt::Key_QuoteLeft:
        return osgGA::GUIEventAdapter::KEY_Backquote;

    case Qt::Key_Minus:
        return osgGA::GUIEventAdapter::KEY_Minus;

    case Qt::Key_Equal:
        return osgGA::GUIEventAdapter::KEY_Equals;

    case Qt::Key_BracketLeft:
        return osgGA::GUIEventAdapter::KEY_Leftbracket;

    case Qt::Key_BracketRight:
        return osgGA::GUIEventAdapter::KEY_Rightbracket;

    case Qt::Key_Semicolon:
        return osgGA::GUIEventAdapter::KEY_Semicolon;

    case Qt::Key_Apostrophe:
        return osgGA::GUIEventAdapter::KEY_Quote;

    case Qt::Key_Comma:
        return osgGA::GUIEventAdapter::KEY_Comma;

    case Qt::Key_Period:
        return osgGA::GUIEventAdapter::KEY_Period;

    case Qt::Key_Slash:
        return osgGA::GUIEventAdapter::KEY_Slash;

    case Qt::Key_Space:
        return osgGA::GUIEventAdapter::KEY_Space;

    case Qt::Key_Left:
        return osgGA::GUIEventAdapter::KEY_Left;

    case Qt::Key_Right:
        return osgGA::GUIEventAdapter::KEY_Right;

    case Qt::Key_Up:
        return osgGA::GUIEventAdapter::KEY_Up;

    case Qt::Key_Down:
        return osgGA::GUIEventAdapter::KEY_Down;

    case Qt::Key_Insert:
        return osgGA::GUIEventAdapter::KEY_Insert;

    case Qt::Key_Delete:
        return osgGA::GUIEventAdapter::KEY_Delete;

    case Qt::Key_Home:
        return osgGA::GUIEventAdapter::KEY_Home;

    case Qt::Key_End:
        return osgGA::GUIEventAdapter::KEY_End;

    case Qt::Key_PageUp:
        return osgGA::GUIEventAdapter::KEY_Page_Up;

    case Qt::Key_PageDown:
        return osgGA::GUIEventAdapter::KEY_Page_Down;

    case Qt::Key_Shift:
        return osgGA::GUIEventAdapter::KEY_Shift_L;

    case Qt::Key_Control:
        return osgGA::GUIEventAdapter::KEY_Control_L;

    case Qt::Key_Meta:
        return osgGA::GUIEventAdapter::KEY_Meta_L;

    case Qt::Key_Alt:
        return osgGA::GUIEventAdapter::KEY_Alt_L;

    case Qt::Key_F1:
        return osgGA::GUIEventAdapter::KEY_F1;

    case Qt::Key_F2:
        return osgGA::GUIEventAdapter::KEY_F2;

    case Qt::Key_F3:
        return osgGA::GUIEventAdapter::KEY_F3;

    case Qt::Key_F4:
        return osgGA::GUIEventAdapter::KEY_F4;

    case Qt::Key_F5:
        return osgGA::GUIEventAdapter::KEY_F5;

    case Qt::Key_F6:
        return osgGA::GUIEventAdapter::KEY_F6;

    case Qt::Key_F7:
        return osgGA::GUIEventAdapter::KEY_F7;

    case Qt::Key_F8:
        return osgGA::GUIEventAdapter::KEY_F8;

    case Qt::Key_F9:
        return osgGA::GUIEventAdapter::KEY_F9;

    case Qt::Key_F10:
        return osgGA::GUIEventAdapter::KEY_F10;

    case Qt::Key_F11:
        return osgGA::GUIEventAdapter::KEY_F11;

    case Qt::Key_F12:
        return osgGA::GUIEventAdapter::KEY_F12;
    }

    return (osgGA::GUIEventAdapter::KeySymbol)(-1);
}

