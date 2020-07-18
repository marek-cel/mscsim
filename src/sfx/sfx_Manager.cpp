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

#include <sfx/sfx_Manager.h>

#include <Common.h>

#include <fdm/utils/fdm_Misc.h>

////////////////////////////////////////////////////////////////////////////////

using namespace sfx;

////////////////////////////////////////////////////////////////////////////////

Manager::Manager() :
    _device  ( NULLPTR ),
    _context ( NULLPTR ),

    _explosion ( NULLPTR ),
    _touchdown ( NULLPTR ),

    _warning   ( NULLPTR ),

    _run_noise ( NULLPTR ),

    _gear_lock ( NULLPTR ),
    _gear_move ( NULLPTR ),

    _prop_c130  ( NULLPTR ),
    _prop_c172  ( NULLPTR ),
    _prop_p51   ( NULLPTR ),
    _prop_f16   ( NULLPTR ),
    _prop_f16ab ( NULLPTR ),
    _prop_uh60  ( NULLPTR ),

    _volume ( 1.0 ),

    _afterburner ( 0.0 ),

    _prev_state ( fdm::DataInp::Idle ),

    _prev_gear ( 0.0 ),

    _prev_ground ( false ),
    _prev_crash  ( false )
{
    _device = alcOpenDevice( NULLPTR );

    if ( _device )
    {
        //ALint attribs[] = { ALC_FREQUENCY, 44000, 0 };
        //_context = alcCreateContext( _device, attribs );
        _context = alcCreateContext( _device, NULL );

        if ( !alcMakeContextCurrent( _context ) )
        {
            Log::e() << "Cannot create context." << std::endl;
        }
    }
    else
    {
        Log::e() << "Cannot open audio device." << std::endl;
    }

    _explosion = new Sample( Path::get( "data/sfx/explosion.wav" ).c_str() );
    _touchdown = new Sample( Path::get( "data/sfx/touchdown.wav" ).c_str() );

    _warning = new Sample( Path::get( "data/sfx/warning.wav" ).c_str(), true );

    _run_noise = new Sample( Path::get( "data/sfx/run_noise.wav" ).c_str(), true );

    _gear_lock = new Sample( Path::get( "data/sfx/gear_lock.wav" ).c_str() );
    _gear_move = new Sample( Path::get( "data/sfx/gear_move.wav" ).c_str(), true );

    _prop_c130  = new Sample( Path::get( "data/sfx/prop_c130.wav"  ).c_str(), true );
    _prop_c172  = new Sample( Path::get( "data/sfx/prop_c172.wav"  ).c_str(), true );
    _prop_p51   = new Sample( Path::get( "data/sfx/prop_p51.wav"   ).c_str(), true );
    _prop_f16   = new Sample( Path::get( "data/sfx/prop_f16.wav"   ).c_str(), true );
    _prop_f16ab = new Sample( Path::get( "data/sfx/prop_f16ab.wav" ).c_str(), true );
    _prop_uh60  = new Sample( Path::get( "data/sfx/prop_uh60.wav"  ).c_str(), true );
}

////////////////////////////////////////////////////////////////////////////////

Manager::~Manager()
{
    DELPTR( _explosion );
    DELPTR( _touchdown );

    DELPTR( _warning );

    DELPTR( _run_noise );

    DELPTR( _gear_lock );
    DELPTR( _gear_move );

    DELPTR( _prop_c130  );
    DELPTR( _prop_c172  );
    DELPTR( _prop_p51   );
    DELPTR( _prop_f16   );
    DELPTR( _prop_f16ab );
    DELPTR( _prop_uh60  );

    alcMakeContextCurrent( NULL );
    alcDestroyContext( _context );
    alcCloseDevice( _device );
}

////////////////////////////////////////////////////////////////////////////////

void Manager::update( const Data::DataBuf *data )
{
    _volume = data->sfx.volume;

    if ( data->stateInp == fdm::DataInp::Work
      || data->stateInp == fdm::DataInp::Freeze )
    {
        // init
        if ( _prev_state != data->stateInp )
        {
            init( data );
        }

        switch ( data->aircraftType )
        {
            case fdm::DataInp::C130: updateAircraft_C130 ( data ); break;
            case fdm::DataInp::C172: updateAircraft_C172 ( data ); break;
            case fdm::DataInp::F16:  updateAircraft_F16  ( data );  break;
            case fdm::DataInp::P51:  updateAircraft_P51  ( data );  break;
            case fdm::DataInp::UH60: updateAircraft_UH60 ( data ); break;
            default: break;
        }
    }
    else
    {
        stop();
    }

    if ( data->ownship.crash && !_prev_crash )
    {
        _explosion->play();
        _explosion->setVolume( _volume );
    }


    _prev_state  = data->stateInp;
    _prev_gear   = data->ownship.norm_landingGear;
    _prev_ground = data->ownship.onGround;
    _prev_crash  = data->ownship.crash;
}

////////////////////////////////////////////////////////////////////////////////

void Manager::init( const Data::DataBuf *data )
{
    _afterburner = 0.0;

    _prev_gear   = data->ownship.norm_landingGear;
    _prev_ground = data->ownship.onGround;
    _prev_crash  = false;
}

////////////////////////////////////////////////////////////////////////////////

void Manager::stop()
{
    _warning->stop();

    _run_noise->stop();

    _gear_move->stop();

    _prop_c130  ->stop();
    _prop_c172  ->stop();
    _prop_p51   ->stop();
    _prop_f16   ->stop();
    _prop_f16ab ->stop();
    _prop_uh60  ->stop();
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updateLandingGear( const Data::DataBuf *data )
{
    if ( _prev_gear != data->ownship.norm_landingGear )
    {
        if ( data->ownship.norm_landingGear < 0.001
          || data->ownship.norm_landingGear > 0.999 )
        {
            _gear_lock->play();
            _gear_lock->setVolume( _volume );
        }

        //_gear_move->play();
        //_gear_move->setVolume( _volume );
    }
    else
    {
        _gear_move->stop();
    }
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updateTouchdown(  const Data::DataBuf *data , double vel_max )
{
    // touchdown
    if ( data->ownship.onGround && !_prev_ground )
    {
        _touchdown->play();
        _touchdown->setVolume( _volume );
    }

    // run noise
    if ( data->ownship.onGround )
    {
        double vel =  data->ownship.groundSpeed;
        double coef = fdm::Misc::satur( 0.0, 1.0, vel / vel_max );
        _run_noise->play();
        _run_noise->setVolume( _volume * coef );
        _run_noise->setPitch( coef );
    }
    else
    {
        _run_noise->stop();
    }
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updateWarning( const Data::DataBuf *data )
{
    if ( data->ownship.stall )
    {
        _warning->play();
        _warning->setVolume( _volume );
    }
    else
    {
        _warning->stop();
    }
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updateAircraft_C130( const Data::DataBuf *data )
{
    updateTouchdown( data, 30.0 );
    updateLandingGear( data );
    updateWarning( data );

    double rpm = data->propulsion.engine[ 0 ].rpm;
    double coef = fdm::Misc::satur( 0.0, 1.0, rpm / 2000.0 );

    _prop_c130->play();
    _prop_c130->setVolume( _volume * coef );
    _prop_c130->setPitch( coef );
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updateAircraft_C172( const Data::DataBuf *data )
{
    updateTouchdown( data, 30.0 );
    updateWarning( data );

    double rpm = data->propulsion.engine[ 0 ].rpm;
    double coef = fdm::Misc::satur( 0.0, 1.0, rpm / 2000.0 );

    _prop_c172->play();
    _prop_c172->setVolume( _volume * coef );
    _prop_c172->setPitch( coef );
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updateAircraft_F16( const Data::DataBuf *data )
{
    updateTouchdown( data, 30.0 );
    updateLandingGear( data );
    updateWarning( data );

    bool afterburner = data->propulsion.engine[ 0 ].afterburner;
    double n2 = data->propulsion.engine[ 0 ].n2;
    double coef = fdm::Misc::satur( 0.0, 1.0, n2 / 95.0 );

    _prop_f16->play();
    _prop_f16->setVolume( _volume * coef );
    _prop_f16->setPitch( coef );

    _afterburner = fdm::Misc::inertia( afterburner ? 1.0 : 0.0, _afterburner, 0.01, 0.5 );

    _prop_f16ab->play();
    _prop_f16ab->setVolume( _volume * _afterburner );
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updateAircraft_P51( const Data::DataBuf *data )
{
    updateTouchdown( data, 30.0 );
    updateLandingGear( data );
    updateWarning( data );

    double rpm = data->propulsion.engine[ 0 ].rpm;
    double coef = fdm::Misc::satur( 0.0, 1.0, rpm / 4000.0 );

    _prop_p51->play();
    _prop_p51->setVolume( _volume * coef );
    _prop_p51->setPitch( coef );
}

////////////////////////////////////////////////////////////////////////////////

void Manager::updateAircraft_UH60( const Data::DataBuf *data )
{
    updateTouchdown( data, 30.0 );
    updateWarning( data );

    if ( data->ownship.mainRotor.omega > 0.0 )
    {
        _prop_uh60->play();
    }
    else
    {
        _prop_uh60->stop();
    }

    _prop_uh60->setVolume( _volume );
}
