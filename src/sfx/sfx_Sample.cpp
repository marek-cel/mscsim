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

#include <sfx/sfx_Sample.h>

#ifdef _MSC_VER
#   include <algorithm>
#endif

#include <cstring>

#include <AL/alut.h>

#include <defs.h>

#include <sim/Log.h>

////////////////////////////////////////////////////////////////////////////////

using namespace sfx;

////////////////////////////////////////////////////////////////////////////////

Sample::Sample( const char *file, bool looping )
{
    bool error = false;

    // source generation
    if ( !error )
    {
        alGenSources( (ALuint)1, &_source );
        error = checkForErrors();
    }

    if ( !error )
    {
        alSourcef( _source, AL_PITCH, 1);
        error = checkForErrors();
    }

    if ( !error )
    {
        alSourcef( _source, AL_GAIN, 1);
        error = checkForErrors();
    }

    if ( !error )
    {
        alSource3f( _source, AL_POSITION, 0, 0, 0);
        error = checkForErrors();
    }

    if ( !error )
    {
        alSource3f( _source, AL_VELOCITY, 0, 0, 0);
        error = checkForErrors();
    }

    if ( !error )
    {
        alSourcei( _source, AL_LOOPING, AL_FALSE );
        error = checkForErrors();
    }

    // buffer generation
    if ( !error )
    {
        alGenBuffers( (ALuint)1, &_buffer );
        error = checkForErrors();
    }

    if ( !error )
    {
        error = loadFile( file );
    }

    // binding source to buffer
    if ( !error )
    {
        alSourcei( _source, AL_BUFFER, _buffer );
        error = checkForErrors();
    }

    setLooping( looping );
}

////////////////////////////////////////////////////////////////////////////////

Sample::~Sample()
{
    alDeleteSources( 1, &_source );
    alDeleteBuffers( 1, &_buffer );
}

////////////////////////////////////////////////////////////////////////////////

void Sample::play()
{
    ALint state;

    alGetSourcei( _source, AL_SOURCE_STATE, &state );

    if ( state != AL_PLAYING )
    {
        alSourcePlay( _source );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Sample::stop()
{
    alSourceStop( _source );
}

////////////////////////////////////////////////////////////////////////////////

void Sample::setLooping( bool looping )
{
    alSourcei( _source, AL_LOOPING, ( looping ? AL_TRUE : AL_FALSE ) );
}

////////////////////////////////////////////////////////////////////////////////

void Sample::setPitch( double pitch )
{
    alSourcef( _source, AL_PITCH,
               static_cast< ALfloat >( std::max( 0.0, std::min( 1.0, pitch ) ) )
             );
}

////////////////////////////////////////////////////////////////////////////////

void Sample::setVolume( double vol )
{
    alSourcef( _source, AL_GAIN,
               static_cast< ALfloat >( std::max( 0.0, std::min( 1.0, vol ) ) )
             );
}


////////////////////////////////////////////////////////////////////////////////

bool Sample::checkForErrors()
{
    ALCenum error = alGetError();

    if ( error != AL_NO_ERROR )
    {
        std::string error_str = "";

        switch ( error )
        {
        case AL_INVALID_NAME:
            error_str = "AL_INVALID_NAME";
            break;

        case AL_INVALID_ENUM:
            error_str = "AL_INVALID_ENUM";
            break;

        case AL_INVALID_VALUE:
            error_str = "AL_INVALID_VALUE";
            break;

        case AL_INVALID_OPERATION:
            error_str = "AL_INVALID_OPERATION";
            break;

        case AL_OUT_OF_MEMORY:
            error_str = "AL_OUT_OF_MEMORY";
            break;

        default:
            error_str = "Unknown Error";
            break;
        }

        Log::e() << "SFX sample error: " << error_str << std::endl;

        return true;
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Sample::loadFile( const char *file )
{
    bool error = false;

    // loading audio stream to buffer
    ALenum format = 0;
    ALsizei size = 0;
    ALsizei freq = 0;

    ALvoid *data = NULLPTR;
    ALboolean loop = AL_FALSE;

    const size_t size_max = 4096;
    ALbyte file_temp[ size_max ];
    size_t size_str = ( strlen( file ) < size_max ) ? ( strlen( file ) + 1 ) : size_max;
    memcpy( file_temp, file, size_str );

    alutLoadWAVFile( file_temp, &format, &data, &size, &freq, &loop );

    error = checkForErrors();

    // loading the raw audio stream into our buffer:
    if ( !error )
    {
        alBufferData( _buffer, format, data, size, freq );
        error = checkForErrors();
    }

    return error;
}
