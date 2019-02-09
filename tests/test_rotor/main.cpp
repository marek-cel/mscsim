#include <QApplication>
#include "MainWindow.h"

////////////////////////////////////////////////////////////////////////////////

/** This is application main function. */
int main( int argc, char *argv[] )
{
    setlocale( LC_ALL, "C" );

#   ifdef _LINUX_
    setenv( "LC_NUMERIC", "en_US", 1 );
#   endif

    QApplication *app = new QApplication( argc, argv );
    MainWindow   *win = new MainWindow();

    win->show();

    int result = app->exec();

    delete win; win = 0;
    delete app; app = 0;

    return result;
}
