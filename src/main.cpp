/****************************************************************************//*
 * Copyright (C) 2021 Marek M. Cel
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

#include <fstream>
#include <iostream>

#include <QApplication>

#ifndef SIM_TEST
#   ifdef WIN32
#       include <QDir>
#   endif
#endif

#include <Manager.h>

#include <sim/Log.h>

////////////////////////////////////////////////////////////////////////////////

/** This is application main function. */
int main( int argc, char *argv[] )
{
    setlocale( LC_ALL, "C" );

#   ifndef SIM_TEST

#   ifdef _LINUX_
    std::string log_file = "/tmp/";
#   endif

#   ifdef WIN32
    std::string log_file = QDir::homePath().toLocal8Bit().data();
    log_file += "/";
#   endif

    log_file += "mscsim.log";

    // redirecting errors output
    std::streambuf *strbuf = std::cerr.rdbuf();
    std::fstream out( log_file.c_str(), std::ios_base::trunc | std::ios_base::out );
    std::cerr.rdbuf( out.rdbuf() );
#   endif

    Log::out() << SIM_APP_NAME << " ";
    Log::out() << SIM_APP_VER  << " ";
    Log::out() << __DATE__ << " ";
    Log::out() << __TIME__ << std::endl;

    QLocale::setDefault( QLocale::system() );

    QApplication *app = new QApplication( argc, argv );

    app->setApplicationName    ( SIM_APP_NAME   );
    app->setApplicationVersion ( SIM_APP_VER    );
    app->setOrganizationDomain ( SIM_ORG_DOMAIN );
    app->setOrganizationName   ( SIM_ORG_NAME   );

    Manager *mgr = new Manager();

    mgr->init();

    int result = app->exec();

    if ( mgr ) { delete mgr; } mgr = NULLPTR;
    if ( app ) { delete app; } app = NULLPTR;

#   ifndef SIM_TEST
    std::cerr.rdbuf( strbuf );
    if ( out.is_open() )
    {
        out.close();
    }
#   endif

    return result;
}
