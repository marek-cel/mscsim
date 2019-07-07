#include <fdm_f16/f16_FLCS_Recorder.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

F16_FLCS_Recorder::F16_FLCS_Recorder() :
    m_file( "f16_flcs.csv" )
{
    for ( int i = 0; i < F16_FLCS_REC_SIZE; i++ )
    {
        m_x[ i ] = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////

F16_FLCS_Recorder::~F16_FLCS_Recorder() {}

////////////////////////////////////////////////////////////////////////////////

void F16_FLCS_Recorder::set( int index, double x )
{
    if ( index - 1 >= 0 && index - 1 < F16_FLCS_REC_SIZE )
    {
        m_x[ index - 1 ] = x;
    }
}

////////////////////////////////////////////////////////////////////////////////

void F16_FLCS_Recorder::write( double t )
{
    m_file << t;

    for ( int i = 0; i < F16_FLCS_REC_SIZE; i++ )
    {
        m_file << "," << m_x[ i ];
    }

    m_file << "\n";
}
