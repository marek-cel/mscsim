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

#include <gui/ScreenSaver.h>

#ifdef _LINUX_
//#   include <QX11Info>
//#   include <X11/extensions/scrnsaver.h>
#endif

////////////////////////////////////////////////////////////////////////////////

void ScreenSaver::disable()
{
#   ifdef _MSC_VER
//    SystemParametersInfo(SPI_SETSCREENSAVEACTIVE,FALSE,NULL,TRUE);
#   endif

#   ifdef _LINUX_
//    ScreenSaverSuspend( QX11Info::display(), True );
//    system( "xset s off" );
//    system( "xset -dpms" );
#   endif
}

////////////////////////////////////////////////////////////////////////////////

void ScreenSaver::enable()
{
#   ifdef _LINUX_
//    XScreenSaverSuspend( QX11Info::display(), False );
#   endif
}
