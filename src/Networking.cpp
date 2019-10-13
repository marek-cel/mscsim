/****************************************************************************//*
 * Copyright (C) 2019 Marek M. Cel
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

#ifdef SIM_NETWORKING

////////////////////////////////////////////////////////////////////////////////

#include <Networking.h>

#include <Common.h>
#include <Data.h>

////////////////////////////////////////////////////////////////////////////////

Networking::Networking() :
    _address ( "127.0.0.1" ),
    _port ( 5505 ),

    _socket ( NULLPTR )
{
    _socket = new QUdpSocket();
    _socket->connectToHost( _address, _port );

    memset( &_data_net, 0, sizeof(DataNet) );
}

////////////////////////////////////////////////////////////////////////////////

Networking::~Networking()
{
    if ( _socket )
    {
        _socket->abort();
        _socket->close();
        delete _socket;
    }
    _socket = NULLPTR;
}

////////////////////////////////////////////////////////////////////////////////

void Networking::update()
{
    _data_net.flightData.latitude      = Data::get()->ownship.latitude;
    _data_net.flightData.longitude     = Data::get()->ownship.longitude;
    _data_net.flightData.altitude_asl  = Data::get()->ownship.altitude_asl;
    _data_net.flightData.altitude_agl  = Data::get()->ownship.altitude_agl;
    _data_net.flightData.roll          = Data::get()->ownship.roll;
    _data_net.flightData.pitch         = Data::get()->ownship.pitch;
    _data_net.flightData.heading       = Data::get()->ownship.heading;
    _data_net.flightData.angleOfAttack = Data::get()->ownship.angleOfAttack;
    _data_net.flightData.sideslipAngle = Data::get()->ownship.sideslipAngle;
    _data_net.flightData.climbAngle    = Data::get()->ownship.climbAngle;
    _data_net.flightData.trackAngle    = Data::get()->ownship.trackAngle;
    _data_net.flightData.slipSkidAngle = Data::get()->ownship.slipSkidAngle;
    _data_net.flightData.airspeed      = Data::get()->ownship.airspeed;
    _data_net.flightData.machNumber    = Data::get()->ownship.machNumber;
    _data_net.flightData.climbRate     = Data::get()->ownship.climbRate;
    _data_net.flightData.rollRate      = Data::get()->ownship.rollRate;
    _data_net.flightData.pitchRate     = Data::get()->ownship.pitchRate;
    _data_net.flightData.yawRate       = Data::get()->ownship.yawRate;
    _data_net.flightData.turnRate      = Data::get()->ownship.turnRate;

    _data_net.navigation.course = Data::get()->navigation.course;

    //_data_net.navigation.course = Data::get()->navigation.

    if ( _socket->state() == QAbstractSocket::UnconnectedState )
    {
        _socket->connectToHost( _address, _port );
    }
    else
    {
        if ( _socket->state() == QAbstractSocket::ConnectedState )
        {
            _socket->writeDatagram( (char*)(&_data_net), (qint64)sizeof(DataNet), _address, _port );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

#endif
