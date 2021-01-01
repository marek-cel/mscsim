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
#ifndef SFX_SAMPLE_H
#define SFX_SAMPLE_H

////////////////////////////////////////////////////////////////////////////////

#include <string>

#include <AL/al.h>
#include <AL/alc.h>

////////////////////////////////////////////////////////////////////////////////

namespace sfx
{

/** */
class Sample
{
public:

    /** @brief Constructor. */
    Sample( const char *file, bool looping = false );

    /** @brief Destructor. */
    virtual ~Sample();

    void play();

    void stop();

    void setLooping( bool looping );
    void setPitch( double pitch );
    void setVolume( double vol );

private:

    ALuint _source;         ///<
    ALuint _buffer;         ///<

    bool checkForErrors();

    bool loadFile( const char *file );
};

} // end of sfx namepsace

////////////////////////////////////////////////////////////////////////////////

#endif // SFX_SAMPLE_H
