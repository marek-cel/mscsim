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
#ifndef KEYHANDLER_H
#define KEYHANDLER_H

////////////////////////////////////////////////////////////////////////////////

#include <osgGA/GUIEventHandler>

#include <hid/hid_Assignment.h>

////////////////////////////////////////////////////////////////////////////////

class WidgetCGI;

////////////////////////////////////////////////////////////////////////////////

/** */
class KeyHandler : public osgGA::GUIEventHandler
{
public:

    /** */
    KeyHandler( WidgetCGI *widgetCGI );

    /** */
    bool handle( const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter & );

    /** */
    void update();

    void keyDn( hid::Assignment::Key key );
    void keyUp( hid::Assignment::Key key );

private:

    WidgetCGI *_widgetCGI;

    bool _keysState[ HID_MAX_KEYS ];

    bool handleKeyDn( const osgGA::GUIEventAdapter &ea );
    bool handleKeyUp( const osgGA::GUIEventAdapter &ea );
};

////////////////////////////////////////////////////////////////////////////////

#endif // KEYHANDLER_H
