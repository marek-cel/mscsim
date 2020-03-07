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
#ifndef SFX_MANAGER_H
#define SFX_MANAGER_H

////////////////////////////////////////////////////////////////////////////////

#include <AL/al.h>
#include <AL/alc.h>

#include <Data.h>
#include <Defines.h>

#include <sfx/sfx_Sample.h>

////////////////////////////////////////////////////////////////////////////////

namespace sfx
{

/**
 * @brief SFX manager class.
 */
class Manager
{
public:

    /** Constructor. */
    Manager();

    /** Destructor. */
    virtual ~Manager();

    /** Updates SFX. */
    void update( const Data::DataBuf *data );

private:

    ALCdevice  *_device;        ///<
    ALCcontext *_context;       ///<

    Sample *_explosion;         ///<
    Sample *_touchdown;         ///<

    Sample *_warning;           ///<

    Sample *_run_noise;         ///<

    Sample *_gear_lock;         ///<
    Sample *_gear_move;         ///<

    Sample *_prop_c130;         ///<
    Sample *_prop_c172;         ///<
    Sample *_prop_p51;          ///<
    Sample *_prop_f16;          ///<
    Sample *_prop_f16ab;        ///<
    Sample *_prop_uh60;         ///<

    double _volume;             ///<

    double _afterburner;        ///<

    int _prev_state;            ///<

    double _prev_gear;          ///<

    bool _prev_ground;          ///<
    bool _prev_crash;           ///<

    void init( const Data::DataBuf *data );
    void stop();

    void updateLandingGear( const Data::DataBuf *data );
    void updateTouchdown( const Data::DataBuf *data, double vel_max );
    void updateWarning( const Data::DataBuf *data );

    void updateAircraft_C130 ( const Data::DataBuf *data );
    void updateAircraft_C172 ( const Data::DataBuf *data );
    void updateAircraft_F16  ( const Data::DataBuf *data );
    void updateAircraft_P51  ( const Data::DataBuf *data );
    void updateAircraft_UH60 ( const Data::DataBuf *data );
};

} // end of sfx namepsace

////////////////////////////////////////////////////////////////////////////////

#endif // SFX_MANAGER_H
