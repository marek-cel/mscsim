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

#include <sfx/sfx_Thread.h>

////////////////////////////////////////////////////////////////////////////////

using namespace sfx;

////////////////////////////////////////////////////////////////////////////////

Thread::Thread() :
    QThread ( NULLPTR ),
    _timer ( NULLPTR ),
    _sfx ( NULLPTR )
{
    memset( &_data, 0, sizeof(Data::DataBuf) );
    moveToThread( this );
}

////////////////////////////////////////////////////////////////////////////////

Thread::~Thread()
{
    DELPTR( _timer );
    DELPTR( _sfx );
}

////////////////////////////////////////////////////////////////////////////////

void Thread::init()
{
    start();
    setPriority( QThread::HighPriority );
}

////////////////////////////////////////////////////////////////////////////////

void Thread::run()
{
    _timer = new QTimer();

    connect( _timer, SIGNAL(timeout()), this, SLOT(update()) );

    _timer->start( 1000.0 * FDM_TIME_STEP );

    ///////////////
    QThread::run();
    ///////////////

    disconnect( _timer, SIGNAL(timeout()), 0, 0 );

    _timer->stop();
}

////////////////////////////////////////////////////////////////////////////////

void Thread::onDataInpUpdated( const Data::DataBuf *data )
{
    memcpy( &_data, data, sizeof(Data::DataBuf) );
}

////////////////////////////////////////////////////////////////////////////////

void Thread::update()
{
    if ( _sfx )
    {
        _sfx->update( &_data );
    }
    else
    {
        _sfx = new sfx::Manager();
    }
}
