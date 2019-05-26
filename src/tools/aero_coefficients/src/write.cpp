#include "write.h"

////////////////////////////////////////////////////////////////////////////////

using namespace std;

////////////////////////////////////////////////////////////////////////////////

void write()
{
    int error = 0;

    if ( error == 0 ) error = write( dragInp, dragOut, drag );
    if ( error == 0 ) error = write( liftInp, liftOut, lift );

    if ( error == 0 ) cout << "Done." << endl << endl;
}
