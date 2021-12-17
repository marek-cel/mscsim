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

#include <fdm/fdm_Recorder.h>

#include <fdm/fdm_Exception.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Recorder::Recorder( double desiredTimeStep ) :
    _desiredTimeStep ( desiredTimeStep ),

    _mode ( DataInp::Recording::Disabled ),

    _time      ( 0.0 ),
    _time_next ( 0.0 ),
    _time_prev ( 0.0 ),
    _time_rec  ( 0.0 ),

    _records ( 0 ),

    _recording ( false ),
    _replaying ( false )
{}

////////////////////////////////////////////////////////////////////////////////

Recorder::~Recorder()
{
    if ( _fstream.is_open() )
    {
        _fstream.flush();
        _fstream.close();
    }

    Variables::iterator it = _variables.begin();
    while ( it != _variables.end() )
    {
        FDM_DELPTR( (*it) );
        it = _variables.erase( it );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Recorder::addVariable( VariableBase *var )
{
    _variables.push_back( var );
}

////////////////////////////////////////////////////////////////////////////////

void Recorder::initialize( Mode mode, const char *file )
{
    _mode = mode;

    if ( _mode != DataInp::Recording::Disabled )
    {
        _fstream.open( file, getOpenMode( _mode ) );

        if ( _fstream.is_open() )
        {
            _recording = _mode == DataInp::Recording::Record;
            _replaying = _mode == DataInp::Recording::Replay;

            if ( _recording )
            {
                headerWrite();

                _fstream.setf( std::ios_base::showpoint );
                _fstream.setf( std::ios_base::fixed );
            }
            else
            {
                headerRead();
                recordRead( _time_next );

                for ( Variables::iterator it = _variables.begin(); it != _variables.end(); ++it )
                {
                    (*it)->initialize();
                }
            }
        }
        else
        {
            Exception e;

            e.setType( Exception::FileReadingError );
            e.setInfo( "Cannot open recording file file \"" + std::string( file ) + "\"." );

            FDM_THROW( e );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Recorder::step( double timeStep )
{
    if ( _fstream.is_open() )
    {
        switch ( _mode )
        {
        default:
        case DataInp::Recording::Disabled:
            // do nothing
            break;

        case DataInp::Recording::Record:
            stepRecord();
            _time_rec += timeStep;
            break;

        case DataInp::Recording::Replay:
            stepReplay();
            break;
        }
    }

    _time += timeStep;
}

////////////////////////////////////////////////////////////////////////////////

std::ios_base::openmode Recorder::getOpenMode( Mode mode )
{
    if ( mode == DataInp::Recording::Record )
    {
        return std::ios_base::out;
    }

    return std::ios_base::in;
}

////////////////////////////////////////////////////////////////////////////////

void Recorder::headerRead()
{
    std::string header;
    getline( _fstream, header );
}

////////////////////////////////////////////////////////////////////////////////

void Recorder::headerWrite()
{
    _fstream << "\"time\"";

    for ( Variables::iterator it = _variables.begin(); it != _variables.end(); ++it )
    {
        _fstream << ";\"" << (*it)->name() << "\"";
    }

    _fstream << "\n";
}

////////////////////////////////////////////////////////////////////////////////

void Recorder::recordRead( double &time )
{
    _fstream >> time;

    char separator;

    for ( Variables::iterator it = _variables.begin(); it != _variables.end(); ++it )
    {
        _fstream >> separator;
        (*it)->read( _fstream );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Recorder::recordWrite( double time )
{
    _fstream << std::setprecision( 4 );
    _fstream << time;

    for ( Variables::iterator it = _variables.begin(); it != _variables.end(); ++it )
    {
        _fstream << ";";
        (*it)->write( _fstream );
    }

    _fstream << "\n";
    _records++;
}

////////////////////////////////////////////////////////////////////////////////

void Recorder::stepRecord()
{
    if ( _time_rec > _desiredTimeStep || _records == 0 )
    {
        if ( _records != 0 ) _time_rec -= _desiredTimeStep;

        recordWrite( _time );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Recorder::stepReplay()
{
    while ( _replaying && _time_next <= _time )
    {
        _time_prev = _time_next;

        if ( !_fstream.eof() )
        {
            recordRead( _time_next );
        }
        else
        {
            _replaying = false;
        }
    }

    // interpolating
    if ( _replaying )
    {
        double t_coef = ( _time - _time_prev ) / ( _time_next - _time_prev );

        for ( Variables::iterator it = _variables.begin(); it != _variables.end(); ++it )
        {
            (*it)->interpolate( t_coef );
        }
    }
}
