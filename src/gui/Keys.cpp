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

#include <gui/Keys.h>

#include <qnamespace.h>

////////////////////////////////////////////////////////////////////////////////

hid::Assignment::Key Keys::getKey( int key )
{
    switch ( key )
    {
    case Qt::Key_0:
        return hid::Assignment::Key0;
        break;

    case Qt::Key_1:
        return hid::Assignment::Key1;
        break;

    case Qt::Key_2:
        return hid::Assignment::Key2;
        break;

    case Qt::Key_3:
        return hid::Assignment::Key3;
        break;

    case Qt::Key_4:
        return hid::Assignment::Key4;
        break;

    case Qt::Key_5:
        return hid::Assignment::Key5;
        break;

    case Qt::Key_6:
        return hid::Assignment::Key6;
        break;

    case Qt::Key_7:
        return hid::Assignment::Key7;
        break;

    case Qt::Key_8:
        return hid::Assignment::Key8;
        break;

    case Qt::Key_9:
        return hid::Assignment::Key9;
        break;

    case Qt::Key_A:
        return hid::Assignment::KeyA;
        break;

    case Qt::Key_B:
        return hid::Assignment::KeyB;
        break;

    case Qt::Key_C:
        return hid::Assignment::KeyC;
        break;

    case Qt::Key_D:
        return hid::Assignment::KeyD;
        break;

    case Qt::Key_E:
        return hid::Assignment::KeyE;
        break;

    case Qt::Key_F:
        return hid::Assignment::KeyF;
        break;

    case Qt::Key_G:
        return hid::Assignment::KeyG;
        break;

    case Qt::Key_H:
        return hid::Assignment::KeyH;
        break;

    case Qt::Key_I:
        return hid::Assignment::KeyI;
        break;

    case Qt::Key_J:
        return hid::Assignment::KeyJ;
        break;

    case Qt::Key_K:
        return hid::Assignment::KeyK;
        break;

    case Qt::Key_L:
        return hid::Assignment::KeyL;
        break;

    case Qt::Key_M:
        return hid::Assignment::KeyM;
        break;

    case Qt::Key_N:
        return hid::Assignment::KeyN;
        break;

    case Qt::Key_O:
        return hid::Assignment::KeyO;
        break;

    case Qt::Key_P:
        return hid::Assignment::KeyP;
        break;

    case Qt::Key_Q:
        return hid::Assignment::KeyQ;
        break;

    case Qt::Key_R:
        return hid::Assignment::KeyR;
        break;

    case Qt::Key_S:
        return hid::Assignment::KeyS;
        break;

    case Qt::Key_T:
        return hid::Assignment::KeyT;
        break;

    case Qt::Key_U:
        return hid::Assignment::KeyU;
        break;

    case Qt::Key_V:
        return hid::Assignment::KeyV;
        break;

    case Qt::Key_W:
        return hid::Assignment::KeyW;
        break;

    case Qt::Key_X:
        return hid::Assignment::KeyX;
        break;

    case Qt::Key_Y:
        return hid::Assignment::KeyY;
        break;

    case Qt::Key_Z:
        return hid::Assignment::KeyZ;
        break;

    case Qt::Key_QuoteLeft:
        return hid::Assignment::KeyBackquote;
        break;

    case Qt::Key_Minus:
        return hid::Assignment::KeyMinus;
        break;

    case Qt::Key_Equal:
        return hid::Assignment::KeyEquals;
        break;

    case Qt::Key_BraceLeft:
        return hid::Assignment::KeyLeftBracket;
        break;

    case Qt::Key_BraceRight:
        return hid::Assignment::KeyRightBracket;
        break;

    case Qt::Key_Semicolon:
        return hid::Assignment::KeySemicolon;
        break;

    case Qt::Key_Apostrophe:
        return hid::Assignment::KeyQuote;
        break;

    case Qt::Key_Comma:
        return hid::Assignment::KeyComma;
        break;

    case Qt::Key_Period:
        return hid::Assignment::KeyPeriod;
        break;

    case Qt::Key_Slash:
        return hid::Assignment::KeySlash;
        break;

    case Qt::Key_Space:
        return hid::Assignment::KeySpace;
        break;

    case Qt::Key_Left:
        return hid::Assignment::KeyLeft;
        break;

    case Qt::Key_Right:
        return hid::Assignment::KeyRight;
        break;

    case Qt::Key_Up:
        return hid::Assignment::KeyUp;
        break;

    case Qt::Key_Down:
        return hid::Assignment::KeyDown;
        break;

    case Qt::Key_Insert:
        return hid::Assignment::KeyInsert;
        break;

    case Qt::Key_Delete:
        return hid::Assignment::KeyDelete;
        break;

    case Qt::Key_Home:
        return hid::Assignment::KeyHome;
        break;

    case Qt::Key_End:
        return hid::Assignment::KeyEnd;
        break;

    case Qt::Key_PageUp:
        return hid::Assignment::KeyPageUp;
        break;

    case Qt::Key_PageDown:
        return hid::Assignment::KeyPageDown;
        break;
    }
}
