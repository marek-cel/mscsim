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

#include <gui/ScreenSaver.h>

#ifdef _MSC_VER
#   include <windows.h>
#endif

#ifdef _LINUX_
#   include <QX11Info>
#   include <X11/extensions/scrnsaver.h>
#   include <X11/Xlib.h>
#endif

////////////////////////////////////////////////////////////////////////////////

void ScreenSaver::disable()
{
#   ifdef _MSC_VER
    SetThreadExecutionState( ES_CONTINUOUS | ES_DISPLAY_REQUIRED | ES_SYSTEM_REQUIRED );
#   endif

#   ifdef _LINUX_
    XScreenSaverSuspend( QX11Info::display(), True );
#   endif
}

////////////////////////////////////////////////////////////////////////////////

void ScreenSaver::enable()
{
#   ifdef _MSC_VER
    SetThreadExecutionState( ES_CONTINUOUS );
#   endif

#   ifdef _LINUX_
    XScreenSaverSuspend( QX11Info::display(), False );
#   endif
}

////////////////////////////////////////////////////////////////////////////////

void ScreenSaver::reset()
{
#   ifdef _LINUX_
    XResetScreenSaver( QX11Info::display() );
#   endif
}
